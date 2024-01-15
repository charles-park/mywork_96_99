//[*]----------------------------------------------------------------------[*]
#include <io51.h>
//[*]----------------------------------------------------------------------[*]
#define		LCD_COLUM		16
#define		LCD_LINE		2
#define		LCD_CON         0xC000
#define		LCD_DATA		0xC001
//[*]----------------------------------------------------------------------[*]
#define		ON				1
#define		OFF				0
#define		disable(x)		x = OFF
#define		enable(y)		y = ON
//[*]----------------------------------------------------------------------[*]
typedef struct lcd__t	{
	byte	x;
	byte	y;
	byte	buffer[LCD_COLUM * LCD_LINE];
}	lcd_t;

static	lcd_t	lcd;
//[*]----------------------------------------------------------------------[*]
void func_set(byte cfg);
void display_onoff(byte cfg);
void cursor_home(void);
void entry_mode(byte cfg);
void lcd_clr(void);
void lcd_printf(byte x, byte y, byte *str);
void cursor_locate(byte x, byte y);
void lcd_write_byte(byte ch);
void delay(word count);
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int(void)
{
	disable(EA);

	cursor_locate(lcd.x, lcd.y);
	lcd_write_byte(lcd.buffer[(lcd.y * LCD_COLUM) + lcd.x]);

	if(lcd.x++ >= LCD_COLUM)	{
		lcd.x = 0;
		if(lcd.y++ >= LCD_LINE)			lcd.y = 0;
	}

	TH0 = 0xF0;		TL0 = 0x0;
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void func_set(byte cfg)
{
	output(LCD_CON, cfg);
	delay(300);
	output(LCD_CON, cfg);
	delay(200);
	output(LCD_CON, cfg);
	delay(100);
}
//[*]----------------------------------------------------------------------[*]
void display_onoff(byte cfg)
{
	output(LCD_CON, cfg & 0xF);
	delay(100);
}
//[*]----------------------------------------------------------------------[*]
void cursor_home(void)
{
	output(LCD_CON, 0x02);
	delay(100);
}
//[*]----------------------------------------------------------------------[*]
void entry_mode(byte cfg)
{
	output(LCD_CON, cfg);
	delay(100);
}
//[*]----------------------------------------------------------------------[*]
void lcd_clr(void)
{
	word	i;

	disable(EA);
	output(LCD_CON, 0x01);			// Hardware clear

	lcd.x = 0;		lcd.y = 0;      // Software clear
	for(i=0; i<sizeof(lcd.buffer); i++)		lcd.buffer[i] = 0x20;

	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void lcd_printf(byte x, byte y, byte *str)
{
	while(*str != 0x00)	{
		lcd.buffer[(y * LCD_COLUM) + x] = *str++;

		if(x++ >= LCD_COLUM)	{
			x = 0;
			if(y++ >= LCD_LINE)			y = 0;
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void cursor_locate(byte x, byte y)
{
	if     (y == 1)		output(LCD_CON, 0xC0             + x);
	else if(y == 2)		output(LCD_CON, 0x80 + LCD_COLUM + x);
	else if(y == 3)		output(LCD_CON, 0xC0 + LCD_COLUM + x);
	else				output(LCD_CON, 0x80             + x);
}
//[*]----------------------------------------------------------------------[*]
void lcd_write_byte(byte ch)
{
	output(LCD_DATA, ch);
}
//[*]----------------------------------------------------------------------[*]
void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)	{ 	i++;	i--;	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]

