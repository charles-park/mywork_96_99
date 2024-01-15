//[*]----------------------------------------------------------------------[*]
//   LED Dot Driver Header File
//[*]----------------------------------------------------------------------[*]
#ifndef	__LED__
#define	__LED__
#endif

#ifndef	H_FONT
	#define	H_FONT	0
#endif

#if   (H_FONT == 1)
	#include "MJ_FONT.HPP"
	const byte *Font_Addr1 = HANFONT1;
	const byte *Font_Addr2 = HANFONT2;
	const byte *Font_Addr3 = HANFONT3;
#elif (H_FONT == 2)
	#include "HANPIL.HPP"
	const byte *Font_Addr1 = HAN_PIL_1;
	const byte *Font_Addr2 = HAN_PIL_2;
	const byte *Font_Addr3 = HAN_PIL_3;
#elif (H_FONT == 3)
	#include "HANBOOT.HPP"
	const byte *Font_Addr1 = HAN_BOOT_1;
	const byte *Font_Addr2 = HAN_BOOT_2;
	const byte *Font_Addr3 = HAN_BOOT_3;
#elif (H_FONT == 4)
	#include "HANCHG2.HPP"
	const byte *Font_Addr1 = HANCHG_1;
	const byte *Font_Addr2 = HANCHG_2;
	const byte *Font_Addr3 = HANCHG_3;
#elif (H_FONT == 5)
	#include "HANCHG4.HPP"
	const byte *Font_Addr1 = HANCHG4_1;
	const byte *Font_Addr2 = HANCHG4_2;
	const byte *Font_Addr3 = HANCHG4_3;
#elif (H_FONT == 6)
	#include "HANSON.HPP"
	const byte *Font_Addr1 = HANSON_1;
	const byte *Font_Addr2 = HANSON_2;
	const byte *Font_Addr3 = HANSON_3;
#else
	#include "HANSOFT.HPP"
	const byte *Font_Addr1 = HANSOFT_1;
	const byte *Font_Addr2 = HANSOFT_2;
	const byte *Font_Addr3 = HANSOFT_3;
#endif

#ifndef	A_FONT
	#define	A_FONT 	1
#endif

#if  (A_FONT == 1)
	#include "ASCII.HPP"
	const byte (*Eng_Font)[16] = ENGFONT;
#endif
//[*]----------------------------------------------------------------------[*]
#define		INTERNAL			0
#define		EXTERNAL			1
//[*]----------------------------------------------------------------------[*]
#define		DATABIT7			0x02
#define		DATABIT8			0x03
//[*]----------------------------------------------------------------------[*]
#define		STOP1				0x00
#define		STOP2				0x04
//[*]----------------------------------------------------------------------[*]
#define		PARITY_NO			0x00
#define		PARITY_ODD			0x08
#define		PARITY_EVEN			0x18
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
#define		M11					11059200L
#define		M22					22118400L
#define		M3					3072000L
#define		M1					1843200L
//[*]----------------------------------------------------------------------[*]
#define		BPS9600				0
#define		BPS4800				1
#define		BPS2400				2
#define		BPS1200				3
#define		BPS600				4
//[*]----------------------------------------------------------------------[*]
#define		SMOD_ON				1
#define		SMOD_OFF			0
#define		NOT_USED			0
//[*]----------------------------------------------------------------------[*]
#define		BUFFER_SIZE			1000
//[*]----------------------------------------------------------------------[*]
#define		ON					1
#define		OFF					0
#define		NORMAL				0
#define		FAIL				1
//[*]----------------------------------------------------------------------[*]
#define		SEN_NORMAL          0xFD
#define		SEN_FAIL			0x02
#define		CLK_NORMAL			0xFE
#define		CLK_FAIL			0x01
#define		ADC_NORMAL			0xFB
#define		ADC_FAIL			0x04
//[*]----------------------------------------------------------------------[*]
#define		watch()				P1.7 = 1 - P1.7
//[*]----------------------------------------------------------------------[*]
#define		disable(y)			y = OFF
#define		enable(z)			z = ON
//[*]----------------------------------------------------------------------[*]
#define		COMMON				0x8002
#define		HI_DATA				0x8001
#define		LO_DATA				0x8000
//[*]----------------------------------------------------------------------[*]
#define		JUMPER				0x9000
//[*]----------------------------------------------------------------------[*]
//   SERIAL EEPROM P1 Port Define
//[*]----------------------------------------------------------------------[*]
#define		CHIP_CS				P1.0
#define		CLK_IN				P1.1
#define		DATA_IN				P1.2
#define		DATA_OUT			P1.3
//[*]----------------------------------------------------------------------[*]
//   OP CODE & OP CODE ADDRESS Define
//[*]----------------------------------------------------------------------[*]
#define		WRITE_OP			0x1
#define		READ_OP				0x2
#define		ERASE_OP			0x3
#define		EWEN_OP				0x0
#define		EWEN_ADDR			0x60
#define		EWDS_OP				0x0
#define		EWDS_ADDR			0x0
#define		ERAL_OP				0x0
#define		ERAL_ADDR			0x40
#define		WRAL_OP				0x0
#define		WRAL_ADDR			0x20
//[*]----------------------------------------------------------------------[*]
//   SERIAL EEPROM Command Define
//[*]----------------------------------------------------------------------[*]
#define		WRITE_CMD    		0
#define		READ_CMD        	1
#define		ERASE_CMD       	2
#define		ENABLE_CMD      	3
#define		DISABLE_CMD     	4
#define		ERASE_ALL_CMD   	5
#define		WRITE_ALL_CMD   	6
//[*]----------------------------------------------------------------------[*]
typedef struct	chk_cfg__t	{
	word		header	;
	byte		cod 	;
	byte		cmd		;
	byte		id 		;
	byte		lid		;
	byte		climt	;
	byte		dp		;
	byte		did		;
	byte		th		;
	byte		tl		;
	byte		baud	;
	byte		ds		;
	byte		nused	;
	byte		xor		;
	word		tail	;
}	chk_cfg_t;

typedef union	chk_cfg__u	{
	chk_cfg_t	rom;
	byte		bytes[sizeof(chk_cfg_t)];
}	chk_cfg_u;

static	chk_cfg_u	chk_cfg;
//[*]----------------------------------------------------------------------[*]
typedef struct	comm__t	{
	byte	cnt	 :4;
	byte	cs	 :3;
	byte	nused:1;
}	comm_t;
typedef union  	comm__u	{
	comm_t	bits;
	byte	byte_data;
}	comm_u;

static	comm_u	comm;
//[*]----------------------------------------------------------------------[*]
typedef	struct hcode__t	{
	unsigned int	lasc	:	5;
	unsigned int	midc	:	5;
	unsigned int	firc	:	5;
	unsigned int	dummy	:	1;
}	hcode_t;
//[*]----------------------------------------------------------------------[*]
typedef	struct hchar__t	{
	unsigned char	fchar;
	unsigned char	schar;
}	hchar_t;
//[*]----------------------------------------------------------------------[*]
typedef union han__u	{
	hcode_t c;
	hchar_t b;
}	han_u;

static	han_u	han;

static	byte	hbuffer[100];
//[*]----------------------------------------------------------------------[*]
typedef struct chk_han__t	{
	unsigned char	n_used	: 7;
	unsigned char	flag	: 1;
}	chk_han_t;

typedef union  chk_han__u	{
	chk_han_t		bits	 ;
	byte			byte_data;
}	chk_han_u;
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
	word			rx_sp, rx_ep;
	word 			tx_sp, tx_ep;
	byte			tx_flag     ;
	byte			cat_flag    ;
	byte			tx_buf[BUFFER_SIZE];
	byte			rx_buf[BUFFER_SIZE];
	byte			pcnt;
	byte			type;
	unsigned long	xtal;
	word			base;
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	com;	// Internal Serial  ( Communication Port )
//[*]----------------------------------------------------------------------[*]
const byte sangyongtable[3][33] = {
	{	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
		0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01	},
	{	0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
		0x00, 0x00, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
		0x00, 0x00, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
		0x00, 0x00, 0x12, 0x13, 0x14, 0x15, 0x00, 0x00, 0x02	},
	{	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
		0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x00, 0x11, 0x12, 0x13, 0x14, 0x15,
		0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x00, 0x00, 0x01	}
};
//[*]----------------------------------------------------------------------[*]
const byte Dml[22] = { 0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1 															     };
const byte Dfm[40] = { 1, 3, 0, 2, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 0, 2, 1, 3, 1, 3, 1, 3 			 };
const byte Dmf[44] = { 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 1, 6, 3, 7, 3, 7, 3, 7, 1, 6, 2, 6, 4, 7, 4, 7, 4, 7, 2, 6, 1, 6, 3, 7, 0, 5};
//[*]----------------------------------------------------------------------[*]
static 	byte 	*code_table[3];		// hangul code	pointer
//[*]----------------------------------------------------------------------[*]
static	word		Sunsor = 0;
static	byte		Common = 0;
static	signed char	Hi_lo  = 0;
static	word		Start_addr = 0x0000;
//[*]----------------------------------------------------------------------[*]
//   Function Prototype
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] 	void T0_int					(void);
interrupt [0x23] 	void SCON_int 				(void);
//[*]----------------------------------------------------------------------[*]
					void init_serial_grp		(protocol_grp_t *comx, byte mode, word b_addr, unsigned long speed,
												 unsigned long crystal, byte smod, byte status, byte protocol_count);
					void init_serial_pro		(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
												 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
					void set_protocol_flag		(protocol_grp_t *comx, byte protocol_number, byte status);
					void protocol_event			(protocol_grp_t *comx, byte com_data);
					void protocol_q				(protocol_grp_t *comx, byte protocol_number, byte com_data);
//[*]----------------------------------------------------------------------[*]
					byte config_read			(void);
					void factory_setup			(void);
					void op_write				(byte op_code);
					void addr_write				(byte addr);
					void clock_puls				(void);
					void delay_count			(byte value);
					byte read_mode				(byte rd_addr, byte op_code);
					void write_mode				(byte cmd, byte wr_addr, byte wr_data, byte op_code);
					byte get_byte				(word addr);
					void set_byte				(byte addr, byte wdata);
					void led_setup				(chk_cfg_u *p);
//[*]----------------------------------------------------------------------[*]
					void putch_engfont			(word index, word start);
					void complete_hangul		(byte mode, word start, byte *dest, const byte *src);
					void dot_puts				(byte *str);
//[*]----------------------------------------------------------------------[*]
					byte disp_chk				(variable_t *var);
					byte disp_cat				(variable_t *var);
					void wind_pros				(word value);
//[*]----------------------------------------------------------------------[*]
					byte read_jumper			(void);
					void variable_init			(void);
					void serial_init			(byte baud);
					void interrupt_init			(void);
					void receive_check			(void);
					void main					(void);
//[*]----------------------------------------------------------------------[*]
