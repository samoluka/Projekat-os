#include "ivtent.h"
#include "myevent.h"
// #define prep(n, c)\
// void interrupt int##n(...);\
// IVTEntry k##n(n, int##n);\
// void interrupt int##n(...){\
// 	k##n.signal();\
// 	if (c == 1)\
// 		k##n.callOld();\
// };

void IVTEntry::signal() {
	if (KernelEv::allSemEvent[num] != NULL)
		KernelEv::allSemEvent[num]->signal();
}
void IVTEntry::callOld() {
	oldRout();
	//cout <<"bio si poziv koji se ne odbija"<<endl;
}
IVTEntry::IVTEntry(unsigned char numEntry, pInterrupt interruptRoutine) {
#ifndef BCC_BLOCK_IGNORE
	oldRout = getvect(numEntry);
	//oldRout je upravo tipa pInterrupt
	setvect(numEntry, interruptRoutine);
#endif
	num = numEntry;
}
IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE

	setvect(num,oldRout);
#endif
	oldRout();
}
