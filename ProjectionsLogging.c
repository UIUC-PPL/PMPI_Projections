// (c) 2009 Isaac Dooley


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <execinfo.h>
#include <stdlib.h>

#include "source_location.h"
#include "ProjectionsLogging.h"

#define OUTBUFSIZE 1024*1024*4

#ifdef __cplusplus
#define EXTERN_C extern "C" 
#else
#define EXTERN_C 
#endif

#define  CREATION           1
#define  BEGIN_PROCESSING   2
#define  END_PROCESSING     3
#define  ENQUEUE            4
#define  DEQUEUE            5
#define  BEGIN_COMPUTATION  6
#define  END_COMPUTATION    7
#define  BEGIN_INTERRUPT    8
#define  END_INTERRUPT      9
#define  MESSAGE_RECV       10
#define  BEGIN_TRACE        11
#define  END_TRACE          12
#define  USER_EVENT         13
#define  BEGIN_IDLE         14
#define  END_IDLE           15
#define  BEGIN_PACK         16
#define  END_PACK           17
#define  BEGIN_UNPACK       18
#define  END_UNPACK         19
#define  CREATION_BCAST     20

#define  CREATION_MULTICAST 21
 
#define  BEGIN_FUNC         22
#define  END_FUNC           23

/* Memory tracing */
#define  MEMORY_MALLOC      24
#define  MEMORY_FREE        25

/* Trace user supplied data */
#define USER_SUPPLIED       26

/* Trace memory usage */
#define MEMORY_USAGE_CURRENT       27

/* Trace user supplied note (text string)  */
#define USER_SUPPLIED_NOTE       28

/* Trace user supplied note (text string, with start, end times, and user event id)  */
#define USER_SUPPLIED_BRACKETED_NOTE       29

/* Support for Phases and time-partial logs */
#define END_PHASE           30
#define SURROGATE_BLOCK     31 /* inserted by cluster analysis only */

#define  USER_EVENT_PAIR    100
#define ABT_Init_event 1
#define ABT_Finalize_event 1000000

int source_locations_count;

double initTime;

int np;

/* One element per rank */
static LogHandle *trace_handle_arr;

/* TODO: For real portability the timer function should be handled by
   a registered a function pointer, or something similar. This works
   for the proof of concept. */

EXTERN_C double ABT_get_wtime(void);

void init_time(){
  initTime = ABT_get_wtime() - 0.001;
}

void ABT_init_tracing(int num_ranks){
  /*  printf("tracing initialized for %d ranks\n",num_ranks);*/
  np=num_ranks;
  trace_handle_arr=  malloc(sizeof(*trace_handle_arr)*num_ranks);
  init_time();
  int i=0;
  for(i=0;i<num_ranks;++i)
    {
      ABT_init_trace(i);
    }
}



long time_us(double currtime){
	return ((ABT_get_wtime()-initTime)*1000000.0);
}


void writeSts(){
  FILE *stsfile=fopen("ProjPABT.sts","w");
  fprintf(stsfile,"PROJECTIONS_ID\n");
  fprintf(stsfile,"VERSION 11.0\n");
  fprintf(stsfile,"MACHINE \"PABT_Logging\"\n");
  fprintf(stsfile,"PROCESSORS %d\n", np);
  fprintf(stsfile,"TOTAL_CHARES %d\n", source_locations_count+1);
  fprintf(stsfile,"TOTAL_EPS %d\n",source_locations_count+1);
  fprintf(stsfile,"TOTAL_MSGS 2\n");
  fprintf(stsfile,"TOTAL_PSEUDOS 0\n");
  fprintf(stsfile,"TOTAL_EVENTS %d\n", NUM_EVENTS);

  /*TODO non set implementation of source_locations ?*/

  for(int iter = 0; iter<= source_locations_count; iter++){
    fprintf(stsfile,"CHARE %d \"tid_%d\" -1\n",iter,iter);
    }

  for(int iter = 0; iter<= source_locations_count; iter++){
      fprintf(stsfile,"ENTRY CHARE %d \"tid_%d\" %d %d\n",iter,iter,iter,0);
    }
  fprintf(stsfile, "MESSAGE 0 0\n");
  fprintf(stsfile,"MESSAGE 1 0\n");
  // Call the function from generated-stsEvents.C that fills in all the "EVENT" records
  /*  generateStsEvents(stsfile);*/
  fprintf(stsfile, "TOTAL_FUNCTIONS 0 \n");
  fprintf(stsfile,"END\n");
  fclose(stsfile);
}

void writeToDisk(int rank){
  fprintf(trace_handle_arr[rank].outfile, "PROJECTIONS-RECORD %ld\n", trace_handle_arr[rank].records_since_flush); // should only appear once in file???
  fprintf(trace_handle_arr[rank].outfile,"%s",trace_handle_arr[rank].out_buf);
  fflush(trace_handle_arr[rank].outfile);
  trace_handle_arr[rank].curr_buf_position = trace_handle_arr[rank].out_buf;
  trace_handle_arr[rank].out_buf[0] = '\0';
  trace_handle_arr[rank].records_since_flush = 0;
}

/// Flush buffer if full
void flush(int rank){
	if(trace_handle_arr[rank].curr_buf_position > trace_handle_arr[rank].flush_point){
		writeToDisk(rank);
	}
}

void write_log_header(int rank){
	int numWritten=sprintf(trace_handle_arr[rank].curr_buf_position, "%d 0\n", BEGIN_COMPUTATION);
	trace_handle_arr[rank].curr_buf_position += numWritten;
	trace_handle_arr[rank].records_since_flush++;
	flush(rank);
}

void write_log_footer(int rank){
  int numWritten=sprintf(trace_handle_arr[rank].curr_buf_position, "%d %ld\n", END_COMPUTATION, time_us(ABT_get_wtime()));
  trace_handle_arr[rank].curr_buf_position += numWritten;
	trace_handle_arr[rank].records_since_flush ++;
	flush(rank);
}
/// Write out the bracketed user event when it finishes
void write_EVENT_PAIR(int rank, int userEventID, long startTime){
	long endTime = time_us(ABT_get_wtime());
	int event = 0;
	int numWritten=sprintf(trace_handle_arr[rank].curr_buf_position, "%d %d %ld %d %d\n%d %d %ld %d %d\n", USER_EVENT_PAIR, userEventID, startTime, event, rank, USER_EVENT_PAIR, userEventID, endTime, event, rank);
	trace_handle_arr[rank].curr_buf_position += numWritten;
	trace_handle_arr[rank].records_since_flush++;
	trace_handle_arr[rank].records_since_flush++;
	flush(rank);
}

/// Write out a user event
void write_EVENT(int rank, int userEventID){
	long time = time_us(ABT_get_wtime());
	int event = 0;
	int numWritten=sprintf(trace_handle_arr[rank].curr_buf_position, "%d %d %ld %d %d\n", USER_EVENT, userEventID, time, event, rank);
	trace_handle_arr[rank].curr_buf_position += numWritten;
	trace_handle_arr[rank].records_since_flush++;
	flush(rank);
}
EXTERN_C void ABT_init_trace(int rank)
{
  /*  printf("ABT_init_trace r %d:%d \n", rank, np);*/
  trace_handle_arr[rank].rank=rank;
  trace_handle_arr[rank].out_buf = (char*)malloc(OUTBUFSIZE);
	assert(trace_handle_arr[rank].out_buf);
	trace_handle_arr[rank].out_buf[0]='\0';
	trace_handle_arr[rank].curr_buf_position = trace_handle_arr[rank].out_buf;
	trace_handle_arr[rank].flush_point = trace_handle_arr[rank].out_buf + OUTBUFSIZE - 400; // This should allow one more record before overflowing the buffer
		
	// open file
	char filename[1024];
	sprintf(filename, "ProjPABT.%d.log", rank);
	
	trace_handle_arr[rank].outfile=fopen(filename,"w");
	
	write_log_header(rank);
	
	write_BEGIN_PROCESSING(rank);
	/*	write_EVENT(rank,ABT_Init_event);*/
	
}


EXTERN_C int ABT_finalize_trace(int rank){
  /*  printf("ABT_finalize_trace %d\n",rank);*/
  /*  write_EVENT(rank,ABT_Finalize_event);*/
	write_END_PROCESSING(rank);
	write_log_footer(rank);
	writeToDisk(rank);
	fclose(trace_handle_arr[rank].outfile);
	if(rank==0){
		writeSts();
	}
	return 1; /*TODO find a meaningful return value*/
}

void add_BEGIN_PROCESSING_ENTRY(int rank, int msglen){
        int mtype = 0;
        int entry = source_location_int();
        long time = time_us(ABT_get_wtime());
        int event = 0;
        long recvTime = 0;
        int id0 = 0;
        int id1 = 0;
        int id2 = 0;
        int id3 = 0;
        long cpuStartTime = 0;
        int numPerfCounts = 0;

        trace_handle_arr[rank].recentSourceLocation = entry;
        //      std::cout << "entry=" << entry << std::endl;

        int numWritten=sprintf(trace_handle_arr[rank].curr_buf_position, "%d %d %d %ld %d %d %d %ld %d %d %d %d %ld %d\n", BEGIN_PROCESSING, mtype, entry, time, event, rank, msglen, recvTime, id0, id1, id2, id3, cpuStartTime, numPerfCounts );
        trace_handle_arr[rank].curr_buf_position += numWritten;
        trace_handle_arr[rank].records_since_flush ++;
        flush(rank);

	/*        source_locations.insert(entry);*/
	/*      printf("%d    TotalSoFar=%d\n", entry, source_locations.size());*/

}

void add_BEGIN_PROCESSING_ABT(int rank, int tid){
        int mtype = 0;
        int entry = tid;
        long time = time_us(ABT_get_wtime());
        int event = 0;
        long recvTime = 0;
        int id0 = 0;
        int id1 = 0;
        int id2 = 0;
        int id3 = 0;
        long cpuStartTime = 0;
        int numPerfCounts = 0;

        trace_handle_arr[rank].recentSourceLocation = entry;
        int numWritten=snprintf(trace_handle_arr[rank].curr_buf_position, 80, "%d %d %d %ld %d %d %d %ld %d %d %d %d %ld %d\n", BEGIN_PROCESSING, mtype, entry, time, event, rank, 0, recvTime, id0, id1, id2, id3, cpuStartTime, numPerfCounts );
        trace_handle_arr[rank].curr_buf_position += numWritten;
        trace_handle_arr[rank].records_since_flush ++;
        flush(rank);

	if(entry > source_locations_count) source_locations_count++;


}
void write_BEGIN_IDLE(int rank) 
{
	int entry = source_location_int();
	long time = time_us(ABT_get_wtime());
	trace_handle_arr[rank].recentSourceLocation = entry;
	int numWritten=sprintf(trace_handle_arr[rank].curr_buf_position, "%d %ld %d\n", BEGIN_IDLE, time, rank);
	trace_handle_arr[rank].curr_buf_position += numWritten;
	trace_handle_arr[rank].records_since_flush ++;
	flush(rank);
	/*	source_locations.insert(entry);*/
} 

void write_END_IDLE(int rank) 
{
	int entry = source_location_int();
	long time = time_us(ABT_get_wtime());
	trace_handle_arr[rank].recentSourceLocation = entry;
	int numWritten=sprintf(trace_handle_arr[rank].curr_buf_position, "%d %ld %d\n", END_IDLE, time, rank);
	trace_handle_arr[rank].curr_buf_position += numWritten;
	trace_handle_arr[rank].records_since_flush ++;
	flush(rank);
	/*	source_locations.insert(entry);*/
}
void write_BEGIN_PROCESSING(int rank){
	
	int msglen = 0;
	add_BEGIN_PROCESSING_ENTRY(rank,msglen);	
}

void write_END_PROCESSING(int rank){
	int mtype = 0;
	int entry = trace_handle_arr[rank].recentSourceLocation; // Must match for NoiseMiner to match with previous BEGIN_PROCESSING
	long time = time_us(ABT_get_wtime());
	int event = 0;
	int msglen = 0;
	long cpuEndTime = 0;
	int numPerfCounts = 0;
	
	int numWritten=sprintf(trace_handle_arr[rank].curr_buf_position, "%d %d %d %ld %d %d %d %ld %d\n", END_PROCESSING, mtype, entry, time, event, rank, msglen, cpuEndTime, numPerfCounts );	
	trace_handle_arr[rank].curr_buf_position += numWritten;
	trace_handle_arr[rank].records_since_flush ++;
	flush(rank);
}

void write_END_PROCESSING_ABT(int rank, int tid){
	int mtype = 0;
	int entry = tid; // Must match for NoiseMiner to match with previous BEGIN_PROCESSING
	long time = time_us(ABT_get_wtime());
	int event = 0;
	int msglen = 0;
	long cpuEndTime = 0;
	int numPerfCounts = 0;
	int numWritten = snprintf(trace_handle_arr[rank].curr_buf_position, 80, "%d %d %d %ld %d %d %d %ld %d\n", END_PROCESSING, mtype, entry, time, event, rank, msglen, cpuEndTime, numPerfCounts );	
	trace_handle_arr[rank].curr_buf_position += numWritten;
	trace_handle_arr[rank].records_since_flush ++;
	flush(rank);
}



void write_USER_SUPPLIED(int rank, int value){
	sprintf(trace_handle_arr[rank].curr_buf_position, "%d %d\n", USER_SUPPLIED, value);
	trace_handle_arr[rank].curr_buf_position += strlen(trace_handle_arr[rank].curr_buf_position);	// Advance pointer to what we just wrote
	trace_handle_arr[rank].records_since_flush ++;
	flush(rank);
}



