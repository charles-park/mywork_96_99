//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
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
#define		PARITY_ODD			0x0
#define		PARITY_EVEN			0x0
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
#define		BPS300				4
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		SMOD_ON				1
#define		SMOD_OFF			0
#define		NOT_USED			0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		BUFFER_SIZE			4000
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		COM_ADDR            0x8000
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		KEY_ADDR			0x9000
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		LED_ADDR			0xA000
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		FND_ADDR0			0xB000
#define		FND_ADDR1			0xC000
#define		FND_COMM0			0xD000
#define		FND_COMM1			0xE000
#define		BLK					0xFF
#define		POINT   			0x7F
#define		MINUS   			0xDF
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		LED_RED(a)			P1.5 = 1 - a
#define		LED_BLUE(b)			P1.6 = 1 - b
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//   Display Buffer Location
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
/*
#define		WD					0
#define		WS      			0
#define		TEMP				4
#define		HUMI    			4
#define		PRESS     			8
*/
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		WD					0
#define		WS      			4
#define		TEMP				0
#define		HUMI    			4
#define		PRESS     			8
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		ON					1
#define		OFF					0
#define		NORMAL				0
#define		FAIL				1
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		enable(x)			x = ON
#define		disable(y)			y = OFF
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		watch()				P1.7 = 1 - P1.7
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		REAL_MODE			0
#define		MIN10_MODE          1
#define		HOUR_MODE           2
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		BUZZER(z)			P1.4 = z
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	TIME_TABLE[10] = {
//  '0'   '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
	0xC0, 0xCF, 0xA4, 0x86, 0x8B, 0x92, 0x90, 0xC3, 0x80, 0x82
};
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//      a
//    f g b
//    e   c
//      d p
//    1 == OFF
//        12481248
//        abcdegfp
// '0'    00000101 = A0
// '1'    10011111 = f9
// '2'    00100011 = C4
// '3'    00001011 = D0
// '4'    10011001 = 99
// '5'    01001001 = 92
// '6'    01000001 = 82
// '7'    00011101 = B8
// '8'    00000001 = 80
// '9'    00001001 = 90
// '-'    11111011 = Df
// ' '    11111111 = ff
// '.'    11111110 = 7f

// 'E'    01100001 = 86
// 'r'    11110011 = cf
// 'S'    01001001 = 92
// 't'    11100001 = 87
// 'b'    11000001 = 83
static	byte	TABLE[10] = {
	0xA0, 0xF9, 0xC4, 0xD0, 0x99, 0x92, 0x82, 0xB8, 0x80, 0x90
//	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90
};
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	buf0[15];
static	byte	buf1[15];
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	word	Common 		= 0x0001;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	word	Disp_loc 	= 0;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	Test_mode 	= OFF;
static	byte	Time_flag 	= OFF;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	Disp_mode 	= REAL_MODE;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	word	Wait_count 	= 0;
static	byte	Wait_flag 	= OFF;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	On_off 		= OFF;
static	word	On_off_count= 0x0000;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	Recv_sta 	= NORMAL;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	byte	Error_flag 	= OFF;
static	word	Error_count = 0;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct	led_disp__t	{
	byte	real	:1;
	byte	min10	:1;
	byte	hour	:1;
	byte	max		:1;
	byte	min 	:1;
	byte	time	:1;
	byte	hh_up	:1;
	byte	bb_up	:1;
}	led_disp_t;

typedef union 	led_disp__u	{
	led_disp_t		bits;
	byte			byte_data;
}	led_disp_u;

static	led_disp_u	led_disp;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct	key_data__t	{
	byte	value	:3;
	byte	not_used:4;
	byte	flag	:1;
}	key_data_t;

typedef union 	key_data__u	{
	key_data_t		bits;
	byte			byte_data;
}	key_data_u;

static	key_data_u	key_data;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct 	sensor__t	{
	word	header	;
	byte	cmd 	;
	byte	sta		;
	byte	scode	;
	byte	sid		;
	word	avr		;
	word	spc		;
	word	min		;
	word	max		;
	byte	xor		;
	word	tail	;
}	sensor_t;

typedef union   sensor__u	{
	sensor_t		send;
	byte			bytes[sizeof(sensor_t)];
}	sensor_u;

static	sensor_u	key;
static	sensor_u	recv;
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

static	protocol_grp_t	com1;
static	protocol_grp_t	com2;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//const static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
//const static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x0B] 	void T0_int 			(void);
interrupt [0x23] 	void SCON_int 			(void);
interrupt [0x2B] 	void T2_int 			(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					void init_serial_grp	(protocol_grp_t *comx, byte mode, word b_addr, word speed,
											 byte crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro	(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
											 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag	(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event		(protocol_grp_t *comx, byte com_data);
					void protocol_q			(protocol_grp_t *comx, byte protocol_number, byte com_data);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					byte rx_chk				(variable_t *var);
					byte rx_cat				(variable_t *var);
					void err_disp			(byte sta, byte id);
					void convert			(word d_data, byte id);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
					byte read_board_id		(void);
					void delay				(word count);
					void display_grp_init	(void);
					void com_port_init		(void);
					void init_interrupt		(void);
					void transmit_check		(void);
					void receive_check		(void);
					void send_struct_init	(void);
					void init_variable		(void);
					void key_check			(void);
					void send_key			(byte mode);
					void send_to_pc			(sensor_u *p);
					void data_move			(protocol_grp_t *comx, byte mov_data);
					void test_func			(void);
					void main				(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
