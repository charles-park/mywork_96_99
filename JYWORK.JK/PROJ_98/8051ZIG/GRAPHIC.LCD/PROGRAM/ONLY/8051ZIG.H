//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//    Graphic LCD Header Only
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		watch()			P1.7 = 1 - P1.7
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		ON				1
#define		OFF				0
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
#define		GR_LCD_PAGE		5
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		enable(a)		a = ON
#define		disable(b)      b = OFF
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
static	word	Sunsor = 0;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//   Function DEFINE
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x0B] 	void T0_int   (void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					void gr_lcd_clear(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
			static	void my_put_c_in_string(byte c, void *ptr);
					void gr_lcd_printf(byte x, byte y, byte pg, const byte *format, ...);
					void gr_lcd_buffer_clear(byte pg);
					void gr_lcd_onoff(byte cs, byte status);
					byte gr_lcd_status_read(byte cs);
					void gr_lcd_reset(void);
					void gr_lcd_set_disp_line(byte cs, byte line);
					void gr_lcd_set_page(byte cs, byte pg);
					void gr_lcd_set_yline(byte yline);
					void gr_lcd_put_data(byte addr, byte p_data);
					void gr_lcd_init(void);
					void gr_lcd_test(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					void led_lcd_test(void);
					void delay(word count);
					void init_interrupt(void);
					void main(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//   Graphic Lcd Direct Access
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
