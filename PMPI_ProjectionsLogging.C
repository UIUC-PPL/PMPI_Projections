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
#include <execinfo.h>
#include <stdlib.h>

#include "source_location.h"
#include "PMPI_ProjectionsLogging.h"

#define OUTBUFSIZE 1024*1024*4


long records_since_flush = 0;
char *out_buf;
char *curr_buf_position;
char *flush_point;
//int collective;

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
void write_EVENT_PAIR(int userEventID, long startTime){
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
void write_EVENT(int userEventID){
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
	int entry = source_location_int();
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

	//	std::cout << "entry=" << entry << std::endl;

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








int MPI_Init(int * p1, char *** p2){
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

int MPI_Finalize(void){
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

// FORTRAN bindings
extern "C"{
void mpi_init_( MPI_Fint *ierr )
{
    *ierr = MPI_Init(NULL, NULL );

}

void mpi_finalize_(int *ierr )
{
    *ierr = MPI_Finalize();
}


void mpi_comm_create_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *__ierr)
{
    MPI_Comm newcommC = MPI_Comm_f2c(*newcomm);
    *__ierr = MPI_Comm_create(MPI_Comm_f2c(*comm), MPI_Group_f2c(*group), &newcommC);
}


int mpi_comm_dup_(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *__ierr)
{
    MPI_Comm newcommC = MPI_Comm_f2c(*newcomm);
    *__ierr = MPI_Comm_dup(MPI_Comm_f2c(*comm), &newcommC);
}

int mpi_comm_split_(MPI_Fint *comm, MPI_Fint* color, MPI_Fint* key, MPI_Fint* newcomm, MPI_Fint *__ierr)
{
    MPI_Comm newcommC = MPI_Comm_f2c(*newcomm);
    *__ierr = MPI_Comm_split(MPI_Comm_f2c(*comm), (int) *color, (int) *key, &newcommC);
}



void mpi_send_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint *__ierr )
{
    *__ierr = MPI_Send(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*dest, (int)*tag, MPI_Comm_f2c(*comm));
}

void mpi_recv_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *src, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint * status,MPI_Fint *__ierr )
{
    MPI_Status statusC;
    MPI_Status_f2c(status,&statusC);
    *__ierr = MPI_Recv(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*src, (int)*tag, MPI_Comm_f2c(*comm), &statusC);
}

void mpi_isend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                 MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                 MPI_Fint *request, MPI_Fint *__ierr )
{
    MPI_Request reqC = MPI_Request_f2c(*request);
    *__ierr = MPI_Isend(buf,(int)*count,MPI_Type_f2c(*datatype),
                        (int)*dest,
                        (int)*tag,MPI_Comm_f2c(*comm),
                        &reqC);
}
void mpi_irecv_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *src, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint * request,MPI_Fint *__ierr )
{
    MPI_Request reqC = MPI_Request_f2c(*request);
    *__ierr = MPI_Irecv(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*src, (int)*tag, MPI_Comm_f2c(*comm),&reqC);
}

void mpi_bsend_( void *buf, MPI_Fint *count, MPI_Fint *datatype, 
                 MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, 
                 MPI_Fint *__ierr )
{
    *__ierr = MPI_Bsend( buf, (int)*count, MPI_Type_f2c(*datatype),
                         (int)*dest, (int)*tag, MPI_Comm_f2c(*comm) );
}
/*void mpi_brecv_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint * status,MPI_Fint *__ierr )
{
    MPI_Status statusC;
    MPI_Status_f2c(status,&statusC);
    *__ierr = MPI_Brecv(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*dest, (int)*tag, MPI_Comm_f2c(*comm), &statusC);
}*/

void mpi_ibsend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                  MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                  MPI_Fint *request, MPI_Fint *__ierr )
{
    MPI_Request reqC = MPI_Request_f2c(*request);
    *__ierr = MPI_Ibsend(buf,(int)*count,MPI_Type_f2c(*datatype),
                         (int)*dest,(int)*tag,MPI_Comm_f2c(*comm),
                         &reqC);
}

/*void mpi_ibrecv_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint * request,MPI_Fint *__ierr )
{
    *__ierr = MPI_Ibrecv(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*dest, (int)*tag, MPI_Comm_f2c(*comm),MPI_Request_f2c(*request));
}*/

void mpi_rsend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                 MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                 MPI_Fint *__ierr )
{
    *__ierr = MPI_Rsend(buf, (int)*count,MPI_Type_f2c(*datatype),
                        (int)*dest, (int)*tag, MPI_Comm_f2c(*comm));
}

/*void mpi_rrecv_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint * status,MPI_Fint *__ierr )
{
    MPI_Status statusC;
    MPI_Status_f2c(status,&statusC);
    *__ierr = MPI_Rrecv(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*dest, (int)*tag, MPI_Comm_f2c(*comm), &statusC);
}*/

void mpi_irsend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                  MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                  MPI_Fint *request, MPI_Fint *__ierr )
{
    MPI_Request reqC = MPI_Request_f2c(*request);
    *__ierr = MPI_Irsend(buf,(int)*count,MPI_Type_f2c(*datatype),
                         (int)*dest,(int)*tag,
                         MPI_Comm_f2c(*comm),&reqC);
}

/*void mpi_irrecv_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint * request,MPI_Fint *__ierr )
{
    *__ierr = MPI_Irrecv(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*dest, (int)*tag, MPI_Comm_f2c(*comm),MPI_Request_f2c(*request));
}*/

void mpi_ssend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                 MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                 MPI_Fint *__ierr )
{
    *__ierr = MPI_Ssend(buf, (int)*count,
                        MPI_Type_f2c(*datatype), (int)*dest, (int)*tag,
                        MPI_Comm_f2c(*comm));
}

/*void mpi_srecv_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint * status,MPI_Fint *__ierr )
{
    MPI_Status statusC;
    MPI_Status_f2c(status,&statusC);
    *__ierr = MPI_Srecv(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*dest, (int)*tag, MPI_Comm_f2c(*comm), &statusC);
}*/

void mpi_issend_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                  MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                  MPI_Fint *request, MPI_Fint *__ierr )
{
    MPI_Request reqC = MPI_Request_f2c(*request);
    *__ierr = MPI_Issend(buf,(int)*count,MPI_Type_f2c(*datatype),
                         (int)*dest, (int)*tag,
                         MPI_Comm_f2c(*comm),
                         &reqC);
}

/*void mpi_isrecv_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm,
                MPI_Fint * request,MPI_Fint *__ierr )
{
    *__ierr = MPI_Isrecv(buf, (int)*count, MPI_Type_f2c(*datatype),
                       (int)*dest, (int)*tag, MPI_Comm_f2c(*comm),MPI_Request_f2c(*request));
}*/

void mpi_sendrecv_( void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype,
                    MPI_Fint *dest, MPI_Fint *sendtag,
                    void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
                    MPI_Fint *source, MPI_Fint *recvtag,
                    MPI_Fint *comm, MPI_Fint *status, MPI_Fint *__ierr )
{
    MPI_Status statusC;
    MPI_Status_f2c(status,&statusC);
    *__ierr = MPI_Sendrecv(sendbuf, (int)*sendcount,
                           MPI_Type_f2c(*sendtype), (int)*dest,
                           (int)*sendtag, recvbuf,
                           (int)*recvcount, MPI_Type_f2c(*recvtype),
                           (int)*source, (int)*recvtag,
                           MPI_Comm_f2c(*comm), &statusC );
}



void mpi_sendrecv_replace_( void *buf, MPI_Fint *count, MPI_Fint *datatype,
                            MPI_Fint *dest, MPI_Fint *sendtag,
                            MPI_Fint *source, MPI_Fint *recvtag,
                            MPI_Fint *comm, MPI_Fint *status,
                            MPI_Fint *__ierr )
{
    MPI_Status statusC;
    MPI_Status_f2c(status,&statusC);
    *__ierr = MPI_Sendrecv_replace(buf, (int)*count,
                                   MPI_Type_f2c(*datatype), (int)*dest,
                                   (int)*sendtag, (int)*source, (int)*recvtag,
                                   MPI_Comm_f2c(*comm), &statusC );
}

void mpi_start_( MPI_Fint *request, MPI_Fint *__ierr )
{
    MPI_Request reqC = MPI_Request_f2c(*request); 
    *__ierr = MPI_Start( &reqC );
}

void mpi_startall_( MPI_Fint *count, MPI_Fint array_of_requests[],
                    MPI_Fint *__ierr )
{

        *__ierr = MPI_Startall((int)*count,(MPI_Request*)array_of_requests);
}


void mpi_allgather_ ( void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype,
                      void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
                      MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Allgather(sendbuf, (int)*sendcount,
                            MPI_Type_f2c(*sendtype),
                            recvbuf,
                            (int)*recvcount,
                            MPI_Type_f2c(*recvtype),
                            MPI_Comm_f2c(*comm));
}

void mpi_allgatherv_ ( void *sendbuf, MPI_Fint *sendcount,  MPI_Fint *sendtype,
                       void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs,
                       MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *__ierr )
{

    *__ierr = MPI_Allgatherv(sendbuf, (int)*sendcount,
                             MPI_Type_f2c(*sendtype),
                             recvbuf, (int*) recvcounts,
                             (int*)displs, MPI_Type_f2c(*recvtype),
                             MPI_Comm_f2c(*comm));
}

void mpi_allreduce_ ( void *sendbuf, void *recvbuf, MPI_Fint *count,
                      MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
                      MPI_Fint *__ierr )
{

    *__ierr = MPI_Allreduce(sendbuf,recvbuf,
                            (int)*count, MPI_Type_f2c(*datatype),
                            MPI_Op_f2c(*op), MPI_Comm_f2c(*comm) );
}

void mpi_alltoall_( void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype,
                    void *recvbuf, MPI_Fint *recvcnt, MPI_Fint *recvtype,
                    MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Alltoall(sendbuf, (int)*sendcount,
                           MPI_Type_f2c(*sendtype), recvbuf,
                           (int)*recvcnt, MPI_Type_f2c(*recvtype),
                           MPI_Comm_f2c(*comm) );
}

void mpi_alltoallv_ ( void *sendbuf, MPI_Fint *sendcnts,
                      MPI_Fint *sdispls, MPI_Fint *sendtype,
                      void *recvbuf, MPI_Fint *recvcnts,
                      MPI_Fint *rdispls, MPI_Fint *recvtype,
                      MPI_Fint *comm, MPI_Fint *__ierr )
{
 
    *__ierr = MPI_Alltoallv(sendbuf, (int*)sendcnts,
                            (int*)sdispls, MPI_Type_f2c(*sendtype),
                            recvbuf, (int*)recvcnts,
                            (int*)rdispls, MPI_Type_f2c(*recvtype),
                            MPI_Comm_f2c(*comm) );

}

/* 
void mpi_alltoallw_ ( void *sendbuf, MPI_Fint *sendcnts,
                      MPI_Fint *sdispls, MPI_Fint *sendtypes,
                      void *recvbuf, MPI_Fint *recvcnts,
                      MPI_Fint *rdispls, MPI_Fint *recvtypes,
                      MPI_Fint *comm, MPI_Fint *__ierr )
{
 
    *__ierr = MPI_Alltoallw(sendbuf, (int*)sendcnts,
                            (int*)sdispls, (MPI_Datatype *)(*sendtypes),
                            recvbuf, (int*)recvcnts,
                            (int*)rdispls, (MPI_Datatype *)(*recvtypes),
                            MPI_Comm_f2c(*comm) );
}
*/

void mpi_barrier_ ( MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Barrier( MPI_Comm_f2c(*comm) );
}

void mpi_bcast_ ( void *buffer, MPI_Fint *count, MPI_Fint *datatype,
                  MPI_Fint *root, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Bcast(buffer, (int)*count,
                        MPI_Type_f2c(*datatype), (int)*root,
                        MPI_Comm_f2c(*comm));
}

void mpi_exscan_ (void *sendbuf, void *recvbuf, MPI_Fint *count,
                      MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
                      MPI_Fint *__ierr )
{
    *__ierr = MPI_Exscan(sendbuf,recvbuf,
                         (int)*count, MPI_Type_f2c(*datatype),
                         MPI_Op_f2c(*op), MPI_Comm_f2c(*comm) );
}

void mpi_gather_ ( void *sendbuf, MPI_Fint *sendcnt, MPI_Fint *sendtype,
                   void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype,
                   MPI_Fint *root, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Gather(sendbuf, (int)*sendcnt,
                         MPI_Type_f2c(*sendtype), recvbuf,
                         (int)*recvcount, MPI_Type_f2c(*recvtype),
                         (int)*root, MPI_Comm_f2c(*comm));
}

void mpi_gatherv_ ( void *sendbuf, MPI_Fint *sendcnt, MPI_Fint *sendtype,
                    void *recvbuf, MPI_Fint *recvcnts, MPI_Fint *displs,
                    MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm,
                    MPI_Fint *__ierr )
{
    *__ierr = MPI_Gatherv(sendbuf, (int)*sendcnt,
                          MPI_Type_f2c(*sendtype), recvbuf,
                          (int*)recvcnts, (int*)displs,
                          MPI_Type_f2c(*recvtype), (int)*root,
                          MPI_Comm_f2c(*comm));
}

void mpi_reduce_scatter_ ( void *sendbuf, void *recvbuf,
                           MPI_Fint *recvcnts, MPI_Fint *datatype,
                           MPI_Fint *op, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Reduce_scatter(sendbuf,
                                 recvbuf, (int*)recvcnts,
                                 MPI_Type_f2c(*datatype), MPI_Op_f2c(*op),
                                 MPI_Comm_f2c(*comm));
}

/*void mpi_reduce_scatter_block_ ( void *sendbuf, void *recvbuf,
                                 MPI_Fint *recvcount, MPI_Fint *datatype,
                                 MPI_Fint *op, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Reduce_scatter_block(sendbuf,
                                 recvbuf, (int)*recvcount,
                                 MPI_Type_f2c(*datatype), 
				 MPI_Op_f2c(*op),
                                 MPI_Comm_f2c(*comm));
}*/

void mpi_reduce_ ( void *sendbuf, void *recvbuf, MPI_Fint *count,
                   MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root,
                   MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Reduce(sendbuf, recvbuf,
                         (int)*count, MPI_Type_f2c(*datatype),
                         MPI_Op_f2c(*op), (int)*root,
                         MPI_Comm_f2c(*comm));
}

/*void mpi_reduce_local_ ( void *inbuf, void *inoutbuf, MPI_Fint *count,
                   MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root,
                   MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Reduce_local(inbuf, inoutbuf,
                               (int)*count, MPI_Type_f2c(*datatype),
                               MPI_Op_f2c(*op));
}*/

void mpi_scan_ ( void *sendbuf, void *recvbuf, MPI_Fint *count,
                 MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
                 MPI_Fint *__ierr )
{
    *__ierr = MPI_Scan(sendbuf, recvbuf,
                       (int)*count, MPI_Type_f2c(*datatype),
                       MPI_Op_f2c(*op), MPI_Comm_f2c(*comm));
}

void mpi_scatter_ ( void *sendbuf, MPI_Fint *sendcnt, MPI_Fint *sendtype,
                    void *recvbuf, MPI_Fint *recvcnt, MPI_Fint *recvtype,
                    MPI_Fint *root, MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Scatter(sendbuf, (int)*sendcnt,
                          MPI_Type_f2c(*sendtype), recvbuf,
                          (int)*recvcnt, MPI_Type_f2c(*recvtype),
                          (int)*root, MPI_Comm_f2c(*comm));
}

void mpi_scatterv_ ( void *sendbuf, MPI_Fint *sendcnts,
                     MPI_Fint *displs, MPI_Fint *sendtype,
                     void *recvbuf, MPI_Fint *recvcnt, 
                     MPI_Fint *recvtype, MPI_Fint *root,
                     MPI_Fint *comm, MPI_Fint *__ierr )
{
    *__ierr = MPI_Scatterv(sendbuf, (int*)sendcnts, (int*)displs,
                           MPI_Type_f2c(*sendtype), recvbuf,
                           (int)*recvcnt, MPI_Type_f2c(*recvtype),
                           (int)*root, MPI_Comm_f2c(*comm) );
}
}

