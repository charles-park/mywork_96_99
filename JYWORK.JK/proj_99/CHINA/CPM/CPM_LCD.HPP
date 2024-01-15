//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define	LCD_DATA		LCD_ADD+1
#define	LCD_SETUP		0x3C
#define	LCD_ENT_MODE	0x06
#define	LCD_DIS_ON_OFF	0x0C
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void lcd_rethome(void);
void lcd_init(void);
void lcd_clear(void);							// LCD Clear Function
void lcd_func_set(void);						// First Config data write to LCD
void lcd_write_char(unsigned char s);			// One charicter write to LCD
void lcd_display_onoff(unsigned char DCB);
void lcd_entry_mode(unsigned char IDS);
void lcd_printf(Byte x, Byte y, Byte del,const char *format, ...);
void lcd_scroll_up(Byte sl, Byte lea, Byte tl);
void lcd_memory_move(Byte dir);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static Byte LCD_BUFFER[20];
static Byte LCD_TMP_BF[20];
static Byte LCD_REDISPLAY_FLAG = 0;
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void lcd_init(void)
{
	lcd_func_set();								// First Config DATA
	lcd_display_onoff(LCD_DIS_ON_OFF);			// Display On/Off Control
	lcd_rethome();								// LCD Cursor Return Home
	lcd_entry_mode(LCD_ENT_MODE);				// Entry Mode Setup
	lcd_display_onoff(LCD_DIS_ON_OFF);			// Display On/Off Control
	lcd_clear();			delay(5);			// LCD CLEAR Function
	output(LCD_ADD,0x80);	delay(3);
	output(LCD_ADD,0x80);	delay(3);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
// 8 Bit interface, 2 Line display, 5*7 font config
// RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0
//  0   0    0   0   1   DL  N   F   *   *
// DL = 8/4 bit, N = 2/1 Line, F = 5*10 / 5*7 dot
void lcd_func_set(void)							// First Config data write to LCD
{
	output(LCD_ADD,LCD_SETUP);	delay(3);
	output(LCD_ADD,LCD_SETUP);  delay(3);
	output(LCD_ADD,LCD_SETUP);  delay(3);
	output(LCD_ADD,LCD_SETUP);  delay(3);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
// Display ON/OFF Control DATA Write to LCD
// RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0
// 0   0    0   0   0   0   1   D   C   B
// D = All Display ON/OFF, C = Cursor ON/OFF, B = Cursor Position BLINK ON/OFF
void lcd_display_onoff(unsigned char DCB)
{
	output(LCD_ADD,DCB & 0xf);
	output(LCD_ADD,DCB & 0xf);
	delay(5);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
// Cursor & DD RAM Address return HOME(Address 0,0)
// RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0
// 0   0    0   0   0   0   0   0   1   *
void lcd_rethome(void)
{
	output(LCD_ADD,0x01);
	output(LCD_ADD,0x01);
	delay(5);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
// Cursor Entry MODE Write data to LCD
// RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0
// 0   0    0   0   0   0   0   1   I/D S
// I/D = Increment/Decrement, S = Display Shift/Cursor Shift
void lcd_entry_mode(unsigned char IDS)
{
	output(LCD_ADD,IDS & 0x7);
	output(LCD_ADD,IDS & 0x7);
	output(LCD_ADD,IDS & 0x7);
	delay(2);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
// CLEAR Display data
// RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0
// 0   0    0   0   0   0   0   0   0   1
void lcd_clear(void)							// LCD Clear Function
{
	Byte i;
	output(LCD_ADD,0x01);
	output(LCD_ADD,0x01);
	output(LCD_ADD,0x01);
	for(i=0; i<16; i++)	LCD_BUFFER[i] = 0x20;
	LCD_REDISPLAY_FLAG = 1;
	delay(2);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static void my_put_c_in_string (char c, void *ptr)
{
	*(*(char **) ptr)++ = c;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void lcd_printf(Byte x, Byte y, Byte del,const char *format, ...)
{
	va_list 		ap;
	Byte 			i;
	int  			cnt;
	char			Buf[20];
	char 		   *pointer = &Buf[0];

	va_start(ap, format);
	cnt = _formatted_write(format, my_put_c_in_string, (void *) &pointer, ap);
	va_end(ap);

	if(del)	for(i=0; i<16; i++)	LCD_BUFFER[i] = 0x20;
	for(i=0; i<cnt; i++,   x++)	LCD_BUFFER[x] = Buf[i];
	LCD_REDISPLAY_FLAG = 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void lcd_memory_move(Byte dir)
{
	Byte i;
	switch(dir)	{
		case 'p':	for(i=0; i<16; i++)	LCD_TMP_BF[i] = LCD_BUFFER[i];
					break;
		case 'g':	for(i=0; i<16; i++)	LCD_BUFFER[i] = LCD_TMP_BF[i];
					LCD_REDISPLAY_FLAG = 1;
					break;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
