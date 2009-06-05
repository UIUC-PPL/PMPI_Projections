#include <stdio.h>
#include <assert.h>
#include <execinfo.h>

#if __USE_STACK_TRACE_TO_IDENTIFY_TASKS__

#include "source_location.h"


/**                                                                                                                              
    Mangle together the return pointers from all stack frames to get a 32-bit integer                                            
    likely to be a unique for each point in the user's code.                                                                     
    I am not sure how likely collisions are using this method.                                                                     
    Note, some mpicxx compilers will destroy this function, so it is in this file by itself to be compiled with g++

*/

unsigned long source_location_ulong(){
  unsigned long long s = 0;
  void* callstack[128];
  int i, frames = backtrace(callstack, 128);
  if(frames > 0){
    s = (unsigned long long)callstack[0];
    for (i = 1; i < frames; ++i) {
      s = s ^ (unsigned long long)callstack[i];
    }
  }


  // Drop this down to an unsigned long      

  unsigned long s_ul = (unsigned long)(s & 0xFFFFFFFF) ^  (unsigned long)((s>>32) & 0xFFFFFFFF);

  return s_ul;
}

int source_location_int(){
  long long s = 0;
  void* callstack[128];
  int i, frames = backtrace(callstack, 128);
  if(frames > 0){
    s = (long long)callstack[0];
    for (i = 1; i < frames; ++i) {
      s = s ^ (long long)callstack[i];
    }
  }

  // Drop this down to an unsigned long

  int s_int = (int)(s & 0xFFFFFFFF) ^  (int)((s>>32) & 0xFFFFFFFF);

  //  printf("%d\n", s_int);
  return s_int;
}


#else

int source_location_int(){
  return 0;
}

unsigned long source_location_ulong(){
  return 0;
}



#endif
