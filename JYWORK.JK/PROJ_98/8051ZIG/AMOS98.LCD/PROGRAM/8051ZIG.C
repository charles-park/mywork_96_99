//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include <io51.h>
#include <stdio.h>
#include <stdlib.h>
#include "8051zig.h"
#include "lcd.hpp"
#include "serial.hpp"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void volume_test(void)
{
	byte	d = 0, i;
	watch();
	for(i=0; i<8;i++)	{
		d |= led_test[i];
		output(VOLUME_L, d);		delay(5000);
	}	d = 0;

	for(i=0; i<8;i++)	{
		d |= led_test[i];
		output(VOLUME_H, d);		delay(5000);
	}	d = 0;

}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void led_lcd_test(void)
{
	SP(OFF);
	output(VOLUME_H, 0x0);		output(VOLUME_L, 0x0);
	P1.6 = P1.5 = P1.4 = 0;		delay(5000);		delay(5000);
	P1.6 = P1.5 = P1.4 = 1;		delay(5000);		delay(5000);
					  //01234567890123456789
	display_buffer(0,0,"JINYANG INDUSTRIAL.",0);
	display_buffer(0,1,"H/W DESINE BY JK.  ",0);
	display_buffer(0,2,"SAMPLE BOARD       ",0);
	display_buffer(0,3,"BAUDRATE 9600 BPS  ",0);
	delay(5000);	delay(5000);	delay(5000);	delay(5000);	delay(5000);
	delay(5000);	delay(5000);	delay(5000);	delay(5000);	delay(5000);

	display_buffer_clear(0);                        delay(5000);
	display_buffer(0,0,"Volume LED TEST....",0);	volume_test();
	display_buffer(0,0,"SPEAKER TEST.......",0);	SP(ON);
	delay(5000);	delay(5000);	delay(5000);	SP(OFF);
	output(VOLUME_H, 0x0);							output(VOLUME_L, 0x0);

	display_buffer_clear(0);                        delay(5000);
	display_buffer(0,0,"VOICE SYNTHESIZER..",0);
	delay(5000);	delay(5000);	delay(5000);	delay(5000);	delay(5000);
	display_buffer(0,1,"COMPUTER BOOTING...",0);
	delay(5000);	delay(5000);	delay(5000);	delay(5000);	delay(5000);
	display_buffer(0,2,"HARDWARE INITIALIZE",0);
	delay(5000);	delay(5000);	delay(5000);	delay(5000);	delay(5000);
	display_buffer(0,3,"PROGRAM RUNNING!!!!",0);
	delay(5000);	delay(5000);	delay(5000);	delay(5000);	delay(5000);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)		watch();
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_interrupt(void)
{
	TMOD |= 0x01;	TH0 = 0xFF;		TL0 = 0;
	enable(ET0);	enable(TR0);	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void key_check(void)
{
	watch();

	key.byte_data = 0x0;	key.byte_data = input(KEY_ADDR);
	if(key_wait)									return;
	if(!key.bits.flag)								return;

	key.bits.flag = OFF;
	switch(key.bits.value)	{
		case	UP	 :		key_send_pc('U');	break;
		case	DOWN :  	key_send_pc('D');	break;
		case	LEFT :  	key_send_pc('L');	break;
		case	RIGHT:  	key_send_pc('R');	break;
		case	ENTR :  	key_send_pc('E');	break;
		case	ESC  :  	key_send_pc('C');	break;
		case	MODE :  	key_send_pc('M');	break;
		case	POFF :  	key_send_pc('F');	break;
		default		 :  						break;
	}	key_wait = ON;		SP(ON);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	watch();
	init_lcd();			init_serial(92, rx_chk, rx_cat);
	init_interrupt();	led_lcd_test();

	while(1)	{
		watch();
		receive_check();		transmit_check();	key_check();

		if(key_wait)			{
			if(key_wait_count++ > 2000)		{
				key_wait_count = 0;		key_wait = OFF;		SP(OFF);
			}
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//	lcd_buffer.page = 8;
//	display_buffer_clear(0);
//	display_buffer(0,0," ERROR KEY!!!", 0);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]

