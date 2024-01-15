#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ad_97.h"
#include "pros.hpp"
#include "send.hpp"
#include "serial.hpp"
#include "jojung.hpp"
#include "chk&cat.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil   Date : 1997. 8. 16~ 8. 24[*]",
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
	init_serial_grp(&com1, INTERNAL, 0x0, BPS9600, M22, SMOD_ON, NOT_USED, 2);
	init_serial_pro(&com1, 0, 15, rx_chk, rx_cat1);
	init_serial_pro(&com1, 1, 32, rx_chk, rx_cat2);
	set_protocol_flag(&com1, 0, ON);	set_protocol_flag(&com1, 1, OFF);
}
//[*]----------------------------------------------------------------------[*]
void interrupt_init(void)
{
	watch();
	TMOD |= 0x1;		enable(TR0);		enable(ET0);
	enable(IT0);		enable(EX0);		enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	watch(); 		Board_id = read_board_id();

	// all channel flag off
	ch.flag.word_data = 0x0000;
	// jugi1 is not used	// jugi2 10 min pros
	ch.jugi1 = 0;		ch.jugi1_flag = OFF;	ch.jugi1_used = OFF;
	ch.jugi2 = 600;		ch.jugi2_flag = OFF;	ch.jugi2_used = ON ;

	mux_select(0);		Sec_count1 = 0;			Sec_count2 = 0;

	// fixd sensor(main) 1->sub1 2->sub2
	switch(Board_id)	{
		case	0:	// MAIN PROS
			init_channel_struct(&ch, 0, temp_pros, 0x02, 0x0C, 0x0, 0x0);
			init_channel_struct(&ch, 1, dewp_pros, 0x07, 0x0D, 0x0, 0x0);
			init_channel_struct(&ch, 5, humi_pros, 0x06, 0x0E, 0x0, 0x1);
			ch.flag.bits.ch0_used = ON;		ch.flag.bits.ch1_used = ON;
			ch.flag.bits.ch5_used = ON;
			break;
		case	1:	// SUB1 PROS
			init_channel_struct(&ch, 0, temp_pros, 0x02, 0x03, 0x0, 0x0);
			init_channel_struct(&ch, 1, dewp_pros, 0x07, 0x04, 0x0, 0x0);
			init_channel_struct(&ch, 2, temp_pros, 0x02, 0x06, 0x0, 0x0);
			init_channel_struct(&ch, 3, dewp_pros, 0x07, 0x07, 0x2, 0x0);
			init_channel_struct(&ch, 5, humi_pros, 0x06, 0x05, 0x0, 0x1);
			init_channel_struct(&ch, 6, humi_pros, 0x06, 0x08, 0x2, 0x3);
			ch.flag.bits.ch0_used = ON; 	ch.flag.bits.ch1_used = ON;
			ch.flag.bits.ch2_used = ON; 	ch.flag.bits.ch3_used = ON;
			ch.flag.bits.ch5_used = ON; 	ch.flag.bits.ch6_used = ON;
			break;
		case	6:	Boksa_offset = 2;	// SUB2 PROS
		case	2:
			init_channel_struct(&ch, 0, temp_pros,  0x02, 0x00, 0x0, 0x0);
			init_channel_struct(&ch, 1, temp_pros,  0x02, 0x01, 0x0, 0x0);
			init_channel_struct(&ch, 2, temp_pros,  0x02, 0x02, 0x0, 0x0);
			init_channel_struct(&ch, 3, boksa_pros, 0x13, 0x0A, 0x0, 0x0); // jun boksa
			init_channel_struct(&ch, 4, boksa_pros, 0x14, 0x0B, 0x0, 0x0); // ban boksa
			ch.flag.bits.ch0_used = ON;		ch.flag.bits.ch1_used = ON;
			ch.flag.bits.ch2_used = ON;		ch.flag.bits.ch3_used = ON;
			ch.flag.bits.ch4_used = ON;
			break;
		case	3:	init_channel_struct(&ch, 0, temp_test,  0x02, 0x00, 0x0, 0x0);
					ch.flag.bits.ch0_used = ON;							break;
		case	4:  init_channel_struct(&ch, 0, dewp_test,  0x07, 0x00, 0x0, 0x0);
					ch.flag.bits.ch0_used = ON;							break;
		case	5:  init_channel_struct(&ch, 0, boksa_test, 0x13, 0x00, 0x0, 0x0);
					ch.flag.bits.ch0_used = ON;							break;
		default  :
			init_channel_struct(&ch, 0, temp_pros, 0x02, 0x0C, 0x0, 0x0);
			init_channel_struct(&ch, 1, dewp_pros, 0x07, 0x0D, 0x0, 0x0);
			init_channel_struct(&ch, 5, humi_pros, 0x06, 0x0E, 0x0, 0x1);
			ch.flag.bits.ch0_used = ON;		ch.flag.bits.ch1_used = ON;
			ch.flag.bits.ch5_used = ON;
			break;
	}
}
//[*]----------------------------------------------------------------------[*]
void init_channel_struct(channel_t *p, byte bunho, void (*ins)(var_back_t *fun),
						 byte s_code, byte s_id, byte id1, byte id2)
{
	watch();
	p->num[bunho].sensor.sen_id           = s_id  ;
	p->num[bunho].sensor.sen_code         = s_code;
	p->num[bunho].sensor.status.byte_data = 0     ;

	p->num[bunho].func         = ins;
	p->num[bunho].ref_id1      = id1;	p->num[bunho].ref_id2      = id2;
	p->num[bunho].sensor.avr   = 0. ;	p->num[bunho].sensor.spc   = 0. ;
	p->num[bunho].sensor.min   = 0. ;	p->num[bunho].sensor.max   = 0. ;

	p->num[bunho].var.mux = bunho;
	p->num[bunho].var.init_flag  = ON;	p->num[bunho].var.reboot_flag= ON;
	p->num[bunho].var.real_sum   = 0;	p->num[bunho].var.real_spc   = 0;
	p->num[bunho].var.real_min   = 0;	p->num[bunho].var.real_max   = 0;

	p->num[bunho].var.nujuk1_sum = 0;	p->num[bunho].var.nujuk1_spc = 0;
	p->num[bunho].var.nujuk1_min = 0;	p->num[bunho].var.nujuk1_max = 0;
	p->num[bunho].var.nujuk1_count = 0;

	p->num[bunho].var.nujuk2_sum = 0;	p->num[bunho].var.nujuk2_spc = 0;
	p->num[bunho].var.nujuk2_min = 0;	p->num[bunho].var.nujuk2_max = 0;
	p->num[bunho].var.nujuk2_count = 0;
}
//[*]----------------------------------------------------------------------[*]
void init_sensor_data(ch_info_t *p)
{
	watch();
	p->var.init_flag   = OFF;
	switch(p->sensor.sen_code)	{
		case	0x13:	case	0x14:
			p->var.real_max   = p->var.real_min   = p->var.real_sum   = 0.;
			p->var.nujuk1_sum = p->var.nujuk1_max = p->var.nujuk1_min = 0.;
			p->var.nujuk2_sum = p->var.nujuk2_max = p->var.nujuk2_min = 0.;
			p->var.jugi1_sum  = p->var.jugi1_max  = p->var.jugi1_min  = 0.;
			p->var.jugi2_sum  =	p->var.jugi2_max  = p->var.jugi2_min  = 0.;
			break;
		default		:
			p->var.real_max   = p->var.real_min   = p->var.real_sum;
			p->var.nujuk1_max = p->var.nujuk1_min = p->var.real_sum;
			p->var.nujuk2_max = p->var.nujuk2_min = p->var.real_sum;
			p->var.jugi1_sum  = p->var.jugi1_min  = p->var.jugi1_max = p->var.real_sum;
			p->var.jugi2_sum  = p->var.jugi2_min  = p->var.jugi2_max = p->var.real_sum;

			p->var.jugi1_spc  = p->var.jugi2_spc    = 0.;
			p->var.nujuk1_sum = p->var.nujuk1_count = 0 ;
			p->var.nujuk2_sum = p->var.nujuk2_count = 0 ;
			break;
	}
}
//[*]----------------------------------------------------------------------[*]
void init_maxmin_data(ch_info_t *p, float max, float min)
{
	watch();

	p->var.reboot_flag = OFF;
	p->var.init_flag   = OFF;

	switch(p->sensor.sen_code)	{
		case	0x13:	case	0x14:
			p->var.jugi1_max  = p->var.jugi2_max  =	p->var.real_max   = max;
			p->var.jugi1_min  = p->var.jugi2_min  = p->var.real_min   = 0.;
			p->var.nujuk1_max = p->var.nujuk2_max = p->var.real_max   = max;
			p->var.nujuk1_min = p->var.nujuk2_min = p->var.real_min   = 0.;
			p->var.real_sum   = p->var.nujuk1_sum =	p->var.nujuk2_sum = 0.;
			p->var.jugi1_sum  =	p->var.jugi2_sum  = 0.;
			break;
		default		:
			p->var.real_max   = max;		p->var.real_min   = min;
			p->var.nujuk1_max = p->var.nujuk1_min = p->var.real_sum;
			p->var.nujuk2_max = p->var.nujuk2_min = p->var.real_sum;
			p->var.jugi1_sum  = p->var.jugi1_min  = p->var.jugi1_max = p->var.real_sum;
			p->var.jugi2_sum  = p->var.jugi2_min  = p->var.jugi2_max = p->var.real_sum;

			p->var.jugi1_spc  = p->var.jugi2_spc    = 0.;
			p->var.nujuk1_sum = p->var.nujuk1_count = 0 ;
			p->var.nujuk2_sum = p->var.nujuk2_count = 0 ;
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
void flag_clear(byte chn)
{
	watch();

	switch(chn)	{
		case	0:		ch.flag.bits.ch0 = OFF;			return;
		case	1:  	ch.flag.bits.ch1 = OFF;			return;
		case	2:  	ch.flag.bits.ch2 = OFF;			return;
		case	3:  	ch.flag.bits.ch3 = OFF;			return;
		case	4:  	ch.flag.bits.ch4 = OFF;			return;
		case	5:  	ch.flag.bits.ch5 = OFF;			return;
		case	6:  	ch.flag.bits.ch6 = OFF;			return;
		default	 :										return;
	}
}
//[*]----------------------------------------------------------------------[*]
byte flag_status(byte chn)
{
	watch();

	switch(chn)	{
		case	0:		return ((ch.flag.bits.ch0) ? ON:OFF);
		case	1:  	return ((ch.flag.bits.ch1) ? ON:OFF);
		case	2:  	return ((ch.flag.bits.ch2) ? ON:OFF);
		case	3:  	return ((ch.flag.bits.ch3) ? ON:OFF);
		case	4:  	return ((ch.flag.bits.ch4) ? ON:OFF);
		case	5:  	return ((ch.flag.bits.ch5) ? ON:OFF);
		case	6:  	return ((ch.flag.bits.ch6) ? ON:OFF);
		default	 :		return OFF;
	}
}
//[*]----------------------------------------------------------------------[*]
byte channel_status(byte chn)
{
	watch();

	switch(chn)	{
		case	0:		return ((ch.flag.bits.ch0_used) ? ON:OFF);
		case	1:  	return ((ch.flag.bits.ch1_used) ? ON:OFF);
		case	2:  	return ((ch.flag.bits.ch2_used) ? ON:OFF);
		case	3:  	return ((ch.flag.bits.ch3_used) ? ON:OFF);
		case	4:  	return ((ch.flag.bits.ch4_used) ? ON:OFF);
		case	5:  	return ((ch.flag.bits.ch5_used) ? ON:OFF);
		case	6:  	return ((ch.flag.bits.ch6_used) ? ON:OFF);
		default	 :		return OFF;
	}
}
//[*]----------------------------------------------------------------------[*]
word ad_convert_read(byte mux_ch)
{
	word	i, value = 0, value2 = 0;

	watch();
	MUX_STATUS = HOLD;
	for(i=0; i<15; i++)	{	i++;	i--;	}
	output(ADCMSB, 0xFF);	// Convert start
	for(i=0; i<15; i++)	{	i++;	i--;	}

	//??? AD_STATUS not connect		while(AD_STATUS == 1)	if(i++ > 100)	break;
	for(i=0; i<30; i++)	{	i++;	i--;	}
	disable(ES);
	value   = input(ADCMSB) <<4;	value |= input(ADCLSB) >>4;
	value2  = value;
	value   = input(ADCMSB) <<4;	value |= input(ADCLSB) >>4;
	value2 += value;
	value   = (value2/2.);
	enable(ES);

	MUX_STATUS = SAMPLE;

	for(i=1; i<6; i++)	{	// next sample channel select
		if(channel_status((mux_ch + i) % 5) == ON)		{
			mux_select((mux_ch + i) % 5);	i = END_CAL;
		}
	}
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
	word	count;

	disable(EA);
	switch(++Pros_count % 9)	{
		case	1: 	ch.flag.bits.ch0 = ((ch.flag.bits.ch0_used) ? ON:OFF);	break;
		case	2: 	ch.flag.bits.ch1 = ((ch.flag.bits.ch1_used) ? ON:OFF);  break;
		case	3: 	ch.flag.bits.ch2 = ((ch.flag.bits.ch2_used) ? ON:OFF);  break;
		case	4: 	ch.flag.bits.ch3 = ((ch.flag.bits.ch3_used) ? ON:OFF);  break;
		case	5: 	ch.flag.bits.ch4 = ((ch.flag.bits.ch4_used) ? ON:OFF);  break;
		case	6: 	ch.flag.bits.ch5 = ((ch.flag.bits.ch5_used) ? ON:OFF);  break;
		case	7: 	ch.flag.bits.ch6 = ((ch.flag.bits.ch6_used) ? ON:OFF);  break;
		case	8:	if(++Sec_count1 == ch.jugi1)	{	ch.jugi1_flag = ON;	Sec_count1 = 0;	}
					if(++Sec_count2 == ch.jugi2)	{	ch.jugi2_flag = ON; Sec_count2 = 0;	}
		default  :  Pros_count = 0;											break;
	}
	Hz_check_count = 0;
	for(count = 0; count < 7; count++)	ch.num[count].sensor.status.bits.clock = NORMAL;
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int (void)      /* Timer 0 Overflow */
{
	word	count;

	disable(TR0);
	if(Hz_check_count++  > MAX_WAIT)	{
		for(count = 0; count < 7; count++)	ch.num[count].sensor.status.bits.clock = FAIL;
		Hz_check_count = 0;
	}
	enable(TR0);
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
	word	i;
	watch();
	variable_init();	com_port_init();	interrupt_init();

	while(1)
	{
		watch();		receive_check();	transmit_check();

		for(i=0; i<7; i++)	{
			if(flag_status(i))
			{	ch.num[i].func(&ch.num[i].var);		flag_clear(i);	}
		}		receive_check();		transmit_check();

		if(ch.jugi1_used)	{	if(ch.jugi1_flag)	jugi1_pros();	}
		receive_check();		transmit_check();
		if(ch.jugi2_used)	{	if(ch.jugi2_flag)	jugi2_pros();	}
		receive_check();		transmit_check();
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
