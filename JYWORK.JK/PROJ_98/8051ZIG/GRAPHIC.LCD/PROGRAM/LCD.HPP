//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//   Direct Hardware control used..
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
/*
void lcd_printf(byte x, byte y, byte *str)
{
	cursor_locate(x, y);			delay(50);
	while(*str != 0x00)	{
		output(LCD_DATA, *str++);	delay(50);
	}
}
*/
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void display_buffer(byte x, byte y, byte *str, byte pg)
{
	if(y  >= LCD_LINE )				return;
	if(pg >= PAGE     )				return;

	lcd_buffer.page = pg;
	while(*str != 0x00)		{
		lcd_buffer.disp[x++][y][pg] = *str++;
		if(x >= LCD_COLUM )		  x = 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	void my_put_c_in_string(byte c, void *ptr)
{
	*(*(char **) ptr)++ = c;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void lcd_printf(byte x, byte y, byte pg, const byte *format, ...)
{
	va_list		ap;
	byte		i;
	word		cnt;
	byte		buf[21];
	byte		*pointer = &buf[0];

	watch();

	if(y  >= LCD_LINE )				return;
	if(pg >= PAGE     )				return;
	for(i=0 ; i<20; i++)			buf[i] = ' ';
	lcd_buffer.page = pg;

	va_start(ap, format);
	cnt = _formatted_write(format, my_put_c_in_string, (void *) &pointer, ap);
	va_end(ap);

	watch();
	for(i=0; i<20; i++)	lcd_buffer.disp[x++][y][pg] = buf[i];
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void lcd_clr(void)
{
	output(LCD_CON, 0x01);		delay(100);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void cursor_home(void)
{
	output(LCD_CON, 0x02);		delay(100);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void entry_mode(byte cfg)
{
	output(LCD_CON, cfg);		delay(100);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void display_onoff(byte cfg)
{
	output(LCD_CON, cfg & 0xF);	delay(100);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void func_set(byte cfg)
{
	output(LCD_CON, cfg);		delay(300);
	output(LCD_CON, cfg);		delay(200);
	output(LCD_CON, cfg);		delay(100);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void display_buffer_clear(byte pg)
{
	word	j, k;

	watch();
	if(pg >= PAGE-1)			return;
	for(j=0; j<LCD_LINE; j++)	{
		for(k=0; k<LCD_COLUM; k++)	lcd_buffer.disp[k][j][pg] = ' ';
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void cursor_locate(byte x, byte y)
{
	if     (y == 1)		output(LCD_CON, 0xC0             + x);
	else if(y == 2)		output(LCD_CON, 0x80 + LCD_COLUM + x);
	else if(y == 3)		output(LCD_CON, 0xC0 + LCD_COLUM + x);
	else				output(LCD_CON, 0x80             + x);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void lcd_write_byte(byte ch)
{
	output(LCD_DATA, ch);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_lcd(void)
{
	word	i;	// lcd buffer test
	byte	ch = '0';	// page test

	watch();
	// Hardware setup
	func_set(0x38);		display_onoff(0x0C);
	cursor_home(); 		entry_mode(0x06);		lcd_clr();

	// Lcd display buffer clear.
	lcd_buffer.page       = 0;		lcd_buffer.new_page   = OFF;
	lcd_buffer.x          = 0;      lcd_buffer.y 		  = 0;
	lcd_buffer.flag		  = ON;
	for(i=0; i < PAGE; i++) 		display_buffer_clear(i);

//	TMOD = 0x01;	TH0 = 0xFF;		TL0 = 0;
//	enable(ET0);	enable(TR0);	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x0B] void T0_int (void)
{
	disable(TR0);	disable(TF0);
	if(lcd_buffer.flag)	{
		lcd_buffer.flag = OFF;
		cursor_locate(lcd_buffer.x, lcd_buffer.y);
	}	else	{
		lcd_buffer.flag = ON;
		lcd_write_byte(lcd_buffer.disp[lcd_buffer.x][lcd_buffer.y][lcd_buffer.page]);
		if(lcd_buffer.x++ >= LCD_COLUM-1)	{
			lcd_buffer.x = 0;
			if(lcd_buffer.y++ >= LCD_LINE-1)	{
				lcd_buffer.y = 0;
				if(lcd_buffer.new_page)		{
					lcd_buffer.page++;				lcd_buffer.new_page = OFF;
					if(lcd_buffer.page >= PAGE)		lcd_buffer.page = 0;
				}
			}
		}
	}
	TH0 = 0xFF;		TL0 = 0x00;
	enable(TR0);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
