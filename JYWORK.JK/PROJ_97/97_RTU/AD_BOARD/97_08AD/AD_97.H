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
#define		BUFFER_SIZE			2000
//[*]----------------------------------------------------------------------[*]
#define		ON					1
#define		OFF					0
#define		NORMAL				0
#define		FAIL				1
//[*]----------------------------------------------------------------------[*]
//   16 bit prescaler timer mode
//   interrupt is  1 sec of 14 appear
//[*]----------------------------------------------------------------------[*]
#define		MAX_WAIT			30
//[*]----------------------------------------------------------------------[*]
#define		watch()	  			P1.7 = 1 - P1.7
#define		tp()	  			P3.3 = 1 - P3.3
//[*]----------------------------------------------------------------------[*]
#define		disable(y)			y = OFF
#define		enable(z)			z = ON
//[*]----------------------------------------------------------------------[*]
#define		REAL_JUGI			0x0000
//[*]----------------------------------------------------------------------[*]
#define		JUMPER				0xB000
//[*]----------------------------------------------------------------------[*]
#define		MUX_STATUS			P1.6
#define		ADCMSB				0x8000
#define		ADCLSB				0x8001
#define		HOLD  				0
#define		SAMPLE              1
//[*]----------------------------------------------------------------------[*]
#define		END_CAL				0xFF		// End calculate
//[*]----------------------------------------------------------------------[*]
typedef struct ack_send__t	{
	byte	header[4];		// 4
	byte	bdid     ;      // 1
	byte	sen_id   ;      // 1
	byte	cmd      ;      // 1
	word	size     ;      // 2
	byte	xor      ;      // 1
	byte	add      ;      // 1
	byte	tail  [4];      // 4
}	ack_send_t;             // 15 bytes

typedef union send_ack__u	{
	ack_send_t		send;
	byte			bytes[sizeof(ack_send_t)];
}	ack_send_u;

static 	ack_send_u	ack;
//[*]----------------------------------------------------------------------[*]
//   channel infomation ( 0-4 : 5 channel & Humi 2 channel )
//[*]----------------------------------------------------------------------[*]
typedef struct status__t	{
	byte			clock	:1;
	byte			sensor	:1;
	byte			adc		:1;
	byte			not_used:5;
}	status_t;

typedef union status__u		{
	status_t    	bits     ;
	byte			byte_data;
}	status_u;

typedef struct sensor_field__t	{
	byte			header[4];
	byte			call_cmd ;
	byte			gubun [2];
	status_u		status   ;
	byte			sen_code ;
	byte			sen_id   ;
	float			avr      ;
	float			spc      ;
	float       	min      ;
	float       	max      ;
	byte			xor      ;
	byte			add      ;
	byte			tail  [4];
}	sensor_field_t;

typedef struct var_back__t	{
	byte		mux        ;
	byte		init_flag  ;
	byte		reboot_flag;
	word		ref_w1     ;
	word		ref_w2     ;
	float		ref_f1     ;
	float		ref_f2     ;

	float		real_sum   ;
	float		real_spc   ;
	float		real_min   ;
	float		real_max   ;

	float		jugi1_sum   ;
	float	    jugi1_spc   ;
	float		jugi1_min   ;
	float		jugi1_max   ;
	float		nujuk1_sum  ;
	float		nujuk1_spc  ;
	float		nujuk1_min  ;
	float		nujuk1_max  ;
	word		nujuk1_count;

	float		jugi2_sum   ;
	float		jugi2_spc   ;
	float		jugi2_min   ;
	float		jugi2_max   ;
	float		nujuk2_sum  ;
	float		nujuk2_spc  ;
	float		nujuk2_min  ;
	float		nujuk2_max  ;
	word		nujuk2_count;
}	var_back_t;

typedef struct ch_info__t	{
	void			(*func)(var_back_t *p);
	var_back_t		var       ;
	sensor_field_t	sensor    ;
	byte 			ref_id1   ;
	byte			ref_id2   ;
	float			ref_value1;
	float			ref_value2;
	float			offset1   ;
	float			offset2   ;
	float			offset3   ;
	float			offset4   ;
	float			offset5   ;
	float			offset6   ;
	float			reserve1  ;
	float			reserve2  ;
}	ch_info_t;

typedef struct flag__t	{
	byte		ch0			:1;
	byte		ch0_used	:1;
	byte		ch1			:1;
	byte		ch1_used	:1;
	byte		ch2			:1;
	byte		ch2_used	:1;
	byte		ch3			:1;
	byte		ch3_used	:1;
	byte		ch4			:1;
	byte		ch4_used	:1;
	byte		ch5			:1;
	byte		ch5_used	:1;
	byte		ch6			:1;
	byte		ch6_used	:1;
	byte		ch7			:1;
	byte		ch7_used	:1;
}	flag_t;

typedef struct flag__u	{
	flag_t		bits;
	word		word_data;
}	flag_u;

typedef struct channel__t	{
	word            jugi1      ;
	byte            jugi1_flag ;
	byte            jugi1_used ;
	word			jugi2      ;
	byte			jugi2_flag ;
	byte			jugi2_used ;
	flag_u			flag       ;
	ch_info_t		num[7]     ;
}	channel_t;

static	channel_t	ch;
//[*]----------------------------------------------------------------------[*]
//   debug struct (test send function used)
//[*]----------------------------------------------------------------------[*]
typedef struct test_send__t		{
	byte	t  [10];
	byte	t1 [10];
	byte	t2 [10];
}	test_send_t;

typedef union test_send__u	{
	test_send_t		send;
	byte			bytes[sizeof(test_send_t)];
}	test_send_u;

static	test_send_u		test;

static	byte	Test_send_flag = OFF;
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
static	byte	Board_id       = 0 ;
static	word	Pros_count     = 0 ;
static	word	Sec_count1     = 0 ;
static	word	Sec_count2     = 0 ;
static	word	Hz_check_count = 0 ;
static	word	Boksa_offset   = 1 ;
//[*]----------------------------------------------------------------------[*]
//   Function Define
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] 	void EX0_int  (void);     /* External Interrupt 0 */
interrupt [0x0B] 	void T0_int   (void);     /* Timer 0 Overflow */
interrupt [0x23] 	void SCON_int (void);
					void test_send(void);
					void send_ack(protocol_grp_t *comx, byte cmd, word d_size, byte sen_id);
					void send_mpm(protocol_grp_t *comx, word jugi, ch_info_t *send, byte number);
					void send_maxmin_call(protocol_grp_t *comx, ch_info_t *send);
					void move_to_serial_buffer(protocol_grp_t *comx, byte s_data);
					void init_serial_grp  (protocol_grp_t *comx, byte mode, word b_addr, word speed,
										   byte crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro  (protocol_grp_t *comx, byte protocol_number, byte buffer_size,
										   byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event   (protocol_grp_t *comx, byte com_data);
					void protocol_q       (protocol_grp_t *comx, byte protocol_number, byte com_data);
					byte rx_chk (variable_t *var);
					byte rx_cat1(variable_t *var);
					byte rx_cat2(variable_t *var);
					void init_channel_struct(channel_t *p, byte bunho, void (*ins)(var_back_t *fun),
											 byte s_code, byte s_id, byte id1, byte id2);
					void init_maxmin_data(ch_info_t *p, float max, float min);
					void init_sensor_data(ch_info_t *p);
					word ad_convert_read(byte mux_ch);
					float humi_cal      (float ddewp,float ttemp);
					void flag_clear     (byte chn);
					byte flag_status    (byte chn);
					byte channel_status (byte chn);
					void mux_select     (byte mux);
					void temp_pros      (var_back_t *p);
					void dewp_pros      (var_back_t *p);
					void humi_pros      (var_back_t *p);
					void boksa_pros     (var_back_t *p);
					void temp_test      (var_back_t *p);
					void dewp_test      (var_back_t *p);
					void boksa_test     (var_back_t *p);
					void max_min_pros   (var_back_t *p);
					byte read_board_id  (void);
					void com_port_init  (void);
					void interrupt_init (void);
					void variable_init  (void);
					void transmit_check (void);
					void receive_check  (void);
					void jugi1_pros     (void);
					void jugi2_pros     (void);
					void main           (void);
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
