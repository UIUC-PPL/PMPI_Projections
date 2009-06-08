// (c) 2009 Isaac Dooley

#include <mpi.h>
#include "generated-eventids.h"

void write_log_header();
void write_log_footer();
void write_EVENT_PAIR(int userEventID, long startTime);
void write_EVENT(int userEventID);
void write_BEGIN_PROCESSING();
void write_END_PROCESSING();
void write_USER_SUPPLIED(int value);
long time_us();

