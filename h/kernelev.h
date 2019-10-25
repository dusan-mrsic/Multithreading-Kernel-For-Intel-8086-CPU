#ifndef _KERNELEV_H_
#define _KERNELEV_H_

#include "event.h"
#include "pcb.h"
#include "kernelev.h"

class PCB;

class KernelEv{
public:

	KernelEv(IVTNo ivtno, Event* ev);

	void wait();
	void signal();

private:

	friend class IVTEntry;

	PCB* pcb;
	volatile char value;


};

#endif
