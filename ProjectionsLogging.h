// (c) 2009 Isaac Dooley



#include "generated-eventids.h"
typedef struct LogHandle
{
  int rank;
  FILE *outfile;
  char *flush_point;
  char *curr_buf_position;
  char *out_buf;
  long records_since_flush;
  int recentSourceLocation;
} LogHandle;

void ABT_init_tracing(int num_ranks);
void write_log_header(int rank);
void write_log_footer(int rank);
void write_EVENT_PAIR(int rank, int userEventID, long startTime);
//void write_EVENT_PAIR_Comm(int userEventID, long startTime,int,MPI_Datatype);
void write_EVENT(int rank, int userEventID);
void write_BEGIN_PROCESSING(int rank);
//void write_BEGIN_PROCESSING_AFTER_RECV(int source,int count,MPI_Datatype datatype);
void write_END_PROCESSING(int rank);
void add_BEGIN_PROCESSING_ABT(int rank, int tid);
void write_END_PROCESSING_ABT(int rank, int tid);
void write_BEGIN_IDLE(int rank);
void write_END_IDLE(int rank);
void write_USER_SUPPLIED(int rank, int value);
long time_us();
void ABT_init_trace(int rank);
int ABT_finalize_trace(int rank);
// This one is automatically generated and is in generated-stsEvents.C:
void generateStsEvents(FILE *stsfile);
