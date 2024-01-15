typedef void (*FP)(void);

typedef unsigned char byte;
typedef unsigned short word;
typedef word *PTR;
typedef unsigned char semaphore;

typedef unsigned char stacktyp;

#define	TRUE	1
#define FALSE	0
#define NIL	((void *)0)

#define FOREVER	for(;;)

#define TS_NEW		0
#define	TS_RUNNING	1
#define TS_READY	2
#define TS_WAITING	3
#define TS_STOPPED	4

#define SIG_CPU		0
#define SIG_KILL	1
#define SIG_TIMEOUT	2
#define SIG_KEYBOARD	4
#define SIG_SEMAPHORE	0x80

#define PID_IDLETASK	0xFf
#define PID_TIMEOUT	0
#define PID_NEXT	0

/* definition of task structure */

 struct TASK {
	struct TASK 	*nextptr;
	byte	pid;
	byte	wait_signals;
	byte	rec_signals;
	byte	timeout;
	byte	state;
	stacktyp idata * sp;
	FP	pushfunc;
	FP	popfunc;
};

#define TT_STACKSIZE	10

#define TIMEOUT_DELAY	100
#define DISPATCH_DELAY	40
#define TICKER_RATE	6

#define IRQ_USRTIMER	0x08
#define IRQ_NEWTIMER	0xF0


#define TINY51_NONPREEMPTIVE	0
#define TINY51_PREEMPTIVE	1

 void StartDispatcher(byte);
 void StopDispatcher(byte);
 int create(struct TASK *,FP);
 int signal(byte, word);
 byte wait(byte);
 void timeout(byte);
 void ScheduleNextTask(void);
 void StoreRegs(byte *);
 void RestoreRegs(byte *);
 void StoreIdleRegs(void);
 void RestoreIdleRegs(void);
 void TimeoutHandler(void);
 void Dispatcher(void);
 void down(semaphore *);
 void up(semaphore *);
 
FP GetFunctionAddress(FP);