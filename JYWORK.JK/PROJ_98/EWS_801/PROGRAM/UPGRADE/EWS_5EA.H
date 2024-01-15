//[*]----------------------------------------------------------------------[*]
#define		INTERNAL		0
//[*]----------------------------------------------------------------------[*]
#define		BUFFER_TX		1000
#define		BUFFER_RX		100
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
#define		RUN				1
#define		STOP			0
#define		END_CAL			0x100
//[*]----------------------------------------------------------------------[*]
#define		PRE_ADDR		0x8000
#define		LED_L    		0xA000
#define		LED_H			0xD000
#define		ADC_MSB			0xC000
#define		ADC_LSB			0xC001
#define		JUMPER			0xB000
#define		PRE_CLEAR		0xE000
//[*]----------------------------------------------------------------------[*]
#define		RTCC			0x9000
#define		RTCC_SEC		RTCC +  0
#define		RTCC_SEC_ALRM   RTCC +  1
#define		RTCC_MIN        RTCC +  2
#define		RTCC_MIN_ALRM   RTCC +  3
#define		RTCC_HOUR       RTCC +  4
#define		RTCC_HOUR_ALRM  RTCC +  5
#define		RTCC_DAY_WEEK   RTCC +  6
#define		RTCC_DAY        RTCC +  7
#define		RTCC_MONTH      RTCC +  8
#define		RTCC_YEAR       RTCC +  9
#define		RTCC_REG_A      RTCC + 10
#define		RTCC_REG_B      RTCC + 11
#define		RTCC_REG_C      RTCC + 12
#define		RTCC_REG_D      RTCC + 13
#define		RTCC_SQW_DATA   0x0E
#define		RTCC_CFG_DATA	0x1E
//[*]----------------------------------------------------------------------[*]
#define		SAMPLE			1
#define		HOLD			0
#define		MUX_STATUS		P1.0
#define		ADC_STATUS		P1.2
#define		watch()			P1.3 = 1 - P1.3
#define		BUZZER(x)		P1.4 = x
#define		WD_SEL()		{ P1.5 = 1;	P1.6 = 0; P1.7 = 0;}
//[*]----------------------------------------------------------------------[*]
#define		LED_DATE		0
#define		LED_TIME        1
#define		WD_LED          2
#define		WS_LED          3
//[*]----------------------------------------------------------------------[*]
#define		REAL			0
//[*]----------------------------------------------------------------------[*]
#define     WD_MAXMIN   	0
#define     WS_MAXMIN   	4
#define		CHECK_DATE		8
#define     CHECK_RAM1      9
#define     CHECK_RAM2      10
#define     CHECK_RAM3      11
//[*]----------------------------------------------------------------------[*]
//  Fixed Save Ram Address ( Address 0x1F00 ~)
//[*]----------------------------------------------------------------------[*]
#define 	save(addr,data)  	((*(char *)(0x011F00+addr)) = data)
#define 	load(addr)       	 (*(char *)(0x011F00+addr))
//[*]----------------------------------------------------------------------[*]
#define		MAX_LOAD 		0
#define		MIN_LOAD 		1
//[*]----------------------------------------------------------------------[*]
#define		DATE_SET		0
#define		TIME_SET		1
#define		ALL_SET			2
#define		NORMAL			0
#define		FAIL			1
//[*]----------------------------------------------------------------------[*]
typedef struct led16__t	{
	word	L0  :1;			// LSB
	word	L1  :1;
	word	L2  :1;
	word	L3  :1;
	word	L4  :1;
	word	L5  :1;
	word	L6  :1;
	word	L7  :1;
	word    L8  :1;
	word	L9  :1;
	word	L10 :1;
	word	L11 :1;
	word	L12 :1;
	word	L13 :1;
	word	L14 :1;
	word    L15 :1;         // MSB      ; Hardware Error!! change data bus.
}	led16_t;

typedef union led16__u	{
	led16_t		bits;
	byte		ddata[2];
}	led16_u;

static	led16_u		led16;
//[*]----------------------------------------------------------------------[*]
#define		led16_clr()		{	led16.ddata[0] = 0x00; 	led16.ddata[1] = 0x00;	}
#define		led16_disp()	{   output(LED_L, ~(led16.ddata[0]));			\
								output(LED_H, ~(led16.ddata[1]));      		\
							}
//[*]----------------------------------------------------------------------[*]
typedef struct init_flag__t	{
	byte		wd     :1;
	byte		ws     :1;
	byte        temp   :1;
	byte		humi   :1;
	byte		pre    :1;
	byte		ypre   :1;
	byte		bul    :1;
	byte		chekam :1;
}	init_flag_t;

typedef union init_flag__u	{
	init_flag_t		flag;
	byte			byte_data;
}	init_flag_u;

static	init_flag_u		init;
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
typedef struct time__t	{
	byte	yy;
	byte	mm;
	byte	dd;
	byte	hh;
	byte	bb;
	byte	ss;
}	time_t;
static	time_t	t;
static	time_t	old;
//[*]----------------------------------------------------------------------[*]
typedef struct function_call__t		{
	void		(*wd_cal    )(byte mode);
	void		(*ws_cal    )(byte mode);
}	function_call_t;
static	function_call_t		select;
//[*]----------------------------------------------------------------------[*]
typedef struct wd_pros__t	{
	word	real_max ;
	word	real_avr ;
	word	real_sum ;
	double	real_sin ;
	double	real_cos ;
}	wd_pros_t;

static	wd_pros_t	wd_pros;
//[*]----------------------------------------------------------------------[*]
typedef struct pros__t	{
	word    real_spc ;
	word    real_avr ;
	word	real_min ;
	word	real_max ;
	word	real_sum ;
}	pros_t;

static pros_t	ws_pros;
//[*]----------------------------------------------------------------------[*]
typedef struct sensor__t	{
	word	header;
	byte	cmd   ;
	byte	sta   ;
	byte	scod  ;
	byte	sid   ;
	word	avr   ;
	word	spc   ;
	word	min   ;
	word	max   ;
	byte	xor   ;
	word	tail  ;
}	sensor_t;					// total = 17 byte

typedef union sensor__u	{
	sensor_t			sam;
	byte				bytes[sizeof(sensor_t)];
}	sensor_u;

static	sensor_u	led[2+2];
//[*]----------------------------------------------------------------------[*]
typedef struct key_send__t	{
	unsigned long	header;		// 4 bytes
	unsigned char 	cmd;		// 1 byte
	unsigned char	key;        // 1 byte
	unsigned char 	xor;        // 1 byte
	unsigned long	tail;       // 4 bytes
}	key_send_t;

typedef union  key_send__u	{
	key_send_t		send;
	unsigned char 	bytes[sizeof(key_send_t)];
}	key_send_u;

static key_send_u	key_send;
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
	byte	tx_buf[BUFFER_TX];
	byte	rx_buf[BUFFER_RX];
	byte	tx_flag ;
	byte	cat_flag;
	byte	pcnt;
	byte	type;
	word	base;
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	com;
//[*]----------------------------------------------------------------------[*]
static	byte	Real_flag  = OFF;
static	byte	Real_count = 0  ;
static	byte	Init_flag  = ON ;
static	byte 	Day_check  = 0  ;
//[*]----------------------------------------------------------------------[*]
static	byte	Read_id    = 0  ;
//[*]----------------------------------------------------------------------[*]
static  byte	Led_send   = OFF;
//[*]----------------------------------------------------------------------[*]
const static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
const static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]----------------------------------------------------------------------[*]
const static  float    A_wd_range[16] = {
  22.5, 45. ,  67.5, 90., 112.5, 135., 157.5, 180., 202.5, 225., 247.5, 270.,
  292.5, 315., 337.5, 360.};
const static  word    A_wd_direction[17] = {
 254, 509, 763,1018,1272,1526,1781,2035,2289,2544,2798,3053,3307,3561,3816,4070,4095};
// 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
const static	word    A_ws_range[16] = {
	 3,   5,  10,  15,  20,  25,  30,  35,  40,  45,  50,  55,  60,  65,  70,  75};
const static  word	A_ws_speed_38[16] = {
	 38, 72, 152, 227, 303, 382, 460, 544, 627, 706, 785, 864, 944,1022,1101,1181};
const static  word	A_ws_speed_45[16] = {
	 45, 80, 160, 248, 333, 420, 507, 593, 680, 766, 853, 940,1027,1113,1200,1287};

//[*]----------------------------------------------------------------------[*]
//   FUNCTION DEFINE LIST
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] 	void T0_int            (void);
interrupt [0x23] 	void SCON_int          (void);
interrupt [0x2B] 	void T2_int            (void);
//[*]----------------------------------------------------------------------[*]
					void init_serial_grp   (protocol_grp_t *comx, byte mode, word b_addr, word speed,
											byte crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro   (protocol_grp_t *comx, byte protocol_number, byte buffer_size,
											byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag (protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event    (protocol_grp_t *comx, byte com_data);
					void protocol_q        (protocol_grp_t *comx, byte protocol_number, byte com_data);
//[*]----------------------------------------------------------------------[*]
					byte rx_check          (variable_t *var);
					byte rx_catch          (variable_t *var);
					void ok_send           (byte key);
//[*]----------------------------------------------------------------------[*]
					void maxmin_save       (byte addr, word max, word min);
					word maxmin_load       (byte addr, byte load_data);
					word adc_read          (void);
					void led16_pros		   (word value);
					void wind_direction_cal(byte cal_mode);
					void wind_speed_ms_cal (byte cal_mode);
					void temp_cal          (byte cal_mode);
					void humi_cal          (byte cal_mode);
					void pre_cal           (byte cal_mode);
					void ypre_cal          (byte cal_mode);
				  double dewpoint          (double Temperature, double Humidity);
					float bul_calculater   (double temp, double humi);
					void bul_cal           (byte cal_mode);
				   float chekamtemp_cal		(float windspd,float temp);
					void chekam_cal        (byte cal_mode);
//[*]----------------------------------------------------------------------[*]
					void rtcc_init         (void);
					byte rtcc_date_time_set(time_t *s_time, byte mode);
					void rtcc_read         (void);
					void save_ram_init     (void);
					void save_ram_load     (void);
					void save_data_check   (void);
					void cal_means_read    (void);
					void struct_initiliaze (sensor_u *sen, byte b_cmd, byte b_scod, byte b_sid);
					void variable_init     (void);
					void pros_struct_init1 (void);
					void pros_struct_init2 (pros_t *p);
					void receive_check     (void);
					void transmit_check    (void);
					void delay             (word count);
					void sensor_init       (void);
					void data_pros         (byte mode, byte sen_num);
					void led_pros          (void);
					void led_data_send     (void);
					void data_move         (byte mov_data);
//[*]----------------------------------------------------------------------[*]
					void main              (void);
//[*]----------------------------------------------------------------------[*]


