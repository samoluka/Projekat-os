#ifndef _PCB_H_
#define _PCB_H_
#include "list.h"
#include "thread.h"
#include "DOS.H"
#include "SCHEDULE.H"
#include "dlist.h"
#include "semaphor.h"
#include "slist.h"

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

    SignalHandlerList regSignalHandler[16];
    short regSignalHandlerStatus[16];
    Thread *myThread;
    PCB* creator;
    List waitForMe;
    List signals;
    

    static void wraper();
    static void initPCB();
    PCB(unsigned time, StackSize stackSize, Thread *thr);

    void finish();

    void doAllSignals();
    void signal(SignalId signal);
    void registerHandler(SignalId signal, SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
    void blockSignal(SignalId signal);
    static void blockSignalGlobally(SignalId signal);
    void unblockSignal(SignalId signal);
    static void unblockSignalGlobally(SignalId signal);


    ~PCB(){ 
        delete[] stack;
    }
};

#endif