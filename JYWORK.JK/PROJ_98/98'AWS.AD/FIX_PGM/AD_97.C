#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ad_97.h"
#include "pros_lib.hpp"
#include "send.hpp"
#include "serial.hpp"
#include "chk&cat.hpp"
#include "default.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
byte	NAME[30][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil   Date : 1998. 1. 19~      [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 2.xx                                     [*]",
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
	board.byte_data = 0;   	board.byte_data = read_board_id();

	Board_status = 0;	// All Normal(CLOCK & AD_CONVERTER)
	LED_RED = OFF;		LED_BLUE = OFF;
	pre_ch_init(0);		pre_ch_init(1);		// PRE channel init

	// Channel flag Initialize.
	for(i=0; i<7; i++)	Ch_flag[i] = OFF;

	Real_flag = OFF;	Jugi_flag = OFF;	mux_select(0);
	Real_ch = 0;		Sujip_ch = 0;       Jugi_count = 0;

//	if(board.dip.auto_set)	{
//		if(serial_eep_read() == FAIL)	config_fail_send();
//	}
	switch(board.dip.mode)	{
		case	0:		default  :		config_0();			break;
		case	1:						config_1();			break;
		case	2:						config_2();			break;
		case	3:						config_3();			break;
	}   board_setup();
}
//[*]----------------------------------------------------------------------[*]
void board_setup(void)
{
	Jugi_value = DEFAULT.SUJIP_COUNT;

	if(DEFAULT.CH0_ON_OFF)		{
		init_channel(&channel[0], DEFAULT.CH0_RUN_FUNCTION_NUMBER
								, DEFAULT.CH0_REFERENCE1 , DEFAULT.CH0_REFERENCE2);
		init_struct (&send_ch[0], DEFAULT.CH0_SENSOR_NAME, DEFAULT.CH0_SENSOR_ID);
		Ch_flag[0] = ON;
	}
	if(DEFAULT.CH1_ON_OFF)	{
		init_channel(&channel[1], DEFAULT.CH1_RUN_FUNCTION_NUMBER
								, DEFAULT.CH1_REFERENCE1 , DEFAULT.CH1_REFERENCE2);
		init_struct (&send_ch[1], DEFAULT.CH1_SENSOR_NAME, DEFAULT.CH1_SENSOR_ID);
		Ch_flag[1] = ON;
	}
	if(DEFAULT.CH2_ON_OFF)	{
		init_channel(&channel[2], DEFAULT.CH2_RUN_FUNCTION_NUMBER
								, DEFAULT.CH2_REFERENCE1 , DEFAULT.CH2_REFERENCE2);
		init_struct (&send_ch[2], DEFAULT.CH2_SENSOR_NAME, DEFAULT.CH2_SENSOR_ID);
		Ch_flag[2] = ON;
	}
	if(DEFAULT.CH3_ON_OFF)	{
		init_channel(&channel[3], DEFAULT.CH3_RUN_FUNCTION_NUMBER
								, DEFAULT.CH3_REFERENCE1 , DEFAULT.CH3_REFERENCE2);
		init_struct (&send_ch[3], DEFAULT.CH3_SENSOR_NAME, DEFAULT.CH3_SENSOR_ID);
		Ch_flag[3] = ON;
	}
	if(DEFAULT.CH4_ON_OFF)	{
		init_channel(&channel[4], DEFAULT.CH4_RUN_FUNCTION_NUMBER
								, DEFAULT.CH4_REFERENCE1 , DEFAULT.CH4_REFERENCE2);
		init_struct (&send_ch[4], DEFAULT.CH4_SENSOR_NAME, DEFAULT.CH4_SENSOR_ID);
		Ch_flag[4] = ON;
	}
	if(DEFAULT.CH5_ON_OFF)	{
		init_channel(&channel[5], DEFAULT.CH5_RUN_FUNCTION_NUMBER
								, DEFAULT.CH5_REFERENCE1 , DEFAULT.CH5_REFERENCE2);
		init_struct (&send_ch[5], DEFAULT.CH5_SENSOR_NAME, DEFAULT.CH5_SENSOR_ID);
		Ch_flag[5] = ON;
	}
	if(DEFAULT.CH6_ON_OFF)	{
		init_channel(&channel[6], DEFAULT.CH6_RUN_FUNCTION_NUMBER
								, DEFAULT.CH6_REFERENCE1 , DEFAULT.CH6_REFERENCE2);
		init_struct (&send_ch[6], DEFAULT.CH6_SENSOR_NAME, DEFAULT.CH6_SENSOR_ID);
		Ch_flag[6] = ON;
	}
}
//[*]----------------------------------------------------------------------[*]
void init_channel(channel_t *p, void (*ins)(var_back_t *fun), byte ch1, byte ch2)
{
	byte	i;
	watch();

	p->real_func = ins;			p->reboot_flag = ON ;
	p->ref_ch1   = ch1;			p->ref_ch2     = ch2;

	p->var.move_cnt    = 0 ;	p->var.move_pos    = 0 ;
	for(i=0; i<20; i++)			p->var.move_aver[i]= 0 ;

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
void pre_ch_init(byte ch_num)
{
	byte	i;
	switch(ch_num)	{
		case	0:
			PRE1_CLR = ON;
			for(i=0; i<10; i++)		{	i++;	watch();	i--;	}
			PRE1_CLR = OFF;
			break;
		case	1:
			PRE2_CLR = ON;
			for(i=0; i<10; i++)		{	i++;	watch();	i--;	}
			PRE2_CLR = OFF;
			break;
		default	 :
			break;
	}
}
//[*]----------------------------------------------------------------------[*]
void init_sensor_data(byte ch_num)
{
	watch();
	channel[ch_num].reboot_flag = OFF;
	switch(send_ch[ch_num].real.sen_code)	{
		case    0x03:			pre_ch_init(ch_num);
		case	0x09:	case	0x08:	case	0x13:  	case	0x14:
			send_ch[ch_num].real.max = 0.;		send_ch[ch_num].real.min = 0.;
			send_ch[ch_num].jugi.max = 0.;		send_ch[ch_num].jugi.min = 0.;
			send_ch[ch_num].real.avr = 0.;		send_ch[ch_num].jugi.avr = 0.;
			channel[ch_num].var.nujuk_max   = 0.;
			channel[ch_num].var.nujuk_min   = 0.;
			channel[ch_num].var.nujuk_spc   = 0. ;
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
			channel[ch_num].var.nujuk_spc   = 0. ;
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
		case    0x03:
			pre_ch_init(ch_num);
			send_ch[ch_num].real.max = max;		send_ch[ch_num].real.min = min;
			send_ch[ch_num].jugi.max = max;		send_ch[ch_num].jugi.min = min;
			send_ch[ch_num].real.avr = max;		send_ch[ch_num].jugi.avr = 0.;
			channel[ch_num].var.nujuk_max   = max;
			channel[ch_num].var.nujuk_min   = 0. ;
			channel[ch_num].var.nujuk_spc   = 0. ;
			channel[ch_num].var.nujuk_sum   = max;
			channel[ch_num].var.nujuk_count = 0  ;
			break;
		case    0x09:
			send_ch[ch_num].real.max = max;		send_ch[ch_num].real.min = min;
			send_ch[ch_num].jugi.max = max;		send_ch[ch_num].jugi.min = min;
			send_ch[ch_num].real.avr = 0. ;		send_ch[ch_num].jugi.avr = 0.;
			channel[ch_num].var.nujuk_max   = 0. ;
			channel[ch_num].var.nujuk_min   = 0. ;
			channel[ch_num].var.nujuk_spc   = 0. ;
			channel[ch_num].var.nujuk_sum   = max * 60.;
			channel[ch_num].var.nujuk_count = 0  ;
			break;
		case	0x13:  	case	0x14:
			send_ch[ch_num].real.max = max;		send_ch[ch_num].real.min = 0.;
			send_ch[ch_num].jugi.max = max;		send_ch[ch_num].jugi.min = 0.;
			channel[ch_num].var.nujuk_max   = max;
			channel[ch_num].var.nujuk_min   = 0. ;
			channel[ch_num].var.nujuk_spc   = 0. ;
			channel[ch_num].var.nujuk_sum   = 0. ;
			channel[ch_num].var.nujuk_count = 0  ;
			break;
		default	:
			send_ch[ch_num].real.max = max;		send_ch[ch_num].real.min = min;
			send_ch[ch_num].jugi.max = max; 	send_ch[ch_num].jugi.min = min;
			channel[ch_num].var.nujuk_max   = max;
			channel[ch_num].var.nujuk_min   = min;
			channel[ch_num].var.nujuk_spc   = 0. ;
			channel[ch_num].var.nujuk_sum   = 0. ;
			channel[ch_num].var.nujuk_count = 0  ;
			break;
	}
}
//[*]----------------------------------------------------------------------[*]
void mux_select(byte mux)
{
	watch();

	if(mux > 4)			return;
	else				output(MUX_PORT, mux);
}
//[*]----------------------------------------------------------------------[*]
word ad_convert_read(void)
{
	word	i, value = 0;

	watch();
	for(i=0; i<150; i++)	{	i++;	i--;	}
	MUX_STATUS = HOLD;
	for(i=0; i<15 ; i++)	{	i++;	i--;	}
	output(ADCMSB, 0xFF);	// Convert start
	for(i=0; i<15 ; i++)	{	i++;	i--;	}

	while(AD_STATUS)	if(i++ > 100)	break;

	if(i >= 100)		Board_status |= ADC_FAIL  ;
	else				Board_status &= ADC_NORMAL;

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
			if(++Real_ch >= 7)		{ 	Real_flag = OFF;	mux_select(0);	}
			else					mux_select(Real_ch);
		}
		if(Jugi_flag)	{
			if(Ch_flag[Sujip_ch])	jugi_pros(Sujip_ch);
			if(++Sujip_ch >= 7)		Jugi_flag = OFF;
		}
		if(Hz_check_count++  > MAX_WAIT)	Board_status |= CLOCK_FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
