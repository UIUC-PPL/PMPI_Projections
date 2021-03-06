#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define msg_size 256

double *workarray;
#define ARRSIZE 10240
#define NWORKITER 500
#define NITER 5

void do_work(){
  for(int iter = 0; iter<NWORKITER;iter++){
    for(int i=0;i<ARRSIZE;i++){
      workarray[i] = 0.5 * workarray[(i*7) % ARRSIZE] +  0.5 * workarray[(i*11) % ARRSIZE];
    }
  }
  assert(workarray[7] > 0.0);
}



int main(int argc, char ** argv){
	  int rank;            /* process id */
	  int np;                /* number of processes */

	  char message_s[msg_size], message_r[msg_size];  /* storage for the message */
	 
	  int *reduce_send_buf, *reduce_recv_buf;

	  MPI_Status status;    /* return status for receive */
	  	  
	  MPI_Request req_send;
	  MPI_Request req_recv;

	  MPI_Init( &argc, &argv );
	  
	  workarray = new double[ARRSIZE];
	  for(int i=0;i<ARRSIZE;i++)
	    workarray[i] = i;

	  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	  MPI_Comm_size( MPI_COMM_WORLD, &np );

	  
	  MPI_Barrier(MPI_COMM_WORLD); 


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

	  do_work();
	 
	  if(rank % 2 == 0) {
	    MPI_Isend(message_s, msg_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, &req_send);
	    MPI_Irecv(message_r, msg_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, &req_recv); 
	  } else {
	    MPI_Irecv(message_r, msg_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, &req_recv); 
	    MPI_Isend(message_s, msg_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, &req_send);
	  }

	  do_work();

	  MPI_Wait ( &req_send, &status);
	  
	  do_work();
 
	  MPI_Wait ( &req_recv, &status);

	  
	  MPI_Barrier(MPI_COMM_WORLD); 
	  
	  
	  do_work();

	  reduce_send_buf = (int*)malloc(5 * sizeof(int));
	  reduce_recv_buf = (int*)malloc(5 * sizeof(int));

	  for(int i=0;i<5;i++)
	    reduce_send_buf[i] = rank;

	  MPI_Allreduce((void*)reduce_send_buf, (void*)reduce_recv_buf, 5, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


	  do_work();

	  for(int i=0;i<5;i++){
	    assert(reduce_recv_buf[i] == (np*(np-1))/2);
	  }
	  

	  free(reduce_send_buf);
	  free(reduce_recv_buf);

	  MPI_Barrier(MPI_COMM_WORLD); 


	  do_work();


	  reduce_send_buf = (int*)malloc(5 * sizeof(int));
	  reduce_recv_buf = (int*)malloc(5 * sizeof(int));

	  for(int i=0;i<5;i++)
	    reduce_send_buf[i] = rank;

	  int root = 3;
	  MPI_Reduce((void*)reduce_send_buf, (void*)reduce_recv_buf, 5, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

	  
	  do_work();

	  if(root == rank){
	    for(int i=0;i<5;i++){
	      assert(reduce_recv_buf[i] == (np*(np-1))/2);
	    }
	  }

	  free(reduce_send_buf);
	  free(reduce_recv_buf);

	  MPI_Barrier(MPI_COMM_WORLD); 
	  printf("[%d] Program completed\n", rank);
	  MPI_Finalize();

	return 0;
}
