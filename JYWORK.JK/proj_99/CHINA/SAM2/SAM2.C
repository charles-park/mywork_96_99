//[*]----------------------------------------------------------------------[*]
#include <io51.h>
#include <stdio.h>
#include <stdlib.h>
#include "sam2.h"
#include "pros.hpp"
#include "send.hpp"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil    Date : 1998. 1. 10~     [*]",
   "[*]  Program End : 1999, 1, 10                          [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : CHINA (SAM2)                        [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD : SAM2 BOARD(Temperature 3 & Precipitation)  [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  ID : TEMP0(2), TEMP1(3), TEMP2(4), PREC(5)         [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  CODE : TEMP -> 0x02, PREC -> 0x03                  [*]",
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
	variable_initialize();

	Sam_id = sensor_id_read(JUMPER_ADDR);
	Sam_id = 0x02;

	// Temperature 0
	struct_initialize(&temp0_data[REAL ] , 'M', 0x2, Sam_id  );	// real
	struct_initialize(&temp0_data[MIN10] , 'T', 0x2, Sam_id  );	// 10 min

	// Temperature 1
	struct_initialize(&temp1_data[REAL ] , 'M', 0x2, Sam_id+1);
	struct_initialize(&temp1_data[MIN10] , 'T', 0x2, Sam_id+1);

	// Temperature 2
	struct_initialize(&temp2_data[REAL ] , 'M', 0x2, Sam_id+2);	// real
	struct_initialize(&temp2_data[MIN10] , 'T', 0x2, Sam_id+2);	// 10 min

	// Precipitation
	struct_initialize(&prec_data[REAL ]  , 'M', 0x3, Sam_id+3);
	struct_initialize(&prec_data[MIN10]  , 'T', 0x3, Sam_id+3);

	// Procesing Struct Value Init
	pros_value_init(&temp0_pros);	pros_value_init(&temp1_pros);
	pros_value_init(&temp2_pros);	pros_value_init(&prec_pros );


	// Serial Port & Interrupt Initialize
	comport_initialize();	interrupt_initialize();		enable(EA);
}
//[*]----------------------------------------------------------------------[*]
//   Variable Initialize
//[*]----------------------------------------------------------------------[*]
void variable_initialize(void)
{
	RX_LED(OFF);				TX_LED(OFF);

	hz_timeout = 0;				TEMP0_SELECT();
	Sample     = OFF;			Sample_count = 0;
	Jugi_flag  = OFF;			Jugi_count   = 0;
	Jugi_pros_count = 0;

	// Precipitation gangdo
	gangdo_pros  = OFF;			gangdo_flag  = OFF;
	gangdo_count = 0;           Prec_count   = 0;

	// reboot check & status Check
	status_temp0.byte_data = 0x00;		Temp0_flag = ON;
	status_temp1.byte_data = 0x00;		Temp1_flag = ON;
	status_temp2.byte_data = 0x00;		Temp2_flag = ON;
	status_prec .byte_data = 0x00;		Prec_flag  = ON;
}
//[*]----------------------------------------------------------------------[*]
//   SAM Board Jumper ID Read
//[*]----------------------------------------------------------------------[*]
byte sensor_id_read(word addr)
{
	byte 	i, read_id = 0, ck_id = 0;

	while(1)
	{
		ck_id = input(addr);
		for(i=0;i<10;i++)		read_id = input(addr);
		if(ck_id == read_id)	break;
	}	read_id  = ~read_id;

	return		(byte)(read_id);
}
//[*]----------------------------------------------------------------------[*]
//   Send Sturct init
//[*]----------------------------------------------------------------------[*]
void struct_initialize(sensor_u *sen, byte b_cmd, byte b_scod, byte b_sid)
{
	sen->sam.header = 0xFFFF;	sen->sam.cmd    = b_cmd ;
	sen->sam.status = 0     ;   sen->sam.scod   = b_scod;
	sen->sam.sid    = b_sid ;	sen->sam.avr    = 0     ;
	sen->sam.spc    = 0     ;	sen->sam.min    = 0     ;
	sen->sam.max    = 0     ;	sen->sam.xor    = 0     ;
	sen->sam.tail   = 0xFFFE;
}
//[*]----------------------------------------------------------------------[*]
//   Pors Struct value initialize
//[*]----------------------------------------------------------------------[*]
void pros_value_init(sensor_pros_t *p)
{
	p->avr   = 0;	p->min = 0 ;	p->max = 0;
	p->count = 0;	p->sum = 0.;
}
//[*]----------------------------------------------------------------------[*]
//   Com Port Initialize ( Baud & Serial Group & Protocol Size .. ETC)
//[*]----------------------------------------------------------------------[*]
void comport_initialize(void)
{
	init_serial_grp  (&com, INTERNAL, 0, BPS9600, M11, SMOD_OFF, NOT_USED, 1);
	init_serial_pro  (&com, 0, 11, rx_check, rx_catch);
	set_protocol_flag(&com, 0, ON);
}
//[*]----------------------------------------------------------------------[*]
//   Interrupt init
//[*]----------------------------------------------------------------------[*]
void interrupt_initialize(void)
{
	disable(ET0);	disable(TR0);	enable (PX1);
	enable (IT0);	enable (EX0);	enable (IT1);	enable (EX1);
}
//[*]----------------------------------------------------------------------[*]
//   Transmit Data Ready??
//[*]----------------------------------------------------------------------[*]
void transmit_check(void)
{
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
	disable(EX0);
	if(Sample == OFF)		Sample = ON;
	hz_timeout = 0; 		enable(EX0);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x13] void EX1_int (void)    /* Precipitation Count */
{
	disable(EX1);	Prec_count++;	enable(EX1);
}
//[*]----------------------------------------------------------------------[*]
//   Internal Serial Interrupt Processing Function
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)    /* Serial Port */
{
	disable(ES);
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
	enable(ES);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	// Struct & Interrupt & Com Port Initialize
	initialize();
	while(1)
	{
		if(Sample)	{
			Sample_count++;
			switch(Sample_count)	{
				case	1:
					TEMP0_SELECT();	temp_cal(&temp0_data[REAL], &temp0_pros);
					TEMP1_SELECT();										break;
				case	2:
					TEMP1_SELECT();	temp_cal(&temp1_data[REAL], &temp1_pros);
					TEMP2_SELECT();										break;
				case	3:
					TEMP2_SELECT();	temp_cal(&temp2_data[REAL], &temp2_pros);
					TEMP0_SELECT();										break;
				case	4:
					prec_cal(&prec_data [REAL], &prec_pros );
					if(++Jugi_count >= 600)	{
						Jugi_flag = ON;		Jugi_count = 0;		Jugi_pros_count = 0;
					}		TEMP0_SELECT();	Sample_count = 0;			break;
				default	 :	TEMP0_SELECT();	Sample_count = 0; 			break;
			}	Sample = OFF;
		}	receive_check();			transmit_check();

		if(Jugi_flag)	{
			jugi_pros(++Jugi_pros_count);
			if(Jugi_pros_count > 4)	{
				Jugi_pros_count = 0; 	Jugi_flag = OFF;
			}
		}   receive_check();			transmit_check();

		if(hz_timeout  > 50000)	{
			status_temp0.bits.clock = status_temp1.bits.clock =
			status_temp2.bits.clock = status_prec .bits.clock = FAIL  ;
		}	else	{
			hz_timeout++;
			status_temp0.bits.clock = status_temp1.bits.clock =
			status_temp2.bits.clock = status_prec .bits.clock = NORMAL;
		}	receive_check();			transmit_check();
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]





