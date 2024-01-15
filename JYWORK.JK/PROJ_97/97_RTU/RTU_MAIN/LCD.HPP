//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]              97' AWS and AMOS RTU(Remote Terminal Unit)             [*]
//[*]                   lcd initial and display program                   [*]
//[*]                Copyright Jinyang industrial co.,ltd                 [*]
//[*]                       Program by Jinmin - Mok                       [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	Use under header with lcd program .....                           [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define LEFT			0x18
#define RIGHT			0x1C
#define LCD_CON			0x0000				// LCD Enable Address
#define LCD_DATA		LCD_CON+1
#define LINE			4
#define COLUM			20					// LCD COLUM Value
#define SETUP			0x38
#define Ent_Mode		0x06
#define Dis_On_Off		0x0C				// default setup
#define L_DLYTIME		5000				// Long Delay Value Configuration
#define	C_ON			0x0F
#define	C_OFF			0x0C
#define	LCD_BUSY(x)		while((input(x)) & 0x80)
//[*]---------------------------------------------------------------------[*]
static byte DISP_BUFFER[81];
static byte TEMP_BUFFER[81];
//[*]---------------------------------------------------------------------[*]
typedef struct 	lcd__t	{
	unsigned char	lcd_redisplay_flag;
	unsigned char	lcd_pos;
	unsigned char	buf_pos;
	unsigned int	cursor_cnt;
	unsigned char	cursor_sta;
	unsigned char	cursor_pos;
}	lcd_t;

static lcd_t		lcd;

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_lcd(void);
void func_set(void);
void display_onoff(idata unsigned char DCB);
void l_rethome(void);
void entry_mode(idata unsigned char IDS);
void lcd_clear(void);
static void my_put_c_in_string (char c, void *ptr);
void lcd_printf(byte x, byte y, byte del,const char *format, ...);
void scroll_up(byte sl, byte lea, byte tl);
void lcd_memory_move(byte dir);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_lcd(void)
{
	/* LCD Enable ---> 0 */
	delay(1000);
	func_set();								// First Config DATA
	func_set();
	func_set();
	LCD_BUSY(0x00);
	display_onoff(Dis_On_Off);				// Display On/Off Control
	l_rethome();							// LCD Cursor Return Home
	entry_mode(Ent_Mode);					// Entry Mode Setup
	lcd_clear();							// LCD CLEAR Function
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
// 8 Bit interface, 2 Line display, 5*7 font config
// RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0
//  0   0    0   0   1   DL  N   F   *   *
// DL = 8/4 bit, N = 2/1 Line, F = 5*10 / 5*7 dot
void func_set(void)      /* First Config data write to LCD  */
{
	WATCHDOG;
	LCD_CS();
	 output(LCD_CON,SETUP);
	 output(LCD_CON,SETUP);
	 output(LCD_CON,SETUP);
	iRAM_CS();
	delay(1000);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
/* Display ON/OFF Control DATA Write to LCD         */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   1   D   C   B           */
/* D = All Display ON/OFF, C = Cursor ON/OFF, B = Cursor Position BLINK ON/OFF */
void display_onoff(idata unsigned char DCB)
{
	WATCHDOG;
	LCD_CS();
	 output(LCD_CON,DCB & 0xf);
	 output(LCD_CON,DCB & 0xf);
	 output(LCD_CON,DCB & 0xf);
	iRAM_CS();
	delay(300);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
/* Cursor & DD RAM Address return HOME(Address 0,0) */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   0   0   1   *           */
void l_rethome(void)
{
	WATCHDOG;
	LCD_CS();
	 output(LCD_CON,0x02);
	 output(LCD_CON,0x02);
	 output(LCD_CON,0x02);
	iRAM_CS();
	delay(1000);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
/* Cursor Entry MODE Write data to LCD              */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   0   1   I/D S           */
/* I/D = Increment/Decrement, S = Display Shift/Cursor Shift */
void entry_mode(idata unsigned char IDS)
{
	WATCHDOG;
	LCD_CS();
	 output(LCD_CON,IDS & 0x7);
	 output(LCD_CON,IDS & 0x7);
	 output(LCD_CON,IDS & 0x7);
	iRAM_CS();
	delay(500);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
/* CLEAR Display data                               */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   0   0   0   1           */
void lcd_clear(void)      /* LCD Clear Function */
{
	byte i;
	WATCHDOG;
	LCD_CS();
	 output(LCD_CON,0x01);
	 output(LCD_CON,0x01);
	 output(LCD_CON,0x01);
	RAM_CS();
	for(i=0; i<80; i++)	DISP_BUFFER[i] = 0x20;
	lcd.lcd_redisplay_flag = 1;
	delay(500);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static void my_put_c_in_string (char c, void *ptr)
{
	*(*(char **) ptr)++ = c;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void lcd_printf(byte x, byte y, byte del,const char *format, ...)
{
	va_list 		ap;
	byte 			i;
	int  			cnt;
	char			Buf[21];
	char 		   *pointer = &Buf[0];
	static byte 	yarray[4] = { 0,2,1,3 };

	WATCHDOG;
	va_start(ap, format);
	cnt = _formatted_write(format, my_put_c_in_string, (void *) &pointer, ap);
	va_end(ap);

	if(del)	for(i=0; i<20; i++)	DISP_BUFFER[(yarray[y] * 20) + i] = 0x20;
	for(i=0; i<cnt;i++,x++)		DISP_BUFFER[(yarray[y] * 20) + x] = Buf[i];
	lcd.lcd_redisplay_flag = 1;
	WATCHDOG;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void scroll_up(byte sl, byte lea, byte tl)
{
	byte i, x;
	static byte 	yarray[4] = { 0,2,1,3 };
	for(i=0; i<lea; i++)	{
		WATCHDOG;
		for(x=0; x < 20; x++)
		DISP_BUFFER[(yarray[tl+i]*20)+x] = DISP_BUFFER[(yarray[sl+i]*20)+x];
	}
	for(x=0; x < 20; x++)
	DISP_BUFFER[(yarray[tl+i]*20)+x] = 0x20;
	lcd.lcd_redisplay_flag = 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void lcd_memory_move(byte dir)
{
	byte i;
	WATCHDOG;
	switch(dir)	{
		case 'p':	for(i=0; i<80; i++)	TEMP_BUFFER[i] = DISP_BUFFER[i];
					break;
		case 'g':	for(i=0; i<80; i++)	DISP_BUFFER[i] = TEMP_BUFFER[i];
					lcd.lcd_redisplay_flag = 1;
					break;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
