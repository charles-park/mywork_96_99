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
#define		VOLUME_H		0xC000
#define		VOLUME_L		0xC001
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
#define		LCD_CON			0x9000
#define		LCD_DATA		0x9001
#define		LCD_COLUM		20
#define		LCD_LINE		4
#define		PAGE			10
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		enable(a)		a = ON
#define		disable(b)      b = OFF
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		BUFFER_SIZE		2000
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
typedef struct	key_send__t	{
	unsigned long	header;
	byte			key	  ;
	byte			add   ;
	byte			xor   ;
	unsigned long	tail  ;
}	key_send_t;

typedef union	key_send__u	{
	key_send_t		pc  ;
	byte			bytes[sizeof(key_send_t)];
}	key_send_u;

static	key_send_u	key_send;
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
typedef struct	variable__t	{
	word	p_sp;
	word	p_ep;
	word	size;
	word	flag;
	byte	*buf;
}	variable_t;

typedef struct	protocol__t	{
	variable_t		var;
	byte			(*pcheck)(variable_t *var);
	byte			(*pcatch)(variable_t *var);
}	protocol_t;

typedef struct protocol_grp__t	{
	word		rx_sp, rx_ep;
	word		tx_sp, tx_ep;
	byte		pcnt;
	byte		tx_flag;
	byte		cat_flag;
	byte		tx_buf[BUFFER_SIZE];
	byte		rx_buf[BUFFER_SIZE];
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	com;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	key_wait = OFF;
static	word	key_wait_count = 0;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//   Function DEFINE
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x0B] 	void T0_int   (void);
interrupt [0x23] 	void SCON_int (void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					void init_serial(word buffer_size, byte (*chk)(variable_t *func1),
													   byte (*cat)(variable_t *func2) );
					void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event(protocol_grp_t *comx, byte com_data);
					void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);
					byte rx_chk(variable_t *var);
					byte rx_cat(variable_t *var);
					void disp_catch(byte d_data, word addr);
					void key_send_pc(byte key_data);
					void move_to_serial_buffer(protocol_grp_t *comx, byte s_data);
					void transmit_check(void);
					void receive_check(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
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
