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
#define		RTU_STATUS_CALL  	's'
#define		RTU_VOLT_CALL    	'd'
#define		RTU_WATCH_CALL   	'Q'
#define		RTU_RESET_CALL   	'R'
#define		RTU_ID_SET			'I'
#define		RTU_PASS_SET		'P'
#define		RTU_TIME_DATE_SET	'T'
#define		RTU_BAUD_SET		'C'
#define		RTU_BUFFER_CLR		'B'
#define		CONFIG_SET			'N'
//[*]----------------------------------------------------------------------[*]
#define		MPM_ID				0x00
#define		RCM_ID				0x01
#define		ACK_MAX_WAIT    	100
#define		EVENT_GETSU			100
//[*]----------------------------------------------------------------------[*]
#define		M12_DATA			0x00
#define		P12_DATA 			0x01
#define		P5_DATA				0x02
//[*]----------------------------------------------------------------------[*]
//   16 bit prescaler timer mode
//   interrupt is  1 sec of 14 appear
//[*]----------------------------------------------------------------------[*]
#define		VOLT_WAIT				15		// 1 sec wait
#define		MPM_WAIT				4200	// 5 min wait
#define		EVENT_SAVE_WAIT_TIME    3
//[*]----------------------------------------------------------------------[*]
#define		POWER_RESET(p)		P1.5 = 1 - p
#define     MPM_RESET(r)        P1.6 = 1 - r
//#define     MPM_RESET(r)        P1.6 = r
#define		watch()             P1.7 = 1 - P1.7
#define		AD_ADD				0xA000
#define		EXT_SIGNAL_ADD		0xB000
//[*]----------------------------------------------------------------------[*]
#define		disable(y)			y = OFF
#define		enable(z)			z = ON
//[*]----------------------------------------------------------------------[*]
typedef struct time__t	{
	byte	yy;
	byte	mm;
	byte	dd;
	byte	hh;
	byte	bb;
	byte	ss;
}	time_t;
//[*]----------------------------------------------------------------------[*]
typedef struct	rtu_bit__t	{
	word	set_num			:4;		// lsb
	word	set_cmd			:4;
	word	reset			:1;
	word	id_set			:1;
	word	pass_set		:1;
	word	time_date_set	:1;
	word	buffer_clr		:1;
	word	baud_set		:1;
	word	config_set		:1;
	word	not_used1		:1;		// msb
}	rtu_bit_t;

typedef	union	rtu_bit__u	{
	rtu_bit_t		bits     ;
	word			word_data;
}	rtu_bit_u;

typedef struct status__t	{
	byte		header [4];		// 4
	time_t		s_time    ;		// 6
	byte		rtuid     ;		// 1
	word		jijum     ;		// 2
	byte		bdid      ;		// 1
	byte		cmd    [2];     // 2
	byte		p12       ;     // 1
	byte		p5        ;     // 1
	byte		m12       ;     // 1
	byte		bd_sta [8];     // 8
	byte		sen_sta[8];     // 8
	rtu_bit_u	cnt_bd    ;     // 2
	byte		xor       ;     // 1
	byte		add       ;     // 1
	byte		tail   [4];     // 4
}	status_t;                   // 43 bytes

typedef	union	status__u	{
	status_t		send;
	byte			bytes[sizeof(status_t)];
}	status_u;

static	status_u	status;

typedef union	event__u	{
	status_t		num[EVENT_GETSU];
	byte			bytes[sizeof(status_t) * EVENT_GETSU];
}	event_u;

static	event_u		event;
//[*]----------------------------------------------------------------------[*]
typedef struct	ext_signal_data__t	{
	byte	sig0	:1;
	byte	sig1	:1;
	byte	sig2	:1;
	byte	sig3	:1;
	byte	sig4	:1;
	byte	sig5	:1;
	byte	sig6	:1;
	byte	sig7	:1;
}	ext_signal_data_t;

typedef union	ext_signal_data__u	{
	ext_signal_data_t		bits;
	byte					byte_data;
}   ext_signal_data_u;

static 	ext_signal_data_u	ext_signal_data;
//[*]----------------------------------------------------------------------[*]
typedef struct event_pros__t	{
	byte		hear   ;
	byte		trace  ;
	byte		forward;
}	event_pros_t;

static	event_pros_t	ev_point;
//[*]----------------------------------------------------------------------[*]
typedef struct 	volts_send__t	{
	byte		header[4] ;       // 4
	time_t		s_time    ;       // 6
	byte		rtuid     ;       // 1
	word		jijum     ;       // 2
	byte		bdid      ;       // 1
	byte		cmd[2]    ;       // 2
	float		p12       ;       // 4
	float		p5        ;       // 4
	float		m12       ;       // 4
	byte		xor       ;       // 1
	byte		add       ;       // 1
	byte		tail   [4];       // 4
}	volts_send_t;                 // 34 bytes

typedef union	volts_send__u	{
	volts_send_t	send;
	byte			bytes[sizeof(volts_send_t)];
}	volts_send_u;

static volts_send_u		volts;
//[*]----------------------------------------------------------------------[*]
typedef	struct	volts_data__t	{
	float		p12        ;
	float		p5         ;
	float		m12        ;
	byte		p12_sta    ;
	byte		p5_sta     ;
	byte		m12_sta    ;
	byte		flag       ;
	byte		ch         ;
	word		count      ;
	word		event_count;
}	volts_data_t;

static	volts_data_t	volts_data;
//[*]----------------------------------------------------------------------[*]
typedef struct 	mpm_send__t	{
	byte		header[4] ;       // 4
	time_t		s_time    ;       // 6
	byte		rtuid     ;       // 1
	word		jijum     ;       // 2
	byte		bdid      ;       // 1
	byte		cmd[2]    ;       // 2
	float		p12       ;       // 4
	float		p5        ;       // 4
	float		m12       ;       // 4
	byte		bd_sta [8];       // 8
	byte		sen_sta[8];       // 8
	rtu_bit_u	cnt_bd    ;       // 2
	byte		xor       ;       // 1
	byte		add       ;       // 1
	byte		tail   [4];       // 4
}	mpm_send_t;                   // 52 bytes

typedef union	mpm_send__u	{
	mpm_send_t		send;
	byte			bytes[sizeof(mpm_send_t)];
}	mpm_send_u;

static	mpm_send_u		mpm;
//[*]----------------------------------------------------------------------[*]
typedef struct ext_ack_send__t	{
	byte	header[4];		// 4
	time_t	s_time   ;      // 6
	byte	rtuid    ;      // 1
	word    jijum    ;      // 2
	byte	bdid     ;      // 1
	byte	cmd[2]   ;      // 2
	word	size     ;      // 2
	byte	xor      ;      // 1
	byte	add      ;      // 1
	byte	tail  [4];      // 4
}	ext_ack_send_t;         // 24 bytes

typedef union send_ack__u	{
	ext_ack_send_t		send;
	byte				bytes[sizeof(ext_ack_send_t)];
}	ext_ack_send_u;

static 	ext_ack_send_u	ext_ack;
//[*]----------------------------------------------------------------------[*]
typedef struct int_ack_send__t	{
	byte	header[4];		// 4
	byte	bdid     ;      // 1
	byte	sen_id   ;		// 1
	byte	cmd      ;      // 1
	word	size     ;      // 2
	byte	xor      ;      // 1
	byte	add      ;      // 1
	byte	tail  [4];      // 4
}	int_ack_send_t;         // 15 bytes

typedef union int_ack_send__u	{
	int_ack_send_t		send;
	byte				bytes[sizeof(int_ack_send_t)];
}	int_ack_send_u;

static 	int_ack_send_u		int_ack;
//[*]----------------------------------------------------------------------[*]
//   com3 port debug mode used.
//[*]----------------------------------------------------------------------[*]
typedef struct	debug__t	{
	byte	header[4] ;
	byte	f_data[40];
	byte	xor       ;
	byte	add       ;
	byte	tail  [4] ;
}	debug_t;

typedef union	debug__u	{
	debug_t		send;
	byte		bytes[sizeof(debug_t)];
}	debug_u;

static	debug_u		debug;
static 	byte		Debug_bit = OFF;
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

static	protocol_grp_t	com1, com2, com3;
//[*]----------------------------------------------------------------------[*]
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]----------------------------------------------------------------------[*]
static	word	Ack_check_count  = 0;
static	word	Mpm_check_count  = 0;
static	word	Jijum            = 0;
static	byte	Rtuid            = 0;
static	byte	Event_check_flag = OFF;
static	byte	Event_save_flag  = OFF;
static	byte	Event_send_flag  = OFF;
static	byte	Event_run_flag   = OFF;
static	byte	Receive_count    = 0;
static	byte	Baud_set_flag    = OFF;
static	word	Baud_set_value   = 0;
static	byte	Baud_set_port    = 0;
//[*]----------------------------------------------------------------------[*]
//   Function Define
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] 	void EX0_int  (void);
interrupt [0x0B] 	void T0_int   (void);
interrupt [0x13] 	void EX1_int  (void);
interrupt [0x23] 	void SCON_int (void);
					byte rx_chk  (variable_t *var);
					byte rx1_cat1(variable_t *var);
					byte rx1_cat2(variable_t *var);
					byte rx2_cat1(variable_t *var);
					byte rx2_cat2(variable_t *var);
					byte rx3_cat1(variable_t *var);
					byte rx3_cat2(variable_t *var);
					byte rx3_debug_chk(variable_t *var);
					byte rx3_debug_cat(variable_t *var);
					void int_send_ack(protocol_grp_t *comx, byte cmd, word d_size);
					void ext_send_ack(protocol_grp_t *comx, byte cmd, word d_size, time_t *s_time);
					void send_volts(protocol_grp_t *comx, byte cmd, time_t *s_time);
					void send_mpm(protocol_grp_t *comx, byte cmd, time_t *s_time);
					void send_status(protocol_grp_t *comx, byte cmd);
					void send_event(protocol_grp_t *comx, byte event_num);
					void move_to_serial_buffer(protocol_grp_t *comx, byte s_data);
					void debug_data_send(protocol_grp_t *comx, variable_t *var);
					void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
										  byte crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
										 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event(protocol_grp_t *comx, byte com_data);
					void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);
					void init_baud(protocol_grp_t *comx, word speed);
					void baud_set(void);
					void ext_signal_set(byte set_cmd, byte set_num);
					void event_q(event_pros_t *ev_pros);
					void event_check   (void);
					void event_save    (void);
					void com_port_init (void);
					void interrupt_init(void);
					void variable_init (void);
					void mpm_reset     (void);
					void rtu_reset     (void);
					void transmit_check(void);
					void receive_check (void);
					byte read_rtuid    (void);
					void main          (void);
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
