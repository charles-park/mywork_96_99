//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*] Header File Start.......                                             [*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define 	MAX			500			// Q Buffer Size Define
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		SID0		0x0 		// TEMP	 Jumper ID
#define		SID1		0x1 		// HUMI	 Jumper ID
#define		SID2		0x3 		// PREC	 Jumper ID
#define		SID3		0x4 		// SUN	 Jumper ID
#define		SID4		0x6 		// SUN/D Jumper ID
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		SCODE0		2	  		// TEMP  Sensor Code
#define		SCODE1		6	  		// HUMI  Sensor Code
#define		SCODE2		3	  		// PREC  Sensor Code
#define		SCODE3		8	  		// SUN   Sensor Code
#define		SCODE4		9			// SUN/D Sensor Code
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define     GREENL      P1.1        // Send LED Define
#define     REDL        P1.0        // Transmit LED Define
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
#define		ADMSB		0x8000      // A/D High Byte Data Address
#define		ADLSB		ADMSB+1		// A/D Low  Byte Data Address
#define		STATUS		P1.7		// AD Status Pin
#define		ADERR		0x4			// AD Error Value
#define		DLY			15			// AD Delay Value(Count Value)
#define		MUX0		P1.3        // Mux Pin Define
#define		MUX1		P1.4
#define		MUX2		P1.5
#define		SUND_CHECK	P1.6		// Sun Dectect Check Pin
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Serial Count Function.
#define	get_serial()														\
{																			\
	if(++Com.RxSp >= MAX)		Com.RxSp = 0;								\
}																			\
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Old Value struct
typedef struct	Old__MaxMin	{
	Word	Maxt    ;			// 10 sec Max
	Word	Sumt    ;			// 10 sec Sum
	Word	Mint    ;			// 10 sec Min
	Word	MaxO    ;			// 1  min Max
	Word	SumO    ;			// 1  min Sum
	Word 	MinO    ;			// 1  min Min
	Word	MaxT    ;			// 10 min Max
	Word	SumT    ;			// 10 min Sum
	Word	MinT    ;			// 10 min Min
	Word	Count_t ;			// 10 sec Count Value
	Word	Count_O ;			// 1  min Count Value
	Word	Count_T ;			// 10 min Count Value
}	OLD_MaxMin;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Serial Struct
typedef struct Serial__t	{
	Byte 	RxBuf[MAX];			// Receive Buffer
	Byte 	TxBuf[MAX];			// Transmit Buffer
	Word	RxSp      ;			// Receive Start Pointer
	Word	RxEp      ;			// Receive End Pointer
	Word	TxSp      ;         // Transmit Start Pointer
	Word	TxEp      ;			// Transmit End Pointer
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
typedef struct Sensor_select   {
		void (*Select)(void);
}       Select__F;
Select__F       Loop;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// static variable initiliaze
static 		Sensor_u     	TEMP_R,TEMP_t,TEMP_O,TEMP_T	;	// Temp
static 		Sensor_u     	HUMI_R,HUMI_t,HUMI_O,HUMI_T	;   // Humi
static 		Sensor_u     	PREC_R,PREC_t,PREC_O,PREC_T	;   // Precipi
static 		Sensor_u     	SUN_R , SUN_t, SUN_O, SUN_T	;   // Sun
static 		Sensor_u     	SUND_R,SUND_t,SUND_O,SUND_T	;   // Sun of dectect
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static 		Command_u		Mpm						   	;
static 		Serial_t		Com							;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static 		OLD_MaxMin		TEMP_OLD,HUMI_OLD			;
static 		OLD_MaxMin		PREC_OLD,SUN_OLD,SUND_OLD	;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static 		Word			ERcount						;
static 		Word			ERROR						;
static 		Word			DLYcount					;
static 		Word			ADValue						;
static 		Word			Sun_Day						;
static 		Byte			Event						;
static 		Byte			Init						;
static 		bit				Flag						;
static 		bit  			PREC_Flag					;// Precipi Dectect.
static 		Word 			PREC_Count					;
static 		double  		Sun_Value;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Function define ( Function List ).
interrupt 	void EX0_int(void);    		// External Interrupt Function
interrupt   void EX1_int(void);    		// Precipi cal
interrupt 	void SCON_int(void);   		// Serial Interrupt Function
			void init_variable(void);
			void init_struct(Sensor_u *Sen, Byte Cmd, Byte Sid, Byte Scode);
			void init_serial(void);
			void initiliaze(void);
			void check_mpm(void);    	// Protocol Check MPM -> SAM Board
			void send_struct(Sensor_u *Send);   		// SAM -> MPM DATA Transmit
			signed int Loc(signed int Comp, signed int Step);    // Q Buffer Point Set
			void AD_read(void);          // A/D Convert DATA Read Function
			void Temp_cal(void);        	// TEMP Calculation Function
			void Sun_cal(void);          // Sun Calculate
			void Humi_cal(void);        	// HUMI Calculation Function
			void main(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
