;			- SFR751.INC -
;  This file contains symbol definitions for Special Function
;  Registers in the 80751 chip.
;


; I2C

I2CFG	DEFINE	0D8H	;I2C configuration register
SLAVEN	DEFINE	I2CFG.7	;I2C slave enable
MASTRQ	DEFINE	I2CFG.6	;I2C master request
CLRT1	DEFINE	I2CFG.5	;I2C clear timer 1 interrupt flag
T1RUN	DEFINE	I2CFG.4	;I2C let timer 1 run
CT1	DEFINE	I2CFG.1	;I2C OSC rate
CT0	DEFINE	I2CFG.0	;I2C OSC rate


I2CON	DEFINE	098H	;I2C control register
RDAT_C 	DEFINE 	I2CON.7 ;I2C RD	received data
ATN	DEFINE	I2CON.6	;I2C RD	attention
DRDY	DEFINE	I2CON.5	;I2C RD	data ready
ARL	DEFINE	I2CON.4	;I2C RD	arbitration loss
STR	DEFINE	I2CON.3	;I2C RD	start condition detected
STP	DEFINE	I2CON.2	;I2C RD	stop condition detected
MASTER	DEFINE	I2CON.1	;I2C RD	is master

CXA	DEFINE	I2CON.7	;I2C WR	clear xmit active
IDLE_C	DEFINE	I2CON.6	;I2C WR	idle until next start condition
CDR	DEFINE	I2CON.5	;I2C WR	clear data ready
CARL	DEFINE	I2CON.4	;I2C WR	clear arbitration
CSTR	DEFINE	I2CON.3	;I2C WR	clear start
CSTP	DEFINE	I2CON.2	;I2C WR	clear stop
XSTR_C	DEFINE	I2CON.1	;I2C WR	xmit repeated start
XSTP_C	DEFINE	I2CON.0	;I2C WR	xmit stop


I2DAT	DEFINE 	099H	;I2C data register
RDAT_D	DEFINE	I2DAT.7	;I2C RD receive data

XDAT	DEFINE	I2DAT.7	;I2C WR xmit data


I2STA	DEFINE	0F8H	;I2C control register
IDLE_S	DEFINE 	I2STA.6	;I2C RD idle until next start condition
XDATA	DEFINE	I2STA.5	;I2C RD content of transmitt buffer
XACTV	DEFINE	I2STA.4	;I2C RD transmitter active
MAKSTR	DEFINE	I2STA.3	;I2C RD effecting start condition
MAKSTP	DEFINE	I2STA.2	;I2C RD effecting repeated start condition
XSTR_S	DEFINE	I2STA.1	;I2C RD xmit repeated start
XSTP_S	DEFINE	I2STA.0	;I2C RD xmit stop

; Ports

SDA	DEFINE  P0.1    ;I2C data
SCL    	DEFINE  P0.0    ;I2C clock

; Serial Channels

PCON    DEFINE  087H    ;Power down register
IDL	DEFINE	PCON.0	
PD	DEFINE	PCON.1

; Timer control
GATE	DEFINE	TCON.7	;TCON timer/counter enable control
CT	DEFINE	TCON.6	;TCON timer/counter operation control
TF	DEFINE	TCON.5	;TCON overflow of TH
TR	DEFINE	TCON.4	;TCON timer/counter enabled

; Interrupt system

EI2     DEFINE  IE.4    ;I2C interrupt enable bit

; Interrupt vectors

I2CI    DEFINE  023H    ;I2C interrupt vector



