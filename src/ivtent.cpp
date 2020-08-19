#include "ivtent.h"


// #define prep(n, c)\
// void interrupt int##n(...);\
// IVTEntry k##n(n, int##n);\
// void interrupt int##n(...){\
// 	k##n.signal();\
// 	if (c == 1)\
// 		k##n.callOld();\
// };



void IVTEntry::callOld() {
    oldRout();
    //cout <<"bio si poziv koji se ne odbija"<<endl;
}