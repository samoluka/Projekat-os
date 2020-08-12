#ifndef _myevent_h_
#define _myevent_h_

#include "event.h"
#include "PCB.h"

class KernelEv {
public:
    KernelEv(IVTNo ivtNo) {
        myThreadId = PCB::running->myId;
        val = 0;
        waiting = -1;
        ev = ivtNo;
        allSemEvent[curr++] = this;
    }
    ~KernelEv() {
    }
    void wait();
    void signal();
    static int maxSemEvent;
    static int curr;
    static KernelEv** allSemEvent;
    unsigned char ev;
private:
    unsigned int myThreadId;
    unsigned int waiting;
    short val;
   
};

#endif