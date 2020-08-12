#include "thread.h"
#include "PCB.h"
#include "mysem.h"
#include "list.h"
#include "SCHEDULER.h"
#include "wt.h"
#include "dlist.h"
#include <FSTREAM.H>

int PCB::id = 0;
const int maxPCB = 500;
PCB **PCB::allPCB = new PCB*[maxPCB];
WaitingThread* PCB::wt = new WaitingThread();
DoubleList* PCB::sleeping = new DoubleList();
unsigned tsp, tss;
volatile int cntr = defaultTimeSlice;
volatile int context_switch_on_demand = 0;
volatile int lockFlag = 1;
volatile int forceTimer = 0;
volatile int pocetniforce=0;
int br = 0;
//ofstream ff[100];

// zabranjuje prekide
#define lock{\
 asm pushf;\
 asm cli;\
}

// dozvoljava prekide
#define unlock asm popf

// nova prekidna rutina tajmera
void tick();

void wake_sleeping() {
	if (PCB::sleeping->empty())
		return;
	//cout <<"\nradim lover\n";
	PCB::sleeping->lower();
	if (PCB::sleeping->head->value <= 0) {
		List::Node* tmp = PCB::sleeping->head->my->head;
		while (tmp) {
			//if (tmp->value<PCB::id && tmp->value>0) {
			PCB::allPCB[tmp->value]->blocked = 0;
			// waiting.delete_elem(waiting.head->value);
			PCB::allPCB[tmp->value]->waitingOn->waiting.delete_elem(tmp->value);
			Scheduler::put(PCB::allPCB[tmp->value]);
			//cout <<"budim nit "<<tmp->value<<endl;
			//}
			tmp = tmp->next;
		}
		PCB::sleeping->del_head();
	}
}


void interrupt timer()
{
	pocetniforce = forceTimer;
	if (!context_switch_on_demand || forceTimer) {
		//wake_sleeping();
		cntr--;
	}
	if (cntr == 0 || context_switch_on_demand)
	{
		if (lockFlag) {
			forceTimer = 0;
			asm{
				// cuva sp
				mov tsp, sp
				mov tss, ss
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;

			if (PCB::running->myId>0)
				Scheduler::put(PCB::running);
			do
			{
				PCB::running = Scheduler::get();
				if (!PCB::running && PCB::running->myId!=1)
				{
					PCB::running = PCB::allPCB[PCB::wt->getId()];
					break;
				}
			} while (PCB::running->active<=0 || PCB::running->blocked || PCB::running->finished);

			tsp = PCB::running->sp;
			tss = PCB::running->ss;

			cntr = PCB::running->quantum;

			asm{
				// restaurira sp
				mov sp, tsp
				mov ss, tss
			}
		}
		else {
			forceTimer = 1;
			if (cntr == 0) {
				cntr == 1;
			}
		}
	}
	if (!context_switch_on_demand || forceTimer) {
		asm int 60h;
		tick();
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
	PCB::running->active-=1;
	myPCB->waitForMe.insert(PCB::running->myId);
	dispatch();
	//PCB::running->active+=1;
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