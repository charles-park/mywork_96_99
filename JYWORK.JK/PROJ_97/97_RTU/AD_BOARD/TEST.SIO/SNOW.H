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
#define		CALL_ON				0x02
#define		CALL_OFF			0x00
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
#define		MAX_WAIT			100
#define		SENSOR_WAIT         5
#define		END_CAL 			0xFF
//[*]----------------------------------------------------------------------[*]
#define		ASCII_CR			0x0D
#define		ASCII_LF			0x0A
//[*]----------------------------------------------------------------------[*]
#define		watch()	  			P1.4 = 1 - P1.4
//[*]----------------------------------------------------------------------[*]
#define		disable(y)			y = OFF
#define		enable(z)			z = ON
//[*]----------------------------------------------------------------------[*]
#define		REAL_JUGI			0x0000
//[*]----------------------------------------------------------------------[*]
#define		JUMPER				0xE000
//[*]----------------------------------------------------------------------[*]
#define		RED  				P1.6
#define		GREEN            	P1.7
//[*]----------------------------------------------------------------------[*]
typedef struct ack_send__t	{
	unsigned long	header;	// 4
	byte			bdid  ; // 1
	byte			sen_id; // 1
	byte			cmd   ; // 1
	word			size  ; // 2
	byte			xor   ; // 1
	byte			add   ; // 1
	unsigned long	tail  ; // 4
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
	unsigned long	header   ;
	byte			call_cmd ;
	word			gubun    ;
	status_u		status   ;
	byte			sen_code ;
	byte			sen_id   ;
	float			avr,spc,min,max;
	byte			xor      ;
	byte			add      ;
	unsigned long	tail     ;
}	sensor_field_t;

typedef union	sensor_field__u	{
	sensor_field_t	snow;
	byte			bytes[sizeof(sensor_field_t)];
}	sensor_field_u;

static	sensor_field_u	real, jugi, offset, maxmin;
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

static	protocol_grp_t	com1, com2, com3;
//[*]----------------------------------------------------------------------[*]
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]----------------------------------------------------------------------[*]
static	byte	Board_id       = 0;
static	word	Pros_count     = 0;
static	word	Sec_count      = 0;
static	word	Hz_check_count = 0;
//[*]----------------------------------------------------------------------[*]
static	word	Temp_value 	   = 0;
static	float	Temp 		   = -9999.0;
//[*]----------------------------------------------------------------------[*]
static	float	Sensor_cat 	   = 0.;
//[*]----------------------------------------------------------------------[*]
static	float	Old_snow         = -9999.0;
static	byte	Sensor_err_count = 0;
static	byte	Sensor_cat_count = 0;
static	byte	Snow_flag        = 1;
static	float	Snow_aver[10];
static	float	Offset           = 0.;
static	byte	Offset_auto_flag = 0 ;
static	float	Off_back         = 0.;
static	word	Sensor_ack_count = 0 ;
static	float	Noise_check      = 0.;
static	word	Sample_delay     = 0 ;
static	byte	Reboot_flag      = 1 ;
//[*]----------------------------------------------------------------------[*]
static	byte	Real_flag = 0 ;
static	byte	Jugi_flag = 0 ;
static	float	Jugi_avr  = 0.;
static	float	Jugi_max  = 0.;
static	float	Jugi_spc  = 0.;
static	float	Jugi_min  = 0.;
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] 	void EX0_int  (void);
interrupt [0x0B] 	void T0_int   (void);
interrupt [0x23] 	void SCON_int (void);
					void snow_pros(void);
					void jugi_pros(void);
					void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
										  byte crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
										 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event(protocol_grp_t *comx, byte com_data);
					void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);
					void send_ack(protocol_grp_t *comx, byte cmd, word d_size, byte sen_id);
					void send_real(protocol_grp_t *comx, word jugi, sensor_field_u *p);
					void send_jugi(protocol_grp_t *comx, word jugi, sensor_field_u *p);
					void send_offset(protocol_grp_t *comx, float offs, sensor_field_u *p);
					void send_maxmin_call(protocol_grp_t *comx, sensor_field_u *p);
					void move_to_serial_buffer(protocol_grp_t *comx, byte s_data);
					byte rx_chk       (variable_t *var);
					byte rx_cat1      (variable_t *var);
					byte rx_cat2      (variable_t *var);
					byte snow_chk     (variable_t *var);
					byte snow_cat1    (variable_t *var);
					byte led_chk_30   (variable_t *var);
					byte led_cat_30   (variable_t *var);
					byte led_chk_daeju(variable_t *var);
					byte led_cat_daeju(variable_t *var);
					byte read_board_id(void);
					void com_port_init(byte led_mode);
					void init_struct  (sensor_field_u *p, byte id, byte s_code, byte cmd, word jugi);
					void interrupt_init(void);
					void variable_init (void);
					void init_sensor_data(void);
					void init_maxmin_data(float max, float min);
					void transmit_check(void);
					void receive_check (void);
					void receive_check_23(void);
					void main(void);
//[*]----------------------------------------------------------------------[*]
//   Function Define
//[*]----------------------------------------------------------------------[*]
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
