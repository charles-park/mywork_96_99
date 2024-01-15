;       =========================
;          DATA CONSTANT SET
;       =========================
STACK       EQU     60H
BLBUF1C     EQU     7FH
BLBUF2C     EQU     FFH
RESET1C     EQU     06H
RESET2C     EQU     64H

;       ==========================
;         DATA MEMORY PARTITION
;       ==========================
ID          EQU     08H
CCOUNT      EQU     09H         ; 0 - 0FH  COLUME COUNT
CLIMIT      EQU     0AH         ; MAX 0FH
DPOINT      EQU     0BH         ; DOT POINT
BLBUF1      EQU     0CH         ; DISPLAY BLINK BUFFER
BLBUF2      EQU     0DH         ;
SRDATA      EQU     0EH         ; -1FH 18 BYTE  'SERIAL DATA BUFFER
PERH        EQU     22H         ; DISPLAY PERIOD
PERL        EQU     23H
BAUD        EQU     24H
JMPID       EQU     25H         ; REAL JUMPER ID
DKUBUN      EQU     26H         ; DISPLAY KUBUN
DSKIND      EQU     27H         ; DISPLAY SENSOR CODE
DSID        EQU     28H         ; DISPLAY SENSOR ID
DSCODE      EQU     29H         ; DISPLAY SENSOR ID
DVALH       EQU     2AH         ; DISPLAY VALUE HIGH     BINARY
DVALL       EQU     2BH         ; DISPLAY VALUE LOW       ~
SSTATUS     EQU     2CH         ; SENSOR STATUS VALUE
BDPOINT     EQU     2DH         ; DECIMAL POINT VALUE / BACKUP
RESETH      EQU     2EH         ; RESET BUFFER
RESETL      EQU     2FH

TESTBF1     EQU     4EH
TESTBF2     EQU     4FH

DISPBF      EQU     50H         ; -5FH 15 BYTE  'DISPLAY BUFFER

;       ========================
;         BIT MAP ( 20H - 21H )
;       ========================
BLINK       EQU     00H         ; VIBRATION BIT
BLKON       EQU     01H         ; DISPLAY BLINK 1 = ON, 0 = OFF
DPDOT       EQU     02H         ;
SIGN        EQU     03H         ; 1 = '-'
DISPON      EQU     04H
RBIT        EQU     05H         ; FREE BIT
INITBIT     EQU     06H

;       =============       INTH 5,6,7,8        ; INT 0
;          I/O MAPs         INTL 1,2,3,4        ; INT 1
;       =============
LT0         .REG    P3.3        ; DATA PORT
LT1         .REG    P3.4        ; COLUME SELECT PORT 1
LT2         .REG    P3.5        ; COLUME SELECT PORT 2
LT3         .REG    P3.2        ; VIEW JUMER            ' ACTIVE
; --
WDT         .REG    P3.1        ; WATCH DOG PIN

CS          .REG    P3.7
CLK         .REG    P1.7
DI          .REG    P1.6
DO          .REG    P1.5
            END
