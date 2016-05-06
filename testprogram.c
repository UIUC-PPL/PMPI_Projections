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
  int numPEs = 2, i;
  if (argc > 1) {
    numPEs = atoi(argv[1]);
  }
  Prj_Init(numPEs);
  printf("Initialized tracing\n");

  int beginNum = Prj_register_event("beginEvent");
  printf("Registered event with index: %d\n", beginNum);

  int endNum = Prj_register_event("endEvent");
  printf("Registered event with index: %d\n", endNum);

  int computeEvent = Prj_register_event("computeEvent");
  printf("Registered event with index: %d\n", computeEvent);


  for (i = 0; i < numPEs; ++i) {
    Prj_add_event(beginNum, i);
    printf("Wrote event with index: %d\n", beginNum);

    doWork();
    long start_time = Prj_get_time();
    int result = doWork();
    printf("Got result: %d (garbage)\n", result);
    Prj_add_bracketed_event(computeEvent, start_time, i);
    printf("Wrote bracketed event with index: %d\n", computeEvent);
    doWork();

    Prj_add_event(endNum, i);
    printf("Wrote event with index: %d\n", endNum);
  }

  Prj_Finalize();
  printf("Finalized tracing\n");

  return 0;
}
