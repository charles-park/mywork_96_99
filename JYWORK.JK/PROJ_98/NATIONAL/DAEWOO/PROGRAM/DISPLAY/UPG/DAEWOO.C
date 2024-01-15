//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "daewoo.h"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#pragma memory=code
byte  NAME[30][60] = {
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]   JIN훊ANG  INDUSTRIAL.,CO LTD                      [*]",
   "[*]  Program : PARK. JongKil   Date : 1998. 10. 28~     [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  Project NAME : DAEWOO JUNG KONG UP                 [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  BOARD : RACK TYPE DISPLAY                          [*]",
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
	value1 = P1 & 0x0F; // read jumper

	while(1) {
		value2 = P1 & 0x0F; // read jumper
		if(value1 == value2) count++;
		else  {  count = 0;  value1 = P1 & 0x0F; }
		if(count > 10)       break;
	}
	value1 = (~value1);

	return 	(value1 & 0x0F);
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

	for(i=0; i<15; i++)   {
		watch();	buf0[i] = BLK;	buf1[i] = BLK;
		if		  (i == 2 )	 {	buf0[i] = buf1[i] = (TABLE[0] & POINT); }
		else	if(i == 3 )  {	buf0[i] = buf1[i] =  TABLE[0]  		;   }
		else	if(i == 6 )  {	buf0[i] = buf1[i] = (TABLE[0] & POINT); }
		else	if(i == 7 )  {	buf0[i] = buf1[i] =  TABLE[0] 		;   }
		else	if(i == 11)  {	buf0[i] = buf1[i] = (TABLE[0] & POINT); }
		else	if(i == 12)  {	buf0[i] = buf1[i] =  TABLE[0] 		;   }
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void com_port_init(void)
{
	watch();
	// Communication Port (Signal Convertor)
	init_serial_grp  (&com1, INTERNAL, 0x00, BPS2400, M22, SMOD_OFF, NOT_USED, 1);
	init_serial_pro  (&com1, 0, 17, rx_chk, rx_cat);
	set_protocol_flag(&com1, 0, ON);

	// Send to PC
	init_serial_grp  (&com2, EXTERNAL, COM_ADDR, BPS2400, M1, NOT_USED,
					 (DATABIT8|PARITY_NO|STOP1), 1);
	init_serial_pro  (&com2, 0, 17, rx_chk, rx_cat);
	set_protocol_flag(&com2, 0, OFF);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_interrupt(void)
{
	watch();

	TMOD |= 0x01;	TH0 = 0xFD;    	TL0 = 0x00;
	enable(ET0);   	enable(TR0);

	T2CON = 0x00;	TH2 = 0x00;		TL2 = 0x00;
	enable(ET2);	enable(TR2);

	enable (EX0);	disable(IT0);
	enable(EA);
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

	if(com2.tx_sp != com2.tx_ep)        {
		if(0x20 & (input(LSR(com2.base))))  {
			output(THR(com2.base), com2.tx_buf[com2.tx_sp++]);
			if(com2.tx_sp >= BUFFER_SIZE)       com2.tx_sp = 0;
		}
	}	else    com2.cat_flag = ON;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void receive_check(void)
{
	watch();
	if(com1.rx_sp != com1.rx_ep)	{
		if(com1.cat_flag != OFF)	{
			protocol_event(&com1, com1.rx_buf[com1.rx_sp++]);
			if(com1.rx_sp >= BUFFER_SIZE) com1.rx_sp = 0;
		}
	}

	watch();
	if(com2.rx_sp != com2.rx_ep)	{
		if(com2.cat_flag != OFF)	{
			protocol_event(&com2, com2.rx_buf[com2.rx_sp++]);
			if(com2.rx_sp >= BUFFER_SIZE) com2.rx_sp = 0;
		}
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
	Disp_mode = REAL_MODE;	Recv_sta   = FAIL;

	// Serial not receive Error detect...
	Error_flag= ON;			Error_count = 0;

	// Send Struct init
	send_struct_init();

	// FND CLEAR..
	output(FND_ADDR0, 0xFF);		output(FND_ADDR1, 0xFF);

	// LED TEST ( ON OFF )
	LED_RED(ON);	LED_BLUE(ON);	delay(5000);	delay(5000);
	delay(5000);	delay(5000);	LED_RED(OFF);	LED_BLUE(OFF);

	// Led display
	led_disp.byte_data = 0x00;		led_disp.bits.real = ON;
	output(LED_ADDR, led_disp.byte_data);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void key_check(void)
{
	watch();
	if(Wait_flag)									return;

	key_data.byte_data = 0x00;
	key_data.byte_data = (~input(KEY_ADDR));

	if(key_data.bits.flag)	{
		key_data.bits.flag = OFF;		led_disp.byte_data = 0x00;
		switch(key_data.bits.value)	{
			case	0:	led_disp.bits.real  = ON;	Disp_mode = REAL_MODE ;	break;
			case	1:	led_disp.bits.min10 = ON;   Disp_mode = MIN10_MODE;	break;
			case	2:	led_disp.bits.hour  = ON;   Disp_mode = HOUR_MODE ;	break;
			default	 :													return;
		}
		send_key(Disp_mode);	output(LED_ADDR, led_disp.byte_data);
		Wait_count = 0;			Wait_flag = ON;
	}	else															return;
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
		data_move(&com1, key.bytes[i]);		watch();
	}	com1.cat_flag = OFF;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_to_pc(sensor_u *p)
{
	word	i;

	watch();
	switch(Disp_mode)	{
		case	REAL_MODE :		p->send.cmd = 'M';		break;
		case	MIN10_MODE:		p->send.cmd = 'T';		break;
		case	HOUR_MODE :		p->send.cmd = 'o';		break;
		default			  :								return;
	}
	p->send.header = 0xFFFF;	p->send.tail = 0xFFFE;
	p->send.xor  = 0x00  ;

	watch();
	for(i=0; i<sizeof(sensor_u); i++)	{
		if(i>1 && i<sizeof(sensor_u) -3)	p->send.xor ^= p->bytes[i];
		data_move(&com2, p->bytes[i]);		watch();
	}   com2.cat_flag = OFF;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void data_move(protocol_grp_t *comx, byte mov_data)
{
	watch();
	comx->tx_buf[comx->tx_ep++] = mov_data;
	if(comx->tx_ep >= BUFFER_SIZE)	comx->tx_ep = 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void test_func(void)
{
	byte	i, j;

	if(!On_off)					On_off = ON;

	for(j=0; j<10; j++)	{
		for(i=0; i<13; i++)	{
			watch();	buf0[i] = TABLE[j];	buf1[i] = TABLE[j];
		}
		led_disp.byte_data = 0x00;				led_disp.bits.real  = ON;
		output(LED_ADDR, led_disp.byte_data);   delay(5000);
		led_disp.byte_data = 0x00;				led_disp.bits.min10 = ON;
		output(LED_ADDR, led_disp.byte_data);   delay(5000);
		led_disp.byte_data = 0x00;				led_disp.bits.hour  = ON;
		output(LED_ADDR, led_disp.byte_data);   delay(5000);
		watch();    	receive_check();     	transmit_check();
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x03] void EX0_int(void)
{
	disable(EX0);
	if(0x4 & (input(IIR(com2.base)) & 0x6)) {
		com2.rx_buf[com2.rx_ep++] = input(RBR(com2.base));
		if(com2.rx_ep >= BUFFER_SIZE)   com2.rx_ep = 0;
	}
	enable (EX0);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
/*
interrupt [0x0B] void T0_int (void)
{
	disable(TR0);  disable(TF0);
	TH0 = 0xFF;    TL0 = 0x00;

	// jansang jaekur
	output(FND_ADDR0, 0xFF);	output(FND_ADDR1, 0xFF);

	output(FND_COMM0, (byte)( Common       & 0x00FF));
	output(FND_COMM1, (byte)((Common >> 8) & 0x00FF));

	output(FND_ADDR0, buf0[Disp_loc]);
	output(FND_ADDR1, buf1[Disp_loc]);

	Common = Common << 1;	Disp_loc++;
	if(Common   >= 0x2000)	Common = 0x0001;
	if(Disp_loc >= 13    )	Disp_loc = 0;

	enable(TR0);
}
*/
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x0B] void T0_int (void)
{
	disable(TR0);  disable(TF0);
	TH0 = 0xFF;    TL0 = 0x00;

	// jansang jaekur
	output(FND_ADDR0, 0xFF);	output(FND_ADDR1, 0xFF);

	output(FND_COMM0, (byte)( Common       & 0x00FF));
	output(FND_COMM1, (byte)((Common >> 8) & 0x00FF));

	// Communication Error
	if(Error_flag)	{
		if(On_off)	{
			output(FND_ADDR0, buf0[Disp_loc]);
			output(FND_ADDR1, buf1[Disp_loc]);
		}	else	{
			output(FND_ADDR0, 0xFF);
			output(FND_ADDR1, 0xFF);
		}
	}	else	{
		output(FND_ADDR0, buf0[Disp_loc]);
		output(FND_ADDR1, buf1[Disp_loc]);
	}

	Common = Common << 1;	Disp_loc++;
	if(Common   >= 0x2000)	Common = 0x0001;
	if(Disp_loc >= 13    )	Disp_loc = 0;

	enable(TR0);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)   {
		disable(RI);		LED_BLUE(ON);
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUFFER_SIZE)    com1.rx_ep = 0;
	} 	else if(TI)  	{
		disable(TI);
		if(com1.tx_sp != com1.tx_ep)  {
			SBUF = com1.tx_buf[com1.tx_sp++];	LED_RED(ON);
			if(com1.tx_sp >= BUFFER_SIZE) com1.tx_sp = 0;
		} 	else   	{
			com1.tx_flag = ON;					LED_RED(OFF);
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

	if(Test_mode != 0x8)	{
		if(On_off_count++ > 10)	{
			On_off_count = 0;
			if(On_off)		{
				On_off = OFF;
				// Serial Data not Receive ON/OFF LED
				if(Error_flag)		output(LED_ADDR, 0x00);
			}	else	{
				On_off = ON ;
				// Serial Data not Receive ON/OFF LED
				if(Error_flag)		output(LED_ADDR, 0xFF);
			}
		}
	}
	enable(TR2);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	init_variable();				Test_mode = read_board_id();
	com_port_init();				init_interrupt();
	display_grp_init();				send_key(Disp_mode);
	while(Test_mode == 0x8)			test_func();

	while(1)
	{
		watch();    	receive_check();     transmit_check();

		// Key Delay Routine
		if(Wait_flag)	{
			if(Wait_count++ > 2000)			{
				Wait_flag = OFF;	output(LED_ADDR, led_disp.byte_data);
			}
		}	key_check();
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]




