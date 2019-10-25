//File: thread.h

#ifndef _THREAD_H_
#define _THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2;
typedef int ID;
const StackSize maxStackSize = 1UL << 16;  // 2^(16)B = 64KB

class PCB;

typedef void (*SignalHandler)();
typedef unsigned SignalId;

class Thread {
public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	//SIGNAL

	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);




protected:

	friend class PCB;

	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:

	friend int main(int argc, char *argv[]);
	PCB* myPCB;
};

void dispatch();

#endif
