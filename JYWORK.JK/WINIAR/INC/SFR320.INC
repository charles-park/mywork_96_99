;			- SFR320.INC -
;  This file contains symbol definitions for Special Function
;  Registers in the 80C320, 80C520, and 80C530 chip.
;


; Extra stackpointer

DPL1	DEFINE	084H
DPH1	DEFINE	085H
DPS	DEFINE	086H

; Serial Channels

PCON    DEFINE  087H    ;Power down register
PCON0   DEFINE  087H    ;Power down register
IDLE	DEFINE	PCON.0	
STOP	DEFINE	PCON.1
GF0	DEFINE	PCON.2
GF1	DEFINE	PCON.3
SMOD0	DEFINE	PCON.6
SMOD	DEFINE	PCON.7


CKCON	DEFINE	08EH		; Clock control register
EXIF	DEFINE	091H		; Extended Interrupt Control Register
SCON0	DEFINE	098H		; Serial port
SBUF0	DEFINE	099H		; Serial buffer

; Extra IE bits

ES1	DEFINE	IE.6
ET2	DEFINE	IE.5
ES0	DEFINE	IE.4

SADDR0	DEFINE	0A9H
SADDR1	DEFINE	0A1H

; Extra IP bits

PS1	DEFINE	IP.6
PT2	DEFINE	IP.5
PS0	DEFINE	IP.4

SADEN0	DEFINE	0B9H
SADEN1	DEFINE	0BAH

; Serial port 1

SCON1	DEFINE	0C0H
SM0_1	DEFINE	SCON1.7
SM1_1	DEFINE	SCON1.6
SM2_1	DEFINE	SCON1.5
REN_1	DEFINE	SCON1.4
TB8_1	DEFINE	SCON1.3
RB8_1	DEFINE	SCON1.2
TI_1	DEFINE	SCON1.1
RI_1	DEFINE	SCON1.0
SBUF1	DEFINE	0C1H

TA	DEFINE	0C7H

; CCU

T2CON   DEFINE  0C8H    ;Timer 2 control register
TF2     DEFINE  T2CON.7 ;Timer 2 overflow flag
EXF2    DEFINE  T2CON.6 ;Timer 2 external flag
RCLK    DEFINE  T2CON.5 ;Timer 2 overflow = serial receive clock
TCLK    DEFINE  T2CON.4 ;Timer 2 overflow = serial transmit clock
EXEN2   DEFINE  T2CON.3 ;Timer 2 external enable flag
TR2     DEFINE  T2CON.2 ;Timer 2 run enable bit
C_T2    DEFINE  T2CON.1 ;Timer 2 counter/timer select
CP_RL2  DEFINE  T2CON.0 ;Timer 2 capture/reload flag

TH2     DEFINE  0CDH    ;Timer 2 counter high byte
TL2     DEFINE  0CCH    ;Timer 2 counter low byte
RCAP2H  DEFINE  0CBH    ;Timer 2 capture high byte
RCAP2L  DEFINE  0CAH    ;Timer 2 capture low byte

; Ports

T2      DEFINE  P1.0    ;Timer/Counter 2 external input pin
T2EX    DEFINE  P1.1    ;Timer/Counter 2 capture/reload trigger pin

; PSW bit

FL	DEFINE	PSW.1

; Watchdog control

WDCON	DEFINE	0D8H
SMOD	DEFINE	WDCON.7
POR	DEFINE	WDCON.6
EPFI	DEFINE	WDCON.5
PFI	DEFINE	WDCON.4
WDIF	DEFINE	WDCON.3
WTRF	DEFINE	WDCON.2
EWT	DEFINE	WDCON.1
RWT	DEFINE	WDCON.0


; Extended interrupt enable

EIE	DEFINE	0E8H
EWDI	DEFINE	EIE.4
EX5	DEFINE	EIE.3
EX4	DEFINE	EIE.2
EX3	DEFINE	EIE.1
EX2	DEFINE	EIE.0

; for 80C530
ERTCI	DEFINE	EIE.5

; 

EIP	DEFINE	0E8H
PWDI	DEFINE	EIP.4
PX5	DEFINE	EIP.3
PX4	DEFINE	EIP.2
PX3	DEFINE	EIP.1
PX2	DEFINE	EIP.0

; for 80C530
PRTCI	DEFINE	EIP.5


; for 80C520 and 80C530
PMR	DEFINE	0C4H
STATUS	DEFINE	0C5H


; for 80C530
RTASS	DEFINE	0F2H
RTAS	DEFINE	0F3H
RTAM	DEFINE	0F4H
RTAH	DEFINE	0F5H
RTCC	DEFINE	0F9H
RTCSS	DEFINE	0FAH
RTCS	DEFINE	0FBH
RTCM	DEFINE	0FCH
RTCH	DEFINE	0FDH
RTCD0	DEFINE	0FEH
RTCD1	DEFINE	0FFH


; Interrupt vectors

ET2I    DEFINE  02BH    ;Timer 2 interrupt vector
PFI	DEFINE	033H	;Power Fail interrupt
SCON1	DEFINE	03BH	;Serial port 1
EX2	DEFINE	043H	;External interrupt 2
EX3	DEFINE	04BH	;External interrupt 3
EX4	DEFINE	053H	;External interrupt 4
EX5	DEFINE	05BH	;External interrupt 5
WDTI	DEFINE	063H	;Watchdog interrupt

; for 80C530
RTCI	DEFINE	06BH	;Real-time clock interrupt



