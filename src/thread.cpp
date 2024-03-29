#include "thread.h"
#include "pcb.h"
#include "lock.h"
#include <stdio.h>

Thread::Thread(StackSize stackSize, Time timeSlice){
	if(stackSize > maxStackSize) stackSize = maxStackSize;
	HARD_LOCK
	myPCB = new PCB(this,stackSize, timeSlice);
	HARD_UNLOCK
}

Thread::~Thread(){
	waitToComplete();
	HARD_LOCK
	delete myPCB;
	HARD_UNLOCK
}

void Thread::waitToComplete(){
    if(myPCB->state == PCB::FINISHED){
    		return;
    }else if(myPCB == PCB::idle){
    	return;
    }else{
   		HARD_LOCK
   	    PCB::running->state = PCB::BLOCKED;
   	    myPCB->blocked->insertElem(PCB::running);
   	    HARD_UNLOCK
		dispatch();
    }
}


ID Thread::getId(){
	return myPCB->getId();
}

ID Thread::getRunningId(){
	return ((PCB*)PCB::running)->getId();
}

Thread* Thread::getThreadById(ID id){
	return PCB::getThreadById(id);
}

void Thread::start(){
	myPCB->start();
}

//SIGNALS

void Thread::signal(SignalId signal){
	myPCB->signal(signal);
}
void Thread::registerHandler(SignalId signal, SignalHandler handler){
	myPCB->registerHandler(signal,handler);
}
void Thread::unregisterAllHandlers(SignalId id){
	myPCB->unregisterAllHandlers(id);
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	myPCB->swap(id,hand1,hand2);
}
void Thread::blockSignal(SignalId signal){
	myPCB->blockSignal(signal);
}
void Thread::blockSignalGlobally(SignalId signal){
	PCB::blockSignalGlobally(signal);
}
void Thread::unblockSignal(SignalId signal){
	myPCB->unblockSignal(signal);
}
void Thread::unblockSignalGlobally(SignalId signal){
	PCB::unblockSignalGlobally(signal);
}
