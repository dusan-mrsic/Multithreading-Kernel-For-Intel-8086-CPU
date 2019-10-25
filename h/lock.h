#ifndef _LOCKS_H_
#define _LOCKS_H_

#include "timer.h"

#define HARD_LOCK asm pushf;\
             asm cli;
#define HARD_UNLOCK asm popf;

#define SOFT_LOCK ++stopPre;
#define SOFT_UNLOCK --stopPre;\
                    if (!stopPre && changeContext)\
                        dispatch();\
                    else ;

#endif
