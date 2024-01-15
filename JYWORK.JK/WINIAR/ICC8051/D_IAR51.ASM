        TITL    'Dispatcher'
;=========================================================================
;       modul : dispatcher for TINY51
;
;       date        : 14.05.92
;       version     : 1.00
;       author      : J. Warta
;
;       description : modul for dispatching and scheduling task for
;                     multitasking kernel
;
;       notice      : dispatcher and scheduler use register bank 3
;                     ActTaskPtr has to be stored in DATA memory
;=========================================================================

;
; constant definition area
;
$tiny51.i                       ; includes task structure definition

;
; data definition area
;
        public  HaltDispatch
        RSEG    BITVARS
HaltDispatch:   ds      1       ; switch for dispatcher 0 - dispatch enable
                                ;                       1 - dispatch disable
UserDispatch:   ds      1       ; switch for timeout update, is used when
                                ; dispatcher is called by task (-> 'wait')

        public  dispatchcnt, timeoutcnt
        RSEG    D_UDATA
timeoutcnt:     ds      1       ; timeout counter, timeouthandler will be
                                ; called if counter reaches zero
dispatchcnt:    ds      1       ; delay counter for dispatcher, when zero
                                ; dispatch process will be activated

        RSEG    I_UDATA
localstack:     ds      8      ; local stack for dispatcher and scheduler
                                ; value can be decreased when no complex
                                ; push- and pop-function is used by task

        EXTERN  ActTaskPtr      ; pointer to active task
        EXTERN  TimeoutTask     ; timeout task structure
;
; code definition aread
;
        COMMON  INTVEC
        org     $+0Bh
                                ; interrupt vector for dispatcher
        ljmp    Dispatcher      ; timer 0 overflow

        RSEG    RCODE

;-------
;       function:       ScheduleNextTask
;
;       description:    dispatcher entry for task
;       input:          -
;       output:         -
;
;       notice:         ActTaskPtr points to new task
;                       before calling this routine
;                       dispatcher has to be disabled
;                       because of timer update interrupt cannot be disable
;-------
        public  ScheduleNextTask
ScheduleNextTask:
        setb    UserDispatch    ; disable timeout counter because
                                ; dispatcher is called asynchronly
        push    psw             ; save all register
        push    acc
        push    b
        push    dpl
        push    dph
        sjmp    dsx00           ; and call dispatcher

;-------
;       function:       Dispatcher
;
;       description:    dispatcher entry for interrupt
;       input:          -
;       output:         -
;
;       notice:         ActTaskPtr points to new task
;-------
Dispatcher:
        push    psw             ; save first only important register
        push    acc

        dec     dispatchcnt     ; and check if dispatcher should
        mov     a,dispatchcnt   ; activated really
        jz      dsi0

        pop     acc             ; if not, leave interrupt routine
        pop     psw             ; by this way dispatcher is called only
        reti                    ; every 10 ms

dsi0:                           ; dispatcher activ
        mov     dispatchcnt,#DISPATCH_DELAY
                                ; load new dispatch delay value
        push    b               ; now save remaining register
        push    dpl
        push    dph

        dec     timeoutcnt      ; check if timeout handler should
        mov     a,timeoutcnt    ; be called
        jnz     ds00
                                ; send signal to timeout task
        mov     TimeoutTask+TASK_RECSIG,#SIG_TIMEOUT

        mov     a,TimeoutTask+TASK_STATE
        cjne    a,#TS_WAITING,ds00

        mov     TimeoutTask+TASK_STATE,#TS_READY
        mov     timeoutcnt,#TIMEOUT_DELAY
                                ; load new timeout counter value

ds00:   jnb     HaltDispatch,dsx00
        ljmp    ds90            ; dispatch only when enabled

dsx00:  mov     psw,#018h       ; switch to register bank 3

        mov     a,ActTaskPtr    ; check memory type of pointer to actual task
        cjne    a,#MEM_XDATA,ds01
                                ; *** handler for XDATA memory pointer
        mov     a,ActTaskPtr+2  ; load address pointer offset for SP in dptr
        add     a,#TASK_SP
        mov     dpl,a
        mov     a,ActTaskPtr+1
        addc    a,#0
        mov     dph,a

        mov     a,sp
        movx    @dptr,a         ; store actual stack pointer in task structure

        mov     sp,#localstack  ; now we can assign localstack to stack pointer

        mov     a,ActTaskPtr+2  ; load address pointer offset for PUSHFUNC in dptr
        add     a,#TASK_PUSHFUNC
        mov     dpl,a
        mov     a,ActTaskPtr+1
        addc    a,#0
        mov     dph,a

        movx    a,@dptr         ; check for NULL-pointer
        inc     dptr
        mov     b,a
        movx    a,@dptr
        orl     a,b
        jz      ds02            ; if NULL-pointer jump over pushfunction call

        call    dsy01           ; store actual PC on stack (for return)
        sjmp    ds02            ; ready, now jump to scheduler
dsy01:  movx    a,@dptr         ; store pushfunction address on stack
        push    acc
        push    b
        ret                     ; and call it

                                ; *** handler for IDATA memory pointer
ds01:   mov     a,ActTaskPtr+2  ; put address offset for SP in R0
        add     a,#TASK_SP
        mov     R0,a

        mov     @R0,sp          ; store actual stack pointer in task structure

        mov     sp,#localstack  ; now, load new stack pointer

        mov     a,ActTaskPtr+2  ; put address offset for PUSHFUNC in R0
        add     a,#TASK_PUSHFUNC
        mov     R0,a

        mov     a,@R0           ; check NULL pointer
        mov     b,a
        inc     R0
        orl     a,@R0
        jz      ds02            ; if so, jump to scheduler

        call    dsy02           ; store actual PC on stack
        sjmp    ds02            ; after pushfunction jump to scheduler
dsy02:  mov     a,@R0           ; store address for pushfunction on stack
        push    acc
        push    b
        ret                     ; and call it

ds02:   lcall   scheduler       ; schedule a new task
                                ; now ActTaskPtr points to a new task
        mov     a,ActTaskPtr    ; check memory type of new task pointer
        cjne    a,#MEM_XDATA,ds11
                                ; *** handler for XDATA memory
        mov     a,ActTaskPtr+2  ; store address offset for POPFUNC in dptr
        add     a,#TASK_POPFUNC
        mov     dpl,a
        mov     a,ActTaskPtr+1
        addc    a,#0
        mov     dph,a

        movx    a,@dptr         ; check NULL pointer
        mov     b,a
        inc     dptr
        movx    a,@dptr
        orl     a,b
        jz      dsx02           ; if so, restoring stack pointer

        call    dsy03           ; save PC on stack
        sjmp    dsx02           ; and call popfunction
dsy03:  movx    a,@dptr
        push    acc
        push    b
        ret                     ; jump to function

dsx02:  mov     a,ActTaskPtr+2  ; store address offset for SP in dptr
        add     a,#TASK_SP
        mov     dpl,a
        mov     a,ActTaskPtr+1
        addc    a,#0
        mov     dph,a

        movx    a,@dptr         ; load actual task stack pointer
        mov     sp,a

        sjmp    ds20            ; and jump to restore part

ds11:                           ; *** handler for IDATA memory  ; IDATA-Behandlung
        mov     a,ActTaskPtr+2  ; put address offset for POPFUNC in R0
        add     a,#TASK_POPFUNC
        mov     R0,a

        mov     a,@R0           ; check NULL pointer
        mov     b,a
        inc     R0
        orl     a,@R0
        jz      dsx04           ; if so, jump to stack pointer restoring

        call    dsy04           ; save PC on stack
        sjmp    dsx04
dsy04:  mov     a,@R0           ; and call popfunction
        push    acc
        push    b
        ret

dsx04:  mov     a,ActTaskPtr+2  ; put address offset for SP in R0
        add     a,#TASK_SP
        mov     R0,a

        mov     sp,@R0          ; and restore task stack pointer

                                ; restore part
ds20:   jbc     UserDispatch,ds99
                                ; if no interrupt call clear flag and jump over
ds90:   pop     dph             ; restore all register
        pop     dpl
        pop     b
        pop     acc
        pop     psw
        reti                    ; and leave interrupt

ds99:   pop     dph             ; this was a normal function call from task
        pop     dpl             ; and therefore we have to leave it normal
        pop     b
        pop     acc
        pop     psw

        clr     HaltDispatch    ; now also enable dispatcher
                                ; before calling dispatcher by 'call' it is
        ret                     ; necessary to disable dispatcher


;-------
;       function:       scheduler
;
;       description:    schedules a new task from the 'ready'-list
;       input:          -
;       output:         -
;
;       notice:         ActTaskPtr points to new task
;-------
scheduler:
        mov     a,ActTaskPtr    ; check memory type
        cjne    a,#MEM_XDATA,sc01
                                ; *** handler for XDATA memory
        mov     a,ActTaskPtr+2  ; put address offset for STATE in dptr
        add     a,#TASK_STATE
        mov     dpl,a
        mov     a,ActTaskPtr+1
        addc    a,#0
        mov     dph,a

        movx    a,@dptr         ; check if task is running
        cjne    a,#TS_RUNNING,sc101

        mov     a,#TS_READY     ; if so, set task to ready state
        movx    @dptr,a

sc101:  mov     dpl,ActTaskPtr+2; put pointer to next task in dptr
        mov     dph,ActTaskPtr+1

        movx    a,@dptr         ; load pointer of next task
        mov     ActTaskPtr,a    ; in ActTaskPtr
        inc     dptr
        movx    a,@dptr
        mov     ActTaskPtr+1,a
        inc     dptr
        movx    a,@dptr
        mov     ActTaskPtr+2,a

        sjmp    sc20            ; and look if ready

sc01:                           ; *** handler for IDATA memory
        mov     a,ActTaskPtr+2  ; put address offset for STATE in R0
        add     a,#TASK_STATE
        mov     R0,a

        cjne    @R0,#TS_RUNNING,sc011
                                ; check if task is running
        mov     @R0,#TS_READY   ; if so, set task to ready state

sc011:
sc100:  mov     R0,ActTaskPtr+2 ; put pointer to next task in R0
        mov     ActTaskPtr,@R0  ; load pointer to next task in
        inc     R0              ; ActTaskPtr
        mov     ActTaskPtr+1,@R0
        inc     R0
        mov     ActTaskPtr+2,@R0

                                ; new taskpointer in ActTaskPtr
sc20:   mov     a,ActTaskPtr    ; check memory type
        cjne    a,#MEM_XDATA,sc21
                                ; *** handler for XDATA memory
        mov     a,ActTaskPtr+2  ; put address offset for STATE in dptr
        add     a,#TASK_STATE
        mov     dpl,a
        mov     a,ActTaskPtr+1
        addc    a,#0
        mov     dph,a

        movx    a,@dptr         ; check if task is ready
        cjne    a,#TS_READY, sc101
        mov     a,#TS_RUNNING   ; if so, set task in running state
        movx    @dptr,a
        sjmp    sc30            ; finnish scheduling

sc21:                           ; *** handler for IDATA memory
        mov     a,ActTaskPtr+2  ; put address offset for STATE in R0
        add     a,#TASK_STATE
        mov     R0,a

        cjne    @R0,#TS_READY,sc100
        mov     @R0,#TS_RUNNING ; check if task is read
                                ; if so, set task in running mode
sc30:   ret

;-------
;       function:       StoreRegs
;
;       description:    stores register bank 0 to specified memory
;       input:          pointer to memory
;       output:         -
;
;       notice:         can be called by pushfunction
;-------
        public  StoreRegs
StoreRegs:                      ; check memory tpye
        cjne    R7,#MEM_XDATA,sr01
                                ; *** handler for XDATA memory
        mov     dph,R6          ; destination pointer in dptr
        mov     dpl,R5
        mov     R1,#0           ; start address of register bank 0
        mov     b,#8            ; consist of eight register

sr02:   mov     a,@R1           ; copy loop
        movx    @dptr,a
        inc     R1
        inc     dptr
        djnz    b,sr02
        sjmp    sr99
sr01:                           ; *** handler for IDATA memory
        mov     a,R5            ; destination pointer in dptr
        mov     R0,a
        mov     R1,#0           ; start address of register bank 0
        mov     b,#8

sr04:   mov     a,@R1           ; copy loop
        mov     @R0,a
        inc     R1
        inc     R0
        djnz    b,sr04

sr99:   ret

;-------
;       function:       RestoreRegs
;
;       description:    restore register bank 0 from specified memory
;       input:          pointer to memory
;       output:         -
;
;       notice:         can be used by popfunction
;-------
        public  RestoreRegs
RestoreRegs:                    ; check memory type
        cjne    R7,#MEM_XDATA,rr01
                                ; *** handler for XDATA memory
        mov     dph,R6          ; source pointer in dptr
        mov     dpl,R5
        mov     R1,#0           ; start address of register bank 0
        mov     b,#8

rr02:   movx    a,@dptr         ; copy loop
        mov     @R1,a
        inc     R1
        inc     dptr
        djnz    b,rr02
        sjmp    rr99            ; finnish copy

rr01:                           ; *** handler for IDATA memory
        mov     a,R5            ; source pointer in R7
        mov     R0,a
        mov     R1,#0           ; start address of register bank 0
        mov     b,#8

rr04:   mov     a,@R0           ; copy loop
        mov     @R1,a
        inc     R1
        inc     R0
        djnz    b,rr04

rr99:   ret

;-------
;       function:       GetFunctionAddress
;
;       description:    determines indirect address pointer for function
;       input:          pointer to indirect address
;       output:         -
;
;       notice:         is called by create for faster access
;                       to function call address in task structure
;-------
        public  GetFunctionAddress
GetFunctionAddress:
        push    acc
        push    dpl
        push    dph

        mov     dpl,R4          ; address to function call address
        mov     dph,R5
        mov     a,#1
        movc    a,@a+dptr
        mov     R4,a

        clr     a
        movc    a,@a+dptr
        mov     R5,a            ; store call address in R4/R5

        pop     dph
        pop     dpl
        pop     acc             ; and finnish function
        ret
        end
