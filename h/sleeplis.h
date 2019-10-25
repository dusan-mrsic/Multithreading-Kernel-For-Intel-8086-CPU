#ifndef _SLEEPLIS_H_
#define _SLEEPLIS_H_
#include "semaphor.h"
#include "pcb.h"

class SleepList{
public:
	SleepList();
	~SleepList();

	void addNode(KernelSem *sem,PCB *pcb, Time timeToWait);
	void removeNode(PCB* pcb);
	void updateSleepList();


	struct Elem{
        KernelSem *sem;
        PCB *pcb;
        Time preostaloVreme;
        Elem *next;
	};


private:


	Elem* head;
	Elem* tail;

};

#endif
