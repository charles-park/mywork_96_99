#include <io51.h>
#include <math.h>

#define 	ON			1					// Led On Status
#define		OFF			0					// Led Off Status
#define 	MAX			100					// Serial Buffer Size
#define		WD_CODE		0					// Fixed Wind Direction Sensor ID
#define		WS_CODE		WD_ID+1				// Fixed Wind Speed Sensor ID : Must (W/D + 1)

#define 	JUMPER		0x8000				// Jumper Address
#define		ADMSB		0xC000              // A/D Convert 12 Bit (Most Bit)
#define		ADLSB		ADMSB+1             // A/D Convert 12 Bit (Last Bit)

#define		REDL		P1.7				// RED LED PIN
#define		BULEL		P1.6				// BULE LED PIN
#define 	WDT			P1.5                // Jump I/O Pin Description
#define 	Watch(X)	X = ( 1 - X )       // Watch Dog, Define Function

//[*]ÄÄÄÄÄ MPM -> SAM Command Define ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define		REAL		'M'                 // Real Data
#define		SEC10		't'					// 10 Sec AVR Data
#define 	MIN1		'O'					// 1 Min AVR Data
#define		MIN10		'T'					// 10 Min AVR Data
#define		INIT		'I'					// Sam Board initiliaze(Only_
#define		RST  		'@'					// After Initiliaze, First data(Min,Max Data) Transmint
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define		int_disable(int)												\
{																			\
	switch(int)																\
	{																		\
		case	ES :	ES  = OFF;	break;                                  \
		case	EX0:	EX0 = OFF;	break;                                  \
		case	EX1:    EX1 = OFF;	break;                                  \
		case	TR0:	TR0 = OFF;	break;									\
		case	TR1:	TR1 = OFF;	break;									\
		case	TR2:	TR2 = OFF;	break;									\
	}																		\
}																			\
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define		int_enable(int)													\
{																			\
	switch(int)																\
	{																		\
		case	ES :	ES  = ON;	break;                                  \
		case	EX0:	EX0 = ON;	break;                                  \
		case	EX1:    EX1 = ON;	break;                                  \
		case	TR0:	TR0 = ON;	break;									\
		case	TR1:	TR1 = ON;	break;									\
		case	TR2:	TR2 = ON;	break;									\
	}																		\
}																			\
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef		unsigned char 	Byte;			// define unsigned char ==> Byte
typedef		unsigned int	Word;			// define unsigned int  ==> Word

//[*]ÄÄÄÄÄ Serial Data Processing Struct ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct Serial__t {
	Byte	Tx_buf[MAX]          ;			// Transmit Buffer (unsigned char)
	Byte	Rx_buf[MAX]          ;			// Receive Buffer (unsigned char)
	Byte	Rx_sp, Rx_ep         ;			// Receive Buffer Start & End Pointer Variable
	Byte	Tx_sp, Tx_ep, Tx_flag;			// Transmit Buffer Start & End & Flag Variable
}	Serial_t;								// Serial struct define

//[*]ÄÄÄÄÄ W/D & W/S Calticulor Variable ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct WdWs__t	{
	Word     		WsMax, WsMin        ;	// Wind Speed Max,Min Variable define
	unsigned long   WsSum               ;	// Wind Speed Sum Variable

	Word     		WdMax, WdMin, WdSum ;	// Wind Direction Max,Min,Sum Variable define
	double			Sin                 ;	// Sin Value
	double			Cos                 ;	// Cosin Value

	Byte			TimeCount			;   // Time Count
}	Values_t;

//[*]ÄÄÄÄÄ MPM Data Processing Struct ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct Command__t {                 // MPM -> SAM Protocol
	Word	Header    ;						// Header : 0xffff
	Byte	Command   ;						// Command Real Data, 10 Sec Data ETC..
	Byte	Id        ;						// SAM Board ID
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
	Byte	Tail      ;						// Tail : 0xfffe
}	Sensor_t;   							// Total = 17 Byte Transmit.

//[*]ÄÄÄÄÄ SAM Board Transmit Data Processing union ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef union Sensor__u {					// Transmit Form Define
	Sensor_t  		Sam                      ;
	Byte 			Bytes[sizeof(Sensor_t)]  ;
}	Sensor_u;

//[*]ÄÄÄÄÄ Variable define ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static	Sensor_u		WsReal, WsSec10, WsMin1, WsMin10;	// Wind Speed Struct
static	Sensor_u		WdReal, WdSec10, WdMin1, WdMin10;	// Wind Direction Struct
static	Serial_t		Sbuf                            ;	// Serial Processing Struct
static	Values_t		Ws_WdValue                      ;	// Wind Speed & Wind Direction Data
// Values_t is Time Value . Sin Cos ..ETC  118 Line is Error .

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void SAM_struct_init(Sensor_u *sensor, Byte code, Byte id, Byte command)
{
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
void init_serial(void)
{
	IE   = 0x80;    // EA Bit Enable
	PCON = 0x80;    // SMOD = 1  SET
	TMOD = 0x25; 	// Timer 1==> mode2, Timer 0 ==> mode 1
	TH1  = 0xf4;	// Reload Value ( Timer 1 Overflow Value )
	TL1  = 0xf4; 	// 22.1184Mhz, SMOD=1 ==> 9600bps
	SCON = 0x50; 	// serial port mode 1, ren(receieve enable)==>1, RB8(STOP bit)==>0
}

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
Byte jumper_check(Byte ID)
{
	// if Jumper Check Roution insert(Knot or m/s or Vector Function)
	ID &= 0x0f;
	ID = ~ID;
	return	ID;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
Byte id_jumper_read(void)
{
	Byte i, ID_comp, ID, ID_ORG;

	while(1)
	{
		Watch(WDT)        ;
		ID = input(JUMPER);

		for( i=0 ; i<10 ;i++ )		ID_comp = input(JUMPER);
		if(ID == ID_comp)							  break;
	}
	return jumper_check(ID);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void initiliaze(void)
{
	Byte ID=0;

	init_serial();

	ID = id_jumper_read();

	SAM_struct_init(&WdReal  , WD_CODE , ID  , REAL  );
	SAM_struct_init(&WdSec10 , WD_CODE , ID  , SEC10 );
	SAM_struct_init(&WdMin1  , WD_CODE , ID  , MIN1  );
	SAM_struct_init(&WdMin10 , WD_CODE , ID  , MIN10 );

	SAM_struct_init(&WsReal  , WS_CODE , ID+1, REAL  );
	SAM_struct_init(&WsSec10 , WS_CODE , ID+1, SEC10 );
	SAM_struct_init(&WsMin1  , WS_CODE , ID+1, MIN1  );
	SAM_struct_init(&WsMin10 , WS_CODE , ID+1, MIN10 );

	int_enable(SERIAL);	int_enable(EXTINT1);	int_enable(EXTINT2);

}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main(void)
{
	initiliaze();		// Serial & Struct Initiliaze

}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt   void    EX0_int(void)			// 4 Hz (Time Process)
{
	switch(Ws_WdValue.TimeCount)
	{
	}
}

interrupt   void    T0_int(void)			// Wind Speed Buffer
{
}

interrupt   void    T2_int(void)			// 4 Hz Watch
{
}

interrupt   void    SCON_int(void)			// Internal Serial Communication
{
}










//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct ws__t	{
	Word (*Wind_speed_cal)(Word Wssum);
}	ws_t;
ws_t ws;

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
	int wspeed;
	if(jump_check == m/s)	ws.Wind_speed_cal = Wind_speed_ms;
	else					ws.Wind_speed_cal = Wind_speed_knot;   //knot

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