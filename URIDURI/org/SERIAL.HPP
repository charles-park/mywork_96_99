#ifndef 	__SERIAL_HPP__
#define		__SERIAL_HPP__
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include 	"HARDWARE.HPP"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#ifndef	COM1_IRQ
	#define COM1_IRQ        		IRQ_04
	#define COM2_IRQ        		IRQ_03
	#define COM3_IRQ                IRQ_10
	#define COM4_IRQ                IRQ_11
#endif
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#ifndef	COM1_BASE
	#define COM1_BASE 				0x03F8
	#define COM2_BASE 				0x02F8
	#define COM3_BASE 				0x03E8
	#define COM4_BASE 				0x02E8
#endif
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#ifndef	COM1
	#define COM1 					0
	#define COM2 					1
	#define COM3 					2
	#define COM4 					3
	#define COM5 					4
	#define COM6 					5
	#define COM7 					6
	#define COM8 					7
#endif
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define THR(COM_BASEADDRESS)		(COM_BASEADDRESS + 0)
#define RBR(COM_BASEADDRESS)		(COM_BASEADDRESS + 0)
#define IER(COM_BASEADDRESS)		(COM_BASEADDRESS + 1)
#define IIR(COM_BASEADDRESS)		(COM_BASEADDRESS + 2)
#define LCR(COM_BASEADDRESS)		(COM_BASEADDRESS + 3)
#define MCR(COM_BASEADDRESS)		(COM_BASEADDRESS + 4)
#define LSR(COM_BASEADDRESS)		(COM_BASEADDRESS + 5)
#define MSR(COM_BASEADDRESS)		(COM_BASEADDRESS + 6)
#define DLL(COM_BASEADDRESS)		(COM_BASEADDRESS + 0)
#define DLM(COM_BASEADDRESS)		(COM_BASEADDRESS + 1)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define LCR_DATABIT 	0x03
#define LCR_STOPBIT 	0x04
#define LCR_PARITY  	0x18
#define LCR_SP      	0x20
#define LCR_SB      	0x40
#define LCR_DLAB    	0x80
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define DATABIT7    	0x02
#define DATABIT8    	0x03
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define STOPBIT1    	0x00
#define STOPBIT2    	0x04
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define NOPARITY    	0x00
#define ODDPARITY   	0x08
#define EVENPARITY  	0x18
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define BAUD300        	384
#define BAUD400        	288
#define BAUD600        	192
#define BAUD1200       	96
#define BAUD2400       	48
#define BAUD4800       	24
#define BAUD9600       	12
#define BAUD14400      	8
#define BAUD19200      	6
#define BAUD28800      	4
#define BAUD38400      	3
#define BAUD57600      	2
#define BAUD115200     	1
#define BAUD(x)         (unsigned int)(1843200L / 16 / x)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define BUFFEREMPTY 	(-1)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct	SERIAL	{
	unsigned char	IRQ_NO;
	unsigned int 	BASE_ADDRESS;

	char 		  *	buffer;
	unsigned int   	buffersize;
	unsigned int   	sp;
	unsigned int   	ep;
	unsigned int   	baud;
	unsigned char  	stop;
	unsigned char  	data;
	unsigned char  	parity;
	unsigned char  	is_open;
	void 		   	(*hook_getc)(char data);
	void interrupt 	(far * near OldHandler)(...);
	void interrupt 	(far * near NewHandler)(...);
};
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
extern  SERIAL      COM[4];
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void  interrupt serial1_interrupt(...);
void  interrupt serial2_interrupt(...);
void  interrupt serial3_interrupt(...);
void  interrupt serial4_interrupt(...);
void  serial_init (void);
void  serial_close(void);
int   com_open 	(int port,unsigned int  baudrate = 0xFFFF,
						  unsigned char data 	 = 0xFF,
						  unsigned char stop     = 0xFF,
						  unsigned char parity   = 0xFF,int buffersize =-1);
int   com_getc  (int port);
void  com_clear (int port);
void  com_close (int port);
void  com_puts  (int port,char *s);
void  com_puts  (int port,char *s,int len);
void  com_putc  (int port,char  c);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	com_clear(port)						COM[port].sp = COM[port].ep = 0
#define	com_isopen(port)					COM[port].is_open
#define	get_com_getc_hook(port)           	COM[port].hook_getc
#define	set_com_getc_hook(port, hook_func)	COM[port].hook_getc = hook_func
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#endif 	__SERIAL_HPP__
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
