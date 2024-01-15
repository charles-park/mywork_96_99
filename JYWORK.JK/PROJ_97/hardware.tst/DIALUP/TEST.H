//[*]----------------------------------------------------------------------[*]
//[*]------ Start Header File ---------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
#include 	<dos.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<bios.h>
#include 	<conio.h>
#include 	<string.h>
#include 	<alloc.h>
#include 	<ctype.h>
//[*]----------------------------------------------------------------------[*]
#define		IRQ3		0x0b  /* Interrupt Request 3 (default : COM2,COM4)    */
#define  	IRQ4		0x0c  /* Interrupt Request 4 (default : COM1,COM3)    */

#define  	C1BASE		0x3f8 /* COM1 BASE ADDRESS                            */
#define  	C2BASE		0x2f8 /* COM2 BASE ADDRESS                            */
#define  	C3BASE		0x3e8 /* COM3 BASE ADDRESS                            */
#define  	C4BASE		0x2e8 /* COM4 BASE ADDRESS                            */

#define		THR			Com.Base
#define		RBR			Com.Base

#define		DLL			Com.Base	/* DLAB = 1 */
#define		DLM			Com.Base+1		/* DLAB = 1 */

#define		IER			Com.Base+1
#define		IIR			Com.Base+2
#define		LCR			Com.Base+3
#define		MCR			Com.Base+4
#define		LSR			Com.Base+5
#define  	MSR			Com.Base+6

#define  	PICMSK		0x21  /* PIC mask register                            */
#define  	PICEOI		0x20  /* PIC EOI register                             */

#define  	I3ENA		0xf7  /* Interrupt Request 3 enable                   */
#define  	I3DIS		0x08  /* Interrupt Request 3 disable                  */
#define  	I4ENA		0xef  /* Interrupt Request 4 enable                   */
#define  	I4DIS		0x10  /* Interrupt Request 4 disable                  */
//[*]----------------------------------------------------------------------[*]
#define		START		0
#define  	END   		1

#define		MAX			1024

#define 	B300L   	0x80
#define 	B300H   	0x01
#define 	B600    	0xC0
#define 	B1200   	0x60
#define 	B2400   	0x30
#define 	B4800   	0x18
#define 	B9600   	0x0c
#define 	B19200  	0x06
#define 	B38400  	0x03

#define		SIZE		2000
#define		COM1		1
#define		COM2		2
#define		COM3		3
#define		COM4		4
//[*]----------------------------------------------------------------------[*]
typedef		unsigned char 		Byte;
typedef		unsigned int		Word;
//[*]----------------------------------------------------------------------[*]
typedef	struct		Serial__t	{
	Byte	RxBuf[SIZE]	;
	Byte	TxBuf[SIZE]	;
	Word	RxSp       	;
	Word	RxEp       	;
	Word	TxSp       	;
	Word	TxEp       	;
	Word	Port	   	;
	Word	Base	   	;
}	Serial_t;
//[*]----------------------------------------------------------------------[*]
static	Serial_t	Com ;
//[*]----------------------------------------------------------------------[*]
static 	unsigned int	call_count = 0;
static struct time	 host_time;
static struct date   host_date;
static unsigned int		LineCount;
//[*]----------------------------------------------------------------------[*]
//[*]----- Define Function ------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
void interrupt 	(*old_vect)();
void interrupt 	SERIAL(void);
void 			print_to_file(unsigned char mode);
signed int 		Point(signed int Hear, signed int Step);    // Q Buffer Point Set
Byte  			CominData(void);
int 			CominREADY(void);
Byte 			CominCH(void);
void 			ComoutCH(Byte ch);
void 			ComoutStr(void);
void 			ok_send(void);
void 			init_Serial(Word MODE);
void 			main(int argc,char **argv);
//[*]----------------------------------------------------------------------[*]
