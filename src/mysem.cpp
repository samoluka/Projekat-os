#include "mysem.h"
#include "PCB.h"
#include "thread.h"

extern volatile int critical;
volatile int oldCS;

// zabranjuje prekide
#define lock{\
 asm pushf;\
 asm cli;\
 oldCS = critical;\
 critical = 1;\
}

// dozvoljava prekide
#define unlock{\
critical = oldCS;\
asm popf;\
}


KernelSem::KernelSem(int initValue) :value(initValue),waiting(new List()) {
};
KernelSem::~KernelSem(){
    delete waiting;
}
int KernelSem::wait(unsigned int time) {
    lock
    value--;
    if (value>=0) {
        unlock
        return 1;
    }
    PCB::running->for_wait = 0;
    PCB::allPCB[PCB::running->myId]->blocked = 1;
    waiting->insert(PCB::running->myId);
    PCB::running->waitingOn=this;
    if (time > 0) {
        //cout <<"nit spava:"<<PCB::running->myId<<endl;
        PCB::allPCB[PCB::running->myId]->slept = 1;
        PCB::sleeping->insert_sorted(time, PCB::running->myId);
    }
    //critical = 0;
    unlock
    dispatch();
    return PCB::running->for_wait;
}
int KernelSem::signal(int n) {
    lock
    if (n>0){
        value +=n;
    }else{
        value++;
        n = 1;
    }
    while (waiting->head && n>0){
        if (PCB::allPCB[waiting->head->value]->slept) {
            PCB::allPCB[waiting->head->value]->slept = 0;
            PCB::sleeping->del_by_id(waiting->head->value);
        }
        PCB::allPCB[waiting->head->value]->blocked=0;
        PCB::allPCB[waiting->head->value]->waitingOn = NULL;
        PCB::allPCB[waiting->head->value]->for_wait = 1;
        Scheduler::put(PCB::allPCB[waiting->head->value]);
        waiting->del_head();
        n--;
        //cout <<"budim nit:"<<waiting.head->value;
    }
    unlock
    return 0;
}