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
#define		MAX_WAIT			2000
//[*]----------------------------------------------------------------------[*]
#define		watch()	  			P1.7 = 1 - P1.7
//[*]----------------------------------------------------------------------[*]
#define		disable(y)			y = OFF
#define		enable(z)			z = ON
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
	unsigned long	header;		 // 4
	byte			bdid  ;      // 1
	byte			sen_id;      // 1
	byte			cmd   ;      // 1
	word			size  ;      // 2
	byte			xor   ;      // 1
	byte			add   ;      // 1
	unsigned long	tail  ;      // 4
}	ack_send_t;             // 15 bytes

typedef union send_ack__u	{
	ack_send_t		send;
	byte			bytes[sizeof(ack_send_t)];
}	ack_send_u;

static 	ack_send_u	ack;
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

static	status_u	status;
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
static	sensor_field_t	maxmin;
//[*]----------------------------------------------------------------------[*]
typedef struct send_ch__t	{
	sensor_field_t	real;
	sensor_field_t	jugi;
}	send_ch_t;
static	send_ch_t	send_ch[7];
//[*]----------------------------------------------------------------------[*]
typedef struct	var_back__t	{
	int		noise_check;
	byte	err_flag;
	float	nujuk_sum;
	float	nujuk_spc;
	float	nujuk_min;
	float	nujuk_max;
	word	nujuk_count;
}	var_back_t;
//[*]----------------------------------------------------------------------[*]
typedef struct channel__t	{
	byte			reboot_flag;
	byte			ref_ch1;
	byte			ref_ch2;
	var_back_t		var;
	byte			(*real_func)(var_back_t *p);
}	channel_t;
static	channel_t	channel[7];
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
static	byte	Board_id   = 0;
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
static	word	Pros_count     = 0 ;
static	word	Hz_check_count = 0 ;
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
					void max_min_pros(var_back_t *p);
					void temp_pros(var_back_t *p);
					void dewp_pros(var_back_t *p);
					float humi_cal(float ttemp, float ddewp);
					void humi_pros(var_back_t *p);
					void boksa_pros(var_back_t *p);
					void jugi_pros(byte s_ch);
//[*]----------------------------------------------------------------------[*]
					void send_ack(protocol_grp_t *comx, byte cmd, word d_size, byte sen_id);
					void real_send(protocol_grp_t *comx, byte ch_num);
					void jugi_send(protocol_grp_t *comx, byte ch_num);
					void init_sensor_data(byte ch_num);
					void send_maxmin_call(protocol_grp_t *comx, byte s_id);
					void move_to_serial_buffer(protocol_grp_t *comx, byte s_data);
//[*]----------------------------------------------------------------------[*]
					byte read_board_id(void);
					void com_port_init(void);
					void interrupt_init(void);
					void variable_init(void);
					void init_channel(channel_t *p, void (*ins)(var_back_t *fun), byte ch1, byte ch2);
					void init_struct(send_ch_t *p, byte s_code, byte s_id);
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
