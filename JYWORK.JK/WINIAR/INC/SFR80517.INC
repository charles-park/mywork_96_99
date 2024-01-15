;                     - SFR80517.INC -
;
;   Special header for the Siemens/SAB 80C517/80C537 processors.
;

; CPU

F1      DEFINE  0D1H

DPSEL   DEFINE  092H

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
CTRELL  DEFINE  0DEH
CTRELH  DEFINE  0DFH
CMSEL   DEFINE  0F7H
CCEN    DEFINE  0C1H
CC4EN   DEFINE  0C9H
CMEN    DEFINE  0F6H
CRCL    DEFINE  0CAH
CRCH    DEFINE  0CBH
CCL1    DEFINE  0C2H
CCH1    DEFINE  0C3H
CCL2    DEFINE  0C4H
CCH2    DEFINE  0C5H
CCL3    DEFINE  0C6H
CCH3    DEFINE  0C7H
CCL4    DEFINE  0CEH
CCH4    DEFINE  0CFH
CML0    DEFINE  0D2H
CMH0    DEFINE  0D3H
CML1    DEFINE  0D4H
CMH1    DEFINE  0D5H
CML2    DEFINE  0D6H
CMH2    DEFINE  0D7H
CML3    DEFINE  0E2H
CMH3    DEFINE  0E3H
CML4    DEFINE  0E4H
CMH4    DEFINE  0E5H
CML5    DEFINE  0E6H
CMH5    DEFINE  0E7H
CML6    DEFINE  0F2H
CMH6    DEFINE  0F3H
CML7    DEFINE  0F4H
CMH7    DEFINE  0F5H

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

P6      DEFINE  0FAH
P7      DEFINE  0DBH            ; Analog/Digital Input
P8      DEFINE  0DDH            ; Analog/Digital Input, 4 Bit

; Serial Channels

S0CON   DEFINE  098H
RI0	DEFINE	S0CON.0
TI0	DEFINE	S0CON.1
RB80	DEFINE	S0CON.2
TB80	DEFINE	S0CON.3
REN0	DEFINE	S0CON.4
SM20	DEFINE	S0CON.5

S0BUF   DEFINE  099H
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

S1CON   DEFINE  09BH
S1BUF   DEFINE  09CH
S1REL   DEFINE  09DH

; A/D converter

ADCON1  DEFINE  0DCH
ADDAT   DEFINE  0D9H
DAPR    DEFINE  0DAH

; Mul/Div unit

ARCON   DEFINE  0EFH
MD0     DEFINE  0E9H
MD1     DEFINE  0EAH
MD2     DEFINE  0EBH
MD3     DEFINE  0ECH
MD4     DEFINE  0EDH
MD5     DEFINE  0EEH

; Interrupt system

IEN0    DEFINE  0A8H
ES0	DEFINE	IEN0.4
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

IEN2    DEFINE  09AH
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

; Fail save mechanisms

WDTREL  DEFINE  086H

; Interrupt vectors

SINT0	DEFINE	023H	;Serial channel 0 interrupt
ET2I    DEFINE  02BH    ;Timer 2 interrupt vector
IADCV   DEFINE  043H    ;A/D converter interrupt vector
IEX2V   DEFINE  04BH    ;External Interrupt 2 vector
IEX3V   DEFINE  053H    ;External Interrupt 3 vector
IEX4V   DEFINE  05BH    ;External Interrupt 4 vector
IEX5V   DEFINE  063H    ;External Interrupt 5 vector
IEX6V   DEFINE  06BH    ;External Interrupt 6 vector
SINT1	DEFINE	083H	;Serial channel 1 interrupt
ICTIM	DEFINE	09BH	;Compare timer overflow





