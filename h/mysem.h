#ifndef _mysem_h_
#define _mysem_h_
#include "list.h"
class KernelSem {
public:
    KernelSem(int initValue);
    ~KernelSem() {};
    int wait(unsigned int time);
    int signal(int n);
    int value;
    List waiting;
};

#endif