;/ /// /////////////////////////////////////////////// /// /;
;/ /// /////////////////////////////////////////////// /// /;
;/ /// ///            New_LED Displayer            /// /// /;
;/ /// ///                                         /// /// /;
;/ /// ///       Program By  Hae-suk. Jin          /// /// /;
;/ /// ///                  1996.  9. 04 At start  /// /// /;
;/ /// ///                  1996.   .    at Ver    /// /// /;
;/ /// /////////////////////////////////////////////// /// /;
;/ /// /////////////////////////////////////////////// /// /;

.INCLUDE NEWLED.H
        ORG     0000H
        SJMP    START

        ORG     0003H
        JMP     START

        ORG     000BH           ; TIMER 0
        JMP     T0INT

        ORG     0013H
        JMP     START

        ORG     001BH           ; TIMER 1
        JMP     START           ; SERIAL ROUTINE USING

        ORG     0023H
        JMP     SINT
START:
        MOV     IE,#80H                 ; INTERRUPT ALL DISABLE
        MOV     SP,#STACK
        MOV     PSW,#00H
        MOV     PCON,#80H
;      =============================
;        INTERNAL MEMORY ALL CLEAR
;      =============================
        MOV     A,#00H
        MOV     R0,#8H
MCLRL:
        MOV     @R0,A
        INC     R0
        CJNE    R0,#STACK,MCLRL
;      ===============================
;        INTERNAL MEMORY DEFAULT SET
;      ===============================



;     =========================
;       EXTERNAL I/O's SETTING
;     =========================



;    ==========================
;      INTERNAL INTERRUPT SET
;    ==========================
; TIMER 0
        MOV     TMOD,#21H               ; T0, 16BIT NO PRESCALER TIMER
        MOV     TH0,#00H                ; INTERNAL TIMER
        MOV     TL0,#00H
        MOV     TH1,#E8H                ; INTERNAL TIMER
        MOV     TL1,#E8H

        SETB    TR0

; ===
ROTATE:




        JMP     ROTATE

;        MOV     A,#00H         ; ADDRESS
;        CALL    READR
;        MOV     P1,R0          ; DATA => R0
;
;        MOV     A,#0AH          ; ADDRESS
;        MOV     R0,#12H         ; DATA 1
;        MOV     R1,#34H         ; DATA 2
;        CALL    WRITER
;
;        JMP     $

; =============================================
WRITER:
        CLR     CS
        CLR     CLK
        CLR     DI
        SETB    CS              ; ENABLE CHIP

        CALL    SCLOCK          ; SETUP CLOCK

        SETB    DI
        CALL    SCLOCK          ; START BIT
        CLR     DI
        CALL    SCLOCK
        CALL    SCLOCK          ; OPCODE SET
        SETB    DI
        CALL    SCLOCK
        CALL    SCLOCK          ; WRITE ENABLE SET
        CLR     DI
        CALL    SCLOCK
        CALL    SCLOCK
        CALL    SCLOCK
        CALL    SCLOCK          ; END
        NOP
        CLR     CS              ; UNLOCKING
        NOP
        NOP
        CLR     CLK
        CLR     DI
        SETB    CS              ; ENABLE CHIP

        CALL    SCLOCK          ; SETUP CLOCK
        SETB    DI              ; START BIT
        CALL    SCLOCK
        CLR     DI              ; OP1 SET ERASE DATA SET
        CALL    SCLOCK
        SETB    DI              ; OP2 SET
        CALL    SCLOCK
        RLC     A
        RLC     A

        PUSH    0               ; ADDRESS SET
        MOV     R0,#6H
WRADD:
        RLC     A
        MOV     DI,C
        CALL    SCLOCK
        DJNZ    R0,WRADD
        POP     0

        MOV     A,R1
        MOV     R1,#8
WRDAT1:
        RLC     A
        MOV     DI,C
        CALL    SCLOCK
        DJNZ    R1,WRDAT1

        MOV     A,R0
        MOV     R0,#8
WRDAT2:
        RLC     A
        MOV     DI,C
        CALL    SCLOCK
        DJNZ    R0,WRDAT2
        NOP
        CLR     CS              ; DISABLE CHIP
        NOP
        SETB    CS
        NOP
WRCHK:
        JNB     DO,WRCHK
        NOP
        CLR     CS
        NOP
        CLR     CLK
        CLR     DI
        SETB    CS              ; ENABLE CHIP

        CALL    SCLOCK          ; SETUP CLOCK

        SETB    DI
        CALL    SCLOCK          ; START BIT
        CLR     DI
        CALL    SCLOCK
        CALL    SCLOCK          ; OPCODE SET
        CLR     DI
        CALL    SCLOCK
        CALL    SCLOCK          ; WRITE DISABLE SET
        CLR     DI
        CALL    SCLOCK
        CALL    SCLOCK
        CALL    SCLOCK
        CALL    SCLOCK          ; END
        NOP
        CLR     CS              ; UNLOCKING
        NOP
        RET
; =============================================
READR:                          ; ADDRESS = A, AD0=R0, AD1=R1,..AD2=R0,AD3=R1
        CLR     CS              ; DISABLE CHIP
        CLR     DI
        CLR     CLK
        NOP
        SETB    CS              ; ENABLE CHIP
        CALL    SCLOCK
        SETB    DI
        CALL    SCLOCK          ; START BIT
        SETB    DI
        CALL    SCLOCK          ; OP1 SET  READ MODE
        CLR     DI
        CALL    SCLOCK          ; OP2 CLEAR
        RLC     A
        RLC     A
        MOV     R0,#6H
READD:
        RLC     A
        MOV     DI,C
        CALL    SCLOCK
        DJNZ    R0,READD        ; ADDRESS SET

        MOV     R1,#8
REDAT1:
        CALL    SCLOCK
        SETB    DO
        NOP
        MOV     C,DO
        RLC     A
        DJNZ    R1,REDAT1
        MOV     R1,A

        MOV     R0,#8
REDAT2:
        CALL    SCLOCK
        SETB    DO
        NOP
        MOV     C,DO
        RLC     A
        DJNZ    R0,REDAT2

        MOV     R0,A

        CLR     CS              ; DISABLE CHIP

        MOV     DPTR,#FF00H
RDLOOP:
        INC     DPTR
        MOV     A,DPH
        ORL     A,DPL
        CJNE    A,#00H,RDLOOP
        RET
; ===
SCLOCK:
        SETB    CLK
        CLR     CLK
        RET
; ===============================================================
HEX2BCD:
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
; =======================================
T0INT:
        PUSH    PSW
        PUSH    A
        PUSH    0

        MOV     P1,#FFH         ; COLUME OFF
        SETB    LT1
        NOP
        CLR     LT1             ; LATCH

        MOV     A,DISCNT
        MOV     R0,#DISPBF
        ADD     A,R0
        MOV     R0,A
        MOV     A,@R0           ; DATA READ

        MOV     P1,A
        SETB    LT0
        NOP
        CLR     LT0             ; DATA LATCH

        MOV     R0,DISCNT

        MOV     A,#7FH
COLCL:
        JZ      R0,COLOUT
COLC:
        RR      A
        DEC     R0
        SJMP    COLCL
COLOUT:







        MOV
        DISPBF


        POP     0
        POP     A
        POP     PSW
        RETI
; =======================================
SINT:
        PUSH    PSW
        PUSH    A

        POP     A
        POP     PSW
        RETI
; =======================================
;            0   1   2   3   4   5   6   7   8   9
F_FONT  DB  3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH
D_FONTN DB  FEH,08H,10H,20H,FEH
D_FONTE DB  FEH,92H,92H,92H,82H
D_FONTW DB  7EH,80H,7CH,80H,7EH
D_FONTS DB  4CH,92H,92H,92H,64H
        END
