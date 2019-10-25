// file: timer.cpp

#include <iostream.h>
#include "pcb.h"
#include <stdio.h>
#include "schedule.h"
#include "lock.h"
#include <stdlib.h>
#include "kernsem.h"
#include "sleeplis.h"

volatile char changeContext;
volatile unsigned stopPre;
volatile char callTimer;
void tick();

static volatile int timeCounter = defaultTimeSlice;
static volatile unsigned nonStopRun;
static volatile unsigned tsp;
static volatile unsigned tss;
static volatile unsigned tbp;

void interrupt timer(...) {
	if (!callTimer) {
		asm int 60h;
		tick();
		timeCounter = timeCounter - 1;
		KernelSem::sleepList.updateSleepList();

	}

	callTimer = 0;

	if (!nonStopRun || changeContext) {
		if (timeCounter == 0 || changeContext) {
			if (!stopPre) {
				changeContext = 0;
#ifndef BCC_BLOCK_IGNORE
				asm {
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}
#endif

				PCB::running->sp = tsp;
				PCB::running->ss = tss;
				PCB::running->bp = tbp;

				if ((PCB::running != PCB::idle)) {
					if (PCB::running->state == PCB::READY) {
						Scheduler::put(PCB::running);
					}
				}

				PCB::running = Scheduler::get();
				if (PCB::running == 0) {
					PCB::running = PCB::idle;
				}

				tsp = PCB::running->sp;
				tss = PCB::running->ss;
				tbp = PCB::running->bp;

				timeCounter = PCB::running->timeSlice;

				if (timeCounter > 0)
					nonStopRun = 0;
				else
					nonStopRun = 1;
#ifndef BCC_BLOCK_IGNORE
				asm {
					mov bp, tbp
					mov sp, tsp
					mov ss, tss
				}
#endif

				if (PCB::running != PCB::idle) {
					SOFT_LOCK
					asm sti;
					PCB::executeSignal();
					asm cli;
					SOFT_UNLOCK
				}

			} else {
				changeContext = 1;
			}
		}
	}

}
