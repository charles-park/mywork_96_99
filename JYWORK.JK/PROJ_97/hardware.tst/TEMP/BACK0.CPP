#include <io51.h>
#include <math.h>

#define 	ON			1					// Led On Status
#define		OFF			0					// Led Off Status
#define 	MAX			100					// Serial Buffer Size
#define		WD			0					// Fixed Wind Direction Sensor ID
#define		WS			WD+1				// Fixed Wind Speed Sensor ID : Must (W/D + 1)

#define 	JUMPER		0x8000				// Jumper Address
#define		ADMSB		0xC000              // A/D Convert 12 Bit (Most Bit)
#define		ADLSB		ADMSB+1             // A/D Convert 12 Bit (Last Bit)

#define		REDL		P1.7				// RED LED PIN
#define		BULEL		P1.6				// BULE LED PIN
#define 	WDT			P1.5                // Jump I/O Pin Description
#define 	Watch(X)	X = ( 1 - X )       // Watch Dog, Define Function
#define		enable(Y)	Y = ON
#define		disable(Z)	Z = OFF

//[*]ÄÄÄÄÄ MPM -> SAM Command Define ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define		REAL		'M'                 // Real Data
#define		SEC10		't'					// 10 Sec AVR Data
#define 	MIN1		'O'					// 1 Min AVR Data
#define		MIN10		'T'					// 10 Min AVR Data
#define		INIT		'I'					// Sam Board initiliaze(Only_
#define		RST  		'@'					// After Initiliaze, First data(Min,Max Data) Transmint
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define		SERIAL		TR1
#define		TIMER		TR0
#define		EXTINT
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define	Serial_Get(void)													\
{                        													\
	if((++Sbuf.Rx_sp) >= MAX)	Sbuf.Rx_sp = 0 ;							\
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]

typedef struct ws_wd_cal_select 	{
	void (*Select)(Word Value);
}	Select__F;
Select__F	Loop;
// Select interrupt Function --> Cal & Wind Direction & Wind Speed & Storage.. ETC
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef		unsigned char 	Byte;			// define unsigned char ==> Byte
typedef		unsigned int	Word;			// define unsigned int  ==> Word

//[*]ÄÄÄÄÄ Serial Data Processing Struct ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct Serial__t {
	Byte	Tx_buf[MAX]          ;			// Transmit Buffer (unsigned char)
	Byte	Rx_buf[MAX]          ;			// Receive Buffer (unsigned char)
	Byte	Rx_sp, Rx_ep         ;			// Receive Buffer Start & End Pointer Variable
	Byte	Tx_sp, Tx_ep		 ;			// Transmit Buffer Start & End & Flag Variable
}	Serial_t;								// Serial struct define

//[*]ÄÄÄÄÄ W/D & W/S Calticulor Variable ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct WdWs__t	{
	Word     		WsMax, WsMin        ;	// Wind Speed Max,Min Variable define
	unsigned long   WsSum               ;	// Wind Speed Sum Variable

	Word     		WdMax, WdMin, WdSum ;	// Wind Direction Max,Min,Sum Variable define
	double			SinValue            ;	// Sin Value
	double			CosValue            ;	// Cosin Value

	Byte			TimeCount			;   // Time Count
	Byte			Flag				;
}	Values_t;
//[*]ÄÄÄÄÄ MPM Data Processing Struct ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct Command__t {                 // MPM -> SAM Protocol
	Word	Header    ;						// Header : 0xffff
	Byte	Command   ;						// Command Real Data, 10 Sec Data ETC..
	Byte	Sid       ;						// SAM Board ID
	Word	Min       ;						// Initiliaze Min Data
	Word	Max       ;						// Initiliaze Max Data
	Byte	Check_sum ;						// Command ~ Max data => XOR Value
	Word	Tail      ;						// Tail : 0xfffe
}	Command_t;   							// Total 11 Byte Received.

//[*]ÄÄÄÄÄ MPM Receive Data Processing union ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef union Command__u {					// Receive Data Form -> Byte
	Command_t		Mpm                     ;
	Byte			Bytes[sizeof(Command_t)];
}	Command_u;

//[*]ÄÄÄÄÄ SAM Board Data Processing Struct ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct Sensor__t {             		// SAM -> MPM Protocol
	Word	Header     ;					// Header : 0xffff
	Byte	Command   ;						// Command : MPM -> SAM -> MPM(MPM Command Equl)
	Byte	Status    ;						// Sensor Status(Error)
	Byte	Scode     ;						// Sensor Code (Fixed) -> Protocol Manual
	Byte	Sid       ;						// SAM Board ID(Jumper Setting)
	Word	Avr       ;						// Average Value(SAM)
	Word	Spc       ;						// Special Value( 3 Sec Average..ETC)
	Word	Min       ;						// Min Value SAM
	Word	Max       ;						// Max Value SAM
	Byte	Check_sum ;						// Form Command ~ Max Value , XOR Value
	Word	Tail      ;						// Tail : 0xfffe
}	Sensor_t;   							// Total = 17 Byte Transmit.
//[*]ÄÄÄÄÄ SAM Board Transmit Data Processing union ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef union Sensor__u {					// Transmit Form Define
	Sensor_t  		Sam                      ;
	Byte 			Bytes[sizeof(Sensor_t)]  ;
}	Sensor_u;
//[*]ÄÄÄÄÄ Variable define ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static	Sensor_u		Real[2], Sec10[2], Min1[2], Min10[2];	// Wind Speed Struct & Wind Direction Struct
static	Serial_t		Sbuf                                ;	// Serial Processing Struct
static	Values_t		EVENT	     	                    ;	// Wind Speed & Wind Direction Data
// Values_t is Time Value . Sin Cos ..ETC  118 Line is Error .

//[*]ÄÄÄÄÄÄÄ START FUNCTION ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define REAL	0
#define SEC  	1
#define MIN1 	2
#define MIN10	3

#define WD		0
#define WS		1
static	Sensor_u		Sensor[4][2];
Sensor[REAL][WD].
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void SAM_struct_init(Sensor_u *sensor, Byte code, Byte id, Byte command)
{
	Byte cmd[4] = "MtOT";
	sensor -> Sam.Header	=	0xFFFF;
	sensor -> Sam.Command	=	command;
	sensor -> Sam.Status	= 	0;
	sensor -> Sam.Scode		=	code;		// Fixed ID Number
	sensor -> Sam.Sid		=	id;         // Hardware Jumper Setting
	sensor -> Sam.Avr		= 	0;
	sensor -> Sam.Spc		=	0;
	sensor -> Sam.Max		= 	0;
	sensor -> Sam.Min		=	0;
	sensor -> Sam.Check_sum = 	0;
	sensor -> Sam.Tail		=   0xFFFE;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
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
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
Byte jumper_check(Byte ID_CON)
{
	// if Jumper Check Roution insert(Knot or m/s or Vector Function)
	ID_CON &= 0x0f;
	ID_CON = ~ID_CON;
	return	ID_CON;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
Byte id_jumper_read(void)
{
	Byte i, ID_comp, ID;

	while(1)
	{
		Watch(WDT)        ;
		ID_ORG = input(JUMPER);

		for( i=0 ; i<10 ;i++ )		ID_comp = input(JUMPER);
		if(ID_ORG == ID_comp)						  break;
	}
	return jumper_check(ID_ORG);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static Byte ID = 0;
void initiliaze(void)
{
	inter_initiliaze();
	ID = id_jumper_read();

	Sbuf.Rx_sp = Sbuf.Tx_sp = Sbuf.Rx_ep = Sbuf.Tx_ep = 0;
	EVENT.Flag = EVENT.TimeCount = 0;

	SAM_struct_init(&Real [WD] , WD , ID  , REAL  );	Watch(WDT);
	SAM_struct_init(&Sec10[WD] , WD , ID  , SEC10 );
	SAM_struct_init(&Min1 [WD] , WD , ID  , MIN1  );
	SAM_struct_init(&Min10[WD] , WD , ID  , MIN10 );

	SAM_struct_init(&Real [WS] , WS , ID+1, REAL  );	Watch(WDT);
	SAM_struct_init(&Sec10[WS] , WS , ID+1, SEC10 );
	SAM_struct_init(&Min1 [WS] , WS , ID+1, MIN1  );
	SAM_struct_init(&Min10[WS] , WS , ID+1, MIN10 );

	for(int i=0;i<4;i++)	{
		SAM_struct_init(&Sensor[i][WD],WD,ID+WD,i);
		SAM_struct_init(&Sensor[i][WS],WS,ID+WS,i);
		Watch(WDT);
	}

	enable(TR0);	enable(EX0);	enable(ES);         Watch(WDT);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
int Point(int Bufsize, int Now, int Step)
{
	Now += Step;
	if(Now >  Bufsize)	Now -= Bufsize;
	if(Now =< Bufsize)  Now += Bufsize;
	return	Now;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void Check_receive(void)
{
	Byte	     CHECK_XOR=0, count=0, WD_WS=0;
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

		if(Cmd.Mpm.Sid == ID)	WD_WS = WD;
		else					WD_WS = WS;

		if(CHECK_XOR == Cmd.Mpm.Check_sum)	{
			switch(Cmd.Mpm.Command)	{
				case	RST  :
					Real [WD_WS].Sam.Max  = Cmd.Mpm.Max;
					Sec10[WD_WS].Sam.Max  = Cmd.Mpm.Max;
					Min1 [WD_WS].Sam.Max  = Cmd.Mpm.Max;
					Min10[WD_WS].Sam.Max  = Cmd.Mpm.Max;

					Real [WD_WS].Sam.Min  = Cmd.Mpm.Min;
					Sec10[WD_WS].Sam.Min  = Cmd.Mpm.Min;
					Min1 [WD_WS].Sam.Min  = Cmd.Mpm.Min;
					Min10[WD_WS].Sam.Min  = Cmd.Mpm.Min;

					Real [WD_WS].Sam.Avr  = Cmd.Mpm.Max;
					Sec10[WD_WS].Sam.Avr  = Cmd.Mpm.Max;
					Min1 [WD_WS].Sam.Avr  = Cmd.Mpm.Max;
					Min10[WD_WS].Sam.Avr  = Cmd.Mpm.Max;	Watch(WDT);	break;
				case    INIT :             					Watch(WDT);	break;
				case	REAL :  Send_to_Mpm(&Real [WD_WS]);	Watch(WDT);	break;
				case	SEC10:  Send_to_Mpm(&Sec10[WD_WS]);	Watch(WDT);	break;
				case	MIN1 :  Send_to_Mpm(&Min1 [WD_WS]);	Watch(WDT);	break;
				case	MIN10:  Send_to_Mpm(&Min10[WD_WS]);	Watch(WDT);	break;
			}
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void Send_to_Mpm(Sensor_u *Send)
{
	Byte	i;
	Send->Sam.Check_sum = 0;
	for(i=0;i<sizeof(Sensor_u);i++)	{
		if(i>1 && i<14)		Send->Sam.Check_sum ^= Send->Bytes[i];
		Sbuf.Send_Tx[Sbuf.Tx_ep++] = Send->Bytes[i];
		if(Sbuf.Tx_ep >= MAX)	Sbuf.Tx_ep = 0;
	}
	enable(REDL);	Watch(WDT);
	SBUF = Sbuf.Send_Tx[Tx_sp++];
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main(void)
{
	initiliaze();		// Serial & Struct Initiliaze
	while(1)
	{
		Check_receive();		Watch(WDT);
		if(EVENT.Flag == 1)		Select.Loop();		Watch(WDT);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void Ws_Read(void)
{
	disable(TR0);
	EVENT.WsValue  = (Word)(TL0)      ;
	EVENT.WsValue |= (Word)(TH0 << 8) ;
	enable(TR0);
	EVENT.WsSum = 0.065 * EVENT.WsValue; // Wind/Speed K value
	if(Real[WS].

	EVENT.Flag = 0;
}

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void Wd_Read(void)
{
	Word	Sum = 0;
	Sum = Value * 0.222; // Wind Direction K Value
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
// if Function is Error ==> #define Function change.(External interrupt0)
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt   void    EX0_int(void)			// 4 Hz (Time Process)
{
	switch(EVENT.TimeCount)
	{
		case	0:	Select.Loop     = Ws_Read   ;	EVENT.Flag = 1;		break;
		case	1: 	Select.Loop     = Wd_Read   ;	EVENT.Flag = 1;		break;
		case	2:  Select.Loop     = Ws_Wd_cal ;	EVENT.Flag = 1;		break;
		case	3:  EVENT.TimeCount = 0         ;	EVENT.Flag = 0;		break;
		default	 :  								EVENT.Flag = 0;		break;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt   void    SCON_int(void)			// Internal Serial Communication
{
	if(RI)	{
		disable(RI);					enable(BLUEL);
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
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
/*

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct ws__t	{
	Word (*Wind_speed_cal)(Word Wssum);
}	ws_t;
ws_t ws;

typedef struct ws_wd_cal_select 	{
	Word (*Select)(Word Value);
}	Select__F;
Select__F	Loop;

Word Wind_speed_ms  (Word Wssum)
{
	// cal m/s
}
Word Wind_speed_knot(Word Wssum)
{
	// cal knot
}

void main(void)
{
	Select.Loop = Wind_Speed;
	Select.Loop = Wind_Direction;

	int wspeed;
	if(jump_check == m/s)	ws.Wind_speed_cal = Wind_speed_ms;
	else					ws.Wind_speed_cal = Wind_speed_knot;   //knot

	Word Value;
	Value = Select.Loop;
	while(1)	{
		// wspeed cal
		wspeed = ws.Wind_speed_cal(Wssum);
		// wdir cal
		{
			if(ws_ms) 	{ 		// 	ws.Wind_speed_cal == Wind_speed_ms

			}
			if(ws_kont)	{		//	ws.Wind_speed_cal != Wind_speed_ms

			}
		}
	}
}

//[*]ÄÄÄÄ static examples Pointer ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
	//	Sensor_u	sen[1000];		===========> STATIC

	//	Sensor_u	*sen;			===========> Dynamic
	//
	//	void init_gl_sensor(void)
	//	{
	//		sen = (Sensor_u *)malloc(sizeof(Sensor_u) * 1000);
	//	}
	//	void close_gl_sensor(void)
	//	{
	//		free(sen);
	//	}

//[*]ÄÄÄ END Examples Struct ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
*/

/*
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void Check_receive(void)
{
	Byte	     CHECK_XOR=0, count=0;
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
		}

		if(CHECK_XOR == Cmd.Mpm.Check_sum)	{
			if(Cmd.Mpm.Sid == ID)		{
				switch(Cmd.Mpm.Command)	{
					case	RST  :
						WdReal .Sam.Max  = Cmd.Mpm.Max;
						WdSec10.Sam.Max  = Cmd.Mpm.Max;
						WdMin1 .Sam.Max  = Cmd.Mpm.Max;
						WdMin10.Sam.Max  = Cmd.Mpm.Max;

						WdReal .Sam.Min  = Cmd.Mpm.Min;
						WdSec10.Sam.Min  = Cmd.Mpm.Min;
						WdMin1 .Sam.Min  = Cmd.Mpm.Min;
						WdMin10.Sam.Min  = Cmd.Mpm.Min;

						WdReal .Sam.Avr  = Cmd.Mpm.Max;
						WdSec10.Sam.Avr  = Cmd.Mpm.Max;
						WdMin1 .Sam.Avr  = Cmd.Mpm.Max;
						WdMin10.Sam.Avr  = Cmd.Mpm.Max;		break;
					case    INIT :             				break;
					case	REAL :  Send_to_Mpm(&WdReal );	break;
					case	SEC10:  Send_to_Mpm(&WdSec10);	break;
					case	MIN1 :  Send_to_Mpm(&WdMin1 );	break;
					case	MIN10:  Send_to_Mpm(&WdMin10);	break;
				}
			}
			if(Cmd.Mpm.Sid == (ID+1))	{
				switch(Cmd.Mpm.Command)	{
					case	RST  :
						WsReal .Sam.Max  = Cmd.Mpm.Max;
						WsSec10.Sam.Max  = Cmd.Mpm.Max;
						WsMin1 .Sam.Max  = Cmd.Mpm.Max;
						WsMin10.Sam.Max  = Cmd.Mpm.Max;

						WsReal .Sam.Min  = Cmd.Mpm.Min;
						WsSec10.Sam.Min  = Cmd.Mpm.Min;
						WsMin1 .Sam.Min  = Cmd.Mpm.Min;
						WsMin10.Sam.Min  = Cmd.Mpm.Min;

						WsReal .Sam.Avr  = Cmd.Mpm.Max;
						WsSec10.Sam.Avr  = Cmd.Mpm.Max;
						WsMin1 .Sam.Avr  = Cmd.Mpm.Max;
						WsMin10.Sam.Avr  = Cmd.Mpm.Max;		break;
					case    INIT :             				break;
					case	REAL :  Send_to_Mpm(&WsReal );	break;
					case	SEC10:  Send_to_Mpm(&WsSec10);	break;
					case	MIN1 :  Send_to_Mpm(&WsMin1 );	break;
					case	MIN10:  Send_to_Mpm(&WsMin10);	break;
				}
			}
		}
	}
} */