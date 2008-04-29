#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#define OUTBUFSIZE 1024*1024*4

#define MPI_Send_user_event 0
#define MPI_Recv_user_event 1
#define MPI_Init_user_event 2
#define MPI_Finalize_user_event 3

long records_since_flush = 0;
char *out_buf;
char *curr_buf_position;
char *flush_point;

std::ofstream outfile;

std::map<int,std::string> entryToName;

double initTime;

int rank;
int np;

void init_time(){
	initTime = MPI_Wtime() - 0.5;
}

long time_us(){
	return ((MPI_Wtime()-initTime)*1000000.0);
}


void writeSts(){
	std::ofstream stsfile("ProjPMPI.sts");
	
	stsfile << "PROJECTIONS_ID\n";
	stsfile << "VERSION 6.6\n";
	stsfile << "MACHINE PMPI_Logging\n";
	stsfile << "PROCESSORS " << np <<"\n";
	stsfile << "TOTAL_CHARES 2\n";
	stsfile << "TOTAL_EPS 2\n";
	stsfile << "TOTAL_MSGS 2\n";
	stsfile << "TOTAL_PSEUDOS 0\n";
	stsfile << "TOTAL_EVENTS 4\n";
	stsfile << "CHARE 0 chare2\n";
	stsfile << "CHARE 1 chare1\n";
	stsfile << "ENTRY CHARE 0 entry1 0 0\n";
	stsfile << "ENTRY CHARE 1 entry2 1 1\n";
	stsfile << "MESSAGE 0 0\n";
	stsfile << "MESSAGE 1 0\n";
	stsfile << "EVENT " << MPI_Send_user_event  << " MPI_Send\n";
	stsfile << "EVENT " << MPI_Recv_user_event  << " MPI_Recv\n";
	stsfile << "EVENT " << MPI_Init_user_event  << " MPI_Init\n";
	stsfile << "EVENT " << MPI_Finalize_user_event  << " MPI_Finalize\n";
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
	int entry = 0;
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
		
	sprintf(curr_buf_position, "2 %d %d %ld %d %d %d %ld %d %d %d %d %ld %d\n", mtype, entry, time, event, pe, msglen, recvTime, id0, id1, id2, id3, cpuStartTime, numPerfCounts );
	curr_buf_position += strlen(curr_buf_position); // Advance pointer to what we just wrote
	records_since_flush ++;
	flush();
}

void write_END_PROCESSING(){
	int mtype = 0;
	int entry = 0;
	long time = time_us();
	int event = 0;
	int pe = rank;
	int msglen = 0;
	long cpuEndTime = time_us();
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
	
	int ret = PMPI_Finalize();

	// flush to file
	writeToDisk();
	outfile.close();
	
	if(rank == 0)
		writeSts();
	
	return ret;
}


