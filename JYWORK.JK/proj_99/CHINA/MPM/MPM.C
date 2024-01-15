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
#define	SPORT_CFG		MPM_CFG_END	 +  10	// Serial port config(100 byte)
#define INDEX_DB		SPORT_CFG    + 110	// Index DB(400 byte)
#define	MAX_MIN_BASE	INDEX_DB     + 410	// Weather min & max storage DB
//[*]---------------------------------------------------------------------[*]
#define	SAM_CFG_BASE	0x0010				// SAM Configration base address
#define SAM_CFG_CHK1	SAM_CFG_BASE    	// Default 0xAA
#define SAM_CFG_CHK2	SAM_CFG_CHK1 +   1	// Default 0xAA
//??????????????????
#define SAM_CFG_XOR		SAM_CFG_CHK2 + 200	// Checksum xor
#define SAM_CFG_SUM		SAM_CFG_XOR  +   1	// Checksum sum
#define SAM_CFG_END		SAM_CFG_XOR			// Configration end address
//[*]---------------------------------------------------------------------[*]
#define	iram_cs()		{ P1 = 0x00;				}
#define	ram_cs()		{ P1 = 0x00;	EA =    1;	}
#define	rtc_cs()		{ EA =    0;	P1 = 0x02;	}
#define	sio2_cs()		{ EA =    0;	P1 = 0x04;	}
#define	sio3_cs()		{ EA =    0;	P1 = 0x06;	}
#define	led_cs()		{ EA =    0;	P1 = 0x08;	}
#define	jmp_cs()		{ EA =    0;	P1 = 0x0A;	}

#define	cfg_ram_cs(a)	{ EA =    0;	P1 = 0x10 + a; }// 0 = mpm, 1 = sam
#define	dat_ram_cs(b)	{ EA =    0;	P1 = 0x12 + b; }// data save ram
//[*]----------------------------------------------------------------------[*]
#define	LED_ADD			0x00				// LED Base address
//[*]---------------------------------------------------------------------[*]
#define	MPM_CFG			0
#define	SAM_CFG			1
#define	MAIN  			1
#define	SUB   			0
#define	MPM_ID			0x00
#define	RCM_ID			0x01
//[*]---------------------------------------------------------------------[*]
#define	COM1			1
#define	COM2			2
#define	COM3			3
#define	RXD				4
#define	TXD				5
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
#define	DEL_TIME		10
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct led_indicator__t	{
	byte	tx2		:1;
	byte	rx2		:1;
	byte	tx1		:1;
	byte	rx1		:1;
	byte	tx3		:1;
	byte	rx3		:1;
	byte	not_used:2;
}	led_indicator_t;

typedef union led__u	{
	led_indicator_t	bits;
	byte			led_dt;
}	led_u;

static	idata led_u	led;

//[*]----------------------------------------------------------------------[*]
typedef struct sensor_field__t	{
	byte			gubun;
	byte			sensor_status;
	byte			sensor_code;
	byte			sensor_id;
	word			aver;
	word			spec;
	word			mini;
	word			maxi;
}	sensor_field_t;

typedef struct weather__t	{
	word			header;
	word			station_id;
	byte			cmd;
	byte			year;
	byte			month;
	byte			day;
	byte			hour;
	byte			min;
	byte			sec;
	sensor_field_t	*sensor;
	byte			chk_xor;
	byte			chk_add;
	word			tail;
}	weather_t;

static weather_t	weather[4];

//[*]---------------------------------------------------------------------[*]
typedef struct	idt__t	{
	unsigned char	buzzer_cnt;
	unsigned char	rtu_id;
	unsigned char	sensor_su;
	unsigned char	day_cnt;
	unsigned char	ram_x;
	unsigned char	ram_limit;
	unsigned char	day_limit;
	unsigned int	record_su;
	unsigned int	extr_addr;
	unsigned int	password;
	unsigned int	station_id;
	unsigned int	call_time;
	unsigned int	save_time;
}	idt_t;

static idata idt_t	i_data;

//[*]---------------------------------------------------------------------[*]
typedef struct	cmp__t	{
	unsigned char	flag;
	unsigned char	again_count;
	unsigned char	call_id;
	unsigned char	receive_status;
	unsigned char	delay_count;
}	cpm_t;

static cpm_t		cpm_st;

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
	unsigned char	real_command;
	unsigned char	call_id;
	unsigned char	receive_status;
	unsigned char	delay_count;
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
	unsigned char	call_command;
	unsigned char	call_id;
	unsigned char	receive_status;
	unsigned char	again_count;
	unsigned char	delay_count;
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
	unsigned char	save_command;
	unsigned char	call_id;
	unsigned char	save_jugi;
	unsigned char	receive_status;
	unsigned char	again_count;
	unsigned char	delay_count;
}	save_t;

static save_t		save;

//[*]---------------------------------------------------------------------[*]
typedef struct	rcm_poll__t	{
	unsigned char	delay_count;
	unsigned int	check_count;
	unsigned int	again_count;
	unsigned int	receive_status;
}	rcm_poll_t;

static rcm_poll_t	rcm_poll;

//[*]---------------------------------------------------------------------[*]
typedef struct	value__t	{
	unsigned char	old_year;
	unsigned char	old_month;
	unsigned char	old_day;
	unsigned char	factory;
	unsigned char	cfg_poll_flag;
	unsigned char	config_receive_status;
	unsigned char	cancel;
	unsigned char	buffer_clr;
	unsigned char	memory_tst;
	unsigned long	day_pos;
	unsigned long	ram_size;
	unsigned long	weather_field;
}	value_t;

static value_t		value;

static unsigned char	BUFFER[20];

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void led_disp(idata byte ch, idata byte sel, idata byte sw);
idata byte jumper_read(void);
void initial_weather(weather_t *weather);
void variable_initial(void);
void mpm_cfg_init(void);
void schedule_initial(void);
void memory_check(void);
void rtu_run(void);
void put_data(idata byte put_d);
void save_data(idata byte b);
char get_data(void);
char load_data(idata byte b);
void max_min_save(void);
void max_min_load(idata byte id);
void board_status_set(byte call_id, byte flag_status);
void sensor_status_set(byte call_id, byte sta);
void mpm_cfg_save(idata word address);			// MPM Configration save
void mpm_cfg_load(idata word address);			// Load config data
void memory_test(void);
void buffer_clear(void);
void poll_real_data(void);
void poll_call_data(void);
void poll_save_data(void);
void rtc_set(void);
void delay(word count);
void main(void);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#include "rtc.hpp"
#include "init&int.hpp"
#include "default.hpp"
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void led_disp(idata byte ch, idata byte sel, idata byte sw)
{
	idata byte led_data = 0;

	switch(ch)	{
		case	COM1:	if(sel == TXD)	led.bits.tx1 = sw;
						else 			led.bits.rx1 = sw;		break;
		case	COM2:	if(sel == TXD)	led.bits.tx2 = sw;
						else 			led.bits.rx2 = sw;		break;
		case	COM3:	if(sel == TXD)	led.bits.tx3 = sw;
						else 			led.bits.rx3 = sw;		break;
	}
	led_data = led.led_dt;
	led_cs();	output(0x00, led_data);		ram_cs();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
idata byte jumper_read(void)
{
	idata byte jump_id, chk_id, count = 0;

	jmp_cs();			jump_id = input(0x00);	ram_cs();
	while(1)	{
		jmp_cs();		chk_id  = input(0x00);	ram_cs();
		if(jump_id == chk_id)	count++;
		else	{
			count = 0;
			jmp_cs();	jump_id = input(0x00);	ram_cs();
		}
		if(count > 10)	break;
	}
	return (~jump_id);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void initial_weather(weather_t *weather)
{
	weather->header = 0xFFFFFFFF;
	weather->sensor	= (sensor_field_t *)(malloc(sizeof(sensor_field_t) * i_data.sensor_su));
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void variable_initial(void)
{
	idata word i, j;

	value.buffer_clr = OFF;
	i_data.extr_addr = 0x0000;
	led.led_dt       = 0x00;

	real.call_id = call.call_id = save.call_id = 0;
	real.flag    = call.flag    = save.flag    = 0;

	rtc.check_count = rtc.sqw_status_count = 0;

	for(i=0; i<4; i++)		{
		status.rtu.sen_board[i] = 0xFF;
		status.rtu.sen_sta[i]	= 0xFF;
	}
	for(i=0; i<sizeof(volt_data_u); i++)	status.rtu.volt.bytes[i] = 0x00;
	status.rtu.volt_sta.word_data		= 0xFFFF;
	status.rtu.cnt_board.bits.nc		= 0x0F;
	status.rtu.cnt_board.bits.rcm		= NORMAL;
	status.rtu.cnt_board.bits.mpm		= NORMAL;
	status.rtu.cnt_board.bits.cpm		= NORMAL;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void mpm_cfg_init(void)
{
	idata byte extr_chkxor, extr_chksum, flag;
	idata word i;
		  byte re_count =   1;
		  byte temt_sec = 100;

	sprintf(BUFFER, "CFG MEMORY CHECK");
	message_send(&com3, BUFFER);	delay(30000);
	extr_chkxor = extr_chksum = 0xAA;
	flag = 0;
	cfg_ram_cs(MPM_CFG);
	if((input(MPM_CFG_CHK1) != extr_chkxor) || (input(MPM_CFG_CHK2) != extr_chkxor))	flag = 1;
	else	{
		for(i=MPM_CFG_BASE; i<MPM_CFG_END; i++)	{
			extr_chkxor ^= input(i);
			extr_chksum += input(i);
		}
		if((extr_chkxor != input(MPM_CFG_XOR)) || (extr_chksum != input(MPM_CFG_SUM)))	flag = 2;
	}
	ram_cs();
	if		  (flag == 1)	sprintf(BUFFER, "CFG HEADER ERROR");
	else	if(flag == 2)	sprintf(BUFFER, "CFG DATA ERROR  ");
	else					sprintf(BUFFER, "CONFIG MEMORY OK");
	message_send(&com3, BUFFER);	delay(30000);

	if(flag)	{
		value.config_receive_status = OFF;
		while(value.config_receive_status == OFF)	{
			if((rtc.sec % 5 == 0) && (rtc.sec != temt_sec))	{
				temt_sec = rtc.sec;
				status_send_to_rcm(&com1);
				send_to_host_ack(&com2,'N');
				sprintf(BUFFER, "DATA REQUEST....");
				message_send(&com3, BUFFER);
				re_count++;
			}
			receive_check__1();	transmit_check__1();
			receive_check_23();	transmit_check_23();
			if((jumper_read() & 0x80) || (re_count > 60))	{
				sprintf(BUFFER, "FACTORY SET.....");
				message_send(&com3, BUFFER);	delay(50000);
				factory_set();	break;
			}
		}
		if(value.config_receive_status == ON)	{
			sprintf(BUFFER, "CFG DATA RECEIVE");
			message_send(&com3, BUFFER);		delay(30000);
		}
	}
	value.config_receive_status = ON;
	mpm_cfg_load(SENSOR_SU );	mpm_cfg_load(DAY_CNT  );
	mpm_cfg_load(STATION_ID);	mpm_cfg_load(PASSWORD );
	mpm_cfg_load(CALL_TIME );	mpm_cfg_load(SAVE_TIME);
	schedule_initial();
	memory_check();
	rtu_run();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void schedule_initial(void)
{
	if(i_data.call_time != 0)	{
		call.call_min = (byte)(i_data.call_time / 60);
		call.call_sec = (byte)(i_data.call_time % 60);
		switch(i_data.call_time)	{
			case	10:	call.call_command = 't';	break;
			case	60:	call.call_command = 'O';	break;
			case   300:	call.call_command = 'm';	break;
			case   600:	call.call_command = 'T';	break;
		}
	} else	{
		call.call_min     = 100;
		call.call_sec     = 100;
		call.call_command = 'X';				// Call data none
	}
	if(i_data.save_time != 0)	{
		save.save_min = (byte)(i_data.save_time / 60);
		save.save_sec = (byte)(i_data.save_time % 60);
		switch(i_data.save_time)	{
			case	10:	save.save_command = 't';	break;
			case	60:	save.save_command = 'O';	break;
			case   300:	save.save_command = 'm';	break;
			case   600:	save.save_command = 'T';	break;
		}
	} else	{
		save.save_min = 100;
		save.save_sec = 100;
		save.save_command = 'X';				// Save data none
	}
	real.real_command = 'M';
	i_data.record_su  = (word)(86400 / i_data.save_time);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void memory_check(void)
{
	idata byte h, get_data;
	idata word pos;
	unsigned long ram_size, recode_size;
	char *p;

	ram_size = 0;
	i_data.extr_addr = 0;
	sprintf(BUFFER, "CFG MEMORY CHECK");
	message_send(&com3, BUFFER);		delay(30000);
	for(h = 0; h < 2;)	{
		cfg_ram_cs(h);
		get_data = input(i_data.extr_addr);
		output(i_data.extr_addr,0xAA);
		if(input(i_data.extr_addr) != 0xAA)		break;
		output(i_data.extr_addr,get_data);
		i_data.extr_addr += 64;
		if(i_data.extr_addr == 0)	h++;
		ram_cs();
		ram_size += 64L;
	}
	ram_cs();
	if(ram_size < 131072L)	{
		sprintf(BUFFER, "CFG MEMORY FAIL!");
		message_send(&com3, BUFFER);
		while(1);
	} else	{
		sprintf(BUFFER, "CONFIG MEMORY OK");
		message_send(&com3, BUFFER);	delay(30000);
	}
	sprintf(BUFFER, "DATA MEMORY CHK ");
	message_send(&com3, BUFFER);		delay(30000);
	ram_size = 0;
	i_data.extr_addr = 0;
	for(i_data.ram_x = 0; i_data.ram_x < 14;)	{
		dat_ram_cs(i_data.ram_x);
		get_data = input(i_data.extr_addr);
		output(i_data.extr_addr,0xAA);
		if(input(i_data.extr_addr) != 0xAA)	break;
		output(i_data.extr_addr,get_data);
		i_data.extr_addr += 64;
		if(i_data.extr_addr == 0)	i_data.ram_x++;
		ram_cs();
		ram_size += 64L;
	}
	ram_cs();
	value.ram_size   = ram_size;
	i_data.ram_limit = (unsigned char)(ram_size / 65536L);
	if(ram_size < 131072L)	{
		sprintf(BUFFER, "DATA MEMORY FAIL");
		message_send(&com3, BUFFER);
		while(1);
	} else	{
		i_data.extr_addr = ram_size / 1024L;
		sprintf(BUFFER, "MEMORY %4dKB OK",i_data.extr_addr+128);
		message_send(&com3, BUFFER);	delay(30000);
	}
	recode_size = (unsigned long)((6 + (i_data.sensor_su * sizeof(sensor_field_t))) * i_data.record_su);
	i_data.day_limit = (char)((ram_size / recode_size) - 1);// Day limit calculate
	sprintf(BUFFER, "STORAGE %3d DAY ",i_data.day_limit);	// Storeage day display
	message_send(&com3, BUFFER);		delay(30000);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rtu_run(void)
{
	byte i;
	idata word pos;
	unsigned long ram_size, recode_size;
	char *p;

	mpm_cfg_load(DAY_CNT);
	pos = i_data.day_cnt * 4;
	cfg_ram_cs(MPM_CFG);
	if((rtc.year  == input(INDEX_DB + pos     )) &&
	   (rtc.month == input(INDEX_DB + pos + 1 )) &&
	   (rtc.day   == input(INDEX_DB + pos + 2)))	{
		ram_cs();
		sprintf(BUFFER, "SAM INITALL :'@'");
		message_send(&com3, BUFFER);	delay(20000);
		for(i=0; i<i_data.sensor_su; i++)	{
			max_min_load(i);
			send_to_sam_call(&com1, i, '@');
			transmit_check__1();
		}
	} else	{
		ram_cs();
		sprintf(BUFFER, "SAM INITALL :'I'");
		message_send(&com3, BUFFER);	delay(20000);
		for(i=0; i<i_data.sensor_su; i++)	{
			send_to_sam_call(&com1, i, 'I');
			transmit_check__1();
		}
		for(pos=0; pos<i_data.sensor_su * 4; pos++)	{	// Maxi & mini pointer clear
			cfg_ram_cs(MPM_CFG);
			output(MAX_MIN_BASE + pos, 0x00);
			ram_cs();
		}
	}
	sprintf(BUFFER, "DAY POS : %d",i_data.day_cnt);		// Day position display
	message_send(&com3, BUFFER);		delay(30000);
	send_to_host_ack(&com2,'R');
	cpm_run_send();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void memory_test(void)
{
	idata word	ext_addr = 0;
	idata byte	get_data, count;
	unsigned long ram_size = 0;

	sprintf(BUFFER, "MEMORY TEST.....");
	message_send(&com3, BUFFER);	delay(30000);
	value.memory_tst = value.cancel = OFF;
	for(count=0; count<=i_data.ram_limit+2;)	{
		cfg_ram_cs(count);
		get_data = input(ext_addr);		output(ext_addr, 0xAA);
		if(input(ext_addr) != 0xAA)		break;
		output(ext_addr, get_data);		ext_addr ++;
		if(ext_addr == 0)	count++;
		ram_cs();
		ram_size++;
		if(!(ext_addr % 1024))	{
			sprintf(BUFFER, "MEMORY : %4dKB",(int)(ram_size/1024));
			message_send(&com3, BUFFER);	delay(30000);
		}
		if(value.cancel==ON)			break;
		if(real.flag == ON)		poll_real_data();
		if(call.flag == ON)		poll_call_data();
		if(save.flag == ON)		poll_save_data();
		receive_check__1();		transmit_check__1();
		receive_check_23();		transmit_check_23();
	}
	ram_cs();
	if(ram_size >= value.ram_size)	{
		status.rtu.cnt_board.bits.mem = NORMAL;
		sprintf(BUFFER, "%4dKB OK",(int)(ram_size/1024));
		message_send(&com3, BUFFER);	delay(60000);
	} else	if(value.cancel == OFF)	{
		sprintf(BUFFER, "MEMORY ERROR !!!");
		message_send(&com3, BUFFER);	delay(60000);
	}
	value.memory_tst = value.cancel = OFF;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void buffer_clear(void)
{
	idata word ext_addr = 0;
	idata byte ram_pos;

	sprintf(BUFFER, "BUFFER CLEAR....");
	message_send(&com3, BUFFER);
	for(ram_pos = 0; ram_pos < 2;)	{
		cfg_ram_cs(ram_pos);
		output(ext_addr, 0x00);
		ext_addr++;
		if(ext_addr == 0)	ram_pos++;
	}
	ram_cs();
	sprintf(BUFFER, "BUFFER CLEAR OK!");
	message_send(&com3, BUFFER);	delay(40000);
	send_to_host_ack(&com2,'n');
	send_to_cpm_cfg(&com3);
	sprintf(BUFFER, "PLEASE REBOOT !!");
	message_send(&com3, BUFFER);
	while(1);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void max_min_save(void)
{
	idata word mini, maxi;
	idata byte id;

	for(id=0; id<i_data.sensor_su; id++)	{
		maxi = weather[REAL].sensor[id].maxi;
		mini = weather[REAL].sensor[id].mini;
		cfg_ram_cs(MPM_CFG);
		  output(MAX_MIN_BASE + (id * 4)    , (byte)(maxi >> 8));
		  output(MAX_MIN_BASE + (id * 4) + 1, (byte)(maxi     ));
		  output(MAX_MIN_BASE + (id * 4) + 2, (byte)(mini >> 8));
		  output(MAX_MIN_BASE + (id * 4) + 3, (byte)(mini     ));
		ram_cs();
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void max_min_load(idata byte id)
{
	idata word mini, maxi;

	cfg_ram_cs(MPM_CFG);
	  maxi  = input(MAX_MIN_BASE + (id * 4)    ) <<8;
	  maxi |= input(MAX_MIN_BASE + (id * 4) + 1)    ;
	  mini  = input(MAX_MIN_BASE + (id * 4) + 2) <<8;
	  mini |= input(MAX_MIN_BASE + (id * 4) + 3)    ;
	ram_cs();

	sam_tx.send.maxi = maxi;		sam_tx.send.mini = mini;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void board_status_set(byte call_id, byte flag_status)
{
	byte	error_check = 0x1, arrary_mok = 0, arrary_na = 0;
	byte 	i;

	if(call_id == 0)	{
		arrary_mok = 0;
		arrary_na  = 0;
	} else	{
		arrary_mok = call_id / 8;
		arrary_na  = call_id % 8;
	}
	i = (error_check << arrary_na);
	if(flag_status == FAIL)		status.rtu.sen_board[arrary_mok] |=  i;
	else						status.rtu.sen_board[arrary_mok] &= ~i;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sensor_status_set(byte call_id, byte sta)
{
	byte	error_check = 0x01, arrary_mok = 0, arrary_na = 0;
	byte	i;

	if(call_id == 0)	{
		arrary_mok = 0;
		arrary_na  = 0;
	} else	{
		arrary_mok = call_id / 8;
		arrary_na  = call_id % 8;
	}
	i = (error_check << arrary_na);
	if(sta & 0x02)	status.rtu.sen_sta[arrary_mok] |=  i;
	else			status.rtu.sen_sta[arrary_mok] &= ~i;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void mpm_cfg_save(idata word address)			// MPM Configration save
{
	idata byte extr_chkxor, extr_chksum;
	idata word i;

	cfg_ram_cs(MPM_CFG);
	switch(address)	{
		case SENSOR_SU  :	output(SENSOR_SU     , i_data.sensor_su      );
							break;
		case DAY_CNT	:	output(DAY_CNT       , i_data.day_cnt        );
							break;
		case STATION_ID	:	output(STATION_ID    , i_data.station_id >> 8);
							output(STATION_ID + 1, i_data.station_id     );
							break;
		case PASSWORD	:	output(PASSWORD      , i_data.password   >> 8);
							output(PASSWORD   + 1, i_data.password       );
							break;
		case CALL_TIME	:	output(CALL_TIME	 , i_data.call_time  >> 8);
							output(CALL_TIME  + 1, i_data.call_time      );
							break;
		case SAVE_TIME	:	output(SAVE_TIME	 , i_data.save_time  >> 8);
							output(SAVE_TIME  + 1, i_data.save_time      );
							break;
	}
	extr_chkxor = extr_chksum = 0xAA;
	output(MPM_CFG_CHK1, extr_chkxor);
	output(MPM_CFG_CHK2, extr_chkxor);
	for(i=MPM_CFG_BASE; i<MPM_CFG_END; i++)	{
		extr_chkxor ^= input(i);
		extr_chksum += input(i);
	}
	output(MPM_CFG_XOR, extr_chkxor);
	output(MPM_CFG_SUM, extr_chksum);
	ram_cs();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void mpm_cfg_load(idata word address)			// Load config data
{
	cfg_ram_cs(MPM_CFG);
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
	ram_cs();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void put_data(idata byte put_d)
{
	dat_ram_cs(i_data.ram_x);
	output(i_data.extr_addr++,put_d);
	ram_cs();
	if(i_data.extr_addr == 0)		i_data.ram_x++;
	if(i_data.ram_x > i_data.ram_limit)	i_data.ram_x = 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void save_data(idata byte b)
{
	idata unsigned char yy, mm, dd;
		  unsigned char hh, bb, ss;
	idata unsigned int  i;
		  unsigned char *p = (unsigned char *)(weather[b].sensor);
		  unsigned int  offset;

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
	cfg_ram_cs(MPM_CFG);
	if((yy != input(INDEX_DB + i)) || (mm != input(INDEX_DB + i + 1))
								   || (dd != input(INDEX_DB + i + 2)))	{
		ram_cs();
		i_data.day_cnt++;
		if(i_data.day_cnt > i_data.day_limit)	i_data.day_cnt = 0;
		i = i_data.day_cnt * 4;
		cfg_ram_cs(MPM_CFG);
		output(INDEX_DB + i    , yy);
		output(INDEX_DB + i + 1, mm);
		output(INDEX_DB + i + 2, dd);
		output(INDEX_DB + i + 3, i_data.day_cnt);		// Save Day count.
	}
	ram_cs();
	mpm_cfg_save(DAY_CNT);
	value.day_pos = ((value.weather_field * (long)(i_data.record_su)) * (long)(i_data.day_cnt))
				   + (value.weather_field * (long)(offset));
	i_data.ram_x     = (byte)(value.day_pos / 65536L);
	i_data.extr_addr = (word)(value.day_pos % 65536L);
	put_data(weather[b].year);	put_data(weather[b].month);	put_data(weather[b].day);
	put_data(weather[b].hour);	put_data(weather[b].min)  ;	put_data(weather[b].sec);
	for(i=0; i<i_data.sensor_su * sizeof(sensor_field_t); i++)	put_data(p[i]);
	if(weather[b].hour == 24)	{
		for(i=0; i<i_data.sensor_su; i++)	{
			send_to_sam_call(&com1, i, 'I');
			if(weather[SAVE].sensor[i].sensor_code == 0x03)	{
				sam_tx.send.mini = weather[SAVE].sensor[i].maxi;
				sam_tx.send.maxi = 0;
				send_to_sam_call(&com1, i, '@');
			}
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char get_data(void)
{
	idata byte get_d;

	dat_ram_cs(i_data.ram_x);
	get_d = input(i_data.extr_addr++);
	ram_cs();
	if(i_data.extr_addr == 0)	i_data.ram_x++;
	if(i_data.ram_x > i_data.ram_limit)	i_data.ram_x = 0;
	return(get_d);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
char load_data(idata byte b)
{
	idata unsigned char yy, mm, dd;
		  unsigned char ys, hh, bb, ss, flag = 0;
	idata unsigned int  i, j;
		  unsigned char *p = (unsigned char *)(weather[b].sensor);
		  unsigned int  offset;

	yy = host.receive.year;	mm = host.receive.month;
	dd = host.receive.day ;	hh = host.receive.hour ;
	bb = host.receive.min ;	ss = host.receive.sec  ;
	ys = host.receive.hour;
	if(hh == 24)	ys = 0;
	offset = (word)((((long)(ys) * 3600L) + ((long)(bb) * 60L) + (long)(ss)) / (long)(i_data.save_time));
	for(j=0; j<=i_data.day_limit*4; j+=4)	{		// INDEX_DB[4] = yy,mm,dd,i_data.day_cnt.
		cfg_ram_cs(MPM_CFG);
		if((yy == input(INDEX_DB+j)) && (mm == input(INDEX_DB+j+1))
									 && (dd == input(INDEX_DB+j+2)))	{
			i_data.day_cnt = input(INDEX_DB+j+3);	// Load day point
			ram_cs();
			flag = 1;
			value.day_pos = ((value.weather_field * (long)(i_data.record_su)) * (long)(i_data.day_cnt))
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
				return('A');
			}
		} ram_cs();
	}
	if(flag)	{
		weather[b].year = weather[b].month = weather[b].day = 0xCC;
		weather[b].hour = weather[b].min   = weather[b].sec	= 0xCC;
		return('A');
	} else	{
		weather[b].year = weather[b].month = weather[b].day = 0xCC;
		weather[b].hour = weather[b].min   = weather[b].sec = 0xCC;
		return('X');
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void poll_real_data(void)
{
	real.again_count = 1;
	if(real.call_id < i_data.sensor_su)	{
		while(real.again_count)	{
			real.again_count--;
			real.receive_status = OFF;
			send_to_sam_call(&com1, real.call_id, real.real_command);
			transmit_check__1();
			real.delay_count = 0;
			while(real.delay_count < DEL_TIME)	{
				if(real.receive_status == ON)	{
					board_status_set(real.call_id, NORMAL);
					real.again_count = 0;	break;
				}
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
			}
		}
		if(real.receive_status == OFF)	{
			weather[REAL].sensor[real.call_id].sensor_status = 0xEF;
			weather[REAL].sensor[real.call_id].sensor_code   = 0xEF;
			weather[REAL].sensor[real.call_id].sensor_id     = 0xEF;
			board_status_set(real.call_id, FAIL);
		}
		real.call_id++;
	} else	{
		real.call_id =   0;
		real.flag    = OFF;
		max_min_save();
		rcm_poll.again_count = 1;
		while(rcm_poll.again_count)	{
			rcm_poll.again_count--;
			rcm_poll.receive_status = OFF;
			status_send_to_rcm(&com1);
			transmit_check__1();
			rcm_poll.delay_count = 0;
			while(rcm_poll.delay_count < DEL_TIME + 3)	{
				if(rcm_poll.receive_status == ON)	{
					status.rtu.cnt_board.bits.rcm = NORMAL;	// RCM Status NORMAL
					rcm_poll.again_count = 0;	break;
				}
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
			}
		}
		if(rcm_poll.receive_status == OFF)
					status.rtu.cnt_board.bits.rcm = FAIL;	// RCM Status FAIL

		send_to_led_data(&com3);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void poll_call_data(void)
{
	call.again_count = 1;
	if(call.call_id < i_data.sensor_su)	{
		while(call.again_count)	{
			call.again_count--;
			call.receive_status = OFF;
			send_to_sam_call(&com1, call.call_id, call.call_command);
			transmit_check__1();
			call.delay_count = 0;
			while(call.delay_count < DEL_TIME)	{
				if(call.receive_status == ON)	{
					call.again_count = 0;	break;
				}
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
			}
		}
		if(call.receive_status == OFF)	{
			weather[CALL].sensor[call.call_id].sensor_status = 0xEF;
			weather[CALL].sensor[call.call_id].sensor_code   = 0xEF;
			weather[CALL].sensor[call.call_id].sensor_id     = 0xEF;
		}
		call.call_id++;
	} else	{
		call.call_id =   0;
		call.flag    = OFF;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void poll_save_data(void)
{
	save.again_count = 2;
	if(save.call_id < i_data.sensor_su)	{
		while(save.again_count)	{
			save.again_count--;
			save.receive_status = OFF;
			send_to_sam_call(&com1, save.call_id, save.save_command);
			transmit_check__1();
			save.delay_count = 0;
			while(save.delay_count < DEL_TIME + 3)	{
				if(save.receive_status == ON)	{
					save.again_count = 0;	break;
				}
				receive_check__1();		transmit_check__1();
				receive_check_23();		transmit_check_23();
			}
		}
		if(save.receive_status == OFF)	{
			weather[SAVE].sensor[save.call_id].sensor_status = 0xEF;
			weather[SAVE].sensor[save.call_id].sensor_code   = 0xEF;
			weather[SAVE].sensor[save.call_id].sensor_id     = 0xEF;
		}
		save.call_id++;
	} else	{
		save.call_id =   0;
		save.flag    = OFF;
		save_data(SAVE);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rtc_set(void)
{
	disable(EA);
	rtc_init();							// RTCC(MC146818) initiliaze.
	rtc.year  = host.receive.year;	rtc.month = host.receive.month;
	rtc.day   = host.receive.day;	rtc_date_write(&rtc);
	disable(EA);
	rtc.hour  = host.receive.hour;	rtc.min   = host.receive.min;
	rtc.sec   = host.receive.sec;	rtc_time_write(&rtc);
	ram_cs();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void delay(word count)
{
	word cnt;
	for(cnt=0; cnt<count; cnt++);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main(void)
{
	iram_cs();
	led_disp(COM1,TXD,ON );	led_disp(COM2,TXD,ON );	led_disp(COM3,TXD,ON );	delay(8000);
	led_disp(COM1,TXD,OFF);	led_disp(COM2,TXD,OFF);	led_disp(COM3,TXD,OFF);	delay(5000);
	led_disp(COM1,RXD,ON );	led_disp(COM2,RXD,ON );	led_disp(COM3,RXD,ON );	delay(8000);
	led_disp(COM1,RXD,OFF);	led_disp(COM2,RXD,OFF);	led_disp(COM3,RXD,OFF);
	rtc_init();							// RTCC(MC146818) initiliaze.
	initiliaze();						// Serial port & rxQ initialize.
	mpm_cfg_init();
	initial_weather(&weather[REAL]);	initial_weather(&weather[CALL]);
	initial_weather(&weather[SAVE]);	initial_weather(&weather[SEND]);
	led_table = (led_t *)(malloc(sizeof(led_t) * (i_data.sensor_su + 2)));
	variable_initial();					// Variable initialize.
	value.weather_field = 6L + ((long)(sizeof(sensor_field_t)) * (long)(i_data.sensor_su));
	while(1)	{
		if(real.flag == ON)		poll_real_data();
		if(call.flag == ON)		poll_call_data();
		if(save.flag == ON)		poll_save_data();
		receive_check_23();		transmit_check_23();
		receive_check_23();		transmit_check_23();
		if(cpm_st.flag == ON)	{
			cpm_st.receive_status = OFF;
			send_to_sam_call(&com1, cpm_st.call_id, 'M');
			cpm_st.delay_count = 0;
			while(cpm_st.delay_count < DEL_TIME)	{
				transmit_check__1();	receive_check__1();
				if(cpm_st.receive_status == ON)		break;
			}
			cpm_st.flag = OFF;
		}
		receive_check_23();		transmit_check_23();
		receive_check_23();		transmit_check_23();
		if(value.buffer_clr)	buffer_clear();
		if(value.memory_tst)	memory_test();
		if((rtc.check_count > 50) || (rtc.sqw_status_count > 50))	{
			rtc.check_count = rtc.sqw_status_count = 0;
			rtc_set();
		}
		receive_check__1();		transmit_check__1();
		receive_check_23();		transmit_check_23();
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
