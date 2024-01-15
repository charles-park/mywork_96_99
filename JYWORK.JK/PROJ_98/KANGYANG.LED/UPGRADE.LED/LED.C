#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "led.h"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil   Date : 1998. 2. 23~      [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : KWANG YANG LED(96')                 [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD : OLD LED BOARD(96')                         [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Wind Direction, Wind Speed, Temperature, Humiduty  [*]",
   "[*]  Precipitation, Yester Precipitation                [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 1.xx                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]----------------------------------------------------------------------[*]
void com_port_init(void)
{
	init_serial_grp(&com1, INTERNAL, 0x0, BPS2400, M11, SMOD_OFF, NOT_USED, 1);
	init_serial_pro(&com1, 0, 146, rx_chk, rx_cat1);
	set_protocol_flag(&com1, 0, ON);
}
//[*]----------------------------------------------------------------------[*]
void interrupt_init(void)
{
	TMOD |= 0x1;	enable(IT0);	enable(EX0);	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	byte 	i;

	for(i=0; i<4; i++)	{		// LED TEST '8'
		output(WD      +i, led_font[8]);	output(WS      +i, led_font[8]);
		output(TEMP_AVR+i, led_font[8]);	output(TEMP_MIN+i, led_font[8]);
		output(PRE_AVR +i, led_font[8]);	output(PRE_MIN +i, led_font[8]);
		output(HUMI    +i, led_font[8]);
	}	delay();

	for(i=0; i<5; i++)	{
		disp_buf.wd      [i] = 0;	disp_buf.ws      [i] = 0;
		disp_buf.temp_avr[i] = 0;	disp_buf.temp_min[i] = 0;
		disp_buf.pre_avr [i] = 0;	disp_buf.pre_min [i] = 0;
		disp_buf.humi    [i] = 0;
	}
	led_disp(WD      , disp_buf.wd      );	led_disp(WS      , disp_buf.ws      );
	led_disp(TEMP_AVR, disp_buf.temp_avr);	led_disp(TEMP_MIN, disp_buf.temp_min);
	led_disp(PRE_AVR , disp_buf.pre_avr );	led_disp(PRE_MIN , disp_buf.pre_min );
	led_disp(HUMI    , disp_buf.humi    );
}
//[*]----------------------------------------------------------------------[*]
void receive_check(void)
{
	if(com1.rx_sp != com1.rx_ep)	{
		protocol_event(&com1, com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= RX_SIZE)	com1.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int   (void)   /* Timer 0 Overflow            */
{
	disable(EA);
	if(count1++ > 5000)	{
		count1 = 0;
		if(count2++ > 5000)	{	count2 = 0;	Led_send = ON;	}
	}
	TH0 = 0;	TL0 = 0;	TF0 = 0;
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)	{
		disable(RI);
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= RX_SIZE)		com1.rx_ep = 0;
	} else if(TI)		disable(TI);
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void led_disp(word addr, byte *p)
{
	byte 	i;
	word	value = 0;


	for(i=0; i<4; i++)		output(addr+i, 0);

	if(p[1] != 0)	{
		output(addr  , led_font[p[1]]);		output(addr+1, led_font[p[2]]);
	}	else if(p[2] != 0)					output(addr+1, led_font[p[2]]);
	output(addr+2, led_font[p[3]] + 0x80);	output(addr+3, led_font[p[4]]);

	if(addr == TEMP_AVR)	{
		if(Minus1)	{	output(addr  , 0x40);	Minus1 = OFF;	}
	}	else if (addr == TEMP_MIN)	{
		if(Minus2)	{	output(addr  , 0x40);	Minus2 = OFF;	}
	}
}
//[*]----------------------------------------------------------------------[*]
void delay(void)
{
	word	i = 0, j = 0;
	for(i=0; i<100; i++)	{
		for(j=0; j<500; j++)	;
	}
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	byte	i= 0, j = 0;
	variable_init();	com_port_init();	interrupt_init();

	while(1)
	{
		receive_check();

		if(Led_send)	{
			led_disp(WD      , disp_buf.wd      );	led_disp(WS      , disp_buf.ws      );
			led_disp(TEMP_AVR, disp_buf.temp_avr);	led_disp(TEMP_MIN, disp_buf.temp_min);
			led_disp(PRE_AVR , disp_buf.pre_avr );	led_disp(PRE_MIN , disp_buf.pre_min );
			led_disp(HUMI    , disp_buf.humi    ); 	Led_send = OFF;
		}
	}
}
//[*]----------------------------------------------------------------------[*]
