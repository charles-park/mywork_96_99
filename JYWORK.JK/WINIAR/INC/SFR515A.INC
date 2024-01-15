;                     - SFR515A.INC -
;
;   Special header for the Siemens/SAB 80C515A/80C535A processors.
;

; CPU

F1      DEFINE  0D1H

XPAGE	DEFINE	091H
SYSCON	DEFINE	0B1H

; CCU

T2CON   DEFINE  0C8H
T2PS    DEFINE  0CFH
I3FR    DEFINE  0CEH
I2FR    DEFINE  0CDH
T2R1    DEFINE  0CCH
T2R0    DEFINE  0CBH
T2CM    DEFINE  0CAH
T2I1    DEFINE  0C9H
T2I0    DEFINE  0C8H

TL2     DEFINE  0CCH
TH2     DEFINE  0CDH

CTCON   DEFINE  0E1H
CCEN    DEFINE  0C1H
CC4EN   DEFINE  0C9H
CRCL    DEFINE  0CAH
CRCH    DEFINE  0CBH
CCL1    DEFINE  0C2H
CCH1    DEFINE  0C3H
CCL2    DEFINE  0C4H
CCH2    DEFINE  0C5H
CCL3    DEFINE  0C6H
CCH3    DEFINE  0C7H

; Ports

I3CC0   DEFINE  P1.0    ;Ext Int 3 / Capture Compare 0 pin
I4CC1   DEFINE  P1.1    ;Ext Int 4 / Capture Compare 1 pin
I5CC2   DEFINE  P1.2    ;Ext Int 5 / Capture Compare 2 pin
I6CC3   DEFINE  P1.3    ;Ext Int 6 / Capture Compare 3 pin
INT2    DEFINE  P1.4    ;External Interrupt 2 input pin
T2EX    DEFINE  P1.5    ;Timer 2 External reload trigger input pin
CLKOUT  DEFINE  P1.6    ;System Clock output pin
T2      DEFINE  P1.7    ;Timer 2 input pin

P4      DEFINE  0E8H    ;Port 4
CM0	DEFINE	P4.0
CM1	DEFINE	P4.1
CM2	DEFINE	P4.2
CM3	DEFINE	P4.3
CM4	DEFINE	P4.4
CM5	DEFINE	P4.5
CM6	DEFINE	P4.6
CM7	DEFINE	P4.7

P5      DEFINE  0F8H    ;Port 5
CCM0	DEFINE	P4.0
CCM1	DEFINE	P4.1
CCM2	DEFINE	P4.2
CCM3	DEFINE	P4.3
CCM4	DEFINE	P4.4
CCM5	DEFINE	P4.5
CCM6	DEFINE	P4.6
CCM7	DEFINE	P4.7

P6      DEFINE  0DBH

; Serial Channels

SRELL	DEFINE	0AAH
SRELH	DEFINE	0BAH
PCON    DEFINE  087H    ;Power down register

ADCON0  DEFINE  0D8H
BD      DEFINE  0DFH
CLK     DEFINE  0DEH
ADEX    DEFINE  0DDH
BSY     DEFINE  0DCH
ADM     DEFINE  0DBH
MX2     DEFINE  0DAH
MX1     DEFINE  0D9H
MX0     DEFINE  0D8H

; A/D converter

ADDATH  DEFINE  0D9H
ADDATL  DEFINE  0DAH

; Interrupt system

IEN0    DEFINE  0A8H
ET2     DEFINE  IEN0.5  ;Timer 2 overflow interrupt enable
WDT     DEFINE  IEN0.6  ;Watchdog timer reset flag
EAL     DEFINE  IEN0.7  ;Master interrupt enable

IEN1    DEFINE  0B8H
EADC    DEFINE  IEN1.0  ;A/D converter interrupt enable
EX2     DEFINE  IEN1.1  ;External interrupt 2 interrupt enable
EX3     DEFINE  IEN1.2  ;External interrupt 3 interrupt enable
EX4     DEFINE  IEN1.3  ;External interrupt 4 interrupt enable
EX5     DEFINE  IEN1.4  ;External interrupt 5 interrupt enable
EX6     DEFINE  IEN1.5  ;External interrupt 6 interrupt enable
SWDT    DEFINE  IEN1.6  ;Watchdog timer start/reset bit
EXEN2   DEFINE  IEN1.7  ;Timer 2 external reload interrupt enable

IP0     DEFINE  0A9H
IP1     DEFINE  0B9H

IRCON   DEFINE  0C0H
EXF2    DEFINE  0C7H
TF2     DEFINE  0C6H
IEX6    DEFINE  0C5H
IEX5    DEFINE  0C4H
IEX4    DEFINE  0C3H
IEX3    DEFINE  0C2H
IEX2    DEFINE  0C1H
IADC    DEFINE  0C0H

; Interrupt vectors

ET2I     DEFINE  02BH    ;Timer 2 interrupt vector
IADCV    DEFINE  043H    ;A/D converter interrupt vector
IEX2V    DEFINE  04BH    ;External Interrupt 2 vector
IEX3V    DEFINE  053H    ;External Interrupt 3 vector
IEX4V    DEFINE  05BH    ;External Interrupt 4 vector
IEX5V    DEFINE  063H    ;External Interrupt 5 vector
IEX6V    DEFINE  06BH    ;External Interrupt 6 vector






