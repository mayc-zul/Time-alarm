#ifndef _TIMER_H
#define _TIMER_H

#define TIMER_DELAY        1000

extern struct repeating_timer timer;
extern volatile bool timer_req;

bool TimerInit(int32_t);

#endif