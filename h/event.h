#ifndef _event_h_
#define _event_h_
#include "ivtent.h"
typedef unsigned char IVTNo;

 #define PREPAREENTRY(n, c)\
void interrupt in##n(...);\
 IVTEntry k##n(n, in##n);\
 void interrupt in##n(...){\
 	k##n.signal();\
 	if (c == 1)\
 		k##n.callOld();\
 };


class KernelEv;

class Event {
public:
 Event (IVTNo ivtNo);
 ~Event ();
 void wait ();
protected:
 friend class KernelEv;
 void signal(); // can call KernelEv
private:
 KernelEv* myImpl;
};
#endif
