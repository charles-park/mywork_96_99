;       =========================
;          DATA CONSTANT SET
;       =========================
STACK       EQU     60H

;       ==========================
;         DATA MEMORY PARTITION
;       ==========================
ID          EQU     08H
DCOUNT      EQU     09H         ;    0 - 0FH
CAPID       EQU     0AH         ; CAPTURE ID
CAPKD       EQU     0BH         ; CAPTURE KIND = CHARACTER
                                ;               IF 'FFH' THEN EVERY ONE

CAPNO       EQU     0BH         ; CAPTURE NUMBER    0 : AVERAGE
                                ;                   1 : SPECIAL
                                ;                   2 : MAXIMUM
                                ;                   3 : MINIMUM

DISCL       EQU     0CH         ; COLUME METHODE    4  : FND 4
                                ;                   5  : FND 5
                                ;                  15  : DOT 3

DISCNT      EQU     0DH         ; COLUME DISPLAY COUNTE BUFFER



SRDATA      EQU     0EH         ; -1FH 18 BYTE  'SERIAL DATA BUFFER
DISPBF      EQU     30H         ; -40H 16 BYTE  'DISPLAY BUFFER

;       ========================
;         BIT MAP ( 20H - XXH )
;       ========================
VIBIT       EQU     00H         ; VIBRATION BIT

;       =============       INTH 5,6,7,8        ; INT 0
;          I/O MAPs         INTL 1,2,3,4        ; INT 1
;       =============
LT3         .REG    P3.2        ; VIEW JUMER
LT0         .REG    P3.3        ; DATA PORT
LT1         .REG    P3.4        ; COLUME SELECT PORT 1
LT2         .REG    P3.5        ; COLUME SELECT PORT 2
; --
WDT         .REG    P3.1        ; WATCH DOG PIN

CS          .REG    P3.7
CLK         .REG    P1.7
DI          .REG    P1.6
DO          .REG    P1.5
            END
