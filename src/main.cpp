// file: main.cpp

#include "lock.h"
#include "pcb.h"
#include <stdio.h>
#include<stdlib.h>
#include "SCHEDULE.H"
unsigned oldTimerOFF, oldTimerSEG;

int userMain(int argc, char *argv[]);


class Idle: public Thread {
public:
    Idle() : Thread() { }
protected:
    void run(){
    	printf("");
        while (1);
    };
};


#include <dos.h>
#include "timer.h"
static void interrupt (*oldTimerInterrupt)(...);

void inic() {
    HARD_LOCK
#ifndef BCC_BLOCK_IGNORE
    oldTimerInterrupt = getvect(0x8);
    setvect(0x8, timer);
    setvect(0x60, oldTimerInterrupt);
    HARD_UNLOCK
#endif
}

void restore() {
    HARD_LOCK
#ifndef BCC_BLOCK_IGNORE
    setvect(0x8, oldTimerInterrupt);
    HARD_UNLOCK
#endif
}



int main(int argc, char *argv[]) {
    inic();
    PCB::running = new PCB();
    Idle *idle = new Idle();
    PCB::idle = idle->myPCB;


    int ret = userMain(argc, argv);

    SOFT_LOCK
    delete idle;
    SOFT_UNLOCK
	restore();
    return ret;
}
