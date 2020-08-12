#include "myevent.h"
#include "thread.h"
#include "PCB.h"
#include "list.h"
#include "dlist.h"
#include "mysem.h"

// zabranjuje prekide
#define lock{\
 asm pushf;\
 asm cli;\
}

// dozvoljava prekide
#define unlock asm popf

int KernelEv::maxSemEvent = 500;
int KernelEv::curr = 0;
KernelEv** KernelEv::allSemEvent = new KernelEv*[KernelEv::maxSemEvent];

void KernelEv::wait() {
    lock;
    if (PCB::running->myId != this->myThreadId) {
        unlock;
        return;
    }
    if (this->val) {
        this->val = 0;
        unlock;
        return;
    }
    this->waiting = PCB::running->myId;
    PCB::running->active -= 1;
    PCB::running->blocked = 1;
    unlock;
    dispatch();
}
void KernelEv::signal() {
    lock;
    if (this->waiting == -1) {
        val = 1;
        return;
    }
    PCB::allPCB[this->myThreadId]->active+=1;
    PCB::allPCB[this->myThreadId]->blocked=0;
    //PCB::allPCB[this->myThreadId]->slept = 0;
    PCB::sleeping->del_by_id(this->myThreadId);
    if (PCB::allPCB[this->myThreadId]->waitingOn)
        PCB::allPCB[this->myThreadId]->waitingOn->waiting.delete_elem(this->myThreadId);
    Scheduler::put(PCB::allPCB[this->myThreadId]);
    this->waiting = -1;
    unlock;
    return;
}