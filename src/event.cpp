#include "event.h"
#include "myevent.h"

Event::Event(IVTNo ivtNo) {
    myImpl = new KernelEv(ivtNo);
}
Event::~Event() {
    delete myImpl;
}
void Event::wait() {
    myImpl->wait();
}

void Event::signal() {
    myImpl->signal();
} // can call KernelEv