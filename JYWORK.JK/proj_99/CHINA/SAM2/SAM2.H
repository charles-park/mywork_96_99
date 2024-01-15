//[*]----------------------------------------------------------------------[*]
//   SAM2.H (Header File)
//[*]----------------------------------------------------------------------[*]
#define		INTERNAL		0
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
#define		JUMPER_ADDR		0xA000
#define		ADC_MSB			0x8000
#define		ADC_LSB			0x8001
//[*]----------------------------------------------------------------------[*]
#define		ADC_STATUS		P1.2	// Hold & Sample
#define		ADC_STS			P1.7	// ADC Conversion OK Singnal
#define		SAMPLE			1
#define		HOLD			0
//[*]----------------------------------------------------------------------[*]
#define		TX_LED(p)      	P1.0 = p
#define		RX_LED(o)		P1.1 = o
//[*]----------------------------------------------------------------------[*]
#define		REAL            0
#define		MIN10           1
#define		INIT			'I'
#define		RST				'@'
//[*]----------------------------------------------------------------------[*]
#define		RUN				1
#define		STOP			0
//[*]----------------------------------------------------------------------[*]
#define		TEMP0_SELECT()	{	P1.3 = 0;	P1.4 = 1;	P1.5 = 0;	}
#define		TEMP1_SELECT()	{	P1.3 = 1;	P1.4 = 1;	P1.5 = 0;	}
#define		TEMP2_SELECT()	{	P1.3 = 0;	P1.4 = 0;	P1.5 = 1;	}
//[*]----------------------------------------------------------------------[*]
#define		SEN_NORMAL		0xFD
#define		SEN_FAIL		0x02
//[*]----------------------------------------------------------------------[*]
typedef struct sam_status_bit__t	{
	byte			clock	:1;
	byte			sensor	:1;
	byte			adc		:1;
	byte			not_used:5;
}	sam_status_bit_t;

typedef	union sam_status_bit__u	{
	sam_status_bit_t	bits;
	byte				byte_data;
}	sam_status_bit_u;

static	sam_status_bit_u	status_temp0;
static	sam_status_bit_u	status_temp1;
static	sam_status_bit_u	status_temp2;
static	sam_status_bit_u	status_prec ;
//[*]----------------------------------------------------------------------[*]
typedef struct 	sensor_pros__t	{
	word		avr  ;
	word		min  ;
	word		max  ;
	word		count;
	long		sum  ;
}	sensor_pros_t;

static	sensor_pros_t	temp0_pros;
static	sensor_pros_t	temp1_pros;
static	sensor_pros_t	temp2_pros;
static	sensor_pros_t	prec_pros ;
//[*]----------------------------------------------------------------------[*]
typedef struct sensor__t	{
	word				header;
	byte				cmd   ;
	byte				status;
	byte				scod  ;
	byte				sid   ;
	word				avr   ;
	word				spc   ;
	word				min   ;
	word				max   ;
	byte				xor   ;
	word				tail  ;
}	sensor_t;

typedef union sensor__u	{
	sensor_t			sam;
	byte				bytes[sizeof(sensor_t)];
}	sensor_u;

static	sensor_u	temp0_data[2];
static	sensor_u	temp1_data[2];
static	sensor_u	temp2_data[2];
static	sensor_u	prec_data[2] ;
static	sensor_u	data_call;
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

static	protocol_grp_t	com;
//[*]----------------------------------------------------------------------[*]
static	word	hz_timeout = 0;
//[*]----------------------------------------------------------------------[*]
static  byte	Sam_id     = 0;
//[*]----------------------------------------------------------------------[*]
static	byte	Sample          = OFF;
static	byte	Sample_count    = 0  ;
static	byte	Jugi_flag       = OFF;
static	word	Jugi_count      = 0  ;
static	word	Jugi_pros_count = 0  ;
//[*]----------------------------------------------------------------------[*]
static	byte	gangdo_pros  = OFF;
static	byte	gangdo_flag  = OFF;
static	word	gangdo_count = 0;
//[*]----------------------------------------------------------------------[*]
static	word	Prec_count   = 0;
//[*]----------------------------------------------------------------------[*]
static	byte	Temp0_flag = OFF;
static	byte	Temp1_flag = OFF;
static	byte	Temp2_flag = OFF;
static	byte	Prec_flag  = OFF;
//[*]----------------------------------------------------------------------[*]
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]----------------------------------------------------------------------[*]
//   Function Define
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] 	void EX0_int 	(void);
interrupt [0x13] 	void EX1_int 	(void);
interrupt [0x23] 	void SCON_int 	(void);
//[*]----------------------------------------------------------------------[*]
					void init_serial_grp		(protocol_grp_t *comx, byte mode, word b_addr, word speed,
												 byte crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro		(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
												 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag		(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event			(protocol_grp_t *comx, byte com_data);
					void protocol_q				(protocol_grp_t *comx, byte protocol_number, byte com_data);
//[*]----------------------------------------------------------------------[*]
					byte rx_check				(variable_t *var);
					byte rx_catch				(variable_t *var);
//[*]----------------------------------------------------------------------[*]
					void data_send				(sensor_u *p, byte call_id, byte cmd);
					void init_send				(byte call_id);
//[*]----------------------------------------------------------------------[*]
					void init_val  				(byte id);
					void maxmin_val				(byte id, word mmax, word mmin);
					word adc_read				(void);
					void temp_cal				(sensor_u *sen, sensor_pros_t *p);
					void prec_cal				(sensor_u *sen, sensor_pros_t *p);
					void jugi_pros				(byte num);
//[*]----------------------------------------------------------------------[*]
					void initialize				(void);
					void variable_initialize	(void);
					byte sensor_id_read			(word addr);
					void struct_initialize		(sensor_u *sen, byte b_cmd, byte b_scod, byte b_sid);
					void pros_value_init		(sensor_pros_t *p);
					void comport_initialize		(void);
					void interrupt_initialize	(void);
					void transmit_check			(void);
					void receive_check			(void);
					void main					(void);
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
