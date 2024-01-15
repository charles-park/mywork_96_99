PAGE 80,132
TITLE SPDRIVE  --  Buffered Interrupt Driven Serial Port Driver
;--------------------------------------------------------------------
;
;		Author:		Paul Braisted
;		Last Edit:	November 28, 1988
;		Version:	2.10
;
;	This program provides an interrupt driven, buffered interface
;	to the IBM PC serial (RS-232) port.  It is a .COM program
;	which attaches itself to the BIOS RS232_IO interrupt 14 hex.
;	Running SPDRIVE attaches to COM1, SPDRIVE /2 attaches to COM2.
;	Calls are as follows:
;
;	AH=04h	Enable serial port
;		takes	AL=port parameters, as used by BIOS RS232_IO
;
;			76543210
;			BBBPPSLL	BBB*32 + PP*8 + S*4 + LL
;
;			BBB      = baud rate code      0=110,  1=19200, 2=300,
;				      3=38400, 4=1200, 5=2400, 6=4800, 7=9600
;			   PP    = parity code
;					0=none, 1=odd, 3=even
;			     S	 = stop bits
;					0=one, 1=two
;			      LL = word length
;					2=seven, 3=eight
;
;		takes	DL=0	use XON/XOFF for flow control
;			   1	use RTS/CTS for flow control
;
;		returns	AH=1 if successful, else 0
;
;	AH=05h	Disable serial port
;		returns	AH=1 if successful, else 0
;
;	AH=06h	Get character (from Rx buffer)
;		returns	AL=character
;		returns	AH=1 if successful, else 0
;
;	AH=07h	Send character (place in Tx buffer)
;		takes	AL=character
;		returns	AH=1 if successful, else 0
;
;	AH=08h	Send line "break"
;		returns	AH=1 if successful, else 0
;
;	AH=09h	Set Flow Control
;		takes	DH=0	transmit flow control off
;			   1	transmit flow control on
;			DL=0	receive flow control off	
;			   1	receive flow control on	
;		returns	AH=1 if successful, else 0
;
;	AH=0Ah	Get installation information
;		returns	BX = segemnt address
;			DH = major version number
;			DL = minor version number
;
;	AH=0Bh	Get port information
;		returns	BX = I/O port base address
;			DH = COM number, or 0 if special
;			DL = IRQ number
;
;	NOTE:  Registers AX, BX, CX, and DX are not preserved.
;
;--------------------------------------------------------------------
page

;			Version History

;	2.10	11/28/88	added version number test to already-installed
;					check (for "Quadtel" clone BIOS)


COM1ADR	EQU	03F8h		; COM1 serial port address
COM2ADR	EQU	02F8h		; COM2 serial port address

IRQ2MSK	EQU	11111011b	; 8259 level 2 interrupt mask
IRQ3MSK	EQU	11110111b	; 8259 level 3 interrupt mask (COM2)
IRQ4MSK	EQU	11101111b	; 8259 level 4 interrupt mask (COM1)
IRQ5MSK	EQU	11011111b	; 8259 level 5 interrupt mask

IERBITS	EQU	00000011b	; Interrupt Enable Register bits
MCRBITS	EQU	00001011b	; DTR, RTS, and OUT2 (INTRPT) enabled
RTSBIT	EQU	00000011b	; RTS & DTR control bit in MCR
CTSBIT	EQU	00010000b	; CTS status bit in MSR

EOI	EQU	20h		; End Of Interrupt command to 8259

XONCHAR	EQU	11h	; DC1 (XON) ASCII character
XOFCHAR	EQU	13h	; DC3 (XOFF) ASCII character


CSEG	SEGMENT PARA PUBLIC 'CODE'
	ASSUME	CS:CSEG,DS:CSEG,ES:CSEG

	ORG	100h		; origin for a .COM program

START	PROC	FAR
	JMP	INIT_SPORT	; go install driver
page
;------------------------
;	Data Areas
;------------------------

SPD_ON		DB	0	; 1 if driver active, else 0

CALL_OFFSET	DB	0	; command code offset, 0 or 10h

CNTDWN 		DW	0	; count-down timer storeage
PORTPARM	DB	11101111b	; serial port parameter byte

COMNUM		DB	1	; 1 for COM1, 2 for COM2
IRQNUM		DB	4	; 4 for COM1, 3 for COM2
INTMASK		DB	IRQ4MSK	; 8259 serial port interrupt mask

PORTADDR	DW	COM1ADR	; serial port base address
PORT_IIR	DW	?	; Interrupt Identification Register
PORT_LCR	DW	?	; Line Control Register
PORT_MCR	DW	?	; Modem Control Register
PORT_LSR	DW	?	; Line Status Register
PORT_MSR	DW	?	; Modem Status Register

BIOS_SPD	DD	?	; place to save BIOS interrupt vector 14h
TISAVE		DD	?	; place to save old timer interrupt vector

RXF_STATE	DB	0	; receive flow control state
TXF_STATE	DB	0	; transmit flow control state
				;	00h = free (no flow control)
				;	01h = run
				;	11h = hold
FLOW_TYPE	DB	0	; flow control type (0=XON/XOFF, 1=RTS/CTS)

TXFORCE		DB	0	; character to transmit immediately!

RXBUFLEN	EQU	256	; Rx buffer length
RXTRIPON	EQU	128	; trip level for Rx back on
RXTRIPOFF	EQU	32	; trip level for Rx off (XOFF)
RXBUF_HEAD	DW	?	; pointer to head of Rx buffer
RXBUF_TAIL	DW	?	; pointer to tail of Rx buffer
RXBUF_ROOM	DW	?	; room remaining in Rx buffer
RXBUFFER	DB	RXBUFLEN DUP(?)
RXBUF_END	LABEL	WORD

TXBUFLEN	EQU	256	; Tx buffer length
TXBUF_HEAD	DW	?	; pointer to head of Tx buffer
TXBUF_TAIL	DW	?	; pointer to tail of Tx buffer
TXBUF_ROOM	DW	?	; room remaining in Tx buffer
TXBUFFER	DB	TXBUFLEN DUP(?)
TXBUF_END	LABEL	WORD
page
;---------------------------------------------------------------
;	Interrupt 14h capture routine.  If call is for SPDRIVE,
;	jumps to desired function.  If for BIOS, go to it.
;---------------------------------------------------------------

SPD_TAB	LABEL	WORD		; command jump vector table
	DW	OFFSET	SER_RESET	; AH=04h
	DW	OFFSET	SER_OFF		; AH=05h
	DW	OFFSET	GET_RX		; AH=06h
	DW	OFFSET	PUT_TX		; AH=07h
	DW	OFFSET	SEND_BRK	; AH=08h
	DW	OFFSET	SET_FLOW	; AH=09h
	DW	OFFSET	GET_I_INFO	; AH=0Ah
	DW	OFFSET	GET_P_INFO	; AH=0Bh
FIRSTAH	EQU	04h		; lowest value for command (AH)
LASTAH	EQU	0Bh		; highest value for command (AH)


SP_DRIVE	PROC	FAR
	SUB	AH,CS:CALL_OFFSET
	CMP	AH,FIRSTAH	; see if call is for us
	JB	SPD1		; if not, go to BIOS routine
	CMP	AH,LASTAH
	JBE	SPD2		; if so, go process it
SPD1:	ADD	AH,CS:CALL_OFFSET	; if not, go to BIOS routine
	JMP	CS:BIOS_SPD

SPD2:	STI			; allow further interrupts
	PUSH	DS		; save segment registers
	PUSH	ES

	MOV	BX,CS		; establish Data Segment
	MOV	DS,BX
	SUB	AH,4		; calculate jump table offset
	MOV	BL,AH
	MOV	BH,0
	SAL	BX,1
	JMP	SPD_TAB[BX]	; go to selected routine

SPD_RET:
	POP	ES		; restore registers
	POP	DS
	IRET
SP_DRIVE	ENDP
page
;-------------------------------------------------------------
;	Reset and Enable Serial Port
;-------------------------------------------------------------
;		takes	AL=port parameter byte
;			DL=0	use XON/XOFF for flow control
;			   1	use RTS/CTS for flow control
;		returns	AH=1 if successful, else 0
;-------------------------------------------------------------
	; divisor values for baud rate generator
BAUD_TAB	LABEL	BYTE
	DW	1047	; 0 = 110 baud
	DW	6	; 1 = 19200
	DW	384	; 2 = 300
	DW	3	; 3 = 38400
	DW	96	; 4 = 1200
	DW	48	; 5 = 2400
	DW	24	; 6 = 4800
	DW	12	; 7 = 9600

SER_RESET:
	MOV	SPD_ON,0	; deactivate driver
	MOV	PORTPARM,AL	; save port parameters
	MOV	FLOW_TYPE,DL	; set flow control type

	;  Mask interrupts from the serial chip

	IN	AL,21h		; read in 8259 IMR
	JMP	SHORT $+2	; I/O delay
	MOV	AH,INTMASK
	NOT	AH
	OR	AL,AH		; disable interrupt
	OUT	21h,AL		; write it out

	;  Program serial port 8250 chip

	MOV	DX,PORT_LCR	; select Line Control Register
	MOV	AL,80h		; set DLAB bit to 1
	OUT	DX,AL
	MOV	BL,PORTPARM	; recall port parameters
	AND	BX,0E0h		; mask for baud rate code
	MOV	CL,4		; make it an index into table
	SHR	BX,CL
	DEC	DX		; select MSByte of Divisor
	DEC	DX
	MOV	AL,BAUD_TAB[BX]+1	; get MSByte from table
	OUT	DX,AL
	DEC	DX		; select LSByte of Divisor
	JMP	SHORT $+2	; I/O delay
	MOV	AL,BAUD_TAB[BX]	; get LSByte from table
	OUT	DX,AL

	ADD	DX,3		; select Line Control Register
	JMP	SHORT $+2	; I/O delay
	MOV	AL,PORTPARM	; recall port parameters
	AND	AL,1Fh		; strip off baud bits and DLAB=0
	OUT	DX,AL

	DEC	DX		; select Interrupt Enable Register
	DEC	DX
	JMP	SHORT $+2	; I/O delay
	MOV	AL,IERBITS	; enable desired serial port interrupts
	OUT	DX,AL
	ADD	DX,3		; select Modem Control Register
	JMP	SHORT $+2	; I/O delay
	MOV	AL,MCRBITS	; enable DTR, RTS, and OUT2 (INTRPT)
	OUT	DX,AL

	;  Make sure serial port interrupt vector is set

	MOV	AH,25h		; DOS set interrupt vector function
	MOV	AL,IRQNUM	; get 8259 interrupt level
	ADD	AL,8		; add offset of 8259 interrupts
	MOV	DX,OFFSET SER_INT	; get address of handler
	INT	21h		; call DOS

	IN	AL,21h		; read in 8259 IMR
	JMP	SHORT $+2	; I/O delay
	AND	AL,INTMASK	; enable interrupt
	OUT	21h,AL		; write it out

	;  Clear the serial chip status

	MOV	DX,PORT_IIR	; select Interrupt Ident. Register
	IN	AL,DX		; read IIR to clear THRE interrupt
	JMP	SHORT $+2	; I/O delay
	MOV	DX,PORTADDR	; select data register
	IN	AL,DX		; do 2 dummy reads to clear status
	JMP	SHORT $+2	; I/O delay
	IN	AL,DX

	;  Clear driver status and buffers

	MOV	RXF_STATE,0	; disable receive flow control
	MOV	TXF_STATE,0	; disable transmit flow control
	MOV	TXFORCE,0	; nothing to transmit immediately
	CALL	RX_CLEAR	; clear receive buffer
	CALL	TX_CLEAR	; clear transmit buffer

	MOV	SPD_ON,1	; activate driver

	MOV	AL,PORTPARM	; return baud rate code in AL
	MOV	AH,1		; signal success
	JMP	SPD_RET		; go return
page
;--------------------------------------------------
;	Turn Off Serial Port and Interrupts
;--------------------------------------------------
;		returns	AH=1 if successful, else 0
;--------------------------------------------------
SER_OFF:

	;  Mask interrupts from the serial chip

	IN	AL,21h		; read in 8259 IMR
	JMP	SHORT $+2	; I/O delay
	MOV	AH,INTMASK
	NOT	AH
	OR	AL,AH		; disable interrupt
	OUT	21h,AL		; write it out

	;  Disable the serial chip

	MOV	DX,PORTADDR	; select Interrupt Enable Register
	INC	DX
	MOV	AL,0		; disable all RS232 interrupts
	OUT	DX,AL
	JMP	SHORT $+2	; I/O delay
	ADD	DX,3		; select Modem Control Register
	MOV	AL,0		; disable everything
	OUT	DX,AL

	MOV	SPD_ON,0	; deactivate driver

	MOV	AL,PORTPARM	; return with AL=port parameters
	MOV	AH,1		; signal success
	JMP	SPD_RET		; go return
page
;--------------------------------------------------
;	Send a Line Break on Serial Port
;--------------------------------------------------
;		returns	AH=1 if successful, else 0
;--------------------------------------------------
SEND_BRK:
	CMP	SPD_ON,1	; check if driver active
	JE	SB2		; if so, go on
	MOV	AH,0		; if not, failed
	JMP	SPD_RET		; go return

SB2:	MOV	DX,PORT_LCR	; read Line Control Register
	IN	AL,DX
	JMP	SHORT $+2	; I/O delay
	OR	AL,40h		; set Break bit
	OUT	DX,AL
	MOV	CNTDWN,6	; wait about .25 to .3 second
SB4:	CMP	CNTDWN,0
	JNE	SB4
	IN	AL,DX
	JMP	SHORT $+2	; I/O delay
	AND	AL,NOT 40h	; turn off Break bit
	OUT	DX,AL

	MOV	AH,1		; signal success
	JMP	SPD_RET		; go return


;--------------------------------------------------------
;	Set Flow Control State
;--------------------------------------------------------
;		takes	DH=0	transmit flow control off
;			   1	transmit flow control on
;			DL=0	receive flow control off	
;			   1	receive flow control on	
;		returns	AH=1 if successful, else 0
;--------------------------------------------------------
SET_FLOW:
	CMP	SPD_ON,1	; check if driver active
	JE	STF2		; if so, go on
	MOV	AH,0		; if not, failed
	JMP	SPD_RET		; go return

STF2:	MOV	AL,DH		; set transmit side
	CALL	TX_FLOWSET
	MOV	AL,DL		; set receive side
	CALL	RX_FLOWSET

	MOV	AH,1		; signal success
	JMP	SPD_RET		; go return
page
;--------------------------------------------------
;	Get a Character from Receive Buffer
;--------------------------------------------------
;		returns AL=character
;			AH=1 if successful, else 0
;--------------------------------------------------
GET_RX:
	CMP	SPD_ON,1	; check if driver active
	JE	GRX1		; if so, go on
	MOV	AH,0		; AH=0, no character
	JMP	SPD_RET		; go return

GRX1:	CLI			; disable interrupts
	MOV	BX,RXBUF_HEAD	; get pointer to head of buffer
	CMP	BX,RXBUF_TAIL	; test end of buffer
	JE	GRX8		; if nothing, tell caller
	MOV	AL,[BX]		; get character
	CALL	INCRXP		; move pointer to next position
	MOV	RXBUF_HEAD,BX	; store value in variable
	INC	RXBUF_ROOM	; room for another character
	CMP	RXBUF_ROOM,RXTRIPON	; does this trip flow on?
	JB	GRX2		; if not, bypass
	CMP	RXF_STATE,11h	; check if in hold state
	JNE	GRX2		; if not, bypass
	MOV	RXF_STATE,01h	; set flow state to run
	CMP	FLOW_TYPE,0	; do selected type of flow control
	JE	GRX3
	MOV	BL,AL		; save the character in BL
	MOV	DX,PORT_MCR	; get Modem Control Status Register
	IN	AL,DX
	JMP	SHORT $+2	; I/O delay
	OR	AL,RTSBIT	; turn on RTS signal
	OUT	DX,AL
	MOV	AL,BL		; recover the character into AL
	JMP	GRX2
GRX3:	MOV	TXFORCE,XONCHAR	; send an XON immediately
GRX2:	STI			; interrupts back on
	MOV	AH,1		; AH=1, character in AL
	JMP	SPD_RET		; go return

GRX8:	STI			; interrupts back on
	MOV	AH,0		; AH=0, no character
	JMP	SPD_RET		; go return
page
;---------------------------------------------------
;	Put a Character into Transmit Buffer
;---------------------------------------------------
;		takes	AL=character
;		returns	AH=1 if successful, else 0
;---------------------------------------------------
PUT_TX:
	CMP	SPD_ON,1	; check if driver active
	JE	PTX1		; if so, go on
	MOV	AH,0		; AH=0, can't accept the character
	JMP	SPD_RET		; go return

PTX1:	CLI			; turn off interrupts
	MOV	BX,TXBUF_TAIL	; get the end pointer to the buffer
	MOV	SI,BX		; save the value
	CALL	INCTXP		; advance the tail
	CMP	BX,TXBUF_HEAD	; has the buffer wrapped around?
	JE	PTX8		; buffer full!
	MOV	[SI],AL		; store the value
	MOV	TXBUF_TAIL,BX	; move the pointer up
	DEC	TXBUF_ROOM	; room for one less character

	CALL	TRY_TX		; wake up transmitter if idle

	STI			; interrupts back on
	MOV	AH,1		; AH=1, successful PUT
	JMP	SPD_RET		; go return

PTX8:	STI			; interrupts back on
	MOV	AH,0		; AH=0, failed PUT
	JMP	SPD_RET		; go return
page
;---------------------------------------------------
;	Get installation information
;---------------------------------------------------
;		returns	AH=1
;			BX=SPDRIVE segment address
;			DH=major version number
;			DL=minor version number
;---------------------------------------------------
GET_I_INFO:
	MOV	BX,CS		; SPDRIVE segment address
	MOV	DH,2		; version 2.10
	MOV	DL,10
	MOV	AH,1		; AH=1, successful PUT
	JMP	SPD_RET		; go return


;-----------------------------------------------------
;	Get I/O port information
;-----------------------------------------------------
;		returns	AH=1
;			BX=I/O port base address
;			DH=COM number, or 0 if special
;			DL=IRQ number
;-----------------------------------------------------
GET_P_INFO:
	MOV	BX,PORTADDR	; I/O port base address
	MOV	DH,COMNUM	; COM number
	MOV	DL,IRQNUM	; IRQ number
	MOV	AH,1		; AH=1, successful PUT
	JMP	SPD_RET		; go return
page
;---------------------------------------------------------
;	Turn Receive Flow Control ON (AL=1) or OFF (AL=0)
;---------------------------------------------------------
RX_FLOWSET	PROC	NEAR
	CMP	AL,0		; check if turning OFF
	JE	RXF2		; if so, just go do it
	CMP	RXF_STATE,0	; check for OFF to ON transition
	JNE	RXF9		; if not, then leave alone
RXF2:	MOV	RXF_STATE,AL	; set receive flow control
	CMP	FLOW_TYPE,0
	JE	RXF9
	MOV	DX,PORT_MCR	; if RTS/CTS flow control, turn RTS on
	IN	AL,DX
	JMP	SHORT $+2	; I/O delay
	OR	AL,RTSBIT
	OUT	DX,AL
RXF9:	RET
RX_FLOWSET	ENDP

;----------------------------
;	Clear Receive Buffer
;----------------------------
RX_CLEAR	PROC	NEAR
	PUSH	AX			; save AX
	CLI				; turn off interrupts
	MOV	AX,OFFSET RXBUFFER	; reset Rx buffer to empty
	MOV	RXBUF_HEAD,AX
	MOV	RXBUF_TAIL,AX
	MOV	RXBUF_ROOM,RXBUFLEN-1	; plenty of room in buffer
	STI				; interrupts back on
	POP	AX			; restore AX
	RET
RX_CLEAR	ENDP

;------------------------------------------------
;	Increment Receive Buffer Pointer (in BX)
;------------------------------------------------
INCRXP	PROC	NEAR
	INC	BX			; move to next byte in list
	CMP	BX,OFFSET RXBUF_END	; at end of buffer?
	JNE	IR1			; no, continue
	MOV	BX,OFFSET RXBUFFER	; yes, reset to buffer beginning
IR1:	RET
INCRXP	ENDP
page
;----------------------------------------------------------
;	Turn Transmit Flow Control ON (AL=1) or OFF (AL=0)
;----------------------------------------------------------
TX_FLOWSET	PROC	NEAR
	MOV	TXF_STATE,AL	; transmit flow control and clear XOFF
	RET
TX_FLOWSET	ENDP

;-----------------------------
;	Clear Transmit Buffer
;-----------------------------
TX_CLEAR	PROC	NEAR
	PUSH	AX			; save AX
	CLI				; turn off interrupts
	MOV	AX,OFFSET TXBUFFER	; reset Tx buffer to empty
	MOV	TXBUF_HEAD,AX
	MOV	TXBUF_TAIL,AX
	MOV	TXBUF_ROOM,TXBUFLEN-1	; plenty of room in buffer
	STI				; interrupts back on
	POP	AX			; restore AX
	RET
TX_CLEAR	ENDP

;-------------------------------------------------
;	Increment Transmit Buffer Pointer (in BX)
;-------------------------------------------------
INCTXP	PROC	NEAR
	INC	BX			; move to next byte in list
	CMP	BX,OFFSET TXBUF_END	; at end of buffer?
	JNE	IT1			; no, continue
	MOV	BX,OFFSET TXBUFFER	; yes, reset to buffer beginning
IT1:	RET
INCTXP	ENDP
page
;------------------------------------------------
;	Serial Port Interrupt Processing Routine
;------------------------------------------------
SER_INT	PROC	FAR
	STI			; allow further interrupts
	PUSH	AX		; save registers
	PUSH	BX
	PUSH	DX
	PUSH	SI
	PUSH	DS
	MOV	AX,CS		; establish addressibility
	MOV	DS,AX
SER_TST:
	CLI			; turn off other interrupts (timer)
	MOV	DX,PORT_IIR	; read RS232 port Interrupt Ident. Regsiter
	IN	AL,DX
	CMP	AL,00000100b	; test for Received Data Available
	JE	SER_RX		; if set, go receive character
	CMP	AL,00000010b	; test for Transmitter Holding Register Empty
	JE	SER_TX		; if set, go transmit a character

SER_RET:
	CLI			; turn interrupts off
	MOV	AL,EOI
	OUT	20h,AL		; End Of Interrupt command to 8259
	POP	DS		; restore registers
	POP	SI
	POP	DX
	POP	BX
	POP	AX
	IRET			; return from interrupt

;	Receive a Character from RS-232 Serial Port into Buffer

SER_RX:	STI			; interrupts (timer) on for receive
	MOV	DX,PORTADDR	; receive a character (into AL)
	IN	AL,DX
	CMP	FLOW_TYPE,0	; check if using XON/XOFF
	JNE	SR2		; if not, don't check for them
	CMP	TXF_STATE,0	; check if flow control enabled
	JE	SR2		; if not, go store character
	CMP	AL,XOFCHAR	; check if character is an XOFF
	JE	SR1		; if so, go set flow state
	CMP	AL,XONCHAR	; check if character is an XON
	JNE	SR2		; if not, go store character
	MOV	TXF_STATE,01h	; set to run state
	JMP	SER_TST		; don't save flow control character

SR1:	MOV	TXF_STATE,11h	; set to hold state
	JMP	SER_TST		; don't save flow control character

SR2:	MOV	BX,RXBUF_TAIL	; get the end pointer to the buffer
	MOV	SI,BX		; save the value
	CALL	INCRXP		; advance the tail
	CMP	BX,RXBUF_HEAD	; has the buffer wrapped around?
	JE	SR3		; buffer full!
	MOV	[SI],AL		; store the value
	MOV	RXBUF_TAIL,BX	; move the pointer up
	DEC	RXBUF_ROOM	; room for one less character
	CMP	RXBUF_ROOM,RXTRIPOFF	; does this trip flow off?
	JA	SER_TST		; if not, go check for anything else
SR3:	CMP	RXF_STATE,01h	; check if XOFF-able (run state)
	JNE	SER_TST		; if not, go check for anything else
	MOV	RXF_STATE,11h	; set flow to hold state
	CMP	FLOW_TYPE,0	; do selected type of flow control
	JNE	SR4
	MOV	TXFORCE,XOFCHAR	; send an XOFF immediately
	JMP	SER_TST		; check if there is anything else

SR4:	MOV	DX,PORT_MCR	; get Modem Control Status Register
	IN	AL,DX
	JMP	SHORT $+2	; I/O delay
	AND	AL,NOT RTSBIT	; turn off RTS signal
	OUT	DX,AL
	JMP	SER_TST		; check if there is anything else

;	Transmit Buffered Characters

SER_TX:	CMP	TXFORCE,0	; check for immediate transmit data
	JE	ST1		; if none, go on
	MOV	AL,TXFORCE	; get the immediate data
	MOV	TXFORCE,0	; clear behind it
	JMP	ST2		; go send it

ST1:	CMP	TXF_STATE,0	; check if flow control enabled
	JE	ST8		; if not, go store character
	CMP	FLOW_TYPE,0	; check if using XON/XOFF
	JE	ST7		; if so, go check if in hold
	MOV	TXF_STATE,01h
	MOV	DX,PORT_MSR	; read Modem Status Register
	IN	AL,DX
	TEST	AL,CTSBIT	; check CTS signal
	JNZ	ST8		; if clear to send, go do it
	MOV	TXF_STATE,11h	; else, we are being held
	JMP	SER_TC
	
ST7:	CMP	TXF_STATE,11h	; check if TX is in hold state
	JE	SER_TC		; if so, just clear interrupt
ST8:	MOV	BX,TXBUF_HEAD	; get pointer to head of buffer
	CMP	BX,TXBUF_TAIL	; test end of buffer
	JE	SER_TC		; if nothing, just clear interrupt
	MOV	AL,[BX]		; get character
	CALL	INCTXP		; move pointer to next position
	MOV	TXBUF_HEAD,BX	; store value in variable
	INC	TXBUF_ROOM	; room for another character
ST2:	MOV	DX,PORTADDR	; select RS-232 port
	OUT	DX,AL		; transmit the character

SER_TC:	JMP	SER_TST		; check if there is anything else

SER_INT	ENDP
page
;------------------------------------
;	Timer Tick Interrupt Handler
;------------------------------------
;		(called by BIOS)
;------------------------------------
TIM_INT	PROC	FAR
	STI			; allow higher level (?) interrupts
	PUSH	AX		; save other registers used
	PUSH	BX
	PUSH	DX
	PUSH	DS
	MOV	AX,CS		; establish addressibility
	MOV	DS,AX

	CMP	SPD_ON,1	; check if driver active
	JNE	TI9		; if not, just return
	MOV	AX,CNTDWN	; get count-down value
	OR	AX,AX
	JZ	TI1		; if zero, bypass
	DEC	AX		; decrement it
	MOV	CNTDWN,AX	; store new value

TI1:	CALL	TRY_TX		; wake up transmitter if necessary

TI9:	POP	DS		; restore registers
	POP	DX
	POP	BX
	POP	AX
	CLI
	JMP	CS:TISAVE	; go do whatever was called by BIOS
				; timer tick interrupt before SPDRIVE
TIM_INT	ENDP
page
;------------------------------------
;	Wake up transmitter if idle
;------------------------------------
TRY_TX	PROC	NEAR

	MOV	DX,PORT_LSR	; get RS232 port Line Status Register
	IN	AL,DX
	TEST	AL,00100000b	; test THRE bit
	JZ	TTX9		; if not set, go return
	TEST	AL,01000000b	; test TSRE bit
	JZ	TTX9		; if not set, go return
	CMP	TXFORCE,0	; check for immediate transmit data
	JE	TTX2		; if none, bypass
	MOV	AL,TXFORCE	; get the immediate data
	MOV	TXFORCE,0	; clear out behind it
	JMP	TTX3		; go send it

TTX2:	CMP	TXF_STATE,0	; check if flow control enabled
	JE	TTX6		; if not, go store character
	CMP	FLOW_TYPE,0	; check if using XON/XOFF
	JE	TTX5		; if so, go check if in hold
	MOV	TXF_STATE,01h
	MOV	DX,PORT_MSR	; read Modem Status Register
	IN	AL,DX
	TEST	AL,CTSBIT	; check CTS signal
	JNZ	TTX6		; if clear to send, go do it
	MOV	TXF_STATE,11h	; else, we are being held
	JMP	TTX9

TTX5:	CMP	TXF_STATE,11h	; check if TX is in hold state
	JE	TTX9		; if so, go return
TTX6:	MOV	BX,TXBUF_HEAD	; get pointer to head of buffer
	CMP	BX,TXBUF_TAIL	; test end of buffer
	JE	TTX9		; if nothing, go return
	MOV	AL,[BX]		; get character
	CALL	INCTXP		; move pointer to next position
	MOV	TXBUF_HEAD,BX	; store value in variable
	INC	TXBUF_ROOM	; room for another character
TTX3:	MOV	DX,PORTADDR	; select RS-232 port
	OUT	DX,AL		; transmit the character

TTX9:	RET

TRY_TX	ENDP
page
;----------------------------------
;	Install Serial Port Driver
;----------------------------------
SPD_TOP		EQU	THIS BYTE

WEL_MESS	DB	'SPDRIVE  version 2.10', 0Dh, 0Ah, '$'
ND2_MESS	DB	'DOS 2.00 or later is required!', 07h, 0Dh, 0Ah, '$'
DAI_MESS	DB	'Serial port driver already installed!'
		DB	07h, 0Dh, 0Ah, 24h
ISP_MESS	DB	'Installing buffered serial port driver on COM'
IM_COM		DB	'1', 0Dh, 0Ah, '$'
ALT_MESS	DB	'as alternate logical port', 0Dh, 0Ah, '$'
BDS_MESS	DB	'Bad syntax or invalid option!', 07h, 0Dh, 0Ah, '$'

INIT_SPORT:
	LEA	DX,WEL_MESS	; display name and version tag
	MOV	AH,9
	INT	21h
	MOV	AH,30h		; get DOS version number
	INT	21h
	CMP	AL,2		; check if DOS 2.00 or later
	JAE	CHK_OPT		; if so, go check for option switches
	LEA	DX,ND2_MESS	; tell user that we need DOS 2.00
	MOV	AH,9
	INT	21h
	INT	20h		; don't install, return to DOS 1.xx

		;  check command line options
CHK_OPT:
	MOV	CL,DS:80h	; get command line count
	MOV	CH,0
	MOV	DI,81h		; point DI at command line
COLOOP:	MOV	AL,'/'
	CLD
	REPNE	SCASB		; scan for a '/' character
	JCXZ	INSTALL		; if nothing, go try to install
	MOV	AL,BYTE PTR [DI]
	CALL	CAPITALIZE
	CMP	AL,'1'		; check for a '1'
	JE	COLOOP		; if so, go check for more
	CMP	AL,'2'		; check for a '2'
	JNE	CO_L		; if not, go on
	MOV	COMNUM,2	; if so, select COM2 port,
	MOV	PORTADDR,COM2ADR	; base address,
	MOV	IRQNUM,3		; and IRQ3
	MOV	IM_COM,'2'
	JMP	COLOOP		; go check for more

CO_L:	CMP	AL,'L'		; check if 'L' option
	JNE	CO_A		; if not, go on
	MOV	CALL_OFFSET,10h	; if so, set command code offset
	JMP	COLOOP		; go check for more

CO_A:	CMP	AL,'A'		; check for 'A' option
	JNE	BAD_SYNTAX	; if not, bad option
	MOV	COMNUM,0	; if so, select special COM port,
	MOV	PORTADDR,03E8h		; base address,
	MOV	IRQNUM,2		; and IRQ2
	MOV	IM_COM,'?'
	JMP	COLOOP		; go check for more

BAD_SYNTAX:
	LEA	DX,BDS_MESS	; tell user about bad syntax
	MOV	AH,9
	INT	21h
	MOV	AX,4C01h	; exit with ERRORLEVEL = 1
	INT	21h

INSTALL:
	MOV	AH,5		; see if already installed
	ADD	AH,CALL_OFFSET		; by doing a SER_OFF call
	MOV	AL,0		; (valid BIOS sub-command, for PS/2 model 30)
	MOV	BX,0
	MOV	CX,0
	MOV	DX,0
	INT	14h
	CMP	AH,1		; AH will be 1 if already installed
	JNE	IST2		; if not, go install

	MOV	AH,0Ah		; get version of alledgedly installed SPDRIVE
	ADD	AH,CALL_OFFSET	; (this test required for "Quadtel" clone BIOS)
	MOV	AL,0
	MOV	BX,0
	MOV	CX,0
	MOV	DX,0
	INT	14h
	CMP	AH,1		; success code in AH should be 1
	JNE	IST2		; if not, go install
	CMP	DH,2		; version number should be 2.10
	JNE	IST2		; if not, go install
	CMP	DL,10
	JNE	IST2

	LEA	DX,DAI_MESS	; tell user we are already installed
	MOV	AH,9
	INT	21h
	INT	20h		; don't install, return to DOS

IST2:	LEA	DX,ISP_MESS	; tell user that we are installing
	MOV	AH,9
	INT	21h

	CMP	CALL_OFFSET,0	; check if alternate port
	JE	IST3		; if not, go on
	LEA	DX,ALT_MESS	; if so, tell user about it
	MOV	AH,9
	INT	21h

	; change interrupt mask, if not IRQ4

IST3:	CMP	IRQNUM,2	; check if IRQ2 interrupt
	JNE	IST4		; if not, continue
	MOV	INTMASK,IRQ2MSK	; set interrupt mask for IRQ2
	JMP	SSU3

IST4:	CMP	IRQNUM,3	; check if IRQ3 interrupt
	JNE	IST5		; if not, continue
	MOV	INTMASK,IRQ3MSK	; set interrupt mask for IRQ3
	JMP	SSU3

IST5:	CMP	IRQNUM,5	; check if IRQ5 interrupt
	JNE	SSU3		; if not, continue
	MOV	INTMASK,IRQ5MSK	; set interrupt mask for IRQ5

	; compute various I/O port addresses

SSU3:	MOV	AX,PORTADDR	; recall port base address
	ADD	AX,2
	MOV	PORT_IIR,AX	; save Interrupt Id. Reg. address
	INC	AX
	MOV	PORT_LCR,AX	; save Line Control Reg. address
	INC	AX
	MOV	PORT_MCR,AX	; save Modem Control Reg. address
	INC	AX
	MOV	PORT_LSR,AX	; save Line Status Reg. address
	INC	AX
	MOV	PORT_MSR,AX	; save Modem Status Reg. address

	IN	AL,21h		; read in 8259 IMR
	JMP	SHORT $+2	; I/O delay
	MOV	AH,INTMASK
	NOT	AH
	OR	AL,AH		; disable interrupt
	OUT	21h,AL		; write it out

	MOV	AX,351Ch	; get old timer interrupt vector 1Ch
	INT	21h
	MOV	WORD PTR TISAVE,BX	; save offset
	MOV	WORD PTR TISAVE+2,ES	; save segment
	MOV	AX,3514h	; get BIOS RS232_IO vector 14h
	INT	21h
	MOV	WORD PTR BIOS_SPD,BX	; save offset
	MOV	WORD PTR BIOS_SPD+2,ES	; save segment

	MOV	AH,25h		; DOS set interrupt vector function
	MOV	AL,IRQNUM	; get 8259 interrupt level
	ADD	AL,8		; add offset of 8259 interrupts
	MOV	DX,OFFSET SER_INT	; get address of handler
	INT	21h		; call DOS

	MOV	AX,251Ch	; set new timer interrupt vector 1Ch
	MOV	DX,OFFSET TIM_INT
	INT	21h
	MOV	AX,2514h	; set new interrupt vector 14h
	MOV	DX,OFFSET SP_DRIVE
	INT	21h

	LEA	DX,SPD_TOP	; point to top of driver
	INT	27h		; terminate but stay resident

;-------------------------------
;	Capitalize letter in AL
;-------------------------------
CAPITALIZE	PROC	NEAR
	CMP	AL,'a'
	JB	CAP9
	CMP	AL,'z'
	JA	CAP9
	AND	AL,5Fh
CAP9:	RET
CAPITALIZE	ENDP

START	ENDP

CSEG	ENDS
	END	START
