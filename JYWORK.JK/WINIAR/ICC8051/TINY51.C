/*				- TINY51.C -

	    #################################################
	    #						    #
	    #    Ladies and gentlemen!			    #
	    #						    #
	    #    Welcome to the world of high-performance,  #
	    #    high-quality and high-productivity system  #
	    #    software.				    #
	    #						    #
	    #		     IAR Systems AB		    #
	    #		     P.O. Box 23051		    #
	    #		S-750 23 Uppsala, SWEDEN	    #
	    #						    #
	    #################################################

   This is the IAR real time kernel for the 8051 micro controller.

   Created: --/May/92		J. Warta
   Revised: 14/Dec/92		IANB	[Comments and bugfixes]

   Version: 1.00
     
   (c) Copyright IAR Systems 1992

*/

#include <io51.h>
#include "tiny51.h"

struct TASK IdleTask = {
	&IdleTask,
	(byte)PID_IDLETASK,
	(byte)0,
	(byte)0,
	(byte)0,

	TS_RUNNING,

	0,
	0,
	0
};

data struct TASK TimeoutTask;
data struct TASK *ActTaskPtr = &IdleTask;

idata byte ttstack[TT_STACKSIZE];

byte IdleRegs[8];			/* memory for register bank 0	*/
					/* for idle task		*/
extern bit HaltDispatch;

struct TASK *ptr;

extern byte timeoutcnt;
extern byte dispatchcnt;

 
/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	StartDispatcher					*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Initialize dispatcher environment.			*/
/*									*/
/*----------------------------------------------------------------------*/
void StartDispatcher (byte mode)
  {
    timeoutcnt = TIMEOUT_DELAY;
    dispatchcnt = DISPATCH_DELAY;
    HaltDispatch = TRUE;

    TimeoutTask.pid = PID_TIMEOUT;	

    TimeoutTask.sp = ttstack;

    TimeoutTask.pushfunc = 0;
    TimeoutTask.popfunc = 0;
    create(&TimeoutTask, TimeoutHandler);


    /* initialize push- and pop-function for idle task */

    IdleTask.pushfunc = GetFunctionAddress(StoreIdleRegs);
    IdleTask.popfunc = GetFunctionAddress(RestoreIdleRegs); 

    if (mode == TINY51_PREEMPTIVE)
      {
	EA = 0;		/* all interrupts disable		*/

	TMOD |= 0x02;	/* load timer 0 for '8-bit auto-reload' */
	TH0 = TICKER_RATE; /* load reload value for timer 0	*/
	TR0 = 1;	/* start timer 0			*/

	ET0 = 1;	/* enable overflow interrupt for timer 0*/
	EA = 1;		/* enable dispatcher			*/
      }
    /* and start dispatcher */

    HaltDispatch = FALSE;
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	StopDispatcher					*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Reset dispatcher environment.			*/
/*									*/
/*----------------------------------------------------------------------*/
void StopDispatcher (byte mode)
  {
    HaltDispatch = TRUE;
    TR0 = 0;		/* disable interrupt for timer 0	*/
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	create						*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Insert task in task list.				*/
/*									*/
/*----------------------------------------------------------------------*/
int create (struct TASK *task, void (*taskfunc)())
  {
    HaltDispatch = TRUE;
    ptr = ActTaskPtr;

    /* search for equal task pid */

    do
      {
	if (ptr->pid == task->pid)
	  {
	    HaltDispatch = FALSE;
	    return -1;
	  }
	ptr = ptr->nextptr;
      }
    while (ptr != ActTaskPtr);

    task->state = TS_READY;
    task->wait_signals = task->rec_signals = 0;
    task->timeout = 0;

    /* chain new task in task list */

    task->nextptr = ActTaskPtr->nextptr;
    ActTaskPtr->nextptr = task;

    /* build task stack */

    *(++task->sp) = ((word)GetFunctionAddress(taskfunc))&0xFF;
    *(++task->sp) = ((word)GetFunctionAddress(taskfunc))>>8;
    *(++task->sp) = 0x00;	/* PSW, register bank 0	*/
    *(++task->sp) = 0;	/* acc 			*/
    *(++task->sp) = 0;	/* b   			*/
    *(++task->sp) = 0;	/* dpl 			*/
    *(++task->sp) = 0;	/* dph 			*/

    /* initialize pushfunction before first task call */

    if (task->pushfunc)
      (*task->pushfunc)();

    /* modify indirect function call for faster access */

    if (task->pushfunc)
      task->pushfunc = GetFunctionAddress(task->pushfunc);

    if (task->popfunc)
      task->popfunc = GetFunctionAddress(task->popfunc);
    HaltDispatch = FALSE;
    return 0;
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	signal						*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Send signal to task.				*/
/*									*/
/*----------------------------------------------------------------------*/
int signal (byte tasknr, word signalnr)
  {
    HaltDispatch = TRUE;
    ptr = ActTaskPtr;

    if (tasknr == PID_NEXT)
      {
	do
	  {	/* search for signal */
	    if ( (ptr->state == TS_WAITING) &&
	         (ptr->wait_signals & signalnr) )
	      {
		ptr->rec_signals |= signalnr;
		ptr->state = TS_READY;

		HaltDispatch = FALSE;
		return 0;
	      }
	    ptr = ptr->nextptr;
	  }
	while (ptr != ActTaskPtr);
      }
    else
      {
	do
	  {	/* search for pid */
	    if (ptr->pid == tasknr)
	      {
		if (ptr->state != TS_WAITING)
		  {
		    HaltDispatch = FALSE;
		    return -1;
		  }

		/* send signal to task		*/

		ptr->rec_signals |= signalnr;

		/* and if all signals received	*/
		/* set task to ready state	*/

		if (ptr->wait_signals & ptr->rec_signals)
		  ptr->state = TS_READY;

		HaltDispatch = FALSE;
		return 0;
	      }
	    ptr = ptr->nextptr;
	  }
	while (ptr != ActTaskPtr);
      }
    HaltDispatch = FALSE;
    return -1;
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	wait						*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Wait for signal.					*/
/*									*/
/*----------------------------------------------------------------------*/
byte wait (byte signalnr)
  {
    byte rec_signals;
    
    HaltDispatch = TRUE;
    
    if (signalnr == SIG_CPU)
      {
      
	/* give up CPU 		*/

	ScheduleNextTask();
	return 0;
      }
    else
      {
	/* wait for signal	*/
	
	ActTaskPtr->wait_signals = signalnr;
	ActTaskPtr->state = TS_WAITING;
	
	/* schedule next task	*/
	
	ScheduleNextTask();
	
	/* task is wake up	*/
	
	HaltDispatch = TRUE;
	
	/* clear received signal */
	
	rec_signals = ActTaskPtr->rec_signals;
	ActTaskPtr->rec_signals = 0;
	HaltDispatch = FALSE;
	
	/* return received signal */
	
	return rec_signals;
      }
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	timeout						*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Special task, generates timeout signal.		*/
/*									*/
/*----------------------------------------------------------------------*/
void timeout (byte timeoutval)
  {
    HaltDispatch = TRUE;

    /* sets only timeout counter of actual task */
    
    ActTaskPtr->timeout = timeoutval;
    
    HaltDispatch = FALSE;
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	up						*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Semaphore operation to leave critical section.	*/
/*									*/
/*----------------------------------------------------------------------*/
void up (semaphore *s)
  {
    HaltDispatch = TRUE;
    (*s)++;
    signal(PID_NEXT,SIG_SEMAPHORE);
    wait(SIG_CPU);
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	down						*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Semaphore operation to enter critical section.	*/
/*									*/
/*----------------------------------------------------------------------*/
void down (semaphore *s)
  {
    HaltDispatch = TRUE;
    if (*s == 0)
      wait(SIG_SEMAPHORE);
    (*s)--;
    HaltDispatch = FALSE;
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	TimeoutHandler					*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised: 09/Dec/92  IANB	Task is ready only when the	*/
/*					timeout counter is zero.	*/
/*									*/
/*									*/
/*	Description: Special task for timeout handling.	The task is	*/
/*		     entered with pointer to itself, so pid is zero. 	*/
/*		     End of task ring is detected by pid == 0.		*/
/*									*/
/*----------------------------------------------------------------------*/
void TimeoutHandler (void)
  {
    struct TASK *ptr;

    FOREVER
      {
	ptr = ActTaskPtr;
	do
	  {
	    if ((ptr->state == TS_WAITING) && (ptr->timeout))
	      {
		if (--ptr->timeout == 0)
		  {
		    ptr->rec_signals = SIG_TIMEOUT;
		    ptr->state = TS_READY;
		  }
	      }
	    ptr = ptr->nextptr;
	  }
	while (ptr->pid);
	wait(SIG_CPU);
      }
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	StoreIdleRegs					*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Store all registers for the idle task.		*/
/*									*/
/*----------------------------------------------------------------------*/
void StoreIdleRegs (void)
  {
    StoreRegs(IdleRegs);
  }


/*----------------------------------------------------------------------*/
/*									*/
/*	Function name:	RestoreIdleRegs					*/
/*									*/
/*	Created: --/May/92  J. Warta					*/
/*	Revised:							*/
/*									*/
/*									*/
/*	Description: Restore all registers for the idle task.		*/
/*									*/
/*----------------------------------------------------------------------*/
void RestoreIdleRegs (void)
  {
    RestoreRegs(IdleRegs);
  }
