//[*]----------------------------------------------------------------------[*]
#define		INTERNAL		0
#define		EXTERNAL		1
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
#define		RUN				1
#define		STOP			0
#define		END_CAL			0x100
//[*]----------------------------------------------------------------------[*]
#define		ADC_MSB			0x9000
#define		ADC_LSB			0x9001
#define		JUMPER			0xA000
#define		PRE_ADDR		0xE000
#define		PRE_CLEAR	    P1.4
//[*]----------------------------------------------------------------------[*]
#define		RTCC			0x8000
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
#define     RTCC_SQW_DATA   0x0E
#define		RTCC_CFG_DATA	0x1E
//[*]----------------------------------------------------------------------[*]
#define		RX_LED			P1.0
#define		TX_LED			P1.1
//[*]----------------------------------------------------------------------[*]
#define		ADC_STATUS		P1.2
#define		watch()			P1.3 = 1 - P1.3
#define		WD_SEL()		{ P1.5 = 1;	P1.6 = 0; P1.7 = 0;}
#define		TEMP_SEL()		{ P1.5 = 0;	P1.6 = 0; P1.7 = 0;}
#define		HUMI_SEL()		{ P1.5 = 0;	P1.6 = 1; P1.7 = 0;}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
#define		DATA_GUBUN		0
#define		PROC_DATE		1
#define		PROC_TIME		2
#define		WIND_DIR		3
#define		WIND_SPD		4
#define		TEMPER			5
#define		HUMIDT			6
#define		PRECIPI			7
#define		YST_PRE			8
#define		BUL_JISU		9
#define		CHEKAM			10
//[*]----------------------------------------------------------------------[*]
//  id in Sensor group
//[*]----------------------------------------------------------------------[*]
#define		WD				0
#define		WS              1
#define		TEMP            2
#define		HUMI            3
#define		PRE             4
#define		YPRE            5
#define		BUL             6
#define		CHE             7
//[*]----------------------------------------------------------------------[*]
#define		SENSOR_SU		8
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
#define		REAL			0
#define		MIN10			1
#define		HOUR			2
//[*]----------------------------------------------------------------------[*]
#define     WD_MAXMIN   	0
#define     WS_MAXMIN   	4
#define     TEMP_MAXMIN 	8
#define     HUMI_MAXMIN 	12
#define     PRE_MAXMIN  	16
#define     YPRE_MAXMIN  	20
#define     BUL_MAXMIN      24
#define		CHEKAM_MAXMIN	28
#define		CHECK_DATE		32
#define     CHECK_RAM1      33
#define     CHECK_RAM2      34
#define     CHECK_RAM3      35
//[*]----------------------------------------------------------------------[*]
//  Fixed Save Ram Address ( Address 0x7F00 ~)
//[*]----------------------------------------------------------------------[*]
#define 	save(addr,data)  	((*(char *)(0x017F00+addr)) = data)
#define 	load(addr)       	 (*(char *)(0x017F00+addr))
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
#define		REAL_MODE		1
#define		MAX_MODE		2
#define		MIN_MODE		3
#define		MIN10_MODE		4
#define		HOUR_MODE		5
#define     TIME_MODE       6
//[*]----------------------------------------------------------------------[*]
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
//[*]----------------------------------------------------------------------[*]
typedef struct function_call__t		{
	void		(*proc_func)(byte mode);
}	function_call_t;
static	function_call_t		select[SENSOR_SU];
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
typedef struct sensor__t	{
	word		header;		// 2 Byte
	byte		cmd   ;     // 1 Byte
	byte		sta   ;     // 1 Byte
	byte		scod  ;     // 1 Byte
	byte		sid   ;     // 1 Byte
	word		avr   ;     // 2 Byte
	word		spc   ;     // 2 Byte
	word		min   ;     // 2 Byte
	word		max   ;     // 2 Byte
	byte		xor   ;     // 1 Byte
	word		tail  ;     // 2 Byte	total = 17 Bytes
}	sensor_t;

typedef union sensor__u	{
	sensor_t	sensor[SENSOR_SU + 3];
	byte		bytes[sizeof(sensor_t) * (SENSOR_SU + 3)];
}	sensor_u;

// +3 is DATA GUBUN, DATE, TIME Field

static	sensor_u	send_grp;
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
typedef struct	save_grp__t	{
	sensor_t	real [SENSOR_SU];
	sensor_t	min10[SENSOR_SU];
	sensor_t	hour [SENSOR_SU];
}	save_grp_t;

static	save_grp_t	save_grp;
//[*]----------------------------------------------------------------------[*]
typedef struct	data__t	{
	word		avr	 		;
	word		min	 		;
	word		max	 		;
	word		value		;
	double		sin  		;
	double		cos  		;
	double		sum  		;
	word		proc_count  ;
}	data_t;

typedef struct	proc_grp__t	{
	data_t		real [SENSOR_SU];
	data_t		min10[SENSOR_SU];
	data_t		hour [SENSOR_SU];
}	proc_grp_t;

static	proc_grp_t	proc_grp;
//[*]----------------------------------------------------------------------[*]
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
	byte	tx_buf[BUFFER_SIZE];
	byte	rx_buf[BUFFER_SIZE];
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
static 	byte	Min10_flag = OFF;
static	byte	Min10_count= 0  ;
static	byte	Hour_flag  = OFF;
static	byte	Hour_count = 0  ;
static	byte	Init_flag  = ON ;
//[*]----------------------------------------------------------------------[*]
static	byte    Hour_check = 0  ;
//[*]----------------------------------------------------------------------[*]
static	byte	Read_id    = 0  ;
//[*]----------------------------------------------------------------------[*]
static	byte	Speed 	   = 0  ;	// 0 then 45Hz, other 38Hz
//[*]----------------------------------------------------------------------[*]
static  byte	Send_flag  = OFF;
static  byte	Send_mode  = REAL_MODE;
//[*]----------------------------------------------------------------------[*]
static	word	Bul_temp   = 0  ;
static	word	Bul_humi   = 0  ;
//[*]----------------------------------------------------------------------[*]
static	float	Chekam_ws   = 0.;
static	float	Chekam_temp = 0.;
//[*]----------------------------------------------------------------------[*]
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]----------------------------------------------------------------------[*]
static  float    A_wd_range[16] = {
  22.5, 45. ,  67.5, 90., 112.5, 135., 157.5, 180., 202.5, 225., 247.5, 270.,
  292.5, 315., 337.5, 360.};
static  word    A_wd_direction[17] = {
 254, 509, 763,1018,1272,1526,1781,2035,2289,2544,2798,3053,3307,3561,3816,4070,4095};
// 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
static	word    A_ws_range[16] = {
	 3,   5,  10,  15,  20,  25,  30,  35,  40,  45,  50,  55,  60,  65,  70,  75};
static  word	A_ws_speed_38[16] = {
	 38, 72, 152, 227, 303, 382, 460, 544, 627, 706, 785, 864, 944,1022,1101,1181};
static  word	A_ws_speed_45[16] = {
	 45, 80, 160, 248, 333, 420, 507, 593, 680, 766, 853, 940,1027,1113,1200,1287};

//[*]----------------------------------------------------------------------[*]
//   FUNCTION DEFINE LIST
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] 	void EX0_int			(void);
interrupt [0x0B] 	void T0_int            	(void);
interrupt [0x23] 	void SCON_int 			(void);
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
//[*]----------------------------------------------------------------------[*]
					void maxmin_save		(byte addr, word max, word min);
					word maxmin_load		(byte addr, byte load_data);
					word adc_read			(void);
					void wind_dir_func		(byte cal_mode);
					void wind_spd_func		(byte cal_mode);
					void temper_func		(byte cal_mode);
					void humidt_func		(byte cal_mode);
					void precipi_func		(byte cal_mode);
					void yst_pre_func		(byte cal_mode);
				  double dewpoint			(double Temperature, double Humidity);
				   float bul_calculater		(double temp, double humi);
					void bul_jisu_func		(byte cal_mode);
				   float chekamtemp_cal		(float windspd,float temp);
					void chekam_func		(byte cal_mode);
//[*]----------------------------------------------------------------------[*]
					void rtcc_init			(void);
					byte rtcc_date_time_set	(time_t *s_time, byte mode);
					void rtcc_read			(void);
					void func_init			(void);
					void struct_init		(void);
					void sensor_struct_init	(sensor_t *p, byte id);
					void proc_struct_init	(data_t *p);
					void save_data_check	(void);
					void save_ram_init		(void);
					void save_ram_load		(void);
					void variable_init		(void);
					void delay				(word count);
					void sensor_init		(void);
					void send_proc			(void);
					void send_data			(void);
					void data_move			(byte mov_data);
					void receive_check		(void);
					void transmit_check		(void);
//[*]----------------------------------------------------------------------[*]
					void main              	(void);
//[*]----------------------------------------------------------------------[*]


