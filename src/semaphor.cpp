#include "semaphor.h"
#include "lock.h"
#include "kernsem.h"
#include <stdio.h>

Semaphore::Semaphore(int init){
	HARD_LOCK;
	myImpl = new KernelSem(init);
	HARD_UNLOCK;
}

Semaphore::~Semaphore(){
	HARD_LOCK;
	delete myImpl;
	HARD_UNLOCK;
}

int Semaphore::wait(Time maxTimeToWait){
	int ret = 0;
	ret = myImpl->wait(maxTimeToWait);
	return ret;
}

int Semaphore::signal(int n){
	int k = 0;
	k = myImpl->signal(n);
	return k;
}

int Semaphore::val() const{
	int Value = 0;
	Value = myImpl->val();
	return Value;
}



