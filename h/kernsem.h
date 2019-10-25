#ifndef _KERNSEM_H_
#define _KERNSEM_H_
#include "Queue.h"
#include "semque.h"
#include "sleeplis.h"


class KernelSem{
	public:
		KernelSem(int i);
		~KernelSem();

		int wait(Time maxTimeToWait);

		int signal(int n);

		int val() const;


	private:

		friend void interrupt timer(...);
		friend class SemList;
		friend class SleepList;
		friend class PCB;

		SemQueue *blocked;
		volatile int value;
		static SemList list;
	    static SleepList sleepList;

};

#endif
