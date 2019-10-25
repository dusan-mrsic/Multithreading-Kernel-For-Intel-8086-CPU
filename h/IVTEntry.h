#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "event.h"
#include "kernelev.h"

class IVTEntry {
public:


	IVTEntry(IVTNo num, void interrupt (*newRoutine)(...));

    ~IVTEntry();

    static IVTEntry *entries[256];

    static void Signal(IVTNo number);

    static void oldRoutineCall(IVTNo number);

private:
    friend class KernelEv;
    IVTNo entryNumber;
    void interrupt (*oldRoutine)(...);
    Event *event;

};


#endif
