#include "mysem.h"
#include "PCB.h"

// zabranjuje prekide
#define lock{\
 asm pushf;\
 asm cli;\
}

// dozvoljava prekide
#define unlock asm popf

KernelSem::KernelSem(int initValue) :value(initValue) {
};
int KernelSem::wait(unsigned int time) {
    lock
    if (value >=1) {
        value--;
        unlock
        return 1;
    }
    value--;
    if (time >0) {
       // cout <<"nit spava:"<<PCB::running->myId<<endl;
        PCB::sleeping->insert_sorted(time, PCB::running->myId);
        PCB::allPCB[PCB::running->myId]->slept = 1;
    }
    PCB::allPCB[PCB::running->myId]->blocked = 1;
    waiting.insert(PCB::running->myId);
    PCB::running->waitingOn=this;
    unlock
    dispatch();
    if (PCB::allPCB[PCB::running->myId]->slept) {
        PCB::allPCB[PCB::running->myId]->slept = 0;
        //cout <<"budim nit "<<PCB::running->myId<<endl;
        return 0;
    }
    //ako sam ovde vracam se iz signal
    return 1;
}
int KernelSem::signal(int n) {
    lock
    value++;
    int c = n>0?n:value;
    if (c<=0) c = 1;
    for (int i=0; i<c; i++) {
        if (!waiting.head)
            break;
        PCB::allPCB[waiting.head->value]->blocked=0;
        if (PCB::allPCB[waiting.head->value]->slept) {
            PCB::sleeping->del_by_id(waiting.head->value);
            PCB::allPCB[waiting.head->value]->slept = 0;
        }
        PCB::allPCB[waiting.head->value]->waitingOn = NULL;
        Scheduler::put(PCB::allPCB[waiting.head->value]);
        waiting.delete_elem(waiting.head->value);
    }
    unlock
    return 0;
}