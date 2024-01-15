//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*] Wind Speed Sensor & Wind Direction Sensor 22.1184MHz set      	   [*]
//[*] 												  Edit Date : 97. 1.24 [*]
//[*]												  Edit By   : Park.    [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]� TEST Program 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include <io51.h>
#include <math.h>
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		SB			100
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		REAL		'M'
#define 	SEC1		't'
#define		MIN1		'O'
#define		MIN10		'T'
#define		INIT		'I'
#define		RST			'@'
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct Serial__t	{
	unsigned char rx_buf[SB];
	unsigned char tx_buf[SB];
	unsigned char rx_sp, rx_ep;
	unsigned char tx_sp, tx_ep, tx_flag;
	unsigned int  baseaddr;
	unsigned char intorext;
}	Serial_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct Command__t {
	unsigned int   header;
	unsigned char  command;
	unsigned char  id;
	unsigned int   min;
	unsigned int   max;
	unsigned char  check_sum;
	unsigned int   tail;
}   Command_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef union Command__u {
	Command_t mpm;
	unsigned char bytes[sizeof(Command_t)];
}   Command_u;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct Sensor__t {
	unsigned int   header;
	unsigned char  command;
	unsigned char  status;
	unsigned char  scode;
	unsigned char  id;
	unsigned int   avr;
	unsigned int   spc;
	unsigned int   min;
	unsigned int   max;
	unsigned char  check_sum;
	unsigned int   tail;
}   Sensor_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef union Sensor__u {
	Sensor_t sam;
	unsigned char bytes[sizeof(Sensor_t)];
}   Sensor_u;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef union Led_ststus__t	{
	struct	{
		unsigned char led3 : 2;
		unsigned char led2 : 2;
		unsigned char led1 : 2;
		unsigned char led0 : 2;
	}	bits;
	unsigned char	status;
}	Led_ststus_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static bit				Call_Flag 	 = 0;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static unsigned char 	Sample_Clock 	 = 	      0;
static unsigned char 	Sec_Clock    	 = 	      0;
static unsigned char 	sensor_error_cnt = 		  0;
static unsigned char	ID		  		 = 		  0;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static Sensor_u			real, sec1, min1, min10;
static Serial_t			com;
static Led_ststus_t		Led_status;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define LED_ADDR		0xB000
#define LOFF			0x00
#define BLUE			0x01
#define RED 			0x02
#define YELLO    		0x03
#define LED_INIT    	0xFF
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define WATCH_DOG(x)	x = (1 - x)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define DATABIT7    	0x02
#define DATABIT8    	0x03
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define STOPBIT1    	0x00
#define STOPBIT2    	0x04
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define NOPARITY    	0x00
#define ODDPARITY   	0x08
#define EVENPARITY  	0x18
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define BUFFEREMPTY		(-1)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define led_disp(pos,color)													\
{                            												\
	switch(pos)	{															\
		case 0x00: Led_status.bits.led0 = color; break;     				\
		case 0x01: Led_status.bits.led1 = color; break;     				\
		case 0x02: Led_status.bits.led2 = color; break;     				\
		case 0x03: Led_status.bits.led3 = color; break;     				\
		case 0xFF: Led_status.status    = color; break;     				\
	}                                                       				\
	write_XDATA(LED_ADDR,Led_status.status);                				\
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define Serial_Tx_interrupt(com)											\
{                                                           				\
	if(com.tx_sp == com.tx_ep)	com.tx_flag = 0;							\
	else {																	\
		if(com.intorext == INTERNAL)	{                       			\
			SBUF = com.tx_buf[com.tx_sp++];               					\
			if(com.tx_sp >= SB) com.tx_sp = 0;               				\
		}                                                       			\
	}																		\
}                                                           				\
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define Serial_Rx_interrupt(com)											\
{                                           								\
	com.rx_buf[com.rx_ep++] = (com.intorext==EXTERNAL)						\
							? read_XDATA(RBR(com.baseaddr))               	\
							: SBUF;											\
	if(com.rx_ep >= SB) com.rx_ep = 0;										\
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define Serial_putc(com,tdata)												\
{                                                                           \
	com.tx_buf[com.tx_ep++] = tdata;                                        \
	if(com.tx_ep >= SB) com.tx_ep = 0;                                      \
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define Serial_getc(com)													\
{                                                                           \
	com.rx_buf[com.rx_sp++];                                            	\
	if(com.rx_sp >= SB) com.rx_sp = 0;                                 	 	\
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*] Function Prototype Predefine
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void SCON_int		(void);
interrupt void EX1_int		(void);
interrupt void EX0_int		(void);
void Init			  		(void);
void tx_send		  		(void);
void id_jumper_check		(void);
void send_mpm				(Sensor_u *send_data);
void sensor_struct_init		(Sensor_u *sen,unsigned char id,unsigned char scode,unsigned char cmd);
void Serial_init			(Serial_t *com,unsigned char mode,unsigned int addr,unsigned int speed,unsigned char status);
int  point					(int bufsize, int now, int step);
void sensor_protocol_check  (void);
void mpm_protocol_check	    (void);
void variable_init		    (void);
void led_temp_protocol_check(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Serial_init(Serial_t *com,unsigned char mode,unsigned int addr,unsigned int speed,unsigned char status)
{
	unsigned char spd_lo = ((speed     ) & 0x00FF);
	unsigned char spd_hi = ((speed >> 8) & 0x00FF);
	com->rx_sp = com->rx_ep = 0;
	com->tx_sp = com->tx_ep = 0; com->tx_flag = 0;
	com->baseaddr = addr;
	com->intorext = mode;
	if(com->intorext == EXTERNAL)	{
		write_XDATA(LCR(addr), (0x80|status));
		write_XDATA(DLL(addr),	      spd_lo);
		write_XDATA(DLM(addr),  	  spd_hi);
		write_XDATA(LCR(addr),  	  status);
		write_XDATA(IER(addr),   	    0x01);
		write_XDATA(MCR(addr),   	    0x0b);
		read_XDATA (RBR(addr));
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void EX1_int(void)
{
	if	   ((read_XDATA(IIR(COM2_ADDR))&0x06)==0x04) { 	// COM2 RX INTERRUPT
		led_disp(2,BLUE);
		Serial_Rx_interrupt(com2);
	}
	else if((read_XDATA(IIR(COM3_ADDR))&0x06)==0x04) { 	// COM3 RX INTERRUPT
		led_disp(3,BLUE);
		Serial_Rx_interrupt(com3);
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void EX0_int(void)
{
	Sample_Clock++;
	if(Call_Flag)		   	 {
		write_XDATA(CALL_ADDR,CALL_OFF); Call_Flag=0;
		led_disp(0,LOFF);
		if(sensor_error_cnt < 100) 	sensor_error_cnt ++;
	}
	if(Sample_Clock >= 4)	 {
		Sec_Clock++;
		Sample_Clock = 0;
		if((Sec_Clock % 3) == 0) {
			write_XDATA(CALL_ADDR,CALL_ON ); Call_Flag=1;
			led_disp(0,YELLO);
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void SCON_int(void)
{
	ES = 0;
	if	   (RI == 1) {	RI=0; led_disp(1,BLUE); Serial_Rx_interrupt(com1); }
	else if(TI == 1) {  TI=0; led_disp(1,RED ); Serial_Tx_interrupt(com1); }
	ES = 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Led_init(void)
{
	int i;
	led_disp(LED_INIT,0x00);
	for(i=0;i<4;i++)	led_disp(i,RED);
	for(i=0;i<5000;i++)	if((i%100)==0) WATCH_DOG(P1.3),i--,i++;
	for(i=0;i<4;i++)	led_disp(i,BLUE);
	for(i=0;i<5000;i++)	if((i%100)==0) WATCH_DOG(P1.3),i--,i++;
	led_disp(LED_INIT,0x00);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int point(int bufsize, int now, int step)
{
	now += step;
	if(now >= bufsize)  now -= bufsize;
	if(now <        0)  now += bufsize;
	return now;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void tx_send(void)
{
	if(com1.tx_flag ==          1) 	return;
	if(com1.tx_sp   == com1.tx_ep)	{	led_disp(1,LOFF);	return;	}
	ES 		   	 = 0;
	com1.tx_flag = 1;
	SBUF 	   	 = com1.tx_buf[com1.tx_sp];
	com1.tx_sp 	 = point(SB,com1.tx_sp,+1);
	ES 		   	 = 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_mpm(Sensor_u *send)
{
	unsigned char i;
	send->sam.check_sum = 0x00;
	for(i = 0; i<sizeof(Sensor_u); i++)		{
		if(i >= 2 && i < (sizeof(Sensor_u))-3)	{
			send->sam.check_sum ^= send->bytes[i];
		}
		Serial_putc(com1,send->bytes[i]);
	}
	tx_send();
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void sensor_struct_init(Sensor_u *sen,unsigned char id,unsigned char scode,unsigned char cmd)
{
	sen->sam.header    = 0xFFFF;
	sen->sam.tail      = 0xFFFE;
	sen->sam.scode     = scode;
	sen->sam.id        = id;
	sen->sam.status    = 0;
	sen->sam.check_sum = 0;
	sen->sam.spc   	   = 0;
	sen->sam.avr   	   = 0;
	sen->sam.max   	   = 0;
	sen->sam.min       = 0;
	sen->sam.command   = cmd;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void mpm_protocol_check(void)
{
	if(com1.rx_sp==com1.rx_ep) return; 	Serial_getc(com1);
	if(com1.rx_buf[point(SB,com1.rx_sp, -1)]!=0xFE) return;
	if(com1.rx_buf[point(SB,com1.rx_sp, -2)]!=0xFF) return;
	if(com1.rx_buf[point(SB,com1.rx_sp,-11)]!=0xFF) return;
	if(com1.rx_buf[point(SB,com1.rx_sp,-10)]!=0xFF) return; led_disp(1,LOFF);
	if(com1.rx_buf[point(SB,com1.rx_sp, -8)]==	ID ||
	   com1.rx_buf[point(SB,com1.rx_sp, -8)]==0xFF)
	{
		Command_u	cmd;
		unsigned char i, xor = 0;

		for(i=0; i<11; i++)	{
			cmd.bytes[i] = com1.rx_buf[point(SB,com1.rx_sp,-(11-i))];
			if(i>1 && i<8)	{
				xor ^= com1.rx_buf[point(SB,com1.rx_sp,-(11-i))];
			}
		}
		if(cmd.mpm.check_sum == xor)	{
			switch(cmd.mpm.command)	{
				case RST   	    : real .sam.min = cmd.mpm.min;
								  min1 .sam.min = cmd.mpm.min;
								  min10.sam.min = cmd.mpm.min;
								  real .sam.max = cmd.mpm.max;
								  min1 .sam.max = cmd.mpm.max;
								  min10.sam.max = cmd.mpm.max;
								  real .sam.avr = cmd.mpm.max;
								  min1 .sam.avr = cmd.mpm.max;
								  min10.sam.avr = cmd.mpm.max;	break;
				case INIT  		:                               break;
				case REAL  		: send_mpm(&real );				break;
				case MIN1  		: send_mpm(&min1 );				break;
				case MIN10 		: send_mpm(&min10);				break;
				case OFFSET_SCAN: Offset = Distance;
								  offset.sam.min = Offset;
								  send_mpm(&offset);			break;
				case OFFSET_SET : Offset = cmd.mpm.min;			break;

			}
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void sensor_protocol_check(void)
{
	static          int 	Snow_aver[10];
	static 			int		Old_snow 		= -9999;
	static unsigned char 	Snow_Err_count  =     0;
	static unsigned char 	Snow_count  	=     0;
	static unsigned char 	Snow_flag   	=     1;
	char					i;
	float					distance;
	long					Snow_total = 0;
	int 					abs_Snow, Snow_now, Snow = 0;

	if(com2.rx_sp==com2.rx_ep)	return;		 Serial_getc(com2);
	if(com2.rx_buf[point(SB,com2.rx_sp, -1)]!=ASCII_LF) return;
	if(com2.rx_buf[point(SB,com2.rx_sp, -2)]!=ASCII_CR)	return;
	if(com2.rx_buf[point(SB,com2.rx_sp, -9)]!=	   '+')	return;
	if(com2.rx_buf[point(SB,com2.rx_sp, -8)] == '.')	{
		Snow += (com2.rx_buf[point(SB,com2.rx_sp, -7)]-0x30)*100;
		Snow += (com2.rx_buf[point(SB,com2.rx_sp, -6)]-0x30)*10;
		Snow += (com2.rx_buf[point(SB,com2.rx_sp, -5)]-0x30);
	}	else  if(com2.rx_buf[point(SB,com2.rx_sp, -7)] == '.')	{
		Snow += (com2.rx_buf[point(SB,com2.rx_sp, -8)]-0x30)*1000;
		Snow += (com2.rx_buf[point(SB,com2.rx_sp, -6)]-0x30)*100;
		Snow += (com2.rx_buf[point(SB,com2.rx_sp, -5)]-0x30)*10;
		Snow += (com2.rx_buf[point(SB,com2.rx_sp, -4)]-0x30);
	}	else	{
		Snow = -9999;
	}

	if(Snow != -9999)	{
		if(Old_snow == -9999) Old_snow = Snow;
		abs_Snow = Snow - Old_snow;
		if(abs_Snow <   0) abs_Snow = abs_Snow * -1;
		if(abs_Snow > 100)	{
			Snow_Err_count++;
			if(Snow_Err_count >= 5)	{
				Snow_Err_count = 0;
				Old_snow = Snow;
			}
		}	else	{
			Snow_aver[Snow_count++] = Old_snow  = Snow;
			if(Snow_count >= 10)      Snow_flag = Snow_count = 0;
			if(Snow_flag == 0 && Temp != -99999.0)	{
				distance = sqrt(Temp / 273.15);
				for(i=0;i<10;i++) Snow_total += Snow_aver[i];
				distance = (float)(Snow_total / 100.) * distance;
				Distance = (  int)(distance   *  10.);
				if(Offset != 0)	{
					if((Offset - Distance) > 0)	{
						Snow_now	  = Offset - Distance;
						if(Snow_now < 5)	 Snow_now = 0;
						real .sam.max = real .sam.avr = Snow_now;
						min1 .sam.max = min1 .sam.avr = Snow_now;
						min10.sam.max = min10.sam.avr = Snow_now;
					}	else	{
						Offset 		  = Distance;
						real .sam.max = real .sam.avr = 0;
						min1 .sam.max = min1 .sam.avr = 0;
						min10.sam.max = min10.sam.avr = 0;
					}
					real .sam.status  = 0;
					min1 .sam.status  = 0;
					min10.sam.status  = 0;
					sensor_error_cnt  = 0;
				}	else	{
					real.sam.avr  	  = -Distance;
					min1.sam.avr  	  = -Distance;
					min10.sam.avr 	  = -Distance;
				}
			}
		}
	}
	led_disp(2,LOFF);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void led_temp_protocol_check(void)
{
	if(com3.rx_sp==com3.rx_ep) return;	 Serial_getc(com3);
	if(com3.rx_buf[point(SB,com3.rx_sp, -1)]!=0xFE) return;
	if(com3.rx_buf[point(SB,com3.rx_sp, -2)]!=0xFF)	return;	led_disp(3,LOFF);
	if(com3.rx_buf[point(SB,com3.rx_sp,-16)]!=0xFF) return;
	if(com3.rx_buf[point(SB,com3.rx_sp,-17)]!=0xFF)	return;
	if(com3.rx_buf[point(SB,com3.rx_sp,-15)]!= 'M')	return;
	if(com3.rx_buf[point(SB,com3.rx_sp,-14)]!=0x00)	return;
	if(com3.rx_buf[point(SB,com3.rx_sp,-13)]!=0x02)	return;
	{
		unsigned char i, xor = 0;
		Sensor_u	  temp;
		for(i=0; i<17; i++)	{
			temp.bytes[i] = com3.rx_buf[point(SB,com3.rx_sp,-(17-i))];
			if(i>1 && i<14)	{
				xor ^= com3.rx_buf[point(SB,com3.rx_sp,-(17-i))];
			}
			if(xor == temp.sam.check_sum)	{
				Temp = (float)((temp.sam.avr-1000) / 10.) + 273.16;
			}
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void id_jumper_check(void)
{
	unsigned char i, check_id = 0, id = 0;
	while(1)	{
		WATCH_DOG(P1.3);
		id = P1 & 0xF0;
		for(i=0;i<10;i++)	{
			if(id == (P1 & 0xF0)) check_id++;
		}
		if(check_id > 8)	break;
	}
	ID = ((~(id >> 4)) & 0x0F);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Init(void)
{
	IE   = 0x80;
	PCON = 0x80;      // SMOD = 1  SET
	P1.3 = 	  0;

	Led_init();

	TMOD = 0x25; // Timer 1==> mode2, Timer 0 ==> mode 1, c/t = 0 ==> timer
	TH1  = 0xf4;
	TL1  = 0xf4; // 22.1184Mhz, SMOD=1 ==> 9600bps

	SCON = 0x50; // serial port mode 1, ren(receieve enable)==>1, RB8(STOP bit)==>0

	EA = 1;		TR0 = 0;	TR1 = 1;
	PS = 1;		IT0 = 1;	IT1 = 0;
	ES = 1;		EX0 = 1;	EX1 = 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	Init();
	id_jumper_check();

	// Wind direction Sensor Sturct initiliaze
	sensor_struct_init(&real  ,ID,0x0C,	   REAL);
	sensor_struct_init(&min1  ,ID,0x0C,	   MIN1);
	sensor_struct_init(&sec1  ,ID,0x0C,    SEC1);
	sensor_struct_init(&min10 ,ID,0x0C,	   MIN10);

	// Wind Speed Sensor Sturct initiliaze
	sensor_struct_init(&real  ,ID+1,0x0C,  REAL);
	sensor_struct_init(&sec1  ,ID+1,0x0C,  SEC1);
	sensor_struct_init(&min1  ,ID+1,0x0C,  MIN1);
	sensor_struct_init(&min10 ,ID+1,0x0C,  MIN10);

	while(1)	{
			 mpm_protocol_check();	WATCH_DOG(P1.3);	tx_send();
		  sensor_protocol_check();  WATCH_DOG(P1.3);    tx_send();
		led_temp_protocol_check();  WATCH_DOG(P1.3);    tx_send();
		// Board Error Check
		if(sensor_error_cnt >= 500)	{
			real.sam.status  |= 0x02;
			min1.sam.status  |= 0x02;
			min10.sam.status |= 0x02;
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
