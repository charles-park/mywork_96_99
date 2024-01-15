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
   "[*]  Program : PARK. JONG KIL   Date : 1998. 11. 23~    [*]",
   "[*]  Program End : 1998, 11, 24                         [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : EWS  WD, WS, TEMP, HUMI, PRESS      [*]",
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
	select[0].proc_func = wind_dir_func; select[1].proc_func = wind_spd_func;
	select[2].proc_func = temper_func  ; select[3].proc_func = humidt_func  ;
	select[4].proc_func = pressure_func;
}
//[*]----------------------------------------------------------------------[*]
void struct_init(void)
{
	word	i;

	watch();
	// Pros struct init...
	for(i=0; i<SENSOR_SU; i++)		{
		watch();	proc_struct_init(&proc_grp, i);
	}

	// Struct init              /id/code/num	( Sensor SU )
	save_struct_init(&save_grp, 3, 0, 0);	// WD
	save_struct_init(&save_grp, 4, 1, 1);	// WS
	save_struct_init(&save_grp, 5, 2, 2);	// TEMP
	save_struct_init(&save_grp, 6, 6, 3);	// HUMI
	save_struct_init(&save_grp, 7, 5, 4);	// PRESS

	for(i=0; i<SENSOR_SU +3; i++)	{
		watch();	send_struct_init(&send_grp, i);
	}
	send_grp.sensor[0].cmd = 'M';	// Real Send Command
	send_grp.sensor[1].cmd = 'N';   // Date Send Command
	send_grp.sensor[2].cmd = 'S';	// Time Send Command
}
//[*]----------------------------------------------------------------------[*]
void send_struct_init(sensor_u *p, word num)
{
	p->sensor[num].header = 0xFFFF;	p->sensor[num].cmd    = 'M'   ;
	p->sensor[num].sta    = 0     ;	p->sensor[num].scod   = 0     ;
	p->sensor[num].sid    = num   ; p->sensor[num].avr    = 0     ;
	p->sensor[num].spc    = 0     ; p->sensor[num].min    = 0     ;
	p->sensor[num].max    = 0     ; p->sensor[num].xor    = 0     ;
	p->sensor[num].tail   = 0xFFFE;
}
//[*]----------------------------------------------------------------------[*]
void save_struct_init(save_grp_t *p, byte id, byte scod, word num)
{
	watch();
	p->real[num].header = 0xFFFF;	p->real[num].cmd    = 'M'   ;
	p->real[num].sta    = 0     ; 	p->real[num].scod   = scod  ;
	p->real[num].sid    = id    ; 	p->real[num].avr    = 0     ;
	p->real[num].spc    = 0     ; 	p->real[num].min    = 0     ;
	p->real[num].max    = 0     ; 	p->real[num].xor    = 0     ;
	p->real[num].tail   = 0xFFFE;

	p->min10[num].header = 0xFFFF;	p->min10[num].cmd    = 'M'   ;
	p->min10[num].sta    = 0     ; 	p->min10[num].scod   = scod  ;
	p->min10[num].sid    = id    ; 	p->min10[num].avr    = 0     ;
	p->min10[num].spc    = 0     ; 	p->min10[num].min    = 0     ;
	p->min10[num].max    = 0     ; 	p->min10[num].xor    = 0     ;
	p->min10[num].tail   = 0xFFFE;

	p->hour[num].header = 0xFFFF;	p->hour[num].cmd    = 'M'   ;
	p->hour[num].sta    = 0     ; 	p->hour[num].scod   = scod  ;
	p->hour[num].sid    = id    ; 	p->hour[num].avr    = 0     ;
	p->hour[num].spc    = 0     ; 	p->hour[num].min    = 0     ;
	p->hour[num].max    = 0     ; 	p->hour[num].xor    = 0     ;
	p->hour[num].tail   = 0xFFFE;
}
//[*]----------------------------------------------------------------------[*]
void proc_struct_init(proc_grp_t *p, word cnt)
{
	watch();
	p->real[cnt].avr  = 0 ;	p->real[cnt].min	 	 = 0 ;
	p->real[cnt].max  = 0 ;	p->real[cnt].value		 = 0 ;
	p->real[cnt].sin  = 0.;	p->real[cnt].cos  		 = 0.;
	p->real[cnt].sum  = 0.; p->real[cnt].proc_count	 = 0 ;

	p->min10[cnt].avr = 0 ;	p->min10[cnt].min  		 = 0 ;
	p->min10[cnt].max = 0 ;	p->min10[cnt].value		 = 0 ;
	p->min10[cnt].sin = 0.;	p->min10[cnt].cos  		 = 0.;
	p->min10[cnt].sum = 0.; p->min10[cnt].proc_count = 0 ;

	p->hour[cnt].avr  = 0 ;	p->hour[cnt].min  		 = 0 ;
	p->hour[cnt].max  = 0 ;	p->hour[cnt].value		 = 0 ;
	p->hour[cnt].sin  = 0.;	p->hour[cnt].cos  		 = 0.;
	p->hour[cnt].sum  = 0.; p->hour[cnt].proc_count	 = 0 ;
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	word	i;
	watch();
	// Default value setting
	Real_flag  = OFF;	Real_count = 0;	Min10_flag = OFF;	Min10_count = 0;
	Hour_flag  = OFF;	Hour_count = 0;	Hour_check = 0;

	RX_LED    = OFF;			TX_LED    = OFF;
	Send_mode = REAL_MODE;		Send_flag = OFF;

	init.byte_data = 0xFF;		init.flag.nused = 0x00;

	PRE_CLEAR = ON;		delay(100);		PRE_CLEAR = OFF;

	Press_init_flag  = OFF;		Press_wait_count = 0;		Press_catch = 0.;
	for(i=0; i<20; i++)			Press_ascii[i] = 0x00;

	// Serial Protocol initiliaze.
	init_serial_grp  (&com1, INTERNAL, 0, BPS2400, M22, SMOD_ON, NOT_USED, 1);
	init_serial_pro  (&com1, 0, 17, rx_check, rx_catch);
	set_protocol_flag(&com1, 0, ON);			// Key data receive

	// Press Sensor Catch Port Initialize(Visala Press Sensor)
	init_serial_grp  (&com2, EXTERNAL, 0xD000, BPS2400, M1,
					   NOT_USED, (DATABIT7|STOP1|PARITY_EVEN), 1);
	init_serial_pro  (&com2, 0, 14, press_chk , press_cat );
	set_protocol_flag(&com2, 0, ON);

	// Interrupt initiliaze.
	TMOD |= 0x05;		TH0 = 0;			TL0 = 0;
	T2CON = 0;			TH2 = 0;			TL2 = 0;

	// Interrupt enable.
	enable (TR0);		// Ws Calculate
	enable (ET2);		enable (TR2);		disable(TF2);
	enable (EX0);		enable (IT0);
	enable (EX1);       disable(IT1);
	enable (EA );

	press_init(&com2);	// Pressure Sensor Initialize.
}
//[*]----------------------------------------------------------------------[*]
void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)	{	i++;	i--;	watch();	}
}
//[*]----------------------------------------------------------------------[*]
void press_init(protocol_grp_t *comx)
{
	word    i;

	// Press Stop command
	watch();
	comx->tx_buf[comx->tx_ep++] = 'S' ;
	if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;
	comx->tx_buf[comx->tx_ep++] = 0x0D;
	if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;
	comx->tx_buf[comx->tx_ep++] = 0x0A;
	if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;
	while(transmit(comx))   ;

	watch();
	for(i=0; i<6; i++)  {           // Press Sensor Form Command
		comx->tx_buf[comx->tx_ep++] = prs_set1[i];
		if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;
	}   while(transmit(comx))   ;       delay(100);

	for(i=0; i<22; i++) {           // Press Sensor Form Set
		comx->tx_buf[comx->tx_ep++] = prs_set2[i];
		if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;
	}   while(transmit(comx))   ;       delay(100);

	for(i=0; i<10; i++) {           // Press Sensor Output Delay(1 s)
		comx->tx_buf[comx->tx_ep++] = prs_set3[i];
		if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;
	}   while(transmit(comx))   ;       delay(100);

	// Press Run command
	watch();
	comx->tx_buf[comx->tx_ep++] = 'R' ;
	if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;
	comx->tx_buf[comx->tx_ep++] = 0x0D;
	if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;
	comx->tx_buf[comx->tx_ep++] = 0x0A;
	if(comx->tx_ep >= BUFFER_SIZE)  comx->tx_ep = 0;

	while(transmit(comx))   ;   delay(100);
	Press_init_flag = OFF;      Press_wait_count = 0;

	// Serial Pointer clear.
	com1.rx_ep = com1.rx_sp = com2.rx_ep = com2.rx_sp = 0;
	com1.tx_ep = com1.tx_sp = com2.tx_ep = com2.tx_sp = 0;
}
//[*]----------------------------------------------------------------------[*]
void send_proc(void)
{
	word	i;

	watch();
	switch(Send_mode)	{		// Send_mode is default REAL, catch function redefine
		case	REAL_MODE :
			send_grp.sensor[DATA_GUBUN].cmd = 'M';	// Real  cmd?? 'M'
			for(i=0; i<SENSOR_SU; i++)	{
				watch();	send_grp.sensor[i+3] = save_grp.real [i];
			}
			break;
		case	MIN10_MODE:
			send_grp.sensor[DATA_GUBUN].cmd = 'T';	// Min10 cmd?? 'T'
			for(i=0; i<SENSOR_SU; i++)	{
				watch();	send_grp.sensor[i+3] = save_grp.min10[i];
			}
			break;
		case	HOUR_MODE :
			send_grp.sensor[DATA_GUBUN].cmd = 'o';	// Hour  cmd?? 'o'
			for(i=0; i<SENSOR_SU; i++)	{
				watch();	send_grp.sensor[i+3] = save_grp.hour [i];
			}
			break;
		case    TIME_MODE :											return;
		default			  :                   						return;
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
			data_move(&com1, send_grp.bytes[j + offset]);
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void data_move(protocol_grp_t *comx, byte mov_data)
{
	watch();
	comx->tx_buf[comx->tx_ep++] = mov_data;
	if(comx->tx_ep >= BUFFER_SIZE)	comx->tx_ep = 0;
}
//[*]----------------------------------------------------------------------[*]
void receive_check(void)	// Key data receive & protocol check.
{
	// Receive data from Rack Type DISPLAY(Key Data)
	watch();
	if(com1.rx_sp != com1.rx_ep)	{
		if(com1.cat_flag != OFF)	{
			protocol_event(&com1, com1.rx_buf[com1.rx_sp++]);
			if(com1.rx_sp >= BUFFER_SIZE)		com1.rx_sp = 0;
		}
	}

	// Receive data from Press sensor
	watch();
	if(com2.rx_sp != com2.rx_ep)    {
		if(com2.cat_flag != OFF)	{
			protocol_event(&com2, com2.rx_buf[com2.rx_sp++]);
			if(com2.rx_sp >= BUFFER_SIZE)   	com2.rx_sp = 0;
		}
	}
}
//[*]----------------------------------------------------------------------[*]
byte transmit(protocol_grp_t *comx)
{
	watch();
	if(comx->tx_sp != comx->tx_ep)  {
		if(0x20 & (input(LSR(comx->base)))) {
			output(THR(comx->base), comx->tx_buf[comx->tx_sp++]);
			if(comx->tx_sp >= BUFFER_SIZE)  	comx->tx_sp = 0;
		}                                                           return  1;
	}   else                                                        return  0;
}
//[*]----------------------------------------------------------------------[*]
void transmit_check(void)	// LED data send.
{
	watch();
	if(com1.tx_sp != com1.tx_ep)	{
		if(com1.tx_flag)	{
			disable(EA);
			SBUF = com1.tx_buf[com1.tx_sp++];
			enable(EA);
			if(com1.tx_sp >= BUFFER_SIZE)	com1.tx_sp = 0;
			com1.tx_flag = OFF;
		}
	}	else	com1.cat_flag = ON;

	// Press Sensor output Port
	if(com2.tx_sp != com2.tx_ep)    {
		if(0x20 & (input(LSR(com2.base))))  {
			output(THR(com2.base), com2.tx_buf[com2.tx_sp++]);
			if(com2.tx_sp >= BUFFER_SIZE)   com2.tx_sp = 0;
		}   com2.tx_flag = ON;
	}   else    com2.cat_flag = ON;
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] void EX0_int(void)
{
	disable(EX0);		rtcc_read();

	if(old.ss != t.ss)	{


		if(!Real_flag)		Real_flag = ON;
		old.yy = t.yy;	old.mm = t.mm;	old.dd = t.dd;
		old.hh = t.hh;	old.bb = t.bb;	old.ss = t.ss;

		if((old.bb % 10 == 0) && (old.ss == 0))	{
			Min10_flag = ON;	Min10_count = 0;
		}
		if(Hour_check != old.hh)	{
			Hour_check = old.hh;
			Hour_flag  = ON;	Hour_count = 0;
		}
		if((old.ss % 5)	== 0)	{
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
interrupt [0x13] void EX1_int (void)
{
	disable(EA);
	if      (0x04 & (input(IIR(com2.base)) & 0x6))  {
		com2.rx_buf[com2.rx_ep++] = input(RBR(com2.base));
		if(com2.rx_ep >= BUFFER_SIZE)   com2.rx_ep = 0;
	}
	enable (EA);
}
//[*]---------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)	// Serial Interrupt processing routine.
{
	disable(EA);
	if(RI)	{
		disable(RI);	RX_LED = ON;
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUFFER_SIZE)	com1.rx_ep = 0;
	}	else if(TI)		{
		disable(TI);	TX_LED = ON;
		if(com1.tx_sp != com1.tx_ep)	{
			SBUF = com1.tx_buf[com1.tx_sp++];
			if(com1.tx_sp >= BUFFER_SIZE)	com1.tx_sp = 0;
		} 	else	{
			com1.tx_flag = ON;	TX_LED = OFF;
		}
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x2B] void T2_int (void)     // Error Detect
{
	disable(TR2);       disable(TF2);

	// Sensor data Receive Check count
	if(Press_wait_count > SENSOR_WAIT)	Press_init_flag = ON;
	else                            	Press_wait_count++;

	TH2 = 0x00;     TL2 = 0x00;         enable (TR2);
}
//[*]---------------------------------------------------------------------[*]
void main(void)
{
	watch();			rtcc_init();		func_init();
	struct_init();		variable_init();

	while(1)
	{
		watch();				transmit_check();		receive_check();
		if(Real_flag )			select[Real_count ++].proc_func(REAL );
		if(Min10_flag)			select[Min10_count++].proc_func(MIN10);
		if(Hour_flag )			select[Hour_count ++].proc_func(HOUR );

		watch();				transmit_check();		receive_check();
		if(Real_count  >= SENSOR_SU)	{	Real_count = 0;	Real_flag = OFF; }
		if(Min10_count >= SENSOR_SU)	{	Min10_count= 0;	Min10_flag= OFF; }
		if(Hour_count  >= SENSOR_SU)	{	Hour_count = 0;	Hour_flag = OFF; }
		watch();				transmit_check();		receive_check();

		if(Send_flag)			{	send_proc();	Send_flag = OFF; }
		if(Press_init_flag)			press_init(&com2);
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]






