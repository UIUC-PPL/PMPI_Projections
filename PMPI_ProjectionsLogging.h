// (c) 2009 Isaac Dooley
#ifdef __cplusplus
extern "C" {
#endif
void Prj_Init(int numThreads);
void Prj_Finalize();

int Prj_register_event(const char* eventName);
void Prj_add_bracketed_event(int userEventID, long startTime, int threadIdx);
void Prj_add_event(int userEventID, int threadIdx);
long Prj_get_time();
#ifdef __cplusplus  
} // extern "C"  
#endif
