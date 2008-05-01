#include<mpi.h>
#include<stdio.h>

#define msg_size 1024

double *workarray;
#define ARRSIZE 1024
#define NWORKITER 2
#define NITER 5000

void do_work(){
  for(int iter = 0; iter<NITER;iter++){
    for(int i=0;i<ARRSIZE;i++){
      workarray[i] = 0.5 * workarray[(i*7) % ARRSIZE] +  0.5 * workarray[(i*11) % ARRSIZE];
    }
  }
}



int main(int argc, char ** argv){
	  int rank;            /* process id */
	  int np;                /* number of processes */

	  char message_s[msg_size], message_r[msg_size];  /* storage for the message */
	  
	  MPI_Status status;    /* return status for receive */
	  double startTime = 0;
	  	  
	  MPI_Init( &argc, &argv );
	  
	  workarray = new double[ARRSIZE];

	  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	  MPI_Comm_size( MPI_COMM_WORLD, &np );

	  
	  MPI_Barrier(MPI_COMM_WORLD); 

	  startTime = MPI_Wtime();

	  for(int i=0;i<NITER;i++){
	    if(rank % 2 == 0) {
	      MPI_Send(message_s, msg_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
	      MPI_Recv(message_r, msg_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, &status); 
	    } else {
	      MPI_Recv(message_r, msg_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, &status); 
	      MPI_Send(message_s, msg_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
	    }


	    do_work();
	  }

	  
	  MPI_Barrier(MPI_COMM_WORLD); 
	  
	  MPI_Finalize();

	return 0;
}
