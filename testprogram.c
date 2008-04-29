#include<mpi.h>
#include<stdio.h>

#define msg_size 1024


int main(int argc, char ** argv){
	  int rank;            /* process id */
	  int np;                /* number of processes */

	  char message_s[msg_size], message_r[msg_size];  /* storage for the message */
	  
	  MPI_Status status;    /* return status for receive */
	  double startTime = 0;
	  	  
	  MPI_Init( &argc, &argv );
	  
	  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	  MPI_Comm_size( MPI_COMM_WORLD, &np );
	
	  MPI_Barrier(MPI_COMM_WORLD); 

	  startTime = MPI_Wtime();

	  if(rank % 2 == 0) {
		  MPI_Send(message_s, msg_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
		  MPI_Recv(message_r, msg_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, &status); 
	  } else {
		  MPI_Recv(message_r, msg_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, &status); 
		  MPI_Send(message_s, msg_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
	  }

	  MPI_Barrier(MPI_COMM_WORLD); 
	  
	  MPI_Finalize();

	return 0;
}
