#include <io51.h>
#include <math.h>
#include <namyang.h>
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#pragma memory=code
Byte    NAME[20][60] = {
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil    Date : 1997. 3. 5~      [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  Sensor : TEMPERLATURE 1 EA  Analog CH : 0, ID : 2  [*]",
   "[*]           SUN          1 EA  Analog CH : 2, ID : 5  [*]",
   "[*]           HUMIDUTIY    1 EA  Analog CH : 7, ID : 3  [*]",
   "[*]           PRECIPI      1 EA  EXT INT1     , ID : 4  [*]",
   "[*]           SUN/D        1 EA  P1.6 (On/Off), ID : 6  [*]",
   "[*]  Sensor Code :                                      [*]",
   "[*]    TEMP 2, SUN 8, HUMI 6 , PREC 3, SUND 9           [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  Used Board : SAM2 Board(Designed PARKIK)           [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]",
   "[*]  VERSION : Test Ver 1.0  First TEST : 97. 3.  7     [*]",
   "[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]"
};
#pragma memory=default
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]  Program Start......                                                 [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_variable(void)
{
	// Sensor Old Value Register Clear (Variable initliaze)
//[*]컴컴 Temp 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
	TEMP_OLD.Maxt    = TEMP_OLD.Sumt    = TEMP_OLD.Mint    = 0 ;
	TEMP_OLD.MaxO    = TEMP_OLD.SumO    = TEMP_OLD.MinO    = 0 ;
	TEMP_OLD.MaxT    = TEMP_OLD.SumT    = TEMP_OLD.MinT    = 0 ;
	TEMP_OLD.Count_t = TEMP_OLD.Count_O = TEMP_OLD.Count_T = 0 ;
//[*]컴컴 Humi 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
	HUMI_OLD.Maxt    = HUMI_OLD.Sumt    = HUMI_OLD.Mint    = 0 ;
	HUMI_OLD.MaxO    = HUMI_OLD.SumO    = HUMI_OLD.MinO    = 0 ;
	HUMI_OLD.MaxT    = HUMI_OLD.SumT    = HUMI_OLD.MinT    = 0 ;
	HUMI_OLD.Count_t = HUMI_OLD.Count_O = HUMI_OLD.Count_T = 0 ;
//[*]컴컴 Prec 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
	PREC_OLD.Maxt    = PREC_OLD.Sumt    = PREC_OLD.Mint    = 0 ;
	PREC_OLD.MaxO    = PREC_OLD.SumO    = PREC_OLD.MinO    = 0 ;
	PREC_OLD.MaxT    = PREC_OLD.SumT    = PREC_OLD.MinT    = 0 ;
	PREC_OLD.Count_t = PREC_OLD.Count_O = PREC_OLD.Count_T = 0 ;
//[*]컴컴 Sun 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]
	SUN_OLD.Maxt     = SUN_OLD.Sumt     = SUN_OLD.Mint     = 0 ;
	SUN_OLD.MaxO     = SUN_OLD.SumO     = SUN_OLD.MinO     = 0 ;
	SUN_OLD.MaxT     = SUN_OLD.SumT     = SUN_OLD.MinT     = 0 ;
	SUN_OLD.Count_t  = SUN_OLD.Count_O  = SUN_OLD.Count_T  = 0 ;
//[*]컴컴 Sun/D 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]
	SUND_OLD.Maxt    = SUND_OLD.Sumt    = SUND_OLD.Mint    = 0 ;
	SUND_OLD.MaxO    = SUND_OLD.SumO    = SUND_OLD.MinO    = 0 ;
	SUND_OLD.MaxT    = SUND_OLD.SumT    = SUND_OLD.MinT    = 0 ;
	SUND_OLD.Count_t = SUND_OLD.Count_O = SUND_OLD.Count_T = 0 ;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
	ERcount   = 0;          // A/D Error Count
	ERROR     = 0;          // Error Status Register
	DLYcount  = 0;          // A/D Delay Count Value
	ADValue   = 0;          // Read A/D Converter Value (Value type is int)
	Event     = 0;          // 4 Hz Counter Value(Used interrupt EX0)
	Init      = 1;          // Initiliaze Min & Max Value Initiliaze
	Flag      = OFF;        // Select Loop Run & Stop Flag (Used interrupt EX0)

	TEMP_R.Sam.Avr = 500;   // Temperature Default Value Setting

	MUX0 = OFF;		MUX1 = OFF;		MUX2 = OFF;  // A/D Convert Select(First Temperature)
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_struct(Sensor_u *Sen, Byte Cmd, Byte Sid, Byte Scode)
{
	Sen->Sam.Head   = 0xFFFF;   // 0,1   Header Value.
	Sen->Sam.Cmd    = Cmd   ;   // 2     'M','t','O','T' Command.
	Sen->Sam.Sta    = 0     ;   // 3     Sensor Status Value
	Sen->Sam.Scode  = Scode ;   // 4     Sensor Code
	Sen->Sam.Sid    = Sid   ;   // 5     This Porgram Sid is Fixed.(Jumper ID)
	Sen->Sam.Avr    = 0     ;   // 6,7   Average Value
	Sen->Sam.Spc    = 0     ;   // 8,9
	Sen->Sam.Max    = 0     ;   // 10,11 Maximum Value.
	Sen->Sam.Min    = 0     ;   // 12,13 Minimum Value.
	Sen->Sam.Cks    = 0     ;   // 14    Check Sum Value.
	Sen->Sam.Tail   = 0xFFFE;   // 15,16 Tail.                   Total = 17 Bytes
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_serial(void)
{
	Com.TxSp = Com.TxEp = Com.RxSp = Com.RxEp = 0;

	disable(EA);
	PCON = 0x80;      // Baudrate Double set
	SCON = 0x50;      // Serial Config Register
	TMOD = 0x21;      // T0 = Internal Timer 16Bit no,Prescaler,
					  // T1 = Using to Serial
	TH1  = 0xfa;
	TL1  = 0xfa;      // 23Mhz = F3, 22Mhz = F4,18Mhz = F6, 11Mhz = fa Set
	TR1  = 1   ;      // Serial Timer Run
	PS   = 1   ;      // Serial Interrupt First.

	IT0  = 1   ;      // Edge Trigger
	IT1  = 1   ;      // Edge Trigger(Precipi)
	EX0  = 1   ;      // 4 Hz Clock Enalble
	EX1  = 1   ;      // Precipi
	ES   = 1   ;      // Serial Interrupt Enable
	enable(EA);       // Interrupt All enable
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void initiliaze(void)
{
	// Jumper_Read(); Jumper Read Function
	disable(GREENL);        disable(REDL);          // LED Clear (TXD & RXD)

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
	// Precipi Struct Initiliaze
	init_struct(&PREC_R, REAL , SID2, SCODE2);
	init_struct(&PREC_t, SEC10, SID2, SCODE2);
	init_struct(&PREC_O, MIN1 , SID2, SCODE2);
	init_struct(&PREC_T, MIN10, SID2, SCODE2);
	// SUN Struct Initiliaze
	init_struct(&SUN_R , REAL , SID3, SCODE3);
	init_struct(&SUN_t , SEC10, SID3, SCODE3);
	init_struct(&SUN_O , MIN1 , SID3, SCODE3);
	init_struct(&SUN_T , MIN10, SID3, SCODE3);
	// SUN/D Struct Initiliaze
	init_struct(&SUND_R, REAL , SID4, SCODE4);
	init_struct(&SUND_t, SEC10, SID4, SCODE4);
	init_struct(&SUND_O, MIN1 , SID4, SCODE4);
	init_struct(&SUND_T, MIN10, SID4, SCODE4);

	init_variable();                // Variable All initiliaze
	init_serial();                  // Serial Port initiliaze
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void check_mpm(void)    // Protocol Check MPM -> SAM Board
{
	Byte    CK=0, Count= 0;

	// RxSp is Point(Center), RxEp increment is interrupt.

	if(Com.RxSp == Com.RxEp)                        return; get_serial();
	if(Com.RxBuf[Loc(Com.RxSp,-1) ] != 0xfe)        return; disable(GREENL);
	if(Com.RxBuf[Loc(Com.RxSp,-2) ] != 0xff)        return;
	if(Com.RxBuf[Loc(Com.RxSp,-10)] != 0xff)        return;
	if(Com.RxBuf[Loc(Com.RxSp,-11)] != 0xff)        return;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
	if(Com.RxBuf[Loc(Com.RxSp,-8)] != SID0 && Com.RxBuf[Loc(Com.RxSp,-8)] != SID1 &&
	   Com.RxBuf[Loc(Com.RxSp,-8)] != SID2 && Com.RxBuf[Loc(Com.RxSp,-8)] != SID3 &&
	   Com.RxBuf[Loc(Com.RxSp,-8)] != SID4)			return;

	for(Count=0; Count<sizeof(Command_t); Count++)  {
		if(Count>1 && Count < 8)
			 CK ^= Com.RxBuf[Loc(Com.RxSp,-(11-Count))];
		Mpm.Bytes[Count] = Com.RxBuf[Loc(Com.RxSp,-(11-Count))];
	}
	if(Mpm.Mpm.Cks != CK)                           return;

	switch(Mpm.Mpm.Cmd)     {
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
		case    REAL :
			switch(Mpm.Mpm.Sid)     {
				case    SID0:   send_struct(&TEMP_R);   break;
				case    SID1:   send_struct(&HUMI_R);   break;
				case    SID2:   send_struct(&PREC_R);   break;
				case    SID3:   send_struct(&SUN_R) ;   break;
				case    SID4:   send_struct(&SUND_R);   break;
				default     :                           break;
				}           							break;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
		case    SEC10:
			switch(Mpm.Mpm.Sid)     {
				case    SID0:   send_struct(&TEMP_t);   break;
				case    SID1:   send_struct(&HUMI_t);   break;
				case    SID2:   send_struct(&PREC_t);   break;
				case    SID3:   send_struct(&SUN_t) ;   break;
				case    SID4:   send_struct(&SUND_t);   break;
				default     :                           break;
				}           							break;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
		case    MIN1 :
			switch(Mpm.Mpm.Sid)     {
				case    SID0:   send_struct(&TEMP_O);   break;
				case    SID1:   send_struct(&HUMI_O);   break;
				case    SID2:   send_struct(&PREC_O);   break;
				case    SID3:   send_struct(&SUN_O) ;   break;
				case    SID4:   send_struct(&SUND_O);   break;
				default     :                           break;
			}    		       							break;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
		case    MIN10:
			switch(Mpm.Mpm.Sid)     {
				case    SID0:   send_struct(&TEMP_T);   break;
				case    SID1:   send_struct(&HUMI_T);   break;
				case    SID2:   send_struct(&PREC_T);   break;
				case    SID3:   send_struct(&SUN_T) ;   break;
				case    SID4:   send_struct(&SUND_T);   break;
				default     :                           break;
			}  		         							break;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
		case    RST      :
			switch(Mpm.Mpm.Sid)     {
				case    SID0:
					TEMP_t.Sam.Max = Mpm.Mpm.Max;	TEMP_t.Sam.Min = Mpm.Mpm.Min;
					TEMP_O.Sam.Max = Mpm.Mpm.Max;   TEMP_O.Sam.Min = Mpm.Mpm.Min;
					TEMP_T.Sam.Max = Mpm.Mpm.Max;	TEMP_T.Sam.Min = Mpm.Mpm.Min;
					break;
				case    SID1:
					HUMI_t.Sam.Max = Mpm.Mpm.Max;	HUMI_t.Sam.Min = Mpm.Mpm.Min;
					HUMI_O.Sam.Max = Mpm.Mpm.Max;	HUMI_O.Sam.Min = Mpm.Mpm.Min;
					HUMI_T.Sam.Max = Mpm.Mpm.Max;	HUMI_T.Sam.Min = Mpm.Mpm.Min;
					break;
				case    SID2:
					PREC_t.Sam.Max = Mpm.Mpm.Max;	PREC_t.Sam.Min = Mpm.Mpm.Min;
					PREC_O.Sam.Max = Mpm.Mpm.Max;	PREC_O.Sam.Min = Mpm.Mpm.Min;
					PREC_T.Sam.Max = Mpm.Mpm.Max;	PREC_T.Sam.Min = Mpm.Mpm.Min;
					break;
				case    SID3:
					SUN_t.Sam.Max = Mpm.Mpm.Max;    SUN_t.Sam.Min = Mpm.Mpm.Min;
					SUN_O.Sam.Max = Mpm.Mpm.Max;    SUN_O.Sam.Min = Mpm.Mpm.Min;
					SUN_T.Sam.Max = Mpm.Mpm.Max;    SUN_T.Sam.Min = Mpm.Mpm.Min;
					break;
				case    SID4:
					SUND_t.Sam.Max = Mpm.Mpm.Max;   SUND_t.Sam.Min = Mpm.Mpm.Min;
					SUND_O.Sam.Max = Mpm.Mpm.Max;   SUND_O.Sam.Min = Mpm.Mpm.Min;
					SUND_T.Sam.Max = Mpm.Mpm.Max;   SUND_T.Sam.Min = Mpm.Mpm.Min;
					break;
				default:                                break;
		}                   							break;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
		case    INIT :      Init = 1;                   break;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
		default      :                                  break;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void send_struct(Sensor_u *Send)   // SAM -> MPM DATA Transmit
{
	Word    Count;

	Send->Sam.Sta = ERROR;          // Error Flag Setting
	for(Count=0; Count<sizeof(Sensor_t); Count++)   {
		if(Count>1 && Count <14)        Send->Sam.Cks ^= Send->Bytes[Count];
		Com.TxBuf[Com.TxEp++] = Send->Bytes[Count];
		if(Com.TxEp >= MAX)             Com.TxEp = 0;
	}
	enable(REDL);
	SBUF = Com.TxBuf[Com.TxSp++];
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
signed int Loc(signed int Comp, signed int Step)    // Q Buffer Point Set
{
	Comp += Step;
	if(Comp < 0)    Comp += MAX;
	return  Comp;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void AD_read(void)          // A/D Convert DATA Read Function
{
	ADCENB = HOLD;          // Voltage is Holding
	for(DLYcount=0; DLYcount < DLY; DLYcount++) { DLYcount++; DLYcount--;}

	output(ADMSB, 0xff);    // A/D Convert Start Command

	for(DLYcount=0; DLYcount < DLY; DLYcount++) { DLYcount++; DLYcount--;}
	ERcount=0;              // Error Count Value Clear
	while(STATUS)		{	if(ERcount++>100)	break;  }
	if(ERcount>100)         ERROR |= ADERR  ;
	else                    ERROR = 0       ;

	disable(ES);
	ADValue = input(ADLSB);		ADValue |= (input(ADMSB)<<8);
	enable(ES);
	ADCENB  = SAMPLE;
	Flag = OFF;             // Disable Flag . (Select.Loop Used. Flag Off)
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Temp_cal(void)        // TEMP Calculation Function
{
	AD_read();
	disable(ES);
	TEMP_R.Sam.Avr =  ( ADValue / 4.095) + 500.;   // Temp Cal.
	enable(ES);

	TEMP_OLD.Sumt += TEMP_R.Sam.Avr;                // Average Value 10 sec		TEMP_OLD.SumO += TEMP_R.Sam.Avr;                // Average Value 1  min
	TEMP_OLD.SumT += TEMP_R.Sam.Avr;                // Average Value 10 min
	// One Day Max, Min processing
	if(TEMP_R.Sam.Avr > TEMP_R.Sam.Max)     TEMP_R.Sam.Max = TEMP_R.Sam.Avr;
	if(TEMP_R.Sam.Avr < TEMP_R.Sam.Min)     TEMP_R.Sam.Min = TEMP_R.Sam.Avr;
	// 10 sec, 1 min, 10 min MAX & MIN proc...
	if(TEMP_R.Sam.Avr > TEMP_OLD.Maxt )     TEMP_OLD.Maxt  = TEMP_R.Sam.Avr;
	if(TEMP_R.Sam.Avr < TEMP_OLD.Mint )     TEMP_OLD.Maxt  = TEMP_R.Sam.Avr;
	if(TEMP_R.Sam.Avr > TEMP_OLD.MaxO ) 	TEMP_OLD.MaxO  = TEMP_R.Sam.Avr;
	if(TEMP_R.Sam.Avr < TEMP_OLD.MinO )     TEMP_OLD.MaxO  = TEMP_R.Sam.Avr;
	if(TEMP_R.Sam.Avr > TEMP_OLD.MaxO ) 	TEMP_OLD.MaxT  = TEMP_R.Sam.Avr;
	if(TEMP_R.Sam.Avr < TEMP_OLD.MinO )     TEMP_OLD.MaxT  = TEMP_R.Sam.Avr;
	// 10 sec proc...
	if(TEMP_OLD.Count_t >= 10.)      {
		TEMP_t.Sam.Avr   = TEMP_OLD.Sumt/10.                  ;
		TEMP_t.Sam.Max   = TEMP_OLD.Maxt                      ;
		TEMP_t.Sam.Min   = TEMP_OLD.Mint                      ;
		TEMP_OLD.Maxt    = TEMP_OLD.Mint = TEMP_OLD.Sumt = 0. ;
		TEMP_OLD.Count_t = 0.;             TEMP_OLD.Count_O++ ;
	}
	// 1 min proc...
	if(TEMP_OLD.Count_O >= 6.)       {
		TEMP_O.Sam.Avr   = TEMP_OLD.SumO/60.                  ;
		TEMP_O.Sam.Max   = TEMP_OLD.MaxO                      ;
		TEMP_O.Sam.Min   = TEMP_OLD.MinO                      ;
		TEMP_OLD.MaxO    = TEMP_OLD.MinO = TEMP_OLD.SumO = 0. ;
		TEMP_OLD.Count_O = 0.;             TEMP_OLD.Count_T++ ;
	}
	// 10 min proc...
	if(TEMP_OLD.Count_T >= 10.)      {
		TEMP_T.Sam.Avr   = TEMP_OLD.SumT/600.                 ;
		TEMP_T.Sam.Max   = TEMP_OLD.MaxT                      ;
		TEMP_T.Sam.Min   = TEMP_OLD.MinT                      ;
		TEMP_OLD.MaxT    = TEMP_OLD.MinT = TEMP_OLD.SumT = 0. ;
		TEMP_OLD.Count_T = 0.;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Sun_cal(void)              // Sun Calculate
{
	// Sun_Day Day of Max
	AD_read();
	disable(ES);
	// Unit = Sensor Spc  * (A/D read)/Div   *  Volt Range / Op-Amp Range
	//  W   = (10e6/9.06) * (AD_read  /4096) *    10       /    100
	Sun_Value = (10000./9.06) * (ADValue / 4096.) * 10.;
	SUN_R.Sam.Avr = (Word)Sun_Value;
	enable(ES);

	SUN_OLD.Sumt += SUN_R.Sam.Avr;          // Average Value 10 sec
	SUN_OLD.SumO += SUN_R.Sam.Avr;          // Average Value 1  min
	SUN_OLD.SumT += SUN_R.Sam.Avr;          // Average Value 10 min
	// 10 sec proc...
	if(SUN_OLD.Count_t >= 10)       {
		SUN_t.Sam.Avr   = SUN_OLD.Sumt/10.                   ;
		SUN_t.Sam.Max   = SUN_T.Sam.Max                      ;
		SUN_OLD.Sumt    = 0;
		SUN_OLD.Count_t = 0;               SUN_OLD.Count_O++ ;
	}
	// 1 min proc...
	if(SUN_OLD.Count_O >= 6)        {
		SUN_O.Sam.Avr   = SUN_OLD.SumO/60.                   ;
		SUN_O.Sam.Max   = SUN_T.Sam.Max                      ;
		SUN_OLD.SumO    = 0;
		SUN_OLD.Count_O = 0;               SUN_OLD.Count_T++ ;
	}
	// 10 min proc...
	if(SUN_OLD.Count_T >= 10)       {
		SUN_T.Sam.Avr   = SUN_OLD.SumT/600.                  ;
		SUN_T.Sam.Max  += SUN_T.Sam.Avr                      ;
		SUN_OLD.SumT    = 0;        	  SUN_OLD.Count_T = 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Humi_cal(void)        // HUMI Calculation Function
{
	AD_read();
	disable(ES);
	HUMI_R.Sam.Avr = ADValue / 4.095;      // HUMI Cal.
	enable(ES);
	if(HUMI_R.Sam.Avr > 999)	HUMI_R.Sam.Avr = 999;   // HUMI MAX SETTING

	HUMI_OLD.Sumt += HUMI_R.Sam.Avr;        // Average Value 10 sec
	HUMI_OLD.SumO += HUMI_R.Sam.Avr;        // Average Value 1  min
	HUMI_OLD.SumT += HUMI_R.Sam.Avr;        // Average Value 10 min
	// One Day Max, Min processing
	if(HUMI_R.Sam.Avr > HUMI_R.Sam.Max)     HUMI_R.Sam.Max = HUMI_R.Sam.Avr;
	if(HUMI_R.Sam.Avr < HUMI_R.Sam.Min)     HUMI_R.Sam.Min = HUMI_R.Sam.Avr;
	// 10 sec, 1 min, 10 min MAX & MIN proc...
	if(HUMI_R.Sam.Avr > HUMI_OLD.Maxt )     HUMI_OLD.Maxt  = HUMI_R.Sam.Avr;
	if(HUMI_R.Sam.Avr < HUMI_OLD.Mint )     HUMI_OLD.Maxt  = HUMI_R.Sam.Avr;
	if(HUMI_R.Sam.Avr > HUMI_OLD.MaxO ) 	HUMI_OLD.MaxO  = HUMI_R.Sam.Avr;
	if(HUMI_R.Sam.Avr < HUMI_OLD.MinO )     HUMI_OLD.MaxO  = HUMI_R.Sam.Avr;
	if(HUMI_R.Sam.Avr > HUMI_OLD.MaxO ) 	HUMI_OLD.MaxT  = HUMI_R.Sam.Avr;
	if(HUMI_R.Sam.Avr < HUMI_OLD.MinO )     HUMI_OLD.MaxT  = HUMI_R.Sam.Avr;
	// 10 sec proc...
	if(HUMI_OLD.Count_t >= 10)      {
		HUMI_t.Sam.Avr   = HUMI_OLD.Sumt/10.                  ;
		HUMI_t.Sam.Max   = HUMI_OLD.Maxt                      ;
		HUMI_t.Sam.Min   = HUMI_OLD.Mint                      ;
		HUMI_OLD.Maxt    = HUMI_OLD.Mint = HUMI_OLD.Sumt = 0  ;
		HUMI_OLD.Count_t = 0;              HUMI_OLD.Count_O++ ;
	}
	// 1 min proc...
	if(HUMI_OLD.Count_O >= 6)       {
		HUMI_O.Sam.Avr   = HUMI_OLD.SumO/60.                  ;
		HUMI_O.Sam.Max   = HUMI_OLD.MaxO                      ;
		HUMI_O.Sam.Min   = HUMI_OLD.MinO                      ;
		HUMI_OLD.MaxO    = HUMI_OLD.MinO = HUMI_OLD.SumO = 0  ;
		HUMI_OLD.Count_O = 0;              HUMI_OLD.Count_T++ ;
	}
	// 10 min proc...
	if(HUMI_OLD.Count_T >= 10)      {
		HUMI_T.Sam.Avr   = HUMI_OLD.SumT/600.                 ;
		HUMI_T.Sam.Max   = HUMI_OLD.MaxT                      ;
		HUMI_T.Sam.Min   = HUMI_OLD.MinT                      ;
		HUMI_OLD.MaxT    = HUMI_OLD.MinT = HUMI_OLD.SumT = 0  ;
		HUMI_OLD.Count_T = 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]  Interrupt Function Start....                                        [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void SCON_int(void)   // Serial Interrupt Function
{
	disable(EA);
	if(RI)  {
		disable(RI);	                disable(GREENL);
		Com.RxBuf[Com.RxEp++] = SBUF;    enable(GREENL);
		if(Com.RxEp >= MAX)              Com.RxEp = 0  ;
		// Portocol Check. ( MPM -> SAM )
		check_mpm();
	}
	else if(TI)     {
		disable(TI);					  disable(REDL);
		if(Com.TxSp != Com.TxEp)	{
			SBUF = Com.TxBuf[Com.TxSp++];  enable(REDL);
		}
		else                              disable(REDL);
		if(Com.TxSp >= MAX)               Com.TxSp = 0 ;
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void EX0_int(void)    // External Interrupt Function
{
	disable(ES);

	if(PREC_Flag == ON)             PREC_Count++;

	switch(Event)
	{
		case    0:  if(TEMP_OLD.Count_t >= 10)	TEMP_OLD.Count_t= 0 ;
					if(HUMI_OLD.Count_t >= 10)  HUMI_OLD.Count_t= 0 ;
					if(SUN_OLD.Count_t  >= 10)  SUN_OLD.Count_t = 0 ;
					if(PREC_OLD.Count_t >= 10)  PREC_OLD.Count_t= 0 ;
					Event++;                                  break ;
					// Temp cal MUX CH:1  0 0 0
		case    1:  MUX0 = 1;	  MUX1 = 0;     MUX2 = 0 ;  Event++ ;
					Loop.Select = Temp_cal;		Flag = ON;	  break ;
					// Sun cal  MUX CH:2  0 0 1
		case    2:  MUX0 = 1;     MUX1 = 1;     MUX2 = 1 ;  Event++ ;
					Loop.Select = Sun_cal ;     Flag = ON;    break ;
					// Humi cal MUX CH:8  1 1 1
		case    3:  MUX0 = 0;     MUX1 = 0;     MUX2 = 0 ;	Event= 0;
					TEMP_OLD.Count_t++;     	HUMI_OLD.Count_t++  ;
					SUN_OLD.Count_t++;      	PREC_OLD.Count_t++  ;
					Loop.Select = Humi_cal;     Flag = ON;	  break ;
		default  :                                            break ;
	}
	enable(ES);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt       void EX1_int(void)              // Precipi cal
{
	disable(ES);

	PREC_Flag      = ON;
	PREC_OLD.Sumt  = PREC_OLD.SumO   = PREC_OLD.SumT += 5.;
	PREC_R.Sam.Max = PREC_R.Sam.Avr += 5.;
	// Sec * 10Min * 4Hz = 14400 ( Wait Time overflow )
	if(PREC_Count > 14400.) {
		PREC_Flag = OFF;        PREC_Count = 0.;
	}
	if(PREC_OLD.Count_t >= 10.)     {
		PREC_t.Sam.Avr = PREC_OLD.Sumt/PREC_OLD.Count_t;
		PREC_t.Sam.Max = PREC_OLD.Sumt;
		PREC_OLD.Sumt  = PREC_OLD.Count_t = 0.;
		PREC_OLD.Count_O++;
	}
	if(PREC_OLD.Count_O >= 6.)      {
		PREC_O.Sam.Avr = PREC_OLD.SumO/PREC_OLD.Count_O;
		PREC_O.Sam.Max = PREC_OLD.SumO;
		PREC_OLD.SumO  = PREC_OLD.Count_O = 0.;
		PREC_OLD.Count_T++;
	}
	if(PREC_OLD.Count_T >= 10.)     {
		PREC_T.Sam.Avr = PREC_OLD.SumT/PREC_OLD.Count_t;
		PREC_T.Sam.Max = PREC_OLD.SumT;
		PREC_OLD.SumT  = PREC_OLD.Count_T = 0.;
		PREC_OLD.Count_T = 0.;
	}
	if(PREC_Flag == ON && PREC_Count != 0.) {
		PREC_R.Sam.Spc = PREC_t.Sam.Spc = 18000./(PREC_Count / 4.);
		PREC_O.Sam.Spc = PREC_T.Sam.Spc = 18000./(PREC_Count / 4.);
		PREC_Count = 0.;
	}
	enable(ES);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]    Main Program Start...                                             [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	initiliaze();              // Initiliaze Serial & Variable & Structs.

	while(1)        {
		if(Init == 1)   {      // Min & Max Value Initiliaze
			TEMP_t.Sam.Max = TEMP_R.Sam.Avr;  TEMP_t.Sam.Min = TEMP_R.Sam.Avr;
			TEMP_O.Sam.Max = TEMP_R.Sam.Avr;  TEMP_O.Sam.Min = TEMP_R.Sam.Avr;
			TEMP_T.Sam.Max = TEMP_R.Sam.Avr;  TEMP_T.Sam.Min = TEMP_R.Sam.Avr;

			HUMI_t.Sam.Max = HUMI_R.Sam.Avr;  HUMI_t.Sam.Min = HUMI_R.Sam.Avr;
			HUMI_O.Sam.Max = HUMI_R.Sam.Avr;  HUMI_O.Sam.Min = HUMI_R.Sam.Avr;
			HUMI_T.Sam.Max = HUMI_R.Sam.Avr;  HUMI_T.Sam.Min = HUMI_R.Sam.Avr;

			PREC_R.Sam.Avr = 0;
			PREC_t.Sam.Max = PREC_R.Sam.Avr;  PREC_t.Sam.Min = PREC_R.Sam.Avr;
			PREC_O.Sam.Max = PREC_R.Sam.Avr;  PREC_O.Sam.Min = PREC_R.Sam.Avr;
			PREC_T.Sam.Max = PREC_R.Sam.Avr;  PREC_T.Sam.Min = PREC_R.Sam.Avr;

			SUN_R.Sam.Avr  = 0;
			SUN_t.Sam.Max  = SUN_R.Sam.Avr;   SUN_t.Sam.Min = SUN_R.Sam.Avr;
			SUN_O.Sam.Max  = SUN_R.Sam.Avr;   SUN_O.Sam.Min = SUN_R.Sam.Avr;
			SUN_T.Sam.Max  = SUN_R.Sam.Avr;   SUN_T.Sam.Min = SUN_R.Sam.Avr;
			Init = 0;
		}
		// Sun  dectect
		SUND_R.Sam.Avr = SUND_t.Sam.Avr = SUND_CHECK;
		SUND_O.Sam.Avr = SUND_T.Sam.Avr = SUND_CHECK;

		if(Flag == ON)          Loop.Select();   // Run Function (Select Function)
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
