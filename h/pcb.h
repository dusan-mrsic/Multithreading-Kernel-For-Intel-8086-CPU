// File: pcb.h

#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "Queue.h"
#include "signlist.h"


#define CPU_FLAGS_INIT_VALUE 0x0200;
class List;

class PCB {

public:

	static PCB* running;
	static PCB* idle;
	static List allPCBs;
	static ID nextID;
	static Thread *getThreadById(ID Id);
	ID getId() const;
	ID parentId;
	static PCB* getPCBid(ID id);

	enum State { NEW, READY, BLOCKED, PAUSED, FINISHED};


	PCB();
	PCB(Thread* thread,StackSize size, Time time);
	~PCB();
	void start();

    //SIGNAL

	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
	static void executeSignal();
	static void killSignal();


protected:

private:

	friend void interrupt timer(...);
	friend class List;
	friend class KernelSem;
	friend class SemQueue;
	friend class SemList;
	friend class KernelEv;
	friend class SleepList;
	friend class Thread;

	friend int main(int argc, char *argv[]);

	static void wrapper();


	ID id;
	Thread* myThread;
	unsigned *stack;            // pointer to stack
	unsigned bp;				// base pointer
	unsigned sp;				// stack pointer
	unsigned ss;				// stack segment
	char semStatus;
	Time timeSlice;				// time slice
	List* blocked;
	volatile State state;
	SignalList* handlers[16];

	int requestArray[16];

	unsigned blockedSignals;
	static volatile unsigned globallyBlockedSlignals;


};

#endif
