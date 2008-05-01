#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <execinfo.h>

#define OUTBUFSIZE 1024*1024*4

#define MPI_Send_user_event 0
#define MPI_Recv_user_event 1
#define MPI_Init_user_event 2
#define MPI_Finalize_user_event 3
#define MPI_Barrier_user_event 4

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
	initTime = PMPI_Wtime() - 0.1;
}

long time_us(){
	return ((PMPI_Wtime()-initTime)*1000000.0);
}

/** 
    Mangle together the return pointers from all stack frames to get a 32-bit integer
    likely to be a unique for each point in the user's code.

	I am not sure how likely collisions are using this method.
*/
unsigned long source_location(){
	unsigned long long s = 0;
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	if(frames > 0){
		s = (unsigned long long)callstack[0];
		for (i = 1; i < frames; ++i) {
			s = s ^ (unsigned long long)callstack[i];
		}
	}

	// Drop this down to an unsigned long
	unsigned long s_ul = (unsigned long)(s & 0xFFFFFFFF) ^  (unsigned long)((s>>32) & 0xFFFFFFFF);
	
	return s_ul;
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
	stsfile << "TOTAL_EVENTS 5\n";
	stsfile << "CHARE 0 MPI_Main\n";

	std::set<int>::iterator iter;
	int id=0;
	for(iter = source_locations.begin(); iter!= source_locations.end(); iter++){
		stsfile << "ENTRY CHARE " << *iter << " (code region " << *iter<< ") 0 -1\n";
	}
	stsfile << "MESSAGE 0 0\n";
	stsfile << "MESSAGE 1 0\n";
	stsfile << "EVENT " << MPI_Send_user_event  << " MPI_Send\n";
	stsfile << "EVENT " << MPI_Recv_user_event  << " MPI_Recv\n";
	stsfile << "EVENT " << MPI_Init_user_event  << " MPI_Init\n";
	stsfile << "EVENT " << MPI_Finalize_user_event  << " MPI_Finalize\n";
	stsfile << "EVENT " << MPI_Barrier_user_event << " MPI_Barrier\n";
	stsfile << "TOTAL_FUNCTIONS 0 \n";
	stsfile << "END\n";
	
	stsfile.close();
}

void inline writeToDisk(){
	outfile << "PROJECTIONS-RECORD " << records_since_flush << "\n";
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
	sprintf(curr_buf_position, "6 0\n");
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}

void write_log_footer(){
	sprintf(curr_buf_position, "7 %ld\n",time_us());
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}


/// Write out the bracketed user event when it finishes
void write_USER_EVENT_PAIR(int userEventID, long startTime){
	long endTime = time_us();
	int event = 0;
	int pe = rank;
	sprintf(curr_buf_position, "100 %d %ld %d %d\n100 %d %ld %d %d\n", userEventID, startTime, event, pe, userEventID, endTime, event, pe);
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	records_since_flush ++;
	flush();
}

/// Write out a user event
void write_USER_EVENT(int userEventID){
	long time = time_us();
	int event = 0;
	int pe = rank;
	sprintf(curr_buf_position, "13 %d %ld %d %d\n", userEventID, time, event, pe);
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}

void write_BEGIN_PROCESSING(){
	
	int mtype = 0;
	int entry = source_location();
	long time = time_us();
	int event = 0;
	int pe = rank;
	int msglen = 0;
	long recvTime = 0;
	int id0 = 0;
	int id1 = 0;
	int id2 = 0;
	int id3 = 0;
	long cpuStartTime = 0;
	int numPerfCounts = 0;
	
	recentSourceLocation = entry;
		
	sprintf(curr_buf_position, "2 %d %d %ld %d %d %d %ld %d %d %d %d %ld %d\n", mtype, entry, time, event, pe, msglen, recvTime, id0, id1, id2, id3, cpuStartTime, numPerfCounts );
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
	
	source_locations.insert(entry);
//	printf("%d    TotalSoFar=%d\n", entry, source_locations.size());
	
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
		
	sprintf(curr_buf_position, "3 %d %d %ld %d %d %d %ld %d\n", mtype, entry, time, event, pe, msglen, cpuEndTime, numPerfCounts );
	curr_buf_position += strlen(curr_buf_position);	// Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}



void write_USER_SUPPLIED(int value){
	sprintf(curr_buf_position, "26 %d\n", value);
	curr_buf_position += strlen(curr_buf_position);	// Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}


int MPI_Barrier(MPI_Comm comm) {
	write_END_PROCESSING();
	
	long startTime = time_us();
	int ret = PMPI_Barrier(comm);
	write_USER_EVENT_PAIR(MPI_Barrier_user_event, startTime);
	
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Send(void * buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
	write_END_PROCESSING();
	
	long startTime = time_us();
	int ret = PMPI_Send(buf, count, datatype, dest, tag, comm);
	write_USER_EVENT_PAIR(MPI_Send_user_event, startTime);
	
	write_BEGIN_PROCESSING();
	return ret;
}

int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Status * status){
	write_END_PROCESSING();
	
	long startTime = time_us();
	int ret = PMPI_Recv(buf, count, datatype, dest, tag, comm, status);
	write_USER_EVENT_PAIR(MPI_Recv_user_event, startTime);
	
	write_BEGIN_PROCESSING();
	return ret;
}


int MPI_Init(int * p1, char *** p2){
	int ret = PMPI_Init(p1,p2);
	PMPI_Comm_rank( MPI_COMM_WORLD, &rank );
	PMPI_Comm_size( MPI_COMM_WORLD, &np );
	init_time();
	
	// The code in trace-projections.C assumes we are in a charm++ program, 
	// so we will do a little setup here so that things will work
	
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
	write_USER_EVENT(MPI_Init_user_event);
	
	return ret;
}

int MPI_Finalize(void){
	write_USER_EVENT(MPI_Finalize_user_event);
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


