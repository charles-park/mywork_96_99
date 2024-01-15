//[*]----------------------------------------------------------------------[*]
#include <IO51.H>
#include <MATH.H>
#include <STDIO.H>
#include <STDLIB.H>
#include "EWS_5EA.H"
#include "PROS.HPP"
#include "SERIAL.HPP"
#include "CAT&CHK.HPP"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. IN KYU     Date : 1997. 8. 25      [*]",
   "[*]  Program End : 1997, 11, 20                         [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : EWS  WD & WS TEMP HUMI PRE TIME     [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Upgrade Program : PARK,JONG-KIL Date : 1998. 4. 6~ [*]",
   "[*]  Program End : 1998, 4, 6                           [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 1.00                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int (void)	{	return  ;	}
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
	}
	t.yy = input(RTCC_YEAR );	t.mm = input(RTCC_MONTH);
	t.dd = input(RTCC_DAY  );	t.hh = input(RTCC_HOUR );
	t.bb = input(RTCC_MIN  );	t.ss = input(RTCC_SEC  );
	if((t.yy > 99) || (t.mm > 12) || (t.dd > 31) ||
	   (t.hh > 23) || (t.bb > 59) || (t.ss > 59)  )	{
		t.yy = 97;	t.mm = 3;	t.dd = 23;
		t.hh = 7;	t.bb = 7;	t.ss = 7;
		while(rtcc_date_time_set(&t, ALL_SET));
	}
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
void save_ram_init(void)
{
	byte	i;
	watch();

	for(i=0; i < CHECK_RAM3+1; i++)		save(i, 0x00);

	// RAM CHECK DATA ( When Read Not Equl then RAM is Broken )
	save(CHECK_DATE, t.dd);		save(CHECK_RAM1, 0x55);
	save(CHECK_RAM2, 0xAA);		save(CHECK_RAM3, 0x77);
	Init_flag = ON;
}
//[*]----------------------------------------------------------------------[*]
void save_ram_load(void)
{
	watch();
	wd_pros.real_max   = maxmin_load(WD_MAXMIN  , MAX_LOAD);
	ws_pros.real_max   = maxmin_load(WS_MAXMIN  , MAX_LOAD);
}
//[*]----------------------------------------------------------------------[*]
void save_data_check(void)
{
	byte	chk_data = 0;

	watch();
	chk_data = load(CHECK_DATE);
	if(chk_data != t.dd)	{	save_ram_init();	return;	}
	chk_data = load(CHECK_RAM1);
	if(chk_data != 0x55)	{	save_ram_init();	return;	}
	chk_data = load(CHECK_RAM2);
	if(chk_data != 0xAA)	{	save_ram_init();	return;	}
	chk_data = load(CHECK_RAM3);
	if(chk_data != 0x77)	{	save_ram_init();	return;	}
	save_ram_load();	// day max & min load
}
//[*]----------------------------------------------------------------------[*]
void cal_means_read(void)
{
	byte 	i = 0, ck_id = 0, read_cal = 0;

	watch();	Read_id = input(JUMPER);

	while(1)
	{
		ck_id = input(JUMPER);
		if(Read_id == ck_id)	i++;
		else					Read_id = input(JUMPER);
		if(i > 10)				break;
	}
	if(Read_id == 0x07)			Read_id = 1;	// Default 45Hz
	else						Read_id = 0;	// Jumper  38Hz

	// Function Pointer Define
	select.wd_cal   	= wind_direction_cal;
	select.ws_cal   	= wind_speed_ms_cal;
}
//[*]----------------------------------------------------------------------[*]
void struct_initiliaze(sensor_u *sen, byte b_cmd, byte b_scod, byte b_sid)
{
	watch();
	sen->sam.header = 0xFFFF;	sen->sam.cmd    = b_cmd ;
	sen->sam.sta    = 0     ;   sen->sam.scod   = b_scod;
	sen->sam.sid    = b_sid ;	sen->sam.avr    = 0     ;
	sen->sam.spc    = 0     ;	sen->sam.min    = 0     ;
	sen->sam.max    = 0     ;	sen->sam.xor    = 0     ;
	sen->sam.tail   = 0xFFFE;
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	watch();
	// Default value setting
	Real_flag  = OFF;	Real_count = 0;
	Init_flag  = OFF;	Read_id    = 0;		Led_send   = OFF;

	init.byte_data = 0xFF;
	cal_means_read();	// Wind Speed calculate select.

	// Buzzer On/Off test.
	BUZZER(ON);    		delay(5000);		BUZZER(OFF);

	// Serial Protocol initiliaze.
	init_serial_grp  (&com, INTERNAL, 0, BPS2400, M22, SMOD_ON, NOT_USED, 1);
	init_serial_pro  (&com, 0, 11, rx_check, rx_catch);
	set_protocol_flag(&com, 0, ON);			// Key data receive

	// Led send data struct initiliaze.
	struct_initiliaze(&led[LED_DATE  ], 'N', 0x0, 3);   // date
	struct_initiliaze(&led[LED_TIME  ], 'S', 0x0, 4);   // time
	struct_initiliaze(&led[WD_LED    ], 'M', 0x0, 0);   // wd      real
	struct_initiliaze(&led[WS_LED    ], 'M', 0x1, 1);   // ws      real

	// Base time initiliaze.
	old.yy = t.yy;      old.mm = t.mm;      old.dd = t.dd;
	old.hh = t.hh;      old.bb = t.bb;      old.ss = t.ss;

	// Initialize day check
	Day_check = t.dd;

	// Interrupt initiliaze.
	TMOD |= 0x05;		TH0 = 0;			TL0 = 0;
	T2CON = 0;			TH2 = 0;			TL2 = 0;

	// Interrupt enable.
	enable(ET2);		enable(TR2);		enable(TR0);// Ws Calculate
	disable(EX0);		disable(EX1);       enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void pros_struct_init1(void)
{
	wd_pros.real_max  = wd_pros.real_avr  = wd_pros.real_sum  = 0;
	wd_pros.real_sin  = wd_pros.real_cos  = 0.;
}
//[*]----------------------------------------------------------------------[*]
void pros_struct_init2(pros_t *p)
{
	p->real_spc  = p->real_avr  = p->real_min  = p->real_max  =	p->real_sum  = 0;
}
//[*]----------------------------------------------------------------------[*]
void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)	{	i++;	i--;	watch();	}
}
//[*]----------------------------------------------------------------------[*]
void sensor_init(void)		// 0 hour data initiliaze.
{
	watch();
	wd_pros  .real_max  = wd_pros  .real_avr;
	ws_pros  .real_max  = ws_pros  .real_avr;
	ws_pros  .real_min  = ws_pros  .real_avr;
	maxmin_save(WD_MAXMIN, wd_pros.real_max, wd_pros.real_max);
	maxmin_save(WS_MAXMIN, ws_pros.real_max, ws_pros.real_min);
}
//[*]----------------------------------------------------------------------[*]
void data_pros(byte mode, byte sen_num)
{
	watch();
	switch(sen_num)	{
		case    0:  select.wd_cal     (mode);   break;
		case	1:  case	2:		case	3:	break;
		case	4:  select.ws_cal     (mode);	break;
		case	5:	case	6:		case	7:	break;
		default	 :	Real_flag  = OFF;
	}
}
//[*]----------------------------------------------------------------------[*]
void led_pros(void)
{
	watch();
	led[WD_LED     ].sam.avr = wd_pros    .real_avr;
	led[WS_LED     ].sam.avr = ws_pros    .real_avr;
	led[WD_LED     ].sam.max = wd_pros    .real_max;
	led[WS_LED     ].sam.max = ws_pros    .real_max;

	if(Init_flag)	{	Init_flag = OFF;	sensor_init();	}
	led_data_send();   	Led_send  = OFF;
}
//[*]----------------------------------------------------------------------[*]
void led_data_send(void)	// Led struct data moving to serial buffer.
{
	word	i,j;

	for(i = 0; i < 4; i++)	{
		led[i].sam.xor = 0;		watch();
		for(j = 0; j < sizeof(sensor_u); j++)	{
			if(j>1 && j<sizeof(sensor_u) -3)    led[i].sam.xor^= led[i].bytes[j];
			data_move(led[i].bytes[j]);
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void data_move(byte mov_data)
{
	com.tx_buf[com.tx_ep++] = mov_data;
	if(com.tx_ep >= BUFFER_TX)	com.tx_ep = 0;
}
//[*]----------------------------------------------------------------------[*]
void receive_check(void)	// Key data receive & protocol check.
{
	watch();

	if(com.rx_sp != com.rx_ep)	{
		if(com.cat_flag == OFF)			return;
		protocol_event(&com, com.rx_buf[com.rx_sp++]);
		if(com.rx_sp >= BUFFER_RX)		com.rx_sp = 0;
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
			if(com.tx_sp >= BUFFER_TX)	com.tx_sp = 0;
			com.tx_flag = OFF;
		}
	}	else	com.cat_flag = ON;
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)	// Serial Interrupt processing routine.
{
	disable(EA);
	if(RI)	{
		disable(RI);
		com.rx_buf[com.rx_ep++] = SBUF;
		if(com.rx_ep >= BUFFER_RX)	com.rx_ep = 0;
	}	else if(TI)		{
		disable(TI);
		if(com.tx_sp != com.tx_ep)	{
			SBUF = com.tx_buf[com.tx_sp++];
			if(com.tx_sp >= BUFFER_TX)	com.tx_sp = 0;
		} else	com.tx_flag = ON;
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x2B] void T2_int (void)		// Overflow Interrupt. RTCC read.
{
	disable(EA);       	TR2 = STOP;
	rtcc_read();
	if(old.ss != t.ss)	{
		old.yy = t.yy;		old.mm = t.mm;		old.dd = t.dd;
		old.hh = t.hh;		old.bb = t.bb;		old.ss = t.ss;

		if(Real_flag != ON)	{
			Real_flag = ON;		Real_count = 0;		// Real Processing flag
		}
		if((old.ss % 10) == 0) 	Led_send = ON;
		if(Day_check != t.dd) 	Init_flag = ON;
	}
	TH2 = 0x00;		TL2 = 0x00;		TF2 = 0x00;
	TR2 = RUN;		enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	data	word	i;
	data	byte	ch=0,ch1=0;

	BUZZER(OFF);
//	while(1)	{
//		watch();
//		output(0x0000, 0x55);
//		input (0x0000);
//	}
//	for(i=0; i<0xFFF; i++)	{
//		watch();			BUZZER(OFF);
//		output(i, ch);		ch1 = input(i);
//		if(ch1 != ch)		{	while(1)	BUZZER(ON);	}
//	}
//while(1)	{	BUZZER(OFF);	watch();	}	// test
	watch();					led16_clr();	// define function

	watch();	 				rtcc_init();
	pros_struct_init1();		pros_struct_init2(&ws_pros  );

	variable_init();			save_data_check();
	led_data_send();    		Led_send  = OFF;

	while(1)
	{
		watch();
		transmit_check();		receive_check();
		if(Real_flag )			data_pros(REAL , Real_count++ );
		if(Led_send  )			led_pros();
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
