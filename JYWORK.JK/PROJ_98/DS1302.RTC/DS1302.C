//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include "io51.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stddef.h"
#include "stdarg.h"
#include "icclbutl.h"
#include "DS1302.H"
#include "SERIAL.HPP"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#pragma memory=code
byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil   Date : 1998. 4. 06~ 4. 07[*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 1.xx                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
byte chk1(variable_t *p){	return 1;}
byte cat1(variable_t *p){	return 1;}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)		watch();
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void clock(void)
{
	SCLOCK = 0;		SCLOCK = 1;		SCLOCK = 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
byte read_ic(byte addr)
{
	byte	ck = 0x01, read_data = 0, i;

	RESET(ON);

	for(i=0; i<8; i++)	{
		if(addr & ck)		IN_OUT = 1;
		else				IN_OUT = 0;
		clock();			ck = ck <<1;
	}

	ck = 0x01;
	for(i=0; i<8; i++)	{
		if(IN_OUT)			read_data |=   ck ;
		else				read_data &= (~ck);
		clock();			ck = ck <<1;
	}

	RESET(OFF);				return (byte)(read_data);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void write_ic(byte addr, byte wr_data)
{
	byte	ck = 0x01, i;

	RESET(ON);
	for(i=0; i<8; i++)	{
		if(addr & ck)		IN_OUT = 1;
		else				IN_OUT = 0;
		clock();			ck = ck <<1;
	}

	ck = 0x01;
	for(i=0; i<8; i++)	{
		if(wr_data & ck)	IN_OUT = 1;
		else            	IN_OUT = 0;
		clock();			ck = ck <<1;
	}
	RESET(OFF);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void rtc_run(void)
{
	while(1)	{
		watch();							d.byte_data = 0x00;
		write_ic(SEC_WR, d.byte_data);		d.byte_data = read_ic(SEC_RD);
		if(!d.bits.flag)					return;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void read_time(time_t *t)
{
	time_t	tt;

	watch();
	d.byte_data = read_ic(SEC_RD);
	if(d.bits.flag)		{	rtc_run();	return;	}	// clock is stopped
	else				tt.ss = (d.bits.msb * 10 + d.bits.lsb);

	d.byte_data = read_ic(MIN_RD);
	tt.bb = (d.bits.msb * 10 + d.bits.lsb);

	d.byte_data = read_ic(HOUR_RD);
	tt.hh = (d.bits.msb * 10 + d.bits.lsb);

	d.byte_data = read_ic(YOIL_RD);
	tt.yoil = d.bits.lsb;

	d.byte_data = read_ic(DAY_RD);
	tt.dd = (d.bits.msb * 10 + d.bits.lsb);

	d.byte_data = read_ic(MONTH_RD);
	tt.mm = (d.bits.msb * 10 + d.bits.lsb);

	d.byte_data = read_ic(YEAR_RD);
	if(d.bits.flag)	tt.yy = (d.bits.msb + 8) * 10 + d.bits.lsb;
	else		   	tt.yy = (d.bits.msb * 10 + d.bits.lsb);

	if(tt.yy > 99 || tt.mm > 12 || tt.mm < 1 || tt.dd > 31 || tt.dd < 1 ||
	   tt.hh > 23 || tt.bb > 59 || tt.ss > 59)
	{
		tt.yy = 98;	tt.mm = 9;	tt.dd = 30;
		tt.hh = 19;	tt.bb = 10;	tt.ss = 0; 	tt.yoil = 3;
		write_time(&time);		// rtcc broken
	}	else	{
		t->yy   = tt.yy  ;	t->mm   = tt.mm  ;	t->dd   = tt.dd  ;
		t->hh   = tt.hh  ;	t->bb   = tt.bb  ;	t->ss   = tt.ss  ;
		t->yoil = tt.yoil;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void write_time(time_t *t)
{
	watch();
	d.byte_data = 0x00;
	d.bits.msb = t->ss / 10;	d.bits.lsb = t->ss % 10;
	d.bits.flag = 0;			write_ic(SEC_WR, d.byte_data);
//  CLOCK is STOPPED
//	d.bits.flag = 1;			write_ic(SEC_WR, d.byte_data);

	watch();
	d.byte_data = 0x00;
	d.bits.msb = t->bb / 10;	d.bits.lsb = t->bb % 10;
	d.bits.flag = 0;			write_ic(MIN_WR, d.byte_data);

	watch();
	d.byte_data = 0x00;
	d.bits.msb = t->hh / 10;	d.bits.lsb = t->hh % 10;
	d.bits.flag = 0;			write_ic(HOUR_WR, d.byte_data);

	watch();
	d.byte_data = 0x00;
	d.bits.msb = t->dd / 10;	d.bits.lsb = t->dd % 10;
	d.bits.flag = 0;			write_ic(DAY_WR, d.byte_data);

	watch();
	d.byte_data = 0x00;
	d.bits.lsb  = t->yoil;
	d.bits.flag = 0;			write_ic(YOIL_WR, d.byte_data);

	watch();
	d.byte_data = 0x00;
	d.bits.msb = t->mm / 10;	d.bits.lsb = t->mm % 10;
	d.bits.flag = 0;			write_ic(MONTH_WR, d.byte_data);

	watch();
	d.byte_data = 0x00;
	d.bits.msb = t->yy / 10;	d.bits.lsb = t->yy % 10;
	if(t->yy > 79)				d.bits.flag = 1;
	else						d.bits.flag = 0;
	write_ic(YEAR_WR, d.byte_data);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void serial_init(void)
{
	// RTCC DATA READ
	init_serial_grp  (&com, INTERNAL, 0x0, BPS9600, M22, SMOD_OFF, NOT_USED, 1);
	init_serial_pro  (&com, 0, 1, chk1, cat1);
	set_protocol_flag(&com, 0, ON);
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void transmit_check(void)
{
	watch();
	if(com.tx_sp != com.tx_ep)	{
		if(com.tx_flag)	{
			disable(EA);
			SBUF = com.tx_buf[com.tx_sp++];
			enable(EA);
			if(com.tx_sp >= BUFFER_SIZE)	com.tx_sp = 0;
			com.tx_flag = OFF;
		}
	}	else	com.cat_flag = ON;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void receive_check(void)
{
	watch();
	if(com.rx_sp != com.rx_ep)	{
		RX_LED(OFF);
		if(com.cat_flag == OFF)				return;
		protocol_event(&com, com.rx_buf[com.rx_sp++]);
		if(com.rx_sp >= BUFFER_SIZE)	com.rx_sp = 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_time(void)
{
	word 	i;
	byte	buff[70];

	for(i=0; i<70; i++)		{	watch();	buff[i] = 0x00;	}

	watch();
	sprintf(buff, "DATE : %d/%d/%d  TIME : %d/%d/%d  YOIL : %d \n\r",
					time.yy, time.mm, time.dd, time.hh, time.bb, time.ss,
					time.yoil                                           );

	i = 0;
	while(buff[i] != 0x00)	{
		watch();
		com.tx_buf[com.tx_ep++] = buff[i++];
		if(com.tx_ep >= BUFFER_SIZE)	com.tx_ep = 0;
	}	com.cat_flag = OFF;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);

	if(RI)	{
		disable(RI);	RX_LED(ON);
		com.rx_buf[com.rx_ep++] = SBUF;
		if(com.rx_ep >= BUFFER_SIZE)		com.rx_ep = 0;
	} else if(TI)	{
		disable(TI);
		if(com.tx_sp != com.tx_ep)	{
			TX_LED(ON);
			SBUF = com.tx_buf[com.tx_sp++];
			if(com.tx_sp >= BUFFER_SIZE)	com.tx_sp = 0;
		} else	{
			com.tx_flag = ON;	TX_LED(OFF);
		}
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	time_t	tt;
	SP(ON);			delay(5000);	SP(OFF);
	IN_OUT = OFF;	SCLOCK = OFF;	RESET(OFF);

	RX_LED(OFF);	TX_LED(OFF);	serial_init();

//	read_time(&time);

	tt.yy = 98;	tt.mm = 10;	tt.dd = 1;
	tt.hh = 18;	tt.bb = 0 ;	tt.ss = 20; 	tt.yoil = 4;
	write_time(&tt);		// rtcc broken

	while(1)	{
		SP(OFF);
		watch();			read_time(&time);	send_time();
		transmit_check();	receive_check();
		delay(5000);	delay(5000);	delay(5000);	delay(5000);
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]






