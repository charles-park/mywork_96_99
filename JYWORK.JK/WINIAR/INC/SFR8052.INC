;			- SFR8052.INC -
;  This file contains symbol definitions for Special Function
;  Registers in the 8052 chip.
;

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

; Serial Channels

PCON    DEFINE  087H    ;Power down register
IDL	DEFINE	PCON.0	
PD2	DEFINE	PCON.1

; Interrupt system

ET2     DEFINE  IE.5    ;Timer 2 interrupt enable bit
PT2     DEFINE  IP.5    ;Timer 2 interrupt priority bit

; Interrupt vectors

ET2I    DEFINE  02BH    ;Timer 2 interrupt vector



