#ifndef TIMER_DOT_H
#define TIMER_DOT_H
      
#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>

class Timer {
   timeb start_time;
   timeb temp;
public:
   Timer() { 
      ftime( &start_time );
   }
   int ms_since_start() {
      ftime( &temp );
      int s = temp.time - start_time.time;
      int ms = temp.millitm - start_time.millitm;
      return 1000*s + ms;
   }
};

#endif
