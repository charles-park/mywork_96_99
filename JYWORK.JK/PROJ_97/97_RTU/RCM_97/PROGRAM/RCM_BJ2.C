//[*]----------------------------------------------------------------------[*]
//   RCM_BJ2.C File (Main File)
//[*]----------------------------------------------------------------------[*]
#include "RCM_BJ2.H"
#include "SERIAL.HPP"
#include "SEND.HPP"
#include "CHK&CAT.HPP"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil    Date : 1997. 3. 26~     [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : BANG-JA 2 CHA                       [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD : RCM BOARD                                  [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 1.xx                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]----------------------------------------------------------------------[*]
//   Serial Port Initiliaze( Protocol & Protocol Size & Baudrate )
//[*]----------------------------------------------------------------------[*]
void Initiliaze(void)
{
	Watch();
	LogID = Read_LogID();
	//[*]-----------------------------------------------------------------[*]
	//   Internal Serial Initiliaze
	//[*]-----------------------------------------------------------------[*]
	Init_Serial_Grp  (&Com1, INTERNAL, 0x0000, BPS9600, M11,
					  SMOD_OFF, NOT_USED, 2);
	Init_Serial_Pro  (&Com1, 0, 18, Rx_chk, Rx1_cat1);
	Init_Serial_Pro  (&Com1, 1, 46, Rx_chk, Rx1_cat2);
	Set_Protocol_Flag(&Com1, 0, ON);
	Set_Protocol_Flag(&Com1, 1, OFF);
	//[*]-----------------------------------------------------------------[*]
	//   External Serial 1 Initiliaze
	//[*]-----------------------------------------------------------------[*]
	Init_Serial_Grp  (&Com2, EXTERNAL, 0x8000, BPS2400, M1,
					  NOT_USED,(DATABIT8|STOP1|PARITY_NO), 2);
	Init_Serial_Pro  (&Com2, 0, 18, Rx_chk, Rx2_cat1);
	Init_Serial_Pro  (&Com2, 1, 24, Rx_chk, Rx2_cat2);
	Set_Protocol_Flag(&Com2, 0, ON);
	Set_Protocol_Flag(&Com2, 1, OFF);
	//[*]-----------------------------------------------------------------[*]
	//   External Serial 2 Initiliaze
	//[*]-----------------------------------------------------------------[*]
	Init_Serial_Grp  (&Com3, EXTERNAL, 0x9000, BPS2400, M1,
					  NOT_USED,(DATABIT8|STOP1|PARITY_NO), 2);
	Init_Serial_Pro  (&Com3, 0, 18, Rx_chk, Rx3_cat1);
	Init_Serial_Pro  (&Com3, 1, 24, Rx_chk, Rx3_cat2);
	Set_Protocol_Flag(&Com3, 0, ON);
	Set_Protocol_Flag(&Com3, 1, OFF);
	//[*]-----------------------------------------------------------------[*]
	TMOD |= 0x1;		enable(TR0);		enable(ET0);
	disable(IT0);		enable(IT1);
	enable(EX0);		enable(EX1);
	enable(EA);         Watch();
	//[*]-----------------------------------------------------------------[*]
	mpm_check_count = 0   ;		event_count      = 0  ;		number = 1201   ;
	event_flag      = OFF ;		event_check_flag = OFF;		HAN_DCD_CON(OFF);
	volt_watch.ch   = V12M;		volt_watch.count = 0  ;
	volt_watch.flag = OFF ;		ack_check_count  = 0  ;
}
//[*]----------------------------------------------------------------------[*]
//   Read Jumper ( Logger ID )
//[*]----------------------------------------------------------------------[*]
Byte Read_LogID(void)
{
	Byte	value = 0, count = 0;

	Watch();
	value = P1;

	while(1)	{
		if(value == P1)			count++;
		else	{	count = 0;	value = P1;	}
		if(count>10)	break;
	}
	value = (~value & 0x0f);	return value ;
}
//[*]----------------------------------------------------------------------[*]
//   RTU(LOGGER) POWER RESET Function
//[*]----------------------------------------------------------------------[*]
void rtu_reset(void)
{
	Word	i;
	Watch();
	POWER_RESET(ON);
	for(i=0; i<50000; i++)	{	Watch();	i++;	i--;	}
	for(i=0; i<50000; i++)	{	Watch();	i++;	i--;	}
	POWER_RESET(OFF);
	Watch();
}
//[*]----------------------------------------------------------------------[*]
//   MPM RESET Function
//[*]----------------------------------------------------------------------[*]
void mpm_reset(void)
{
	Word	i;
	Watch();
	MPM_RESET(ON);
	for(i=0; i<1000; i++)	{	Watch();	i++;	i--;	}
	mpm_check_count = 0;
	MPM_RESET(OFF);
	Watch();
}
//[*]----------------------------------------------------------------------[*]
//   Event Save Point
//[*]----------------------------------------------------------------------[*]
int event_point(int step)
{
	int hear = 0;
	Watch();
	hear = event_ep + step;
	if     (hear <        0 )		hear += BUF_SIZE;
	else if(hear >= BUF_SIZE)		hear -= BUF_SIZE;
	return	hear;
}
//[*]----------------------------------------------------------------------[*]
//   Event Send Point
//[*]----------------------------------------------------------------------[*]
int ev_point(int step)
{
	int hear = 0;
	Watch();
	if(event_count >= BUF_SIZE)	{
		hear = ((event_ep + 1) % BUF_SIZE);
		return	hear;
	}
	hear = event_ep + step;
	if     (hear <        0 )		hear += BUF_SIZE;
	else if(hear >= BUF_SIZE)		hear -= BUF_SIZE;
	return	hear;
}
//[*]----------------------------------------------------------------------[*]
//   Save End Pointer Increment & Q Running.
//[*]----------------------------------------------------------------------[*]
void inc_Q_ep_point(void)
{
	Watch();
	event_ep++   ;
	event_count++;
	event_ep %= BUF_SIZE;
	if(event_count >= BUF_SIZE)			event_count = BUF_SIZE;
}
//[*]----------------------------------------------------------------------[*]
//   Event Check(Power & LOGGER Status)
//[*]----------------------------------------------------------------------[*]
void event_check(void)
{
	Word i;

	if(event_flag != OFF)	{	Watch();	rtu_event_save();		return;	}

	for(i = 0; i < 4; i++)	{
		if(mpm_sta.send.bd[i]  != rtu_watch.event[event_point(-1)].bd[i]	)
		{	Watch();		rtu_event_save();			return;	}
	}
	for(i = 0; i < 8; i++)	{
		if(mpm_sta.send.sen[i] != rtu_watch.event[event_point(-1)].sen[i]	)
		{	Watch();		rtu_event_save();			return;	}
	}
}
//[*]----------------------------------------------------------------------[*]
//   Event Data Save Function
//[*]----------------------------------------------------------------------[*]
void rtu_event_save(void)
{
	Word 	i = 0;

	Watch();
	rtu_watch.event[event_ep].xor = 0;
	rtu_watch.event[event_ep].add = 0;

	for(i=0;i<4;i++)	{
		rtu_watch.event[event_ep].head[i] = 0xFF;
		rtu_watch.event[event_ep].tail[i] = 0xFE;
	}
	rtu_watch.event[event_ep].cmd  = 'Q';
	rtu_watch.event[event_ep].year = mpm_sta.send.year;
	rtu_watch.event[event_ep].mon  = mpm_sta.send.mon ;
	rtu_watch.event[event_ep].day  = mpm_sta.send.day ;
	rtu_watch.event[event_ep].hour = mpm_sta.send.hour;
	rtu_watch.event[event_ep].min  = mpm_sta.send.min ;
	rtu_watch.event[event_ep].sec  = mpm_sta.send.sec ;

	if(volt_watch.p12 <  10)     rtu_watch.event[event_ep].p12 = FAIL  ;
	else                         rtu_watch.event[event_ep].p12 = NORMAL;
	if(volt_watch.p5  <   4)     rtu_watch.event[event_ep].p5  = FAIL  ;
	else                         rtu_watch.event[event_ep].p5  = NORMAL;
	if(volt_watch.m12 > -10)     rtu_watch.event[event_ep].m12 = FAIL  ;
	else                         rtu_watch.event[event_ep].m12 = NORMAL;

	Watch();

	for(i=0;i<4;i++)
		rtu_watch.event[event_ep].bd[i]   =  mpm_sta.send.bd[i] ;
	for(i=0;i<8;i++)
		rtu_watch.event[event_ep].sen[i]  =  mpm_sta.send.sen[i];

	rtu_watch.event[event_ep].rtu[0] = (mpm_sta.send.rtu[0] | event_flag);
	rtu_watch.event[event_ep].rtu[1] = (mpm_sta.send.rtu[1]             );

	for(i=0; i<sizeof(rtu_watch_db_t); i++)	{
		if(i>3 && i<sizeof(rtu_watch_db_t) - 6)		{
			rtu_watch.event[event_ep].xor ^= rtu_watch.Bytes[i + event_ep * sizeof(rtu_watch_db_t)];
			rtu_watch.event[event_ep].add += rtu_watch.Bytes[i + event_ep * sizeof(rtu_watch_db_t)];
			Watch();
		}
	}
	inc_Q_ep_point()      ;
	event_check_flag = OFF;				event_flag  = OFF;
}
//[*]----------------------------------------------------------------------[*]
//   Voltage Watch Function
//[*]----------------------------------------------------------------------[*]
void voltage_watch(void)	// Voltage Event Check & Event Save
{
	float cal = 0;

	Watch();
	switch(volt_watch.ch)		{
		case	V12M:
			cal            = input(ADC(volt_watch.ch)) * 0.019453125;
			cal            = cal * 5.484460695;
			volt_watch.m12 = (float)cal * (-1);
			volt_watch.ch  = V12;	volt_watch.count = 0;
			if(volt_watch.m12 > -10)	{ 	event_check_flag = ON;	}
			break;
		case    V12 :
			cal            = input(ADC(volt_watch.ch)) * 0.019453125;
			cal            = cal * 4.034969738;
			volt_watch.p12 = (float)cal;
			volt_watch.ch  = V5;    volt_watch.count = 0;
			if(volt_watch.p12 < 10)		{ 	event_check_flag = ON;	}
			break;
		case	V5  :
			cal            = input(ADC(volt_watch.ch)) * 0.019453125;
			cal            = cal * 1.996805112;
			volt_watch.p5  = (float)cal;
			volt_watch.ch  = V12M;  volt_watch.count = 0;
			if(volt_watch.p5  < 4)		{ 	event_check_flag = ON;	}
			break;
	}
	volt_watch.flag = OFF;				output(ADC(volt_watch.ch),0x0);
}
//[*]----------------------------------------------------------------------[*]
//   RTU & MPM Watch
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int (void)      // Timer 0 Overflow
{
	disable(EA);
	mpm_check_count++;
	volt_watch.count++;
	if(volt_watch.flag == ON)	volt_delay_count++;
	else						volt_delay_count = 0;
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
//   A/D Convert End of Convert
//[*]----------------------------------------------------------------------[*]
interrupt [0x13] void EX1_int (void)     // External Interrupt 1
{
	disable(EA);
	volt_watch.flag = ON;
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
//   Start Main Progran Start !!!!!
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	Watch();
	Initiliaze();                               // Serial & Protocol Init
	POWER_RESET(OFF);

	while(1)	{
		receive_check ();		Watch();		// Receive  Buffer Check
		transmit_check();       Watch();		// Transive Buffer Check

		if(event_check_flag) 			event_check();		Watch();

		if(mpm_check_count >= MAX_WAIT)		{
			mpm_reset ();				Watch();
			event_flag = RESET_MPM;		event_check_flag = ON;
		}

		if(volt_watch.flag && volt_delay_count > 10)		voltage_watch();

		if(volt_watch.count >= 100)	{
			output(ADC(volt_watch.ch),0x0) ;		volt_watch.count = 0;
		}
		Watch();
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
