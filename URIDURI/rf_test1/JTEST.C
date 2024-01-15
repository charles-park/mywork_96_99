//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include <dos.h>
#include <stdio.h>
#include <conio.h>
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include "debug.hpp"
#include "serial.hpp"
#include "protocol.hpp"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include "mystruct.hpp"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static protocol_grp *com;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static rtu_recv_t	rtu_recv;
static send_pc_t 	send_pc ;
static cmd_t		cmd     ;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//   Function prototype
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Print(char dt);
unsigned int exchange(unsigned int ex);
float exchange		(float ex);
int chk_func		(protocol *p);
int cat_func		(protocol *p);
int cat_func1		(protocol *p);
int cat_func2		(protocol *p);
void ack_send		(void);
void send_cmd		(unsigned char scmd, unsigned int variable = 0x0000);
void main			(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Print(char dt)
{
	printf("%c", dt & 0x00FF);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int chk_func(protocol *p)
{
	if(!p->check_sp(9, '9'))	return 0;
	if(!p->check_sp(8, '8'))	return 0;
	if(!p->check_sp(7, '7'))	return 0;
	if(!p->check_sp(6, '6'))	return 0;
	if(!p->check_sp(5, '5'))	return 0;
	if(!p->check_sp(4, '4'))	return 0;
	if(!p->check_sp(3, '3'))	return 0;
	if(!p->check_sp(2, '2'))	return 0;
	if(!p->check_sp(1, '1'))	return 0;
	if(!p->check_sp(0, '0'))	return 0;

								return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int chk_func1(protocol *p)
{
	if(!p->check_sp(3, ':'))	return 0;
	if(!p->check_sp(2, 'd'))	return 0;
	if(!p->check_sp(1, 'm'))	return 0;
	if(!p->check_sp(0, 'c'))	return 0;

								return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int cat_func(protocol *p)
{
	ack_send();					return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	unsigned char Send_flag = 0x00;
static 	unsigned char Stream    = 0x00;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int cat_func1(protocol *p)
{
	if(Send_flag)	{
		Send_flag = 0x00;		com_putc(COM2, 't' );
		com_putc(COM2, 0x0D);	com_putc(COM2, 0x0A);
		ack_send();
	}	else	{
		Send_flag = 0x01;
		if(Stream)	{
			Stream = 0x00;
			com_putc(COM2, 0x02  );		// ctrl - B
			com_putc(COM2, 'A'   );
			com_putc(COM2, '0x0D');
			com_putc(COM2, '0x0A');
		}	else	{
			Stream = 0x01;
			com_putc(COM2, 0x02  );		// ctrl - B
			com_putc(COM2, 'B'   );
			com_putc(COM2, '0x0D');
			com_putc(COM2, '0x0A');
		}
	}
	return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void ack_send(void)
{
	for(int i=0; i<10; i++)		com_putc(COM2, i + 0x30);
	com_putc(COM2, 0x0D);		com_putc(COM2, 0x0A);
	com_putc(COM2, 0x01);		com_putc(COM2, 0x01);
	com_putc(COM2, 0x01);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	int		key = 0x0000;
	serial_init();
	com_open(COM2, BAUD2400, DATABIT8, STOPBIT1, NOPARITY, 2048);
	set_com_getc_hook(COM2, Print);

	com = new protocol_grp   (COM2, 2, com_getc);
	com->grp[0].init_protocol(10, chk_func , cat_func );
	com->grp[1].init_protocol(4 , chk_func1, cat_func1);

	com->grp[0].set_flag(1);	com->grp[1].set_flag(1);

	com_putc(COM2, 0x0D);		com_putc(COM2, 0x0A);
	while(1)	{
		if(kbhit())	{
			key = getch();
			if((key & 0x00FF) == 0x1b)		break;
		}	com->protocol_event();
	}
	delete com;
	com_close(COM2);	serial_close();
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]





