// (c) 2009 Isaac Dooley

#include <mpi.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "generated-eventids.h"

void write_log_header();
void write_log_footer();
void write_EVENT_PAIR(int userEventID, long startTime);
void write_EVENT(int userEventID);
void write_BEGIN_PROCESSING();
void write_END_PROCESSING();
void write_USER_SUPPLIED(int value);
long time_us();


// This one is automatically generated and is in generated-stsEvents.C:
void generateStsEvents(std::ofstream &stsfile);
