#include "IVTEntry.h"
#include "event.h"
#include "kernelev.h"
#include "lock.h"
#include <dos.h>


IVTEntry *IVTEntry::entries[256];

IVTEntry::IVTEntry(IVTNo number, void interrupt (*newRoutine)(...)){
	entryNumber = number;
	event = 0;
    HARD_LOCK
#ifndef BCC_BLOCK_IGNORE
    oldRoutine = getvect(number);
    setvect(number, newRoutine);
#endif
    IVTEntry::entries[number] = this;

    HARD_UNLOCK
}

IVTEntry::~IVTEntry() {
	HARD_LOCK
#ifndef BCC_BLOCK_IGNORE
    setvect(entryNumber, oldRoutine);
#endif
	HARD_UNLOCK
}

void IVTEntry::Signal(IVTNo number) {
        IVTEntry::entries[number]->event->signal();
}

void IVTEntry::oldRoutineCall(IVTNo number) {
    IVTEntry::entries[number]->oldRoutine();
}

