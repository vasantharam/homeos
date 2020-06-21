#ifndef __internal_i386_timer
#define __internal_i386_timer

#define HZ 100
/* Lets get the timer interrupt 
 * enabled for the scheduler. */
#define CLOCK_TICK_RATE 1193180
#define LATCH 1000*CLOCK_TICK_RATE/HZ

#endif
