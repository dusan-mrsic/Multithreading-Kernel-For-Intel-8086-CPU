#include "kernelev.h"
#include "IVTEntry.h"
#include "lock.h"
#include "SCHEDULE.H"

KernelEv::KernelEv(IVTNo ivtno, Event* ev){
	pcb = PCB::running;
	IVTEntry::entries[ivtno]->event = ev;
	value = 0;
}

void KernelEv::wait(){
	if(PCB::running == pcb){
	    HARD_LOCK
	    if (value - 1 < 0) {
	    	value = value - 1;
	        PCB::running->state = PCB::BLOCKED;
	        dispatch();
	        HARD_UNLOCK
	        return;
	    }
	    HARD_UNLOCK
	}else{
		value = value - 1;
		return;
	}
}

void KernelEv::signal(){
    HARD_LOCK
	if(value >= 0) value = 1;
	else{
		pcb->state = PCB::READY;
		value = 0;
		Scheduler::put(pcb);
	}
    HARD_UNLOCK
    dispatch();
}
