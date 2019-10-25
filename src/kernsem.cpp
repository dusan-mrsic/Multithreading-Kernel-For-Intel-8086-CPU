// file: kernsem.cpp

#include "kernsem.h"
#include "lock.h"
#include "semque.h"
#include "SCHEDULE.H"
#include <stdio.h>

SleepList KernelSem::sleepList;

KernelSem::KernelSem(int i){

	value = i;
	blocked = new SemQueue();


}

KernelSem::~KernelSem(){
	delete blocked;
}

int KernelSem::val() const {
    return value;
}



int KernelSem::signal(int n){
	HARD_LOCK
	int i = 0;
	if(n < 0){
		HARD_UNLOCK
		return n;
	}else if(n == 0){
		if (value + 1 <= 0){
			char tempWaited;
			PCB* temp = blocked->get(&tempWaited);
			if(tempWaited){
				KernelSem::sleepList.removeNode(temp);
			}
			++value;
			temp->state = PCB::READY;
			temp->semStatus = 1;
			Scheduler::put(temp);
			HARD_UNLOCK
			return 0;
		}else{
			 ++value;
			 HARD_UNLOCK
			 return 0;
		}
	}else{
		while (n > 0){
			if(value + 1 <= 0){
				char tempWaited;
				PCB* temp = blocked->get(&tempWaited);
				if(tempWaited){
					KernelSem::sleepList.removeNode(temp);
				}
				temp->state = PCB::READY;
				++value;
				++i;
				temp->semStatus = 1;
				Scheduler::put(temp);
				n--;
			}else{
				++value;
				n--;
			}
		}
	}
	HARD_UNLOCK
	return i;
}

int KernelSem::wait(Time maxTimeToWait){
    HARD_LOCK
    if (--value < 0) {
        PCB::running->state = PCB::BLOCKED;
        blocked->insert(PCB::running, maxTimeToWait > 0);
        if (maxTimeToWait > 0){
        	KernelSem::sleepList.addNode(this,PCB::running,maxTimeToWait);
        }
        dispatch();

        HARD_UNLOCK
        return PCB::running->semStatus;
    }
    HARD_UNLOCK
    return 1;
}


