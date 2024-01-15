
/*
   example for TINY51


   - simple print out of counter states to show how multitasking is working

   - this version runs only in preemptive mode because tasks don't give
     up CPU
*/

#include <stdio.h>

#include "tiny51.h"


#define TASK_STACKSIZE  30
typedef struct stack
  {
    byte vect[TASK_STACKSIZE];
  };
  
void task1a(void);
void task2a(void);

void PushFunction1(void);
void PopFunction1(void);
void PushFunction2(void);
void PopFunction2(void);

#pragma memory=idata
struct stack taskstack;

#pragma memory=xdata
struct stack taskstack1;
struct stack taskstack2;
byte regstack1[8];
byte regstack2[8];

#pragma memory=idata
int counter1, counter2;

#pragma memory=default

/* main function is idle task !!! */
void main(void);

void main()
  {
    int ch;
#pragma memory=xdata
    struct TASK task1, task2;
#pragma memory=default

    /* initialize task structure */
    task1.pid = 1;
    task1.sp = &taskstack.vect[0];
    task1.pushfunc = PushFunction1;
    task1.popfunc = PopFunction1;
    create(&task1, task1a);

    task2.pid = 2;
    task2.sp = &taskstack.vect[0];
    task2.pushfunc = PushFunction2;
    task2.popfunc = PopFunction2;
    create(&task2, task2a);

    counter1 = 0;
    counter2 = 0;

    StartDispatcher(TINY51_PREEMPTIVE);

    do
      {
	printf("%d %d\r",counter1,counter2);
	ch = FALSE;
      }
    while (!ch);

    StopDispatcher(TINY51_PREEMPTIVE);
  }

void task1a()
  {
    for (;;)
      {
	counter1++;
      }
  }

void task2a()
  {
    for (;;)
      {
	counter2++;
      }
  }

void PushFunction1()
  {
    StoreRegs(regstack1);
    taskstack1=taskstack;
  }

void PopFunction1()
  {
    RestoreRegs(regstack1);
    taskstack=taskstack1;
  }

void PushFunction2()
  {
    StoreRegs(regstack2);
    taskstack2=taskstack;
  }

void PopFunction2()
  {
    RestoreRegs(regstack2);
    taskstack=taskstack2;
  }
