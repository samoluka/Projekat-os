#ifndef _ivtent_h_
#define _ivtent_h_


typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
	IVTEntry(unsigned char numEntry, pInterrupt interruptRoutine);
	~IVTEntry();
	void signal() ;
	void callOld();
private:
	pInterrupt oldRout;
	unsigned char num;
};

#endif
