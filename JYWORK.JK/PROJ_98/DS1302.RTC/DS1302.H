//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		INTERNAL			0
#define		EXTERNAL			1
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		DATABIT7			0x2
#define		DATABIT8			0x3
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		STOP1				0x0
#define		STOP2				0x4
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		PARITY_NO			0x0
#define		PARITY_ODD			0x4
#define		PARITY_EVEN			0x8
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
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
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		M11					0
#define		M22					1
#define		M3					2
#define		M1					3
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		BPS9600				0
#define		BPS4800				1
#define		BPS2400				2
#define		BPS1200				3
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		SMOD_ON				1
#define		SMOD_OFF			0
#define		NOT_USED			0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		enable(a)			a = ON
#define		disable(b)      	b = OFF
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		BUFFER_SIZE			2000
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		watch()				P1.7 = 1 - P1.7
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		ON					1
#define		OFF					0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		SEC_WR				0x80
#define		SEC_RD				0x81
#define		MIN_WR				0x82
#define		MIN_RD				0x83
#define		HOUR_WR				0x84
#define		HOUR_RD				0x85
#define		DAY_WR				0x86
#define		DAY_RD				0x87
#define		MONTH_WR			0x88
#define		MONTH_RD			0x89
#define		YOIL_WR				0x8A
#define		YOIL_RD             0x8B
#define		YEAR_WR				0x8C
#define		YEAR_RD				0x8D
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		RESET(a)			P1.0 = a
#define		IN_OUT				P1.1
#define		SCLOCK				P1.2
#define		TX_LED(x)			P1.4 = x
#define		RX_LED(y)			P1.5 = y
#define		SP(c)  				P1.6 = c
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct data_type__t	{
	byte		lsb	:4;
	byte		msb	:3;
	byte		flag:1;
}	data_type_t;

typedef union data_type__u	{
	data_type_t	bits;
	byte		byte_data;
}	data_type_u;

static	data_type_u		d;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct time__t	{
	byte	ss;
	byte	bb;
	byte	hh;
	byte	dd;
	byte	mm;
	byte	yy;
	byte	yoil;
}	time_t;

static	time_t	time;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
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

static	protocol_grp_t	com;

//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
const static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
const static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x23] 	void SCON_int 		(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					void init_serial_grp	(protocol_grp_t *comx, byte mode, word b_addr, word speed,
											 byte crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro	(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
											 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag	(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event		(protocol_grp_t *comx, byte com_data);
					void protocol_q			(protocol_grp_t *comx, byte protocol_number, byte com_data);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					byte chk1			(variable_t *p);
					byte cat1			(variable_t *p);
					void delay			(word count);
					void clock			(void);
					byte read_ic		(byte addr);
					void write_ic		(byte addr, byte wr_data);
					void read_time		(time_t *t);
					void write_time		(time_t *t);
					void serial_init	(void);
					void transmit_check	(void);
					void receive_check	(void);
					void send_time		(void);
					void rtc_run		(void);
					void main			(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]

