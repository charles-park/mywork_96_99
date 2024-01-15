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
#define		BUFFER_SIZE			4000
//[*]----------------------------------------------------------------------[*]
#define		ON					1
#define		OFF					0
#define		NORMAL				0
#define		FAIL				1
//[*]----------------------------------------------------------------------[*]
#define		CLOCK_NORMAL        0
#define		CLOCK_FAIL          1
#define		SEN_NORMAL          0
#define		SEN_FAIL            2
#define		ADC_NORMAL			0
#define		ADC_FAIL			4
//[*]----------------------------------------------------------------------[*]
#define		MAX_WAIT			2000
//[*]----------------------------------------------------------------------[*]
#define		watch()	  			P1.7 = 1 - P1.7
//[*]----------------------------------------------------------------------[*]
#define		disable(y)			y = OFF
#define		enable(z)			z = ON
//[*]----------------------------------------------------------------------[*]
#define     PRE1_CLR            P1.4
#define     PRE2_CLR            P1.5
//[*]----------------------------------------------------------------------[*]
#define		ADCMSB				0x8000
#define		ADCLSB				0x8001
#define     PRE1                0x9000
#define     PRE2                0xA000
#define		JUMPER				0xB000
#define		MUX_PORT			0xC000
#define		MUX_STATUS			P1.6
#define		AD_STATUS			P3.3
#define		LED_RED				P3.4
#define		LED_BLUE			P3.5
#define		HOLD  				0
#define		SAMPLE              1
//[*]----------------------------------------------------------------------[*]
#define		END_CAL				0xFF		// End calculate
//[*]----------------------------------------------------------------------[*]
//   Sensor Code List(Default.Hpp file Used)
//[*]----------------------------------------------------------------------[*]
#define	TEMPERATURE			2
#define	PRECIPITATION		3
#define	PRE_DETECT			4
#define	PRESSURE			5
#define	HUMIDITY            6
#define	DEW_POINT           7
#define	SUN_RADA            8
#define	SUN_DETECT          9
#define	EVAPORATION         10
#define	GND_HUMIDITY        11
#define	SKY_EMISS           19
#define	GND_EMISS           20
//[*]----------------------------------------------------------------------[*]
typedef struct ack_send__t	{
	unsigned long	header;		// 4
	byte			bdid  ;		// 1
	byte			sen_id;     // 1
	byte			cmd   ;     // 1
	word			size  ;     // 2
	byte			xor   ;     // 1
	byte			add   ;     // 1
	unsigned long	tail  ;     // 4
}	ack_send_t;         		// 15 bytes

typedef union send_ack__u	{
	ack_send_t		send;
	byte			bytes[sizeof(ack_send_t)];
}	ack_send_u;

static 	ack_send_u	ack;
//[*]----------------------------------------------------------------------[*]
typedef struct sensor_field__t	{
	unsigned long	header  ;
	byte			call_cmd;
	word 			gubun   ;
	byte			status  ;
	byte			sen_code;
	byte			sen_id  ;
	float			avr     ;
	float			spc     ;
	float       	min     ;
	float       	max     ;
	byte			xor     ;
	byte			add     ;
	unsigned long	tail    ;
}	sensor_field_t;
//[*]----------------------------------------------------------------------[*]
static	sensor_field_t	maxmin;
typedef struct send_ch__t	{
	sensor_field_t	real;
	sensor_field_t	jugi;
}	send_ch_t;

static	send_ch_t	send_ch[7];
//[*]----------------------------------------------------------------------[*]
typedef struct	var_back__t	{
	int		noise_check;
	byte	err_flag;
	byte	move_cnt;
	byte	move_pos;
	float   move_aver[20];
	float	nujuk_sum;
	float	nujuk_spc;
	float	nujuk_min;
	float	nujuk_max;
	float	nujuk_count;
}	var_back_t;

typedef struct channel__t	{
	byte			reboot_flag;
	byte			ref_ch1;
	byte			ref_ch2;
	var_back_t		var;
//	byte			(*real_func)(var_back_t *p);
	void			(*real_func)(var_back_t *p);
}	channel_t;
static	channel_t	channel[7];
//[*]----------------------------------------------------------------------[*]
typedef struct default__t	{
	// --- CH0 Information ---
	void			(*CH0_RUN_FUNCTION_NUMBER)(var_back_t *p);
	byte			CH0_SENSOR_NAME, CH0_SENSOR_ID  ;
	byte			CH0_REFERENCE1 , CH0_REFERENCE2 ;
	byte			CH0_ON_OFF     ;
	// --- CH1 Information ---
	void			(*CH1_RUN_FUNCTION_NUMBER)(var_back_t *p);
	byte			CH1_SENSOR_NAME, CH1_SENSOR_ID  ;
	byte			CH1_REFERENCE1 , CH1_REFERENCE2 ;
	byte			CH1_ON_OFF     ;
	// --- CH2 Information ---
	void			(*CH2_RUN_FUNCTION_NUMBER)(var_back_t *p);
	byte			CH2_SENSOR_NAME, CH2_SENSOR_ID  ;
	byte			CH2_REFERENCE1 , CH2_REFERENCE2 ;
	byte			CH2_ON_OFF     ;
	// --- CH3 Information ---
	void			(*CH3_RUN_FUNCTION_NUMBER)(var_back_t *p);
	byte			CH3_SENSOR_NAME, CH3_SENSOR_ID  ;
	byte			CH3_REFERENCE1 , CH3_REFERENCE2 ;
	byte			CH3_ON_OFF     ;
	// --- CH4 Information ---
	void			(*CH4_RUN_FUNCTION_NUMBER)(var_back_t *p);
	byte			CH4_SENSOR_NAME, CH4_SENSOR_ID  ;
	byte			CH4_REFERENCE1 , CH4_REFERENCE2 ;
	byte			CH4_ON_OFF     ;
	// --- CH5 Information ---
	void			(*CH5_RUN_FUNCTION_NUMBER)(var_back_t *p);
	byte			CH5_SENSOR_NAME, CH5_SENSOR_ID  ;
	byte			CH5_REFERENCE1 , CH5_REFERENCE2 ;
	byte			CH5_ON_OFF     ;
	// --- CH6 Information ---
	void			(*CH6_RUN_FUNCTION_NUMBER)(var_back_t *p);
	byte			CH6_SENSOR_NAME, CH6_SENSOR_ID  ;
	byte			CH6_REFERENCE1 , CH6_REFERENCE2 ;
	byte			CH6_ON_OFF     ;
	// Sujip Information
	word			SUJIP_COUNT    ;
}	default_t;

static	default_t	DEFAULT;
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
	byte	tx_buf[BUFFER_SIZE];
	byte	rx_buf[BUFFER_SIZE];
	byte	pcnt;
	byte	type;
	byte	xtal;
	word	base;
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	com1;
//[*]----------------------------------------------------------------------[*]
typedef struct	dip_sw__t	{
	byte	id			: 2;
	byte	mode		: 2;
	byte	auto_set	: 1;
	byte	not_used	: 3;
}	dip_sw_t;

typedef union dip_sw__u	{
	dip_sw_t	dip;
	byte		byte_data;
}	dip_sw_u;

static	dip_sw_u	board;
//[*]----------------------------------------------------------------------[*]
//static	byte	Board_id   = 0;
//[*]----------------------------------------------------------------------[*]
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]----------------------------------------------------------------------[*]
static	word	temp_table[] =
{	7,  322, 639, 956, 1272, 1587, 1903, 2218, 2531, 2843, 3155, 3467, 3777, 4088	};
// -50  -40  -30  -20   -10     0   +10   +20   +30   +40   +50   +60   +70   +80
static	word	temp_offset[] =
{	 2,    2,   2,  2,    2,    2,    3,    3,    3,    3,    3,    3,    3,    3	};
// -50  -40  -30  -20   -10     0   +10   +20   +30   +40   +50   +60   +70   +80
//[*]----------------------------------------------------------------------[*]
static	word	dewp_table[] =
{	7,  264, 517, 770, 1018, 1262, 1506, 1747, 1986, 2219, 2459, 2722,
// -40  -35  -30  -25   -20   -15   -10    -5     0    +5   +10   +15
	2984, 3247, 3522, 3826,  4090
//   +20   +25   +30   +35   +40
};
static	word	dewp_offset[] =
{	 2,    2,   2,   2,    2,   2,   3,  3,  3,  3,   3,   3,   3,  3,   3,   3,   2};
// -40   -35  -30  -25   -20  -15  -10  -5   0  +5  +10  +15  +20 +25  +30  +35  +40
//[*]----------------------------------------------------------------------[*]
static 	word	boksa_table[] = {
	13, 28, 40, 52, 63, 126, 187, 248, 312, 372, 432, 496, 556, 617, 680,
//  20  40  60  80 100  200  300  400  500  600  700  800  900 1000 1100
	741, 803, 865, 925, 988, 1050, 1111, 1172, 1236, 1850, 2465, 3080, 3698
// 1200 1300 1400 1500 1600  1700  1800  1900  2000  3000  4000  5000  6000W
};
//[*]----------------------------------------------------------------------[*]
static	byte	Ch_flag[7];
//[*]----------------------------------------------------------------------[*]
static	byte	Real_ch    = 0;
static	byte	Real_flag  = OFF;
//[*]----------------------------------------------------------------------[*]
static	word	Jugi_count = 0;
static	word	Jugi_value = 0;
static	byte	Sujip_ch   = 0;
static	byte	Jugi_flag  = OFF;
//[*]----------------------------------------------------------------------[*]
static	byte	Board_status   = 0;
static	byte	Pros_count     = 0;
static	word	Hz_check_count = 0;

static	word	noize_count    = 0;
//[*]----------------------------------------------------------------------[*]
//   Function Define
//[*]----------------------------------------------------------------------[*]
interrupt [0x03]	void EX0_int (void);
interrupt [0x23] 	void SCON_int(void);
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
					byte rx_cat2(variable_t *var);
//[*]----------------------------------------------------------------------[*]
//					void max_min_pros(var_back_t *p);
//					void temp_pros(var_back_t *p);
//					void pre_pros (var_back_t *p);
//					void pred_pros(var_back_t *p);
//					void humi_pros(var_back_t *p);
//					void sund_pros(var_back_t *p);
//					void jugi_pros(byte s_ch);
//[*]----------------------------------------------------------------------[*]
					void send_ack(protocol_grp_t *comx, byte cmd, word d_size, byte sen_id);
					void real_send(protocol_grp_t *comx, byte ch_num);
					void jugi_send(protocol_grp_t *comx, byte ch_num);
					void send_maxmin_call(protocol_grp_t *comx, byte s_id);
					void move_to_serial_buffer(protocol_grp_t *comx, byte s_data);
//[*]----------------------------------------------------------------------[*]
					void config_0(void);
					void config_1(void);
					void config_2(void);
					void config_3(void);
//[*]----------------------------------------------------------------------[*]
					byte read_board_id(void);
					void com_port_init(void);
					void interrupt_init(void);
					void variable_init(void);
					void board_setup(void);
					void init_channel(channel_t *p, void (*ins)(var_back_t *fun), byte ch1, byte ch2);
					void init_struct(send_ch_t *p, byte s_code, byte s_id);
					void pre_ch_init(byte ch_num);
					void init_sensor_data(byte ch_num);
					void init_maxmin_data(byte ch_num, float max, float min);
					void mux_select(byte mux);
					word ad_convert_read(void);
					void transmit_check(void);
					void receive_check(void);
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
