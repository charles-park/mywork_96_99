#include <io51.h>
#include <math.h>
#include <awsj.h>
//[*]컴컴컴� START FUNCTION 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]
void SAM_struct_init(Sensor_u *sensor, Byte cd, Byte id, Byte command)
{
	Byte cmd[4]             =  "MtOT";
	sensor->Sam.Header	    =	0xFFFF;
	sensor->Sam.Command	    =	cmd[command];
	sensor->Sam.Status	    = 	0;
	sensor->Sam.Scode    	=	cd;		// Fixed ID Number
	sensor->Sam.Sid	    	=	id;         // Hardware Jumper Setting
	sensor->Sam.Avr	     	= 	0;
	sensor->Sam.Spc	    	=	0;
	sensor->Sam.Max	     	= 	0;
	sensor->Sam.Min	     	=	0;
	sensor->Sam.Check_sum   = 	0;
	sensor->Sam.Tail		=   0xFFFE;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void inter_initiliaze(void)
{
	// Serial Interrupt initiliaze
	IE   = 0x80;    // EA Bit Enable
	PCON = 0x80;    // SMOD = 1  SET , Double Baudrate
	TH1  = 0xf4;	// Reload Value ( Timer 1 Overflow Value )
	TL1  = 0xf4; 	// 22.1184Mhz, SMOD=1 ==> 9600bps
	TH0  =    0;	// Wind Speed Check (MSB)
	TL0  =    0;	// Wind Speed Check (LSB)
	TMOD = 0x25; 	// Timer 1==> mode2, Timer 0 ==> mode 1
	SCON = 0x50; 	// serial port mode 1, ren(receieve enable)==>1, RB8(STOP bit)==>0
	enable(TR1);	// serial Run
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte jumper_check(Byte ID_CON)
{
	// if Jumper Check Roution insert(Knot or m/s or Vector Function)
	ID_CON &= 0x0f;
	ID_CON = ~ID_CON;
	return	ID_CON;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte id_jumper_read(void)
{
	Byte i, ID_comp, ID_ORG;

	while(1)
	{
		Watch(WDT);
		ID_ORG = input(JUMPER);

		for( i=0 ; i<10 ;i++ )		ID_comp = input(JUMPER);
		if(ID_ORG == ID_comp)						  break;
	}
	return jumper_check(ID_ORG);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void initiliaze(void)
{
	Byte i;

	inter_initiliaze();
	ID = id_jumper_read();

	Sbuf.Rx_sp = Sbuf.Tx_sp = Sbuf.Rx_ep = Sbuf.Tx_ep = 0;
	EVENT.Flag = EVENT.TimeCount = 0;

	for(i=0;i<4;i++)	{
		SAM_struct_init(&Sensor[i][WD],WD,ID+WD,i);
		SAM_struct_init(&Sensor[i][WS],WS,ID+WS,i);		Watch(WDT);
	}
	enable(TR0);	enable(EX0);	enable(ES);         Watch(WDT);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
int Point(int Bufsize, int Now, int Step)
{
	Now += Step;
	if(Now >  Bufsize)	Now -= Bufsize;
	if(Now <= Bufsize)  Now += Bufsize;
	return	Now;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Check_receive(void)
{
	Byte	     CHECK_XOR=0, count=0, WD_WS=0, i;
	Command_u	 Cmd  	             ;   // MPM -> SAM Board Protocol Checking Struct

	if(Sbuf.Rx_sp == Sbuf.Rx_ep)						return;	Serial_Get();
	if(Sbuf.Rx_buf[Point(MAX,Sbuf.Rx_sp,-1)]  != 0xfe) 	return;
	if(Sbuf.Rx_buf[Point(MAX,Sbuf.Rx_sp,-2)]  != 0xff)	return;
	if(Sbuf.Rx_buf[Point(MAX,Sbuf.Rx_sp,-10)] != 0xff)	return;
	if(Sbuf.Rx_buf[Point(MAX,Sbuf.Rx_sp,-11)] != 0xff)	return;	disable(BULEL);

	if(Sbuf.Rx_buf[Point(MAX,Sbuf.Rx_sp,-8)] == ID ||
	   Sbuf.Rx_buf[Point(MAX,Sbuf.Rx_sp,-8)] == (ID+1))
	{
		for(count = 0; count < 11 ; count++)	{
			Cmd.Bytes[count] = Sbuf.Rx_buf[Point(MAX, Sbuf.Rx_sp, -(11-count))];
			if(count > 1 && count < 9)
				  CHECK_XOR ^= Sbuf.Rx_buf[Point(MAX, Sbuf.Rx_sp, -(11-count))];
		}    Watch(WDT);

		WD_WS = (Cmd.Mpm.Sid == ID) ? WD : WS;

		if(CHECK_XOR == Cmd.Mpm.Check_sum)	{
			switch(Cmd.Mpm.Command)	{
				case	RST  :
					for(i=0;i<4;i++)	{
						Sensor[i][WD_WS].Sam.Max  = Cmd.Mpm.Max;
						Sensor[i][WD_WS].Sam.Min  = Cmd.Mpm.Min;
					}
					Watch(WDT);
					break;
				case    INIT :             							break;
				case	REAL :  Send_to_Mpm(&Sensor[REAL ][WD_WS]);	break;
				case	SEC10:  Send_to_Mpm(&Sensor[SEC10][WD_WS]);	break;
				case	MIN1 :  Send_to_Mpm(&Sensor[MIN1 ][WD_WS]);	break;
				case	MIN10:  Send_to_Mpm(&Sensor[MIN10][WD_WS]);	break;
			}
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Send_to_Mpm(Sensor_u *Send)     // Check Porgram.... 97.2.11
{
	Byte	i;
	Send->Sam.Check_sum = 0;
	for(i=0;i<sizeof(Sensor_u);i++)	{
		if(i>1 && i<14)		Send->Sam.Check_sum ^= Send->Bytes[i];
		Sbuf.Tx_buf[Sbuf.Tx_ep++] = Send->Bytes[i];
		if(Sbuf.Tx_ep >= MAX)	Sbuf.Tx_ep = 0;
	}
	enable(REDL);	  Watch(WDT);
	SBUF = Sbuf.Tx_buf[Sbuf.Tx_sp++];
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	initiliaze();		// Serial & Struct Initiliaze
	while(1)	{
		Check_receive();		Watch(WDT);
		if(EVENT.Flag == 1)		Loop.Select();		Watch(WDT);
	}
}

//[*]컴컴� W/D & W/S Calticulor Variable 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct WdWs__t	{
	Word     		WsValue             ;	// Wind Speed Max,Min Variable define
	unsigned long   WsReal,WsSec10      ;	// Wind Speed Sum Variable
	unsigned long   WsMin1,WsMin10      ;	// Wind Speed Sum Variable

	Word     		WdMax, WdMin, WdSum ;	// Wind Direction Max,Min,Sum Variable define
	double			SinSec10,CosSec10   ;
	double			SinMin1,CosMin1     ;
	double			SinMin10,CosMin10   ;

	Byte			TimeCount			;   // Time Count
	Byte			CountSec10,CountMin1,CountMin10;
	Byte			Flag				;   // Function Flag
}	Values_t;

//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte _Sec10(Byte Pros)
{
	if(Pros == WS)   	EVENT.WsSec10 += EVENT.WsReal;
	if(Pros == WD)		EVENT.SinSec10 += double(sin(EVENT.WdReal) * 180./3.141592)
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte _Min1(Byte Pros)
{

}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte _Min10(Byte Pros)
{

}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Ws_Read(void)
{
	disable(TR0);
	EVENT.WsValue  = (Word)(TL0)      ;
	EVENT.WsValue |= (Word)(TH0 << 8) ;
	enable(TR0);
	EVENT.WsReal =  0.0655 * EVENT.WsValue; // Wind/Speed K value
	Sensor[REAL][WS].Sam.Avr = (Word)(EVENT.WsReal * 10);

	if(EVENT.WsSum > Sensor[REAL][WS].Sam.Max)
		Sensor[REAL][WS].Sam.Max = (Word)(EVENT.WsReal * 10);

	if((EVENT.CountSec10 % 10) == 0)	{
		Sensor[SEC10][WS].Sam.Avr = (Word)(EVENT.WsSec10/10);
		EVENT.CountSec10 = 0;	EVENT.CountMin1++;
		EVENT.WsSec10 = 0;
	}
	else EVENT.WsSec10 += EVENT.WsReal;

	if((EVENT.CountMin1 % 60) == 0) 	{
		Sensor[MIN1][WS].Sam.Avr = (Word)(EVENT.WsMin/60);
		EVENT.WsMin1 += EVENT.WsReal;
	}
	//!= EVENT.WsMin10   += EVENT.WsReal;
	//
	EVENT.Flag = 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Wd_Read(void)
{
	Word	Sum = 0;
	// Sum = Value * 0.08845; // Wind Direction K Value
}
void Ws_Wd_cal(void)
{
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// if Function is Error ==> #define Function change.(External interrupt0)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt   void    EX0_int(void)			// 4 Hz (Time Process)
{
	switch(EVENT.TimeCount)
	{
		case	0:	Loop.Select     = Ws_Read   ;	EVENT.Flag = 1;		break;
		case	1: 	Loop.Select     = Wd_Read   ;	EVENT.Flag = 1;		break;
		case	2:  Loop.Select     = Ws_Wd_cal ;	EVENT.Flag = 1;		break;
		case	3:  EVENT.TimeCount = 0         ;	EVENT.Flag = 0;		break;
		default	 :  								EVENT.Flag = 0;		break;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt   void    SCON_int(void)			// Internal Serial Communication
{
	if(RI)	{
		disable(RI);					enable(BULEL);
		Sbuf.Rx_buf[Sbuf.Rx_ep++] = SBUF;
		if(Sbuf.Rx_ep >= MAX)			Sbuf.Rx_ep = 0;
	}
	if(TI)	{
		disable(TI);
		if(Sbuf.Tx_sp != Sbuf.Tx_ep)	SBUF = Sbuf.Tx_buf[Sbuf.Tx_sp++];
		else							disable(REDL);
		if(Sbuf.Tx_sp >= MAX)			Sbuf.Tx_sp = 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
