//[*]----------------------------------------------------------------------[*]
#include <IO51.H>
#include <MATH.H>
#include <STDIO.H>
#include <STDLIB.H>
#include "PROC_EWS.H"
#include "PROS.HPP"
#include "SERIAL.HPP"
#include "CAT&CHK.HPP"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JONG KIL   Date : 1998. 7. 20~     [*]",
   "[*]  Program End : 1997, 11, 20                         [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : EWS  WD, WS, TEMP, HUMI, PRE,       [*]",
   "[*]                 Yester PRE, BUL-KISU, CHEKAM TEMP   [*]",
   "[*]                 TIME                                [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 2.00                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]----------------------------------------------------------------------[*]
void rtcc_init(void)
{
	byte    rtcc_test = 1;

	watch();
	output(RTCC_REG_A, RTCC_SQW_DATA);
	output(RTCC_REG_B, RTCC_CFG_DATA);

	rtcc_test = input(RTCC_REG_D) & 0x80;
	if(!rtcc_test)	{
		t.yy = 97;  t.mm = 11;   t.dd = 22;
		t.hh = 11;  t.bb = 50;   t.ss = 7;
		while(rtcc_date_time_set(&t, ALL_SET));
		Hour_check = t.hh;
	}
	t.yy = input(RTCC_YEAR );	t.mm = input(RTCC_MONTH);
	t.dd = input(RTCC_DAY  );	t.hh = input(RTCC_HOUR );
	t.bb = input(RTCC_MIN  );	t.ss = input(RTCC_SEC  );
	if((t.yy > 99) || (t.mm > 12) || (t.dd > 31) ||
	   (t.hh > 23) || (t.bb > 59) || (t.ss > 59)  )	{
		t.yy = 99;	t.mm = 9;	t.dd = 9;
		t.hh = 9;	t.bb = 9;	t.ss = 9;
		while(rtcc_date_time_set(&t, ALL_SET));
	}	Hour_check = t.hh;
}
//[*]----------------------------------------------------------------------[*]
byte rtcc_date_time_set(time_t *s_time, byte mode)
{
	watch();
	if((s_time->yy > 99) || (s_time->mm > 12) || (s_time->dd > 31) ||
	   (s_time->hh > 23) || (s_time->bb > 59) || (s_time->ss > 59) )	return FAIL;

	if(mode == DATE_SET || mode == ALL_SET) {
		output(RTCC_YEAR , s_time->yy);
		output(RTCC_MONTH, s_time->mm);
		output(RTCC_DAY  , s_time->dd);
	}
	if(mode == TIME_SET || mode == ALL_SET)	{
		output(RTCC_HOUR , s_time->hh);
		output(RTCC_MIN  , s_time->bb);
		output(RTCC_SEC  , s_time->ss);
	}
	s_time->yy = input(RTCC_YEAR );	s_time->mm = input(RTCC_MONTH);
	s_time->dd = input(RTCC_DAY  );	s_time->hh = input(RTCC_HOUR );
	s_time->bb = input(RTCC_MIN  );	s_time->ss = input(RTCC_SEC  );

	if((s_time->yy > 99) || (s_time->mm > 12) || (s_time->dd > 31) ||
	   (s_time->hh > 23) || (s_time->bb > 59) || (s_time->ss > 59) )	return FAIL;

	return NORMAL;
}
//[*]----------------------------------------------------------------------[*]
void rtcc_read(void)
{
	time_t	cmp;

	cmp.yy = input(RTCC_YEAR );	cmp.mm = input(RTCC_MONTH);
	cmp.dd = input(RTCC_DAY  );	cmp.hh = input(RTCC_HOUR );
	cmp.bb = input(RTCC_MIN  );	cmp.ss = input(RTCC_SEC  );
	if((cmp.mm == 0) || (cmp.dd == 0))						return;
	if((cmp.yy > 99) || (cmp.mm > 12) || (cmp.dd > 31))		return;
	if((cmp.hh > 23) || (cmp.bb > 59) || (cmp.ss > 59))		return;
	t.yy = cmp.yy;	t.mm = cmp.mm;	t.dd = cmp.dd;
	t.hh = cmp.hh;	t.bb = cmp.bb;	t.ss = cmp.ss;
}
//[*]----------------------------------------------------------------------[*]
void func_init(void)
{
	byte 	i = 0, ck_id = 0, read_id = 0;

	watch();	read_id = input(JUMPER);
	while(1)
	{
		ck_id = input(JUMPER);
		if(read_id == ck_id)	i++;
		else					read_id = input(JUMPER);
		if(i > 10)				break;
	}	read_id = ~read_id;

	// 0 then 45Hz, other 38Hz ( Wind Speed Sensor select )
	if(read_id != 0)	Speed = 1;
	else				Speed = 0;

	// insert var -> Mode		// Function Define
	select[0].proc_func = wind_dir_func;
	select[1].proc_func = wind_spd_func;
	select[2].proc_func = temper_func  ;
}
//[*]----------------------------------------------------------------------[*]
void struct_init(void)
{
	word	i;

	watch();
	for(i=0; i<SENSOR_SU +3; i++)	{
		sensor_struct_init(&send_grp.sensor[i], i);
		if(i < SENSOR_SU)	{
			proc_struct_init  (&proc_grp.real [i]	  );
			sensor_struct_init(&save_grp.real [i], i+3);
			save_grp.real[i].scod = i;
		}
	}
	send_grp.sensor[1].cmd = 'N';
	send_grp.sensor[2].cmd = 'S';
}
//[*]----------------------------------------------------------------------[*]
void sensor_struct_init(sensor_t *p, byte id)
{
	watch();
	p->header = 0xFFFF;		p->cmd    = 0     ;
	p->sta    = 0     ; 	p->scod   = 0     ;
	p->sid    = id    ; 	p->avr    = 0     ;
	p->spc    = 0     ; 	p->min    = 0     ;
	p->max    = 0     ; 	p->xor    = 0     ;
	p->tail   = 0xFFFE;
}
//[*]----------------------------------------------------------------------[*]
void proc_struct_init(data_t *p)
{
	watch();
	p->avr	 		= 0 ;	p->min	 		= 0 ;
	p->max	 		= 0 ;	p->value		= 0 ;
	p->sin  		= 0.;	p->cos  		= 0.;
	p->sum  		= 0.;   p->proc_count	= 0 ;
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	watch();
	// Base time initiliaze.
	old.yy = t.yy;      old.mm = t.mm;      old.dd = t.dd;
	old.hh = t.hh;      old.bb = t.bb;      old.ss = t.ss;

	// Default value setting
	Real_flag  = OFF;	Real_count = 0;

	RX_LED    = OFF;	TX_LED = OFF;	Send_flag = OFF;

	init.byte_data = 0xFF;

	// Serial Protocol initiliaze.
	init_serial_grp  (&com, INTERNAL, 0, BPS2400, M22, SMOD_ON, NOT_USED, 1);
	init_serial_pro  (&com, 0, 17, rx_check, rx_catch);
	set_protocol_flag(&com, 0, OFF);			// Key data receive

	// Interrupt initiliaze.
	TMOD |= 0x05;		TH0 = 0;			TL0 = 0;
	T2CON = 0;			TH2 = 0;			TL2 = 0;

	// Interrupt enable.
	disable(ET2);		disable(TR2);		enable(TR0);// Ws Calculate
	enable (EX0);		disable(EX1);       enable(EA );
}
//[*]----------------------------------------------------------------------[*]
void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)	{	i++;	i--;	watch();	}
}
//[*]----------------------------------------------------------------------[*]
void send_proc(void)
{
	word	i;

	watch();
	send_grp.sensor[DATA_GUBUN].cmd  = 'M';
	send_grp.sensor[DATA_GUBUN].scod = 0  ;

	for(i=0; i<SENSOR_SU; i++)	{
		send_grp.sensor[i+3]     = save_grp.real [i];
		send_grp.sensor[i+3].cmd = 'M';
	}	send_data();
}
//[*]----------------------------------------------------------------------[*]
void send_data(void)
{
	word	i,j, offset = 0;

	watch();
	for(i = 0; i < SENSOR_SU+3; i++)	{
		offset = sizeof(sensor_t) * i;
		send_grp.sensor[i].xor = 0;		watch();
		for(j = 0; j < sizeof(sensor_t); j++)	{
			if(j>1 && j<sizeof(sensor_t) -3)    send_grp.sensor[i].xor^= send_grp.bytes[j + offset];
			data_move(send_grp.bytes[j + offset]);
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void data_move(byte mov_data)
{
	watch();
	com.tx_buf[com.tx_ep++] = mov_data;
	if(com.tx_ep >= BUFFER_SIZE)	com.tx_ep = 0;
}
//[*]----------------------------------------------------------------------[*]
void receive_check(void)	// Key data receive & protocol check.
{
	watch();
	if(com.rx_sp != com.rx_ep)	{
		if(com.cat_flag == OFF)				return;
		protocol_event(&com, com.rx_buf[com.rx_sp++]);
		if(com.rx_sp >= BUFFER_SIZE)		com.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
void transmit_check(void)	// LED data send.
{
	watch();
	if(com.tx_sp != com.tx_ep)	{
		if(com.tx_flag)	{
			disable(EA);
			SBUF = com.tx_buf[com.tx_sp++];
			enable(EA);
			if(com.tx_sp >= BUFFER_SIZE)	com.tx_sp = 0;
			com.tx_flag = OFF;
		}
	}	else	com.cat_flag = ON;
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] void EX0_int(void)
{
	disable(EX0);		rtcc_read();

	if(old.ss != t.ss)	{

		if(!Real_flag)		Real_flag = ON;

		old.yy = t.yy;	old.mm = t.mm;	old.dd = t.dd;
		old.hh = t.hh;	old.bb = t.bb;	old.ss = t.ss;

		if((old.ss % 10) == 0)	{
			send_grp.sensor[PROC_DATE].avr = old.yy;
			send_grp.sensor[PROC_DATE].spc = old.mm;
			send_grp.sensor[PROC_DATE].min = old.dd;

			send_grp.sensor[PROC_TIME].avr = old.hh;
			send_grp.sensor[PROC_TIME].spc = old.bb;
			send_grp.sensor[PROC_TIME].min = old.ss;
			if(!Send_flag)		Send_flag = ON;
		}
	}
	enable(EX0);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)	// Serial Interrupt processing routine.
{
	disable(PS);
	if(RI)	{
		disable(RI);	RX_LED = ON;
		com.rx_buf[com.rx_ep++] = SBUF;
		if(com.rx_ep >= BUFFER_SIZE)	com.rx_ep = 0;
	}	else if(TI)		{
		disable(TI);	TX_LED = ON;
		if(com.tx_sp != com.tx_ep)	{
			SBUF = com.tx_buf[com.tx_sp++];
			if(com.tx_sp >= BUFFER_SIZE)	com.tx_sp = 0;
		} 	else	{
			com.tx_flag = ON;	TX_LED = OFF;
		}
	}
	enable(PS);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	watch();
	rtcc_init();	func_init();	struct_init();	variable_init();

	while(1)
	{
		watch();
		transmit_check();		receive_check();
		if(Real_flag )			select[Real_count ++].proc_func(REAL );

		transmit_check();		receive_check();
		if(Real_count  >= SENSOR_SU)	{	Real_count = 0;	Real_flag = OFF; }

		transmit_check();		receive_check();

		if(Send_flag)			{	send_proc();	Send_flag = OFF;	}
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]






