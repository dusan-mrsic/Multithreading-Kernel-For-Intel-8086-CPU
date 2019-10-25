#include <dos.h>
#include "pcb.h"
#include "Queue.h"
#include "SCHEDULE.H"
#include "timer.h"
#include "lock.h"
#include <stdio.h>
#include "semaphor.h"
#include "signlist.h"
#include <stdlib.h>

PCB* PCB::running;
PCB* PCB::idle;
List PCB::allPCBs;
ID PCB::nextID;
volatile unsigned PCB::globallyBlockedSlignals = 0xFFFF;

Semaphore sleepSemaphore(0);

PCB::PCB(){
	this->stack=0;
	bp=0;
	sp=0;
	ss=0;
	this->timeSlice=defaultTimeSlice;
	this->id = nextID++;
	this->state = PCB::READY;
	this->myThread = 0;
	this->blocked = new List();
	semStatus = 0;
	this->parentId = 0;

	for(int i = 0; i <16; i++){
		requestArray[i] = -1;
	}

	for(int j = 0; j <16; j++){
		handlers[j]=0;
	}
	handlers[0] = new SignalList();
	handlers[0]->add(PCB::killSignal);
	blockedSignals = 0xFFFF;
	PCB::allPCBs.insertElem(this);

}

PCB::PCB(Thread* thread,StackSize stackSize, Time time){

	id = nextID++;
	this->myThread=thread;
	this->timeSlice = time;
	state = PCB::NEW;
	this->blocked = new List();
	semStatus = 0;
	parentId = PCB::running->id;


	for(int i = 0; i <16; i++){
		if(PCB::running->handlers[i] == 0) this->handlers[i] = 0;
		else {
			handlers[i] = new SignalList();
			handlers[i]->handlerCopy(PCB::running->handlers[i]);
		}
	}
	blockedSignals = PCB::running->blockedSignals;

	for(int j = 0; j <16; j++){
		this->requestArray[j] = PCB::running->requestArray[j];
	}

	stackSize = stackSize / 2;
	stack = new unsigned[stackSize];
	bp=0;
	sp=0;
	ss=0;



#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = 0x200;
	stack[stackSize - 2] = FP_SEG(PCB::wrapper);
	stack[stackSize - 3] = FP_OFF(PCB::wrapper);
	ss = FP_SEG(stack + (stackSize - 12));
	sp = FP_OFF(stack + (stackSize - 12));
#endif
	 PCB::allPCBs.insertElem(this);

}

PCB::~PCB(){
	 PCB::allPCBs.removeElem(this);
	 delete blocked;
	 if (stack) delete[] stack;
}

void PCB::start() {
    HARD_LOCK
    if (state == PCB::NEW) {
        state = PCB::READY;
        Scheduler::put(this);
    }
    HARD_UNLOCK
}


ID PCB::getId() const { return id; }

Thread* PCB::getThreadById(ID Id){
	List::Elem* curr = PCB::allPCBs.head;
	while(curr != 0){
		if(curr->pcb->id == Id) return curr->pcb->myThread;
		curr = curr->next;
	}
	return 0;
}

PCB* PCB::getPCBid(ID id){
	List::Elem* curr = PCB::allPCBs.head;
	while(curr != 0){
		if(curr->pcb->id == id) return curr->pcb;
		curr = curr->next;
	}
	return 0;
}

void PCB::wrapper(){
	PCB::running->myThread->run();


	SOFT_LOCK

	PCB *temp = 0;
	while(1){
		temp = PCB::running->blocked->getElem();
		if(temp == 0) break;
		temp->state = PCB::READY;
	    Scheduler::put(temp);
    }

	PCB::running->state = PCB::FINISHED;

	unsigned mask = (1U << 2);
	if((PCB::running->blockedSignals & mask) && (globallyBlockedSlignals & mask)){
		if(running->handlers[2]){
			SignalList::Elem* tek = (running->handlers[2])->head;
			while(tek != 0){
				tek->hand();
				tek=tek->next;
			}
		}
	}

	PCB* tek = getPCBid(PCB::running->parentId);
	if(tek != 0){
		tek->signal(1);
	}

	SOFT_UNLOCK
	dispatch();
}


void dispatch() {
    if (stopPre)
        return;
    HARD_LOCK
	changeContext = 1;
    callTimer = 1;
    timer();
    HARD_UNLOCK
}

//SIGNAL

void PCB::signal(SignalId signal){

	int i = 0;
	if(!(signal < 16))
		return;
	SOFT_LOCK
	while((requestArray[i] != -1) && (i < 16)){
		i++;
	}
	requestArray[i] = signal;
	SOFT_UNLOCK

}
void PCB::registerHandler(SignalId signal, SignalHandler handler){
   // printf("\n USAO U RegHandler \n");
	if (!(signal < 16))
        return;
    SOFT_LOCK
    if(handlers[signal]==0){
    	handlers[signal] = new SignalList();
    }
    handlers[signal]->add(handler);
    SOFT_UNLOCK

}
void PCB::unregisterAllHandlers(SignalId id){

	handlers[id] = 0;

}
void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){

	if(handlers[id] == 0) return;
	SOFT_LOCK
	handlers[id]->changePositions(hand1,hand2);
	SOFT_UNLOCK

}
void PCB::blockSignal(SignalId signal){
	if (!(signal < 16))
        return;
    SOFT_LOCK
    blockedSignals &= ~(1U << signal);
    SOFT_UNLOCK

}

void PCB::blockSignalGlobally(SignalId signal){
  if (!(signal < 16))
        return;
    SOFT_LOCK
    globallyBlockedSlignals &= ~(1U << signal);

    SOFT_UNLOCK

}
void PCB::unblockSignal(SignalId signal){
  if (!(signal < 16))
        return;
    SOFT_LOCK
    blockedSignals |= (1U << signal);
    SOFT_UNLOCK
}
void PCB::unblockSignalGlobally(SignalId signal){
   if (!(signal < 16))
        return;
    SOFT_LOCK
    globallyBlockedSlignals |= (1U << signal);
    SOFT_UNLOCK
}

void PCB::executeSignal(){

	int i = 0;
	while((running->requestArray[i] != -1) && (i < 16)){
		unsigned mask = (1U << running->requestArray[i]);
		if((running->blockedSignals & mask) && (globallyBlockedSlignals & mask)){
			SignalList::Elem* tek = running->handlers[running->requestArray[i]]->head;
			if(running->handlers[running->requestArray[i]] != 0){
				while(tek != 0){
					tek->hand();
					tek=tek->next;
				}
			}
			running->requestArray[i] = -1;
		}
		i++;
	}

	int k = 0;
	int j = 0;
	for(;k < 16; k++){
		if(running->requestArray[k] != -1){
			running->requestArray[j] = running->requestArray[k];
			if( j != k){
				running->requestArray[k] = -1;
			}
			j++;
		}
	}

}

void PCB::killSignal(){
	PCB *temp = 0;
	//printf("\n USAO U KILL \n");
	while ((temp = PCB::running->blocked->getElem()) != 0) {
		temp->state = PCB::READY;
	    Scheduler::put(temp);
    }
	PCB::running->state = PCB::FINISHED;
	PCB* tek = getPCBid(PCB::running->parentId);
	if(tek != 0){
		tek->signal(1);
	}
	SOFT_UNLOCK
	dispatch();
}







