//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*] Header File Start.......                                             [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define 	MAX			200			// Q Buffer Size Define

#define		SID0		2			// TEMP	Jumper ID
#define		SID1		5			// HUMI	Jumper ID

#define		SCODE0		2	  		// TEMP Sensor Code
#define		SCODE1		6			// HUMI Sensor Code

#define		GREENL      P1.0    	// Send LED Define
#define		REDL        P1.1		// Transmit LED Define
#define		ON			1
#define		OFF			0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		disable(X)	X = OFF 	// Define Function
#define		enable(Y)	Y = ON
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		REAL		'M'     	// Data Fild Define. Gust
#define		SEC10		't'			// 10 sec
#define     MIN1		'O'			// 1 min
#define		MIN10		'T'			// 10 min
#define		INIT		'I'			// Initiliaze
#define		RST			'@'			// Initiliaze & Max,Min Transmit
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		ADCENB		P1.2		// A/D Sample Holda
#define		HOLD		OFF
#define		SAMPLE		ON
#define		ADMSB		0x2000      // A/D High Byte Data Address
#define		ADLSB		ADMSB+1		// A/D Low  Byte Data Address
#define		STATUS		P1.7		// AD Status Pin
#define		ADERR		0x4			// AD Error Value
#define		DLY			15			// AD Delay Value(Count Value)
#define		MUX0		P1.3        // Mux Pin Define
#define		MUX1		P1.4
#define		MUX2		P1.5
#define		AvrCount 	5			//5 sec Avr Value
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Serial Count Function.
#define	get_serial()														\
{																			\
	if(++Com.RxSp >= MAX)		Com.RxSp = 0;								\
}																			\
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Old Value struct
typedef struct	Old__MaxMin	{
	Word	Real	;			// Real Value
	Word	Maxt    ;			// 10 sec Max
	DWord	Sumt    ;			// 10 sec Sum
	Word	Mint    ;			// 10 sec Min
	Word	MaxO    ;			// 1  min Max
	DWord	SumO    ;			// 1  min Sum
	Word 	MinO    ;			// 1  min Min
	Word	MaxT    ;			// 10 min Max
	DWord	SumT    ;			// 10 min Sum
	Word	MinT    ;			// 10 min Min
	Word	Count_t ;			// 10 sec Count Value
	Word	Count_O ;			// 1  min Count Value
	Word	Count_T ;			// 10 min Count Value
}	OLD_MaxMin;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Serial Struct
typedef struct Serial__t	{
	Byte 		RxBuf[MAX];			// Receive Buffer
	Byte 		TxBuf[MAX];			// Transmit Buffer
	signed int	RxSp      ;			// Receive Start Pointer
	signed int	RxEp      ;			// Receive End Pointer
	signed int	TxSp      ;         // Transmit Start Pointer
	signed int	TxEp      ;			// Transmit End Pointer
}	Serial_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// MPM -> SAM Protocol Struct
typedef struct 	Command__t	{
	Word 	Head;      // 0,1	Head
	Byte	Cmd ;      // 2		Command
	Byte	Sid ;      // 3		Jumper ID
	Word	Min ;      // 4,5	'@' Command Max,Min Value Transmit
	Word	Max ;      // 6,7
	Byte	Cks ;      // 8     Check Sum
	Word	Tail;      // 9,10	Tail                    	Total = 11 Bytes
}	Command_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef union 	Command__u	{
	Command_t	Mpm						;
	Byte  		Bytes[sizeof(Command_t)];
}	Command_u;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// SAM->MPM Protocol Struct
typedef struct	Sensor__t	{
	Word 	Head  ;		// 0,1
	Byte	Cmd   ;     // 2
	Byte 	Sta	  ;     // 3
	Byte 	Scode ;     // 4
	Byte	Sid   ;     // 5
	Word	Avr   ;     // 6,7
	Word	Spc   ;     // 8,9
	Word	Max   ;     // 10,11
	Word	Min   ;     // 12,13
	Byte	Cks   ;     // 14
	Word	Tail  ;     // 15,16	Total = 17 Bytes
}	Sensor_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef union Sensor__u	{
	Sensor_t	Sam                    ;
	Byte		Bytes[sizeof(Sensor_t)];
}	Sensor_u;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Function Pointer Struct
typedef struct TMP_HMI_select   {
		void (*Select)(void);
}       Select__F;
Select__F       Loop;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// static variable initiliaze
static 		Sensor_u     	TEMP_R,TEMP_t,TEMP_O,TEMP_T	;	// Temp
static 		Sensor_u     	HUMI_R,HUMI_t,HUMI_O,HUMI_T	;   // Humi
static 		Command_u		Mpm						   	;
static 		Serial_t		Com							;
static 		OLD_MaxMin		TEMP_OLD,HUMI_OLD			;
static 		Word			ERcount						;
static 		Word			ERROR						;
static 		Word			DLYcount					;
static 		Word			ADValue						;
static 		Byte			Event						;
static 		Byte			Init						;
static 		bit				Flag						;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//static  	Word            TEMP_Move[AvrCount] 		;
//static    Word			TEMP_Point					;
//static  	Word            HUMI_Move[AvrCount] 		;
//static    Word			HUMI_Point					;
//static    Word			MovCount					;
//static    Word			MovValue					;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Function define ( Function List ).
interrupt 	void SCON_int(void);
interrupt 	void EX0_int(void);
			void init_struct(Sensor_u *Sen, Byte Cmd, Byte Sid, Byte Scode);
			void init_serial(void);
			void initiliaze(void);
			void check_mpm(void);
			void send_struct(Sensor_u *Send);
			signed int Loc(signed int Comp, signed int Step);    // Q Buffer Point Set
			void main(void);
			void init_variable(void);
			void AD_read(void);
			void Temp_cal(void);
			void Humi_cal(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
