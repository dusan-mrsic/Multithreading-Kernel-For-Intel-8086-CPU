#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "pcb.h"

class PCB;

class List{
	public:
		List();
		~List();
		void insertElem(PCB* pcb);
		void removeElem(PCB* pcb);
		PCB* getElem();
		int size();

	private:
		friend class PCB;
		struct Elem{
			Elem(PCB* pcb);
			PCB* pcb;
			Elem *next;
		};
		Elem *head;
		Elem *tail;
	};

#endif
