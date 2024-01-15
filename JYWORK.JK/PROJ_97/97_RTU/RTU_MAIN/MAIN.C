#pragma memory = code
unsigned char	NAME[7][77] = {
"[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]",
"[*]              97' AWS and AMOS RTU(Remote Terminal Unit)             [*]",
"[*]                            main program                             [*]",
"[*]          Program version 9708.12   Copyright Jinyang inc..          [*]",
"[*]                          Date 1997-08-12                            [*]",
"[*]                     Program by Jinmin - Mok                         [*]",
"[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]" };
#pragma memory = default
#include <io51.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
//#include <math.h>
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	Use under header with main program .....                          [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define	MPM_CFG_BASE	0x0010				// MPM Configration base address
#define MPM_CFG_CHK1	MPM_CFG_BASE		// Default 0xAA
#define MPM_CFG_CHK2	MPM_CFG_CHK1 +   1	// Default 0xAA
#define SENSOR_SU		MPM_CFG_CHK2 +   1	// Sensor limit
#define DAY_CNT			SENSOR_SU    +   1	// Status data RAM select
#define STATION_ID		DAY_CNT      +   1	// Station id (WORD)
#define PASSWORD		STATION_ID   +   2	// Password   (WORD)
#define CALL_TIME		PASSWORD     +   2	// Call time  (WORD)
#define SAVE_TIME		CALL_TIME    +   2	// Save time  (WORD)
#define	ID_CODE			SAVE_TIME    +   2	// Sensor id & code DB(200 byte)
#define MPM_CFG_XOR		ID_CODE	     + 210	// Checksum xor
#define MPM_CFG_SUM		MPM_CFG_XOR  +   1	// Checksum sum
#define MPM_CFG_END		MPM_CFG_XOR			// Configration end address
#define	SPORT_CFG		MPM_CFG_END	 +  20	// Serial port config(100 byte)
#define INDEX_DB		SPORT_CFG    + 500	// Index DB(400 byte)
#define	MAX_MIN_BASE	INDEX_DB     + 500	// Weather min & max storage DB
#define	SPECIAL_BASE	MAX_MIN_BASE + 500	// Weather special storage DB
#define	OFFSET_BASE		SPECIAL_BASE + 500	// Weather offset storage DB
//[*]---------------------------------------------------------------------[*]
#define	SAM_CFG_BASE	0x0010				// SAM Configration base address
#define SAM_CFG_CHK1	SAM_CFG_BASE		// Default 0xAA
#define SAM_CFG_CHK2	SAM_CFG_CHK1 +   1	// Default 0xAA
//??????????????????
#define SAM_CFG_XOR		SAM_CFG_CHK2 + 200	// Checksum xor
#define SAM_CFG_SUM		SAM_CFG_XOR  +   1	// Checksum sum
#define SAM_CFG_END		SAM_CFG_XOR			// Configration end address
//[*]---------------------------------------------------------------------[*]
#define iRAM_CS()		{ P1 = 0xFF;         }
#define RAM_CS()		{ P1 = 0xFF; EA = 1; }
#define RTC_CS()		{ EA = 0; P1 = 0x02; }
#define SI2_CS()		{ EA = 0; P1 = 0x04; }
#define SI3_CS()		{ EA = 0; P1 = 0x06; }
#define LED_CS()		{ EA = 0; P1 = 0x08; }
#define RST_CS()		{ EA = 0; P1 = 0x0A; }
#define CNT_CSH()		{ EA = 0; P1 = 0x0C; }
#define CNT_CSL()		{ EA = 0; P1 = 0x0E; }
#define LCD_CS()		{ EA = 0; P1 = 0x10; }
#define KEY_CS()		{ EA = 0; P1 = 0x12; }
//[*]---------------------------------------------------------------------[*]
#define	LED_ADD			0x00				// LED Base address
//[*]---------------------------------------------------------------------[*]
#define WATCHDOG		( P1.7 = 1 - P1.7 )
#define	BUZZER_ON		( P3.5 = 1 )
#define	BUZZER_OFF		( P3.5 = 0 )
#define	COUNTER_RESET()	{ P3.4 = 1;	P3.4 = 0; }			// 4040 Reset bit
//[*]---------------------------------------------------------------------[*]
#define CFG_RAM(x)		{ EA = 0; P1 = 0x20 + x;	 }	// MPM_CFG:x=0,SAM_CFG:x=1
#define DAT_RAM(ram_x)	{ EA = 0; P1 = 0x22 + ram_x; }	// Data ram = 0x22 ~ 0x2F
//[*]---------------------------------------------------------------------[*]
#define	MPM_CFG			0
#define	SAM_CFG			1
#define	MAIN  			1
#define	SUB   			0
#define	MPM_ID			0x00
#define	RCM_ID			0x01
//[*]---------------------------------------------------------------------[*]
#define	COM1   			1
#define	COM2   			2
#define	COM3   			3
#define	TX				0
#define	RX				1
//[*]---------------------------------------------------------------------[*]
#define	SUPER			7741
#define	REAL			0
#define	CALL			1
#define	SAVE			2
#define	SEND			3
//[*]---------------------------------------------------------------------[*]
#define disable(y)		y = OFF
#define enable(z)		z = ON
//[*]---------------------------------------------------------------------[*]
#define	ON				1
#define	OFF				0
#define	NORMAL			0
#define	FAIL			1
//[*]---------------------------------------------------------------------[*]
#define	UP_ARROW		0x0A
#define	DN_ARROW		0x0B
#define	ESC1   			0x0C
#define	ESC2  	 		0x0D
#define	ENTER1			0x0E
#define	ENTER2			0x0F
//[*]---------------------------------------------------------------------[*]
// Gubun field
#define	ACK_SEND		0xFF41				// Acknowledge.
#define	SIZE_SEND		0xFF44				// Calling
#define	REQUEST			0xFF4E				// Config data request
//[*]---------------------------------------------------------------------[*]
// Data size field
#define	RESET			0xFF52				// 'R'
#define	ID_SET			0xFF49				// 'I'
#define	PASS_SET		0xFF50				// 'P'
#define	DT_SET			0xFF54				// 'T'
#define	BUFF_CLEAR		0xFF43				// 'C'
#define	CONFIG_SET		0xFF53				// 'S'
#define	BUAD_SET		0xFF42				// 'B'
#define	SOF_SET			0xFF2A				// '*'
#define	UOF_SET			0xFF23				// '#'
#define	SET_FAIL		0xFF58				// 'X'
//[*]---------------------------------------------------------------------[*]
typedef struct led_indicator__t	{
	unsigned char	rx3:1;
	unsigned char	tx3:1;
	unsigned char	rx2:1;
	unsigned char	tx2:1;
	unsigned char	rx1:1;
	unsigned char	tx1:1;
	unsigned char	n_c:2;
}	led_indicator_t;

typedef union led__u	{
	led_indicator_t	bits;
	byte			led_data;
}	led_u;

static	idata led_u	led;

//[*]---------------------------------------------------------------------[*]
typedef struct	menuitem__t	{
	unsigned char	menustr[19];
	void			(*runfunc)(unsigned char pos);
}	menuitem_t;

typedef struct 	menu__t	{
	unsigned char	menuflag;
	unsigned char	menuquit;
	unsigned char	itemcnt;
	  signed char	scr_line;
	  signed char	scr_spos;
	  signed char	scr_cpos;
	menuitem_t		*item;
}	menu_t;

static menu_t	main_menu;
static menu_t	real_menu;
static menu_t	aver_menu;
static menu_t	status_menu;
static menu_t	module_menu;
static menu_t	offset_menu;
static menu_t	sub_menu[5];

//[*]---------------------------------------------------------------------[*]
typedef	struct	sensor_table__t		{
	unsigned char	sencode;
	unsigned char	name[19];
}	sensor_table_t;

static sensor_table_t *menu_sensor_table;

//[*]---------------------------------------------------------------------[*]
typedef	struct	module_table__t		{
	unsigned char	modulecode;
	unsigned char	name[19];
}	module_table_t;

//[*]---------------------------------------------------------------------[*]
typedef struct 	key__t	{
	unsigned char	key_data[21];
	unsigned char	key_pos_st;
	unsigned char	key_pos_ed;
	unsigned char	key_chtr;
	unsigned char	key_cmp;
	unsigned char	key_del;
	unsigned char	key_down_flag;
	unsigned char	key_up_flag;
}	key_t;

static key_t		key;

//[*]---------------------------------------------------------------------[*]
typedef struct	idt__t	{
	unsigned char	rtu_id;
	unsigned char	sensor_su;
	unsigned char	day_cnt;
	unsigned char	ram_x;
	unsigned char	ram_limit;
	unsigned char	day_limit;
	unsigned int	extr_addr;
	unsigned int	password;
	unsigned int	station_id;
	unsigned int	call_time;
	unsigned int	save_time;
}	idt_t;

static idata idt_t	i_data;

//[*]---------------------------------------------------------------------[*]
typedef struct	real__t	{
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	flag;
	unsigned char	again_count;
	unsigned char	call_id;
	unsigned char	receive_status;
	unsigned int	delay_count;
}	real_t;

static real_t		real;

//[*]---------------------------------------------------------------------[*]
typedef struct	call__t	{
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	flag;
	unsigned char	call_min;
	unsigned char	call_sec;
	unsigned char	call_id;
	unsigned char	receive_status;
	unsigned char	again_count;
	unsigned int	delay_count;
}	call_t;

static call_t		call;

//[*]---------------------------------------------------------------------[*]
typedef struct	save__t	{
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	flag;
	unsigned char	save_min;
	unsigned char	save_sec;
	unsigned char	call_id;
	unsigned char	receive_status;
	unsigned char	again_count;
	unsigned int	delay_count;
}	save_t;

static save_t		save;

//[*]---------------------------------------------------------------------[*]
typedef struct	rcm_poll__t	{
	unsigned int	again_count;
	unsigned int	delay_count;
	unsigned int	receive_status;
}	rcm_poll_t;

static rcm_poll_t	rcm_poll;

//[*]---------------------------------------------------------------------[*]
typedef struct	value__t	{
	unsigned char	buzzer_cnt;
	unsigned char	old_year;
	unsigned char	old_month;
	unsigned char	old_day;
	unsigned char	host_year;
	unsigned char	host_month;
	unsigned char	host_day;
	unsigned char	host_hour;
	unsigned char	host_min;
	unsigned char	host_sec;
	unsigned char	factory;
	unsigned char	cfg_poll_flag;
	unsigned char	config_receive_status;
	unsigned int	delay_count;
	unsigned int	record_su;
	unsigned long	day_pos;
	unsigned long	ram_size;
	unsigned long	weather_field;
}	value_t;

static value_t		value;

//[*]---------------------------------------------------------------------[*]
static sensor_table_t sensor_table[] =	{
	0x00,"WIND DIR......[#]",
	0x01,"WIND SPEED....[#]",
    0x02,"TEMPERATURE...[#]",
	0x03,"PRECIPITATION.[#]",
	0x04,"PREC/DETECTE..[#]",
	0x05,"PRESSURE......[#]",
	0x06,"R/HUMIDITY....[#]",
	0x07,"DEW TEMPERATE.[#]",
	0x08,"SUN/RADATION..[#]",
	0x09,"SUN/DETECTE...[#]",
	0x0A,"EVAPORATION...[#]",
	0x0B,"GND/HUMIDITY..[#]",
	0x0C,"SNOW..........[#]",
	0x0D,"DEWDROP.......[#]",
	0x0E,"FOG...........[#]",
	0x0F,"CLOUD.........[#]",
	0x10,"VISIBILITY....[#]",
	0x11,"BAK/LUMIRUNCE.[#]",
	0x12,"SIGMA.........[#]",
	0x13,"SKY EMISSION..[#]",
	0x14,"GND EMISSION..[#]",
	0x15,"PH............[#]",
	0x16,"CONDUCTE......[#]",
	0xFF,"",
};
//[*]---------------------------------------------------------------------[*]
static module_table_t module_table[] =	{
	0x00,"MAIN PROC.....[#]",
	0x01,"RCM  PROC.....[#]",
	0xFF,"",
};
//[*]---------------------------------------------------------------------[*]
typedef struct 	sensor_field__t	{
	unsigned char	sensor_st;
	unsigned char	sensor_code;
	unsigned char	sensor_id;
			 float	aver;
			 float	spec;
			 float	mini;
			 float	maxi;
}	sensor_field_t;

typedef struct 	weather__t	{
	unsigned long	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	rtu_id;
	unsigned int	station_id;
	unsigned char	board_id;
	unsigned int	schedule;
	sensor_field_t	*sensor;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	weather_t;

weather_t weather[4];
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
idata byte read_rtu_id(void);
void led_con(idata byte ch,idata byte sel,idata byte sw);
void initial_weather(weather_t *weather);
int weather_size(void);
char *get_module_name(unsigned char modulecode);
char *org_get_sensor_name(unsigned char scode);
char *get_sensor_name(unsigned char id);
void menu_sensor_table_init(void);
unsigned char get_sensor_code(unsigned char id);
void menu_initliaze(void);
void main_menu_func(unsigned char pos);
void sub_menu_func(unsigned char pos);
void init_menu(menu_t *menu, unsigned char menuflag,
							 unsigned char itemcnt,
							   signed char line,
							   signed char spos,
							   signed char cpos);
void init_menu_item(menu_t *menu,int pos,unsigned char *str,void (*runfunc)(unsigned char pos));
void menu_display(menu_t *menu);
void main_loop(menu_t *menu);
void sub_menu_control(menu_t *menu);
char up_dn_key_insert(void);
void real_display_func(unsigned char pos);
void aver_display_func(unsigned char pos);
void sensor_status_display_func(unsigned char pos);
void module_status_display_func(unsigned char pos);
char mk_cursor(byte xpos, byte ypos, byte kbuf_pos, byte gubun);
unsigned int  h_to_a(char *buf,int sp,int len);
byte password_check(void);
void date_set(unsigned char pos);
void time_set(unsigned char pos);
void id_set(unsigned char pos);
void password_set(unsigned char pos);
void offset_set_func(unsigned char pos);
void clear_buf(unsigned char pos);
void memory_test(unsigned char pos);
void volt_disp(unsigned char pos);
void poll_real_data(void);
void poll_call_data(void);
void poll_save_data(void);
void sensor_status_set(byte call_id, byte status);
void board_status_set(byte call_id, byte status);
void variable_initial(void);
void mpm_cfg_init(void);
void mpm_cfg_save(idata word address);
void mpm_cfg_load(idata word address);
void memory_check(void);
void buffer_clear(void);
void put_data(idata byte put_d);
void save_data(idata byte b);
char get_data(void);
word load_data(idata byte b);
void max_min_save(void);
void max_min_load(idata byte sen_id);
void variable_save(idata word base, idata byte sen_id, idata float v);
float variable_load(idata word base, idata byte sen_id);
void rtc_set(void);
void delay(word count);
idata unsigned char keyinput(void);
void main(void);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#include "rtc.hpp"
#include "lcd.hpp"
#include "init&int.hpp"
#include "default.mpm"
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
idata byte read_rtu_id(void)
{
	idata byte value_1, value_2;

	CNT_CSH();	value_1 = input(0x00) & 0xF0;	RAM_CS();
	value_2 = ((~value_1) >> 4) & 0x0F;
	value.factory = value_2 & 0x08;
	value_2       = value_2 & 0x07;

	return value_2;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void led_con(idata byte ch, idata byte sel, idata byte sw)
{
	switch(ch)	{
		case	COM1:	if(sel == TX)	led.bits.tx1 = sw;
						else 			led.bits.rx1 = sw;	break;
		case	COM2:	if(sel == TX)	led.bits.tx2 = sw;
						else 			led.bits.rx2 = sw;	break;
		case	COM3:	if(sel == TX)	led.bits.tx3 = sw;
						else 			led.bits.rx3 = sw;	break;
	}
	LED_CS();	output(LED_ADD, led.led_data);	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void initial_weather(weather_t *weather)
{
	weather->header = 0xFFFFFFFF;
	weather->sensor	= (sensor_field_t *)(malloc(sizeof(sensor_field_t) * i_data.sensor_su));
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
int weather_size(void)
{
	return  sizeof(weather_t) - sizeof(sensor_field_t *) +
			sizeof(sensor_field_t) * i_data.sensor_su;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char *get_module_name(unsigned char modulecode)
{
	byte i = 0;

	while(1)	{
		if(module_table[i].modulecode ==    0xFF)	 return "";
		if(module_table[i].modulecode == modulecode) return module_table[i].name;
		i++;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char *org_get_sensor_name(unsigned char scode)
{
	byte i = 0;

	while(1)	{
		if(sensor_table[i].sencode ==  0xFF) return "";
		if(sensor_table[i].sencode == scode) return sensor_table[i].name;
		i++;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char *get_sensor_name(unsigned char id)
{
	return menu_sensor_table[id].name;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void menu_sensor_table_init(void)
{
	unsigned char   scode, i, id;
	unsigned char 	code_count_array[100];

	menu_sensor_table = (sensor_table_t *)malloc(sizeof(sensor_table_t) * i_data.sensor_su);

	for(i=0; i<100; i++) code_count_array[i] = 0;

	for(id=0; id<i_data.sensor_su; id++)	{
		scode 						  = get_sensor_code(id);
		menu_sensor_table[id].sencode = scode;
		strcpy(menu_sensor_table[id].name, org_get_sensor_name(scode));
		code_count_array[scode]++;
		menu_sensor_table[id].name[15] = code_count_array[scode] + 0x30;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
unsigned char get_sensor_code(unsigned char id)
{
	idata byte i, sen_code;

	WATCHDOG;
	i = id * 2 + 1;
	CFG_RAM(MPM_CFG);	sen_code = input(ID_CODE + i);	RAM_CS();
	switch(sen_code)	{
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
	byte pos;

	init_menu	  (&main_menu,MAIN,4,3,0,1);
	init_menu_item(&main_menu,  0," REAL DISPLAY    ",main_menu_func);
	init_menu_item(&main_menu,  1," AVERAGE DISPLAY ",main_menu_func);
	init_menu_item(&main_menu,  2," RTU STATUS      ",main_menu_func);
	init_menu_item(&main_menu,  3," RTU INITIALL    ",main_menu_func);

	init_menu	  (&sub_menu[0],SUB,4,4,0,0);
	init_menu_item(&sub_menu[0],0," SENSOR STATUS   ",sub_menu_func);
	init_menu_item(&sub_menu[0],1," MODULE STATUS   ",sub_menu_func);
	init_menu_item(&sub_menu[0],2," VOLTAGE STATUS  ",volt_disp);
	init_menu_item(&sub_menu[0],3," MEMORY TEST     ",memory_test);

	init_menu	  (&sub_menu[1],SUB,6,4,0,0);
	init_menu_item(&sub_menu[1],0," DATE SET        ",date_set);
	init_menu_item(&sub_menu[1],1," TIME SET        ",time_set);
	init_menu_item(&sub_menu[1],2," ID SET          ",id_set);
	init_menu_item(&sub_menu[1],3," PASSWORD SET    ",password_set);
	init_menu_item(&sub_menu[1],4," OFFSET SET      ",sub_menu_func);
	init_menu_item(&sub_menu[1],5," BUFFER CLEAR    ",clear_buf);

	init_menu	  (&real_menu,SUB,i_data.sensor_su,4,0,0);
	for(pos=0; pos<i_data.sensor_su; pos++)	{
		init_menu_item(&real_menu,pos,get_sensor_name(pos),real_display_func);
	}
	init_menu	  (&aver_menu,SUB,i_data.sensor_su,4,0,0);
	for(pos=0; pos<i_data.sensor_su; pos++)	{
		init_menu_item(&aver_menu,pos,get_sensor_name(pos),aver_display_func);
	}
	init_menu	  (&status_menu,SUB,i_data.sensor_su,4,0,0);
	for(pos=0; pos<i_data.sensor_su; pos++)	{
		init_menu_item(&status_menu,pos,get_sensor_name(pos),sensor_status_display_func);
	}
	init_menu	  (&module_menu,SUB,i_data.sensor_su+2,4,0,0);
	for(pos=0; pos<i_data.sensor_su+2; pos++)	{
		if(pos < 2)	init_menu_item(&module_menu,pos,get_module_name(pos),module_status_display_func);
		else		init_menu_item(&module_menu,pos,get_sensor_name(pos-2),module_status_display_func);
	}
	init_menu	  (&offset_menu,SUB,i_data.sensor_su,4,0,0);
	for(pos=0; pos<i_data.sensor_su; pos++)	{
		init_menu_item(&offset_menu,pos,get_sensor_name(pos),offset_set_func);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main_menu_func(unsigned char pos)
{
	lcd_clear();
	if     (pos == 0)	sub_menu_control(&real_menu);
	else if(pos == 1)	sub_menu_control(&aver_menu);
	else if(pos == 2)	sub_menu_control(&sub_menu[0]);
	else				sub_menu_control(&sub_menu[1]);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sub_menu_func(unsigned char pos)
{
	if     (pos == 0)	sub_menu_control(&status_menu);
	else if(pos == 1)	sub_menu_control(&module_menu);
	else if(pos == 4)	sub_menu_control(&offset_menu);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_menu(menu_t *menu, unsigned char menuflag,
							 unsigned char itemcnt,
							   signed char line,
							   signed char spos,
							   signed char cpos)
{
	WATCHDOG;
	menu->menuflag = menuflag;
	menu->itemcnt  =  itemcnt;
	menu->scr_line =	 line;
	menu->scr_spos =     spos;
	menu->scr_cpos =	 cpos;
	menu->menuquit =        0;
	menu->item     = (menuitem_t *)(malloc(sizeof(menuitem_t) * itemcnt)); //menu->itemcnt));
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_menu_item(menu_t *menu,int pos,unsigned char *str,void (*runfunc)(unsigned char pos))
{
	WATCHDOG;
	menu->item[pos].runfunc  =  runfunc;
	strcpy(menu->item[pos].menustr,str);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void menu_display(menu_t *menu)
{
	idata byte i;

	for(i=menu->scr_spos; i<(menu->scr_spos+menu->scr_line); i++) {
		if(i < menu->itemcnt)		{
			lcd_printf(2,(i-menu->scr_spos)+(4-menu->scr_line),1,menu->item[i].menustr);
		}	else	{
			lcd_printf(2,(i-menu->scr_spos)+(4-menu->scr_line),1," ");
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main_loop(menu_t *menu)
{
	signed char old_spos = -1;
	signed char old_cpos =  1;
	menu->menuquit		 =  0;

	while(!menu->menuquit)	{
		if(old_spos != menu->scr_spos)	{
			lcd_printf(0,0,1,"[--- MAIN MENUS ---]");
			menu_display(menu);
			lcd_printf( 0,menu->scr_cpos,0,"=>");
			lcd_printf(18,menu->scr_cpos,0,"<=");
			old_spos = menu->scr_spos;
		}
		if(old_cpos != menu->scr_cpos)	{
			lcd_printf( 0,old_cpos      ,0,"  ");
			lcd_printf(18,old_cpos      ,0,"  ");
			lcd_printf( 0,menu->scr_cpos,0,"=>");
			lcd_printf(18,menu->scr_cpos,0,"<=");
			old_cpos = menu->scr_cpos;
		}
		switch(keyinput())	{
			case UP_ARROW :	menu->scr_cpos--;
							if(menu->scr_cpos < 1)	{
								menu->scr_cpos = 1;
								if(menu->scr_spos > 0)
									menu->scr_spos--;
							}
							break;
			case DN_ARROW :	menu->scr_cpos++;
							if(menu->scr_cpos >= menu->itemcnt)
								menu->scr_cpos = menu->itemcnt + 1;
							if(menu->scr_cpos > 3)	{
								menu->scr_cpos = 3;
								if(menu->scr_spos < (menu->itemcnt - 3))
									menu->scr_spos++;
							}
							break;
			case ENTER1   :
			case ENTER2   :	menu->item[menu->scr_spos + menu->scr_cpos - 1].runfunc(menu->scr_spos + menu->scr_cpos - 1);
							old_spos = -1;
							break;
			case ESC2     :
			case ESC1     :	if(menu->menuflag == 0)	menu->menuquit = 1;
							break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#pragma function = recursive
void sub_menu_control(menu_t *menu)
{
	signed char old_spos = -1;
	signed char old_cpos =  0;
	menu->menuquit		 =  0;

	while(!menu->menuquit)	{
		if(old_spos != menu->scr_spos)	{
			menu_display(menu);
			lcd_printf(0,menu->scr_cpos,0,"=>");
			old_spos = menu->scr_spos;
		}
		if(old_cpos != menu->scr_cpos)	{
			lcd_printf(0,old_cpos      ,0,"  ");
			lcd_printf(0,menu->scr_cpos,0,"=>");
			old_cpos = menu->scr_cpos;
		}
		switch(keyinput())	{
			case UP_ARROW :	menu->scr_cpos--;
							if(menu->scr_cpos < 0)	{
								menu->scr_cpos = 0;
								if(menu->scr_spos > 0)
									menu->scr_spos--;
							}
							break;
			case DN_ARROW :	menu->scr_cpos++;
							if(menu->scr_cpos >= menu->itemcnt)
								menu->scr_cpos = menu->itemcnt - 1;
							if(menu->scr_cpos > 3)	{
								menu->scr_cpos = 3;
								if(menu->scr_spos < (menu->itemcnt-4))
									menu->scr_spos++;
							}
							break;
			case ENTER1   :
			case ENTER2   :	menu->item[menu->scr_spos + menu->scr_cpos].runfunc(menu->scr_spos + menu->scr_cpos);
							old_spos = -1;
							break;
			case ESC1	  :
			case ESC2     :	if(menu->menuflag == 0)	menu->menuquit = 1;
							break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char up_dn_key_insert(void)
{
	switch(keyinput())	{
		case ESC1    :
		case ESC2    :	return(1);
		case UP_ARROW:	key.key_data[key.key_pos_st++] = UP_ARROW;
						if(key.key_pos_st > 20)	key.key_pos_st = 0;
						key.key_data[key.key_pos_st++] = ENTER1;
						if(key.key_pos_st > 20)	key.key_pos_st = 0;
						return(1);
		case DN_ARROW:	key.key_data[key.key_pos_st++] = DN_ARROW;
						if(key.key_pos_st > 20)	key.key_pos_st = 0;
						key.key_data[key.key_pos_st++] = ENTER1;
						if(key.key_pos_st > 20)	key.key_pos_st = 0;
						return(1);
		default		 :	return(0);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void real_display_func(unsigned char pos)
{
	byte oldsec = 70;

	lcd_clear();
	lcd_printf(0,1,1,"[*]");
	lcd_printf(3,1,0,get_sensor_name(pos));
	while(1)	{
		if(oldsec != rtc.sec)	{
			oldsec = rtc.sec;
			lcd_printf(0,0,1," %02d'%02d/%02d  %02d:%02d:%02d ",
					 rtc.year,rtc.month,rtc.day,rtc.hour,rtc.min,rtc.sec);
			if(weather[REAL].sensor[pos].sensor_id == pos)	{
				switch(weather[REAL].sensor[pos].sensor_code)	{
					case  Prec_Detect:
						if(weather[REAL].sensor[pos].aver)	lcd_printf(5,2,0,"PRE : ON ");
						else								lcd_printf(5,2,0,"PRE : OFF");
						break;
					case  Fog:
					case  Visibility:
						lcd_printf(0,2,0,"RL:%6.2f  MI:%6.2f",
							(float)(weather[REAL].sensor[pos].aver),
							(float)(weather[REAL].sensor[pos].mini));
						lcd_printf(0,3,0,"SP:%6.2f  MX:%6.2f",
							(float)(weather[REAL].sensor[pos].spec),
							(float)(weather[REAL].sensor[pos].maxi));
						break;
					default:
						lcd_printf(0,2,0,"RL:%6.1f  MI:%6.1f",
							(float)(weather[REAL].sensor[pos].aver),
							(float)(weather[REAL].sensor[pos].mini));
						lcd_printf(0,3,0,"SP:%6.1f  MX:%6.1f",
							(float)(weather[REAL].sensor[pos].spec),
							(float)(weather[REAL].sensor[pos].maxi));
						break;
				}
			} else	lcd_printf(3,2,0,"NOT RECEIVE !!");
			real.receive_status = OFF;
			sam_sign_send(&com1, 0x00, pos, sizeof(sam_t));
			sam_send(&com1, 'W', REAL, pos);
			real.delay_count = 0;
			while(real.delay_count < 300)	{
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
				if(real.receive_status == ON)	break;
			}
		}
		if(up_dn_key_insert())	break;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void aver_display_func(unsigned char pos)
{
	byte oldsec = 70;

	lcd_clear();
	lcd_printf(0,1,1,"[*]");
	lcd_printf(3,1,0,get_sensor_name(pos));
	while(1)	{
		if(oldsec != rtc.sec)	{
			oldsec = rtc.sec;
			lcd_printf(0,0,1," %02d'%02d/%02d  %02d:%02d:%02d ",
					 rtc.year,rtc.month,rtc.day,rtc.hour,rtc.min,rtc.sec);
			if(weather[SAVE].sensor[pos].sensor_id == pos)	{
				switch(weather[SAVE].sensor[pos].sensor_code)	{
					case  Prec_Detect:
						if(weather[SAVE].sensor[pos].aver)	lcd_printf(5,2,0,"PRE : ON ");
						else								lcd_printf(5,2,0,"PRE : OFF");
						break;
					case  Fog:
					case  Visibility:
						lcd_printf(0,2,0,"AV:%6.2f  MI:%6.2f",
							(float)(weather[SAVE].sensor[pos].aver),
							(float)(weather[SAVE].sensor[pos].mini));
						lcd_printf(0,3,0,"SP:%6.2f  MX:%6.2f",
							(float)(weather[SAVE].sensor[pos].spec),
							(float)(weather[SAVE].sensor[pos].maxi));
						break;
					default:
						lcd_printf(0,2,0,"AV:%6.1f  MI:%6.1f",
							(float)(weather[SAVE].sensor[pos].aver),
							(float)(weather[SAVE].sensor[pos].mini));
						lcd_printf(0,3,0,"SP:%6.1f  MX:%6.1f",
							(float)(weather[SAVE].sensor[pos].spec),
							(float)(weather[SAVE].sensor[pos].maxi));
						break;
				}
			} else	lcd_printf(3,2,0,"NOT RECEIVE !!");
		}
		if(up_dn_key_insert())	break;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sensor_status_display_func(unsigned char pos)
{
	word dl_time;
	byte oldsec = 70, sta_check, arrary_mok, arrary_na, a_data;

	lcd_clear();
	dl_time = rtc.hour * 60 + rtc.min;
	lcd_printf(0,1,1,"[*]");
	lcd_printf(3,1,0,get_sensor_name(pos));
	lcd_printf(3,2,1,"SENSOR : ");
	while(1)	{
		if(oldsec != rtc.sec)	{
			oldsec = rtc.sec;
			lcd_printf(0,0,1," %02d'%02d/%02d  %02d:%02d:%02d ",
					 rtc.year,rtc.month,rtc.day,rtc.hour,rtc.min,rtc.sec);
			if(!pos)	{ arrary_mok = 0;		arrary_na = 0;       }
			else		{ arrary_mok = pos / 7;	arrary_na = pos % 7; }
			sta_check = 0x01;
			sta_check = (sta_check << arrary_na);
			a_data    = rcm.send.sen_sta[arrary_mok];
			if(!(a_data & sta_check))	lcd_printf(12,2,0,"NORMAL");
			else						lcd_printf(12,2,0,"FAIL  ");
		}
		if(up_dn_key_insert())	break;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void module_status_display_func(unsigned char pos)
{
	word dl_time;
	byte oldsec = 70, sta_check, arrary_mok, arrary_na, a_data;

	lcd_clear();
	dl_time = rtc.hour * 60 + rtc.min;
	lcd_printf(0,1,1,"[*]");
	lcd_printf(3,2,1,"MODULE : ");
	if(pos < 2)	lcd_printf(3,1,0,get_module_name(pos    ));
	else		lcd_printf(3,1,0,get_sensor_name(pos - 2));
	while(1)	{
		if(oldsec != rtc.sec)	{
			oldsec = rtc.sec;
			lcd_printf(0,0,1," %02d'%02d/%02d  %02d:%02d:%02d ",
					 rtc.year,rtc.month,rtc.day,rtc.hour,rtc.min,rtc.sec);
			if(!pos)	{ arrary_mok = 0;		arrary_na = 0;       }
			else		{ arrary_mok = pos / 7;	arrary_na = pos % 7; }
			sta_check = 0x01;
			sta_check = (sta_check << arrary_na);
			a_data    = rcm.send.bod_sta[arrary_mok];
			if(!(a_data & sta_check))	lcd_printf(12,2,0,"NORMAL");
			else						lcd_printf(12,2,0,"FAIL  ");
		}
		if(up_dn_key_insert())	break;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char mk_cursor(byte xpos, byte ypos, byte kbuf_pos, byte gubun)
{
	if(lcd.cursor_cnt > 200)	{
		if(lcd.cursor_sta == 0x5F)	lcd.cursor_sta = ' ';
		else	lcd.cursor_sta = 0x5F;
		lcd.cursor_cnt = 0;
	}
	switch(gubun)	{
		case 'd':	if((kbuf_pos == 2) || (kbuf_pos == 5))	{
						lcd_printf(xpos+kbuf_pos,ypos,0,"//");
						kbuf_pos++;
					} else	if(kbuf_pos > 8)	{
						kbuf_pos = 0;
						lcd_printf(xpos+kbuf_pos,ypos,0,"         ");
					}	break;
		case 't':	if((kbuf_pos == 2) || (kbuf_pos == 5))	{
						lcd_printf(xpos+kbuf_pos,ypos,0,":");
						kbuf_pos++;
					} else	if(kbuf_pos > 8)	{
						kbuf_pos = 0;
						lcd_printf(xpos+kbuf_pos,ypos,0,"         ");
					}	break;
		default :	if(kbuf_pos > 4)	{
						kbuf_pos = 0;
						lcd_printf(xpos+kbuf_pos,ypos,0,"     ");
					}	break;
	}
	lcd_printf(xpos+kbuf_pos,ypos,0,"%c",lcd.cursor_sta);
	return(kbuf_pos);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
unsigned int  h_to_a(char *buf,int sp,int len)
{
	byte BUF[10];
	int  i,cnt=0;

	for(i=sp; i<sp+len; i++)	BUF[cnt++] = buf[i];
	BUF[cnt] = 0;
	return(atoi(BUF));
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte password_check(void)
{
	byte i, k, x, y, kbuf_pos = 0;
	word pass;
	byte K_BUF[10];
	byte oldsec = 70;

	lcd_clear();
	i = 0;	x = 12;	y = 1;
	lcd_printf(0,1,1," PASSWORD : **** ");
	while(1)	{
		if(oldsec != rtc.sec)	{
			lcd_printf(x,y,0,"**** ");
			oldsec = rtc.sec;
		}
		kbuf_pos = mk_cursor(x,y,kbuf_pos,'p');
		k = keyinput();
		if( i >= 3 )						return(0);
		if((k == ESC1  ) || (k == ESC2  ))	return(0);
		if((k == ENTER1) || (k == ENTER2))	{
			if(kbuf_pos >= 4)	{
				pass = h_to_a(K_BUF,0,4);
				if((i_data.password == pass) || (SUPER == pass))	return(1);
				else	{
					kbuf_pos = 0;	i++;
				}
			}
		} else	if(k < 10)	{
			lcd_printf(x+kbuf_pos,y,0,"*");
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void date_set(unsigned char pos)
{
	byte k, x, y, kbuf_pos = 0;
	byte K_BUF[10];
	byte oldsec = 70;

	pos = pos;
	if(!password_check())   return;
	lcd_clear();
	x = 11;	y = 2;
	lcd_printf(0,0,1,"[---- DATE SET ----]");
	lcd_printf(0,1,1,"NOW DATE :");
	lcd_printf(0,2,1,"NEW DATE :");
	while(1)	{
		if(oldsec != rtc.sec)	{
			lcd_printf(x,1,0,"%02d/%02d/%02d ",rtc.year,rtc.month,rtc.day);
			lcd_printf(0,3,1," ");
			oldsec = rtc.sec;
		}
		kbuf_pos = mk_cursor(x,y,kbuf_pos,'d');
		k = keyinput();
		if((k == ESC1) || (k == ESC2))	break;
		if((k == ENTER1) || (k == ENTER2))	{
			if(kbuf_pos >= 8)	{
				disable(EA);
				rtc.year  = h_to_a(K_BUF,0,2);
				rtc.month = h_to_a(K_BUF,3,2);
				rtc.day   = h_to_a(K_BUF,6,2);
				rtc_date_write(&rtc);
			}
		} else	if(k < 10)	{
			lcd_printf(x+kbuf_pos,y,0,"%c",k + 0x30);
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void time_set(unsigned char pos)
{
	byte k, x, y, kbuf_pos = 0;
	byte K_BUF[10];
	byte oldsec = 70;

	pos = pos;
	if(!password_check())   return;
	lcd_clear();
	x = 11;	y = 2;
	lcd_printf(0,0,1,"[---- TIME SET ----]");
	lcd_printf(0,1,1,"NOW TIME :");
	lcd_printf(0,2,1,"NEW TIME :");
	while(1)	{
		if(oldsec != rtc.sec)	{
			lcd_printf(x,1,0,"%02d:%02d:%02d ",rtc.hour,rtc.min,rtc.sec);
			lcd_printf(0,3,1," ");
			oldsec = rtc.sec;
		}
		kbuf_pos = mk_cursor(x,y,kbuf_pos,'t');
		k = keyinput();
		if((k == ESC1) || (k == ESC2))	break;
		if((k == ENTER1) || (k == ENTER2))	{
			if(kbuf_pos >= 8)	{
				disable(EA);
				rtc.hour = h_to_a(K_BUF,0,2);
				rtc.min  = h_to_a(K_BUF,3,2);
				rtc.sec  = h_to_a(K_BUF,6,2);
				rtc_time_write(&rtc);
			}
		} else	if(k < 10)	{
			lcd_printf(x+kbuf_pos,y,0,"%c",k + 0x30);
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void id_set(unsigned char pos)
{
	byte k, x, y, kbuf_pos = 0;
	byte K_BUF[10];
	byte oldsec = 70;

	pos = pos;
	if(!password_check())   return;
	lcd_clear();
	x = 15;	y = 2;
	lcd_printf(0,0,1,"[- STATION ID SET -]");
	lcd_printf(0,1,1,"NOW STATION ID:");
	lcd_printf(0,2,1,"NEW STATION ID:");
	while(1)	{
		if(oldsec != rtc.sec)	{
			lcd_printf(x,1,0,"%04d ",i_data.station_id);
			lcd_printf(0,3,1," ");
			oldsec = rtc.sec;
		}
		kbuf_pos = mk_cursor(x,y,kbuf_pos,'i');
		k = keyinput();
		if((k == ESC1) || (k == ESC2))	break;
		if((k == ENTER1) || (k == ENTER2))	{
			if(kbuf_pos >= 4)	{
				i_data.station_id = h_to_a(K_BUF,0,4);
				mpm_cfg_save(STATION_ID);
			}
		} else	if(k < 10)	{
			lcd_printf(x+kbuf_pos,y,0,"%c",k + 0x30);
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void password_set(unsigned char pos)
{
	byte k, x, y, kbuf_pos = 0;
	byte togul = 0;
	word pass;
	byte K_BUF[10];
	byte oldsec = 70;

	pos = pos;
	lcd_clear();
	x = 15;	y = 1;
	lcd_printf(0,0,1,"[-- PASSWORD SET --]");
	lcd_printf(0,1,1,"NOW PASSWORD : **** ");
	while(1)	{
		if(oldsec != rtc.sec)	{
			lcd_printf(x,y,0,"**** ");
			lcd_printf(0,3,1," ");
			oldsec = rtc.sec;
		}
		kbuf_pos = mk_cursor(x,y,kbuf_pos,'p');
		k = keyinput();
		if(( k == ESC1  ) || (k == ESC2  ))	break;
		if(((k == ENTER1) || (k == ENTER2)) && togul == 0)	{
			if(kbuf_pos >= 4)	{
				pass = h_to_a(K_BUF,0,4);
				if((i_data.password == pass) || (SUPER == pass))	{
					lcd_printf(x,y,0,"**** ");
					k = 0x70;
					y = 2;
					togul = 1;
					kbuf_pos = 0;
					lcd_printf(0,y,1,"NEW PASSWORD :");
				} else	kbuf_pos = 10;
			}
		}
		if(((k == ENTER1) || (k == ENTER2)) && togul == 1)	{
			if(kbuf_pos >= 4)	{
				i_data.password = h_to_a(K_BUF,0,4);
				mpm_cfg_save(PASSWORD);
				break;
			}
		} else	if(k < 10)	{
			lcd_printf(x+kbuf_pos,y,0,"*");
			K_BUF[kbuf_pos] = k + 0x30;
			kbuf_pos++;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void offset_set_func(unsigned char pos)
{
	byte k;

	lcd_clear();
	lcd_printf(0,0,1,"[-- OFFSET SETUP --]");

	lcd_printf(0,1,1,"[*]");
	lcd_printf(3,1,0,get_sensor_name(pos));
	lcd_printf(2,2,1,"PRESS ENTER KEY..");
	while(1)	{
		k = keyinput();
		if((k == ESC1  ) || (k == ESC2  ))	break;
		if((k == ENTER1) || (k == ENTER2))	{
			sam_sign_send(&com1, 0x00, pos, sizeof(sam_t));
			sam_send(&com1, '*', REAL, pos);
			variable_save(SPECIAL_BASE, pos, 0.0);
			variable_save(OFFSET_BASE, pos, 0.0);
			break;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void clear_buf(unsigned char pos)
{
	pos = pos;
	if(!password_check())   return;
	buffer_clear();
	while(1)	disable(EA);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void memory_test(unsigned char pos)
{
	idata byte x, y, h, get_data, flag;
	idata word dl_time, extr_addr;
	unsigned long ram_size;

	pos = pos;
	lcd_clear();
	dl_time = rtc.hour * 60 + rtc.min;
	flag      = 1;
	ram_size  = 0;
	extr_addr = 0;
	x = 12;	y = 1;
	lcd_printf(0,0,1,"[-- MEMORY TEST ---]");
	lcd_printf(0,y,1,"CFG MEMORY");
	for(h = 0; h < 2;)	{
		CFG_RAM(h);
		get_data = input(extr_addr);
		output(extr_addr,0xAA);
		if(input(extr_addr) != 0xAA)	break;
		output(extr_addr,get_data);
		RAM_CS();
		extr_addr ++;
		if(extr_addr == 0)	h ++;
		if((extr_addr % 512) == 0)	lcd_printf(x++,y,0,".");
		if(x > 19)	{
			x = 12;	lcd_printf(x,y,0,"        ");
		}
		WATCHDOG;
		ram_size ++;
		if(keyinput() != 0x70)	{
			flag = 0;	break;
		}
	}
	RAM_CS();
	if(flag)	{
		if(ram_size < 131072L)	lcd_printf(12,y,0,": ERROR ");
		else					lcd_printf(12,y,0,": OK    ");
	} else	lcd_printf(12,y,0,": CANCEL");

	dl_time = rtc.hour * 60 + rtc.min;
	flag      = 1;
	ram_size  = 0;
	extr_addr = 0;
	x = 12;	y = 2;
	lcd_printf(0,y,1,"DATA MEMORY");
	for(h = 0; h < 14;)	{
		DAT_RAM(h);
		get_data = input(extr_addr);
		output(extr_addr,0xAA);
		if(input(extr_addr) != 0xAA)	break;
		output(extr_addr,get_data);
		RAM_CS();
		extr_addr ++;
		if(extr_addr == 0)	h++;
		if((extr_addr % 512) == 0)	lcd_printf(x++,y,0,".");
		if(x > 19)	{
			x = 12;
			lcd_printf(x,y,0,"        ");
		}
		WATCHDOG;
		ram_size ++;
		if(keyinput() != 0x70)	{
			flag = 0;	break;
		}
	}
	RAM_CS();
	if(flag)	{
		if(ram_size < value.ram_size)	lcd_printf(12,y,0,": ERROR ");
		else							lcd_printf(12,y,0,": OK    ");
	} else	lcd_printf(12,y,0,": CANCEL");

	dl_time = rtc.hour * 60 + rtc.min;
	lcd_printf(2,3,1,"PRESS ANY KEY....");
	delay(60000);	delay(60000);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
/*char auto_delay(word dl_time, char del)
{
	if(keyinput() != 0x70)	return(1);
	if((rtc.hour * 60 + rtc.min) - dl_time > del)	return(1);
	return(0);
}*/
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void volt_disp(unsigned char pos)
{
	byte oldsec = 70;
	word dl_time;

	pos = pos;
	lcd_clear();
	dl_time = rtc.hour * 60 + rtc.min;
	lcd_printf(0,0,1,"[---- VOLTAGES ----]");
	while(1)	{
		if(oldsec != rtc.sec)	{
			oldsec = rtc.sec;
			lcd_printf(0,1,1,"  VCC ------  %5.2f",rcm.send.p5);
			lcd_printf(0,2,1,"  VPP ------  %5.2f",rcm.send.p12);
			lcd_printf(0,3,1,"  VSS ------ %5.2f ",rcm.send.m12);
		}
		if(up_dn_key_insert())	break;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void variable_initial(void)
{
	idata word i, j;

	lcd.cursor_cnt	= 0x0000;
	lcd.lcd_pos		= 0x00;		lcd.buf_pos		= 0x00;
	lcd.cursor_sta	= 0x5F;		lcd.cursor_pos	= 0x00;

	key.key_chtr	= 0x00;		key.key_cmp		= 0x00;
	key.key_del		= 0x00;
	key.key_pos_st  = 0;		key.key_pos_ed  = 0;

	i_data.extr_addr = 0x0000;
	value.buzzer_cnt = led.led_data     = 0x00;

	real.call_id = call.call_id = save.call_id = 0;

	for(i=0; i<8; i++)	rcm.send.bod_sta[i] = 0x7F;
	for(i=0; i<8; i++)	rcm.send.sen_sta[i] = 0x7F;
	for(i=0; i<2; i++)	rcm.send.event  [i] = 0x00;

	for(i=0; i<4; i++)	{						// Weather valiable clear
		for(j=0; j<i_data.sensor_su; j++)	{
			weather[i].sensor[j].aver = weather[i].sensor[j].spec = 0.0;
			weather[i].sensor[j].mini = weather[i].sensor[j].maxi = 0.0;
		}
	}
	call.call_min = (byte)(i_data.call_time / 60);
	call.call_sec = (byte)(i_data.call_time % 60);
	save.save_min = (byte)(i_data.save_time / 60);
	save.save_sec = (byte)(i_data.save_time % 60);

	value.record_su  = (word)(86400 / i_data.save_time);
	value.delay_count = 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void mpm_cfg_init(void)
{
	idata byte extr_chkxor, extr_chksum, flag;
	idata word i;
		  word re_count =   1;
		  byte temt_sec = 100;

	extr_chkxor = extr_chksum = 0xAA;
	flag = 0;
	CFG_RAM(MPM_CFG);
	if((input(MPM_CFG_CHK1) != extr_chkxor) || (input(MPM_CFG_CHK2) != extr_chkxor))		flag = 1;
	else	{
		for(i=MPM_CFG_BASE; i<MPM_CFG_END; i++)	{
			extr_chkxor ^= input(i);
			extr_chksum += input(i);
		}
		if((extr_chkxor != input(MPM_CFG_XOR)) || (extr_chksum != input(MPM_CFG_SUM)))		flag = 2;
	}
	RAM_CS();
	initiliaze();												// Serial port & rxQ initialize.
	if(flag)	{
		lcd_clear();
		switch(flag)	{
			case 1 : lcd_printf(0,0,1,"CONFIG DATA HEADER  ");	break;
			case 2 : lcd_printf(0,0,1,"CONFIG DATA CHECKSUM");	break;
		}
		lcd_printf(0,1,1,"     BROKEN !!      ");
		lcd_printf(0,2,1,"CONFIG DATA REQUEST.");
		value.config_receive_status = OFF;
		while(value.config_receive_status == OFF)	{
			if((rtc.sec % 5 == 0) && (rtc.sec != temt_sec))	{
				temt_sec = rtc.sec;
				host_sign_send_com2(&com2, MPM_ID, REQUEST, 0);	// Cfg data request send
				lcd_printf(0,3,1,"REQUEST CNT : %5d",re_count++);
			}
			receive_check_23();	transmit_check_23();
			if((value.factory == 0x08) && (re_count > 12))		break;
		}
		if(value.config_receive_status == ON)	{
			lcd_printf(0,3,1,"CONFIG DATA RECEIVED");	delay(30000);
		} else	{
			lcd_clear();
			lcd_printf(0,1,1,"FACTORY SET .......!");	delay(30000);
			factory_set();
			initiliaze();										// Serial port & rx Q initialize
			led_con(COM1,TX,OFF);	led_con(COM1,RX,OFF);
			led_con(COM2,TX,OFF);	led_con(COM2,RX,OFF);
			led_con(COM3,TX,OFF);	led_con(COM3,RX,OFF);
		}
	}
	mpm_cfg_load(SENSOR_SU );	mpm_cfg_load(DAY_CNT  );
	mpm_cfg_load(STATION_ID);	mpm_cfg_load(PASSWORD );
	mpm_cfg_load(CALL_TIME );	mpm_cfg_load(SAVE_TIME);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void mpm_cfg_save(idata word address)			// MPM Configration save
{
	idata byte extr_chkxor, extr_chksum;
	idata word i;

	CFG_RAM(MPM_CFG);
	switch(address)	{
		case SENSOR_SU  :	output(SENSOR_SU     ,i_data.sensor_su      );
							break;
		case DAY_CNT	:	output(DAY_CNT       ,i_data.day_cnt        );
							break;
		case STATION_ID	:	output(STATION_ID    ,i_data.station_id >> 8);
							output(STATION_ID + 1,i_data.station_id     );
							break;
		case PASSWORD	:	output(PASSWORD      ,i_data.password   >> 8);
							output(PASSWORD   + 1,i_data.password       );
							break;
		case CALL_TIME	:	output(CALL_TIME	 ,i_data.call_time  >> 8);
							output(CALL_TIME  + 1,i_data.call_time      );
							break;
		case SAVE_TIME	:	output(SAVE_TIME	 ,i_data.save_time  >> 8);
							output(SAVE_TIME  + 1,i_data.save_time      );
							break;
	}
	extr_chkxor = extr_chksum = 0xAA;
	output(MPM_CFG_CHK1,extr_chkxor);
	output(MPM_CFG_CHK2,extr_chkxor);
	for(i=MPM_CFG_BASE; i<MPM_CFG_END; i++)	{
		extr_chkxor ^= input(i);
		extr_chksum += input(i);
	}
	output(MPM_CFG_XOR,extr_chkxor);
	output(MPM_CFG_SUM,extr_chksum);
	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void mpm_cfg_load(idata word address)			// Load config data
{
	CFG_RAM(MPM_CFG);
	switch(address)	{
		case SENSOR_SU  :	i_data.sensor_su   = input(SENSOR_SU   );
							break;
		case DAY_CNT	:	i_data.day_cnt     = input(DAY_CNT     );
							break;
		case STATION_ID	:	i_data.station_id  = input(STATION_ID+1)     ;
							i_data.station_id |= input(STATION_ID  ) << 8;
							break;
		case PASSWORD	:	i_data.password    = input(PASSWORD + 1)     ;
							i_data.password   |= input(PASSWORD    ) << 8;
							break;
		case CALL_TIME	:	i_data.call_time   = input(CALL_TIME +1)     ;
							i_data.call_time  |= input(CALL_TIME   ) << 8;
							break;
		case SAVE_TIME	:	i_data.save_time   = input(SAVE_TIME +1)     ;
							i_data.save_time  |= input(SAVE_TIME   ) << 8;
							break;
	}
	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void memory_check(void)
{
	byte i;
	idata byte x, y, h, get_data;
	idata word pos;
	unsigned long ram_size, recode_size;
//	char *p;

	ram_size = 0;
	i_data.extr_addr = 0;
	x = 12;	y = 2;
	lcd_printf(0,y,1,"CFG  MEMORY");
	for(h = 0; h < 2;)	{
		CFG_RAM(h);
		get_data = input(i_data.extr_addr);
		output(i_data.extr_addr,0xAA);
		if(input(i_data.extr_addr) != 0xAA)	break;
		output(i_data.extr_addr,get_data);
		i_data.extr_addr += 64;
		if(i_data.extr_addr == 0)	h++;
		RAM_CS();
		if((i_data.extr_addr % 16384) == 0)	lcd_printf(x++,y,0,".");
		if(x > 19)	{
			x = 12;		lcd_printf(x,y,0,"        ");
		}
		WATCHDOG;
		ram_size += 64L;
	}
	RAM_CS();
	if(ram_size < 131072L)	{
		lcd_printf(12,y,0,": ERROR");
		delay(30000);
		disable(EA);	while(1);
	} else	{
		lcd_printf(12,y,0,": OK   ");
		delay(30000);
	}
	ram_size  = 0;
	i_data.extr_addr = 0;
	x = 12;	y = 3;
	lcd_printf(0,y,1,"DATA MEMORY");
	for(i_data.ram_x = 0; i_data.ram_x < 14;)	{
		DAT_RAM(i_data.ram_x);
		get_data = input(i_data.extr_addr);
		output(i_data.extr_addr,0xAA);
		if(input(i_data.extr_addr) != 0xAA)	break;
		output(i_data.extr_addr,get_data);
		i_data.extr_addr += 64;
		if(i_data.extr_addr == 0)	i_data.ram_x++;
		RAM_CS();
		if((i_data.extr_addr % 32768) == 0)	lcd_printf(x++,y,0,".");
		if(x > 19)	{
			x = 12;
			lcd_printf(x,y,0,"        ");
		}
		WATCHDOG;
		ram_size += 64L;
	}
	RAM_CS();
	value.ram_size   = ram_size;
	i_data.ram_limit = (unsigned char)(ram_size / 65536L);
	if(ram_size < 131072L)	{
		lcd_printf(12,y,0,": ERROR");
		delay(30000);
		disable(EA);	while(1);
	} else	{
		lcd_printf(12,y,0,": OK   ");
		i_data.extr_addr = ram_size / 1024L;
		delay(30000);
		scroll_up(2, 2, 1);
		x = 0;	y = 3;
		lcd_printf(x,y,1,"MEMORY SIZE : %-4d",i_data.extr_addr+128);
	}
	delay(30000);
	scroll_up(2, 2, 1);
	lcd_printf(x,y,1,"SENSOR LIMIT: %d",i_data.sensor_su);	// Sensor suu display
	delay(30000);
	scroll_up(2, 2, 1);
	recode_size = ((long)(i_data.sensor_su) * (long)(sizeof(sensor_field_t)) + 6L)
				* (long)(value.record_su);
	i_data.day_limit = (byte)(ram_size / recode_size);		// Day limit calculate
	if(i_data.day_limit > 0 )	i_data.day_limit -= 1;
	lcd_printf(x,y,1,"STORAGE DAY : %d",i_data.day_limit+1);// Storeage day display
	delay(30000);
	scroll_up(2, 2, 1);
	mpm_cfg_load(DAY_CNT);
	lcd_printf(x,y,1,"DAY POS     : %d",i_data.day_cnt);	// Day position display
	delay(30000);

	pos = i_data.day_cnt * 4;
	CFG_RAM(MPM_CFG);
	if((rtc.year  == input(INDEX_DB + pos     )) &&
	   (rtc.month == input(INDEX_DB + pos + 1 )) &&
	   (rtc.day   == input(INDEX_DB + pos + 2)))	{
		RAM_CS();
		scroll_up(2, 2, 1);
		lcd_printf(x,y,1,"SAM INITALL : '@'");
		delay(30000);
		for(i=0; i<i_data.sensor_su; i++)	{
			max_min_load(i);
			sam_sign_send(&com1, 0x00, i, sizeof(sam_t));
			sam_send(&com1, '@', REAL, i);					// 0x00 = Board id
		}
		for(i=0; i<i_data.sensor_su; i++)	{
			sam.send.mini = variable_load(OFFSET_BASE, i);
			sam_sign_send(&com1, 0x00, i, sizeof(sam_t));
			sam_send(&com1, '#', REAL, i);					// 0x00 = Board id
		}
	} else	{
		RAM_CS();
		scroll_up(2, 2, 1);
		lcd_printf(x,y,1,"SAM INITALL : 'I'");
		delay(30000);
		for(i=0; i<i_data.sensor_su; i++)	{
			sam_sign_send(&com1, 0x00, i, sizeof(sam_t));	// 0x00 = Board id
			sam_send(&com1, 'I', REAL, i);
			variable_save(SPECIAL_BASE, i, 0.0);
			variable_save(OFFSET_BASE, i, 0.0);
		}
		for(pos=0; pos<i_data.sensor_su * 8; pos++)	{		// Maxi & mini pointer clear
			CFG_RAM(MPM_CFG);
			output(MAX_MIN_BASE + pos, 0x00);
			RAM_CS();
		}
	}
	transmit_check__1();
//	sam_sign_send(&com1, RCM_ID, 0xFA, sizeof(rcm_t));
//	rcm.send.p12 = (float)(sport[COM2].baud);
//	rcm.send.m12 = (float)(sport[COM3].baud);
//	rcm.send.event[0] = sport[COM2].data_bit|sport[COM2].stop_bit|sport[COM2].parity;
//	rcm.send.event[0] = sport[COM3].data_bit|sport[COM3].stop_bit|sport[COM3].parity;
//	rcm_send(&com1, 0xFF6C);
 //	transmit_check__1();
	scroll_up(2, 2, 1);
	if(i_data.save_time < 60)	lcd_printf(x,y,1,"SAVE TIME   : %d SEC",i_data.save_time);
	else						lcd_printf(x,y,1,"SAVE TIME   : %d MIN",i_data.save_time / 60);
	delay(30000);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void buffer_clear(void)
{
	idata byte x = 0, y = 1;

	lcd_clear();
	i_data.extr_addr = 0;
	lcd_printf(0,0,1,"[-- BUFFER CLEAR --]");
	sport_cfg_load();
	for(i_data.ram_x = 0; i_data.ram_x < i_data.ram_limit+2;)	{
		CFG_RAM(i_data.ram_x);
		output(i_data.extr_addr,0x00);
		RAM_CS();
		if(++i_data.extr_addr == 0)	{
			i_data.ram_x++;
		}
		if((i_data.extr_addr % 16384) == 0) lcd_printf(x++,y,0,".");
		if(x > 19)	{	x = 0;	y++;	}
		WATCHDOG;
	}
	sport_cfg_save(COM1);
	sport_cfg_save(COM2);
	sport_cfg_save(COM3);
	lcd_clear();
	lcd_printf(0,1,1,"   BUFFER CLEAR OK  ");
	lcd_printf(0,2,1,"       REBOOT !!    ");
	delay(30000);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void put_data(idata byte put_d)
{
	DAT_RAM(i_data.ram_x);
	output(i_data.extr_addr++,put_d);
	RAM_CS();
	if(i_data.extr_addr == 0)		i_data.ram_x++;
	if(i_data.ram_x > i_data.ram_limit)	i_data.ram_x = 0;
	WATCHDOG;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void save_data(idata byte b)
{
	idata unsigned char yy, mm, dd;
		  unsigned char hh, bb, ss;
	idata unsigned int  i;
		  unsigned char *p = (unsigned char *)(weather[b].sensor);
		  unsigned int  offset;

	WATCHDOG;
	if((save.hour == 0) && (save.min == 0) && (save.sec == 0))	{
		yy = value.old_year;	mm = value.old_month;	dd = value.old_day;
		hh = 24;				bb = 0;					ss =  0;
	} else	{
		yy = save.year;		mm = save.month;	dd = save.day;
		hh = save.hour;		bb = save.min;		ss = save.sec;
		value.old_year  = yy;
		value.old_month = mm;
		value.old_day   = dd;
	}
	weather[b].year = yy;	weather[b].month = mm;
	weather[b].day  = dd;	weather[b].hour  = hh;
	weather[b].min  = bb;	weather[b].sec   = ss;

	if(hh == 24)	hh = 0;
	offset = (word)((((long)(hh) * 3600L) + ((long)(bb) * 60L) + (long)(ss)) / (long)(i_data.save_time));
	mpm_cfg_load(DAY_CNT);
	i = i_data.day_cnt * 4;
	CFG_RAM(MPM_CFG);
	if((yy != input(INDEX_DB + i)) || (mm != input(INDEX_DB + i + 1))
								   || (dd != input(INDEX_DB + i + 2)))	{
		RAM_CS();
		i_data.day_cnt++;
		if(i_data.day_cnt > i_data.day_limit)	i_data.day_cnt = 0;
		i = i_data.day_cnt * 4;
		CFG_RAM(MPM_CFG);
		output(INDEX_DB + i    , yy);
		output(INDEX_DB + i + 1, mm);
		output(INDEX_DB + i + 2, dd);
		output(INDEX_DB + i + 3, i_data.day_cnt);	// Save Day count.
	}
	RAM_CS();
	mpm_cfg_save(DAY_CNT);
	value.day_pos = ((value.weather_field * (long)(value.record_su)) * (long)(i_data.day_cnt))
				   + (value.weather_field * (long)(offset));
	i_data.ram_x     = (byte)(value.day_pos / 65536L);
	i_data.extr_addr = (word)(value.day_pos % 65536L);
	put_data(weather[b].year);	put_data(weather[b].month);	put_data(weather[b].day);
	put_data(weather[b].hour);	put_data(weather[b].min)  ;	put_data(weather[b].sec);
	for(i=0; i<i_data.sensor_su * sizeof(sensor_field_t); i++)	put_data(p[i]);
	for(i=0; i<i_data.sensor_su; i++)	{
		if(weather[SAVE].sensor[i].sensor_code == Snow)	{
		variable_save(OFFSET_BASE, i, weather[SAVE].sensor[i].aver);
		}
	}
	if(weather[b].hour == 24)	{
		for(i=0; i<i_data.sensor_su; i++)	{
			sam_sign_send(&com1, 0x00, i, sizeof(sam_t));
			sam_send(&com1, 'I', REAL, i);
			if(weather[SAVE].sensor[i].sensor_code == Precipi)	{
				sam.send.mini = weather[SAVE].sensor[i].maxi;
				sam.send.maxi = 0;
				sam_sign_send(&com1, 0x00, i, sizeof(sam_t));
				sam_send(&com1, '@', REAL, i);
			} else	if(weather[SAVE].sensor[i].sensor_code == Snow)	{
				float snow_val;
				snow_val = variable_load(SPECIAL_BASE, i);
				variable_save(SPECIAL_BASE, i, weather[SAVE].sensor[i].aver);
				sam.send.mini = weather[SAVE].sensor[i].aver - snow_val;
				if(sam.send.mini <= 0.0)	sam.send.mini = 0.0;
				sam_sign_send(&com1, 0x00, i, sizeof(sam_t));
				sam_send(&com1, '@', REAL, i);
			}
			transmit_check__1();
		}
		dialup_modem_init();
//		sam_sign_send(&com1, RCM_ID, 0xFA, sizeof(rcm_t));
//		rcm.send.p12 = (float)(sport[COM2].baud);
//		rcm.send.m12 = (float)(sport[COM3].baud);
//		rcm.send.event[0] = sport[COM2].data_bit|sport[COM2].stop_bit|sport[COM2].parity;
//		rcm.send.event[0] = sport[COM3].data_bit|sport[COM3].stop_bit|sport[COM3].parity;
//		rcm_send(&com1, 0xFF6C);
//		transmit_check__1();
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char get_data(void)
{
	idata byte get_d;
	DAT_RAM(i_data.ram_x);
	get_d = input(i_data.extr_addr++);
	RAM_CS();
	if(i_data.extr_addr == 0)	i_data.ram_x++;
	if(i_data.ram_x > i_data.ram_limit)	i_data.ram_x = 0;
	WATCHDOG;
	return(get_d);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
word load_data(idata byte b)
{
	idata unsigned char yy, mm, dd;
		  unsigned char ys, hh, bb, ss, flag = 0;
	idata unsigned int  i, j;
		  unsigned char *p = (unsigned char *)(weather[b].sensor);
		  unsigned int  offset;

	yy = host_com2.receive.year;	mm = host_com2.receive.month;
	dd = host_com2.receive.day ;	hh = host_com2.receive.hour ;
	bb = host_com2.receive.min ;	ss = host_com2.receive.sec  ;
	ys = host_com2.receive.hour;
	if(hh == 24)	ys = 0;
	offset = (word)((((long)(ys) * 3600L) + ((long)(bb) * 60L) + (long)(ss)) / (long)(i_data.save_time));
	for(j=0; j<=i_data.day_limit*4; j+=4)	{		// INDEX_DB[4] = yy,mm,dd,i_data.day_cnt.
		CFG_RAM(MPM_CFG);
		if((yy == input(INDEX_DB+j)) && (mm == input(INDEX_DB+j+1))
									 && (dd == input(INDEX_DB+j+2)))	{
			i_data.day_cnt = input(INDEX_DB+j+3);	// Load day point
			RAM_CS();
			flag = 1;
			value.day_pos = ((value.weather_field * (long)(value.record_su)) * (long)(i_data.day_cnt))
						   + (value.weather_field * (long)(offset));
			i_data.ram_x     = (byte)(value.day_pos / 65536L);
			i_data.extr_addr = (word)(value.day_pos % 65536L);
			weather[b].year  = get_data();	weather[b].month = get_data();
			weather[b].day   = get_data();	weather[b].hour  = get_data();
			weather[b].min   = get_data();	weather[b].sec   = get_data();
			if((weather[b].year == yy) && (weather[b].month == mm) &&
			   (weather[b].day  == dd) && (weather[b].hour  == hh) &&
			   (weather[b].min  == bb) && (weather[b].sec   == ss))	{
				for(i=0; i<i_data.sensor_su * sizeof(sensor_field_t); i++)	p[i] = get_data();
				return(i_data.save_time);
			}
		} WATCHDOG;	RAM_CS();
	}
	if(flag)	{
		weather[b].year = weather[b].month = weather[b].day = 0xCC;
		weather[b].hour = weather[b].min   = weather[b].sec	= 0xCC;
		return(i_data.save_time);
	} else	{
		weather[b].year = weather[b].month = weather[b].day = 0xCC;
		weather[b].hour = weather[b].min   = weather[b].sec = 0xCC;
		return(0xFF58);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void max_min_save(void)
{
	idata byte maxi, mini, sen_id, i;
	char *p;

	for(sen_id=0; sen_id<i_data.sensor_su; sen_id++)	{
		for(i=0; i<4; i++)	{
			p = (char *)(&weather[REAL].sensor[sen_id].maxi);
			maxi = p[i];
			CFG_RAM(MPM_CFG);
			output(MAX_MIN_BASE + (sen_id * 8)     + i,maxi);	RAM_CS();
			p = (char *)(&weather[REAL].sensor[sen_id].mini);
			mini = p[i];
			CFG_RAM(MPM_CFG);
			output(MAX_MIN_BASE + (sen_id * 8) + 4 + i,mini);	RAM_CS();
		}	WATCHDOG;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void max_min_load(idata byte sen_id)
{
	idata byte maxi, mini, i;
	char *p;

	for(i=0; i<4; i++)	{
		CFG_RAM(MPM_CFG);
		maxi = input(MAX_MIN_BASE + (sen_id * 8)     + i);	RAM_CS();
		p = (char *)(&sam.send.maxi);
		p[i] = maxi;
		CFG_RAM(MPM_CFG);
		mini = input(MAX_MIN_BASE + (sen_id * 8) + 4 + i);	RAM_CS();
		p = (char *)(&sam.send.mini);
		p[i] = mini;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void variable_save(idata word base, idata byte sen_id, idata float v)
{
	float val = v;
	idata byte vv, i;
	char *p;

	p = (char *)(&val);
	for(i=0; i<4; i++)	{
		vv = p[i];
		CFG_RAM(MPM_CFG);
		output(base + (sen_id * 4) + i, vv);	RAM_CS();
		WATCHDOG;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
float variable_load(idata word base, idata byte sen_id)
{
	float val;
	idata byte vv, i;
	char *p;

	p = (char *)(&val);
	for(i=0; i<4; i++)	{
		CFG_RAM(MPM_CFG);
		vv = input(base + (sen_id * 4) + i);	RAM_CS();
		p[i] = vv;
	}
	return(val);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void poll_real_data(void)
{
	WATCHDOG;
	real.again_count = 1;
	if(real.call_id < i_data.sensor_su)	{
		while(real.again_count)	{
			real.again_count--;
			real.receive_status = OFF;
			sam_sign_send(&com1, 0x00, real.call_id, sizeof(sam_t));
			sam_send(&com1, 'W', REAL, real.call_id);
			real.delay_count = 0;
			while(real.delay_count < 300)	{
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
				if(real.receive_status == ON)	{
					board_status_set(real.call_id, NORMAL);
					real.again_count = 0;	break;
				}
			}
		}
		if(real.receive_status == OFF)	{
			weather[REAL].sensor[real.call_id].sensor_st   =
			weather[REAL].sensor[real.call_id].sensor_code =
			weather[REAL].sensor[real.call_id].sensor_id   = 0xEF;
			board_status_set(real.call_id, FAIL);
			sensor_status_set(real.call_id, FAIL);
		}
		real.call_id++;
	} else	{
		real.call_id = 0;	real.flag = OFF;
		max_min_save();
		rcm_poll.again_count = 1;
		while(rcm_poll.again_count)	{
			rcm_poll.again_count--;
			rcm_poll.receive_status = OFF;
			sam_sign_send(&com1, RCM_ID, 0xFA, sizeof(rcm_t));
			rcm_send(&com1, 0xFF4C);
			rcm_poll.delay_count = 0;
			while(rcm_poll.delay_count < 400)	{
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
				if(rcm_poll.receive_status == ON)	{
					rcm.send.bod_sta[0] &= 0xFD;		// RCM Status NORMAL
					rcm_poll.again_count = 0;	break;
				} else	rcm.send.bod_sta[0] |= 0x02;	// RCM Status FAIL
			}
		}
		send_led_data(&com1);
		send_led_data(&com2);
		send_led_data(&com3);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void poll_call_data(void)
{
	WATCHDOG;
	call.again_count = 1;
	if(call.call_id < i_data.sensor_su)	{
		while(call.again_count)	{
			call.again_count--;
			call.receive_status = OFF;
			sam_sign_send(&com1, 0x00, call.call_id, sizeof(sam_t));
			sam_send(&com1, 'W', i_data.call_time, call.call_id);
			call.delay_count = 0;
			while(call.delay_count < 300)	{
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
				if(call.receive_status == ON)	{
					call.again_count = 0;	break;
				}
			}
		}
		if(call.receive_status == OFF)	{
			weather[CALL].sensor[call.call_id].sensor_st   =
			weather[CALL].sensor[call.call_id].sensor_code =
			weather[CALL].sensor[call.call_id].sensor_id   = 0xEF;
		}
		call.call_id++;
	} else	{
		call.call_id = 0;	call.flag = OFF;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void poll_save_data(void)
{
	WATCHDOG;
	save.again_count = 2;
	if(save.call_id < i_data.sensor_su)	{
		while(save.again_count)	{
			save.again_count--;
			save.receive_status = OFF;
			sam_sign_send(&com1, 0x00, save.call_id, sizeof(sam_t));
			sam_send(&com1, 'W', i_data.save_time, save.call_id);
			save.delay_count = 0;
			while(save.delay_count < 300)	{
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
				if(save.receive_status == ON)	{
					save.again_count = 0;	break;
				}
			}
		}
		if(save.receive_status == OFF)	{
			weather[SAVE].sensor[save.call_id].sensor_st   =
			weather[SAVE].sensor[save.call_id].sensor_code =
			weather[SAVE].sensor[save.call_id].sensor_id   = 0xEF;
		}
		save.call_id++;
	} else	{
		save.call_id = 0;	save.flag = OFF;

		save_data(SAVE);

		if(value.delay_count > 1800)	{
			value.delay_count = 0;
			key.key_data[key.key_pos_st++] = ESC1;
			if(key.key_pos_st > 20)	key.key_pos_st = 0;
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sensor_status_set(byte call_id, byte status)
{
	byte	i, arrary_mok, arrary_na, error_check = 0x01;

	if(call_id == 0)	{
		arrary_mok = 0;
		arrary_na  = 0;
	} else	{
		arrary_mok = call_id / 7;
		arrary_na  = call_id % 7;
	}
	i = (error_check << arrary_na);
	if(status & 0x02)	rcm.send.sen_sta[arrary_mok] |=  i;
	else				rcm.send.sen_sta[arrary_mok] &= ~i;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void board_status_set(byte call_id, byte status)
{
	byte	i, arrary_mok, arrary_na, error_check = 0x01;

	call_id += 2;
	arrary_mok = call_id / 7;
	arrary_na  = call_id % 7;
	i = (error_check << arrary_na);
	if(status)	rcm.send.bod_sta[arrary_mok] |=  i;
	else		rcm.send.bod_sta[arrary_mok] &= ~i;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rtc_set(void)
{
	disable(EA);
	rtc.year  = host_com2.receive.year;	rtc.month = host_com2.receive.month;
	rtc.day   = host_com2.receive.day ;	rtc_date_write(&rtc);
	disable(EA);
	rtc.hour  = host_com2.receive.hour;	rtc.min   = host_com2.receive.min;
	rtc.sec   = host_com2.receive.sec ;	rtc_time_write(&rtc);
	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void delay(word count)
{
	word comp;
	for(comp=0; comp<count; comp++)		WATCHDOG;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
idata unsigned char keyinput(void)
{
	byte return_key = 0x70;

	WATCHDOG;
	receive_check__1();	transmit_check__1();
	receive_check_23();	transmit_check_23();
	if(real.flag == ON)	poll_real_data();
	receive_check_23();	transmit_check_23();
	if(call.flag == ON)	poll_call_data();
	receive_check_23();	transmit_check_23();
	if(save.flag == ON)	poll_save_data();
	receive_check_23();	transmit_check_23();
	WATCHDOG;
	if(key.key_pos_st != key.key_pos_ed)	{
		value.delay_count = 0;
		return_key = key.key_data[key.key_pos_ed++];
		if(key.key_pos_ed > 20)	key.key_pos_ed = 0;
	}
	receive_check_23();	transmit_check_23();
	return(return_key);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main(void)
{
	iRAM_CS();
	RST_CS();	output(0x00,0xFF);	iRAM_CS();
	BUZZER_OFF;
	led_con(COM1,TX, ON); delay(10000);	led_con(COM1,TX,OFF); delay(500);
	led_con(COM2,TX, ON); delay(10000);	led_con(COM2,TX,OFF); delay(500);
	led_con(COM3,TX, ON); delay(10000);	led_con(COM3,TX,OFF); delay(500);
	led_con(COM1,RX, ON); delay(10000);	led_con(COM1,RX,OFF); delay(500);
	led_con(COM2,RX, ON); delay(10000);	led_con(COM2,RX,OFF); delay(500);
	led_con(COM3,RX, ON); delay(10000);	led_con(COM3,RX,OFF);
	rtc_init();							// RTCC(MC146818) initiliaze.
	init_lcd();							// initialize LCD(20 Chars * 4 Line)
	i_data.rtu_id = read_rtu_id();
	mpm_cfg_init();
	lcd_clear();
	lcd_printf(0,0,1,"[- SELF CHECKING --]" );
	lcd_printf(0,1,1,"RTU ID      : %02d",i_data.rtu_id);

	menu_sensor_table_init();
	menu_initliaze();
	initial_weather(&weather[REAL]);	initial_weather(&weather[CALL]);
	initial_weather(&weather[SAVE]);	initial_weather(&weather[SEND]);
	led_table = (led_t *)(malloc(sizeof(led_t) * (i_data.sensor_su + 2)));

	variable_initial();					// Variable initialize.
	memory_check();
	value.weather_field = 6L + ((long)(sizeof(sensor_field_t)) * (long)(i_data.sensor_su));
	BUZZER_ON;	value.buzzer_cnt = 60;		delay(8000);
	BUZZER_ON;	value.buzzer_cnt = 60;
	dialup_modem_init();
	host_sign_send_com2(&com2, MPM_ID, ACK_SEND, RESET);
	lcd_clear();
	main_loop(&main_menu);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
