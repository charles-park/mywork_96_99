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
//void reset_send		(void);
//void watch_call		(void);
//void status_call	(void);
//void volts_call		(void);
//void buff_clear		(void);
void main			(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Print(char dt)
{
	printf("%02X  ", dt & 0x00FF);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
unsigned int exchange(unsigned int ex)
{
	unsigned char d0, d1;
	unsigned char *p = (unsigned char *)(&ex);
	d0   = p[0];	d1   = p[1];
	p[0] = d1  ;	p[1] = d0  ;

	return	(unsigned int)ex;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
float exchange(float ex)
{
	unsigned char d0, d1, d2, d3;
	unsigned char *p = (unsigned char *)(&ex);
	d0   = p[0];	d1   = p[1];	d2   = p[2];	d3   = p[3];
	p[0] = d3  ;	p[1] = d2  ;	p[2] = d1  ;	p[3] = d0  ;

	return	(float)ex;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int chk_func(protocol *p)
{
	if(!p->check_ep(1, 0xFE))	return 0;
	if(!p->check_ep(2, 0xFE))	return 0;
	if(!p->check_ep(3, 0xFE))	return 0;
	if(!p->check_ep(4, 0xFE))	return 0;

	if(!p->check_sp(3, 0xFF))	return 0;
	if(!p->check_sp(2, 0xFF))	return 0;
	if(!p->check_sp(1, 0xFF))	return 0;
	if(!p->check_sp(0, 0xFF))	return 0;

								return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int cat_func(protocol *p)
{
	int count, p_size;
	unsigned char xor = 0, add = 0;
	char *buf = p->get_qbuffer(0);

	unsigned char *recv = (unsigned char *)(&rtu_recv);

	for(count = 4; count < sizeof(rtu_recv_t)-6; count++)	{
		recv[count] = buf[count];
		xor        ^= buf[count];	add        += buf[count];
	}

	if(!p->check_ep(6, xor))					return 0;
	if(!p->check_ep(5, add))    				return 0;

	printf("SID  ->%02X ", rtu_recv.sen_id);
	printf("SCODE->%02X ", rtu_recv.sen_cd);
	printf("SSTA ->%02X ", rtu_recv.sen_st);

	printf("OK!\n");	ack_send();				return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int cat_func1(protocol *p)
{
	printf("\r\nSTATUS DATA DETECT!\n");		return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int cat_func2(protocol *p)
{
	printf("\r\nVOLTS DATA DETECT!\n");			return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void ack_send(void)
{
	unsigned char *send = (unsigned char *)(&send_pc );

	send_pc.header = 0x02  ;	send_pc.cmd    = 'A' ;
	send_pc.tail   = 0x040D;

	send_pc.tail   = exchange(send_pc.tail);

	for(int i=0; i<sizeof(send_pc); i++)	com_putc(COM2, send[i]);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_cmd(unsigned char scmd, unsigned int variable)
{
	unsigned char *send = (unsigned char *)(&cmd);

	cmd.header = 0xFFFFFFFF;	cmd.tail   = 0xFEFEFEFE;
	cmd.xor    = 0x00      ;	cmd.add    = 0x00      ;
	cmd.cmd    = scmd      ;    cmd.value  = exchange(variable);

	for(int i=0; i<sizeof(cmd); i++)	{
		if(i>3 && i<sizeof(cmd)-6)	{
			cmd.xor ^= send[i];		cmd.add += send[i];
		}	com_putc(COM2, send[i]);
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
/*
void reset_send(void)
{
	unsigned char *send = (unsigned char *)(&cmd);

	cmd.header = 0xFFFFFFFF;	cmd.tail   = 0xFEFEFEFE;
	cmd.cmd    = 'R'       ;	cmd.xor    = 0x00;
	cmd.add    = 0x00      ;

	for(int i=0;i<sizeof(cmd);i++)	{
		if(i > 3 && i < sizeof(cmd)-6)	{
			cmd.xor ^= send[i];	cmd.add += send[i];
		}	com_putc(COM2, send[i]);
	}
	printf("\r\nRTU_RESET\n");
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void watch_call(void)
{
	unsigned char *send = (unsigned char *)(&cmd);

	cmd.header = 0xFFFFFFFF;	cmd.tail   = 0xFEFEFEFE;
	cmd.cmd    = 'E'       ;	cmd.xor    = 0x00;
	cmd.add    = 0x00      ;

	for(int i=0;i<sizeof(cmd);i++)	{
		if(i > 3 && i < sizeof(cmd)-6)	{
			cmd.xor ^= send[i];	cmd.add += send[i];
		}	com_putc(COM2, send[i]);
	}
	printf("\r\nWATCH_CALL\n");
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void status_call(void)
{
	unsigned char *send = (unsigned char *)(&cmd);

	cmd.header = 0xFFFFFFFF;	cmd.tail   = 0xFEFEFEFE;
	cmd.cmd    = 's'       ;	cmd.xor    = 0x00;
	cmd.add    = 0x00      ;

	for(int i=0;i<sizeof(cmd);i++)	{
		if(i > 3 && i < sizeof(cmd)-6)	{
			cmd.xor ^= send[i];	cmd.add += send[i];
		}	com_putc(COM2, send[i]);
	}
	printf("\r\nSTATUS_CALL\n");
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void volts_call(void)
{
	unsigned char *send = (unsigned char *)(&cmd);

	cmd.header = 0xFFFFFFFF;	cmd.tail   = 0xFEFEFEFE;
	cmd.cmd    = 's'       ;	cmd.xor    = 0x00;
	cmd.add    = 0x00      ;

	for(int i=0;i<sizeof(cmd);i++)	{
		if(i > 3 && i < sizeof(cmd)-6)	{
			cmd.xor ^= send[i];	cmd.add += send[i];
		}	com_putc(COM2, send[i]);
	}
	printf("\r\nVOLTS_CALL\n");
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void buff_clear(void)
{
	unsigned char *send = (unsigned char *)(&cmd);

	cmd.header = 0xFFFFFFFF;	cmd.tail   = 0xFEFEFEFE;
	cmd.cmd    = 'C'       ;	cmd.xor    = 0x00;
	cmd.add    = 0x00      ;

	for(int i=0;i<sizeof(cmd);i++)	{
		if(i > 3 && i < sizeof(cmd)-6)	{
			cmd.xor ^= send[i];	cmd.add += send[i];
		}	com_putc(COM2, send[i]);
	}
	printf("\r\nBUFFER_CLEAR\n");
}
*/
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	int		key = 0x0000;
	serial_init();
	com_open(COM2, BAUD2400, DATABIT8, STOPBIT1, NOPARITY, 2048);
	set_com_getc_hook(COM2, Print);

	com = new protocol_grp   (COM2, 3, com_getc);
	com->grp[0].init_protocol(sizeof(rtu_recv_t)   , chk_func, cat_func );
	com->grp[1].init_protocol(sizeof(status_call_t), chk_func, cat_func1);
	com->grp[2].init_protocol(sizeof(volts_call_t) , chk_func, cat_func2);

	com->grp[0].set_flag(1);
	com->grp[1].set_flag(1);
	com->grp[2].set_flag(1);

	printf("Protocol1 Catch Size : %d\n",sizeof(rtu_recv_t));
	printf("Protocol2 Catch Size : %d\n",sizeof(status_call_t));
	printf("Protocol3 Catch Size : %d\n",sizeof(volts_call_t));

	while(1)	{
		if(kbhit())	{
			key = getch();
			if((key & 0x00FF) == 0x1b)		break;
			else	{
				switch(key & 0x00FF)	{
					case	'R':
						send_cmd('R');	printf("\nRESET COMMAND SEND.\n\r");
						break;
					case	'E':
						send_cmd('E');	printf("\nEVENT CALL COMMAND SEND.\n\r");
						break;
					case	'C':
						send_cmd('C');	printf("\nBUFFER CLEAR COMMAND SEND.\n\r");
						break;
					case	'D':
						send_cmd('D');	printf("\nRTU DATE&TIME SET.\n\r");
						break;
					case	's':
						send_cmd('s');	printf("\nRTU STATUS CALL.\n\r");
						break;
					case	'I':
						send_cmd('I', 1234);	printf("\nRTU ID SET.\n\r");
						break;
					case	'P':
						send_cmd('P', 4321);	printf("\nRTU PASS SET.\n\r");
						break;
					default	   :									break;
				}
			}
		}
		com->protocol_event();
	}
	delete com;
	com_close(COM2);	serial_close();
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]





