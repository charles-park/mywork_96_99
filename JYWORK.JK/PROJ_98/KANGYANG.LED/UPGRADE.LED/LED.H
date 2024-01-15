//[*]----------------------------------------------------------------------[*]
#define		INTERNAL			0
#define		EXTERNAL			1
//[*]----------------------------------------------------------------------[*]
#define		DATABIT7			0x2
#define		DATABIT8			0x3
//[*]----------------------------------------------------------------------[*]
#define		STOP1				0x0
#define		STOP2				0x4
//[*]----------------------------------------------------------------------[*]
#define		PARITY_NO			0x0
#define		PARITY_ODD			0x0
#define		PARITY_EVEN			0x0
//[*]----------------------------------------------------------------------[*]
#define		DLL(x)				(x+0)		// 16C550 DLL Register
#define		DLM(x)				(x+1)       // 16C550 DLM Register
#define		THR(x)				(x+0)       // 16C550 THR Register
#define		RBR(x)				(x+0)       // 16C550 RBR Register
#define		IER(x)				(x+1)       // 16C550 IER Register
#define		IIR(x)				(x+2)       // 16C550 IIR Register
#define		LCR(x)				(x+3)       // 16C550 LCR Register
#define		MCR(x)				(x+4)       // 16C550 MCR Register
#define		LSR(x)				(x+5)       // 16C550 LSR Register
#define		MSR(x)				(x+6)       // 16C550 MSR Register
//[*]----------------------------------------------------------------------[*]
#define		M11					0
#define		M22					1
#define		M3					2
#define		M1					3
//[*]----------------------------------------------------------------------[*]
#define		BPS9600				0
#define		BPS4800				1
#define		BPS2400				2
#define		BPS1200				3
//[*]----------------------------------------------------------------------[*]
#define		SMOD_ON				1
#define		SMOD_OFF			0
#define		NOT_USED			0
//[*]----------------------------------------------------------------------[*]
#define		TX_SIZE				2
#define		RX_SIZE				1000
//[*]----------------------------------------------------------------------[*]
#define		ON					1
#define		OFF					0
#define		NORMAL				0
#define		FAIL				1
//[*]----------------------------------------------------------------------[*]
#define		disable(y)			y = OFF
#define		enable(z)			z = ON
//[*]----------------------------------------------------------------------[*]
#define		WD					0x8000
#define		WS              	0x8004
#define		TEMP_AVR        	0x8008
#define		TEMP_MIN        	0x800C
#define		PRE_AVR         	0x9000
#define		PRE_MIN         	0x9004
#define		HUMI            	0x9008
//[*]----------------------------------------------------------------------[*]
typedef struct disp_buf__t	{
	byte	wd      [5];
	byte	ws      [5];
	byte	temp_avr[5];
	byte	temp_min[5];
	byte	pre_avr [5];
	byte	pre_min [5];
	byte	humi    [5];
}	disp_buf_t;

static disp_buf_t	disp_buf;
//[*]----------------------------------------------------------------------[*]
// ASCII TABLE :                0  1   2   3    4    5    6  7    8    9
static	byte	led_font[] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111 };
//[*]----------------------------------------------------------------------[*]
typedef struct variable__t	{
	word	p_sp;
	word	p_ep;
	word	size;
	byte	flag;
	byte	*buf;
}	variable_t;

typedef struct protocol__t	{
	variable_t		var             ;
	byte	(*pcheck)(variable_t *p);
	byte	(*pcatch)(variable_t *p);
}	protocol_t;

typedef struct protocol_grp__t	{
	word	rx_sp, rx_ep;
	word 	tx_sp, tx_ep;
	byte	tx_flag     ;
	byte	cat_flag    ;
	byte	tx_buf[TX_SIZE];
	byte	rx_buf[RX_SIZE];
	byte	pcnt;
	byte	type;
	byte	xtal;
	word	base;
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	com1;
//[*]----------------------------------------------------------------------[*]
static	byte	Led_send = OFF;
static	word	count1 = 0;
static 	word	count2 = 0;
static	byte	Minus1 = OFF;
static	byte	Minus2 = OFF;
//[*]----------------------------------------------------------------------[*]
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]----------------------------------------------------------------------[*]
//   Function Define
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] 	void EX0_int (void);
interrupt [0x23] 	void SCON_int (void);
//[*]----------------------------------------------------------------------[*]
					void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
										  byte crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
										 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event(protocol_grp_t *comx, byte com_data);
					void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);
//[*]----------------------------------------------------------------------[*]
					byte rx_chk(variable_t *var);
					byte rx_cat1(variable_t *var);
					void led_data_catch(variable_t *var);
					void disp_buffer_save(byte scode, word v1, word v2);
//[*]----------------------------------------------------------------------[*]
					void com_port_init(void);
					void interrupt_init(void);
					void variable_init(void);
					void receive_check(void);
					void delay(void);
					void led_disp(word addr, byte *p);
					void main(void);
//[*]----------------------------------------------------------------------[*]
/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

// interrupt [0x03] void EX0_int  (void);	/* External Interrupt 0        */

// interrupt [0x0B] void T0_int   (void);   /* Timer 0 Overflow            */

// interrupt [0x13] void EX1_int  (void);   /* External Interrupt 1        */

// interrupt [0x1B] void T1_int   (void);   /* Timer 1 Overflow            */

// interrupt [0x23] void SCON_int (void);   /* Serial Port                 */

// interrupt [0x2B] void T2_int   (void);   /* Timer 2 Overflow            */

// interrupt [0x2B] void T2EX_int (void);   /* Negative Transition on T2EX */
//[*]----------------------------------------------------------------------[*]
