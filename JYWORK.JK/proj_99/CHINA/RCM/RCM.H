//[*]----------------------------------------------------------------------[*]
//   OLD_RCM.H (Header File)
//[*]----------------------------------------------------------------------[*]
#define		INTERNAL		0
#define		EXTERNAL		1
//[*]----------------------------------------------------------------------[*]
#define		DATABIT7		0x2
#define		DATABIT8		0x3
//[*]----------------------------------------------------------------------[*]
#define		STOP1			0x0
#define		STOP2			0x4
//[*]----------------------------------------------------------------------[*]
#define		PARITY_NO		0x0
#define		PARITY_ODD		0x8
#define		PARITY_EVEN		0x18
//[*]----------------------------------------------------------------------[*]
#define		DLL(x)			(x+0)		// 16C450 DLL Register
#define		DLM(x)			(x+1)       // 16C450 DLM Register
#define		THR(x)			(x+0)       // 16C450 THR Register
#define		RBR(x)			(x+0)       // 16C450 RBR Register
#define		IER(x)			(x+1)       // 16C450 IER Register
#define		IIR(x)			(x+2)       // 16C450 IIR Register
#define		LCR(x)			(x+3)       // 16C450 LCR Register
#define		MCR(x)			(x+4)       // 16C450 MCR Register
#define		LSR(x)			(x+5)       // 16C450 LSR Register
#define		MSR(x)			(x+6)       // 16C450 MSR Register
//[*]----------------------------------------------------------------------[*]
#define		BUFFER_SIZE		1000
//[*]----------------------------------------------------------------------[*]
#define		M11				0
#define		M22				1
#define		M3				2
#define		M1				3
//[*]----------------------------------------------------------------------[*]
#define		BPS9600			0
#define		BPS4800			1
#define		BPS2400			2
#define		BPS1200			3
//[*]----------------------------------------------------------------------[*]
#define		SMOD_ON			1
#define		SMOD_OFF		0
#define		NOT_USED		0
//[*]----------------------------------------------------------------------[*]
#define		disable(y)		y = OFF
#define		enable(z)		z = ON
//[*]----------------------------------------------------------------------[*]
#define		ON				1
#define		OFF				0
#define		NORMAL			0
#define		FAIL			1
//[*]----------------------------------------------------------------------[*]
#define		MPM_RESET(r)	P1.7 = 1 - r
//[*]----------------------------------------------------------------------[*]
#define		EXT_COM			0x8000
#define		JUMPER			0x9000
#define		ADC_LSB			0xA000
#define		ADC_MSB			0xB000
//[*]----------------------------------------------------------------------[*]
#define		ADC_LSB_EOC		P1.0
#define		ADC_MSB_EOC		P1.1
#define		LSB				0
#define		MSB 			1
//[*]----------------------------------------------------------------------[*]
#define		TX_LED1(p)      P1.3 = p
#define		RX_LED1(o)		P1.4 = o
#define		TX_LED2(s)      P1.5 = s
#define		RX_LED2(q)      P1.6 = q
//[*]----------------------------------------------------------------------[*]
#define		ERROR_TIME		5000	// Wait Time 10 Min
//[*]----------------------------------------------------------------------[*]
//   Time Struct
//[*]----------------------------------------------------------------------[*]
typedef struct time__t	{
	byte			year ;
	byte			month;
	byte			day  ;
	byte			hour ;
	byte			min  ;
	byte			sec  ;
}	time_t;

static	time_t	t;
//[*]----------------------------------------------------------------------[*]
//   Voltage Status Bit Define Struct
//[*]----------------------------------------------------------------------[*]
typedef struct volt_bit__t	{
	word			pv5_1	:1;
	word			pv5_2	:1;
	word			pv12_1	:1;
	word			pv12_2	:1;
	word			mv12_1	:1;
	word			mv12_2	:1;
	word			battery	:1;
	word			ac_volt	:1;
	word			fan		:1;
	word			door	:1;
	word			wd		:1;
	word			ws		:1;
	word			ped		:1;
	word			prs		:1;
	word			rh		:1;
	word			tsp		:1;
}	volt_bit_t;

typedef union volt_bit__u	{
	volt_bit_t		bits     ;
	word			word_data;
}	volt_bit_u;
//[*]----------------------------------------------------------------------[*]
//   Control Board Bit Define Struct
//[*]----------------------------------------------------------------------[*]
typedef struct board_bit__t	{
	byte			RCM_STA :1;
	byte			MPM_STA :1;
	byte			MEM_STA :1;
	byte			CPM_STA :1;
	byte			N_USED  :4;
}	board_bit_t;

typedef union board_bit__u	{
	board_bit_t		bits     ;
	byte			byte_data;
}	board_bit_u;
//[*]----------------------------------------------------------------------[*]
//   Board Status & Sensor Status Define Struct
//[*]----------------------------------------------------------------------[*]
typedef struct status_data__t	{
	volt_bit_u		volt     ;
	byte			board[4] ;
	board_bit_u		cnt_board;
	byte			sensor[4];
}	status_data_t;
//[*]----------------------------------------------------------------------[*]
//   Voltage Detail Value Struct (Read Value X 10)
//[*]----------------------------------------------------------------------[*]
typedef struct detail__t	{
	word			pv5_1  ;
	word			pv5_2  ;
	word			pv12_1 ;
	word			pv12_2 ;
	word			mv12_1 ;
	word			mv12_2 ;
	word			battery;
	word			ac_volt;
	word			fan    ;
	word			door   ;
	word			wd     ;
	word			ws     ;
	word			ped    ;
	word			prs    ;
	word			rh     ;
	word			tsp    ;
}	detail_t;
//[*]----------------------------------------------------------------------[*]
//   RCM to MPM Send Form
//[*]----------------------------------------------------------------------[*]
typedef struct rcm_to_mpm__t	{
	word			header;
	byte			cmd   ;
	status_data_t	sta   ;
	detail_t		volt  ;
	byte			xor   ;
	word			tail  ;
}	rcm_to_mpm_t;

typedef union rcm_to_mpm__u	{
	rcm_to_mpm_t	send                       ;
	byte			bytes[sizeof(rcm_to_mpm_t)];
}	rcm_to_mpm_u;

static rcm_to_mpm_u		mpm;
//[*]----------------------------------------------------------------------[*]
//   RCM to HOST SET Instruction Ack Struct
//[*]----------------------------------------------------------------------[*]
typedef struct rcm_to_host_ack__t	{
	word			header;
	word			logid ;
	byte			cmd   ;
	byte			year  ;
	byte			month ;
	byte			day   ;
	byte			hour  ;
	byte			min   ;
	byte			ack[4];
	byte			xor   ;
	byte			add   ;
	word			tail  ;
}	rcm_to_host_ack_t;

typedef union rcm_to_host_ack__u	{
	rcm_to_host_ack_t		send;
    byte                    bytes[sizeof(rcm_to_host_ack_t)];
}	rcm_to_host_ack_u;

static rcm_to_host_ack_u	rcm_to_host_ack;
//[*]----------------------------------------------------------------------[*]
//   RCM to HOST Send Form(Logger Status)
//[*]----------------------------------------------------------------------[*]
typedef struct status_send_to_host__t	{
	word			header;
	word			logid ;
	byte			cmd   ;
	byte			year  ;
	byte			month ;
	byte			day   ;
	byte			hour  ;
	byte			min   ;
	byte			sec   ;
	status_data_t	sta   ;
	byte			xor   ;
	byte			add   ;
	byte			tail  ;
}	status_send_to_host_t;

typedef union status_send_to_host__u	{
	status_send_to_host_t		send                                ;
	byte						bytes[sizeof(status_send_to_host_t)];
}	status_send_to_host_u;

static	status_send_to_host_u	host_status;
//[*]----------------------------------------------------------------------[*]
//   RCM to HOST Send Form(Detail Voltage)
//[*]----------------------------------------------------------------------[*]
typedef struct detail_send_to_host__t	{
	word			header;
	word			logid ;
	byte			cmd   ;
	byte			year  ;
	byte			month ;
	byte			day   ;
	byte			hour  ;
	byte			min   ;
	byte			sec   ;
	detail_t		volt  ;
	byte			xor   ;
	byte			add   ;
	word			tail  ;
}	detail_send_to_host_t;

typedef union detail_send_to_host__u	{
	detail_send_to_host_t	send                                ;
	byte					bytes[sizeof(detail_send_to_host_t)];
}	detail_send_to_host_u;

static	detail_send_to_host_u 	host_detail;
//[*]----------------------------------------------------------------------[*]
//   Serial Protocol Cat & Chk Struct
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
	byte	tx_buf[BUFFER_SIZE];
	byte	rx_buf[BUFFER_SIZE];
	byte	pcnt;
	byte	type;
	word	base;
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	com_int, com_ext;
//[*]----------------------------------------------------------------------[*]
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]----------------------------------------------------------------------[*]
static	word	read_volt_ararry[20];
static	word	msb_volt_ch  = 0;
static	word	lsb_volt_ch  = 0;
static	word	mpm_status   = 0;
static	word	pros_volt_ch = 0;
static	word	LOG_ID       = 0;
static 	byte	channel_select = LSB;
static	word	call_count   = 0;
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] void EX0_int (void);    /* External Interrupt 0 */
interrupt [0x13] void EX1_int (void);     /* External Interrupt 1 */
interrupt [0x23] void SCON_int (void);    /* Serial Port */
				 void com_port_initiliaze(void);
				 void transmit_data_check(void);
				 void receive_data_check(void);
				 void read_voltage(word ad_select);
				 void pros_voltage(void);
				 void volts_status_check(volt_bit_t *d);
				 void mpm_reset(void);
				 void variable_initiliaze(void);
				 void send_to_host_ack(protocol_grp_t *com, byte gubun);
				 void send_to_mpm(protocol_grp_t *com);
				 void send_to_host_status(protocol_grp_t *com);
				 void send_to_host_detail(protocol_grp_t *com);
				 byte rx_check(variable_t *var);
				 byte rx_catch_int(variable_t *var);
				 byte rx_catch_ext(variable_t *var);
				 void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
									   byte crystal, byte smod, byte status, byte protocol_count);
				 void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
									   byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
				 void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
				 void protocol_event(protocol_grp_t *comx, byte com_data);
				 void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);
				 void main(void);
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

// interrupt [0x03] void EX0_int (void);    /* External Interrupt 0 */

// interrupt [0x0B] void T0_int (void);      /* Timer 0 Overflow */

// interrupt [0x13] void EX1_int (void);     /* External Interrupt 1 */

// interrupt [0x1B] void T1_int (void);      /* Timer 1 Overflow */

// interrupt [0x23] void SCON_int (void);    /* Serial Port */

// interrupt [0x2B] void T2_int (void);      /* Timer 2 Overflow */

// interrupt [0x2B] void T2EX_int (void);    /* Negative Transition on T2EX */
//[*]----------------------------------------------------------------------[*]
