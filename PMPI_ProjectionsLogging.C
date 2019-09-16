// (c) 2009 Isaac Dooley

#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <execinfo.h>
#include <stdlib.h>

#include "source_location.h"
#include "PMPI_ProjectionsLogging.h"

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


long records_since_flush = 0;
char *out_buf;
char *curr_buf_position;
char *flush_point;

std::ofstream outfile;

std::map<int,std::string> entryToName;
std::set<int> source_locations;

int recentSourceLocation;

double initTime;

int rank;
int np;

void init_time(){
	initTime = PMPI_Wtime() - 0.001;
}

int getTypeSize(MPI_Datatype ty)
{
    int s;
    // C MPI_Datatypes
    if(ty==MPI_CHAR) s = sizeof(char);
    else if(ty==MPI_BYTE) s = sizeof(unsigned char);
    else if(ty==MPI_SHORT) s = sizeof(short);
    else if(ty==MPI_INT) s =sizeof(int);
    else if(ty==MPI_LONG) s = sizeof(long);
    else if(ty==MPI_FLOAT) s = sizeof(float);
    else if(ty==MPI_DOUBLE) s = sizeof(double);
    else if(ty==MPI_UNSIGNED_CHAR) s = sizeof(unsigned char);
    else if(ty==MPI_UNSIGNED_SHORT) s = sizeof(unsigned short);
    else if(ty==MPI_UNSIGNED) s = sizeof(unsigned int);
    else if(ty==MPI_UNSIGNED_LONG) s = sizeof(unsigned long);
    else if(ty==MPI_LONG_DOUBLE) s = sizeof(long double);
    else if(ty==MPI_LONG_LONG_INT) s = sizeof(long long);
    // Fortran MPI_Datatypes
    else if(ty==MPI_CHARACTER) s = sizeof(char);
    else if(ty==MPI_REAL) s = sizeof(float);
    else if(ty==MPI_REAL4) s = sizeof(float);
    else if(ty==MPI_REAL8) s = sizeof(double);
    else if(ty==MPI_INTEGER) s = sizeof(int);
    else if(ty==MPI_INTEGER2) s = sizeof(short);
    else if(ty==MPI_INTEGER4) s = sizeof(int);
    else if(ty==MPI_LOGICAL) s = sizeof(int);
    else if(ty==MPI_DOUBLE_PRECISION) s = sizeof(double);
    else s = -1;
    
    return s;      
}



long time_us(){
	return ((PMPI_Wtime()-initTime)*1000000.0);
}


void writeSts(){
	std::ofstream stsfile("ProjPMPI.sts");
	
	stsfile << "PROJECTIONS_ID\n";
	stsfile << "VERSION 6.6\n";
	stsfile << "MACHINE PMPI_Logging\n";
	stsfile << "PROCESSORS " << np <<"\n";
	stsfile << "TOTAL_CHARES 1\n";
	stsfile << "TOTAL_EPS " << source_locations.size() << "\n";
	stsfile << "TOTAL_MSGS 2\n";
	stsfile << "TOTAL_PSEUDOS 0\n";
	stsfile << "TOTAL_EVENTS " << NUM_EVENTS << "\n";
	stsfile << "CHARE 0 MPI_Main\n";

	std::set<int>::iterator iter;

	for(iter = source_locations.begin(); iter!= source_locations.end(); iter++){
		stsfile << "ENTRY CHARE " << *iter << " (code region " << *iter<< ") 0 -1\n";
	}
	stsfile << "MESSAGE 0 0\n";
	stsfile << "MESSAGE 1 0\n";
	
	// Call the function from generated-stsEvents.C that fills in all the "EVENT" records
	generateStsEvents(stsfile);

	stsfile << "TOTAL_FUNCTIONS 0 \n";
	stsfile << "END\n";
	
	stsfile.close();
}

void inline writeToDisk(){
  outfile << "PROJECTIONS-RECORD " << records_since_flush << "\n"; // should only appear once in file???
	outfile << out_buf;
	outfile.flush();
	curr_buf_position = out_buf;
	out_buf[0] = '\0';
	records_since_flush = 0;
}

/// Flush buffer if full
void inline flush(){
	if(curr_buf_position > flush_point){
		writeToDisk();
	}
}

void write_log_header(){
	sprintf(curr_buf_position, "%d 0\n", BEGIN_COMPUTATION);
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}

void write_log_footer(){
	sprintf(curr_buf_position, "%d %ld\n", END_COMPUTATION, time_us());
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}

void write_EVENT_PAIR_Comm(int userEventID, long startTime,int count,MPI_Datatype ty){
	int s=getTypeSize(ty);
	int pe = rank;
        sprintf(curr_buf_position, "%d %d 0 %ld 0 %d %d 0\n", CREATION, DEQUEUE, startTime, pe, s*count);
        curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
        records_since_flush ++;
        flush();
	write_EVENT_PAIR(userEventID,startTime);
}
/// Write out the bracketed user event when it finishes
void write_EVENT_PAIR(int userEventID, long startTime){
	long endTime = time_us();
	int event = 0;
	int pe = rank;
	sprintf(curr_buf_position, "%d %d %ld %d %d\n%d %d %ld %d %d\n", USER_EVENT_PAIR, userEventID, startTime, event, pe, USER_EVENT_PAIR, userEventID, endTime, event, pe);
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	records_since_flush ++;
	flush();
}

/// Write out a user event
void write_EVENT(int userEventID){
	long time = time_us();
	int event = 0;
	int pe = rank;
	sprintf(curr_buf_position, "%d %d %ld %d %d\n", USER_EVENT, userEventID, time, event, pe);
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}

void add_BEGIN_PROCESSING_ENTRY(int msglen,int pe){
        int mtype = 0;
        int entry = source_location_int();
        long time = time_us();
        int event = 0;
        long recvTime = 0;
        int id0 = 0;
        int id1 = 0;
        int id2 = 0;
        int id3 = 0;
        long cpuStartTime = 0;
        int numPerfCounts = 0;

        recentSourceLocation = entry;
        //      std::cout << "entry=" << entry << std::endl;

        sprintf(curr_buf_position, "%d %d %d %ld %d %d %d %ld %d %d %d %d %ld %d\n", BEGIN_PROCESSING, mtype, entry, time, event, pe, msglen, recvTime, id0, id1, id2, id3, cpuStartTime, numPerfCounts );
        curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
        records_since_flush ++;
        flush();

        source_locations.insert(entry);
//      printf("%d    TotalSoFar=%d\n", entry, source_locations.size());

}

void write_BEGIN_PROCESSING_AFTER_RECV(int source,int count,MPI_Datatype datatype){

        int pe = source;
        int msglen=count*getTypeSize(datatype);
        add_BEGIN_PROCESSING_ENTRY(msglen,pe);
}


void write_BEGIN_IDLE() 
{
	int entry = source_location_int();
	long time = time_us();
	recentSourceLocation = entry;
    sprintf(curr_buf_position, "%d %ld %d\n", BEGIN_IDLE, time, rank);
    curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
	source_locations.insert(entry);
} 

void write_END_IDLE() 
{
	int entry = source_location_int();
	long time = time_us();
	recentSourceLocation = entry;
    sprintf(curr_buf_position, "%d %ld %d\n", END_IDLE, time, rank);
    curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
	source_locations.insert(entry);
}
void write_BEGIN_PROCESSING(){
	
	int pe = rank;
	int msglen = 0;
	add_BEGIN_PROCESSING_ENTRY(msglen,pe);	
}

void write_END_PROCESSING(){
	int mtype = 0;
	int entry = recentSourceLocation; // Must match for NoiseMiner to match with previous BEGIN_PROCESSING
	long time = time_us();
	int event = 0;
	int pe = rank;
	int msglen = 0;
	long cpuEndTime = 0;
	int numPerfCounts = 0;
	
    sprintf(curr_buf_position, "%d %d %d %ld %d %d %d %ld %d\n", END_PROCESSING, mtype, entry, time, event, pe, msglen, cpuEndTime, numPerfCounts );	
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}



void write_USER_SUPPLIED(int value){
	sprintf(curr_buf_position, "%d %d\n", USER_SUPPLIED, value);
	curr_buf_position += strlen(curr_buf_position);	// Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}








EXTERN_C int MPI_Init(int * p1, char *** p2){
	int ret = PMPI_Init(p1,p2);
	PMPI_Comm_rank( MPI_COMM_WORLD, &rank );
	PMPI_Comm_size( MPI_COMM_WORLD, &np );
	init_time();
     	
	out_buf = (char*)malloc(OUTBUFSIZE);
	assert(out_buf);
	out_buf[0]='\0';
	curr_buf_position = out_buf;
	flush_point = out_buf + OUTBUFSIZE - 400; // This should allow one more record before overflowing the buffer
		
	// open file
	char filename[1024];
	sprintf(filename, "ProjPMPI.%d.log", rank);
	
	outfile.open(filename);
	
	write_log_header();
	
	write_BEGIN_PROCESSING();
	write_EVENT(MPI_Init_event);
	
	return ret;
}


EXTERN_C int MPI_Init_thread(int * p1, char *** p2, int required, int *provided){
	int ret = PMPI_Init_thread(p1,p2, required, provided);
	PMPI_Comm_rank( MPI_COMM_WORLD, &rank );
	PMPI_Comm_size( MPI_COMM_WORLD, &np );
	init_time();

	out_buf = (char*)malloc(OUTBUFSIZE);
	assert(out_buf);
	out_buf[0]='\0';
	curr_buf_position = out_buf;
	flush_point = out_buf + OUTBUFSIZE - 400; // This should allow one more record before overflowing the buffer

	// open file
	char filename[1024];
	sprintf(filename, "ProjPMPI.%d.log", rank);

	outfile.open(filename);

	write_log_header();

	write_BEGIN_PROCESSING();
	write_EVENT(MPI_Init_event);

	return ret;
}

EXTERN_C int MPI_Finalize(void){
	write_EVENT(MPI_Finalize_event);
	write_END_PROCESSING();
	write_log_footer();
	writeToDisk();
	outfile.close();
	
	PMPI_Barrier(MPI_COMM_WORLD);
	
	//	printf("Starting with %d mangled source code locations\n", source_locations.size());

	// Send all results to zero
	int tag = 7777;
	if(rank==0){
		for(int p=1;p<np;p++){
			MPI_Status status;
			int incoming_source_locations[256];
			PMPI_Recv(incoming_source_locations, 256, MPI_INT, p, tag, MPI_COMM_WORLD, &status);

			for(int j=0; j<incoming_source_locations[0]; j++){
				source_locations.insert(incoming_source_locations[1+j]);
				//				printf("Now we have %d mangled source code locations\n", source_locations.size());
			}
		}

		writeSts();
	} else {
		assert(source_locations.size()<256);
		int source_locations_flat[256];
		source_locations_flat[0] = source_locations.size();
		int c=1;
		for(std::set<int>::iterator i = source_locations.begin(); i!=source_locations.end();i++){
			source_locations_flat[c++] = *i;
		}
		PMPI_Send(source_locations_flat, 256, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}
	

	int ret = PMPI_Finalize();
	return ret;
}

// FORTRAN bindings

EXTERN_C void mpi_init_( MPI_Fint *ierr )
{
    *ierr = MPI_Init(NULL, NULL );
}

EXTERN_C void mpi_init_thread_( MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr )
{
    *ierr = MPI_Init_thread(NULL, NULL, *required, provided );
}

EXTERN_C void mpi_finalize_(int *ierr )
{
    *ierr = MPI_Finalize();
}


EXTERN_C void mpi_comm_create_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *__ierr)
{
    *__ierr = MPI_Comm_create((MPI_Comm) *comm, (MPI_Group) *group, (MPI_Comm*)newcomm);
    
}


EXTERN_C int mpi_comm_dup_(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *__ierr)
{
    *__ierr = MPI_Comm_dup((MPI_Comm) *comm, (MPI_Comm*) newcomm);
}

EXTERN_C int mpi_comm_split_(MPI_Fint *comm, MPI_Fint* color, MPI_Fint* key, MPI_Fint* newcomm, MPI_Fint *__ierr)
{
    *__ierr = MPI_Comm_split((MPI_Comm) *comm, (int) *color, (int) *key, (MPI_Comm*)newcomm);
}



EXTERN_C void mpi_send_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint *__ierr )
{
    *__ierr = MPI_Send(buf, (int)*count, (MPI_Datatype)*datatype,
                       (int)*dest, (int)*tag, (MPI_Comm)*comm);
}

EXTERN_C void mpi_isend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                 MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                 MPI_Fint *request, MPI_Fint *__ierr )
{
    *__ierr = MPI_Isend(buf,(int)*count,(MPI_Datatype)*datatype,
                        (int)*dest,
                        (int)*tag,(MPI_Comm)*comm,
                        (MPI_Request*)request);
}

EXTERN_C void mpi_bsend_( void *buf, MPI_Fint *count, MPI_Fint *datatype, 
                 MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, 
                 MPI_Fint *__ierr )
{
    *__ierr = MPI_Bsend( buf, (int)*count, (MPI_Datatype)*datatype,
                         (int)*dest, (int)*tag, (MPI_Comm)*comm );
}

EXTERN_C void mpi_ibsend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                  MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                  MPI_Fint *request, MPI_Fint *__ierr )
{
    *__ierr = MPI_Ibsend(buf,(int)*count,(MPI_Datatype)*datatype,
                         (int)*dest,(int)*tag,(MPI_Comm)*comm,
                         (MPI_Request*)request);
}


EXTERN_C void mpi_rsend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                 MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                 MPI_Fint *__ierr )
{
    *__ierr = MPI_Rsend(buf, (int)*count,(MPI_Datatype)*datatype,
                        (int)*dest, (int)*tag, (MPI_Comm)*comm);
}

EXTERN_C void mpi_irsend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                  MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                  MPI_Fint *request, MPI_Fint *__ierr )
{
    *__ierr = MPI_Irsend(buf,(int)*count,(MPI_Datatype)*datatype,
                         (int)*dest,(int)*tag,
                         (MPI_Comm)*comm,(MPI_Request*)request);
}


EXTERN_C void mpi_ssend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                 MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                 MPI_Fint *__ierr )
{
    *__ierr = MPI_Ssend(buf, (int)*count,
                        (MPI_Datatype)*datatype, (int)*dest, (int)*tag,
                        (MPI_Comm)*comm);
}


EXTERN_C void mpi_issend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                  MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                  MPI_Fint *request, MPI_Fint *__ierr )
{
    
    *__ierr = MPI_Issend(buf,(int)*count,(MPI_Datatype)*datatype,
                         (int)*dest, (int)*tag,
                         (MPI_Comm)*comm,
                         (MPI_Request*)request);
}


EXTERN_C void mpi_sendrecv_( void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype,
                    MPI_Fint *dest, MPI_Fint *sendtag,
                    void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
                    MPI_Fint *source, MPI_Fint *recvtag,
                    MPI_Fint *comm, MPI_Fint *status, MPI_Fint *__ierr )
{
    *__ierr = MPI_Sendrecv(sendbuf, (int)*sendcount,
                           (MPI_Datatype)*sendtype, (int)*dest,
                           (int)*sendtag, recvbuf,
                           (int)*recvcount, (MPI_Datatype)*recvtype,
                           (int)*source, (int)*recvtag,
                           (MPI_Comm)*comm, (MPI_Status*)status);
}



EXTERN_C void mpi_sendrecv_replace_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                            MPI_Fint *dest, MPI_Fint *sendtag,
                            MPI_Fint *source, MPI_Fint *recvtag,
                            MPI_Fint *comm, MPI_Fint *status,
                            MPI_Fint *__ierr )
{
    *__ierr = MPI_Sendrecv_replace(buf, (int)*count,
                                   (MPI_Datatype)*datatype, (int)*dest,
                                   (int)*sendtag, (int)*source, (int)*recvtag,
                                   (MPI_Comm)*comm, (MPI_Status*)status );
}

EXTERN_C void mpi_start_( MPI_Fint *request, MPI_Fint *__ierr )
{
 
    *__ierr = MPI_Start( (MPI_Request*)request );
}

EXTERN_C void mpi_startall_( MPI_Fint *count, MPI_Fint array_of_requests[],
                    MPI_Fint *__ierr )
{

        *__ierr = MPI_Startall((int)*count,(MPI_Request*)array_of_requests);
}


EXTERN_C void mpi_allgather_ ( void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype,
                      void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
                      MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Allgather(sendbuf, (int)*sendcount,
                            (MPI_Datatype)*sendtype,
                            recvbuf,
                            (int)*recvcount,
                            (MPI_Datatype)*recvtype,
                            (MPI_Comm)*comm);
}

EXTERN_C void mpi_allgatherv_ ( void *sendbuf, MPI_Fint *sendcount,  MPI_Fint *sendtype,
                       void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs,
                       MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *__ierr )
{

    *__ierr = MPI_Allgatherv(sendbuf, (int)*sendcount,
                             (MPI_Datatype)*sendtype,
                             recvbuf, (int*) recvcounts,
                             (int*)displs, (MPI_Datatype)*recvtype,
                             (MPI_Comm)*comm);
}

EXTERN_C void mpi_allreduce_ ( void *sendbuf, void *recvbuf, MPI_Fint *count,
                      MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
                      MPI_Fint *__ierr )
{

    *__ierr = MPI_Allreduce(sendbuf,recvbuf,
                            (int)*count, (MPI_Datatype)*datatype,
                            (MPI_Op)*op, (MPI_Comm)*comm );
}

EXTERN_C void mpi_alltoall_( void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype,
                    void *recvbuf, MPI_Fint *recvcnt, MPI_Fint *recvtype,
                    MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Alltoall(sendbuf, (int)*sendcount,
                           (MPI_Datatype)*sendtype, recvbuf,
                           (int)*recvcnt, (MPI_Datatype)*recvtype,
                           (MPI_Comm)*comm );
}

EXTERN_C void mpi_alltoallv_ ( void *sendbuf, MPI_Fint *sendcnts,
                      MPI_Fint *sdispls, MPI_Fint *sendtype,
                      void *recvbuf, MPI_Fint *recvcnts,
                      MPI_Fint *rdispls, MPI_Fint *recvtype,
                      MPI_Fint *comm, MPI_Fint *__ierr )
{
 
    *__ierr = MPI_Alltoallv(sendbuf, (int*)sendcnts,
                            (int*)sdispls, (MPI_Datatype)*sendtype,
                            recvbuf, (int*)recvcnts,
                            (int*)rdispls, (MPI_Datatype)*recvtype,
                            (MPI_Comm)*comm );

}

/* 
EXTERN_C void mpi_alltoallw_ ( void *sendbuf, MPI_Fint *sendcnts,
                      MPI_Fint *sdispls, MPI_Fint *sendtypes,
                      void *recvbuf, MPI_Fint *recvcnts,
                      MPI_Fint *rdispls, MPI_Fint *recvtypes,
                      MPI_Fint *comm, MPI_Fint *__ierr )
{
 
    *__ierr = MPI_Alltoallw(sendbuf, (int*)sendcnts,
                            (int*)sdispls, (MPI_Datatype *)sendtypes,
                            recvbuf, (int*)recvcnts,
                            (int*)rdispls, (MPI_Datatype *)recvtypes,
                            (MPI_Comm)*comm );
}
*/

EXTERN_C void mpi_barrier_ ( MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Barrier( (MPI_Comm)*comm );
}

EXTERN_C void mpi_bcast_ ( void *buffer, MPI_Fint *count, MPI_Fint *datatype,
                  MPI_Fint *root, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Bcast(buffer, (int)*count,
                        (MPI_Datatype)*datatype, (int)*root,
                        (MPI_Comm)*comm);
}

EXTERN_C void mpi_exscan_ (void *sendbuf, void *recvbuf, MPI_Fint *count,
                      MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
                      MPI_Fint *__ierr )
{
    *__ierr = MPI_Exscan(sendbuf,recvbuf,
                         (int)*count, (MPI_Datatype)*datatype,
                         (MPI_Op)*op, (MPI_Comm)*comm );
}

EXTERN_C void mpi_gather_ ( void *sendbuf, MPI_Fint *sendcnt, MPI_Fint *sendtype,
                   void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
                   MPI_Fint *root, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Gather(sendbuf, (int)*sendcnt,
                         (MPI_Datatype)*sendtype, recvbuf,
                         (int)*recvcount, (MPI_Datatype)*recvtype,
                         (int)*root, (MPI_Comm)*comm);
}

EXTERN_C void mpi_gatherv_ ( void *sendbuf, MPI_Fint *sendcnt, MPI_Fint *sendtype,
                    void *recvbuf, MPI_Fint *recvcnts, MPI_Fint *displs,
                    MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm,
                    MPI_Fint *__ierr )
{
    *__ierr = MPI_Gatherv(sendbuf, (int)*sendcnt,
                          (MPI_Datatype)*sendtype, recvbuf,
                          (int*)recvcnts, (int*)displs,
                          (MPI_Datatype)*recvtype, (int)*root,
                          (MPI_Comm)*comm);
}

EXTERN_C void mpi_reduce_scatter_ ( void *sendbuf, void *recvbuf,
                           MPI_Fint *recvcnts, MPI_Fint *datatype,
                           MPI_Fint *op, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Reduce_scatter(sendbuf,
                                 recvbuf, (int*)recvcnts,
                                 (MPI_Datatype)*datatype, (MPI_Op)*op,
                                 (MPI_Comm)*comm);
}

/*void mpi_reduce_scatter_block_ ( void *sendbuf, void *recvbuf,
                                 MPI_Fint *recvcount, MPI_Fint *datatype,
                                 MPI_Fint *op, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Reduce_scatter_block(sendbuf,
                                 recvbuf, (int)*recvcount,
                                 MPI_Type_f2c(*datatype), 
				 (MPI_Op)*op,
                                 (MPI_Comm)*comm);
}*/

EXTERN_C void mpi_reduce_ ( void *sendbuf, void *recvbuf, MPI_Fint *count,
                   MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root,
                   MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Reduce(sendbuf, recvbuf,
                         (int)*count, (MPI_Datatype)*datatype,
                         (MPI_Op)*op, (int)*root,
                         (MPI_Comm)*comm);
}

/*void mpi_reduce_local_ ( void *inbuf, void *inoutbuf, MPI_Fint *count,
                   MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root,
                   MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Reduce_local(inbuf, inoutbuf,
                               (int)*count, (MPI_Datatype)*datatype,
                               (MPI_Op)*op);
}*/

EXTERN_C void mpi_scan_ ( void *sendbuf, void *recvbuf, MPI_Fint *count,
                 MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
                 MPI_Fint *__ierr )
{
    *__ierr = MPI_Scan(sendbuf, recvbuf,
                       (int)*count, (MPI_Datatype)*datatype,
                       (MPI_Op)*op, (MPI_Comm)*comm);
}

EXTERN_C void mpi_scatter_ ( void *sendbuf, MPI_Fint *sendcnt, MPI_Fint *sendtype,
                    void *recvbuf, MPI_Fint *recvcnt, MPI_Fint *recvtype,
                    MPI_Fint *root, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Scatter(sendbuf, (int)*sendcnt,
                          (MPI_Datatype)*sendtype, recvbuf,
                          (int)*recvcnt, (MPI_Datatype)*recvtype,
                          (int)*root, (MPI_Comm)*comm);
}

EXTERN_C void mpi_scatterv_ ( void *sendbuf, MPI_Fint *sendcnts,
                     MPI_Fint *displs, MPI_Fint *sendtype,
                     void *recvbuf, MPI_Fint *recvcnt, 
                     MPI_Fint *recvtype, MPI_Fint *root,
                     MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Scatterv(sendbuf, (int*)sendcnts, (int*)displs,
                           (MPI_Datatype)*sendtype, recvbuf,
                           (int)*recvcnt, (MPI_Datatype)*recvtype,
                           (int)*root, (MPI_Comm)*comm );
}

EXTERN_C int mpi_recv_ ( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint *status, MPI_Fint *__ierr )
{
    *__ierr = MPI_Recv(buf, (int)*count, (MPI_Datatype)*datatype,
                       (int)*source, (int)*tag, (MPI_Comm)*comm,
                       (MPI_Status*)status);
}

EXTERN_C int mpi_irecv_ ( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                 MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm,
                 MPI_Fint *request, MPI_Fint *__ierr )
{
    *__ierr = MPI_Irecv(buf,(int)*count,(MPI_Datatype)*datatype,
                        (int)*source,
                        (int)*tag,(MPI_Comm)*comm,
                        (MPI_Request*)request);
}

EXTERN_C double mpi_wtime_ (MPI_Fint *__ierr )
{
    *__ierr = MPI_Wtime();
}

EXTERN_C double mpi_wtick_ (MPI_Fint *__ierr )
{
    *__ierr = MPI_Wtick();
}

EXTERN_C int mpi_cart_create_ (MPI_Fint *comm_old, MPI_Fint *ndims, MPI_Fint *dims,
                             MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart,
                             MPI_Fint *__ierr )
{
    *__ierr = MPI_Cart_create((MPI_Comm)*comm_old, (int)*ndims, (int*)dims,
                              (int*)periods, (int)*reorder, (MPI_Comm*)comm_cart);
}

EXTERN_C int mpi_cart_coords_ (MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, 
                               MPI_Fint *coords, MPI_Fint *__ierr )
{
    *__ierr = MPI_Cart_coords((MPI_Comm)*comm, (int)*rank, (int)*maxdims, (int*)coords);
}

EXTERN_C int mpi_cart_shift_ (MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *disp, 
                              MPI_Fint *rank_source, MPI_Fint *rank_dest, MPI_Fint *__ierr )
{
    *__ierr = MPI_Cart_shift((MPI_Comm)*comm, (int)*direction, (int)*disp, (int*)rank_source,
                              (int*)rank_dest);
}

EXTERN_C int mpi_cart_sub_ (MPI_Fint *comm, MPI_Fint *remain_dims, MPI_Fint *newcomm,
                            MPI_Fint *__ierr )
{
    *__ierr = MPI_Cart_sub((MPI_Comm)*comm, (int*)remain_dims, (MPI_Comm*)newcomm);
}

EXTERN_C int mpi_cart_rank_ (MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *__ierr )
{
    *__ierr = MPI_Cart_rank((MPI_Comm)*comm, (int*)coords, (int*)rank);
}

EXTERN_C int mpi_cart_get_ (MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods,
                            MPI_Fint *coords, MPI_Fint *__ierr )
{
    *__ierr = MPI_Cart_get((MPI_Comm)*comm, (int)*maxdims, (int*)dims, (int*)periods, (int*)coords);
}

EXTERN_C int mpi_cart_map_ (MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods,
                            MPI_Fint *newrank, MPI_Fint *__ierr )
{
    *__ierr = MPI_Cart_map((MPI_Comm)*comm, (int)*ndims, (int*)dims, (int*)periods, (int*)newrank);
}

