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
byte	NAME[30][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil   Date : 1999. 5. 8~       [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : DAEJUN WONJARUK                     [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD : 98' AD BOARD ( 4 Channel )                 [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Sensor CODE Description.                           [*]",
   "[*]  WIND = 0x0, WSPD  = 0x1, TEMP  = 0x2               [*]",
   "[*]  HUMI = 0x6, SIGMA = 0x1B                           [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Sensor ID Description.                             [*]",
   "[*]  (DIP SW == 0x00)                                   [*]",
   "[*]  WDIR  = 0x0, WSPD = 0x1, TEMP = 0x4, HUMI = 0x6    [*]",
   "[*]  SIGMA = 0x8                                        [*]",
   "[*]  (DIP SW == 0x01)                                   [*]",
   "[*]  WDIR  = 0x2, WSPD = 0x3, TEMP = 0x5, HUMI = 0x7    [*]",
   "[*]  SIGMA = 0x9                                        [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  A/D Channel Description.                           [*]",
   "[*]  VERSION : 1.1x                                     [*]",
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
	value1 = (~value1) & 0x1F;	return (value1);
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
	watch();

	Board_id = 0x00;	Board_id = read_board_id();

	Board_status = 0;	// All Normal(CLOCK & AD_CONVERTER)
	LED_RED = OFF;		LED_BLUE = OFF;

	// Channel flag Initialize.
	for(i=0; i<6; i++)	Ch_flag[i] = OFF;

	Real_ch    = 0;		Real_flag = OFF;	mux_select(0);
	Jugi_count = 0;		Jugi_value = 900;	Sujip_ch = 0;	Jugi_flag = OFF;

	wd_sin_tot    = 0.;		wd_cos_tot    = 0.;
	sigma_tot     = 0.; 	Wd15avr       = 0.;

	switch(Board_id)	{
		case	0:		default  :
			// WD , WS, TEMP, HUMI, SIGMA
			Ch_flag[0] = Ch_flag[1] = Ch_flag[2] = Ch_flag[3] = Ch_flag[4] = ON;
			init_channel(&channel[0], wdir_pros , 0x1, 0x1);
			init_struct (&send_ch[0], 0x00, 0x00);
			init_channel(&channel[1], wspd_pros , 0x0, 0x0);
			init_struct (&send_ch[1], 0x01, 0x02);
			init_channel(&channel[2], temp_pros , 0x0, 0x0);
			init_struct (&send_ch[2], 0x02, 0x04);
			init_channel(&channel[3], humi_pros , 0x0, 0x0);
			init_struct (&send_ch[3], 0x06, 0x06);
			init_channel(&channel[4], sigma_pros, 0x0, 0x0);
			init_struct (&send_ch[4], 0x12, 0x08);
			break;
		case	1:
			// WD , WS, TEMP, HUMI, SIGMA
			Ch_flag[0] = Ch_flag[1] = Ch_flag[2] = Ch_flag[3] = Ch_flag[4] = ON;
			init_channel(&channel[0], wdir_pros , 0x1, 0x1);
			init_struct (&send_ch[0], 0x00, 0x01);
			init_channel(&channel[1], wspd_pros , 0x0, 0x0);
			init_struct (&send_ch[1], 0x01, 0x03);
			init_channel(&channel[2], temp_pros , 0x0, 0x0);
			init_struct (&send_ch[2], 0x02, 0x05);
			init_channel(&channel[3], humi_pros , 0x0, 0x0);
			init_struct (&send_ch[3], 0x06, 0x07);
			init_channel(&channel[4], sigma_pros, 0x0, 0x0);
			init_struct (&send_ch[4], 0x12, 0x09);
			break;
		case	2:
			// WD , WS, TEMP, HUMI, SIGMA
			Ch_flag[0] = Ch_flag[1] = ON;
			init_channel(&channel[0], fan_pros , 0x0, 0x0);
			init_struct (&send_ch[0], 0x1A, 0x0A);
			init_channel(&channel[1], fan_pros , 0x0, 0x0);
			init_struct (&send_ch[1], 0x1A, 0x0B);
			break;
	}
}
//[*]----------------------------------------------------------------------[*]
void init_channel(channel_t *p, void (*ins)(var_back_t *fun), byte ch1, byte ch2)
{
	byte	i;
	watch();

	p->real_func = ins;
	p->ref_ch1   = ch1;			p->ref_ch2     = ch2;

	p->var.move_cnt    = 0 ;	p->var.move_pos    = 0 ;
	for(i=0; i<20; i++)			p->var.move_aver[i]= 0 ;

	p->var.nujuk_sum   = 0.;	p->var.nujuk_spc   = 0.;
	p->var.nujuk_min   = 0.;	p->var.nujuk_max   = 0.;
	p->var.nujuk_count = 0 ;
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
	p->jugi.call_cmd= 'W'       ;	p->jugi.gubun	= 900       ;
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
	switch(send_ch[ch_num].real.sen_code)	{
		case	0x12:			break;
		default	:
			send_ch[ch_num].real.max = send_ch[ch_num].real.avr;
			send_ch[ch_num].real.min = send_ch[ch_num].real.avr;
			send_ch[ch_num].jugi.max = send_ch[ch_num].real.avr;
			send_ch[ch_num].jugi.min = send_ch[ch_num].real.avr;
			send_ch[ch_num].jugi.avr = send_ch[ch_num].real.avr;
			Wd15avr				     = send_ch[ch_num].real.avr;
			send_ch[ch_num].real.spc = 0.;
			send_ch[ch_num].jugi.spc = 0.;
			channel[ch_num].var.nujuk_count = 0 ;
			break;
	}
}
//[*]----------------------------------------------------------------------[*]
void mux_select(byte mux)
{
	byte	i;
	watch();

	if(mux > 3)			return;

	output(MUX_PORT, mux);
//	for(i=0; i<15; i++)	{	i++;	i--;	}
}
//[*]----------------------------------------------------------------------[*]
word ad_convert_read(void)
{
	word	i, value = 0;

	watch();
	for(i=0; i<100; i++)	{	i++;	i--;	}
	i = 0;
	disable(EA);
	MUX_STATUS = HOLD;

	output(ADCMSB, 0xFF);	// Convert start
//	for(i=0; i<15 ; i++)	{	i++;	i--;	}

//	while(AD_STATUS)	if(i++ > 100)	break;
	while(AD_STATUS);//	if(i++ > 100)	break;

	value   = input(ADCMSB) <<4;	value  |= input(ADCLSB) >>4;
	enable(EA);
	MUX_STATUS = SAMPLE;

	if(i >= 100)		Board_status |= ADC_FAIL  ;
	else				Board_status &= ADC_NORMAL;

	return	(word)(value);
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
		case	1:	case	2:	case	3:
		case	4:  case	5:	case	6:
					if(Real_flag == OFF)	Real_flag = ON;
											break;
		case	8:	if(++Jugi_count == Jugi_value)
					{	Jugi_flag = ON;	Jugi_count = 0;	Sujip_ch = 0;	}
					Pros_count = 0;			break;
		default  :  						break;
	}
	Hz_check_count = 0;		Board_status &= CLOCK_NORMAL;	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)	{
		disable(RI);	LED_BLUE = ON;
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUFFER_SIZE)		com1.rx_ep = 0;
	} else if(TI)	{
		disable(TI);	LED_RED = ON;
		if(com1.tx_sp != com1.tx_ep)	{
			SBUF = com1.tx_buf[com1.tx_sp++];
			if(com1.tx_sp >= BUFFER_SIZE)	com1.tx_sp = 0;
		} else	{
			com1.tx_flag = ON;	LED_RED = OFF;
		}
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
		watch();		receive_check();	transmit_check();
		if(Real_flag)	{
			if(Ch_flag[Real_ch])	channel[Real_ch].real_func(&channel[Real_ch].var);
			if(++Real_ch >= 6)		Real_ch = 0;
			mux_select(Real_ch);    Real_flag = OFF;
		}	receive_check();		transmit_check();

		watch();
		if(Jugi_flag)	{
			if(Ch_flag[Sujip_ch])	jugi_pros(Sujip_ch);
			if(++Sujip_ch >= 6)		Jugi_flag = OFF;
		}	receive_check();		transmit_check();

		if(Hz_check_count++  > MAX_WAIT)	Board_status |= CLOCK_FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]




