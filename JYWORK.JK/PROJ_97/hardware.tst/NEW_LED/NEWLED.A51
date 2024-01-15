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
        ORG     0000H   ORG ORG ORG ORG ORG ORG ORG   ORG   ORG  ORG
        SJMP    START   SJMPSJMPSJMPSJMPSJMPSJMPSJMP  SJMP  SJMP SJMP

        ORG     0003H   ORG ORG ORG ORG ORG ORG ORG   ORG   ORG  ORG
        SJMP    START   SJMPSJMPSJMPSJMPSJMPSJMPSJMP  SJMP  SJMP SJMP

        ORG     000BH   ORG ORG ORG ORG ORG ORG ORG   ORG   ORG  ORG    ; TIMER 0
        AJMP    T0INT   AJMPAJMPAJMPAJMPAJMPAJMPAJMP  AJMP  AJMP AJMP

        ORG     0013H   ORG ORG ORG ORG ORG ORG ORG   ORG   ORG  ORG
        SJMP    START   SJMPSJMPSJMPSJMPSJMPSJMPSJMP  SJMP  SJMP SJMP

        ORG     001BH   ORG ORG ORG ORG ORG ORG ORG   ORG   ORG  ORG    ; TIMER 1
        SJMP    START   SJMPSJMPSJMPSJMPSJMPSJMPSJMP  SJMP  SJMP SJMP   ; SERIAL ROUTINE USING

        ORG     0023H
        LJMP    SINT
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
;     =========================
;       EXTERNAL I/O's SETTING
;     =========================
        CLR     CS              ; DISABLE CHIP
        CLR     CLK             ; DISABLE CHIP
        CLR     DI              ; DISABLE CHIP

        CLR     LT0
        CLR     LT1
        CLR     LT2             ; CHIP's DISABLE
        CLR     LT3             ; ID ACTIVE
        MOV     P1,#FFH
        NOP
        MOV     A,P1
        CPL     A
        RR      A
        ANL     A,#7FH
        MOV     JMPID,A
        SETB    LT3             ; CHIP's DISABLE
;      ===============================
;        INTERNAL MEMORY DEFAULT SET
;      ===============================
        MOV     R2,#SRDATA
        MOV     R3,#09H
SEPLOAD:
        MOV     A,R3
        CALL    READR
        MOV     A,R0
        MOV     4,1

        MOV     0,2
        MOV     @R0,A
        INC     R0
        MOV     A,R4
        MOV     @R0,A
        INC     R0

        MOV     2,0
        DEC     R3
        CJNE    R3,#00H,SEPLOAD

        CALL    CHKSUM
        JNZ     CHKLOAD
CHKBAD:
        MOV     CLIMIT,#04
        MOV     DPOINT,#0H
        MOV     PERH,#FAH                   ; INTERNAL TIMER
        MOV     PERL,#FFH
        MOV     BAUD,#E8H
        SETB    BLKON
        SJMP    FINIT
CHKLOAD:
        MOV     A,SRDATA+CH
        ANL     A,#07H
        MOV     DPTR,#TBLBAUD
        MOVC    A,@A+DPTR
        MOV     BAUD,A

        MOV     A,SRDATA+AH
        MOV     PERH,A
        ADD     A,#02H
        JNC     VSETCON
        MOV     PERH,#FDH
VSETCON:
        MOV     DKUBUN,SRDATA+4
        MOV     DSKIND,SRDATA+9H
        MOV     A,SRDATA+7          ; CLIMIT VALUE
        ANL     A,#0FH
        JZ      CHKLOAD             ; IF CLIMIT IS LOW THEN GOTO DEFAULT
        MOV     CLIMIT,A
        MOV     DSID,SRDATA+5
        MOV     PERL,SRDATA+BH
        CLR     BLKON

        MOV     A,SRDATA+8
        JNB     A.7,FNDON

        SETB    DPDOT
        MOV     DPOINT,#0
        JMP     DINIT
FNDON:
        MOV     DPOINT,A
        MOV     BDPOINT,A
        CLR     DPDOT
; ----
FINIT:
        MOV     R0,#DISPBF
        MOV     R1,#07H
        MOV     R2,#1H      ; COUNT VALUE
        CLR     RBIT
MINIT:
        MOV     A,R2
        CJNE    A,DPOINT,INIT01
        MOV     A,#BFH
        SETB    RBIT
        SJMP    INIT03
INIT01:
        CJNE    A,CLIMIT,INIT04
        MOV     A,#3FH
        SETB    RBIT
        SJMP    INIT03
INIT04:
        JNB     RBIT,INIT02
        MOV     A,#3FH
        SJMP    INIT03
INIT02:
        CLR     A
INIT03:
        MOV     @R0,A
        INC     R0
        INC     R2
        DJNZ    R1,MINIT
        SJMP    MNCONT
; ----
DINIT:
        MOV     DPTR,#D_FONTO
        MOV     R0,#05H
        MOV     R1,#DISPBF+0AH
DINIT01:
        CLR     A
        MOVC    A,@A+DPTR
        MOV     @R1,A
        INC     DPTR
        INC     R1
        DJNZ    R0,DINIT01
MNCONT:
        MOV     RESETH,#RESET1C
        MOV     RESETL,#RESET2C

;    ==========================
;      INTERNAL INTERRUPT SET
;    ==========================
; TIMER 0
        MOV     TMOD,#21H               ; T0, 16BIT NO PRESCALER TIMER
        MOV     SCON,#50H
        MOV     TH0,PERH                ; INTERNAL TIMER
        MOV     TL0,PERL
        MOV     TH1,BAUD                ; 2400 BPS SET
        MOV     TL1,BAUD

        SETB    TR0
        SETB    TR1

        SETB    ET0
        SETB    ES
        SETB    PS
        SETB    EA
; ===
ROTATE:
        CPL     WDT
        CALL    BLINKP
        CALL    DREADY
        SJMP    ROTATE
; =============================================
DREADY:
        JB      DISPON,DRCONT
        RET
DRCONT:
        MOV     RESETH,#RESET1C
        MOV     RESETL,#RESET2C
        SETB    INITBIT

        CLR     DISPON
        MOV     A,DSCODE
        CJNE    A,#02H,DRCONT01
        JMP     CALFTEMP
DRCONT01:
        CJNE    A,#07H,DRCONT02
        JMP     CALFTEMP
DRCONT02:
        JNB     DPDOT,CALFNORM
        CJNE    A,#00H,DRCONT03
        JMP     CALDWIND
DRCONT03:
        CJNE    A,#04H,CALFNORM
        JMP     CALDPRED
; ===
CALFNORM:
        MOV     R0,DVALH
        MOV     R1,DVALL

        CALL    HEX2BCD

        MOV     R0,#DISPBF-1
        MOV     A,CLIMIT
        ADD     A,R0
        MOV     R0,A            ; DISPLAY START ADDRESS
        MOV     R1,#7H
FNORM02:
        CLR     ET0             ; TIMER 0 INTERRUPT DISABLE
        CJNE    R0,#DISPBF-1,FNORM01
        SJMP    FNORM03
FNORM01:
        MOV     A,@R1
        CALL    FNTLOAD
        MOV     @R0,A
        DEC     R0
        DEC     R1
        SJMP    FNORM02
FNORM03:
        JMP     DRDY
; ===
CALFTEMP:
        MOV     R2,DVALH
        MOV     R3,DVALL

        CLR     SIGN
        MOV     0,2                     ; Load Temp Average data High
        MOV     A,R3                    ; Load Temp Average data LOW
        CLR     C
        SUBB    A,#E8H
        MOV     R1,A
        MOV     A,R0
        SUBB    A,#03H
        MOV     R0,A
        JNC     TBHA_0
        MOV     A,#0
        CLR     C
        SUBB    A,R1
        MOV     R1,A
        MOV     A,#0
        SUBB    A,R0
        MOV     R0,A
        SETB    SIGN
TBHA_0:
        CALL    HEX2BCD                  ; Hex To BCD

        MOV     R0,#DISPBF-1
        MOV     A,CLIMIT
        ADD     A,R0
        MOV     R0,A            ; DISPLAY START ADDRESS
        MOV     R1,#7H
FTEMP02:
        CLR     ET0             ; TIMER0 INTERRUPT DISABLE
        CJNE    R0,#DISPBF-1,FTEMP01
        SJMP    FTEMP03
FTEMP01:
        MOV     A,@R1
        CALL    FNTLOAD
        MOV     @R0,A
        DEC     R0
        DEC     R1
        SJMP    FTEMP02
FTEMP03:
        SETB    ET0             ; TIMER0 INTERRUPT DISABLE
        JNB     SIGN,FTEMP04
        MOV     DISPBF,#40H     ; MINUS CODE SET

FTEMP04:
        JMP     DRDY
; ===
CALDWIND:
        MOV     R0,DVALH
        MOV     R1,DVALL

        MOV     R2,#00H
        MOV     R3,#113
        MOV     R4,#0H
DWIND01:
        MOV     A,R1
        CLR     C
        SUBB    A,R3
        MOV     A,R0
        SUBB    A,R2
        JC      DWIND02

        MOV     A,R3
        ADD     A,#225
        MOV     R3,A
        MOV     A,R2
        ADDC    A,#0H
        MOV     R2,A
        INC     R4
        SJMP    DWIND01
DWIND02:
        MOV     A,R4            ; DISPLY TABLE NO LOAD
        MOV     B,#03H
        MUL     AB              ; DISPLAY TABLE CALC
        MOV     DPTR,#WD_FNT
        ADD     A,DPL
        MOV     DPL,A
        MOV     A,DPH
        ADDC    A,#00H
        MOV     DPH,A           ;

        CLR     A
        MOVC    A,@A+DPTR
        MOV     R5,A            ; FIRST DOT CODE LOAD

        INC     DPTR
        CLR     A
        MOVC    A,@A+DPTR
        MOV     R6,A            ; SECOND DOT CODE LOAD

        INC     DPTR
        CLR     A
        MOVC    A,@A+DPTR
        MOV     R7,A            ; THERD DOT CODE LOAD

        MOV     R0,#DISPBF

        MOV     A,R5
        CALL    DOTLOAD
        MOV     A,R6
        CALL    DOTLOAD
        MOV     A,R7
        CALL    DOTLOAD
        JMP     DRDY
; ===
CALDPRED:
        MOV     R0,DVALH
        MOV     R1,DVALL

        CLR     A
        ORL     A,R0
        ORL     A,R1

        JNZ     DPRED01
        MOV     R5,#4H
        MOV     R6,#5H
        MOV     R7,#5H
        SJMP    DPRED02
DPRED01:
        MOV     R5,#6H
        MOV     R6,#4H
        MOV     R7,#0H
DPRED02:
        MOV     R0,#DISPBF

        MOV     A,R5
        CALL    DOTLOAD
        MOV     A,R6
        CALL    DOTLOAD
        MOV     A,R7
        CALL    DOTLOAD

        JMP     DRDY
; ===========
DRDY:
        MOV     A,SSTATUS
        JNB     A.7,DRDY03
        MOV     DPOINT,#0H
        SJMP    DRDY04
DRDY03:
        MOV     DPOINT,BDPOINT
DRDY04:
        ANL     A,#7FH
        JZ      DRDY05
        SETB    BLKON
        SJMP    DRDY06
DRDY05:
        CLR     BLKON
DRDY06:
        JB      DPDOT,DRDYOUT
; --
        MOV     R0,#1H              ; USING IN DOT POINT FIND
        MOV     R1,#DISPBF
DRDYLOP:
        MOV     A,DPOINT
        CJNE    A,0,DRAY01
        SJMP    DRDYDOT
DRAY01:
        MOV     A,CLIMIT
        CJNE    A,0,DRDY02
        SJMP    DRDYDOT
DRDY02:
        MOV     A,@R1
        CJNE    A,#3FH,BLANK01      ; ZERO
        CLR     A
        MOV     @R1,A
        INC     R1
        INC     R0
        SJMP    DRDYLOP
BLANK01:
        CJNE    A,#40H,DRDYDOT      ; MINUS
        INC     R1
        INC     R0
        SJMP    DRDYLOP
DRDYDOT:
        MOV     A,#DISPBF-1
        ADD     A,DPOINT
        MOV     R0,A
        MOV     A,@R0
        ORL     A,#80H              ; DOT SET
        MOV     @R0,A
;        SJMP    DRDYOUT
DRDYOUT:
        SETB    ET0             ; TIMER 0 INTERRUPT DISABLE
        RET
; =============================================
BLINKP:
        DEC     BLBUF2
        MOV     A,BLBUF2
        JNZ     BLOUT
        DEC     BLBUF1
        MOV     A,BLBUF1
        JNZ     BLOUT
        MOV     BLBUF1,#BLBUF1C
        MOV     BLBUF2,#BLBUF2C

        CPL     BLINK

        DEC     RESETL
        MOV     A,RESETL
        JNZ     BLOUT

        DEC     RESETH
        MOV     A,RESETH
        JNZ     BLOUT
        JNB     INITBIT,BLOUT

        CLR     EA              ; RESET
        JMP     $
BLOUT:
        RET
; =============================================
WRITER:
        CLR     CS
        NOP
        CLR     CLK
        NOP
        CLR     DI
        NOP
        SETB    CS              ; ENABLE CHIP

        PUSH    0
        MOV     R0,#00H
WDLOOP:
        DJNZ    R0,WDLOOP
WDLOOP2:
        DJNZ    R0,WDLOOP2
WDLOOP3:
        DJNZ    R0,WDLOOP3
        POP     0

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
        SETB    DO
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
        NOP
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
        AJMP     HTOA_EXIT
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

        MOV     A,#00H
        CALL    DLATCH          ; SET BLANK

        INC     CCOUNT
        MOV     A,CLIMIT
        CJNE    A,CCOUNT,COLUP
        MOV     CCOUNT,#00H     ; ZERO SET
; -
COLUP:
        CLR     C
        MOV     R0,CCOUNT
        ANL     0,#07H
        MOV     A,#FFH
COLC:
        RLC     A
        CJNE    R0,#00H,COLD
        SJMP    COLOUT
COLD:
        DEC     R0
        SETB    C
        SJMP    COLC
COLOUT:
        MOV     R0,A

        MOV     A,CCOUNT
        JB      A.3,NLATUP

        MOV     A,R0
        CALL    C1LATCH
        MOV     A,#FFH
        CALL    C2LATCH
        SJMP     DOUT
NLATUP:
        MOV     A,R0
        CALL    C2LATCH
        MOV     A,#FFH
        CALL    C1LATCH
; ---
DOUT:
        MOV     A,CCOUNT
        ADD     A,#DISPBF
        MOV     R0,A
        MOV     A,@R0
        JNB     BLKON,DOUTT
        JNB     BLINK,DOUTT
        ANL     A,#80H
DOUTT:
        CALL    DLATCH

        CLR     TR0
        MOV     TH0,PERH
        MOV     TL0,PERL
        SETB    TR0

        POP     0
        POP     A
        POP     PSW
        RETI
; =======================================
SINT:
        PUSH    PSW
        PUSH    A
        PUSH    0
        PUSH    1
        PUSH    2
        PUSH    3
        PUSH    B

        JBC     RI,RXINT
        CLR     TI
SREND:
        POP     B
        POP     3
        POP     2
        POP     1
        POP     0
        POP     A
        POP     PSW
        RETI
; ----------
RXINT:
        MOV     R0,#SRDATA+1
        MOV     R1,#11H
DMOVEL:
        MOV     A,@R0
        DEC     R0
        MOV     @R0,A
        INC     R0
        INC     R0
        DJNZ    R1,DMOVEL
        MOV     A,SBUF
        DEC     R0
        MOV     @R0,A

        CALL    CHKSUM
        JZ      SREND       ; CHECKSUM WRONG

        CALL    DISPLAYP    ; DISPLAY DATA COMPARE

        CALL    PROGRAM     ; PROGRAM PROTOCOL COMPARE


        SJMP    SREND
; ===========================
PROGRAM:
        MOV     A,SRDATA+3
        CJNE    A,#'P',PROOUT
        MOV     A,SRDATA+6
        CJNE    A,JMPID,PROOUT

        MOV     R2,#SRDATA
        MOV     R3,#09H
PROLOP:
        MOV     0,2
        MOV     1,3

        MOV     A,@R0
        MOV     R2,A
        INC     R0
        MOV     A,@R0
        MOV     R3,A
        INC     R0

        MOV     A,R2        ; R2 <---> R0
        XCH     A,R0        ; R3 <---> R1
        MOV     R2,A
        MOV     A,R3
        XCH     A,R1
        MOV     R3,A

        CLR     EA
        CALL    WRITER
        SETB    EA
        DEC     R3
        CJNE    R3,#00H,PROLOP
        CLR     EA
        SJMP    $
PROOUT:
        RET
; ===========================
DISPLAYP:
        MOV     A,SRDATA+3
        CJNE    A,DKUBUN,DPROOUT    ; DATA KUBUN

        CJNE    A,#'P',DISPCON
        SJMP    DPROOUT
DISPCON:
        MOV     A,SRDATA+6
        CJNE    A,DSID,DPROOUT      ; SENSOR ID
        MOV     DSCODE,SRDATA+5     ; SENSOR CODE LOAD

        MOV     B,#02H
        MOV     A,DSKIND
        ANL     A,#03H
        MUL     AB
        ADD     A,#SRDATA+7
        MOV     R0,A
        MOV     A,@R0
        MOV     R2,A
        INC     R0
        MOV     A,@R0
        MOV     R3,A

        MOV     DVALH,2
        MOV     DVALL,3
        MOV     SSTATUS,SRDATA+4        ; STATUS STORE

        SETB    DISPON
DPROOUT:
        RET
; ===========================
CHKSUM:
        MOV     A,SRDATA+11H
        CJNE    A,#FEH,CHKOUT
        MOV     A,SRDATA+10H        ;  END FRAME
        CJNE    A,#FFH,CHKOUT
        MOV     A,SRDATA+1
        CJNE    A,#FFH,CHKOUT       ; HEAD FRAME
        MOV     A,SRDATA+2
        CJNE    A,#FFH,CHKOUT

        CLR     A
        MOV     R0,#SRDATA+3
        MOV     R1,#12
CHKSL:
        XRL     A,@R0
        INC     R0
        DJNZ    R1,CHKSL

        MOV     R1,A
        MOV     A,@R0
        CJNE    A,1,CHKOUT
        MOV     A,#FFH              ; PROTOCOL GOOD
        RET
CHKOUT:
        MOV     A,#00H
        RET
; =======================================
DLATCH:
        JNB     DPDOT,DLAT01
        CPL     A
DLAT01:
        MOV     P1,A
        SETB    LT0
        NOP
        NOP
        CLR     LT0
        RET
; =======================================
C1LATCH:
        JNB     DPDOT,C1LAT01
        CPL     A
C1LAT01:
        MOV     P1,A
        SETB    LT1
        NOP
        NOP
        CLR     LT1
        RET
; =======================================
C2LATCH:
        JNB     DPDOT,C2LAT01
        CPL     A
C2LAT01:
        MOV     P1,A
        SETB    LT2
        NOP
        NOP
        CLR     LT2
        RET
; =======================================
FNTLOAD:
        ANL     A,#0FH
        MOV     DPTR,#F_FONT
        MOVC    A,@A+DPTR
        RET
; =======================================
DOTLOAD:
        MOV     B,#05H
        MUL     AB
        MOV     DPTR,#D_FONTN
        ADD     A,DPL
        MOV     DPL,A
        MOV     A,DPH
        ADDC    A,#0H
        MOV     DPH,A

        MOV     R1,#05H
DLOAD01:
        CLR     A
        MOVC    A,@A+DPTR
        MOV     @R0,A
        INC     R0
        INC     DPTR
        DJNZ    R1,DLOAD01

        RET
; =======================================
;            0   1   2   3   4   5   6   7   8   9   -   B   C   D   E   F
F_FONT  DB  3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH,40H,7CH,39H,5EH,79H,71H

D_FONTN DB  7FH,04H,08H,10H,7FH     ; DISPLAY CODE 0
D_FONTE DB  7FH,49H,49H,49H,41H     ; DISPLAY CODE 1
D_FONTW DB  3FH,40H,3EH,40H,3FH     ; DISPLAY CODE 2
D_FONTS DB  26H,49H,49H,49H,32H     ; DISPLAY CODE 3
D_FONTO DB  3EH,41H,41H,41H,3EH     ; DISPLAY CODE 4
D_FONTF DB  7FH,09H,09H,09H,01H     ; DISPLAY CODE 5
D_FONT0 DB  00H,00H,00H,00H,00H     ; DISPLAY CODE 6
        DB  00H,00H,00H,00H,00H     ; DISPLAY CODE 7

WD_FNT  DB  6,6,0       ; "  N" 660  0
        DB  0,0,1       ; "NNE" 001  1
        DB  6,0,1       ; " NE" 601  2
        DB  1,0,1       ; "ENE" 101  3
        DB  6,6,1       ; "  E" 661  4
        DB  1,3,1       ; "ESE" 131  5  *
        DB  6,3,1       ; " SE" 631  6
        DB  3,3,1       ; "SSE" 331  7
        DB  6,6,3       ; "  S" 663  8
        DB  3,3,2       ; "SSW" 332  9
        DB  6,3,2       ; " SW" 632  A
        DB  2,3,2       ; "WSW" 232  B
        DB  6,6,2       ; "  W" 662  C
        DB  2,0,2       ; "WNW" 202  D  *
        DB  6,0,2       ; " NW" 602  E
        DB  0,0,2       ; "NNW" 002  F
        DB  6,6,0       ; "  N" 660  10

TBLBAUD DB  40H,A0H,D0H, E8H, F4H, FDH,  E8H, E8H
;           300 600 1200 2400 4800 19200 2400 BPS

        END
