#include "PCB.h"
#include "dlist.h"
#include "wt.h"




int PCB::id = 0;
PCB **PCB::allPCB = new PCB*[300];
WaitingThread* PCB::wt = new WaitingThread();
DoubleList* PCB::sleeping = new DoubleList();
PCB* PCB::running = NULL;


void PCB::finish(){
    finished = 1;
    blocked = 1;
    List::Node* tmp = waitForMe.head;
    while (tmp) {
        allPCB[tmp->value]->blocked = 0;
        Scheduler::put(allPCB[tmp->value]);
        tmp = tmp->next;
    }
    if (creator != NULL)
        creator->signal(1);
    SignalHandlerList::Node* t = regSignalHandler[2].head;
    while(t){
        t->value();
        t=t->next;
    }
    //delete [] stack;
}

void PCB::wraper()
{
    if (running->myThread && !running->finished) {
        running->myThread->run();
        running->finish();
    }
    dispatch();
}
void PCB::initPCB()
{
    Thread* t = new Thread();
    // PCB::running = new PCB(defaultTimeSlice,defaultStackSize,NULL);
    // PCB::allPCB[1] = PCB::running;
    PCB::running = t->myPCB;
}
PCB::PCB(unsigned time, StackSize stackSize, Thread *thr)
{
    stackSize /= sizeof(unsigned);
    creator = running;
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
    for (int i = 0; i<16; i++)
        regSignalHandlerStatus[i]=1;
    if(creator != NULL){
        for(int j=0; j<16; j++){
            SignalHandlerList::Node* tmp = creator->regSignalHandler[j].head;
            while(tmp){
                regSignalHandler[j].insert(tmp->value);
                tmp=tmp->next;
            }
        }
    }
    stack[stackSize - 1] = 0x200;
    stack[stackSize - 2] = FP_SEG(PCB::wraper);
    stack[stackSize - 3] = FP_OFF(PCB::wraper);

    ss = FP_SEG(stack + stackSize - 12);
    sp = FP_OFF(stack + stackSize - 12);
    bp = sp; //po pravilu BP se na početku stavlja da pokazuje na SP
}


void PCB::doAllSignals(){
    List::Node* t = signals.head;
    while(t){
        int val = t->value;
        if (val == 0){
            finish();
            return;
        }
        t = t->next;
        SignalHandlerList::Node* tmp = regSignalHandler[val].head;
        if(regSignalHandlerStatus[val]){
            while(tmp){
                tmp->value();
                tmp = tmp->next;
            }
            signals.delete_elem(val);
        }
    }
}

void PCB::signal(SignalId signal){
    signals.insert(signal);
}
void PCB::registerHandler(SignalId signal, SignalHandler handler){
    regSignalHandler[signal].insert(handler);
}
void PCB::unregisterAllHandlers(SignalId id){
    while(regSignalHandler[id].head) regSignalHandler[id].del_head();
};
void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
    SignalHandlerList::Node* t1 = NULL;
    SignalHandlerList::Node* t2 = NULL;
    SignalHandlerList::Node* tmp = regSignalHandler[id].head;
    while(tmp){
        if (tmp->value == hand1)
            t1 = tmp;
        if (tmp->value == hand2)
            t2 = tmp;
        tmp = tmp->next;
    }
    if (!t1 || !t2)
        return;
    SignalHandler pom = t1->value;
    t1->value = t2->value;
    t2->value = pom;
    t1 = NULL;
    t2 = NULL;
    tmp = NULL;
    // delete t1;
    // delete t2;
    // delete tmp;
};
void PCB::blockSignal(SignalId signal){
    regSignalHandlerStatus[signal] = 0;
};
void PCB::blockSignalGlobally(SignalId signal){
    for(int i=0; i<PCB::id; i++)
        if (allPCB[i]!=NULL)
            allPCB[i]->blockSignal(signal);
};
void PCB::unblockSignal(SignalId signal){
    regSignalHandlerStatus[signal] = 1;
};
void PCB::unblockSignalGlobally(SignalId signal){
        for(int i=0; i<PCB::id; i++)
        if (allPCB[i]!=NULL)
            allPCB[i]->unblockSignal(signal);
};
