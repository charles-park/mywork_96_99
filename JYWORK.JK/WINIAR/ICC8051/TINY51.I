
TASK_NEXTPTR	equ	0
TASK_PID	equ	3
TASK_WAITSIG	equ	4
TASK_RECSIG	equ	5
TASK_TIMEOUT	equ	6
TASK_STATE	equ	7
TASK_SP		equ	8
TASK_PUSHFUNC	equ	9
TASK_POPFUNC	equ	11

TS_NEW		equ	0
TS_RUNNING	equ	1
TS_READY	equ	2
TS_WAITING	equ	3
TS_STOPPED	equ	4

SIG_KILL	equ	1
SIG_TIMEOUT	equ	2
SIG_KEYBOARD	equ	4

TIMEOUT_DELAY	equ	100	; delay value for timeout -> 100 x 10ms = 1s
DISPATCH_DELAY	equ	40	; delay value for taskswitching 40 x 250 us = 10ms

IRQ_NEWTIMER	equ	0F0h

MEM_XDATA	equ	1	; memory type specify for XDATA
				; Keil8051 = 2, IAR = 1
