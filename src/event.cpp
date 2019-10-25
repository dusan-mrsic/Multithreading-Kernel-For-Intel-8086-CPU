#include "event.h"
#include "lock.h"
#include "kernelev.h"
#include "IVTEntry.h"

Event::Event(IVTNo ivtNo){
	HARD_LOCK
	myImpl = new KernelEv(ivtNo, this);
	HARD_UNLOCK
}

Event::~Event(){
	HARD_LOCK
	delete myImpl;
	HARD_UNLOCK
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}

