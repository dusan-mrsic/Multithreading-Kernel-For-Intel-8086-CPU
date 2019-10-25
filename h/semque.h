//file: semque.h

#ifndef _SEMQUE_H_
#define _SEMQUE_H_
#include "pcb.h"

class SemQueue{
	public:
		SemQueue();
		~SemQueue();
		void insert(PCB* pcb, char tempWait);
		void remove(PCB* pcb);
		PCB* get(PCB* pcb);
		PCB* get(char *tempWait);
		int size();

	private:
		friend class KernelSem;
		friend class PCB;
		struct Elem{
			Elem(PCB* pcb, char wait);
			PCB* pcb;
			Elem *next;
			char WaitFlag;
		};
		Elem *head;
		Elem *tail;
	};

#endif
