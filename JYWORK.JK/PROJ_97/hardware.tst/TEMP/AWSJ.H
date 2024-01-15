//[*]ÄÄÄÄÄ Start Header File ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
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
#define 	REAL		 0
#define 	SEC10  		 1
#define 	MIN1 		 2
#define 	MIN10		 3
#define		INIT		'I'					// Sam Board initiliaze(Only_
#define		RST  		'@'					// After Initiliaze, First data(Min,Max Data) Transmint
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define	Serial_Get()    													\
{                        													\
	if((++Sbuf.Rx_sp) >= MAX)	Sbuf.Rx_sp = 0 ;							\
}                                                                           \
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct ws_wd_cal_select 	{
	void (*Select)(void);
}	Select__F;
Select__F	Loop;
// Select interrupt Function --> Cal & Wind Direction & Wind Speed & Storage.. ETC
//[*]ÄÄÄÄÄ Serial Data Processing Struct ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct Serial__t {
	Byte	Tx_buf[MAX]          ;			// Transmit Buffer (unsigned char)
	Byte	Rx_buf[MAX]          ;			// Receive Buffer (unsigned char)
	Byte	Rx_sp, Rx_ep         ;			// Receive Buffer Start & End Pointer Variable
	Byte	Tx_sp, Tx_ep		 ;			// Transmit Buffer Start & End & Flag Variable
}	Serial_t;								// Serial struct define
//[*]ÄÄÄÄÄ W/D & W/S Calticulor Variable ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct WdWs__t	{
	Word     		WsMax               ;	// Wind Speed Max,Min Variable define
	unsigned long   WsReal,WsSec10      ;	// Wind Speed Sum Variable
	unsigned long   WsMin1,WsMin10      ;	// Wind Speed Sum Variable

	Word     		WdMax, WdMin, WdSum ;	// Wind Direction Max,Min,Sum Variable define
	double			SinReal,CosReal     ;
	double			SinSec10,CosSec10   ;
	double			SinMin1,CosMin1     ;
	double			SinMin10,CosMin10   ;

	Byte			TimeCount			;   // Time Count
	Byte			Flag				;   // Function Flag
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
//[*]ÄÄÄÄÄ Variable define ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
static	Sensor_u		Sensor[4][2]                        ;	// Wind Speed Struct & Wind Direction Struct
static	Serial_t		Sbuf                                ;	// Serial Processing Struct
static	Values_t		EVENT        	                    ;	// Wind Speed & Wind Direction Data
static  Byte ID         =           0                       ;
// Values_t is Time Value . Sin Cos ..ETC  118 Line is Error .

//[*]ÄÄÄÄÄÄÄÄ Define Function ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void SAM_struct_init(Sensor_u *sensor, Byte cd, Byte id, Byte command);
void inter_initiliaze(void);
Byte jumper_check(Byte ID_CON);
Byte id_jumper_read(void);
void initiliaze(void);
int Point(int Bufsize, int Now, int Step);
void Check_receive(void);
void Send_to_Mpm(Sensor_u *Send);     // Check Porgram.... 97.2.11
void main(void);
void Ws_Read(void);
void Wd_Read(void);
void Ws_Wd_cal(void);
interrupt   void    EX0_int(void);			// 4 Hz (Time Process)
interrupt   void    SCON_int(void);			// Internal Serial Communication
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
