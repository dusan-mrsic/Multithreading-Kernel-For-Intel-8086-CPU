#ifndef _TIMER_H_
#define _TIMER_H_

void interrupt timer(...);
extern volatile unsigned stopPre;
extern volatile char changeContext;
extern volatile char callTimer;

#endif
