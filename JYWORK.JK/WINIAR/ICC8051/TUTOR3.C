
#pragma language=extended
#include <io51.h>

int result;
int rtime;

/* Example interrupt handler for the timer */
interrupt [11] void timer_int( void) 
  {
    rtime++;
    P0.1 ^= 1;			/* toggle port 0 bit */
  }

void do_foreground_process(void)
  {
    result++;
  }

void main(void)
  {
    TMOD &= 0xF0;		/* set timer mode */    
    TMOD |= 02;
    TH0 = 8;			/* interrupt rate*/                 
    TCON.4 = 1;			/* turn on timer */
    IE.1 = 1;			/* set timer interrupt flag */
    EA = 1;			/* enable the interrupts */
    while (1)
      { 
	do_foreground_process();
      }
  }
