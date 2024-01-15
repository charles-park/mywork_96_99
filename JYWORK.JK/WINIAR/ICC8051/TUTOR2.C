#pragma language=extended	/* enable use of extended keywords */
#include <io51.h>		/* include sfr definitions for IO registers */
#include <stdio.h>		/* include C IO function prototypes */
#define EnableRx     (96)

/*********************************** 
*                                 * 
*        Start of code            * 
*                                 * 
***********************************/
char my_char;   
int call_count;

char read_char(void) 
  {  
    while (!RI);		/* RI indicates receive data */  
    RI=0;			/* reset receive  */
    my_char= SBUF;		/* return receive register */ 
    return(my_char);
  }

void do_foreground_process(void)
  {
    call_count++;
    putchar(my_char);
  }

void main(void)
  { 
    /* Initialize comms channel */ 
    TH1 = 0xF4;			/* the baud rate */
    TMOD = 96;			/* start timer */
    SCON = EnableRx ;		/* Control register */
   
    /* now loop forever, taking input when ready */  
    while (1)  
      {    
	if (read_char()) do_foreground_process();  
      } 
  }
