#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "snow.h"
#include "pros.hpp"
#include "send.hpp"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil   Date : 1997. 10. 6~      [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : KANG-RUNG SNOW SENSOR               [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD : SERIAL BOARD                               [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  SENSOR : SNOW SENSOR(1)                            [*]",
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
		if(value1 == value2)		count++;
		else	{	count = 0;		value1 = input(JUMPER);}
		if(count > 10)				break;
	}
	value1 = (~value1) & 0x3F;		return (value1);
}
//[*]----------------------------------------------------------------------[*]
void com_port_init(byte led_mode)
{
	init_serial_grp(&com1, INTERNAL, 0x0, BPS9600, M22, SMOD_ON, NOT_USED, 2);
	init_serial_pro(&com1, 0, 15, rx_chk, rx_cat1);
	init_serial_pro(&com1, 1, 32, rx_chk, rx_cat2);
	set_protocol_flag(&com1, 0, ON);	set_protocol_flag(&com1, 1, OFF);

	init_serial_grp(&com2, EXTERNAL, 0x9000, BPS1200, M1,
					NOT_USED, (DATABIT8|STOP1|PARITY_NO), 1);
	init_serial_pro(&com2, 0, 9, snow_chk, snow_cat1);
	set_protocol_flag(&com2, 0, ON);

	init_serial_grp(&com3, EXTERNAL, 0xA000, BPS1200, M1,
					NOT_USED, (DATABIT8|STOP1|PARITY_NO), 1);

	// DIP SW 5 ON -> daeju LED Protocol catch
	if(led_mode == 1)	{
		init_serial_pro(&com3, 0, 37, led_chk_daeju, led_cat_daeju);
	}	else	{
		init_serial_pro(&com3, 0, 36, led_chk_30, led_cat_30);
	}	set_protocol_flag(&com3, 0, ON);
}
//[*]----------------------------------------------------------------------[*]
void init_struct(sensor_field_u *p, byte id, byte s_code, byte cmd, word jugi)
{
	watch();
	p->snow.header   = 0xFFFFFFFF;	p->snow.tail     = 0xFEFEFEFE;
	p->snow.sen_id   = id        ;	p->snow.sen_code = s_code    ;
	p->snow.call_cmd = cmd       ;	p->snow.gubun    = jugi      ;
	p->snow.xor      = 0         ;	p->snow.add      = 0         ;
	p->snow.avr      = 0.        ;	p->snow.spc      = 0.        ;
	p->snow.min      = 0.        ;  p->snow.max      = 0.        ;
	p->snow.status.byte_data = 0 ;
}
//[*]----------------------------------------------------------------------[*]
void interrupt_init(void)
{
	watch();
	output(MCR(com2.base),CALL_OFF);		// Sensor Output Disable
	TMOD |= 0x5;	TH0  = 0xFF;	TL0  = 0xFF;
	enable(EX0);	disable(IT0);	enable(ET0);	enable(TR0);
	enable(PS);		enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	watch();

	Board_id = read_board_id();		status.byte_data = 0x0;

	if(Board_id & 0x10)		com_port_init(1);	// Daeju LED
	else					com_port_init(0);	// Jinyabg LED

	Board_id &= 0x0F;
	init_struct(&real  , Board_id, 0x0C, 'W', 0x0);
	init_struct(&jugi  , Board_id, 0x0C, 'W', 600);
	init_struct(&offset, Board_id, 0x0C, '*', 0x0);
	init_struct(&maxmin, Board_id, 0x0C, '$', 0x0);
	interrupt_init();
}
//[*]----------------------------------------------------------------------[*]
void init_sensor_data(void)
{
	Reboot_flag = 0;
	real.snow.avr = 0.;		real.snow.max = 0.;
	real.snow.min = 0.;		real.snow.spc = 0.;
	jugi.snow.avr = 0.;		jugi.snow.max = 0.;
	jugi.snow.min = 0.;		jugi.snow.spc = 0.;
}
//[*]----------------------------------------------------------------------[*]
void init_maxmin_data(float max, float min)
{
	Reboot_flag = 0;
	real.snow.avr = 0.;		real.snow.max = 0.;
	real.snow.min = 0.;		real.snow.spc = 0.;
	jugi.snow.avr = 0.;		jugi.snow.max = 0.;
	jugi.snow.min = 0.;		jugi.snow.spc = 0.;
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
		if(com1.cat_flag == OFF)					return;
		protocol_event(&com1, com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= BUFFER_SIZE)	com1.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
void receive_check_23(void)
{
	watch();
	if(com2.rx_sp != com2.rx_ep)	{
		protocol_event(&com2, com2.rx_buf[com2.rx_sp++]);
		if(com2.rx_sp >= BUFFER_SIZE)	com2.rx_sp = 0;
	}
	if(com3.rx_sp != com3.rx_ep)	{
		protocol_event(&com3, com3.rx_buf[com3.rx_sp++]);
		if(com3.rx_sp >= BUFFER_SIZE)	com3.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] void EX0_int (void)
{
	disable(EA);
	if     (0x04 & (input(IIR(com2.base)) & 0x6))	{
		com2.rx_buf[com2.rx_ep++] = input(RBR(com2.base));
		if(com2.rx_ep >= BUFFER_SIZE)	com2.rx_ep = 0;
	}
	else if(0x04 & (input(IIR(com3.base)) & 0x6))	{
		com3.rx_buf[com3.rx_ep++] = input(RBR(com3.base));
		if(com3.rx_ep >= BUFFER_SIZE)	com3.rx_ep = 0;
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int (void)
{
	disable(EA);		disable(TF0);

	switch(++Pros_count % 9)	{
		case	1:  Real_flag = ON;							break;

		case	5:  if(Sec_count++ >= 600)
					{	Jugi_flag = ON;		Sec_count = 0;}
															break;
		case	8:  if(Sample_delay++ > 5)
					{	output(MCR(com2.base), CALL_ON);	Sample_delay = 0;}
					Pros_count = 0;							break;
		default	 :											break;
	}	Hz_check_count = 0;		status.bits.clock = NORMAL;
		TH0 = 0xFF;		TL0 = 0xFF;

	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)	{
		disable(RI);		enable(GREEN);
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUFFER_SIZE)		com1.rx_ep = 0;
	}	else if(TI)	{
		disable(TI);
		if(com1.tx_sp != com1.tx_ep)	{
			enable(RED);
			SBUF = com1.tx_buf[com1.tx_sp++];
			if(com1.tx_sp >= BUFFER_SIZE)	com1.tx_sp = 0;
		}	else	{
			com1.tx_flag = ON;		disable(RED);
		}
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	watch();	disable(GREEN);		disable(RED);	variable_init();
	while(1)	{
		watch();
		receive_check();	transmit_check();		receive_check_23();

		if(Hz_check_count++ > MAX_WAIT)		status.bits.clock  = FAIL;
		if(Sensor_ack_count > SENSOR_WAIT)	status.bits.sensor = FAIL;
		else								status.bits.sensor = NORMAL;

		if(Real_flag)	{	snow_pros();	Real_flag = OFF;	}
		if(Jugi_flag)	{	jugi_pros();	Jugi_flag = OFF;	}
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
