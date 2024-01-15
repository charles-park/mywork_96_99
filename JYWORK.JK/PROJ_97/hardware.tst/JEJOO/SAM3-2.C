#include <io51.h>
#include <math.h>
#include <sam3-2.h>
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil    Date : 1997. 2. 26~     [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  Sensor : TEMPERLATURE 1 EA  Analog CH : 0, ID : 0  [*]",
   "[*]           HUMIDUTIY    1 EA  Analog CH : 1, ID : 1  [*]",
   "[*]  Sensor Code : TEMP = 2, HUMI = 6                   [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  Used Board : SAM3 Board(Designed PARKIK)           [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  VERSION : Test Ver 1.0  First TEST : 97. 3. 10     [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]"
};
#pragma memory=default
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]  Program Start......                                                 [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_variable(void)
{
	// TEMP & HUMI Old Value Register Clear (Variable initliaze)
	TEMP_OLD.Maxt    = TEMP_OLD.Sumt    = TEMP_OLD.Mint    = 0 ;
	TEMP_OLD.MaxO    = TEMP_OLD.SumO    = TEMP_OLD.MinO    = 0 ;
	TEMP_OLD.MaxT    = TEMP_OLD.SumT    = TEMP_OLD.MinT    = 0 ;
	TEMP_OLD.Count_t = TEMP_OLD.Count_O = TEMP_OLD.Count_T = 0 ;

	HUMI_OLD.Maxt    = HUMI_OLD.Sumt    = HUMI_OLD.Mint    = 0 ;
	HUMI_OLD.MaxO    = HUMI_OLD.SumO    = HUMI_OLD.MinO    = 0 ;
	HUMI_OLD.MaxT    = HUMI_OLD.SumT    = HUMI_OLD.MinT    = 0 ;
	HUMI_OLD.Count_t = HUMI_OLD.Count_O = HUMI_OLD.Count_T = 0 ;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//	for(MovCount = 0; MovCount<AvrCount; MovCount++)	{
//		TEMP_Move[MovCount] = HUMI_Move[MovCount] = 0;
//	}

//	TEMP_Point = 0		;
//	HUMI_Point = 0		;
//	MovCount   = 0 		;
//	MovValue   = 0 		;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
	ERcount	  = 0;			// A/D Error Count
	ERROR	  = 0;          // Error Status Register
	DLYcount  = 0;          // A/D Delay Count Value
	ADValue	  = 0;			// Read A/D Converter Value (Value type is int)
	Event	  = 0;          // 4 Hz Counter Value(Used interrupt EX0)
	Init	  = 1;			// Initiliaze Min & Max Value Initiliaze
	Flag      = OFF;		// Select Loop Run & Stop Flag (Used interrupt EX0)

	TEMP_R.Sam.Avr = 500;	// Temperature Default Value Setting

	MUX0 = OFF;		MUX1 = OFF;		MUX2 = OFF;		// A/D Convert Select(First Temperature)
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_struct(Sensor_u *Sen, Byte Cmd, Byte Sid, Byte Scode)
{
	Sen->Sam.Head   = 0xFFFF;	// 0,1   Header Value.
	Sen->Sam.Cmd    = Cmd 	;   // 2	 'M','t','O','T' Command.
	Sen->Sam.Sta    = 0     ;   // 3	 Sensor Status Value
	Sen->Sam.Scode 	= Scode	;   // 4	 Sensor Code
	Sen->Sam.Sid   	= Sid	;   // 5	 This Porgram Sid is Fixed.(Jumper ID)
	Sen->Sam.Avr   	= 0		;   // 6,7   Average Value
	Sen->Sam.Spc   	= 0		;   // 8,9
	Sen->Sam.Max   	= 0		;   // 10,11 Maximum Value.
	Sen->Sam.Min   	= 0		;   // 12,13 Minimum Value.
	Sen->Sam.Cks   	= 0		;   // 14    Check Sum Value.
	Sen->Sam.Tail  	= 0xFFFE;   // 15,16 Tail.	             Total = 17 Bytes
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_serial(void)
{
	Com.TxSp = Com.TxEp = Com.RxSp = Com.RxEp = 0;

	disable(EA);
	PCON = 0x80;      // Baudrate Double set
	SCON = 0x50;      // Serial Config Register
	TMOD = 0x25;      // T0 = Internal Timer 16Bit no,Prescaler,
					  // T1 = Using to Serial
	TH1  = 0xfa;
	TL1  = 0xfa;      // 23Mhz = F3, 22Mhz = F4,18Mhz = F6, 11Mhz = fa Set
	TR1  = 1   ;      // Serial Timer Run
	PS   = 1   ;	  // Serial Interrupt First.

	IT0  = 1   ;      // Edge Trigger
	EX0  = 1   ;      // 4 Hz Clock Enalble
	ES   = 1   ;      // Serial Interrupt Enable
	enable(EA) ;       // Interrupt All enable
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void initiliaze(void)
{
	// Jumper_Read(); Jumper Read Function
	disable(GREENL);	disable(REDL);		// LED Clear (TXD & RXD)

	// Temp Struct Initiliaze
	init_struct(&TEMP_R, REAL , SID0, SCODE0);
	init_struct(&TEMP_t, SEC10, SID0, SCODE0);
	init_struct(&TEMP_O, MIN1 , SID0, SCODE0);
	init_struct(&TEMP_T, MIN10, SID0, SCODE0);
	// Humi Struct Initiliaze
	init_struct(&HUMI_R, REAL , SID1, SCODE1);
	init_struct(&HUMI_t, SEC10, SID1, SCODE1);
	init_struct(&HUMI_O, MIN1 , SID1, SCODE1);
	init_struct(&HUMI_T, MIN10, SID1, SCODE1);

	init_variable(); 		// Variable All initiliaze
	init_serial();			// Serial Port initiliaze
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void check_mpm(void) 	// Protocol Check MPM -> SAM Board
{
	Byte	   CK		  =	0;
	Word 	   Count	  =	0;
	bit 	   TEMP_HUMI  = 0;

	// RxSp is Point(Center), RxEp increment is interrupt.

	if(Com.RxSp == Com.RxEp)					return; get_serial();
	if(Com.RxBuf[Loc(Com.RxSp,-1) ] != 0xfe)	return;	disable(GREENL);
	if(Com.RxBuf[Loc(Com.RxSp,-2) ] != 0xff)	return;
	if(Com.RxBuf[Loc(Com.RxSp,-10)] != 0xff)	return;
	if(Com.RxBuf[Loc(Com.RxSp,-11)] != 0xff)	return;
	if(Com.RxBuf[Loc(Com.RxSp,-8) ] != SID0 &&
	   Com.RxBuf[Loc(Com.RxSp,-8) ] != SID1)	return;

	for(Count=0; Count<sizeof(Command_t); Count++)  {
		if(Count>1 && Count<8)   CK ^= Com.RxBuf[Loc(Com.RxSp,-(11-Count))];
		Mpm.Bytes[Count] 		     = Com.RxBuf[Loc(Com.RxSp,-(11-Count))];
	}
	if(Mpm.Mpm.Cks != CK)						return;

	TEMP_HUMI = (Mpm.Mpm.Sid == SID0) ?	1 : 0;

	switch(Mpm.Mpm.Cmd)	{
		case	REAL :  	if(TEMP_HUMI)	send_struct(&TEMP_R);
							else			send_struct(&HUMI_R);	break;
		case	SEC10:      if(TEMP_HUMI)	send_struct(&TEMP_t);
							else			send_struct(&HUMI_t);	break;
		case	MIN1 :      if(TEMP_HUMI)	send_struct(&TEMP_O);
							else			send_struct(&HUMI_O);	break;
		case	MIN10:      if(TEMP_HUMI)	send_struct(&TEMP_T);
							else			send_struct(&HUMI_T);	break;
		case	INIT :      Init = 1;								break;
		case	RST	 :
			TEMP_t.Sam.Max = Mpm.Mpm.Max;	TEMP_t.Sam.Min = Mpm.Mpm.Min;
			TEMP_O.Sam.Max = Mpm.Mpm.Max;	TEMP_O.Sam.Min = Mpm.Mpm.Min;
			TEMP_T.Sam.Max = Mpm.Mpm.Max;	TEMP_T.Sam.Min = Mpm.Mpm.Min;
			HUMI_t.Sam.Max = Mpm.Mpm.Max;	HUMI_t.Sam.Min = Mpm.Mpm.Min;
			HUMI_O.Sam.Max = Mpm.Mpm.Max;	HUMI_O.Sam.Min = Mpm.Mpm.Min;
			HUMI_T.Sam.Max = Mpm.Mpm.Max;	HUMI_T.Sam.Min = Mpm.Mpm.Min;
																	break;
		default		 :												break;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_struct(Sensor_u *Send)   // SAM -> MPM DATA Transmit
{
	Word	Count;

	Send->Sam.Sta = ( ERROR & 0 );		// Error Flag Setting
	for(Count=0; Count<sizeof(Sensor_t); Count++)   {
		if(Count>1 && Count <14)	Send->Sam.Cks ^= Send->Bytes[Count];
		Com.TxBuf[Com.TxEp++] = Send->Bytes[Count];
		if(Com.TxEp >= MAX)		Com.TxEp = 0;
	}
	enable(REDL);
	SBUF = Com.TxBuf[Com.TxSp];
	Com.TxSp = Loc(Com.TxSp,+1);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
signed int Loc(signed int Comp, signed int Step)    // Q Buffer Point Set
{
	Comp += Step;
	if(Comp >= MAX)		Comp -= MAX;
	if(Comp < 0)  		Comp += MAX;
	return	Comp;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void AD_read(void)		// A/D Convert DATA Read Function
{
	ADCENB = HOLD;  		// Voltage is Holding
	for(DLYcount=0; DLYcount < DLY; DLYcount++) { DLYcount++; DLYcount--;}

	output(ADMSB, 0xff);    // A/D Convert Start Command

	for(DLYcount=0; DLYcount < DLY; DLYcount++) { DLYcount++; DLYcount--;}
	ERcount=0;				// Error Count Value Clear
	while(STATUS == ON)		{	if(ERcount++>100)	break;  }

	if(ERcount>100)		ERROR |= ADERR	;
	else				ERROR = 0		;

	ADValue = 0;
	disable(ES);
	ADValue = (input(ADLSB) >> 4);		ADValue |= (input(ADMSB) << 4);
	enable(ES);
	ADCENB  = SAMPLE;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Temp_cal(void)        // TEMP Calculation Function
{
	AD_read();
	/////////////////
//	TEMP_Move[TEMP_Point++] 	=	 ADValue;
//	if(TEMP_Point >= AvrCount)		 TEMP_Point = 0;
//	for(MovCount = 0; MovCount<AvrCount ; MovCount++)	MovValue += TEMP_Move[MovCount];
//	ADValue 	 = MovValue/AvrCount;
//	MovValue 	 = 0;
	/////////////////
	disable(ES);
	TEMP_OLD.Real  =  (ADValue / 4.095) + 500.;   // Temp Cal.
	if	   (TEMP_OLD.Real > 1500)   TEMP_OLD.Real = 1500;
	else if(TEMP_OLD.Real < 500 )   TEMP_OLD.Real =  500;
	TEMP_R.Sam.Avr =  TEMP_OLD.Real           ;
	// One Day Max, Min processing
	if(TEMP_OLD.Real > TEMP_R.Sam.Max)	TEMP_R.Sam.Max = TEMP_OLD.Real;
	if(TEMP_OLD.Real < TEMP_R.Sam.Min)	TEMP_R.Sam.Min = TEMP_OLD.Real;
	enable(ES);
				///
	TEMP_OLD.Sumt += TEMP_OLD.Real;		// Average Value 10 sec
	TEMP_OLD.SumO += TEMP_OLD.Real;		// Average Value 1  min
	TEMP_OLD.SumT += TEMP_OLD.Real;		// Average Value 10 min
	// 10 sec, 1 min, 10 min MAX & MIN proc...
	if(TEMP_OLD.Real > TEMP_OLD.Maxt )	TEMP_OLD.Maxt  = TEMP_OLD.Real;
	if(TEMP_OLD.Real < TEMP_OLD.Mint )  TEMP_OLD.Mint  = TEMP_OLD.Real;
	if(TEMP_OLD.Real > TEMP_OLD.MaxO )  TEMP_OLD.MaxO  = TEMP_OLD.Real;
	if(TEMP_OLD.Real < TEMP_OLD.MinO )	TEMP_OLD.MinO  = TEMP_OLD.Real;
	if(TEMP_OLD.Real > TEMP_OLD.MaxO )  TEMP_OLD.MaxT  = TEMP_OLD.Real;
	if(TEMP_OLD.Real < TEMP_OLD.MinO )	TEMP_OLD.MinT  = TEMP_OLD.Real;
	// 10 sec proc...
	if(TEMP_OLD.Count_t >= 10)	{
		disable(ES);
		TEMP_t.Sam.Avr   = (Word)(TEMP_OLD.Sumt/10.)          ;
		enable(ES);
		TEMP_t.Sam.Max   = TEMP_OLD.Maxt                      ;
		TEMP_t.Sam.Min   = TEMP_OLD.Mint                      ;
		TEMP_OLD.Maxt    = TEMP_OLD.Mint = TEMP_OLD.Real	  ;
		TEMP_OLD.Sumt    = 0  ;
	}
	// 1 min proc...
	if(TEMP_OLD.Count_O >= 60)	{
		disable(ES);
		TEMP_O.Sam.Avr   = (Word)(TEMP_OLD.SumO/60.)          ;
		enable(ES);
		TEMP_O.Sam.Max   = TEMP_OLD.MaxO                      ;
		TEMP_O.Sam.Min   = TEMP_OLD.MinO                      ;
		TEMP_OLD.MaxO    = TEMP_OLD.MinO = TEMP_OLD.Real      ;
		TEMP_OLD.SumO    = 0  ;
	}
	// 10 min proc...
	if(TEMP_OLD.Count_T >= 600)	{
		disable(ES);
		TEMP_T.Sam.Avr   = (Word)(TEMP_OLD.SumT/600.)         ;
		enable(ES);
		TEMP_T.Sam.Max   = TEMP_OLD.MaxT                      ;
		TEMP_T.Sam.Min   = TEMP_OLD.MinT                      ;
		TEMP_OLD.MaxT    = TEMP_OLD.MinT = TEMP_OLD.Real      ;
		TEMP_OLD.SumT    = 0  ;
	}

	if(TEMP_OLD.Count_t >= 10)			TEMP_OLD.Count_t = 0;
	else								TEMP_OLD.Count_t++  ;
	if(TEMP_OLD.Count_O >= 60)			TEMP_OLD.Count_O = 0;
	else								TEMP_OLD.Count_O++  ;
	if(TEMP_OLD.Count_T >= 600)			TEMP_OLD.Count_T = 0;
	else								TEMP_OLD.Count_T++  ;

	disable(ES);
	MUX0 = 1;	  MUX1 = 0;	    MUX2 = 0 ;  // Humi CH Select
	Flag = OFF;				 // Disable Flag . (Select.Loop Used. Flag Off)
	enable(ES);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Humi_cal(void)        // HUMI Calculation Function
{
	AD_read();
	///////////////////////////
//	HUMI_Move[HUMI_Point++] 	= ADValue;
//	if(HUMI_Point >= AvrCount)	  HUMI_Point = 0;
//	for(MovCount = 0; MovCount<AvrCount ; MovCount++)	MovValue += HUMI_Move[MovCount];
//	ADValue  	 = MovValue/AvrCount;
//	MovValue 	 = 0;
	///////////////////////////
	disable(ES);
	HUMI_OLD.Real  = ADValue / 4.095;      // HUMI Cal.
	if(HUMI_OLD.Real > 999)		HUMI_OLD.Real = 999;	// HUMI MAX SETTING
	HUMI_R.Sam.Avr = HUMI_OLD.Real;
	enable(ES);

	HUMI_OLD.Sumt += HUMI_OLD.Real;        // Average Value 10 sec
	HUMI_OLD.SumO += HUMI_OLD.Real;        // Average Value 1  min
	HUMI_OLD.SumT += HUMI_OLD.Real;        // Average Value 10 min
	// One Day Max, Min processing
	if(HUMI_OLD.Real > HUMI_R.Sam.Max)	HUMI_R.Sam.Max = HUMI_OLD.Real;
	if(HUMI_OLD.Real < HUMI_R.Sam.Min)	HUMI_R.Sam.Min = HUMI_OLD.Real;
	// 10 sec, 1 min, 10 min MAX & MIN proc...
	if(HUMI_OLD.Real > HUMI_OLD.Maxt )	HUMI_OLD.Maxt  = HUMI_OLD.Real;
	if(HUMI_OLD.Real < HUMI_OLD.Mint )	HUMI_OLD.Mint  = HUMI_OLD.Real;
	if(HUMI_OLD.Real > HUMI_OLD.MaxO )  HUMI_OLD.MaxO  = HUMI_OLD.Real;
	if(HUMI_OLD.Real < HUMI_OLD.MinO )	HUMI_OLD.MinO  = HUMI_OLD.Real;
	if(HUMI_OLD.Real > HUMI_OLD.MaxO )  HUMI_OLD.MaxT  = HUMI_OLD.Real;
	if(HUMI_OLD.Real < HUMI_OLD.MinO )	HUMI_OLD.MinT  = HUMI_OLD.Real;
	// 10 sec proc...
	if(HUMI_OLD.Count_t >= 10)	{
		disable(ES);
		HUMI_t.Sam.Avr   = (Word)(HUMI_OLD.Sumt/10.)          ;
		enable(ES);
		HUMI_t.Sam.Max   = HUMI_OLD.Maxt                      ;
		HUMI_t.Sam.Min   = HUMI_OLD.Mint                      ;
		HUMI_OLD.Maxt    = HUMI_OLD.Mint =	HUMI_OLD.Real	  ;
		HUMI_OLD.Sumt    = 0  ;
	}
	// 1 min proc...
	if(HUMI_OLD.Count_O >= 60)	{
		disable(ES);
		HUMI_O.Sam.Avr   = (Word)(HUMI_OLD.SumO/60.)          ;
		enable(ES);
		HUMI_O.Sam.Max   = HUMI_OLD.MaxO                      ;
		HUMI_O.Sam.Min   = HUMI_OLD.MinO                      ;
		HUMI_OLD.MaxO    = HUMI_OLD.MinO = HUMI_OLD.Real      ;
		HUMI_OLD.SumO    = 0  ;
	}
	// 10 min proc...
	if(HUMI_OLD.Count_T >= 600)	{
		disable(ES);
		HUMI_T.Sam.Avr   = (Word)(HUMI_OLD.SumT/600.)         ;
		enable(ES);
		HUMI_T.Sam.Max   = HUMI_OLD.MaxT                      ;
		HUMI_T.Sam.Min   = HUMI_OLD.MinT                      ;
		HUMI_OLD.MaxT    = HUMI_OLD.MinT = HUMI_OLD.Real      ;
		HUMI_OLD.SumT    = 0  ;
	}

	if(HUMI_OLD.Count_t >= 10)			HUMI_OLD.Count_t = 0;
	else								HUMI_OLD.Count_t++  ;
	if(HUMI_OLD.Count_O >= 60)			HUMI_OLD.Count_O = 0;
	else								HUMI_OLD.Count_O++  ;
	if(HUMI_OLD.Count_T >= 600)			HUMI_OLD.Count_T = 0;
	else								HUMI_OLD.Count_T++  ;

	disable(ES);
	MUX0 = 0;     MUX1 = 0;     MUX2 = 0 ;	// Temp CH Select
	Flag = OFF;				 // Disable Flag . (Select.Loop Used. Flag Off)
	enable(ES);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]  Interrupt Function Start....                                        [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void SCON_int(void)	// Serial Interrupt Function
{
	disable(EA);
	if(RI)	{
		disable(RI);			enable(GREENL);
		Com.RxBuf[Com.RxEp++] = SBUF;
		if(Com.RxEp >= MAX)		Com.RxEp = 0;
		check_mpm();			 // Portocol Check. ( MPM -> SAM )
	}
	else if(TI)	{
		disable(TI);
		if(Com.TxSp != Com.TxEp)	SBUF = Com.TxBuf[Com.TxSp++];
		else						disable(REDL);
		if(Com.TxSp >= MAX)			Com.TxSp = 0;
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void EX0_int(void)    // External Interrupt Function
{
	disable(EA);
	switch(Event)
	{
		case	0:  Loop.Select = Temp_cal;		Flag = ON;
					Event++;   									  break;
		case	1:  Event++;									  break;
		case	2:	Loop.Select = Humi_cal;		Flag = ON;
					Event++;									  break;
		case    3:	Event=0 ;   								  break;
		default	 :												  break;
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]    Main Program Start...                                             [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	initiliaze();    		   // Initiliaze Serial & Variable & Structs.

	while(1)  	{
		if(Init == 1)	{      // Min & Max Value Initiliaze
			disable(EA);
			TEMP_t.Sam.Max = TEMP_OLD.Real;  TEMP_t.Sam.Min = TEMP_OLD.Real;
			TEMP_O.Sam.Max = TEMP_OLD.Real;  TEMP_O.Sam.Min = TEMP_OLD.Real;
			TEMP_T.Sam.Max = TEMP_OLD.Real;  TEMP_T.Sam.Min = TEMP_OLD.Real;

			HUMI_t.Sam.Max = HUMI_OLD.Real;  HUMI_t.Sam.Min = HUMI_OLD.Real;
			HUMI_O.Sam.Max = HUMI_OLD.Real;  HUMI_O.Sam.Min = HUMI_OLD.Real;
			HUMI_T.Sam.Max = HUMI_OLD.Real;  HUMI_T.Sam.Min = HUMI_OLD.Real;
			Init = 0;
			enable(EA);
		}
		if(Flag == ON)		Loop.Select();   // Run Function (Select Function)
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
