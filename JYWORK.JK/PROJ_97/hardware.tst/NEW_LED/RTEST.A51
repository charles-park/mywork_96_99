;------------------------------------------------------------------------------
; 2 Byte Hex to Ascii conversion procedure.
; Input   : R0,R1                       ; Hex data
; Output  : R2, R3, R4, R5, R6, R7
;------------------------------------------------------------------------------
.INCLUDE RTEST.H
        ORG     00H

        ORG     30H
        MOV     SP,#STACK
        MOV     R0,#FFH
        MOV     R1,#FFH
        MOV     P3,#FFH
        MOV     P1,#FFH
        MOV     R0,#FEH
MLOOP:
; --
        CLR     LT3
        MOV     P1,#FFH
        NOP
        MOV     A,P1        ; JUMPER READ
        SETB    LT3         ; DISABLE

        MOV     P1,A        ; DATA OUT
        SETB    LT0
        NOP
        NOP
        NOP
        NOP
        CLR     LT0

        MOV     P1,R0       ; COLUME OUT
        SETB    LT1
        NOP
        NOP
        NOP
        NOP
        CLR     LT1

        MOV     A,R0
        RL      A
        MOV     R0,A

        CPL     WDT
        CALL    DELAY

        JMP     MLOOP
; =======
DELAY:
        MOV     DPTR,#FE7FH
DLOOP:
        INC     DPTR
        MOV     A,DPH
        ORL     A,DPL
        JNZ     DLOOP
        RET
; =======


        CALL    HTOA_2




        JMP     $
; =======================================

; =======================================
HTOA_2:
        CLR     A
        MOV     R7,A
        MOV     R6,A
        MOV     R5,A
        MOV     R4,A
        MOV     R3,A
        MOV     R2,A

        MOV     A,R1
        MOV     B,#100
        DIV     AB
        MOV     R5,A
        XCH     A,B
        MOV     B,#10
        DIV     AB
        MOV     R6,A
        MOV     R7,B

        MOV     A,R0
        JNZ     HBC_GO
        JMP     HTOA_EXIT
HBC_GO:
        ANL     A,#00000001B
        JZ      HBC_0
        MOV     A,R5
        ADD     A,#2
        MOV     R5,A
        MOV     A,R6
        ADD     A,#5
        MOV     R6,A
        MOV     A,R7
        ADD     A,#6
        MOV     R7,A
HBC_0:
        MOV     A,R0
        ANL     A,#00000010B
        JZ      HBC_1
        MOV     A,R5
        ADD     A,#5
        MOV     R5,A
        MOV     A,R6
        ADD     A,#1
        MOV     R6,A
        MOV     A,R7
        ADD     A,#2
        MOV     R7,A
HBC_1:
        MOV     A,R0
        ANL     A,#00000100B
        JZ      HBC_2
        MOV     A,R4
        ADD     A,#1
        MOV     R4,A
        MOV     A,R6
        ADD     A,#2
        MOV     R6,A
        MOV     A,R7
        ADD     A,#4
        MOV     R7,A
HBC_2:
        MOV     A,R0
        ANL     A,#00001000B
        JZ      HBC_3
        MOV     A,R4
        ADD     A,#2
        MOV     R4,A
        MOV     A,R6
        ADD     A,#4
        MOV     R6,A
        MOV     A,R7
        ADD     A,#8
        MOV     R7,A
HBC_3:
        MOV     A,R0
        ANL     A,#00010000B
        JZ      HBC_4
        MOV     A,R4
        ADD     A,#4
        MOV     R4,A
        MOV     A,R6
        ADD     A,#9
        MOV     R6,A
        MOV     A,R7
        ADD     A,#6
        MOV     R7,A
HBC_4:
        MOV     A,R0
        ANL     A,#00100000B
        JZ      HBC_5
        MOV     A,R4
        ADD     A,#8
        MOV     R4,A
        MOV     A,R5
        ADD     A,#1
        MOV     R5,A
        MOV     A,R6
        ADD     A,#9
        MOV     R6,A
        MOV     A,R7
        ADD     A,#2
        MOV     R7,A
HBC_5:
        MOV     A,R0
        ANL     A,#01000000B
        JZ      HBC_6
        MOV     A,R3
        ADD     A,#1
        MOV     R3,A
        MOV     A,R4
        ADD     A,#6
        MOV     R4,A
        MOV     A,R5
        ADD     A,#3
        MOV     R5,A
        MOV     A,R6
        ADD     A,#8
        MOV     R6,A
        MOV     A,R7
        ADD     A,#4
        MOV     R7,A
HBC_6:
        MOV     A,R0
        ANL     A,#10000000B
        JZ      HTOA_EXIT
        MOV     A,R3
        ADD     A,#3
        MOV     R3,A
        MOV     A,R4
        ADD     A,#2
        MOV     R4,A
        MOV     A,R5
        ADD     A,#7
        MOV     R5,A
        MOV     A,R6
        ADD     A,#6
        MOV     R6,A
        MOV     A,R7
        ADD     A,#8
        MOV     R7,A

HTOA_EXIT:
        MOV     R0,#7
        MOV     R1,#6
        MOV     A,@R0
HBC_LOP:                                ; Make ASCII
        MOV     B,#10
        DIV     AB
        XCH     A,B
        MOV     @R0,A
        DEC     R0
        MOV     A,@R0
        ADD     A,B
        DJNZ    R1,HBC_LOP
        RET


F_FONT  DB  3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH
