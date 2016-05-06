// (c) 2009 Isaac Dooley

#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <execinfo.h>
#include <stdlib.h>

#include "PMPI_ProjectionsLogging.h"

#define OUTBUFSIZE 1024*1024*4

#ifdef __cplusplus
#define EXTERN_C extern "C" 
#else
#define EXTERN_C 
#endif

#define BEGIN_COMPUTATION  6
#define END_COMPUTATION  7

#define  USER_EVENT         13
/* Trace user supplied data */
#define USER_SUPPLIED       26

/* Trace user supplied note (text string)  */
#define USER_SUPPLIED_NOTE       28

/* Trace user supplied note (text string, with start, end times, and user event id)  */
#define USER_SUPPLIED_BRACKETED_NOTE       29

#define  USER_EVENT_PAIR    100


std::vector<long> records_since_flush;
std::vector<char *> out_buf;
std::vector<char *> curr_buf_position;
std::vector<char *> flush_point;

std::ofstream* outfile;

std::map<int,std::string> eventToName;
int currentEvent = 0;

double initTime;
int np;

double getAbsoluteTime() {
  struct timespec ts;
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts.tv_sec = mts.tv_sec;
  ts.tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_MONOTONIC, &ts);
#endif

  return ts.tv_sec + (ts.tv_nsec / 1000000000.0);

}

long Prj_get_time(){
  return ((getAbsoluteTime()-initTime)*1000000.0);
}

void writeSts(){
  std::ofstream stsfile("Proj.sts");
	
  stsfile << "PROJECTIONS_ID\n";
  stsfile << "VERSION 6.6\n";
  stsfile << "MACHINE Regular_Program\n";
  stsfile << "PROCESSORS " << np <<"\n";
  stsfile << "TOTAL_CHARES 1\n";
  stsfile << "TOTAL_EPS 0\n";
  stsfile << "TOTAL_MSGS 2\n";
  stsfile << "TOTAL_PSEUDOS 0\n";
  stsfile << "TOTAL_EVENTS " << eventToName.size() << "\n";
  stsfile << "CHARE 0 Pgm_Main\n";
  stsfile << "MESSAGE 0 0\n";
  stsfile << "MESSAGE 1 0\n";

  for (std::map<int, std::string>::iterator it = eventToName.begin(); it != eventToName.end(); ++it) {
    stsfile << "EVENT " << it->first << " " << it->second << "\n";
  }
  
  stsfile << "TOTAL_FUNCTIONS 0 \n";
  stsfile << "END\n";
	
  stsfile.close();
}

void inline writeToDisk(int threadIdx){
  outfile[threadIdx] << "PROJECTIONS-RECORD " << records_since_flush[threadIdx] << "\n"; // should only appear once in file???
  outfile[threadIdx] << out_buf[threadIdx];
  outfile[threadIdx].flush();
  curr_buf_position[threadIdx] = out_buf[threadIdx];
  out_buf[threadIdx][0] = '\0';
  records_since_flush[threadIdx] = 0;
}

/// Flush buffer if full
void inline flush(int threadIdx){
  if(curr_buf_position[threadIdx] > flush_point[threadIdx]){
    writeToDisk(threadIdx);
  }
}

void write_log_header(int threadIdx){
  sprintf(curr_buf_position[threadIdx], "%d 0\n", BEGIN_COMPUTATION);
  curr_buf_position[threadIdx] += strlen(curr_buf_position[threadIdx]); // Advance pointer to what we just wrote
  records_since_flush[threadIdx]++;
  flush(threadIdx);
}

void write_log_footer(int threadIdx){
  sprintf(curr_buf_position[threadIdx], "%d %ld\n", END_COMPUTATION, Prj_get_time());
  curr_buf_position[threadIdx] += strlen(curr_buf_position[threadIdx]); // Advance pointer to what we just wrote
  records_since_flush[threadIdx]++;
  flush(threadIdx);
}

int Prj_register_event(const char* eventName) {
  int event = currentEvent++;
  eventToName[event] = std::string(eventName);
  return event;
}

/// Write out the bracketed user event when it finishes
void Prj_add_bracketed_event(int userEventID, long startTime, int threadIdx){
  long endTime = Prj_get_time();
  int event = 0;
  int pe = threadIdx;
  sprintf(curr_buf_position[threadIdx], "%d %d %ld %d %d\n%d %d %ld %d %d\n", USER_EVENT_PAIR, userEventID, startTime, event, pe, USER_EVENT_PAIR, userEventID, endTime, event, pe);
  curr_buf_position[threadIdx] += strlen(curr_buf_position[threadIdx]); // Advance pointer to what we just wrote
  records_since_flush[threadIdx]++;
  records_since_flush[threadIdx]++;
  flush(threadIdx);
}

/// Write out a user event
void Prj_add_event(int userEventID, int threadIdx){
  long time = Prj_get_time();
  int event = 0;
  int pe = threadIdx;
  sprintf(curr_buf_position[threadIdx], "%d %d %ld %d %d\n", USER_EVENT, userEventID, time, event, pe);
  curr_buf_position[threadIdx] += strlen(curr_buf_position[threadIdx]); // Advance pointer to what we just wrote
  records_since_flush[threadIdx]++;
  flush(threadIdx);
}

void Prj_Init(int numThreads) {
  initTime = getAbsoluteTime();
  np = numThreads;
  
  records_since_flush.resize(np, 0);
  out_buf.resize(np);
  curr_buf_position.resize(np);
  flush_point.resize(np);

  outfile = new std::ofstream[np];

  for (int i = 0; i < np; ++i) {
    out_buf[i] = (char*)malloc(OUTBUFSIZE);
    assert(out_buf[i]);
    out_buf[i][0]='\0';
    curr_buf_position[i] = out_buf[i];
    flush_point[i] = out_buf[i] + OUTBUFSIZE - 400; // This should allow one
    // more record before
    // overflowing the buffer
		
    // open file
    char filename[1024];
    sprintf(filename, "Proj.%d.log", i);
    outfile[i].open(filename);

    write_log_header(i);
  }
}

void Prj_Finalize(void) {
  for (int i = 0; i < np; ++i) {
    write_log_footer(i);
    writeToDisk(i);
    outfile[i].close();
  }
  writeSts();
}
