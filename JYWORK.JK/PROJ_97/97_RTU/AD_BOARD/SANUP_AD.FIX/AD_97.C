#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ad_97.h"
#include "pros.hpp"
#include "send.hpp"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil   Date : 1997. 10. 19      [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : SAN UP AWS                          [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD : AD BOARD                                   [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  MAIN : TEMP(4.0), DEW(4.0), HUMI(4.0) --> 3 EA     [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  SUB1 : TEMP(0.5), DEW(0.5), HUMI(0.5)              [*]",
   "[*]         TEMP(1.5), DEW(1.5), HUMI(1.5) --> 6 EA     [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  SUB2 : TEMP(0.30), TEMP(0.10), TEMP(0.0)           [*]",
   "[*]         JUN BOKSA , BAN BOKSA          --> 5 EA     [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 1.xx                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]----------------------------------------------------------------------[*]
byte read_board_id(void)
{
	byte	value1, value2, count = 0;

	watch();
	value1 = input(JUMPER);	// read jumper

	while(1)	{
		value2 = input(JUMPER);
		if(value1 == value2)	count++;
		else	{	count = 0;	value1 = input(JUMPER);}
		if(count > 10)			break;
	}
	value1 = (~value1) & 0x0f;	return (value1);
}
//[*]----------------------------------------------------------------------[*]
void com_port_init(void)
{
	watch();
	init_serial_grp(&com1, INTERNAL, 0x0, BPS9600, M22, SMOD_OFF, NOT_USED, 2);
	init_serial_pro(&com1, 0, 15, rx_chk, rx_cat1);
	init_serial_pro(&com1, 1, 32, rx_chk, rx_cat2);
	set_protocol_flag(&com1, 0, ON);	set_protocol_flag(&com1, 1, OFF);
}
//[*]----------------------------------------------------------------------[*]
void interrupt_init(void)
{
	watch();
	TMOD |= 0x1;	enable(IT0);	enable(EX0);	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	byte	i;
	watch(); 		Board_id = read_board_id();

	status.byte_data = 0x0;
	// Channel flag Initialize.
	for(i=0; i<7; i++)	Ch_flag[i] = OFF;

	Real_ch = 0;		Real_flag = OFF;	mux_select(0);
	Jugi_count = 0;		Jugi_value = 600;	Sujip_ch = 0;	Jugi_flag = OFF;
	Hz_check_count = 0 ;

	switch(Board_id)	{
		case	0:
		//TEMP(4.0), DEW(4.0), HUMI(4.0) --> 3 EA
			Ch_flag[0] = Ch_flag[1] = Ch_flag[2] = ON;
			init_channel(&channel[0], temp_pros, 0x0, 0x0);
			init_struct (&send_ch[0], 0x02, 0x0C);
			init_channel(&channel[1], dewp_pros, 0x0, 0x0);
			init_struct (&send_ch[1], 0x07, 0x0D);
			init_channel(&channel[2], humi_pros, 0x0, 0x1);
			init_struct (&send_ch[2], 0x06, 0x0E);
			break;
		case	1:
		//TEMP(0.5), DEW(0.5), HUMI(0.5),TEMP(1.5), DEW(1.5), HUMI(1.5) --> 6 EA
			Ch_flag[0] = Ch_flag[1] = Ch_flag[2] = ON;
			Ch_flag[3] = Ch_flag[4] = Ch_flag[5] = ON;
			init_channel(&channel[0], temp_pros, 0x0, 0x0);
			init_struct (&send_ch[0], 0x02, 0x03);
			init_channel(&channel[1], dewp_pros, 0x0, 0x0);
			init_struct (&send_ch[1], 0x07, 0x04);
			init_channel(&channel[2], temp_pros, 0x0, 0x0);
			init_struct (&send_ch[2], 0x02, 0x06);
			init_channel(&channel[3], dewp_pros, 0x2, 0x0);
			init_struct (&send_ch[3], 0x07, 0x07);
			init_channel(&channel[4], humi_pros, 0x0, 0x1);
			init_struct (&send_ch[4], 0x06, 0x05);
			init_channel(&channel[5], humi_pros, 0x2, 0x3);
			init_struct (&send_ch[5], 0x06, 0x08);
			break;
		case	2:
		//SUB2 : TEMP(0.30), TEMP(0.10), TEMP(0.0),JUN BOKSA , BAN BOKSA  --> 5 EA
			Ch_flag[0] = Ch_flag[1] = Ch_flag[2] = ON;
			Ch_flag[3] = Ch_flag[4] = ON;
			init_channel(&channel[0], temp_pros, 0x0, 0x0);
			init_struct (&send_ch[0], 0x02, 0x00);
			init_channel(&channel[1], temp_pros, 0x0, 0x0);
			init_struct (&send_ch[1], 0x02, 0x01);
			init_channel(&channel[2], temp_pros, 0x0, 0x0);
			init_struct (&send_ch[2], 0x02, 0x02);
			init_channel(&channel[3], boksa_pros, 0x0, 0x0);
			init_struct (&send_ch[3], 0x13, 0x0A);
			init_channel(&channel[4], boksa_pros, 0x0, 0x0);
			init_struct (&send_ch[4], 0x14, 0x0B);
			break;
		default  :
		//   MAIN : TEMP(4.0), DEW(4.0), HUMI(4.0) --> 3 EA
			Ch_flag[0] = Ch_flag[1] = Ch_flag[2] = ON;
			init_channel(&channel[0], temp_pros, 0x0, 0x0);
			init_struct (&send_ch[0], 0x02, 0x0C);
			init_channel(&channel[1], dewp_pros, 0x0, 0x0);
			init_struct (&send_ch[1], 0x07, 0x0D);
			init_channel(&channel[2], humi_pros, 0x0, 0x0);
			init_struct (&send_ch[2], 0x06, 0x0E);
			break;
	}		Board_id = 0;
}
//[*]----------------------------------------------------------------------[*]
void init_channel(channel_t *p, void (*ins)(var_back_t *fun), byte ch1, byte ch2)
{
	watch();

	p->real_func = ins;			p->reboot_flag = ON ;
	p->ref_ch1   = ch1;			p->ref_ch2     = ch2;

	p->var.nujuk_sum   = 0.;	p->var.nujuk_spc   = 0.;
	p->var.nujuk_min   = 0.;	p->var.nujuk_max   = 0.;
	p->var.noise_check = 0 ;	p->var.nujuk_count = 0 ;
	p->var.err_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void init_struct(send_ch_t *p, byte s_code, byte s_id)
{
	watch();
	// real sujip data struct initialize
	p->real.header 	= 0xFFFFFFFF;	p->real.tail   	= 0xFEFEFEFE;
	p->real.call_cmd= 'W'       ;	p->real.gubun	= 0x0       ;
	p->real.sen_code= s_code    ;   p->real.sen_id	= s_id      ;
	p->real.status	= 0x0       ;   p->real.xor     = 0         ;
	p->real.add		= 0         ;   p->real.avr		= 0.        ;
	p->real.max		= 0.        ;   p->real.min		= 0.        ;
	p->real.spc		= 0.        ;

	// jugi sujip data struct initialize
	p->jugi.header 	= 0xFFFFFFFF;	p->jugi.tail   	= 0xFEFEFEFE;
	p->jugi.call_cmd= 'W'       ;	p->jugi.gubun	= 600       ;
	p->jugi.sen_code= s_code    ;   p->jugi.sen_id	= s_id      ;
	p->jugi.status	= 0x0       ;   p->jugi.xor     = 0         ;
	p->jugi.add		= 0         ;   p->jugi.avr		= 0.        ;
	p->jugi.max		= 0.        ;   p->jugi.min		= 0.        ;
	p->jugi.spc		= 0.        ;
}
//[*]----------------------------------------------------------------------[*]
void init_sensor_data(byte ch_num)
{
	watch();
	channel[ch_num].reboot_flag = OFF;
	switch(send_ch[ch_num].real.sen_code)	{
		case	0x13:  	case	0x14:
			send_ch[ch_num].real.max = 0.;		send_ch[ch_num].real.min = 0.;
			send_ch[ch_num].jugi.max = 0.;		send_ch[ch_num].jugi.min = 0.;
			send_ch[ch_num].real.avr = 0.;		send_ch[ch_num].jugi.avr = 0.;
			channel[ch_num].var.nujuk_max   = 0.;
			channel[ch_num].var.nujuk_min   = 0.;
			channel[ch_num].var.nujuk_sum   = 0.;
			channel[ch_num].var.nujuk_count = 0 ;
			break;
		default	:
			send_ch[ch_num].real.max = send_ch[ch_num].real.avr;
			send_ch[ch_num].real.min = send_ch[ch_num].real.avr;
			send_ch[ch_num].jugi.max = send_ch[ch_num].real.avr;
			send_ch[ch_num].jugi.min = send_ch[ch_num].real.avr;
			send_ch[ch_num].jugi.avr = send_ch[ch_num].real.avr;
			channel[ch_num].var.nujuk_max = send_ch[ch_num].real.avr;
			channel[ch_num].var.nujuk_min = send_ch[ch_num].real.avr;
			channel[ch_num].var.nujuk_sum   = 0.;
			channel[ch_num].var.nujuk_count = 0 ;
			break;
	}
}
//[*]----------------------------------------------------------------------[*]
void init_maxmin_data(byte ch_num, float max, float min)
{
	watch();

	channel[ch_num].reboot_flag = OFF;
	switch(send_ch[ch_num].real.sen_code)	{
		case	0x13:  	case	0x14:
			send_ch[ch_num].real.max = max;		send_ch[ch_num].real.min = 0.;
			send_ch[ch_num].jugi.max = max;		send_ch[ch_num].jugi.min = 0.;
			channel[ch_num].var.nujuk_max   = max;
			channel[ch_num].var.nujuk_min   = 0. ;
			channel[ch_num].var.nujuk_sum   = 0. ;
			channel[ch_num].var.nujuk_count = 0  ;
			break;
		default	:
			send_ch[ch_num].real.max = max;		send_ch[ch_num].real.min = min;
			send_ch[ch_num].jugi.max = max; 	send_ch[ch_num].jugi.min = min;
			channel[ch_num].var.nujuk_max   = max;
			channel[ch_num].var.nujuk_min   = min;
			channel[ch_num].var.nujuk_sum   = 0. ;
			channel[ch_num].var.nujuk_count = 0  ;
			break;
	}
}
//[*]----------------------------------------------------------------------[*]
void mux_select(byte mux)
{
	watch();

	switch(mux)	{
		case	0:	P3.5 = 0;	P3.4 = 0;	P3.3 = 0;	break;	// ch0 select
		case	1:	P3.5 = 0;	P3.4 = 0;	P3.3 = 1;	break;  // ch1 select
		case	2:	P3.5 = 0;	P3.4 = 1;	P3.3 = 0;	break;  // ch2 select
		case	3:	P3.5 = 0;	P3.4 = 1;	P3.3 = 1;	break;  // ch3 select
		case	4:	P3.5 = 1;	P3.4 = 0;	P3.3 = 0;	break;  // ch4 select
		default	 :  									break;  // not used.
	}
}
//[*]----------------------------------------------------------------------[*]
word ad_convert_read(void)
{
	word	i, value = 0;

	watch();
	for(i=0; i<150; i++)	{	i++;	i--;	}
	MUX_STATUS = HOLD;
	for(i=0; i<15; i++)	{	i++;	i--;	}
	output(ADCMSB, 0xFF);	// Convert start
	for(i=0; i<15; i++)	{	i++;	i--;	}

	//??? AD_STATUS not connect		while(AD_STATUS == 1)	if(i++ > 100)	break;
	for(i=0; i<30; i++)	{	i++;	i--;	}
	disable(ES);
	value   = input(ADCMSB) <<4;	value  |= input(ADCLSB) >>4;
	enable(ES);

	MUX_STATUS = SAMPLE;		return	(word)(value);
}
//[*]----------------------------------------------------------------------[*]
void transmit_check(void)
{
	watch();
	if(com1.tx_sp != com1.tx_ep)	{
		if(com1.tx_flag)	{
			disable(EA);
			SBUF = com1.tx_buf[com1.tx_sp++];
			enable(EA);
			if(com1.tx_sp >= BUFFER_SIZE)	com1.tx_sp = 0;
			com1.tx_flag = OFF;
		}
	}	else	com1.cat_flag = ON;
}
//[*]----------------------------------------------------------------------[*]
void receive_check(void)
{
	watch();
	if(com1.rx_sp != com1.rx_ep)	{
		if(com1.cat_flag == OFF)				return;
		protocol_event(&com1, com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= BUFFER_SIZE)	com1.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] void EX0_int (void)     /* External Interrupt 0 */
{
	disable(EA);
	switch(++Pros_count % 9)	{
		case	1:	if(Real_flag == OFF)
					{	Real_flag = ON;	Real_ch = 0;	}
					break;
		case	5:	if(++Jugi_count == Jugi_value)
					{	Jugi_flag = ON;	Jugi_count = 0;	Sujip_ch = 0;	}
					break;
		case	8:  Pros_count = 0;                             break;
		default  :  											break;
	}
	Hz_check_count = 0;		status.bits.clock = NORMAL;		enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)	{
		disable(RI);
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUFFER_SIZE)		com1.rx_ep = 0;
	} else if(TI)	{
		disable(TI);
		if(com1.tx_sp != com1.tx_ep)	{
			SBUF = com1.tx_buf[com1.tx_sp++];
			if(com1.tx_sp >= BUFFER_SIZE)	com1.tx_sp = 0;
		} else	com1.tx_flag = ON;
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	watch();
	variable_init();	com_port_init();	interrupt_init();

	while(1)
	{
		watch();

		if(Real_flag)	{
			if(Ch_flag[Real_ch])	channel[Real_ch].real_func(&channel[Real_ch].var);
			if(++Real_ch >= 7)		{ 	Real_flag = OFF;	mux_select(0);	}
			else					mux_select(Real_ch);
		}	receive_check();		transmit_check();

		if(Jugi_flag)	{
			if(Ch_flag[Sujip_ch])	jugi_pros(Sujip_ch);
			if(++Sujip_ch >= 7)		Jugi_flag = OFF;
		}	receive_check();		transmit_check();

		if(Hz_check_count++  > MAX_WAIT)	status.bits.clock = FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
