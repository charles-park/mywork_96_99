;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³           96' AWS MPM (Main Processing Module) Include program             ³
;³                           program version : 7.10                           ³
;³                        Jinyang industrial CO.,LTD.                         ³
;³                             DATE : 04/11/1996                              ³
;³                             Program by M.J.M                               ³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

;------------------------------------------------------------------------------
; Serial initial procedure.
;------------------------------------------------------------------------------
SINIT:  CLR     0AAH                    ; Timer 1 interrupt vector clear
        MOV     89H,#20H                ; 8 Bit auto relode mode (89H=TMOD)
        MOV     SCON,#52H               ; Serial : MODE 1, REN=1, TI=1
                                        ; 8 Bit, No parity, 1 Stop bit
        MOV     87H,#00H                ; 87H = PCON, (PCON.7=SMOD)
        MOV     8DH,#0FDH               ; 8DH = TH1
                                        ; TH1 = 256-(2eSMODE/32 * (F/BAUD*12))
                                        ; F = 11.0592MHz
                                        ; TH1 = 0FDH : 19.2K(SMOD=1), 9600(SMOD=0) BAUD
                                        ; TH1 = 0E8H : 2400(SMOD=1),  1200(SMOD=0) BAUD
        RET

;------------------------------------------------------------------------------
; i16C450 initial procedure.
;       INPUT   : DPH = Address
;               : R0  = Baud Rate
;------------------------------------------------------------------------------
i16C450:
        MOV     DPH,#00H
        MOV     DPL,#01H
        MOV     A,#00H                  ; Interrupt enable register clear
        MOVX    @DPTR,A

        MOV     DPL,#03H
        MOV     A,#80H                  ; Divisor latch mode
        MOVX    @DPTR,A

        MOV     DPL,#00H                ; Divisor loe byte
        MOV     A,R0
        MOVX    @DPTR,A

        MOV     DPL,#00000001B          ; Divisor high byte
        MOV     A,#00H
        MOVX    @DPTR,A

        MOV     DPL,#00000011B          ; Line controll register
        MOV     A,#00000011B
        MOVX    @DPTR,A

        MOV     DPL,#00000001B          ; Interrupt enable register
        MOV     A,#00000011B            ; TX/RX
        MOVX    @DPTR,A
        RET

;------------------------------------------------------------------------------
; Real Time Clock read procedure.
;------------------------------------------------------------------------------
RTC_READ:

        CLR     EA
        PUSH    A
        PUSH    0
        MOV     P1_RTC,P1_STATUS
        MOV     R0,#00H

        CALL    CS1
        MOVX    A,@R0                   ; Secound
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS
        MOV     SEC,A

        MOV     R0,#02H
        CALL    CS1
        MOVX    A,@R0                   ; Minute
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS
        MOV     MIN,A

        MOV     R0,#04H
        CALL    CS1
        MOVX    A,@R0                   ; Hour
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS
        MOV     HOU,A

        MOV     R0,#07H
        CALL    CS1
        MOVX    A,@R0                   ; Day
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS
        MOV     DAY,A

        MOV     R0,#08H
        CALL    CS1
        MOVX    A,@R0                   ; Month
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS
        MOV     MON,A

        MOV     R0,#09H
        CALL    CS1
        MOVX    A,@R0                   ; Year
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS
        MOV     YEA,A

        POP     0
        POP     A
        SETB    EA
        RET

;------------------------------------------------------------------------------
; Real Time Clock write procedure.
;------------------------------------------------------------------------------
RTC_WRITE:

        CLR     EA
        PUSH    A
        PUSH    0
        MOV     P1_RTC,P1_STATUS

        MOV     R0,#00H
        MOV     A,SEC
        CALL    CS1
        MOVX    @R0,A                   ; Secound
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS

        MOV     R0,#02H
        MOV     A,MIN
        CALL    CS1
        MOVX    @R0,A                   ; Minute
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS

        MOV     R0,#04H
        MOV     A,HOU
        CALL    CS1
        MOVX    @R0,A                   ; Hour
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS

        MOV     R0,#07H
        MOV     A,DAY
        CALL    CS1
        MOVX    @R0,A                   ; Day
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS

        MOV     R0,#08H
        MOV     A,MON
        CALL    CS1
        MOVX    @R0,A                   ; Month
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS

        MOV     R0,#09H
        MOV     A,YEA
        CALL    CS1
        MOVX    @R0,A                   ; Year
        MOV     P1_STATUS,P1_RTC
        MOV     P1,P1_STATUS

        POP     0
        POP     A
        SETB    EA
        RET

;------------------------------------------------------------------------------
; 1 Byte Hex to Ascii conversion procedure.
; Input   : R1                          ; Hex data
; Output  : ASCII_3, ASCII_2, ASCII_1
; Express : 1)R1(48H)-> ASCII_3 = '20H', ASCII_2 = '37H', ASCII_1 = '32H' ( 72)
;           2)AL(AAH)-> ASCII_3 = '31H', ASCII_2 = '37H', ASCII_1 = '30H' (170)
;------------------------------------------------------------------------------
HTOA_1:
        MOV     A,R1
        MOV     B,#100
        DIV     AB
        ADD     A,#30H
        MOV     ASCII_3,A
        XCH     A,B
        MOV     B,#10
        DIV     AB
        ADD     A,#30H
        MOV     ASCII_2,A
        XCH     A,B
        ADD     A,#30H
        MOV     ASCII_1,A

        MOV     R1,#ASCII_3
        MOV     R2,#3
A_CON_1B:                               ; ASCII Check
        CJNE    @R1,#30H,A_OK_1B
        MOV     A,R2
        XRL     A,#1
        JZ      A_OK_1B
        MOV     @R1,#20H
        INC     R1
        DJNZ    R2,A_CON_1B
        RET
A_OK_1B:
        RET

;------------------------------------------------------------------------------
; 2 Byte Hex to Ascii conversion procedure.
; Input   : R1,R2                       ; Hex data
; Output  : ASCII_5, ASCII_4, ASCII_3, ASCII_2, ASCII_1
; Express : R1,R2 = A4C8H : ASCII_5 = '34H', ASCII_4 = '32H', ASCII_3 = '31H'
;                           ASCII_2 = '38H', ASCII_1 = '34H'  (42184)
;         : R1,R2 = 002AH : ASCII_5 = '20H', ASCII_4 = '20H', ASCII_3 = '20H'
;                           ASCII_2 = '34H', ASCII_1 = '32H'  (   42)
;------------------------------------------------------------------------------
HTOA_2:
        CLR     A
        MOV     ASCII_1,A
        MOV     ASCII_2,A
        MOV     ASCII_3,A
        MOV     ASCII_4,A
        MOV     ASCII_5,A

        MOV     A,R2
        MOV     B,#100
        DIV     AB
        MOV     ASCII_3,A
        XCH     A,B
        MOV     B,#10
        DIV     AB
        MOV     ASCII_2,A
        MOV     ASCII_1,B

        MOV     A,R1
        JNZ     HBC_GO
        JMP     HTOA_EXIT
HBC_GO:
        ANL     A,#00000001B
        JZ      HBC_0
        MOV     A,ASCII_3
        ADD     A,#2
        MOV     ASCII_3,A
        MOV     A,ASCII_2
        ADD     A,#5
        MOV     ASCII_2,A
        MOV     A,ASCII_1
        ADD     A,#6
        MOV     ASCII_1,A
HBC_0:
        MOV     A,R1
        ANL     A,#00000010B
        JZ      HBC_1
        MOV     A,ASCII_3
        ADD     A,#5
        MOV     ASCII_3,A
        MOV     A,ASCII_2
        ADD     A,#1
        MOV     ASCII_2,A
        MOV     A,ASCII_1
        ADD     A,#2
        MOV     ASCII_1,A
HBC_1:
        MOV     A,R1
        ANL     A,#00000100B
        JZ      HBC_2
        MOV     A,ASCII_4
        ADD     A,#1
        MOV     ASCII_4,A
        MOV     A,ASCII_2
        ADD     A,#2
        MOV     ASCII_2,A
        MOV     A,ASCII_1
        ADD     A,#4
        MOV     ASCII_1,A
HBC_2:
        MOV     A,R1
        ANL     A,#00001000B
        JZ      HBC_3
        MOV     A,ASCII_4
        ADD     A,#2
        MOV     ASCII_4,A
        MOV     A,ASCII_2
        ADD     A,#4
        MOV     ASCII_2,A
        MOV     A,ASCII_1
        ADD     A,#8
        MOV     ASCII_1,A
HBC_3:
        MOV     A,R1
        ANL     A,#00010000B
        JZ      HBC_4
        MOV     A,ASCII_4
        ADD     A,#4
        MOV     ASCII_4,A
        MOV     A,ASCII_2
        ADD     A,#9
        MOV     ASCII_2,A
        MOV     A,ASCII_1
        ADD     A,#6
        MOV     ASCII_1,A
HBC_4:
        MOV     A,R1
        ANL     A,#00100000B
        JZ      HBC_5
        MOV     A,ASCII_4
        ADD     A,#8
        MOV     ASCII_4,A
        MOV     A,ASCII_3
        ADD     A,#1
        MOV     ASCII_3,A
        MOV     A,ASCII_2
        ADD     A,#9
        MOV     ASCII_2,A
        MOV     A,ASCII_1
        ADD     A,#2
        MOV     ASCII_1,A
HBC_5:
        MOV     A,R1
        ANL     A,#01000000B
        JZ      HTOA_EXIT
        MOV     A,ASCII_5
        ADD     A,#1
        MOV     ASCII_5,A
        MOV     A,ASCII_4
        ADD     A,#6
        MOV     ASCII_4,A
        MOV     A,ASCII_3
        ADD     A,#3
        MOV     ASCII_3,A
        MOV     A,ASCII_2
        ADD     A,#8
        MOV     ASCII_2,A
        MOV     A,ASCII_1
        ADD     A,#4
        MOV     ASCII_1,A

HTOA_EXIT:
        MOV     R1,#ASCII_1
        MOV     R2,#6
        MOV     A,@R1
HBC_LOP:                                ; Make ASCII
        MOV     B,#10
        DIV     AB
        XCH     A,B
        ADD     A,#30H
        MOV     @R1,A
        DEC     R1
        MOV     A,@R1
        ADD     A,B
        DJNZ    R2,HBC_LOP

        MOV     R1,#ASCII_5
        MOV     R2,#5
A_CON_2B:                               ; ASCII Check
        CJNE    @R1,#30H,A_OK_EXT
        CJNE    R2,#1,A_OK_2B
        RET
A_OK_2B:
        MOV     @R1,#20H
        INC     R1
        DJNZ    R2,A_CON_2B
A_OK_EXT:
        RET

;------------------------------------------------------------------------------
; 2 Byte Ascii to 1 Byte Hex conversion procedure.
; Input   : ASCII_2, ASCII_1
; Output  : R1                          ; Hex data
; Express : 45(ascii) => R1=2DH
;------------------------------------------------------------------------------
A2_TO_H1:
        MOV     A,ASCII_2
        ANL     A,#0FH
        MOV     B,#10
        MUL     AB
        XCH     A,B
        MOV     A,ASCII_1
        ANL     A,#0FH
        ADD     A,B
        MOV     R1,A
        RET

;------------------------------------------------------------------------------
; 5 Byte Ascii to 2 Byte Hex conversion procedure.
; Input   : ASCII_5, ASCII_4, ASCII_3, ASCII_2, ASCII_1
; Output  : R1, R2                      ; Hex data
; Express : MAX 65535(ascii) => R1=FFH, R2=FFH
;------------------------------------------------------------------------------
A5_TO_H2:
        PUSH    0

        MOV     A,ASCII_5
        ANL     A,#0FH
        MOV     B,#100
        MUL     AB
        MOV     R1,B                    ; High
        MOV     B,#100
        MUL     AB
        MOV     R2,A                    ; Low
        MOV     A,R1
        MOV     R1,B
        MOV     B,#100
        MUL     AB
        ADD     A,R1
        MOV     R1,A

        MOV     A,ASCII_4
        ANL     A,#0FH
        MOV     B,#100
        MUL     AB
        MOV     R3,B                    ; High
        MOV     B,#10
        MUL     AB
        MOV     R4,A                    ; Low
        MOV     R0,B
        MOV     A,R3
        MOV     B,#10
        MUL     AB
        ADD     A,R0
        MOV     R3,A

        MOV     A,R2
        ADD     A,R4
        MOV     R2,A
        CLR     A
        ADDC    A,R1
        ADD     A,R3
        MOV     R1,A

        MOV     A,ASCII_3
        ANL     A,#0FH
        MOV     B,#100
        MUL     AB
        ADD     A,R2
        MOV     R2,A
        CLR     A
        ADDC    A,R1
        ADD     A,B
        MOV     R1,A

        MOV     A,ASCII_2
        ANL     A,#0FH
        MOV     B,#10
        MUL     AB
        ADD     A,R2
        MOV     R2,A
        CLR     A
        ADDC    A,R1
        ADD     A,B
        MOV     R1,A

        MOV     A,ASCII_1
        ANL     A,#0FH
        ADD     A,R2
        MOV     R2,A
        CLR     A
        ADDC    A,R1
        ADD     A,B
        MOV     R1,A

        POP     0
        RET

;------------------------------------------------------------------------------
; INCREMENT RX1 TRACE POINT
; INPUT  : R7 = INCREMENT VALUE
; INPUT  : R0 = RX1 TRACE BASE POINT
; OUTPUT : R0 = RX1 TRACE INCREMENT POINT
;------------------------------------------------------------------------------
INC_RX1_TRACE:
        INC     R0
        CJNE    R0,#C1_RX_END+1,INC_RX1
        MOV     R0,#C1_RX_START
INC_RX1:
        DJNZ    R7,INC_RX1_TRACE
        RET

;------------------------------------------------------------------------------
; DECREMENT RX1 TRACE POINT
; INPUT  : R7 = DECREMENT VALUE
; INPUT  : R0 = RX1 TRACE BASE POINT
; OUTPUT : R0 = RX1 TRACE DECREMENT POINT
;------------------------------------------------------------------------------
DEC_RX1_TRACE:
        DEC     R0
        CJNE    R0,#C1_RX_START-1,DEC_RX1
        MOV     R0,#C1_RX_END
DEC_RX1:
        DJNZ    R7,DEC_RX1_TRACE
        RET

;------------------------------------------------------------------------------
; INCREMENT RX2 TRACE POINT
; INPUT  : R7 = INCREMENT VALUE
; INPUT  : R0 = RX2 TRACE BASE POINT
; OUTPUT : R0 = RX2 TRACE INCREMENT POINT
;------------------------------------------------------------------------------
INC_RX2_TRACE:
        INC     R0
        CJNE    R0,#C2_RX_END+1,INC_RX2
        MOV     R0,#C2_RX_START
INC_RX2:
        DJNZ    R7,INC_RX2_TRACE
        RET

;------------------------------------------------------------------------------
; DECREMENT RX2 TRACE POINT
; INPUT  : R7 = DECREMENT VALUE
; INPUT  : R0 = RX2 TRACE BASE POINT
; OUTPUT : R0 = RX2 TRACE DECREMENT POINT
;------------------------------------------------------------------------------
DEC_RX2_TRACE:
        DEC     R0
        CJNE    R0,#C2_RX_START-1,DEC_RX2
        MOV     R0,#C2_RX_END
DEC_RX2:
        DJNZ    R7,DEC_RX2_TRACE
        RET

;------------------------------------------------------------------------------
; INCREMENT RX3 TRACE POINT
; INPUT  : R7 = INCREMENT VALUE
; INPUT  : R0 = RX3 TRACE BASE POINT
; OUTPUT : R0 = RX3 TRACE INCREMENT POINT
;------------------------------------------------------------------------------
INC_RX3_TRACE:
        INC     R0
        CJNE    R0,#C3_RX_END+1,INC_RX3
        MOV     R0,#C3_RX_START
INC_RX3:
        DJNZ    R7,INC_RX3_TRACE
        RET

;------------------------------------------------------------------------------
; DECREMENT RX3 TRACE POINT
; INPUT  : R7 = DECREMENT VALUE
; INPUT  : R0 = RX3 TRACE BASE POINT
; OUTPUT : R0 = RX3 TRACE DECREMENT POINT
;------------------------------------------------------------------------------
DEC_RX3_TRACE:
        DEC     R0
        CJNE    R0,#C3_RX_START-1,DEC_RX3
        MOV     R0,#C3_RX_END
DEC_RX3:
        DJNZ    R7,DEC_RX3_TRACE
        RET

;------------------------------------------------------------------------------
; Delay procedure.
;------------------------------------------------------------------------------
DELAY_1:
        MOV     DELAY_L,#AFH
DLOOP_0:
        DJNZ    DELAY_L,DLOOP_0
        RET

DELAY_2:
        MOV     DELAY_H,#02H
DLOOP_1:
        MOV     DELAY_L,#FFH
DLOOP_2:
        DJNZ    DELAY_L,DLOOP_2
        DJNZ    DELAY_H,DLOOP_1
        RET

DELAY_3:
        MOV     DELAY_H,#4FH
DLOOP_3:
        MOV     DELAY_L,#FFH
DLOOP_4:
        DJNZ    DELAY_L,DLOOP_4
        DJNZ    DELAY_H,DLOOP_3
        RET          

DELAY_4:
        MOV     DELAY_H,#FFH
DLOOP_5:
        MOV     DELAY_L,#FFH
DLOOP_6:
        DJNZ    DELAY_L,DLOOP_6
        DJNZ    DELAY_H,DLOOP_5
        MOV     DELAY_H,#FFH
DLOOP_7:
        MOV     DELAY_L,#FFH
DLOOP_8:
        DJNZ    DELAY_L,DLOOP_8
        DJNZ    DELAY_H,DLOOP_7
        RET

DELAY_5:
        MOV     DELAY_H,#FFH
DLOOP_9:
        MOV     DELAY_L,#FFH
DLOOP_10:
        DJNZ    DELAY_L,DLOOP_10
        DJNZ    DELAY_H,DLOOP_9
        MOV     DELAY_H,#FFH
DLOOP_11:
        MOV     DELAY_L,#FFH
DLOOP_12:
        DJNZ    DELAY_L,DLOOP_12
        DJNZ    DELAY_H,DLOOP_11
        MOV     DELAY_H,#FFH
DLOOP_13:
        MOV     DELAY_L,#FFH
DLOOP_14:
        DJNZ    DELAY_L,DLOOP_14
        DJNZ    DELAY_H,DLOOP_13
        RET

