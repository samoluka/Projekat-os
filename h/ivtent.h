#ifndef _ivtent_h_
#define _ivtent_h_
#include "myevent.h"

typedef void interrupt(*pInterrupt)(...);



class IVTEntry {
public:
    IVTEntry(unsigned char numEntry, pInterrupt interruptRoutine) {
        oldRout = getvect(numEntry);
        //oldRout je upravo tipa pInterrupt
        setvect(numEntry, interruptRoutine);
        num = numEntry;
    }
    ~IVTEntry(){
        setvect(num,oldRout);
        oldRout();
    }
    void signal() {
        // for (int i=0; i<KernelEv::curr; i++) {
        //     if (KernelEv::allSemEvent[i]->ev == num)
        //         KernelEv::allSemEvent[i]->signal();
        if (KernelEv::allSemEvent[num] != NULL)
            KernelEv::allSemEvent[num]->signal();
    }
    void callOld();
private:
    pInterrupt oldRout;
    unsigned char num;
};

#endif