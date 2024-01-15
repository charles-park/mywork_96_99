;			- SFR552.INC -
; Siemens 80552/562
; Special Function Register definition for IAR Systems A8051 Assembler
;

; CPU

F1      DEFINE  0D1H

; A/D converter

ADCH    DEFINE  0C6H    ;A/D Converter Data Register
ADCON   DEFINE  0C5H    ;A/D Converter Program Register

; Capture

CTCON	DEFINE 	0EBH	; Capture control
CTH3	DEFINE	0CFH	; Capture high 3
CTH2	DEFINE	0CEH	; Capture high 2
CTH1	DEFINE	0CDH	; Capture high 1
CTH0	DEFINE	0CCH	; Capture high 0
CMH2	DEFINE	0CBH	; Compare high 2
CMH1	DEFINE	0CAH	; Compare high 1
CMH0	DEFINE	0C9H	; Compare high 0
CTL3	DEFINE	0AFH	; Capture low 3
CTL2	DEFINE	0AEH	; Capture low 2
CTL1	DEFINE	0ADH	; Capture low 1
CTL0	DEFINE	0ACH	; Capture low 0
CML2	DEFINE	0ABH	; Compare low 2
CML1	DEFINE	0AAH	; Compare low 1
CML0	DEFINE	0A9H	; Compare low 0

; Interrupt system

IEN0	DEFINE  0A8H    ;Interrupt Enable Register 0
EAD     DEFINE  IEN0.6  ;Enable A/D interrupt
ES1     DEFINE  IEN0.5  ;Enable SIO1 interrupt
ES0     DEFINE  IEN0.4  ;Enable SIO0 interrupt

IEN1    DEFINE  0E8H    ;Interrupt Enable Register 1
ET2     DEFINE  IEN1.7  ;Enable timer T2 overflow interrupt
ECM2    DEFINE  IEN1.6  ;Enable T2 comparator 2 interrupt
ECM1    DEFINE  IEN1.5  ;Enable T2 comparator 1 interrupt
ECM0    DEFINE  IEN1.4  ;Enable T2 comparator 0 interrupt
ECT3    DEFINE  IEN1.3  ;Enable T2 capture register 3 interrupt
ECT2    DEFINE  IEN1.2  ;Enable T2 capture register 2 interrupt
ECT1    DEFINE  IEN1.1  ;Enable T2 capture register 1 interrupt
ECT0    DEFINE  IEN1.0  ;Enable T2 capture register 0 interrupt

IP0     DEFINE  0B8H    ;Interrupt Priority Register 0
PAD	DEFINE	IP0.6	;A/D interrupt priority level
PS1	DEFINE	IP0.5	;SIO1 interrupt priority level
PS0	DEFINE	IP0.4	;SIO0 interrupt priority level

IP1     DEFINE  0F8H    ;Interrupt Priority Register 1
PT2	DEFINE	IP1.7	;Timer T2 overflow interrupt(s) priority level
PCM2	DEFINE	IP1.6	;Timer T2 comparator 2 interrupt priority level
PCM1	DEFINE	IP1.5	;Timer T2 comparator 1 interrupt priority level
PCM0	DEFINE	IP1.4	;Timer T2 comparator 0 interrupt priority level
PCT3	DEFINE	IP1.3	;Timer T2 capture register 3 interrupt priority level
PCT2	DEFINE	IP1.2	;Timer T2 capture register 2 interrupt priority level
PCT1	DEFINE	IP1.1	;Timer T2 capture register 1 interrupt priority level
PCT0	DEFINE	IP1.0	;Timer T2 capture register 0 interrupt priority level 

; Ports

; P1 bits
SDA	DEFINE	P1.7	;Serial port data line I2C bus
SCL	DEFINE	P1.6	;Serial port clock line I2C bus
RT2	DEFINE	P1.5	;T2 timer reset signal
T2	DEFINE	P1.4	;T2 event input
CT3I	DEFINE	P1.3	;Capture timer T2 input signal 3
CT2I	DEFINE	P1.2	;Capture timer T2 input signal 2
CT1I	DEFINE	P1.1	;Capture timer T2 input signal 1
CT0I	DEFINE	P1.0	;Capture timer T2 input signal 0

P4      DEFINE  0C0H    ;Port 4

P5      DEFINE  0C4H    ;Port 5

; Serial Channels

PCON    DEFINE  087H    ;Power down register

; PWM

PWMP	DEFINE	0FEH	; PWM prescaler
PWM1	DEFINE 	0FDH	; PWM register 1
PWM0	DEFINE 	0FCH	; PWM register 0

; RTE

RTE	DEFINE	0EFH	; Reset/Toggle enable

; Serial I/O

S0BUF	DEFINE	099H	; Serial 0 data buffer
S0CON	DEFINE	098H	; Serial 0 control
S1ADR	DEFINE	0DBH	; Serial 1 address
S1DAT	DEFINE 	0DAH	; Serial 1 data buffer
S1STA	DEFINE	0D9H	; Serial 1 status
S1CON	DEFINE	0D8H	; Serial 1 control
CR2	DEFINE	S1CON.7	;Clock rate bit 2
ENS1	DEFINE	S1CON.6	;SIO1 enable bit
STA	DEFINE	S1CON.5	;START flag
STO	DEFINE	S1CON.4	;STOP flag
SI	DEFINE	S1CON.3	;Serial interrupt request
AA	DEFINE	S1CON.2	;Assert acknowledge flag
CR1	DEFINE	S1CON.1	;Clock rate bit 1
CR0	DEFINE	S1CON.0	;Clock rate bit 0

; STE

STE	DEFINE	0EEH	; Set enable

; Timers

TMH2	DEFINE	0EDH	; Timer high 2
TML2	DEFINE	0ECH	; Timer low 2
TM2CON	DEFINE	0EAH	; Timer 2 control
T2IS1	DEFINE	TM2CON.7 ;Timer T2 16-bit overflow interrupt select
T2IS0	DEFINE	TM2CON.6 ;Timer T2 byte overflow interrupt select
T2ER	DEFINE	TM2CON.5 ;Timer T2 external reset enable
T2B0	DEFINE	TM2CON.4 ;Timer T2 byte overflow interrupt flag
T2P1	DEFINE	TM2CON.3 ;Timer T2 prescaler select
T2P0	DEFINE	TM2CON.2 ;Timer T2 prescaler select
T2MS1	DEFINE	TM2CON.1 ;Timer T2 mode select
T2MS0	DEFINE	TM2CON.0 ;Timer T2 mode select

TM2IR	DEFINE	0C8H	; Timer 2 interrupt flag register

T3	DEFINE	0FFH	; Timer 3

; Interrupt vectors

SIO0I	DEFINE	023H	; UART interrupt vector
SIO1I	DEFINE	02BH	; I2C interrupt vector
T2CT0I	DEFINE	033H	; Timer 2 capture 0 interrupt vector
T2CT1I	DEFINE	03BH	; Timer 2 capture 1 interrupt vector
T2CT2I	DEFINE	043H	; Timer 2 capture 2 interrupt vector
T2CT3I	DEFINE	04BH	; Timer 2 capture 3 interrupt vector
ADCI	DEFINE	053H	; A/D completion interrupt vector
T2CM0I	DEFINE	05BH	; Timer 2 compare 0 interrupt vector
T2CM1I	DEFINE	063H	; Timer 2 compare 1 interrupt vector
T2CM2I	DEFINE	06BH	; Timer 2 compare 2 interrupt vector
T2I	DEFINE	073H	; Timer 2 overflow interrupt vector










