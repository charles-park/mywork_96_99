//[*]----------------------------------------------------------------------[*]
#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "sam1.h"
#include "pros.hpp"
#include "send.hpp"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil    Date : 1998. 12. 30     [*]",
   "[*]  Program End : 1999, 1, 10                          [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : CHINA (SAM1)                        [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD : SAM1 BOARD(Wind Direction & Wind Speed)    [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  WD : 0x0, WS : 0x1                                 [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 2.xx                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]----------------------------------------------------------------------[*]
//   Struct & Variable initialize
//[*]----------------------------------------------------------------------[*]
void initialize(void)
{
	// Interrupt & Static Variable Initialize
	watch();				variable_initialize();

	jumper.byte_data = 0x00;
	jumper.byte_data = sensor_id_read(JUMPER_ADDR);
	Sam_id           = 0;
	Sam_id           = (byte)(jumper.bits.id  );
	function_init     ((byte) jumper.bits.func);

	// Wind Direction Struct Initialize
	struct_initialize(&wd_data[REAL ] , 'M', 0x0, Sam_id  );	// real
	struct_initialize(&wd_data[MIN1 ] , 'O', 0x0, Sam_id  );	// 1 min
	struct_initialize(&wd_data[MIN5 ] , 'm', 0x0, Sam_id  );	// 5 min
	struct_initialize(&wd_data[MIN10] , 'T', 0x0, Sam_id  );	// 10 min

	// Wind Speed Sturct Initialize
	struct_initialize(&ws_data[REAL ] , 'M', 0x1, Sam_id+1);
	struct_initialize(&ws_data[MIN1 ] , 'O', 0x1, Sam_id+1);
	struct_initialize(&ws_data[MIN5 ] , 'm', 0x1, Sam_id+1);
	struct_initialize(&ws_data[MIN10] , 'T', 0x1, Sam_id+1);

	// Procesing Struct Value Init
	pros_value_init_wd(&wd_pros[REAL]);	pros_value_init_wd(&wd_pros[MIN1 ]);
	pros_value_init_wd(&wd_pros[MIN5]);	pros_value_init_wd(&wd_pros[MIN10]);

	pros_value_init_ws(&ws_pros[REAL]);	pros_value_init_ws(&ws_pros[MIN1 ]);
	pros_value_init_ws(&ws_pros[MIN5]);	pros_value_init_ws(&ws_pros[MIN10]);

	// Serial Port & Interrupt Initialize
	comport_initialize();	interrupt_initialize();		enable(EA);
}
//[*]----------------------------------------------------------------------[*]
//   Variable Initialize
//[*]----------------------------------------------------------------------[*]
void variable_initialize(void)
{
	word	i;
	watch();
	RX_LED(ON);					TX_LED(ON);

	Arry_p = 0x00;
	for(i=0; i<100; i++)	{
		watch();
		wd_arry[i]    = ws_arry[i]    = ws_cal_dat[i] = 0x00;
		wd_cal_sin[i] = wd_cal_cos[i] = 0.;
	}

	error.wd_count   = 0;		error.ws_count = 0;
	error.wd_data    = 0;		error.ws_data  = 0;
	error.hz_timeout = 0;       Sample     = OFF;

	// reboot check
	Wd_reboot  = ON;			Ws_reboot  = ON;
	RX_LED(OFF);				TX_LED(OFF);

	status_wd.byte_data  = 0;	status_ws.byte_data  = 0;
}
//[*]----------------------------------------------------------------------[*]
//   SAM Board Jumper ID Read
//[*]----------------------------------------------------------------------[*]
byte sensor_id_read(word addr)
{
	byte 	i, read_id = 0, ck_id = 0;

	watch();
	while(1)
	{
		ck_id = input(addr);
		for(i=0;i<10;i++)		read_id = input(addr);
		if(ck_id == read_id)	break;
	}	read_id  = ~read_id;

	return		(byte)(read_id);
}
//[*]----------------------------------------------------------------------[*]
//   Function Init
//[*]----------------------------------------------------------------------[*]
void function_init(byte val)
{
	watch();
	switch(val)	{
		case	0:	func.ws_cal = ws_ms_38hz  ;		break;
		case	1:	func.ws_cal = ws_ms_45hz  ;		break;
		case	2:  func.ws_cal = ws_knot_38hz;		break;
		case	3:	func.ws_cal = ws_knot_45hz;		break;
		default	 :	func.ws_cal = ws_knot_45hz;		break;
	}	func.wd_cal = wind_cal;
}
//[*]----------------------------------------------------------------------[*]
//   Send Sturct init
//[*]----------------------------------------------------------------------[*]
void struct_initialize(sensor_u *sen, byte b_cmd, byte b_scod, byte b_sid)
{
	watch();
	sen->sam.header = 0xFFFF;	sen->sam.cmd    = b_cmd ;
	sen->sam.status = 0     ;   sen->sam.scod   = b_scod;
	sen->sam.sid    = b_sid ;	sen->sam.avr    = 0     ;
	sen->sam.spc    = 0     ;	sen->sam.min    = 0     ;
	sen->sam.max    = 0     ;	sen->sam.xor    = 0     ;
	sen->sam.tail   = 0xFFFE;
}
//[*]----------------------------------------------------------------------[*]
//   Wd Pors Struct value initialize
//[*]----------------------------------------------------------------------[*]
void pros_value_init_wd(wd_pros_t *p)
{
	p->avr = 0 ;	p->min = 0 ;	p->max = 0;
	p->sin = 0.;	p->cos = 0.;
}
//[*]----------------------------------------------------------------------[*]
//   Ws Pors Struct value initialize
//[*]----------------------------------------------------------------------[*]
void pros_value_init_ws(ws_pros_t *p)
{
	p->avr   = 0;	p->min = 0 ;	p->max = 0;
	p->count = 0;	p->sum = 0.;
}
//[*]----------------------------------------------------------------------[*]
//   Com Port Initialize ( Baud & Serial Group & Protocol Size .. ETC)
//[*]----------------------------------------------------------------------[*]
void comport_initialize(void)
{
	watch();
	init_serial_grp  (&com, INTERNAL, 0, BPS9600, M22, SMOD_ON, NOT_USED, 1);
	init_serial_pro  (&com, 0, 11, rx_check, rx_catch);
	set_protocol_flag(&com, 0, ON);
}
//[*]----------------------------------------------------------------------[*]
//   Interrupt init
//[*]----------------------------------------------------------------------[*]
void interrupt_initialize(void)
{
	watch();
	TMOD |= 0x05;		TH0 = 0;			TL0 = 0;
	enable (TR0);		enable (IT0);		enable (EX0);
}
//[*]----------------------------------------------------------------------[*]
//   A/D Converter READ
//[*]----------------------------------------------------------------------[*]
word adc_read(void)
{
	word	i, ret_value = 0;

	for(i=0; i<10; i++)		{	i++;	i--;	}
	output(ADC_MSB, 0xff);
	for(i=0; i<10; i++)		{	i++;	i--;	}

	i = 0;
	while(ADC_STS)	{	if(i++ > 100)		break;	}
	if(i > 100)		status_wd.bits.adc = status_ws.bits.adc = FAIL  ;
	else       		status_wd.bits.adc = status_ws.bits.adc = NORMAL;

	ret_value  = input(ADC_LSB) >>4;	ret_value |= input(ADC_MSB) <<4;

	return	(word)(ret_value);
}
//[*]----------------------------------------------------------------------[*]
//   Sec10 Gust catch & avrage catch(REAL DATA PROCESS)
//[*]----------------------------------------------------------------------[*]
void processing(void)
{
	byte	pos        = 0;
	word	wd_cal_dat = 0 , i = 0, j = 0;
	double	wd_tot_sin = 0., wd_tot_cos = 0.;
	double  wd_gust_sin= 0., wd_gust_cos= 0., wd_vector = 0.;
	long	ws_tot_sum = 0., gust_tot   = 0.;


	if(Arry_p >= 40)		pos = 40;
	else					pos = 0 ;

	for(i=pos; i<pos+40; i++)	{
		watch();
		wd_cal_dat    = func.wd_cal(wd_arry[i]);
		wd_cal_sin[i] = sin((double)((double)wd_cal_dat / 10.) * M_PI / 180.);
		wd_tot_sin   += wd_cal_sin[i];
		wd_cal_cos[i] = cos((double)((double)wd_cal_dat / 10.) * M_PI / 180.);
		wd_tot_cos   += wd_cal_cos[i];
		ws_cal_dat[i] = func.ws_cal(ws_arry[i]);
		ws_tot_sum   += ws_cal_dat[i];
	}
	watch();		receive_check();	transmit_check();
	watch();		receive_check();	transmit_check();

	wd_vector         = atan(wd_tot_sin / wd_tot_cos) * (180. / M_PI);
	if     (wd_tot_sin < 0 && wd_tot_cos > 0)		wd_vector += 360.;
	else if(wd_tot_sin > 0 && wd_tot_cos < 0)		wd_vector += 180.;
	else if(wd_tot_sin < 0 && wd_tot_cos < 0)		wd_vector += 180.;

	wd_pros[REAL].avr = (word)(wd_vector  * 10.);
	ws_pros[REAL].avr = (word)(ws_tot_sum / 40.);

	// Average Data Error Detect
	if( wd_pros[REAL].avr >= 3600)	{
		wd_pros[REAL].avr  = 3599;
		status_wd.bits.sensor = status_wd.bits.adc = FAIL;
	}	else	{
		// Sensor Error Detect
		if( error.wd_data != wd_pros[REAL].avr)	{
			error.wd_data  = wd_pros[REAL].avr;
			error.wd_count = 0;
			status_wd.bits.sensor = status_wd.bits.adc = NORMAL;
		}	else	{
			if(error.wd_count > 2160)	{		// 6 Hour
				status_wd.bits.sensor = status_wd.bits.adc = FAIL;
			}	else	{
				error.wd_count++;
				status_wd.bits.sensor = status_wd.bits.adc = NORMAL;
			}
		}
	}
	wd_pros[REAL].avr = (word)(wd_vector  * 10.);
	ws_pros[REAL].avr = (word)(ws_tot_sum / 40.);

	if(jumper.bits.func > 1)	{	// Wind Speed Knot Pros
		// Average Data Error Detect
		if( ws_pros[REAL].avr >= 1200)	{
			ws_pros[REAL].avr  = 1200;
			status_ws.bits.sensor = status_ws.bits.adc = FAIL  ;
		}	else 	{
			// Sensor Error Detect
			if( error.ws_data != ws_pros[REAL].avr)	{
				error.ws_data  = ws_pros[REAL].avr;
				error.ws_count = 0;
				status_ws.bits.sensor = status_ws.bits.adc = NORMAL;
			}	else	{
				if(error.ws_count > 2160)	{		// 6 Hour
					status_ws.bits.sensor = status_ws.bits.adc = FAIL;
				}	else	{
					error.ws_count++;
					status_ws.bits.sensor = status_ws.bits.adc = NORMAL;
				}
			}	watch();
		}
	}	else	{                   // Wind Speed m/s Pros
		// Average Data Error Detect
		if( ws_pros[REAL].avr >=  600)	{
			ws_pros[REAL].avr  =  600;
			status_ws.bits.sensor = status_ws.bits.adc = FAIL  ;
		}	else	{
			// Sensor Error Detect
			if( error.ws_data != ws_pros[REAL].avr)	{
				error.ws_data  = ws_pros[REAL].avr;
				error.ws_count = 0;
				status_ws.bits.sensor = status_ws.bits.adc = NORMAL;
			}	else	{
				if(error.ws_count > 2160)	{		// 6 Hour
					status_ws.bits.sensor = status_ws.bits.adc = FAIL;
				}	else	{
					error.ws_count++;
					status_ws.bits.sensor = status_ws.bits.adc = NORMAL;
				}
			}	watch();
		}
	}

	ws_pros[REAL].max = 0;	ws_pros[REAL].min = 5000;
	for(i=pos; i<pos+40; i++)	{
		if((i < 72) && ((i % 4) == 0))	{
			gust_tot    = 0.;
			wd_gust_sin = wd_gust_cos = 0.;
			for(j=0; j<12; j++)	{
				gust_tot    += ws_cal_dat[i+j];
				wd_gust_sin += wd_cal_sin[i+j];
				wd_gust_cos += wd_cal_cos[i+j];
			}
			gust_tot = gust_tot / 12.;
			if( ws_pros[REAL].max < (word)gust_tot)	{
				ws_pros[REAL].max = (word)gust_tot;
				wd_vector   = atan(wd_gust_sin / wd_gust_cos) * (180. / M_PI);
				if     (wd_gust_sin < 0 && wd_gust_cos > 0)	wd_vector += 360.;
				else if(wd_gust_sin > 0 && wd_gust_cos < 0)	wd_vector += 180.;
				else if(wd_gust_sin < 0 && wd_gust_cos < 0)	wd_vector += 180.;
				wd_pros[REAL].max = (word)(wd_vector * 10.);
			}
		}	watch();
		if( ws_pros[REAL].min > ws_cal_dat[i])	{
			ws_pros[REAL].min = ws_cal_dat[i];
			wd_vector = atan(wd_cal_sin[i]/wd_cal_cos[i]) * (180. / M_PI);
			if     (wd_cal_sin[i] < 0 && wd_cal_cos[i] > 0)	wd_vector += 360.;
			else if(wd_cal_sin[i] > 0 && wd_cal_cos[i] < 0)	wd_vector += 180.;
			else if(wd_cal_sin[i] < 0 && wd_cal_cos[i] < 0)	wd_vector += 180.;
			wd_pros[REAL].min = (word)(wd_vector * 10.);
		}	watch();
	}

	watch();		receive_check();	transmit_check();
	watch();		receive_check();	transmit_check();

	// Jugi Pros Value
	ws_pros[MIN1 ].sum += ws_tot_sum;	ws_pros[MIN1 ].count += 40;
	ws_pros[MIN5 ].sum += ws_tot_sum;   ws_pros[MIN5 ].count += 40;
	ws_pros[MIN10].sum += ws_tot_sum;   ws_pros[MIN10].count += 40;

	wd_pros[MIN1 ].sin += wd_tot_sin;	wd_pros[MIN1 ].cos   += wd_tot_cos;
	wd_pros[MIN5 ].sin += wd_tot_sin;	wd_pros[MIN5 ].cos   += wd_tot_cos;
	wd_pros[MIN10].sin += wd_tot_sin;	wd_pros[MIN10].cos   += wd_tot_cos;

	if         (ws_pros[MIN1].max < ws_pros[REAL].max)	{
		ws_pros[MIN1 ].max = ws_pros[REAL].max;
		wd_pros[MIN1 ].max = wd_pros[REAL].max;
	}	else if(ws_pros[MIN1].min > ws_pros[REAL].min)	{
		ws_pros[MIN1 ].min = ws_pros[REAL].min;
		wd_pros[MIN1 ].min = wd_pros[REAL].min;
	}

	if         (ws_pros[MIN5].max < ws_pros[REAL].max)	{
		ws_pros[MIN5 ].max = ws_pros[REAL].max;
		wd_pros[MIN5 ].max = wd_pros[REAL].max;
	}	else if(ws_pros[MIN5].min > ws_pros[REAL].min)	{
		ws_pros[MIN5 ].min = ws_pros[REAL].min;
		wd_pros[MIN5 ].min = wd_pros[REAL].min;
	}

	if         (ws_pros[MIN10].max < ws_pros[REAL].max)	{
		ws_pros[MIN10].max = ws_pros[REAL].max;
		wd_pros[MIN10].max = wd_pros[REAL].max;
	}	else if(ws_pros[MIN10].min > ws_pros[REAL].min)	{
		ws_pros[MIN10].min = ws_pros[REAL].min;
		wd_pros[MIN10].min = wd_pros[REAL].min;
	}

	watch();
	if(ws_pros[MIN1 ].count >= 240 )	{
		jugi_pros(MIN1 );	save_struct(MIN1 );
	}
	if(ws_pros[MIN5 ].count >= 1200)	{
		jugi_pros(MIN5 );	save_struct(MIN5 );
	}
	if(ws_pros[MIN10].count >= 2400)	{
		jugi_pros(MIN10);	save_struct(MIN10);
	}
	save_struct(REAL );
	watch();		receive_check();	transmit_check();
	watch();		receive_check();	transmit_check();
}
//[*]----------------------------------------------------------------------[*]
void jugi_pros(byte num)
{
	double	wd_vector = 0.;

	watch();
	wd_vector   = atan(wd_pros[num].sin / wd_pros[num].cos) * (180. / M_PI);
	if     (wd_pros[num].sin < 0 && wd_pros[num].cos > 0)	wd_vector += 360.;
	else if(wd_pros[num].sin > 0 && wd_pros[num].cos < 0)	wd_vector += 180.;
	else if(wd_pros[num].sin < 0 && wd_pros[num].cos < 0)	wd_vector += 180.;
	wd_pros[num].avr = (word)(wd_vector * 10.);
	ws_pros[num].avr = (word)(ws_pros[num].sum / (long)ws_pros[num].count);
}
//[*]----------------------------------------------------------------------[*]
void save_struct(byte num)
{
	watch();
	wd_data[num].sam.avr = wd_pros[num].avr;
	wd_data[num].sam.min = wd_pros[num].min;
	wd_data[num].sam.max = wd_pros[num].max;

	wd_pros[num].max = wd_pros[num].min = wd_pros[REAL].avr;
	wd_pros[num].sin = wd_pros[num].cos = 0.;

	ws_data[num].sam.avr = ws_pros[num].avr;
	ws_data[num].sam.min = ws_pros[num].min;
	ws_data[num].sam.max = ws_pros[num].max;

	ws_pros[num].max = ws_pros[num].min   = ws_pros[REAL].avr;
	ws_pros[num].sum = ws_pros[num].count = 0 ;
}
//[*]----------------------------------------------------------------------[*]
//   Transmit Data Ready??
//[*]----------------------------------------------------------------------[*]
void transmit_check(void)
{
	watch();
	if(com.tx_ep != com.tx_sp)	{
		if(com.tx_flag == OFF)	{
			disable(ES);					TX_LED(ON);
			SBUF = com.tx_buf[com.tx_sp++];
			if(com.tx_sp >= BUFFER_SIZE)	com.tx_sp = 0;
			com.tx_flag = ON;				enable(ES);
		}
	}	else	{	com.tx_flag = OFF;		TX_LED(OFF);}
}
//[*]----------------------------------------------------------------------[*]
//   Receive Data Ready??
//[*]----------------------------------------------------------------------[*]
void receive_check(void)
{
	watch();
	if(com.rx_sp != com.rx_ep)	{
		RX_LED(ON);
		protocol_event(&com, com.rx_buf[com.rx_sp++]);
		if(com.rx_sp >= BUFFER_SIZE)	com.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
//   4 Hz Processing Function ( Check MPM Communication Time )
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] void EX0_int (void)    /* External Interrupt 0 */
{
	disable(EA);

	ADC_STATUS       = HOLD   ;		wd_arry[Arry_p]  = adc_read();
	ADC_STATUS       = SAMPLE ;

	ws_arry[Arry_p]  = TH0 <<8;		ws_arry[Arry_p] |= TL0;
	ws_arry[Arry_p] *= 4      ;
	TH0 = 0x0;						TL0 = 0x0;

	if(++Arry_p >= MAX_BUFFER)	{	Sample = ON;	Arry_p = 0;	}
	if(  Arry_p == 40        )		Sample = ON;

	error.hz_timeout = 0;			enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int (void)	{	return;	}	/* Timer 0 Overflow */
//[*]----------------------------------------------------------------------[*]
//   Internal Serial Interrupt Processing Function
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)    /* Serial Port */
{
	disable(EA);
	if(RI)	{
		disable(RI);
		com.rx_buf[com.rx_ep++] = SBUF;
		if(com.rx_ep >= BUFFER_SIZE)		com.rx_ep = 0;
	}	else if(TI)	{
		disable(TI);
		if(com.tx_sp != com.tx_ep)	{
			SBUF = com.tx_buf[com.tx_sp++];
			if(com.tx_sp >= BUFFER_SIZE)	com.tx_sp = 0;
		}
		else								com.tx_flag = OFF;
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	// Struct & Interrupt & Com Port Initialize
	watch();			initialize();

	while(1)
	{
		watch();		receive_check();	transmit_check();
		if(Sample)	{	processing   ();	Sample = OFF;	}

		if(error.hz_timeout  > 50000)	{
			status_wd.bits.clock = status_ws.bits.clock = FAIL  ;
		}	else	{
			error.hz_timeout++;
			status_wd.bits.clock = status_ws.bits.clock = NORMAL;
		}
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]





