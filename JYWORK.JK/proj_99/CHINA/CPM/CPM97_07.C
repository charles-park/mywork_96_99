#pragma memory = code
unsigned char	NAME[4][80] = {
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
"                      RTU Communication module program                    ",
"              Program version 9706.10   Copyright Jinyang inc..           ",
"                              Date 1997-06-10                             ",
"                          Program by Jinmin - Mok                         "};
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#pragma memory = default
#include <io51.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <icclbutl.h>
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[ BIT MAP ]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static		bit		key_down_Flag;
static		bit		key_up_Flag;
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define	BUZZER(x)	(P1.6 = x)
#define	LCD(x)		(P1.7 = x)

#define KEY_ADD		0xB000						// Keypad address
#define	LCD_ADD		0xC000						// LCD address
#define LED_ADD		0xD000						// LED Indicator address

#define	TX1_ON		0x80
#define	RX1_ON		0x40
#define	TX2_ON		0x20
#define	RX2_ON		0x10
#define	TX3_ON		0x08
#define	RX3_ON		0x04
#define	TX4_ON		0x02
#define	RX4_ON		0x01
#define	TX1_OFF		0x7F
#define	RX1_OFF		0xBF
#define	TX2_OFF		0xDF
#define	RX2_OFF		0xEF
#define	TX3_OFF		0xF7
#define	RX3_OFF		0xFB
#define	TX4_OFF		0xFD
#define	RX4_OFF		0xFE

#define	MODE		0x0A
#define	ENTER		0x0B
#define	F1			0x0C
#define	F2			0x0D
#define	UP_ARROW	0x0E
#define	DN_ARROW	0x0F

#define	MAIN  		1
#define	SUB   		0

#define	COM1   		1
#define	COM2   		2
#define	COM3   		3
#define	COM4   		4

#define	TX			0
#define	RX			1

#define	NUL			0
#define	ON			1
#define	OFF			0
#define	reon		0
#define	reoff		1

#define	SUPER		7777

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static	Byte	SENSOR_CODE[100];
static	Byte	ASCII_BUF  [ 20];
static	Byte	KEY_BUF	   [ 10];

static	Word	Key_pos_ed	= 0;
static	Word	Key_pos_st	= 0;
static	Byte	Key_del		= 0;
static	Byte	Start_up	= 1;
static	Byte	Run_up		= 1;
static	Byte	Start_key	= 1;
static	Byte	Gubun_cd;
static	Byte	Gubun_id;
static	Byte	Sensor_cd;
static	Byte	data_form;
static	Byte	Nul_1		= 0;
static	Byte	Nul_2		= 0;
static	Byte	Sensor_suu  = 0;
static	Byte	Ld_dt		= 0;
static	Byte	Cursor_sta	= 0x5F;
static	Byte	Cursor_pos	= 0x00;
static	Byte	Lcd_pos		= 0x00;
static	Byte	Buf_pos		= 0x00;
static	Byte	Sw_flag		= 1;
static	float	Value		= 0.0;

static	Word	Led_size	= 0x0000;
static	Word	Delay_cnt	= 0x0000;
static	Word	Cursor_cnt	= 0x0000;
static	Word	Station_id;
static	Word	Password;

static	  signed int	Buzzer_cnt	= 0x0000;
static	unsigned long	Lcd_cnt		= 0x0000;

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct	menuitem__t	{
	unsigned char					  menustr[17];
	void 			(*runfunc)(unsigned char pos);
}	menuitem_t;
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct 	menu__t	{
	unsigned char		menuflag;
	unsigned char		menuquit;
	unsigned char		itemcnt;
	  signed char		scr_line;
	  signed char		scr_spos;
	  signed char		scr_cpos;
	menuitem_t			*item;
}	menu_t;
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static menu_t			main_menu;
static menu_t			real_menu;
static menu_t			status_menu;
static menu_t			module_menu;
static menu_t			sub_menu[5];

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef	struct	sensor_table__t		{
	unsigned char		sencode;
	unsigned char		name[10];
}	sensor_table_t;

static sensor_table_t sensor_table[] =	{
	0x00,"W/DIR   ",
	0x01,"W/SPEED ",
	0x02,"TEMPERA ",
	0x03,"PRECIPI ",
	0x04,"PREC/DT ",
	0x05,"PRESS   ",
	0x06,"R/HUMID ",
	0x07,"DEW/TMP ",
	0x08,"SUN/RAD ",
	0x09,"SUN/DET ",
	0x0A,"EVAPORA ",
	0x0B,"GND/HUM ",
	0x0C,"SNOW    ",
	0x0D,"DEWDROP ",
	0x0E,"FOG     ",
	0x0F,"CLOUD   ",
	0x10,"VISIBIL ",
	0x11,"BK/LUMI ",
	0x12,"SIGMA   ",
	0x13,"???     ",
	0x14,"???     ",
	0x15,"PH      ",
	0x16,"CONDUCT ",
	0xFF,"",
};

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef	struct	module_table__t		{
	unsigned char		modulecode;
	unsigned char		name[10];
}	module_table_t;

static module_table_t module_table[] =	{
	0x00,"MAIN  BD",
	0x01,"MEMOR BD",
	0x02,"COMMU BD",
	0x03,"REMOT BD",
	0xFF,"",
};

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct 	key_data__t	{
	unsigned long		header;
	unsigned char		cmd;
	unsigned char		gubun;
	unsigned char		gubun_id;
	unsigned char		yy_hh;
	unsigned char		mm_mm;
	unsigned char		dd_ss;
			 float		set_d;
	unsigned char		chk_xor;
	unsigned long		tail;
}	key_data_t;
typedef union key_data__u	{
	key_data_t	send;
	Byte		bytes[sizeof(key_data_t)];
}	key_data_u;
static key_data_u kd;

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
unsigned char key_check(void);
void led_con(Byte ch, Byte sel, Byte sw);
void main_initial(void);
char *get_sensor_name(unsigned char sencode);
char *get_module_name(unsigned char modelecode);

void init_menu(menu_t *menu, unsigned char menuflag, unsigned char itemcnt,unsigned char line,unsigned char spos,unsigned char cpos);
void init_menu_item(menu_t *menu,int pos,unsigned char *str,void (*runfunc)(unsigned char pos));
void menu_display(menu_t *menu);
void main_loop(menu_t *menu);
void sub_menu_control(menu_t *menu);
void main_menu_func(unsigned char pos);
void sub_menu_func(unsigned char pos);
Byte get_sensor_code(unsigned char i);

void real_display_func(unsigned char pos);
char mk_cursor(Byte xpos, Byte ypos, Byte kbuf_pos, Byte gubun);
unsigned int  h_to_a(char *buf,int sp,int len);
Byte password_check(void);
void date_set(unsigned char pos);
void time_set(unsigned char pos);
void id_set(unsigned char pos);
void password_set(unsigned char pos);
void offset_set(unsigned char pos);
void clear_buf(unsigned char pos);
void sensor_status_display_func(unsigned char pos);
void module_status_display_func(unsigned char pos);
void memory_test(unsigned char pos);
void volt_disp(unsigned char pos);

void delay(Word del);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#include <cpm_lcd.hpp>
#include <init&int.hpp>
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void led_con( Byte ch, Byte sel, Byte sw)
{
	Byte on, off;

	if(sel == 0)	{
		if     (ch == 1) { on = TX1_ON; off = TX1_OFF; }
		else if(ch == 2) { on = TX2_ON; off = TX2_OFF; }
		else if(ch == 3) { on = TX3_ON; off = TX3_OFF; }
		else if(ch == 4) { on = TX4_ON; off = TX4_OFF; }
	}	else	{
		if     (ch == 1) { on = RX1_ON; off = RX1_OFF; }
		else if(ch == 2) { on = RX2_ON; off = RX2_OFF; }
		else if(ch == 3) { on = RX3_ON; off = RX3_OFF; }
		else if(ch == 4) { on = RX4_ON; off = RX4_OFF; }
	}
	Ld_dt = (sw ? (Ld_dt | on) : (Ld_dt & off));
	output(LED_ADD,Ld_dt);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char *get_sensor_name(unsigned char sencode)
{
	Byte i = 0;

	while(1)	{
		if(sensor_table[i].sencode ==    0xFF) return "";
		if(sensor_table[i].sencode == sencode) return sensor_table[i].name;
		i++;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char *get_module_name(unsigned char modulecode)
{
	Byte i = 0;

	while(1)	{
		if(module_table[i].modulecode ==       0xFF) return "";
		if(module_table[i].modulecode == modulecode) return module_table[i].name;
		i++;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
unsigned char get_sensor_code(unsigned char id)
{
	switch(SENSOR_CODE[id])	{
		case  0	: return 0x00;
		case  1	: return 0x01;
		case  2	: return 0x02;
		case  3	: return 0x03;
		case  4	: return 0x04;
		case  5	: return 0x05;
		case  6	: return 0x06;
		case  7	: return 0x07;
		case  8	: return 0x08;
		case  9	: return 0x09;
		case 10	: return 0x0A;
		case 11	: return 0x0B;
		case 12	: return 0x0C;
		case 13	: return 0x0D;
		case 14	: return 0x0E;
		case 15	: return 0x0F;
		case 16	: return 0x10;
		case 17	: return 0x11;
		case 18	: return 0x12;
		case 19	: return 0x13;
		case 20	: return 0x14;
		case 21	: return 0x15;
		case 22	: return 0x16;
		case 23	: return 0x17;
		case 24	: return 0x18;
		case 25	: return 0x19;
		case 26	: return 0x1A;
		case 27	: return 0x1B;
		case 28	: return 0x1C;
		case 29	: return 0x1D;
		case 30	: return 0x1E;
	}
	return 0xFF;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void menu_initliaze(void)
{
	Byte pos;

	init_menu	  (&main_menu, MAIN,3,1,0,0);
	init_menu_item(&main_menu,  0," REAL DISPLAY   ",main_menu_func);
	init_menu_item(&main_menu,  1," RTU STATUS     ",main_menu_func);
	init_menu_item(&main_menu,  2," RTU SETUP      ",main_menu_func);

	init_menu	  (&sub_menu[0],SUB,4,1,0,0);
	init_menu_item(&sub_menu[0],0," SENSOR STATUS  ",sub_menu_func);
	init_menu_item(&sub_menu[0],1," MODULE STATUS  ",sub_menu_func);
	init_menu_item(&sub_menu[0],2," VOLTAGE STATUS ",sub_menu_func);
	init_menu_item(&sub_menu[0],3," MEMORY TEST    ",memory_test);

	init_menu	  (&sub_menu[1],SUB,6,1,0,0);
	init_menu_item(&sub_menu[1],0," DATE SET       ",date_set);
	init_menu_item(&sub_menu[1],1," TIME SET       ",time_set);
	init_menu_item(&sub_menu[1],2," ID SET         ",id_set);
	init_menu_item(&sub_menu[1],3," PASSWORD SET   ",password_set);
	init_menu_item(&sub_menu[1],4," OFFSET SET     ",offset_set);
	init_menu_item(&sub_menu[1],5," BUFFER CLEAR   ",clear_buf);

	init_menu	  (&sub_menu[2],SUB,5,1,0,0);
	init_menu_item(&sub_menu[2],0," +5V            ",volt_disp);
	init_menu_item(&sub_menu[2],1," +12V           ",volt_disp);
	init_menu_item(&sub_menu[2],2," -12V           ",volt_disp);
	init_menu_item(&sub_menu[2],3," AC/V           ",volt_disp);
	init_menu_item(&sub_menu[2],4," BATTERY        ",volt_disp);

	init_menu	  (&real_menu,SUB,Sensor_suu,1,0,0);
	for(pos=0; pos<Sensor_suu; pos++)	{
		init_menu_item(&real_menu,pos,get_sensor_name(get_sensor_code(pos)),
						real_display_func);
	}
	init_menu	  (&status_menu,SUB,Sensor_suu,1,0,0);
	for(pos=0; pos<Sensor_suu; pos++)	{
		init_menu_item(&status_menu,pos,get_sensor_name(get_sensor_code(pos)),
						sensor_status_display_func);
	}
	init_menu	  (&module_menu,SUB,Sensor_suu+4,1,0,0);
	for(pos=0; pos<Sensor_suu+4; pos++)	{
		if(pos < 4)	init_menu_item(&module_menu,pos,get_module_name(pos),
							module_status_display_func);
		else		init_menu_item(&module_menu,pos,get_sensor_name(get_sensor_code(pos-4)),
							module_status_display_func);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main_menu_func(unsigned char pos)
{
	lcd_clear();
	if     (pos == 0)	sub_menu_control(&real_menu  );
	else if(pos == 1)	sub_menu_control(&sub_menu[0]);
	else				sub_menu_control(&sub_menu[1]);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sub_menu_func(unsigned char pos)
{
	if(pos == 0)	sub_menu_control(&status_menu);
	if(pos == 1)	sub_menu_control(&module_menu);
	if(pos == 2)	sub_menu_control(&sub_menu[2]);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_menu(menu_t *menu, unsigned char menuflag,
							 unsigned char itemcnt,
							 unsigned char line,
							 unsigned char spos,
							 unsigned char cpos)
{
	menu->scr_spos =     spos;
	menu->scr_cpos =	 cpos;
	menu->scr_line =	 line;
	menu->menuquit =        0;
	menu->itemcnt  =  itemcnt;
	menu->menuflag = menuflag;
	menu->item     = (menuitem_t *)(malloc(sizeof(menuitem_t)*menu->itemcnt));
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_menu_item(menu_t *menu,int pos,unsigned char *str,void (*runfunc)(unsigned char pos))
{
	menu->item[pos].runfunc  =  runfunc;
	strcpy(menu->item[pos].menustr,str);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void menu_display(menu_t *menu)
{
	Byte i;

	for(i=menu->scr_spos; i<(menu->scr_spos+menu->scr_line); i++)
		lcd_printf(0,0,1,menu->item[i].menustr);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main_loop(menu_t *menu)
{
	signed char Old_spos = -1;
	signed char Old_cpos =  0;
	menu->menuquit		 =  0;

	while(!menu->menuquit)	{
		if(Old_spos != menu->scr_spos)	{
			menu_display(menu);
			Old_spos = menu->scr_spos;
		}
		if(Old_cpos != menu->scr_cpos)	Old_cpos = menu->scr_cpos;
		switch(key_check())	{
			case UP_ARROW :	menu->scr_cpos--;
							if(menu->scr_cpos < 0)	{
								menu->scr_cpos = 0;
								if(menu->scr_spos > 0)	menu->scr_spos--;
							} break;
			case DN_ARROW :	menu->scr_cpos++;
							if(menu->scr_cpos >= menu->itemcnt + 1)
								menu->scr_cpos = menu->itemcnt;
							if(menu->scr_cpos > 0)	{
								menu->scr_cpos = 0;
								if(menu->scr_spos < (menu->itemcnt - 1))
									menu->scr_spos++;
							} break;
			case ENTER    :
							menu->item[menu->scr_spos + menu->scr_cpos].runfunc(menu->scr_spos + menu->scr_cpos);
							Old_spos = -1;
							break;
			case MODE	  :	if(menu->menuflag == 0)	menu->menuquit = 1;
							break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#pragma function = recursive
void sub_menu_control(menu_t *menu)
{
	signed char Old_spos = -1;
	signed char Old_cpos =  0;
	menu->menuquit		 =  0;

	while(!menu->menuquit)	{
		if(Old_spos != menu->scr_spos)	{
			menu_display(menu);
			Old_spos = menu->scr_spos;
		}
		if(Old_cpos != menu->scr_cpos)	Old_cpos = menu->scr_cpos;
		switch(key_check())	{
			case UP_ARROW :	menu->scr_cpos--;
							if(menu->scr_cpos < 0)	{
								menu->scr_cpos = 0;
								if(menu->scr_spos > 0)	menu->scr_spos--;
							} break;
			case DN_ARROW :	menu->scr_cpos++;
							if(menu->scr_cpos >= menu->itemcnt)
								menu->scr_cpos = menu->itemcnt - 1;
							if(menu->scr_cpos > 0)	{
								menu->scr_cpos = 0;
								if(menu->scr_spos < (menu->itemcnt-1))
									menu->scr_spos++;
							} break;
			case ENTER    :	menu->item[menu->scr_spos + menu->scr_cpos].runfunc(menu->scr_spos + menu->scr_cpos);
							Old_spos = -1;
							break;
			case MODE     :	if(menu->menuflag == 0)	menu->menuquit = 1;
							break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void real_display_func(unsigned char pos)
{
	Delay_cnt = 0;
	Lcd_cnt   = 0;
	lcd_printf(0,0,1,get_sensor_name(get_sensor_code(pos)));
	key_send(&Com2,'C',0,pos);					// 0 = real display code, pos = sensor id
	if(Sensor_cd == 0x02)	Value = 1000.;
	else					Value =    0.;
	while(1)	{
		switch(Sensor_cd)	{
			case 0x02	:	lcd_printf(9,0,0,"%6.1f",
							(float)(Value / 10.0) - 100.);
							break;
			case 0x04	:	if(Value)	lcd_printf(9,0,0," ON     ");
							else		lcd_printf(9,0,0," OFF    ");
							break;
			default		:	lcd_printf(9,0,0,"%6.1f",(float)(Value / 10.));
							break;
		}
		if(Delay_cnt >= 200)	{
			key_send(&Com2,'C',0,pos);			// 0 = real display code, pos = sensor id
			Delay_cnt = 0;
		}
		switch(key_check())	{
			case MODE	  :	lcd_printf(9,0,0,"        ");
							return;
			case UP_ARROW :	KEY_BUF[Key_pos_ed++] = UP_ARROW;	// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							KEY_BUF[Key_pos_ed++] = ENTER;		// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							return;
			case DN_ARROW :	KEY_BUF[Key_pos_ed++] = DN_ARROW;	// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							KEY_BUF[Key_pos_ed++] = ENTER;		// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							return;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sensor_status_display_func(unsigned char pos)
{
	Byte i;

	Delay_cnt = 0;
	Lcd_cnt   = 0;
	for(i=0; i>16; i++)	ASCII_BUF[i] = NUL;
	lcd_printf(0,0,1,get_sensor_name(get_sensor_code(pos)));
	key_send(&Com2,'C',1,pos);					// 1 = sensor status code, pos = sensor id
	while(1)	{
		ASCII_BUF[8] = NUL;
		lcd_printf(9,0,0,"%s",ASCII_BUF);
		if(Delay_cnt >= 200)	{
			key_send(&Com2,'C',1,pos);			// 1 = sensor status code, pos = sensor id
			Delay_cnt = 0;
		}
		switch(key_check())	{
			case MODE	  :	lcd_printf(9,0,0,"        ");
							return;
			case UP_ARROW :	KEY_BUF[Key_pos_ed++] = UP_ARROW;	// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							KEY_BUF[Key_pos_ed++] = ENTER;		// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							return;
			case DN_ARROW :	KEY_BUF[Key_pos_ed++] = DN_ARROW;	// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							KEY_BUF[Key_pos_ed++] = ENTER;		// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							return;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void module_status_display_func(unsigned char pos)
{
	Byte i;

	Delay_cnt = 0;
	Lcd_cnt   = 0;
	for(i=0; i>16; i++)	ASCII_BUF[i] = NUL;
	key_send(&Com2,'C',2,pos);					// 2 = module status code, pos = sensor id
	while(1)	{
		ASCII_BUF[8] = NUL;
		lcd_printf(9,0,0,"%s",ASCII_BUF);
		if(Delay_cnt >= 200)	{
			key_send(&Com2,'C',2,pos);			// 2 = module status code, pos = sensor id
			Delay_cnt = 0;
		}
		switch(key_check())	{
			case MODE	  :	lcd_printf(9,0,0,"        ");
							return;
			case UP_ARROW :	KEY_BUF[Key_pos_ed++] = UP_ARROW;	// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							KEY_BUF[Key_pos_ed++] = ENTER;		// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							return;
			case DN_ARROW :	KEY_BUF[Key_pos_ed++] = DN_ARROW;	// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							KEY_BUF[Key_pos_ed++] = ENTER;		// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							return;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void volt_disp(unsigned char pos)
{
	Value     = 0.0;
	Delay_cnt = 0;
	Lcd_cnt   = 0;
	key_send(&Com2,'C',3,pos);					// 3 = volt status code, pos = sensor id
	while(1)	{
		if(pos == 2)	lcd_printf(9,0,0,"%6.1f",(float)(Value / 10.0) * -1.);
		else			lcd_printf(9,0,0,"%6.1f",(float)(Value / 10.0));
		if(Delay_cnt >= 200)	{
			key_send(&Com2,'C',3,pos);			// 3 = volt status code, pos = sensor id
			Delay_cnt = 0;
		}
		switch(key_check())	{
			case MODE	  :	lcd_printf(9,0,0,"        ");
							return;
			case UP_ARROW :	KEY_BUF[Key_pos_ed++] = UP_ARROW;	// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							KEY_BUF[Key_pos_ed++] = ENTER;		// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							return;
			case DN_ARROW :	KEY_BUF[Key_pos_ed++] = DN_ARROW;	// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							KEY_BUF[Key_pos_ed++] = ENTER;		// Keypad data store
							if(Key_pos_ed >= 10)	Key_pos_ed = 0;
							return;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char mk_cursor(Byte xpos, Byte ypos, Byte kbuf_pos, Byte gubun)
{
	if(Cursor_cnt > 80)	{
		if(Cursor_sta == 0x5F)	Cursor_sta = ' ';
		else	Cursor_sta = 0x5F;
		Cursor_cnt = 0;
	}
	switch(gubun)	{
		case 'd':	if((kbuf_pos == 2) || (kbuf_pos == 5))	{
						lcd_printf(xpos+kbuf_pos,ypos,0,"//");
						kbuf_pos++;
					} else	if(kbuf_pos > 8)	{
						kbuf_pos  = 0;
						Start_key = 1;
						lcd_printf(xpos+kbuf_pos,ypos,0,"         ");
					}	break;
		case 't':	if((kbuf_pos == 2) || (kbuf_pos == 5))	{
						lcd_printf(xpos+kbuf_pos,ypos,0,":");
						kbuf_pos++;
					} else	if(kbuf_pos > 8)	{
						kbuf_pos  = 0;
						Start_key = 1;
						lcd_printf(xpos+kbuf_pos,ypos,0,"         ");
					}	break;
		case 'p':	if(kbuf_pos > 4)	{
						kbuf_pos  = 0;
						lcd_printf(xpos+kbuf_pos,ypos,0,"**** ");
					}	break;
		default :	if(kbuf_pos > 4)	{
						kbuf_pos  = 0;
						lcd_printf(xpos+kbuf_pos,ypos,0,"     ");
					}	break;
	}
	lcd_printf(xpos+kbuf_pos,ypos,0,"%c",Cursor_sta);
	return(kbuf_pos);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
unsigned int  h_to_a(char *buf,int sp,int len)
{
	Byte  BUF[10];
	int i,cnt=0;

	for(i=sp; i<sp+len; i++) {
			BUF[cnt++] = buf[i];
	}
	BUF[cnt] = 0;
	return(atoi(BUF));
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
Byte password_check(void)
{
	Byte i, k, x, y, kbuf_pos = 0;
	Word pass;
	Byte K_BUF[10];

	Delay_cnt = 0;
	Lcd_cnt   = 0;
	key_send(&Com2,'C',5,3);					// 5 = set code, 3 = password
	i = 0;	x = 11;	y = 0;
	lcd_printf(0,0,1,"PASSWORD : **** ");
	while(1)	{
		kbuf_pos = mk_cursor(x,y,kbuf_pos,'p');
		k = key_check();
		if(k == MODE)	i = 4;
		if( i >= 3 )										return(0);
		if(k == ENTER)	{
			if(kbuf_pos >= 4)	{
				lcd_printf(x,y,0,"**** ");
				pass = h_to_a(K_BUF,0,4);
				if((Password == pass) || (SUPER == pass))	return(1);
				else	{
					kbuf_pos = 0;	i++;
				}
			}
		} else	if(k < 10)	{
			lcd_printf(x+kbuf_pos,y,0,"*");
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
		if(Delay_cnt >= 200)	{
			key_send(&Com2,'C',5,3);			// 5 = set code, 3 = password
			Delay_cnt = 0;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void date_set(unsigned char pos)
{
	Byte k, x, y, kbuf_pos = 0;
	Byte K_BUF[10];

	if(!password_check())   return;
	Delay_cnt = 0;
	Lcd_cnt   = 0;
	Start_key = 1;
	x = 7;	y = 0;
	key_send(&Com2,'C',5,pos);					// 5 = set code, pos(0) = date
	kd.send.yy_hh = kd.send.mm_mm = kd.send.dd_ss = 0;
	lcd_printf(0,0,1,"DATE : %02d/%02d/%02d",kd.send.yy_hh,kd.send.mm_mm,kd.send.dd_ss);
	while(1)	{
		if(!Start_key)	kbuf_pos = mk_cursor(x,y,kbuf_pos,'d');
		k = key_check();
		if(k == MODE)	break;
		if(k == ENTER)	{
			if(kbuf_pos >= 8)	{
				kd.send.yy_hh = h_to_a(K_BUF,0,2);
				kd.send.mm_mm = h_to_a(K_BUF,3,2);
				kd.send.dd_ss = h_to_a(K_BUF,6,2);
				key_send(&Com2,'S',5,pos);		// 5 = set code, pos(0) = date
				Start_key = 1;
			}
		} else	if(k < 10)	{
			Start_key = 0;
			lcd_printf(x+kbuf_pos,y,0,"%c",k + 0x30);
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
		if(Delay_cnt >= 200 && Start_key == 1)	{
			key_send(&Com2,'C',5,pos);			// 5 = set code, pos(0) = date
			lcd_printf(0,0,1,"DATE : %02d/%02d/%02d",kd.send.yy_hh,kd.send.mm_mm,kd.send.dd_ss);
			Delay_cnt = 0;
		}
		if(k == MODE)	{
			lcd_printf(9,0,0,"        ");
			break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void time_set(unsigned char pos)
{
	Byte k, x, y, kbuf_pos = 0;
	Byte K_BUF[10];

	if(!password_check())   return;
	Delay_cnt = 0;
	Lcd_cnt   = 0;
	Start_key = 1;
	x = 7;	y = 0;
	key_send(&Com2,'C',5,pos);					// 5 = set code, pos(1) = time
	kd.send.yy_hh = kd.send.mm_mm = kd.send.dd_ss = 0;
	lcd_printf(0,0,1,"TIME : %02d:%02d:%02d",kd.send.yy_hh,kd.send.mm_mm,kd.send.dd_ss);
	while(1)	{
		if(!Start_key)	kbuf_pos = mk_cursor(x,y,kbuf_pos,'t');
		k = key_check();
		if(k == MODE)	break;
		if(k == ENTER)	{
			if(kbuf_pos >= 8)	{
				kd.send.yy_hh = h_to_a(K_BUF,0,2);
				kd.send.mm_mm = h_to_a(K_BUF,3,2);
				kd.send.dd_ss = h_to_a(K_BUF,6,2);
				key_send(&Com2,'S',5,pos);		// 5 = set code, pos(1) = time
				Start_key = 1;
			}
		} else	if(k < 10)	{
			Start_key = 0;
			lcd_printf(x+kbuf_pos,y,0,"%c",k + 0x30);
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
		if(Delay_cnt >= 200 && Start_key == 1)	{
			key_send(&Com2,'C',5,pos);			// 5 = set code, pos(1) = time
			lcd_printf(0,0,1,"TIME : %02d:%02d:%02d",kd.send.yy_hh,kd.send.mm_mm,kd.send.dd_ss);
			Delay_cnt = 0;
		}
		if(k == MODE)	{
			lcd_printf(9,0,0,"        ");
			break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void id_set(unsigned char pos)
{
	Byte k, x, y, kbuf_pos = 0;
	Byte K_BUF[10];

	if(!password_check())   return;
	Delay_cnt = 0;
	Lcd_cnt   = 0;
	Start_key = 1;
	x = 11;	y = 0;
	key_send(&Com2,'C',5,pos);					// 5 = set code, pos(2) = station id
	lcd_printf(0,0,1,"STATION ID:%04d",Station_id);
	while(1)	{
		if(!Start_key)	kbuf_pos = mk_cursor(x,y,kbuf_pos,'i');
		k = key_check();
		if(k == MODE)	break;
		if(k == ENTER)	{
			if(kbuf_pos >= 4)	{
				kd.send.set_d = (float)(h_to_a(K_BUF,0,4));
				key_send(&Com2,'S',5,pos);		// 5 = set code, pos(2) = station id
				Start_key = 1;
			}
		} else	if(k < 10)	{
			Start_key = 0;
			lcd_printf(x+kbuf_pos,y,0,"%c",k + 0x30);
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
		if(Delay_cnt >= 200 && Start_key == 1)	{
			key_send(&Com2,'C',5,pos);			// 5 = set code, pos(2) = station id
			lcd_printf(0,0,1,"STATION ID:%04d",Station_id);
			Delay_cnt = 0;
		}
		if(k == MODE)	{
			lcd_printf(9,0,0,"        ");
			break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void password_set(unsigned char pos)
{
	Byte i, k, x, y, kbuf_pos = 0;
	Byte togul = 0;
	Word pass;
	Byte K_BUF[10];

	x = 11;	y = 0;
	key_send(&Com2,'C',5,pos);					// 5 = setup, pos(3) = passwprd
	lcd_printf(0,0,1,"OLD PASS :");
	while(1)	{
		lcd_printf(x,y,0,"**** ");
		kbuf_pos = mk_cursor(x,y,kbuf_pos,'p');
		k = key_check();
		if(k == MODE)	{
			KEY_BUF[Key_pos_ed++] = MODE;		// Keypad data store
			if(Key_pos_ed >= 10)	Key_pos_ed = 0;
			break;
		}
		if(k == ENTER && togul == 0)	{
			if(kbuf_pos >= 4)	{
				pass = h_to_a(K_BUF,0,4);
				if((Password == pass) || (SUPER == pass))	{
					lcd_printf(x,y,0,"**** ");
					k = 0xFF;	y = 0;	togul = 1;
					kbuf_pos = 0;
					lcd_printf(0,0,1,"NEW PASS :");
				} else	kbuf_pos = 10;
			}
		}
		if(k == ENTER && togul == 1)	{
			if(kbuf_pos >= 4)	{
				kd.send.set_d = (float)(h_to_a(K_BUF,0,4));
				key_send(&Com2,'S',5,pos);		// 4 = set code, pos(3) = password
				KEY_BUF[Key_pos_ed++] = MODE;	// Keypad data store
				if(Key_pos_ed >= 10)	Key_pos_ed = 0;
				break;
			}
		} else	if(k < 10)	{
			lcd_printf(x+kbuf_pos,y,0,"*");
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
		if(k == MODE)	{
			lcd_printf(9,0,0,"        ");
			KEY_BUF[Key_pos_ed++] = MODE;		// Keypad data store
			if(Key_pos_ed >= 10)	Key_pos_ed = 0;
			break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void offset_set(unsigned char pos)
{
	if(!password_check())   return;
	key_send(&Com2,'S',5,pos);					// 4 = setup, pos(5) = offset set
	lcd_printf(0,0,1,"PRESS ANY KEY ..");
	while(1)	{
		if(key_check() == MODE)	{
			KEY_BUF[Key_pos_ed++] = MODE;		// Keypad data store
			if(Key_pos_ed >= 10)	Key_pos_ed = 0;
			break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void clear_buf(unsigned char pos)
{
	if(!password_check())   return;
	key_send(&Com2,'S',5,pos);					// 5 = setup, pos(5) = buffer clear
	lcd_printf(0,0,1,"BUFFER CLEAR ...");
	while(1)		key_check();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void memory_test(unsigned char pos)
{
	Run_up = 1;
	key_send(&Com2,'C',4,0);					// 4 = memory test, 0 = nul
	while(Run_up)	{
		if(key_check() == MODE)	{
			lcd_printf(0,0,1,"MEM TEST CANCEL ");
			key_send(&Com2,'X',4,0);			// 4 = memory test, X = Cancel
			Run_up = 0;
		}
	}
	while(1)	{
		if(key_check() == MODE)	{
			lcd_printf(9,0,0,"        ");
			KEY_BUF[Key_pos_ed++] = MODE;		// Keypad data store
			if(Key_pos_ed >= 10)	Key_pos_ed = 0;
			break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
unsigned char key_check()
{
	Byte key_value, i;

	receive_check();	transmit_check();
	receive_check();	transmit_check();
	receive_check();	transmit_check();
	receive_check();	transmit_check();
	receive_check();	transmit_check();
	receive_check();	transmit_check();
	receive_check();	transmit_check();
	receive_check();	transmit_check();
	receive_check();	transmit_check();
	receive_check();	transmit_check();
	if(Sw_flag)	{
		if(Lcd_cnt >= 500000)	{
			for(i=0; i<3; i++)	{
				KEY_BUF[Key_pos_ed++] = MODE;	// Keypad data store
				if(Key_pos_ed >= 10)	Key_pos_ed = 0;
			}
			Sw_flag = 0;
		}
	}
	key_value = 0xFF;
	if(Key_pos_ed != Key_pos_st)	{
		key_value = KEY_BUF[Key_pos_st++];		// Keypad data load
		if(Key_pos_st >= 10)	Key_pos_st = 0;
		Sw_flag = 1;	Lcd_cnt = 0;
	}
	return(key_value);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_data(protocol_grp_t *Comx, Byte sdata)
{
	Comx->tx_buf[Comx->tx_ep++] = sdata;
	if(Comx->tx_ep >= BUF_SIZE)	Comx->tx_ep = 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void key_send(protocol_grp_t *Comx, Byte comd, Byte gb, Byte gb_id)
{
	Byte i;
	kd.send.header		= 0xFFFFFFFF;
	kd.send.cmd			= comd;
	kd.send.gubun		= gb;
	kd.send.gubun_id	= gb_id;
	kd.send.chk_xor		= 0x00;
	kd.send.tail		= 0xFEFEFEFE;
	for(i=0; i<sizeof(key_data_u); i++)	{
		if(i>=4 && i<sizeof(key_data_u) - 5)	{
			kd.send.chk_xor ^= kd.bytes[i];
		}
		send_data(Comx,kd.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void delay(Word del)
{
	Delay_cnt = 0;
	while(1)	if(Delay_cnt > del)	break;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main(void)
{
	BUZZER(reoff);
	output(LED_ADD,0x00);						// All led off
	initiliaze();								// Initiliaze interrupt vector
	led_con(COM1,TX,ON );	led_con(COM2,TX,ON );
	led_con(COM3,TX,ON );	led_con(COM4,TX,ON );
	LCD(reon);				lcd_init();			// Initiliaze lcd (16 * 1)
	lcd_printf(0,0,1," BAUD RATE 2400 ");
	led_con(COM1,TX,OFF);	led_con(COM2,TX,OFF);
	led_con(COM3,TX,OFF);	led_con(COM4,TX,OFF);	delay(50);
	led_con(COM1,RX,ON );	led_con(COM2,RX,ON );
	led_con(COM3,RX,ON );	led_con(COM4,RX,ON );	delay(50);
	led_con(COM1,RX,OFF);	led_con(COM2,RX,OFF);
	led_con(COM3,RX,OFF);	led_con(COM4,RX,OFF);
	Delay_cnt = 0;			Start_up   = 1;
	while(Start_up)	{
		receive_check ();
		transmit_check();
		if(Delay_cnt > 2000)	{
			key_send(&Com2,'C',6,0);			// 6 = Config data call, 0 = Nul
			Delay_cnt = 0;
		}
	}
	menu_initliaze();
	main_loop(&main_menu);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
