#ifndef _wt_h_
#define _wt_h_

#include "thread.h"

class WaitingThread : public Thread
{
public:

    WaitingThread() : Thread(defaultStackSize,1) {
        //cout <<getId();
    }

    ~WaitingThread()
    {
        waitToComplete();
    }
protected:

    void run();

};

#endif