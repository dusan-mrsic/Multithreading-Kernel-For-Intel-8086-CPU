#ifndef _SIGNLIST_H_
#define _SIGNLIST_H_

#include "thread.h"

class SignalList{
public:
	SignalList();
	~SignalList();
	void add(SignalHandler hand);
	void changePositions(SignalHandler hand1, SignalHandler hand2);
	void handlerCopy(SignalList* list);

private:
	friend class PCB;
	struct Elem{

		SignalHandler hand;
		Elem* next;

	};

	Elem* head;
	Elem* tail;

};

#endif
