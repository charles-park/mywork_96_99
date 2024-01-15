#include <io8096.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct 	bits__t	{
	unsigned		b0	:1;
	unsigned		b1	:1;
	unsigned		b2	:1;
	unsigned		b3	:1;
	unsigned		b4	:1;
	unsigned		b5	:1;
	unsigned		b6	:1;
	unsigned		b7	:1;
}	bits_t;

typedef union	bits__u	{
	byte	dt;
	bits_t	bits;
}	bits_u;

static	bits_u	port0;
static	bits_u  port1;
static	bits_u  port2;

#define		watch()		{	port2.bits.b7 = 1 - port2.bits.b7;	P2 = port2.dt;	}

void delay			 (word count);
void main 			 (void);


void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)
	{  	outputb(0x8000, 0xFF);	i++;	i--;	watch();	}
}


void main(void)
{
	disable();
	port0.dt = 0x00;
	port1.dt = 0x00;
	port2.dt = 0x00;
	P0 = port0.dt;
	P1 = port1.dt;
	P2 = port2.dt;
	while(1)	{
		port0.dt++;		port1.dt++;		delay(500);
		P0 = port0.dt;
		P1 = port1.dt;
	}
}

void interrupt_TIMER (void)	{	return;	}
void interrupt_AD    (void)	{	return;	}
void interrupt_HSI   (void)	{	return;	}
void interrupt_HSO   (void)	{	return;	}
void interrupt_HSIO  (void)	{	return;	}
void interrupt_STIMER(void)	{	return;	}
void interrupt_SCON  (void)	{	return;	}
void interrupt_EINT  (void)	{	return;	}
