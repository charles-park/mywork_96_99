//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include <dos.h>
#include <stdlib.h>
#include "SERIAL.HPP"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
SERIAL 		COM[4];
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#pragma warn -eas
void interrupt serial1_interrupt(...)
{
	COM[0].ep %= COM[0].buffersize;
	COM[0].buffer[COM[0].ep++] = inportb(COM[0].BASE_ADDRESS);
	End_of_Interrupt(COM[0].IRQ_NO);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void interrupt serial2_interrupt(...)
{
	COM[1].ep %= COM[1].buffersize;
	COM[1].buffer[COM[1].ep++] = inportb(COM[1].BASE_ADDRESS);
	End_of_Interrupt(COM[1].IRQ_NO);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void interrupt serial3_interrupt(...)
{
	COM[2].ep %= COM[2].buffersize;
	COM[2].buffer[COM[2].ep++] = inportb(COM[2].BASE_ADDRESS);
	End_of_Interrupt(COM[2].IRQ_NO);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void interrupt serial4_interrupt(...)
{
	COM[3].ep %= COM[3].buffersize;
	COM[3].buffer[COM[3].ep++] = inportb(COM[3].BASE_ADDRESS);
	End_of_Interrupt(COM[3].IRQ_NO);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#pragma warn .eas
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void serial_init(void)
{
	for(int port=COM1; port<=COM4; port ++)	{
		COM[port].buffer     = NULL;
		COM[port].buffersize = 0;
		COM[port].sp         = 0;
		COM[port].ep         = 0;
		COM[port].baud       = BAUD2400;
		COM[port].stop       = STOPBIT1;
		COM[port].data       = DATABIT8;
		COM[port].parity     = NOPARITY;
		COM[port].is_open    = 0;
		COM[port].OldHandler = 0;
		COM[port].hook_getc  = NULL;
	}

	COM[0].NewHandler = serial1_interrupt;
	COM[1].NewHandler = serial2_interrupt;
	COM[2].NewHandler = serial3_interrupt;
	COM[3].NewHandler = serial4_interrupt;

	COM[0].IRQ_NO = COM1_IRQ;	COM[0].BASE_ADDRESS = COM1_BASE;
	COM[1].IRQ_NO = COM2_IRQ;	COM[1].BASE_ADDRESS = COM2_BASE;
	COM[2].IRQ_NO = COM3_IRQ;	COM[2].BASE_ADDRESS = COM3_BASE;
	COM[3].IRQ_NO = COM4_IRQ;	COM[3].BASE_ADDRESS = COM4_BASE;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void serial_close(void)
{
	for(int port=COM1; port<=COM4; port ++)	{
		if(COM[port].is_open) com_close(port);
		if(COM[port].buffersize != 0)	{
			delete  [] COM[port].buffer;
			COM[port].buffersize = 0;
			COM[port].sp		 = 0;
			COM[port].ep         = 0;
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void com_close(int port)
{
	if(COM[port].is_open == 0) return;		delay(100);
	while(!(inportb(LSR(COM[port].BASE_ADDRESS)) & 0x20));
	outportb(MCR(COM[port].BASE_ADDRESS), 0x00);
	outportb(IER(COM[port].BASE_ADDRESS), 0x00);

	intrrupt_close(COM[port].IRQ_NO, COM[port].OldHandler);

	COM[port].sp = COM[port].ep = 0; COM[port].is_open = 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int com_open(int port,unsigned int baudrate,unsigned char data,unsigned char stop,unsigned char parity,int buffersize)
{
	if(baudrate	 != 0xFFFF) COM[port].baud	  = baudrate;
	if(stop    	 != 0xFF  )	COM[port].stop	  = stop;
	if(data    	 != 0xFF  )	COM[port].data	  = data;
	if(parity    != 0xFF  )	COM[port].parity  = parity;
	if(buffersize== -1	  ) buffersize = COM[port].buffersize;
	if(COM[port].is_open)	com_close(port);
	if(COM[port].buffersize != buffersize)	{
		if(COM[port].buffersize != 0)	{
			delete [] COM[port].buffer;
			COM[port].buffersize = 0;
		}
		COM[port].buffer = new char[buffersize];
		COM[port].buffersize = buffersize;
	}
	int settings = COM[port].stop|COM[port].data|COM[port].parity;
	outportb(LCR(COM[port].BASE_ADDRESS), settings & (LCR_DATABIT|LCR_STOPBIT|LCR_PARITY));
	outportb(LCR(COM[port].BASE_ADDRESS), inportb(LCR(COM[port].BASE_ADDRESS))|  LCR_DLAB);
	outportb(DLL(COM[port].BASE_ADDRESS), (unsigned char)(COM[port].baud)				 );
	outportb(DLM(COM[port].BASE_ADDRESS), COM[port].baud >> 8		   	 			     );
	outportb(LCR(COM[port].BASE_ADDRESS), inportb(LCR(COM[port].BASE_ADDRESS))& ~LCR_DLAB);
	outportb(MCR(COM[port].BASE_ADDRESS), 0x0b					   		 				 );
	outportb(IER(COM[port].BASE_ADDRESS), 0x01					   		 				 );

	COM[port].OldHandler = intrrupt_open(COM[port].IRQ_NO,COM[port].NewHandler);

	COM[port].sp = COM[port].ep = 0; COM[port].is_open = 1;
	return COM[port].is_open;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int com_getc(int port)
{
	if(COM[port].sp == COM[port].ep) return BUFFEREMPTY;
	COM[port].sp %= COM[port].buffersize;
	if(COM[port].hook_getc)	COM[port].hook_getc((COM[port].buffer[COM[port].sp] & 0x00FF));
	return (COM[port].buffer[COM[port].sp++] & 0x00FF);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void com_putc(int port, char c)
{
	while (!(inportb(LSR(COM[port].BASE_ADDRESS)) & 0x20));
	outportb(THR(COM[port].BASE_ADDRESS), c);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void com_puts(int port,char *s)
{
	while (*s) com_putc(port,*s++);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void com_puts(int port,char *s,int len)
{
	for(int i=0;i<len;i++)	com_putc(port,s[i]);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
