//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "disp_ews.h"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#pragma memory=code
byte  NAME[30][60] = {
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]   JIN훊ANG  INDUSTRIAL.,CO LTD                      [*]",
   "[*]  Program : PARK. JongKil   Date : 1998. 8. 14~      [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  Project NAME : EWS 801A 10 SET                     [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  BOARD : EWS NEW BOARD(DISPLAY)                     [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  VERSION : 2.0                                      [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
};
#pragma memory=default
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
byte read_board_id(void)
{
	byte  value1, value2, count = 0;

	watch();
	value1 = input(JUMPER); // read jumper

	while(1) {
		value2 = input(JUMPER);
		if(value1 == value2) count++;
		else  {  count = 0;  value1 = input(JUMPER);}
		if(count > 10)       break;
	}
	value1 = (~value1);

	return 	(value1);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void delay(word count)
{
	word  i;

	for(i=0; i<count; i++)     {  watch(); i++;  i--;  }
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void display_grp_init(void)
{
	word  i;

	for(i=0; i<12; i++)   {
		watch();	buf0[i] = BLK;	buf1[i] = BLK;	buf2[i] = BLK;
		if		  (i == 2 )	 {	buf0[i] = buf1[i] = buf2[i] = (TABLE[0] & POINT); }
		else	if(i == 3 )  {	buf0[i] = buf1[i] = buf2[i] =  TABLE[0]  		; }
		else	if(i == 6 )  {	buf0[i] = buf1[i] = buf2[i] = (TABLE[0] & POINT); }
		else	if(i == 7 )  {	buf0[i] = buf1[i] = buf2[i] =  TABLE[0] 		; }
		else	if(i == 10)  {	buf0[i] = buf1[i] = buf2[i] = (TABLE[0] & POINT); }
		else	if(i == 11)  {	buf0[i] = buf1[i] = buf2[i] =  TABLE[0] 		; }
	}	buf2[TIME+1] = TABLE[0] & POINT;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void com_port_init(void)
{
	watch();
	init_serial_grp  (&com1, INTERNAL, 0x00, BPS2400, M22, SMOD_OFF, NOT_USED, 1);
	init_serial_pro  (&com1, 0, 17, rx_chk, rx_cat);
	set_protocol_flag(&com1, 0, ON);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_interrupt(void)
{
	watch();

	TMOD |= 0x01;	TH0 = 0xFD;    	TL0 = 0;
	enable(ET0);   	enable(TR0);

	T2CON = 0x00;	TH2   = 0x00;	TL2 = 0x00;
	enable(ET2);	enable(TR2);

	disable(EX0);	disable(EX1);	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void transmit_check(void)
{
	watch();
	if(com1.tx_sp != com1.tx_ep)  {
		if(com1.tx_flag)  	{
			disable(EA);
			SBUF = com1.tx_buf[com1.tx_sp++];
			enable(EA);
			if(com1.tx_sp >= BUFFER_SIZE) com1.tx_sp = 0;
			com1.tx_flag = OFF;
		}
	}  	else  	com1.cat_flag = ON;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void receive_check(void)
{
	watch();
	if(com1.rx_sp != com1.rx_ep)  {
		if(com1.cat_flag == OFF)            return;
		protocol_event(&com1, com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= BUFFER_SIZE) com1.rx_sp = 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_struct_init(void)
{
	watch();
	key.send.header = 0xFFFF;   key.send.cmd   = 'K'      ;
	key.send.sta  	= 0x0   ;   key.send.scode = REAL_MODE;
	key.send.sid  	= 0x0   ;   key.send.avr   = 0x0000   ;
	key.send.spc  	= 0x0000;   key.send.min   = 0x0000   ;
	key.send.max  	= 0x0000;   key.send.xor   = 0x00     ;
	key.send.tail   = 0xFFFE;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_variable(void)
{
	watch();
	Common    = 0x0001;		BUZZER(OFF)     ;	Test_mode    = OFF;
	Disp_loc  = 0x00  ;   	Wait_count = 0  ;	On_off_count = 0  ;
	Time_flag = OFF   ;		Wait_flag  = OFF;	On_off       = OFF;
	Disp_mode = REAL_MODE;	Recv_sta   = NORMAL;

	// Serial not receive Error detect...
	Error_flag= ON;			Error_count = 0;

	// Send Struct init
	send_struct_init();
	// LED TEST ( ON OFF )
	output(LED_OUT, 0x00);	delay(5000);	delay(5000);	output(LED_OUT, 0xFF);

	// Led display
	led_disp.byte_data = 0x00;		led_disp.bits.real = ON;
	output(LED_OUT, ~(led_disp.byte_data));

	// Time Struct initialize
	time.yy = time.mm = time.dd = time.hh = time.bb = time.ss = 0;
	old_time.yy = old_time.mm = old_time.dd = 0;
	old_time.hh = old_time.bb = old_time.ss = 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void key_check(void)
{
	watch();
	if(Wait_flag)									return;

	key_data.byte_data = 0x00;
	key_data.byte_data = (~input(KEY_BOARD));

	if(key_data.bits.flag)	{
		led_disp.byte_data = 0x00;
		switch(key_data.bits.value)	{
			case	0:	led_disp.bits.real  = ON;	Disp_mode = REAL_MODE ;	break;
			case	1:	led_disp.bits.min10 = ON;   Disp_mode = MIN10_MODE;	break;
			case	2:	led_disp.bits.hour  = ON;   Disp_mode = HOUR_MODE ;	break;
			case	3:	led_disp.bits.max   = ON;   Disp_mode = MAX_MODE  ;	break;
			case	4:	led_disp.bits.min   = ON;   Disp_mode = MIN_MODE  ;	break;
			case	5:	led_disp.bits.time  = ON;   Disp_mode = TIME_MODE ;
						if(Time_flag)	{
							Time_flag = OFF;			send_time(&time);
							led_disp.bits.time	= OFF;
							led_disp.bits.real  = ON;	Disp_mode = REAL_MODE ;
						}	else	{
							Time_flag = ON;				send_key(Disp_mode);
							old_time.hh = time.hh;		old_time.bb = time.bb;
						}												break;
			case	6:  if(!Time_flag)									return;
						if(Time_flag)	{
							Error_count = 0;			Error_flag = OFF;
							time.hh = ++time.hh % 24;	Disp_mode = HOUR_UP;
							led_disp.bits.time  = ON;	led_disp.bits.hh_up = ON;
							convert(time.hh * 100 + time.bb, 2);
						}												break;
			case	7:  if(!Time_flag)									return;
						if(Time_flag)	{
							Error_count = 0;			Error_flag = OFF;
							time.bb = ++time.bb % 60;	Disp_mode = MIN_UP;
							led_disp.bits.time  = ON;	led_disp.bits.bb_up = ON;
							convert(time.hh * 100 + time.bb, 2);
						}     											break;
			default	 :													return;
		}	output(LED_OUT, ~(led_disp.byte_data));
		Wait_count = 0;		Wait_flag = ON;		key_data.bits.flag = OFF;
	}	else															return;

	if(!Time_flag)			send_key(Disp_mode);
	else	{
		if(key_data.bits.value < 5)		{
			Time_flag = OFF;
			time.hh = old_time.hh;	time.bb = old_time.bb;
			convert(time.hh * 100 + time.bb, 2);
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_key(byte mode)
{
	word	i;

	watch();
	key.send.header = 0xFFFF;	key.send.cmd 	= 'K'   ;
	key.send.sta	= 0x00	;	key.send.scode	= mode  ;
	key.send.sid	= 0x00	;	key.send.avr	= 0x00	;
	key.send.spc	= 0x00	;	key.send.min	= 0x00	;
	key.send.max	= 0x00	;	key.send.xor    = 0x00  ;
	key.send.tail   = 0xFFFE;

	watch();
	for(i=0; i<sizeof(sensor_u); i++)	{
		if(i>1 && i<sizeof(sensor_u) -3)	key.send.xor ^= key.bytes[i];
		data_move(key.bytes[i]);			watch();
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_time(time_t *t)
{
	word	i;

	key.send.header  = 0xFFFF;	key.send.cmd 	= 'S'	;
	key.send.sta	 = 0x00	 ;	key.send.scode	= 0x00  ;
	key.send.sid	 = 0x00	 ;	key.send.avr	= t->hh	;
	key.send.spc	 = t->bb ;	key.send.min	= t->ss	;
	key.send.max	 = 0x00  ;	key.send.xor    = 0x00  ;
	key.send.tail    = 0xFFFE;

	watch();
	for(i=0; i<sizeof(sensor_u); i++)	{
		if(i>1 && i<sizeof(sensor_u) -3)	key.send.xor ^= key.bytes[i];
		data_move(key.bytes[i]);			watch();
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void data_move(byte mov_data)
{
	watch();
	com1.tx_buf[com1.tx_ep++] = mov_data;
	if(com1.tx_ep >= BUFFER_SIZE)	com1.tx_ep = 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void test_func(void)
{
	byte	i, j;

	for(j=0; j<10; j++)	{
		for(i=0; i<12; i++)	{
			watch();				buf0[i] = TABLE[j];
			buf1[i] = TABLE[j];
			if(i == 10)				buf2[i] = TIME_TABLE[j];
			else                    buf2[i] = TABLE     [j];
		}
		led_disp.byte_data = 0x00;					led_disp.bits.real  = ON;
		output(LED_OUT, ~(led_disp.byte_data));     delay(3000);
		led_disp.byte_data = 0x00;					led_disp.bits.min10 = ON;
		output(LED_OUT, ~(led_disp.byte_data));     delay(3000);
		led_disp.byte_data = 0x00;					led_disp.bits.hour  = ON;
		output(LED_OUT, ~(led_disp.byte_data));     delay(3000);
		led_disp.byte_data = 0x00;					led_disp.bits.max   = ON;
		output(LED_OUT, ~(led_disp.byte_data));     delay(3000);
		led_disp.byte_data = 0x00;					led_disp.bits.min   = ON;
		output(LED_OUT, ~(led_disp.byte_data));     delay(3000);
		led_disp.byte_data = 0x00;					led_disp.bits.time  = ON;
		output(LED_OUT, ~(led_disp.byte_data));     delay(3000);
		led_disp.byte_data = 0x00;					led_disp.bits.hh_up = ON;
		output(LED_OUT, ~(led_disp.byte_data));     delay(3000);
		led_disp.byte_data = 0x00;					led_disp.bits.bb_up = ON;
		output(LED_OUT, ~(led_disp.byte_data));     delay(3000);
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x0B] void T0_int (void)
{
	disable(TR0);  disable(TF0);
	TH0 = 0xFE;    TL0 = 0x00;

	// jansang jaekur
	output(LED_DISP0, 0xFF);
	output(LED_DISP1, 0xFF);
	output(LED_DISP2, 0xFF);

	output(LED_COM0, (byte)( Common       & 0x00FF));
	output(LED_COM1, (byte)((Common >> 8) & 0x00FF));

	output(LED_DISP0, buf0[Disp_loc]);
	output(LED_DISP1, buf1[Disp_loc]);
	output(LED_DISP2, buf2[Disp_loc]);

	Common = Common << 1;	Disp_loc++;
	if(Common   >= 0x1000)	Common = 0x0001;
	if(Disp_loc >= 12    )	Disp_loc = 0;

	enable(TR0);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)   {
		disable(RI);
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUFFER_SIZE)    com1.rx_ep = 0;
	} 	else if(TI)  	{
		disable(TI);
		if(com1.tx_sp != com1.tx_ep)  {
			SBUF = com1.tx_buf[com1.tx_sp++];
			if(com1.tx_sp >= BUFFER_SIZE) com1.tx_sp = 0;
		} 	else   	{
			com1.tx_flag = ON;
		}
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x2B] void T2_int (void)
{
	disable(TR2);  disable(TF2);
	TH2 = 0x00;    TL2 = 0x00;

	if(Error_count < 1200)		Error_count++;
	else						Error_flag = ON;

	if(!Test_mode)	{
		if(On_off_count++ > 10)	{
			On_off_count = 0;
			if(On_off)		{
				On_off = OFF;
				// Serial Data not Receive ON/OFF LED
				if(Error_flag)		output(LED_OUT, 0x00);
				if(Time_flag)	{
					buf2[TIME+1] = buf2[TIME+1] & POINT;
					buf2[TIME+2] = buf2[TIME+2] & POINT;
				}	else	{
					buf2[TIME+1] = buf2[TIME+1] | 0x80;
					buf2[TIME+2] = buf2[TIME+2] | 0x80;
				}
			}	else	{
				On_off = ON ;
				// Serial Data not Receive ON/OFF LED
				if(Error_flag)		output(LED_OUT, 0xFF);
				if(Time_flag)	{
					buf2[TIME+1] = buf2[TIME+1] & POINT;
					buf2[TIME+2] = buf2[TIME+2] & POINT;
				}	else	{
					buf2[TIME+1] = buf2[TIME+1] & POINT;
					buf2[TIME+2] = buf2[TIME+2] & POINT;
				}
			}
		}
	}
	enable(TR2);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{

	init_variable();		Test_mode = read_board_id();
	com_port_init();		init_interrupt();
	display_grp_init();		send_key(Disp_mode);

	while(Test_mode)		test_func();

	while(1)
	{
		watch();    	receive_check();     transmit_check();

		// Key Delay Routine
		if(Wait_flag)	{
			if(Wait_count++ > 2000)			{
				Wait_flag = OFF;
				if 		  (Disp_mode == HOUR_UP)	led_disp.bits.hh_up = OFF;
				else	if(Disp_mode == MIN_UP )	led_disp.bits.bb_up = OFF;
				output(LED_OUT, ~(led_disp.byte_data));
			}
		}	key_check();

		if(Time_flag && Error_flag)	{
			Time_flag = OFF;	send_time(&time);	send_key(REAL_MODE);
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
