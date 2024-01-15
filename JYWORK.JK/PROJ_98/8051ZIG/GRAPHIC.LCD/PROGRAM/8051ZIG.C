//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include <io51.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "8051zig.h"
#include "lcd.hpp"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include "ASCII.H"		//	ASCII FONT
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_onoff(byte cs, byte status)
{
	if(cs > 1)							return;
	if(cs)		{
		if(status)		output(GR_LCD_CNT1, 0x3F);
		else			output(GR_LCD_CNT1, 0x3E);
	}	else	{
		if(status)		output(GR_LCD_CNT0, 0x3F);
		else			output(GR_LCD_CNT0, 0x3E);
	}
	delay(10);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
byte graphic_lcd_status_read(byte cs)
{
	if(cs > 1)						return 0xFF;
	if(cs)	return	(byte)(input(GR_LCD_STA1));
	else	return	(byte)(input(GR_LCD_STA0));
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_reset(void)
{
	GR_LCD_RST(OFF);	delay(100);
	GR_LCD_RST(ON); 	delay(100);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_set_disp_line(byte cs, byte line)
{
	if(line >= GR_LCD_LINE)				return;
	if(cs > 1)							return;
	if(cs)	{
		output(GR_LCD_CNT1, 0xC0 + line );	delay(10);	// display line
	}
	else	{
		output(GR_LCD_CNT0, 0xC0 + line );	delay(10);	// display line
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_set_page(byte cs, byte pg)
{
	if(pg >= GR_LCD_X)					return;
	if(cs > 1)							return;
	if(cs)		{
		output(GR_LCD_CNT1, 0xB8 + pg);		delay(10);	// x page
	}	else	{
		output(GR_LCD_CNT0, 0xB8 + pg);		delay(10);	// x page
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_set_yline(byte yline)
{
	if(yline >= GR_LCD_Y)				return;
	if(yline >= GR_LCD_LINE)	{
		output(GR_LCD_CNT1, 0x40 + (yline % GR_LCD_LINE));	delay(10);	// y address
	}	else	{
		output(GR_LCD_CNT0, 0x40 +  yline               );	delay(10);	// y address
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_put_data(byte addr, byte p_data)
{
	graphic_lcd_set_yline(addr);
	if(addr >= GR_LCD_Y)			return;
	if(addr >= GR_LCD_LINE)	{
		output(GR_LCD_DATA1, p_data);	delay(20);
	}	else	{
		output(GR_LCD_DATA0, p_data);	delay(20);
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_clear(void)
{
	byte	addr, pg;

	graphic_lcd_onoff(0, OFF);
	graphic_lcd_onoff(1, OFF);
	for(pg = 0; pg <= GR_LCD_X; pg++)	{ //???
		graphic_lcd_set_page(0, pg);
		graphic_lcd_set_page(1, pg);
		for(addr = 0; addr < GR_LCD_Y; addr++)	{
			graphic_lcd_put_data(addr, 0x0);
		}
	}
	graphic_lcd_set_page(0, 0);
	graphic_lcd_set_page(1, 0);
	graphic_lcd_onoff(0, ON);
	graphic_lcd_onoff(1, ON);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_init(void)
{
	graphic_lcd_reset();
	graphic_lcd_onoff(0, ON);
	graphic_lcd_onoff(1, ON);
	graphic_lcd_set_disp_line(0, 0);
	graphic_lcd_set_disp_line(1, 0);
	graphic_lcd_set_page(0, 1);
	graphic_lcd_set_page(1, 1);
	graphic_lcd_clear();

	lcd_printf(0,0,0,"sample %d %c",ON,'0');	//test
	delay(5000);
	delay(5000);
	delay(5000);
	delay(5000);
	delay(5000);
	delay(5000);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void graphic_lcd_test(void)
{
	word 	ap = 0;
	byte	l = 0;
	graphic_lcd_init();

	while(1)	{
		for(ap = 0; ap < 255; ap++)	{
			l = (byte)ap;
			display_buffer_clear(0);
			display_buffer(0,0,&l,0);
			display_buffer(0,1,&l,0);
			display_buffer(0,2,&l,0);
			display_buffer(0,3,&l,0);

			watch();
			display_buffer(10,0,&l,0);
			display_buffer(10,1,&l,0);
			display_buffer(10,2,&l,0);
			display_buffer(10,3,&l,0);

			graphic_lcd_clear();
			EngFontPutch((byte)ap);	// test
			delay(5000);
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void led_lcd_test(void)
{
	graphic_lcd_test();	// test

	P1.6 = P1.5 = P1.4 = 0;		delay(5000);		delay(5000);
	P1.6 = P1.5 = P1.4 = 1;		delay(5000);		delay(5000);
					  //01234567890123456789
	display_buffer(0,0,"JINYANG INDUSTRIAL.",0);
	display_buffer(0,1,"H/W DESINE BY JK.  ",0);
	display_buffer(0,2,"SAMPLE BOARD       ",0);
	display_buffer(0,3,"GRAPHIC LCD TEST!!!",0);
	delay(5000);	delay(5000);	delay(5000);	delay(5000);	delay(5000);
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
	TMOD = 0x01;	TH0 = 0xFF;		TL0 = 0;
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
		case	UP	 :		break;
		case	DOWN :  	break;
		case	LEFT :  	break;
		case	RIGHT:  	break;
		case	ENTR :  	break;
		case	ESC  :  	break;
		case	MODE :  	break;
		case	POFF :  	break;
		default		 :  	break;
	}	key_wait = ON;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void EngFontPutch(byte ascii)
{
	byte ft, addr = 0, i, j;
	byte	t = 0x80;		// rotate left -> right
	word	y = 0x0001;       // rotate down -> up

	for(j = 0; j < 8; j++)	{
		conv.word_data = 0;
		for(i=0; i<16; i++)	{
			ft = EngFont[ascii][i];
			if(ft & t)	conv.word_data |= y;
			y = y <<1;
		}
		graphic_lcd_set_page(0, 0);
		graphic_lcd_put_data(addr, conv.byte_data.b2);
		graphic_lcd_set_page(0, 1);
		graphic_lcd_put_data(addr, conv.byte_data.b1);
		graphic_lcd_set_page(0, 2);
		graphic_lcd_put_data(addr, conv.byte_data.b2);
		graphic_lcd_set_page(0, 3);
		graphic_lcd_put_data(addr, conv.byte_data.b1);
		graphic_lcd_set_page(0, 4);
		graphic_lcd_put_data(addr, conv.byte_data.b2);
		graphic_lcd_set_page(0, 5);
		graphic_lcd_put_data(addr, conv.byte_data.b1);
		graphic_lcd_set_page(0, 6);
		graphic_lcd_put_data(addr, conv.byte_data.b2);
		graphic_lcd_set_page(0, 7);
		graphic_lcd_put_data(addr, conv.byte_data.b1);

		graphic_lcd_set_page(1, 0);
		graphic_lcd_put_data(addr + GR_LCD_LINE, conv.byte_data.b2);
		graphic_lcd_set_page(1, 1);
		graphic_lcd_put_data(addr + GR_LCD_LINE, conv.byte_data.b1);
		graphic_lcd_set_page(1, 2);
		graphic_lcd_put_data(addr + GR_LCD_LINE, conv.byte_data.b2);
		graphic_lcd_set_page(1, 3);
		graphic_lcd_put_data(addr + GR_LCD_LINE, conv.byte_data.b1);
		graphic_lcd_set_page(1, 4);
		graphic_lcd_put_data(addr + GR_LCD_LINE, conv.byte_data.b2);
		graphic_lcd_set_page(1, 5);
		graphic_lcd_put_data(addr + GR_LCD_LINE, conv.byte_data.b1);
		graphic_lcd_set_page(1, 6);
		graphic_lcd_put_data(addr + GR_LCD_LINE, conv.byte_data.b2);
		graphic_lcd_set_page(1, 7);
		graphic_lcd_put_data(addr + GR_LCD_LINE, conv.byte_data.b1);

		graphic_lcd_set_page(0, 0);
		graphic_lcd_put_data(addr + 8, conv.byte_data.b2);
		graphic_lcd_set_page(0, 1);
		graphic_lcd_put_data(addr + 8, conv.byte_data.b1);
		graphic_lcd_set_page(0, 2);
		graphic_lcd_put_data(addr + 8, conv.byte_data.b2);
		graphic_lcd_set_page(0, 3);
		graphic_lcd_put_data(addr + 8, conv.byte_data.b1);
		graphic_lcd_set_page(0, 4);
		graphic_lcd_put_data(addr + 8, conv.byte_data.b2);
		graphic_lcd_set_page(0, 5);
		graphic_lcd_put_data(addr + 8, conv.byte_data.b1);
		graphic_lcd_set_page(0, 6);
		graphic_lcd_put_data(addr + 8, conv.byte_data.b2);
		graphic_lcd_set_page(0, 7);
		graphic_lcd_put_data(addr + 8, conv.byte_data.b1);


		graphic_lcd_set_page(1, 0);
		graphic_lcd_put_data(addr + 8 + GR_LCD_LINE, conv.byte_data.b2);
		graphic_lcd_set_page(1, 1);
		graphic_lcd_put_data(addr + 8 + GR_LCD_LINE, conv.byte_data.b1);
		graphic_lcd_set_page(1, 2);
		graphic_lcd_put_data(addr + 8 + GR_LCD_LINE, conv.byte_data.b2);
		graphic_lcd_set_page(1, 3);
		graphic_lcd_put_data(addr + 8 + GR_LCD_LINE, conv.byte_data.b1);
		graphic_lcd_set_page(1, 4);
		graphic_lcd_put_data(addr + 8 + GR_LCD_LINE, conv.byte_data.b2);
		graphic_lcd_set_page(1, 5);
		graphic_lcd_put_data(addr + 8 + GR_LCD_LINE, conv.byte_data.b1);
		graphic_lcd_set_page(1, 6);
		graphic_lcd_put_data(addr + 8 + GR_LCD_LINE, conv.byte_data.b2);
		graphic_lcd_set_page(1, 7);
		graphic_lcd_put_data(addr + 8 + GR_LCD_LINE, conv.byte_data.b1);
		addr++;
		y = 0x0001;
		t = t >>1;
	}

//	for(fy=0; fy<16; fy++)	{
//		graphic_lcd_put_data(fy, EngFont[ascii][fy]);
//	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	watch();
	init_lcd();		init_interrupt();	led_lcd_test();

	while(1)	{
		watch();
		key_check();

		if(key_wait)	{
			if(key_wait_count++ > 2000)		{
				key_wait_count = 0;		key_wait = OFF;
			}
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//	lcd_buffer.page = 8;
//	display_buffer_clear(0);
//	display_buffer(0,0," ERROR KEY!!!", 0);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]

