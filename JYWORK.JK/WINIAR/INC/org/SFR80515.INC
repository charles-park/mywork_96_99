;			- SFR80515.INC -
; Siemens 80515
; Special Function Register definition for IAR Systems A8051 Assembler
;

; CPU

F1      DEFINE  0D1H

; CCU

T2CON	DEFINE  0C8H    ;Timer 2 control register
T2I0    DEFINE  T2CON.0 ;Timer 2 input selection bit 0
T2I1    DEFINE  T2CON.1 ;Timer 2 input selection bit 1
T2CM    DEFINE  T2CON.2 ;Compare mode bit
T2R0    DEFINE  T2CON.3 ;Timer 2 reload mode select bit 0
T2R1    DEFINE  T2CON.4 ;Timer 2 reload mode select bit 1
I2FR    DEFINE  T2CON.5 ;External Interrupt 2 Rise/Fall select bit
I3FR    DEFINE  T2CON.6 ;External Interrupt 3 Rise/Fall select bit
T2PS    DEFINE  T2CON.7 ;Prescaler select bit

TH2     DEFINE  0CDH    ;Timer 2 counter high byte
TL2     DEFINE  0CCH    ;Timer 2 counter low byte
CCEN    DEFINE  0C1H    ;Compare/Capture Enable Register
CRCL	DEFINE	0CAH
CRCH	DEFINE	0CBH
CCL1    DEFINE  0C2H    ;Compare/Capture Register 1, Low Byte
CCH1    DEFINE  0C3H    ;Compare/Capture Register 1, High Byte
CCL2    DEFINE  0C4H    ;Compare/Capture Register 2, Low Byte
CCH2    DEFINE  0C5H    ;Compare/Capture Register 2, High Byte
CCL3    DEFINE  0C6H    ;Compare/Capture Register 3, Low Byte
CCH3    DEFINE  0C7H    ;Compare/Capture Register 3, High Byte

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

P6	DEFINE	0DBH

; Serial Channels

PCON    DEFINE  087H    ;Power down register

ADCON   DEFINE  0D8H    ;A/D Converter Control Register
MX0     DEFINE  ADCON.0 ;A/D input channel selection bit 0
MX1     DEFINE  ADCON.1 ;A/D input channel selection bit 1
MX2     DEFINE  ADCON.2 ;A/D input channel selection bit 2
ADM     DEFINE  ADCON.3 ;A/D conversion mode
BSY     DEFINE  ADCON.4 ;A/D busy bit
BD      DEFINE  ADCON.7 ;Baud rate enable

; A/D converter

ADDAT   DEFINE  0D9H    ;A/D Converter Data Register
DAPR    DEFINE  0DAH    ;A/D Converter Program Register

; Interrupt system

IEN0	DEFINE  0A8H    ;Interrupt Enable Register 0
ET2     DEFINE  IEN0.5  ;Timer 2 overflow interrupt enable
WDT     DEFINE  IEN0.6  ;Watchdog timer reset flag
EAL     DEFINE  IEN0.7  ;Master interrupt enable

IEN1    DEFINE  0B8H    ;Interrupt Enable Register 1
EADC    DEFINE  IEN1.0  ;A/D converter interrupt enable
EX2     DEFINE  IEN1.1  ;External interrupt 2 interrupt enable
EX3     DEFINE  IEN1.2  ;External interrupt 3 interrupt enable
EX4     DEFINE  IEN1.3  ;External interrupt 4 interrupt enable
EX5     DEFINE  IEN1.4  ;External interrupt 5 interrupt enable
EX6     DEFINE  IEN1.5  ;External interrupt 6 interrupt enable
SWDT    DEFINE  IEN1.6  ;Watchdog timer start/reset bit
EXEN2   DEFINE  IEN1.7  ;Timer 2 external reload interrupt enable

IP0     DEFINE  0A9H    ;Interrupt Priority Register 0
IP1      DEFINE  0B9H    ;Interrupt Priority Register 1

IRCON    DEFINE  0C0H    ;Interrupt Request Control Register
IADC     DEFINE  IRCON.0 ;A/D converter interrupt request flag
IEX2     DEFINE  IRCON.1 ;External interrupt 2 edge flag
IEX3     DEFINE  IRCON.2 ;External interrupt 3 edge flag
IEX4     DEFINE  IRCON.3 ;External interrupt 4 edge flag
IEX5     DEFINE  IRCON.4 ;External interrupt 5 edge flag
IEX6     DEFINE  IRCON.5 ;External interrupt 6 edge flag
TF2      DEFINE  IRCON.6 ;Timer 2 overflow flag
EXF2     DEFINE  IRCON.7 ;Timer 2 external reload flag

; Interrupt vectors

ET2I     DEFINE  02BH    ;Timer 2 interrupt vector
IADCV    DEFINE  043H    ;A/D converter interrupt vector
IEX2V    DEFINE  04BH    ;External Interrupt 2 vector
IEX3V    DEFINE  053H    ;External Interrupt 3 vector
IEX4V    DEFINE  05BH    ;External Interrupt 4 vector
IEX5V    DEFINE  063H    ;External Interrupt 5 vector
IEX6V    DEFINE  06BH    ;External Interrupt 6 vector









