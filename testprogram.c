#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "PMPI_ProjectionsLogging.h"

#define msg_size 256

int doWork() {
  int i, accum = 0;
  for (i = 0; i < 1000000; ++i) {
    accum += i;
    accum *= i - 2;
  }

  return accum;
}

int main(int argc, char ** argv){
  Projections_Init(1);
  printf("Initialized tracing\n");

  int eventNum =  register_EVENT("testEvent");
  printf("Registered event with index: %d\n", eventNum);

  write_EVENT(eventNum, 0);
  printf("Wrote event with index: %d\n", eventNum);
  
  int result = doWork();
  printf("Got result: %d (garbage)\n", result);

  write_EVENT(eventNum, 0);
  printf("Wrote event with index: %d\n", eventNum);

  Projections_Finalize();
  printf("Finalized tracing\n");

  return 0;
}
