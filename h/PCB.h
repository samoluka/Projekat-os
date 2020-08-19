#ifndef _PCB_H_
#define _PCB_H_
#include "list.h"
#include "thread.h"
#include "DOS.H"
#include "SCHEDULE.H"
#include "dlist.h"
#include "semaphor.h"

class WaitingThread;



class PCB
{
public:
    static  PCB** allPCB;
    static  PCB *running;
    static  WaitingThread* wt;
    static  DoubleList* sleeping;
    static  int id;
    unsigned *stack;
    unsigned ss;
    unsigned sp;
    unsigned bp;
    unsigned timeslice;
    KernelSem* waitingOn;
    int active;
    int quantum;
    int myId;
    volatile int finished;
    volatile int blocked;
    volatile int started;
    volatile int for_wait;
    volatile int slept;

    Thread *myThread;
    List waitForMe;

    static void wraper()
    {
        if (running->myThread && !running->finished) {
            running->myThread->run();
            running->finished = 1;
            running->blocked = 1;
            //sleeping->del_by_id(running->myId);
        }
        if (!running->waitForMe.empty()) {
            List::Node* tmp = running->waitForMe.head;
            while (tmp) {
                //if (tmp->value<id && tmp->value >0) {
                allPCB[tmp->value]->blocked = 0;
                Scheduler::put(allPCB[tmp->value]);
                tmp = tmp->next;
            }
        }
        dispatch();
    }
    static void initPCB() {
        // PCB::running = PCB::allPCB[1];
        PCB::running = new PCB(defaultTimeSlice,defaultStackSize,NULL);
        //cout <<PCB::running->myId<<endl;
        // PCB::running->myId = 1;
        // PCB::running->active = 1;
        // PCB::running->slept = 0;
        // PCB::running->quantum = defaultTimeSlice;
        // PCB::running->blocked = 0;
        // PCB::running->finished=0;
        // PCB::running->stack = new unsigned[defaultStackSize];
        // PCB::running->stack[defaultStackSize - 1]= 0x200;
        // PCB::running->ss = FP_SEG(PCB::running->stack + defaultStackSize - 12);
        // PCB::running->sp = FP_OFF(PCB::running->stack + defaultStackSize - 12);
        // PCB::running->waitForMe = *new List();
        // PCB::running->started = 1;
        // PCB::running->myThread = NULL;
        PCB::allPCB[1] = PCB::running;
      // id++;
    }
    PCB(unsigned time, StackSize stackSize, Thread *thr)
    {
        stackSize /= sizeof(unsigned);
        myId = id++;
        myThread = thr;
        quantum = time;
        active = 1;
        started = 0;
        for_wait = 0;
        waitingOn= NULL;
        //waitForMe = new List();
        finished = 0;
        blocked = 0;
        slept = 0;
        stack = new unsigned[stackSize];
        stack[stackSize - 1] = 0x200;
        stack[stackSize - 2] = FP_SEG(PCB::wraper);
        stack[stackSize - 3] = FP_OFF(PCB::wraper);

        ss = FP_SEG(stack + stackSize - 12);
        sp = FP_OFF(stack + stackSize - 12);
        bp = sp; //po pravilu BP se na početku stavlja da pokazuje na SP
    }


    ~PCB(){ 
        delete[] stack;
    }
};

#endif