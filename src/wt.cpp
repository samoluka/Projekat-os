#include "wt.h"
#include <IOSTREAM.H>
#include "thread.h"
#include "PCB.h"

void WaitingThread::run() {
    while (1) {
        // cout <<"active blocked slept finished\n";
        // for (int i=2; i<PCB::id;i++) {
        //     Scheduler::put(PCB::allPCB[i]);
        //     cout <<"nit "<<i<<" je: "<<PCB::allPCB[i]->active<<PCB::allPCB[i]->blocked;
        //     cout <<PCB::allPCB[i]->slept<<PCB::allPCB[i]->finished;
        //     cout<<PCB::allPCB[i]->waitingOn<<endl;
        // }
        //cout <<endl;
        //PCB::sleeping->print();
        // Scheduler::put(PCB::allPCB[3]);
        // Scheduler::put(PCB::allPCB[2]);
        // Scheduler::put(PCB::allPCB[4]);
        //for (int i=0; i<1000000; i++);
        // asm sti
        //cout << "waiting";
        //Scheduler::put(PCB::allPCB[4]);
        //dispatch();
        // cout <<"waiting";
        // for (int i=0; i<10000; i++){
        //     i--;
        //     dispatch();
        // }
        //break;
        //context_switch_on_demand = 1;
        //unlock
        //dispatch();
    }
}