#include "myevent.h"
#include "thread.h"
#include "PCB.h"
#include "list.h"
#include "dlist.h"
#include "mysem.h"

// zabranjuje prekide
#define lock{\
    oldCE = critical;\
    critical = 1;\
}

// dozvoljava prekide
#define unlock {\
    critical = oldCE;\
}

extern volatile int critical;
extern volatile int theEnd;
volatile int oldCE;

//int KernelEv::maxSemEvent = 250;
KernelEv** KernelEv::allSemEvent = new KernelEv*[256];

void KernelEv::wait() {
    lock;
    if (PCB::running->myId != this->myThreadId) {
        unlock;
        return;
    }
    if (this->val == 1) {
        this->val = 0;
        unlock;
        return;
    }
    this->waiting = PCB::running->myId;
    PCB::running->blocked = 1;
    //cout <<"nit ceka signal za event:"<<PCB::running->myId<<endl;
    unlock;
    dispatch();
}
void KernelEv::signal() {
    lock;
    if (this->waiting == -1) {
        this->val = 1;
        return;
    }
    if (PCB::allPCB[this->waiting]){
        PCB::allPCB[this->waiting]->blocked = 0;
        PCB::allPCB[this->waiting]->slept = 0;
        Scheduler::put(PCB::allPCB[this->waiting]);
    }
    this->waiting = -1;
    //cout <<"signal za event:"<<this->myThreadId<<theEnd<<endl;
    unlock;
    return;
}