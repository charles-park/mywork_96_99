/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#ifndef	__MAIN_H__
#define __MAIN_H__
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#include <io.h>
#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#ifdef __cplusplus
	#define __CPPARGS ...
#else
	#define __CPPARGS
#endif
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#define	COM1		0
#define	COM2		1

#define	SUPER		7741
#define	REAL		0
#define	CALL		1
#define	SAVE		2
#define	SEND		3
/*[*]--------------------------------------------------------------------[*]*/
#define NC			0
#define nc			0
#define	ON			1
#define	on			1
#define	OFF			0
#define	off			0
#define	FAIL		1
#define	fale		1
#define	NORMAL		0
#define	normal		0
#define	TRUE		1
#define	true		1
#define	FALSE		0
#define	false		0
/*[*]--------------------------------------------------------------------[*]*/
#define	UP_ARROW	72
#define	DN_ARROW	80
#define	LT_ARROW	75
#define	RT_ARROW	77
#define	ESC			27
#define	ENTER		13
#define	F1			59
#define	F2			60
#define	F3			61
#define	F4			62
#define	F5			63
#define	F6			64
#define	F7			65
#define	F8			66
#define	F9			67
#define	F10			68
/*[*]--------------------------------------------------------------------[*]*/
#define	BUF_SIZE	1024
/*[*]--------------------------------------------------------------------[*]*/
/*[*]	Use under header with serial io program .....                    [*]*/
/*[*]--------------------------------------------------------------------[*]*/
#define	DLL(x)		(x+0)				// 16C550 DLL Register
#define	DLM(x)		(x+1)				// 16C550 DLM Register
#define	THR(x)		(x+0)				// 16C550 THR Register
#define	RBR(x)		(x+0)				// 16C550 RBR Register
#define	IER(x)		(x+1)				// 16C550 IER Register
#define	IIR(x)		(x+2)				// 16C550 IIR Register
#define	LCR(x)		(x+3)				// 16C550 LCR Register
#define	MCR(x)		(x+4)				// 16C550 MCR Register
#define	LSR(x)		(x+5)				// 16C550 LSR Register
#define	MSR(x)		(x+6)				// 16C550 MSR Register
/*[*]---------------------- Serial port address -------------------------[*]*/
#define	COM1_ADDR	0x03F8
#define	COM2_ADDR	0x02F8
/*[*]------------------- Interrupt Request Numbers ----------------------[*]*/
#define COM1_IRQ	0x0C				// Serial 1 IRQ : IRQ 4
#define COM2_IRQ	0x0B				// Serial 2 IRQ : IRQ 3
/*[*]----------- 8259 PIC(Programmable Interrupt Controller)-------------[*]*/
#define IMR			0x21				// I/O address of OCW1(IMR) of 8259 PIC
#define OCW2		0x20				// I/O address of OCW2 of 8259 PIC
										// OCW: Operation Command Word
										// IMR: Interrupt Mask Register
#define MASKON		0xE7				// Mask IRQ3/IRQ4 on  -> IMR
#define MASKOFF		0x18				// Mask IRQ3/IRQ4 off -> IMR
#define EOI			0x20				// Non-specific End of Interrupt command -> OCW2
/*[*]--------------------------------------------------------------------[*]*/
#define	DATA7		0x02				// 7 Data bit
#define	DATA8		0x03				// 8 Data bit
#define	STOP1		0x00				// 1 Stop bit
#define	STOP2		0x04				// 2 Stop bit
#define	PRT_NONE	0x00				// None parity bit
#define	PRT_ODD		0x08				// Odd  parity bit
#define	PRT_EVEN	0x18				// Even parity bit
/*[*]--------------------------------------------------------------------[*]*/
#define	Y18432		1843200				// GM16C550 OSC : 1.8432MHz
#define	Y36864		3686400				// GM16C550 OSC : 3.6864MHz
/*[*]--------------------------------------------------------------------[*]*/
/*[*]--------------------------------------------------------------------[*]*/
typedef	unsigned char	byte;
typedef	unsigned int	word;
/*[*]--------------------------------------------------------------------[*]*/
typedef	struct	variable__t	{
	word	p_sp;
	word	p_ep;
	word	size;
	word	dsize;
	byte	flag;
	byte	*buf;
}	variable_t;

typedef	struct	protocol__t	{
	variable_t	var;
	byte	(*pcheck)(variable_t *p);
	byte	(*pcatch)(variable_t *p);
}	protocol_t;

typedef	struct	protocol_grp__t	{
	word	rx_sp,rx_ep;
	word	tx_sp,tx_ep;
	byte	tx_flag;
	byte	tx_buf[BUF_SIZE];
	byte	rx_buf[BUF_SIZE];
	byte	pcnt;
	byte	old_bdl;
	byte	old_bdh;
	byte	old_lcr;
	byte	old_mcr;
	byte	old_ier;
	word	base;
	protocol_t	*p;
}	protocol_grp_t;
/*[*]--------------------------------------------------------------------[*]*/
typedef struct	value__t	{
	unsigned char	comport;
	unsigned char	old_20;
	unsigned char	old_21;

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
/*[*]--------------------------------------------------------------------[*]*/
typedef struct 	key__t	{
	unsigned char	key_data[20];
	unsigned char	key_pos_st;
	unsigned char	key_pos_ed;
	unsigned char	key_chtr;
	unsigned char	key_cmp;
	unsigned char	key_del;
	unsigned char	key_down_flag;
	unsigned char	key_up_flag;
}	key_t;
/*[*]--------------------------------------------------------------------[*]*/
typedef struct sensor__t	{
	word		header;
	word		stid  ;
	dt_t		dt    ;
	byte		hour  ;
	byte		min   ;
	word		wd1   ;
	word		ws1   ;
	word		wd2	  ;
	word		ws2   ;
	word		mxwd  ;
	word		mxws  ;
	word		temp  ;
	word		mitemp;
	word		mxtemp;
	word		pred  ;
	word		gang1 ;
	word		gang2 ;
	word		pre   ;
	word		hpre  ;
	word		humi  ;
	word		mihumi;
	word		mxhumi;
	word		pres  ;
	word		mipres;
	word		mxpres;
	word		used1 ;
	word		used2 ;
	word		vsta  ;
	word		ssta  ;
	byte		xor   ;
	byte		add   ;
	word		tail  ;
}	sensor_t;

typedef union  sensor__u	{
	sensor_t	recv;
	byte		bytes[sizeof(sensor_t)];
}	sensor_u;

static	sensor_u	sensor;
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]--------------------------------------------------------------------[*]*/
/*[*]--------------------------------------------------------------------[*]*/
/*[*]--------------------------------------------------------------------[*]*/
typedef struct	host__t	{
	unsigned int	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned int	station_id;
	unsigned int	password;
	unsigned char	command;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned int	tail;
}	host_t;

typedef union host__u	{
	host_t			receive;
	unsigned char	bytes[sizeof(host_t)];
}	host_u;
/*[*]--------------------------------------------------------------------[*]*/
typedef struct	rtc__t	{
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
}	rtc_t;
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]	RTC FUNCTION DEFINE												 [*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void rtc_read(void);
void rtc_date_write(rtc_t *da);
void rtc_time_write(rtc_t *ti);
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]	INIT & INT FUNCTION DEFINE										 [*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void initiliaze(void);
void init_serial_vector(byte port);
void return_serial_vector(byte port);
void init_serial_grp(protocol_grp_t *comx, word b_addr, long speed,
					 long crystal, byte status, byte protocol_count);
void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
					 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
void protocol_event(protocol_grp_t *comx, byte com_data);
void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);

void comport_event(void);
byte header_tail_check(variable_t *var);
byte com1_sign_catch(variable_t *var);
byte com1_cmd_catch(variable_t *var);
byte com2_sign_catch(variable_t *var);
byte com2_cmd_catch(variable_t *var);
void send_data(byte port, byte sdata);

void send_led_data(byte port, byte cmd, byte sta, byte cd, byte id);
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]	MAIN FUNCTION DEFINE											 [*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void print_err_msg(char x);
char argument_check(void);
void variable_initial(void);
byte keyinput(void);
int main(int arguc, char *argus[]);
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
extern key_t			key;
extern value_t			value;
extern rtc_t			rtc;
extern protocol_grp_t	com1, com2;
extern host_u			host;
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#endif 	__MAIN_H__
