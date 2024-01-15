//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		watch()			P1.7 = 1 - P1.7
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		ON				1
#define		OFF				0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		TX_LED(x)		P1.6 = 1 - x
#define		RX_LED(y)		P1.5 = 1 - y
#define		CATCH_LED(z)	P1.4 = 1 - z
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		KEY_ADDR		0xA000
#define		UP				0
#define		ENTR        	1
#define		DOWN        	4
#define		ESC         	5
#define		LEFT        	8
#define		MODE        	9
#define		RIGHT       	12
#define		POFF        	13
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		LCD_CON			0x9000
#define		LCD_DATA		0x9001
#define		LCD_COLUM		20
#define		LCD_LINE		4
#define		PAGE			10
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		GR_LCD_RST(d)  	P1.1 = d
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		GR_LCD_CNT1		0xC000
#define		GR_LCD_DATA1	0xC001
#define		GR_LCD_STA1		0xC002
#define		GR_LCD_RD1		0xC003

#define		GR_LCD_CNT0		0xC004
#define		GR_LCD_DATA0	0xC005
#define		GR_LCD_STA0		0xC006
#define		GR_LCD_RD0		0xC007
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		GR_LCD_LINE		64
#define		GR_LCD_X        64 / 8	// bits --> byte
#define		GR_LCD_Y        128
#define		GR_LCD_PAGE		10
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		enable(a)		a = ON
#define		disable(b)      b = OFF
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct	conv__t {
	byte		b1;
	byte		b2;
}	conv_t;
typedef union conv__u	{
	conv_t		byte_data;
	word		word_data;
}	conv_u;
static	conv_u	conv;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct key__t	{
	byte		value	:4;
	byte		n_used	:3;
	byte		flag	:1;
}	key_t;

typedef union key__u	{
	key_t		bits;
	byte		byte_data;
}	key_u;

static	key_u	key;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct lcd_buffer__t	{
	byte		x;
	byte		y;
    byte        page;
    byte        new_page;
	byte		flag;
	byte		disp[LCD_COLUM][LCD_LINE][PAGE];
}	lcd_buffer_t;

static	lcd_buffer_t	lcd_buffer;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct gr_lcd_buffer__t	{
	byte		x;
	byte		y;
	byte		d_page;
	byte		new_page;
	byte		disp[GR_LCD_X][GR_LCD_Y][GR_LCD_PAGE];
}	gr_lcd_buffer_t;

static	gr_lcd_buffer_t		gr_lcd_buffer;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	key_wait = OFF;
static	word	key_wait_count = 0;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//   Function DEFINE
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x0B] 	void T0_int   (void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					void graphic_lcd_onoff(byte cs, byte status);
					byte graphic_lcd_status_read(byte cs);
					void graphic_lcd_reset(void);
					void graphic_lcd_set_disp_line(byte cs, byte line);
					void graphic_lcd_set_page(byte cs, byte pg);
					void graphic_lcd_set_yline(byte yline);
					void graphic_lcd_init(void);
					void graphic_lcd_put_data(byte addr, byte p_data);
					void graphic_lcd_clear(void);
					void graphic_lcd_test(void);
void EngFontPutch(byte ascii);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
			static	void my_put_c_in_string(byte c, void *ptr);
					void lcd_printf(byte x, byte y, byte page, const byte *format, ...);
					void display_buffer(byte x, byte y, byte *str, byte pg);
					void display_buffer_clear(byte pg);
					void delay(word count);
					void lcd_clr(void);
					void cursor_home(void);
					void entry_mode(byte cfg);
					void display_onoff(byte cfg);
					void func_set(byte cfg);
					void cursor_locate(byte x, byte y);
					void lcd_write_byte(byte ch);
					void init_lcd(void);
					void key_check(void);
					void led_lcd_test(void);
					void main(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//		LCD Hardware Direct Control used function.
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//					void lcd_printf(byte x, byte y, byte *str);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
