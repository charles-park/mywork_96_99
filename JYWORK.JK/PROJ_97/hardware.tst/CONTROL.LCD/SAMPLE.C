#include <io51.h>
#include <stdio.h>
#include <stdlib.h>

//#define	output(addr, odata)		((*(char *)(0x010000 + addr)) = odata)
//#define	input(addr)             (*(char *)(0x010000 + addr))
#define		ON				1
#define		OFF				0
#define		disable(x)		x = OFF
#define		enable(y)		y = ON
//typedef	unsigned char 	byte;
//typedef unsigned int	word;

//[*]----------------------------------------------------------------------[*]
// Link lcd.r03
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] 	void T0_int(void);
			extern	void func_set(byte cfg);
			extern	void display_onoff(byte cfg);
			extern	void cursor_home(void);
			extern 	void entry_mode(byte cfg);
			extern 	void lcd_clr(void);
			extern	void lcd_printf(byte x, byte y, byte *str);
			extern	void cursor_locate(byte x, byte y);
			extern	void lcd_write_byte(byte ch);
			extern	void delay(word count);
					void init_lcd(void);
					void init_timer(void);
					void main(void);
//[*]----------------------------------------------------------------------[*]
void init_lcd(void)
{
	func_set(0x38);		display_onoff(0x0C);
	cursor_home();      entry_mode(0x06);
	lcd_clr();
}
//[*]----------------------------------------------------------------------[*]
void init_timer(void)
{
	TMOD = 0x21;	TH0 = 0xF0;		TL0 = 0x0;
	SCON = 0x52;	enable(ET0);	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	init_lcd();		init_timer();

	// sample program
	while(1)	{
		lcd_printf(0, 0, "START PROGRAM..!");
		delay(2000);
		lcd_printf(0, 1, "END   PROGRAM..!");
		delay(2000);
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
