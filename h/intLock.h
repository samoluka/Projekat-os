#ifndef _INTLOCK_H_
#define _INTLOCK_H_

#include "Thread.h"

extern int mutex_glb;
extern volatile int critical,forceTimer;
void mutex_glb_wait();



// #define lockcout() critical = 1;
// #define unlockcout() critical = 0; if(forceTimer) dispatch();

#define intLock mutex_glb_wait(); 
#define intUnlock mutex_glb = 1; 

#endif
