// (c) 2009 Isaac Dooley
#ifdef __cplusplus
extern "C" {
#endif
void Projections_Init(int numThreads);
void Projections_Finalize();

int register_EVENT(const char* eventName);
void write_EVENT_PAIR(int userEventID, long startTime, int threadIdx);
void write_EVENT(int userEventID, int threadIdx);
#ifdef __cplusplus  
} // extern "C"  
#endif
