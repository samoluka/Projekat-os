#include "thread.h"
#include "PCB.h"
#include "mysem.h"
#include "list.h"
#include "SCHEDULER.h"
#include "wt.h"
#include "dlist.h"
#include <FSTREAM.H>


unsigned tsp, tss;
volatile int cntr = defaultTimeSlice;
volatile int context_switch_on_demand = 0;
volatile int lockFlag = 1;
volatile int forceTimer = 0;
volatile int critical = 0;
volatile int oldCritical=0;

// zabranjuje prekide
#define lock{\
	asm pushf;\
	asm cli;\
}

// dozvoljava prekide
#define unlock{\
	asm popf;\
}

// nova prekidna rutina tajmera

void tick();

void wake_sleeping() {
	if (PCB::sleeping->empty())
		return;
	//cout <<"\nradim lover\n";
	PCB::sleeping->lower();
	//PCB::sleeping->print();
	while (PCB::sleeping->head != NULL && PCB::sleeping->head->value <=0) {
		List::Node* tmp = PCB::sleeping->head->my->head;
		while (tmp != NULL && PCB::allPCB[tmp->value] != NULL) {
			PCB::allPCB[tmp->value]->blocked = 0;
			PCB::allPCB[tmp->value]->slept = 0;
			PCB::allPCB[tmp->value]->waitingOn->waiting->delete_elem(tmp->value);
			PCB::allPCB[tmp->value]->waitingOn = NULL;
			Scheduler::put(PCB::allPCB[tmp->value]);
			tmp = tmp->next;
		}
		PCB::sleeping->del_head();
	}
}


void interrupt timer()
{
	//cout <<"Timer "<<context_switch_on_demand<<cntr<<lockFlag<<critical<<endl;
	if (context_switch_on_demand && forceTimer)
		wake_sleeping();
	if (!context_switch_on_demand) {
		tick();
		cntr--;
	}
	if (cntr == 0 || context_switch_on_demand)
	{
		if (!critical) {
			forceTimer = 0;
		
			asm{
				// cuva sp
				mov tsp, sp
				mov tss, ss
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;

			if (PCB::running->myId>0 && PCB::running->blocked == 0 && PCB::running->finished == 0 && PCB::running->slept == 0) 
				Scheduler::put(PCB::running);
			while(1){
				do{
					PCB::running = Scheduler::get();
					if (!PCB::running){
						PCB::running = PCB::allPCB[PCB::wt->getId()];
						break;
					}
				} while (PCB::running->blocked || PCB::running->finished || PCB::running->slept);
				// if (theEnd){
				// 	cout <<"The end odabrao nit: "<<PCB::running->myId<<endl;
				// }
				//cout <<"Nit:"<<PCB::running->myId<<endl;

				tsp = PCB::running->sp;
				tss = PCB::running->ss;

				cntr = PCB::running->quantum;

				asm{
					// restaurira sp
					mov sp, tsp
					mov ss, tss
				}

				oldCritical = critical;
				critical = 1;
				//asm pushf;
				asm std;
				PCB::running->doAllSignals();
				asm cli;
				critical = oldCritical;
				//asm popf;
				if (!PCB::running->finished) break;
			}

		}
		else {
			forceTimer = 1;
			if (cntr == 0) {
				cntr == 1;
			}
		}
	}
	if (!context_switch_on_demand) {
		asm int 60h;
		if (!critical)
			wake_sleeping();
	}
	context_switch_on_demand = 0;
}

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	myPCB = new PCB(timeSlice, stackSize, this);
	PCB::allPCB[myPCB->myId] = myPCB;
}
Thread::~Thread()
{
	PCB::allPCB[myPCB->myId] = NULL;
	delete myPCB;
}
void Thread::waitToComplete()
{
	if (myPCB->finished || !myPCB->started) {
		return;
	}
	PCB::running->blocked = 1;
	myPCB->waitForMe.insert(PCB::running->myId);
	dispatch();
}

void Thread::start()
{
	//cout <<"startujem nit: "<<myPCB->myId<<endl;
	myPCB->started = 1;
	Scheduler::put(this->myPCB);
}
Thread* Thread::getThreadById(ID id) {
	if (id>=PCB::id)
		return NULL;
	return PCB::allPCB[id]->myThread;
};
// sinhrona promena konteksta

void dispatch()
{
	//cout <<"dispatch "<<lockFlag<<" "<<cntr<<endl;
	lock
	context_switch_on_demand = 1;
	critical = 0;
	timer();
	unlock
}
ID Thread::getId()
{
	return myPCB->myId;
};
ID Thread::getRunningId() {
	return PCB::running->myId;
}


void Thread::signal(SignalId signal){
	myPCB->signal(signal);
}
void Thread::registerHandler(SignalId signal, SignalHandler handler){
	myPCB->registerHandler(signal,handler);
}
void Thread::unregisterAllHandlers(SignalId id){
	myPCB->unregisterAllHandlers(id);
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	myPCB->swap(id,hand1,hand2);
}
void Thread::blockSignal(SignalId signal){
	myPCB->blockSignal(signal);
}
void Thread::blockSignalGlobally(SignalId signal){
	PCB::blockSignalGlobally(signal);
}
void Thread::unblockSignal(SignalId signal){
	myPCB->unblockSignal(signal);
}
void Thread::unblockSignalGlobally(SignalId signal){
	PCB::unblockSignalGlobally(signal);
}