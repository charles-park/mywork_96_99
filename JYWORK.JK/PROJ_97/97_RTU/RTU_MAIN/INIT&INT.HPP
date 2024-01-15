//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]              97' AWS and AMOS RTU(Remote Terminal Unit)             [*]
//[*]                      protocal search program                        [*]
//[*]                Copyright Jinyang industrial co.,ltd                 [*]
//[*]                      Program by Jinmin - Mok                        [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	Use under header with serial io program .....                     [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define		DLL(x)					(x+0)		// 16C450 DLL Register
#define		DLM(x)					(x+1)       // 16C450 DLM Register
#define		THR(x)					(x+0)       // 16C450 THR Register
#define		RBR(x)					(x+0)       // 16C450 RBR Register
#define		IER(x)					(x+1)       // 16C450 IER Register
#define		IIR(x)					(x+2)       // 16C450 IIR Register
#define		LCR(x)					(x+3)       // 16C450 LCR Register
#define		MCR(x)					(x+4)       // 16C450 MCR Register
#define		LSR(x)					(x+5)       // 16C450 LSR Register
#define		MSR(x)					(x+6)       // 16C450 MSR Register
//[*]---------------------------------------------------------------------[*]
#define		BUF_SIZE				1000
//[*]---------------------------------------------------------------------[*]
#define		INTERNAL				0
#define		EXTERNAL				1
//[*]---------------------------------------------------------------------[*]
#define		BPS9600					0
#define		BPS4800					1
#define		BPS2400					2
#define		BPS1200					3
#define		BPS600					4
#define		BPS300					5
//[*]---------------------------------------------------------------------[*]
#define		DATA_BIT_7				0x02
#define		DATA_BIT_8				0x03
#define		STOP_BIT_1				0x00
#define		STOP_BIT_2				0x04
#define		PARITY_NO				0x00
#define		PARITY_ODD				0x08
#define		PARITY_EVEN				0x18
//[*]---------------------------------------------------------------------[*]
#define		M11						0
#define		M22						1
#define		M3						2
#define		M1						3
//[*]---------------------------------------------------------------------[*]
#define		SMOD_ON					1
#define		SMOD_OFF				0
#define		NC						0
//[*]---------------------------------------------------------------------[*]
typedef struct sport_cfg__t	{
	unsigned char	baud;
	unsigned char	data_bit;
	unsigned char	stop_bit;
	unsigned char	parity;
}	sport_cfg_t;

static	sport_cfg_t	sport[5];

//[*]---------------------------------------------------------------------[*]
typedef	struct	variable__t	{
	word	p_sp;
	word	p_ep;
	word	size;
	word	dsize;
	byte	flag;
	byte	*buf;
}	variable_t;

typedef	struct	protocol__t	{
	variable_t	var;
	byte	(*pcheck)(variable_t *p);
	byte	(*pcatch)(variable_t *p);
}	protocol_t;

typedef	struct	protocol_grp__t	{
	word	rx_sp,rx_ep;
	word	tx_sp,tx_ep;
	byte	tx_flag;
	byte	tx_buf[BUF_SIZE];
	byte	rx_buf[BUF_SIZE];
	byte	pcnt;
	byte	type;
	word	base;
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	com1,com2,com3;

//[*]---------------------------------------------------------------------[*]
static	byte	INTER_BAUD[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	word	EXTER_BAUD[] = {0x000C, 0x0018, 0x0030, 0x0060, 0x00C0, 0x0180,
								0x0014, 0x0028, 0x0050, 0x00A0, 0x0000, 0x0000};
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct 	sam_sign__t	{
	unsigned long	header;
	unsigned char	board_id;
	unsigned char	sensor_id;
	unsigned char	command;
	unsigned int	data_size;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	sam_sign_t;

typedef union sam_sign__u	{
	sam_sign_t		send;
	unsigned char	bytes[sizeof(sam_sign_t)];
}	sam_sign_u;

static sam_sign_u	sam_sign;

//[*]---------------------------------------------------------------------[*]
typedef struct sam__t	{
	unsigned long	header;
	unsigned char	command;
	unsigned int	schedule;
	unsigned char	status;
	unsigned char	sensor_code;
	unsigned char	sensor_id;
			 float	aver;
			 float	spec;
			 float	mini;
			 float	maxi;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	sam_t;

typedef union sam__u	{
	sam_t			send;
	unsigned char	bytes[sizeof(sam_t)];
}	sam_u;

static	sam_u		sam;

//[*]---------------------------------------------------------------------[*]
typedef struct 	rcm__t	{
	unsigned long	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	rtu_id;
	unsigned int	station_id;
	unsigned char	board_id;
	unsigned int	schedule;
			 float	p12;
			 float	p5;
			 float	m12;
	unsigned char	bod_sta[8];
	unsigned char	sen_sta[8];
	unsigned char	event[2];
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	rcm_t;

typedef union rcm__u	{
	rcm_t			send;
	unsigned char	bytes[sizeof(rcm_t)];
}	rcm_u;

static rcm_u		rcm;

//[*]---------------------------------------------------------------------[*]
typedef struct 	com2_sign__t	{
	unsigned long	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	rtu_id;
	unsigned int	station_id;
	unsigned char	board_id;
	unsigned int	schedule;
	unsigned int	data_size;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	com2_sign_t;

typedef union com2_sign__u	{
	com2_sign_t		send;
	unsigned char	bytes[sizeof(com2_sign_t)];
}	com2_sign_u;

static com2_sign_u	host_sign_com2;

//[*]---------------------------------------------------------------------[*]
typedef struct 	com3_sign__t	{
	unsigned long	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	rtu_id;
	unsigned int	station_id;
	unsigned char	board_id;
	unsigned int	schedule;
	unsigned int	data_size;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	com3_sign_t;

typedef union com3_sign__u	{
	com3_sign_t		send;
	unsigned char	bytes[sizeof(com3_sign_t)];
}	com3_sign_u;

static com3_sign_u	host_sign_com3;

//[*]---------------------------------------------------------------------[*]
typedef struct	com2_host__t	{
	unsigned long	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	rtu_id;
	unsigned int	station_id;
	unsigned char	board_id;
	unsigned int	schedule;
	unsigned int	set_value;
	unsigned int	nc;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	com2_host_t;

typedef union com2_host__u	{
	com2_host_t		receive;
	unsigned char	bytes[sizeof(com2_host_t)];
}	com2_host_u;

static com2_host_u	host_com2;

//[*]---------------------------------------------------------------------[*]
typedef struct	com3_host__t	{
	unsigned long	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	rtu_id;
	unsigned int	station_id;
	unsigned char	board_id;
	unsigned int	schedule;
	unsigned int	set_value;
	unsigned int	nc;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	com3_host_t;

typedef union com3_host__u	{
	com3_host_t		receive;
	unsigned char	bytes[sizeof(com3_host_t)];
}	com3_host_u;

static com3_host_u	host_com3;

//[*]---------------------------------------------------------------------[*]
typedef struct mpm_cfg__t	{
	unsigned long	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	rtu_id;
	unsigned int	station_id;
	unsigned char	board_id;
	unsigned int	command;
	unsigned char	sensor_su;
	unsigned char	id_code[200];
	unsigned int	schedule_1;
	unsigned int	schedule_2;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}   mpm_cfg_t;

typedef union mpm_cfg__u	{
	mpm_cfg_t		send;
	unsigned char	bytes[sizeof(mpm_cfg_t)];
}	mpm_cfg_u;

static	mpm_cfg_u	mpm_cfg;

//[*]---------------------------------------------------------------------[*]
typedef	struct	version__t	{
	unsigned long	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	rtu_id;
	unsigned int	station_id;
	unsigned char	board_id;
	unsigned int	schedule;
	unsigned char	version[30];
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	version_t;

typedef union version__u	{
	version_t		send;
	unsigned char	bytes[sizeof(version_t)];
}	version_u;

static version_u	version;

//[*]---------------------------------------------------------------------[*]
typedef struct 	led__t	{
	unsigned int	header;
	unsigned char	cmd;
	unsigned char	status;
	unsigned char	sensor_code;
	unsigned char	sensor_id;
	unsigned int	aver;
	unsigned int	spec;
	unsigned int	mini;
	unsigned int	maxi;
	unsigned char	chk_xor;
	unsigned int	tail;
}	led_t;

static led_t		*led_table;

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt void T0_int(void);
interrupt [0x23] void SCON_int(void);
interrupt [0x03] void EX0_int(void);
interrupt [0x13] void EX1_int(void);
void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
					  byte crystal, byte smod, byte sta, byte protocol_count);
void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
					 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
void protocol_event(protocol_grp_t *comx, byte com_data);
void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);
void sport_cfg_save(idata char port);
void sport_cfg_load(void);
void initiliaze(void);
void receive_check__1(void);
void receive_check_23(void);
byte header_tail_check(variable_t *var);
byte com1_sign_catch(variable_t *var);
byte com1_sam_catch(variable_t *var);
void sam_data_catch(void);
byte com1_rcm_catch(variable_t *var);
byte com2_sign_catch(variable_t *var);
byte com2_cmd_catch(variable_t *var);
byte com2_cfg_catch(variable_t *var);
byte com3_sign_catch(variable_t *var);
byte com3_cmd_catch(variable_t *var);
byte com3_cfg_catch(variable_t *var);
void transmit_check__1(void);
void transmit_check_23(void);
void send_data(protocol_grp_t *comx, byte sdata);
void sam_sign_send(protocol_grp_t *comx, byte board_id, byte sensor_id, word size);
void sam_send(protocol_grp_t *comx, byte cmd, word schedule, byte sensor_id);
void rcm_send(protocol_grp_t *comx, word cmd);
void host_sign_send_com2(protocol_grp_t *comx, byte bod_id, word schedule, word size);
void host_sign_send_com3(protocol_grp_t *comx, byte bod_id, word schedule, word size);
void version_send(protocol_grp_t *comx, word schedule);
void dialup_modem_init(void);
void data_send(protocol_grp_t *comx, word schedule, byte pos);
void mpm_cfg_send(protocol_grp_t *comx);
void send_led_data(protocol_grp_t *comx);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
					  byte crystal, byte smod, byte sta, byte protocol_count)
{
	idata byte	baud_value = 0, status    = 0;
	idata word	baud	   = 0, base_addr = 0;

	disable(EA);
	comx->base    = b_addr;
	comx->type    = mode;
	comx->pcnt    = protocol_count;
	comx->rx_sp   = comx->rx_ep = comx->tx_sp = comx->tx_ep = 0;
	comx->p       = (protocol_t *)(malloc( sizeof(protocol_t) * comx->pcnt));
	comx->tx_flag = ON;

	WATCHDOG;
	if(comx->type == INTERNAL)	{
		baud_value += ((crystal == M22) ? 1:0);

		if(smod)	{	PCON |= 0x80;	baud_value += 1; }
		else		{	PCON &= 0x7F;	baud_value += 0; }
		switch(speed)	{
			case	BPS9600:    baud_value += 0;
								TH1 = INTER_BAUD[baud_value];	break;
			case	BPS4800:    baud_value += 1;
								TH1 = INTER_BAUD[baud_value];	break;
			case	BPS2400:    baud_value += 2;
								TH1 = INTER_BAUD[baud_value];	break;
			case	BPS1200:    baud_value += 3;
								TH1 = INTER_BAUD[baud_value];	break;
		}
		TMOD |= 0x21;
		SCON |= 0x52;
		TH0   = 0xF0;
		TL0   = 0x00;
		// Interrupt enable register
		enable(ES);
		disable(ET1);
		enable(ET0);
		enable(EX1);
		enable(EX0);
		// TCON Register initial
		enable(TR1);
		enable(TR0);
		disable(IT1);
		disable(IT0);

		enable(PS);

	} else	{
		baud_value += ((crystal == M3) ? 6:0);
		switch(speed)	{
			case	BPS9600:	baud_value += 0;
								baud = EXTER_BAUD[baud_value];	break;
			case	BPS4800:	baud_value += 1;
								baud = EXTER_BAUD[baud_value];	break;
			case	BPS2400:	baud_value += 2;
								baud = EXTER_BAUD[baud_value];	break;
			case	BPS1200:	baud_value += 3;
								baud = EXTER_BAUD[baud_value];	break;
			case	BPS600:		baud_value += 4;
								baud = EXTER_BAUD[baud_value];	break;
			case	BPS300:		baud_value += 5;
								baud = EXTER_BAUD[baud_value];	break;
		}
		base_addr = comx->base;					// move xdata->data (local)
		status    = sta       ;					// move xdata->data (local)
		if(comx->base == 0x8000)	{
			SI2_CS();							// external com2 port select
			  output( LCR(base_addr),(0x80 | status));
			  output( DLL(base_addr),(byte)( baud  ));
			  output( DLM(base_addr),(byte)( baud >> 8));
			  output( LCR(base_addr),(0x7F & status));
			  output( IER(base_addr), 0x01          );
			  output( MCR(base_addr), 0x0b          );
			iRAM_CS();							// base ram select (0~0x7fff)
		} else if(comx->base == 0x9000)	{
			SI3_CS();							// external com3 port select
			  output( LCR(base_addr),(0x80 | status));
			  output( DLL(base_addr),(byte)( baud  ));
			  output( DLM(base_addr),(byte)( baud >> 8));
			  output( LCR(base_addr),(0x7F & status));
			  output( IER(base_addr), 0x01          );
			  output( MCR(base_addr), 0x0b          );
			iRAM_CS();							// base ram select (0~0x7fff)
		}
	}
	iRAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
					 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2))
{
	comx->p[protocol_number].var.p_sp   = 0  ;
	comx->p[protocol_number].var.p_ep   = 0  ;
	comx->p[protocol_number].var.flag   = 0  ;
	comx->p[protocol_number].var.size   = buffer_size;
	comx->p[protocol_number].var.buf    = (byte *)(malloc(comx->p[protocol_number].var.size));
	comx->p[protocol_number].pcheck 	= chk;
	comx->p[protocol_number].pcatch 	= cat;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status)
{
	comx->p[protocol_number].var.flag = status;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void protocol_event(protocol_grp_t *comx, byte com_data)
{
	idata byte	position;

	for(position = 0; position < comx->pcnt; position++)	{
		if(comx->p[position].var.flag == ON)	{
			protocol_q(comx, position, com_data);
			if(comx->p[position].pcheck(&comx->p[position].var))	{
				if(comx->base == 0x0000)	led_con(COM1,RX,OFF);
				if(comx->base == 0x8000)	led_con(COM2,RX,OFF);
				if(comx->base == 0x9000)	led_con(COM3,RX,OFF);
				if(comx->p[position].pcatch(&comx->p[position].var))	{
					set_protocol_flag(comx, position, OFF);
				}
			}
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data)
{
	WATCHDOG;
	comx->p[protocol_number].var.p_ep %= comx->p[protocol_number].var.size;
	if(comx->p[protocol_number].var.p_ep == comx->p[protocol_number].var.p_sp)
	{
		comx->p[protocol_number].var.p_sp++;
		comx->p[protocol_number].var.p_sp %= comx->p[protocol_number].var.size;
	}
	comx->p[protocol_number].var.buf[comx->p[protocol_number].var.p_ep++] = com_data;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sport_cfg_save(idata char port)
{
	idata byte val;

	val = sport[port].baud;
	CFG_RAM(MPM_CFG);	output(SPORT_CFG + (port * 4),     val);	RAM_CS();
	val = sport[port].data_bit;
	CFG_RAM(MPM_CFG);	output(SPORT_CFG + (port * 4) + 1, val);	RAM_CS();
	val = sport[port].stop_bit;
	CFG_RAM(MPM_CFG);	output(SPORT_CFG + (port * 4) + 2, val);	RAM_CS();
	val = sport[port].parity;
	CFG_RAM(MPM_CFG);	output(SPORT_CFG + (port * 4) + 3, val);	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sport_cfg_load(void)
{
	idata byte i, val;

	for(i=0; i<4; i++)	{
		CFG_RAM(MPM_CFG);  val = input(SPORT_CFG + (i * 4)    );	RAM_CS();
		sport[i].baud     = val;
		CFG_RAM(MPM_CFG);  val = input(SPORT_CFG + (i * 4) + 1);	RAM_CS();
		sport[i].data_bit = val;
		CFG_RAM(MPM_CFG);  val = input(SPORT_CFG + (i * 4) + 2);	RAM_CS();
		sport[i].stop_bit = val;
		CFG_RAM(MPM_CFG);  val = input(SPORT_CFG + (i * 4) + 3);	RAM_CS();
		sport[i].parity   = val;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void initiliaze(void)
{
	word i, bps[4];
	byte dbit[4], sbit[4], prit[4];

	sport_cfg_load();
	disable(EA);									// All interrupt disable.
	init_serial_grp(&com1, INTERNAL, 0, sport[COM1].baud, M22, SMOD_OFF, NC, 3);
	init_serial_pro(&com1, 0, 15, header_tail_check, com1_sign_catch);
	init_serial_pro(&com1, 1, 32, header_tail_check, com1_sam_catch);
	init_serial_pro(&com1, 2, 52, header_tail_check, com1_rcm_catch);
	set_protocol_flag(&com1, 0, ON );
	set_protocol_flag(&com1, 1, OFF);
	set_protocol_flag(&com1, 2, OFF);
	//[*]-----------------------------------------------------------------[*]
	init_serial_grp(&com2, EXTERNAL, 0x8000, sport[COM2].baud, M1, NC,
		(sport[COM2].data_bit|sport[COM2].stop_bit|sport[COM2].parity), 3);
	init_serial_pro(&com2, 0,  24, header_tail_check, com2_sign_catch);
	init_serial_pro(&com2, 1,  26, header_tail_check, com2_cmd_catch);
	init_serial_pro(&com2, 2, 227, header_tail_check, com2_cfg_catch);
	set_protocol_flag(&com2, 0, ON );
	set_protocol_flag(&com2, 1, OFF);
	set_protocol_flag(&com2, 2, OFF);
	//[*]-----------------------------------------------------------------[*]
	init_serial_grp(&com3, EXTERNAL, 0x9000, sport[COM3].baud, M1, NC,
		(sport[COM3].data_bit|sport[COM3].stop_bit|sport[COM3].parity), 3);
	init_serial_pro(&com3, 0,  24, header_tail_check, com3_sign_catch);
	init_serial_pro(&com3, 1,  26, header_tail_check, com3_cmd_catch);
	init_serial_pro(&com3, 2, 227, header_tail_check, com3_cfg_catch);
	set_protocol_flag(&com3, 0, OFF);
	set_protocol_flag(&com3, 1, OFF);
	set_protocol_flag(&com3, 2, OFF);
	//[*]-----------------------------------------------------------------[*]
	enable(EA);									// All interrupt enable.
	lcd_memory_move('p');
	lcd_clear();
	for(i=0; i<4; i++)	{
		switch(sport[i].baud)	{
			case BPS300      :	bps[i]  = 300 ;	break;
			case BPS600      :	bps[i]  = 600 ;	break;
			case BPS1200     :	bps[i]  = 1200;	break;
			case BPS2400     :	bps[i]  = 2400;	break;
			case BPS4800     :	bps[i]  = 4800;	break;
			case BPS9600     :	bps[i]  = 9600;	break;
		}
		switch(sport[i].data_bit)	{
			case DATA_BIT_7  :	dbit[i] = 7   ;	break;
			case DATA_BIT_8  :	dbit[i] = 8   ;	break;
		}
		switch(sport[i].stop_bit)	{
			case STOP_BIT_1  :	sbit[i] = 1   ;	break;
			case STOP_BIT_2  :	sbit[i] = 2   ;	break;
		}
		switch(sport[i].parity)	{
			case PARITY_NO   :	prit[i] = 'N' ;	break;
			case PARITY_ODD  :	prit[i] = 'O' ;	break;
			case PARITY_EVEN :	prit[i] = 'E' ;	break;
		}
	}
	lcd_printf(0,0,1,"[ PORTS INITIALIZE ]" );
	lcd_printf(0,1,1," COM1:%4d, %d, %d, %c",bps[1],dbit[1],sbit[1],prit[1]);
	lcd_printf(0,2,1," COM2:%4d, %d, %d, %c",bps[2],dbit[2],sbit[2],prit[2]);
	lcd_printf(0,3,1," COM3:%4d, %d, %d, %c",bps[3],dbit[3],sbit[3],prit[3]);
	led_con(COM1,TX,OFF);	led_con(COM1,RX,OFF);
	led_con(COM2,TX,OFF);	led_con(COM2,RX,OFF);
	led_con(COM3,TX,OFF);	led_con(COM3,RX,OFF);
	delay(60000);
	lcd_memory_move('g');
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void receive_check__1(void)
{
	if(com1.rx_sp != com1.rx_ep)	{
		led_con(COM1,RX,ON);
		protocol_event(&com1,com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= BUF_SIZE)	com1.rx_sp = 0;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void receive_check_23(void)
{
	if(com2.rx_sp != com2.rx_ep)	{
		led_con(COM2,RX,ON);
		protocol_event(&com2,com2.rx_buf[com2.rx_sp++]);
		if(com2.rx_sp >= BUF_SIZE)	com2.rx_sp = 0;
	}
	if(com3.rx_sp != com3.rx_ep)	{
		led_con(COM3,RX,ON);
		protocol_event(&com3,com3.rx_buf[com3.rx_sp++]);
		if(com3.rx_sp >= BUF_SIZE)	com3.rx_sp = 0;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte header_tail_check(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFE)    return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != 0xFE)    return 0;
	if(var->buf[(var->p_sp             +3) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp             +2) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com1_sign_catch(variable_t *var)
{
	byte val, chk_xor = 0, chk_add = 0;
	word count;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		sam_sign.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(sam_sign.send.chk_xor != chk_xor)							return 0;
	if(sam_sign.send.chk_add != chk_add)							return 0;
	if(sam_sign.send.command == 'D')	{
		for(count=0; count<com1.pcnt; count++)	{
			if(sam_sign.send.data_size == com1.p[count].var.size)	{
				set_protocol_flag(&com1, count, ON);
			}
		}
	}
	WATCHDOG;														return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com1_sam_catch(variable_t *var)
{
	byte val, chk_xor = 0,	chk_add = 0;
	word count;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		sam.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(sam.send.chk_xor != chk_xor)									return 1;
	if(sam.send.chk_add != chk_add)									return 1;
	if(sam.send.sensor_id >= i_data.sensor_su)						return 1;
	switch(sam.send.command)	{
		case 'W' :	sam_data_catch();								return 1;
		case '$' :	max_min_load(sam.send.sensor_id);
					sam_sign_send(&com1, 0x00, sam.send.sensor_id, sizeof(sam_t));
					sam_send(&com1, '@', 0x00, sam.send.sensor_id);	return 1;
		case '*' :	variable_save(OFFSET_BASE, sam.send.sensor_id, sam.send.mini);
																	return 1;
		case 'C' :	// SAM Config data send
																	return 1;
	}
	WATCHDOG;														return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sam_data_catch(void)
{
	byte p, id;
	word val;

	WATCHDOG;
	id = sam.send.sensor_id;
	if(sam.send.schedule == 0x0000)	{						// Real data
		led_table[id + 2].cmd         = 'M';
		led_table[id + 2].status      = sam.send.status;
		led_table[id + 2].sensor_code = sam.send.sensor_code;
		led_table[id + 2].sensor_id   = sam.send.sensor_id;

		if((sam.send.sensor_code == 2) || (sam.send.sensor_code == 7))	{
			led_table[id + 2].aver = (int)((sam.send.aver + 100.) * 10.);
			led_table[id + 2].spec = (int)((sam.send.spec + 100.) * 10.);
			led_table[id + 2].mini = (int)((sam.send.mini + 100.) * 10.);
			led_table[id + 2].maxi = (int)((sam.send.maxi + 100.) * 10.);
		} else	if(sam.send.sensor_code == 9)	{
			led_table[id + 2].aver = (int)(sam.send.aver * 10.);
			led_table[id + 2].mini = (int)(sam.send.mini * 10.);
			led_table[id + 2].maxi = (int)(sam.send.maxi * 10.);
			val = (int)(sam.send.maxi) / 60;
			led_table[id + 2].spec = val * 100;
			val = (int)(sam.send.maxi) % 60;
			led_table[id + 2].spec += val;
		} else	{
			led_table[id + 2].aver = (int)(sam.send.aver * 10.);
			led_table[id + 2].spec = (int)(sam.send.spec * 10.);
			led_table[id + 2].mini = (int)(sam.send.mini * 10.);
			led_table[id + 2].maxi = (int)(sam.send.maxi * 10.);
		}
		led_table[id + 2].header   = 0xFFFF;
		led_table[id + 2].tail     = 0xFFFE;

		p = REAL;
		sensor_status_set(sam.send.sensor_id, sam.send.status);
		real.receive_status = ON;
	} else	if(sam.send.schedule == i_data.call_time)	{	// Call data
		p = CALL;
		call.receive_status = ON;
	} else	if(sam.send.schedule == i_data.save_time)	{	// Save data
		p = SAVE;
		save.receive_status = ON;
	}
	weather[p].year  = rtc.year;	weather[p].month = rtc.month;
	weather[p].day   = rtc.day ;	weather[p].hour  = rtc.hour ;
	weather[p].min   = rtc.min ;	weather[p].sec   = rtc.sec  ;
	id = sam.send.sensor_id;
	weather[p].sensor[id].sensor_st	  = sam.send.status;
	weather[p].sensor[id].sensor_code = sam.send.sensor_code;
	weather[p].sensor[id].sensor_id	  = sam.send.sensor_id;
	weather[p].sensor[id].aver		  = sam.send.aver;
	weather[p].sensor[id].spec		  = sam.send.spec;
	weather[p].sensor[id].mini		  = sam.send.mini;
	weather[p].sensor[id].maxi		  = sam.send.maxi;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com1_rcm_catch(variable_t *var)
{
	byte val, chk_xor = 0,	chk_add = 0;
	word count;

	if(var->buf[(var->p_sp + 13) % var->size] != RCM_ID)   			return 1;
	if(var->buf[(var->p_sp + 15) % var->size] != 'L'   )			return 1;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(chk_xor != var->buf[(var->p_sp + 46) % var->size])			return 1;
	if(chk_add != var->buf[(var->p_sp + 47) % var->size])    		return 1;

	for(count=16; count<46; count++)	{	// Voltage +12, -12, +5 & Status
		rcm.bytes[count] = var->buf[(var->p_sp + count) % var->size];
	}
	rcm_poll.receive_status = ON;
	WATCHDOG;														return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com2_sign_catch(variable_t *var)
{
	byte val, chk_xor = 0, chk_add = 0, command;
	word count;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		host_sign_com2.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(host_sign_com2.send.rtu_id   != i_data.rtu_id)				return 0;
	if(host_sign_com2.send.board_id != MPM_ID )						return 0;
	if(host_sign_com2.send.chk_xor  != chk_xor)						return 0;
	if(host_sign_com2.send.chk_add  != chk_add)						return 0;
	command = var->buf[(var->p_sp + 15) % var->size];
	if(command == 'D')	{
		for(count=0; count<com2.pcnt; count++)	{
			if(host_sign_com2.send.data_size == com2.p[count].var.size)	{
				set_protocol_flag(&com2, count, ON);
			}
		}
	}
	WATCHDOG;														return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com2_cmd_catch(variable_t *var)
{
	byte val, chk_add = 0, chk_xor = 0, cmd_h, cmd_l;
	word count;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		host_com2.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(host_com2.receive.rtu_id   != i_data.rtu_id)					return 1;
	if(host_com2.receive.board_id != MPM_ID )						return 1;
	if(host_com2.receive.chk_xor  != chk_xor)						return 1;
	if(host_com2.receive.chk_add  != chk_add)						return 1;
	cmd_h = var->buf[(var->p_sp + 14) % var->size];
	cmd_l = var->buf[(var->p_sp + 15) % var->size];
	WATCHDOG;
	if(cmd_h == 0xFF)	{
		switch(cmd_l)	{
			case 'v' :	host_sign_send_com2(&com2, MPM_ID, SIZE_SEND, sizeof(version_t));
						version_send(&com2, 0xFF76);
						break;
			case 'I' :	i_data.station_id = host_com2.receive.station_id;
						mpm_cfg_save(STATION_ID);
						host_sign_send_com2(&com2, MPM_ID, ACK_SEND, ID_SET);
						break;
			case 'P' :	i_data.password = host_com2.receive.set_value;
						mpm_cfg_save(PASSWORD);
						host_sign_send_com2(&com2, MPM_ID, ACK_SEND, PASS_SET);
						break;
			case 'T' :	rtc_set();
						host_sign_send_com2(&com2, MPM_ID, ACK_SEND, DT_SET);
						break;
			case 'C' :	buffer_clear();
						host_sign_send_com2(&com2, MPM_ID, ACK_SEND, BUFF_CLEAR);
						while(com2.tx_sp != com2.tx_ep)	{
							transmit_check__1();	receive_check__1();
							transmit_check_23();	receive_check_23();
						}
						while(1)	disable(EA);
			case 'B' :	// Baud rate set function call
						if     (host_com2.receive.set_value == 1200)	sport[COM2].baud = BPS1200;
						else if(host_com2.receive.set_value == 2400)	sport[COM2].baud = BPS2400;
						else if(host_com2.receive.set_value == 4800)	sport[COM2].baud = BPS4800;
						else if(host_com2.receive.set_value == 9600)	sport[COM2].baud = BPS9600;
						else	{
							host_sign_send_com2(&com2, MPM_ID, ACK_SEND, 0x0058);	// 0x42 = 'X'
							break;
						}
						host_sign_send_com2(&com2, MPM_ID, ACK_SEND, BUAD_SET);
						while(com2.tx_sp != com2.tx_ep)	transmit_check_23();
						sport_cfg_save(COM2);
						initiliaze();							// Serial port & rx Q initialize
						break;
			case '*' :	// SAM Board auto offset set function
						cmd_h = host_com2.receive.year;			// Sensor id
						sam_sign_send(&com1, 0x00, cmd_h, sizeof(sam_t));
						sam_send(&com1, '*', REAL, cmd_h);
						host_sign_send_com2(&com2, MPM_ID, ACK_SEND, SOF_SET);
						variable_save(SPECIAL_BASE, cmd_h, 0.0);
						variable_save(OFFSET_BASE, cmd_h, 0.0);
						break;
			case '#' :	// User define offset set function
						cmd_h = host_com2.receive.year;			// Sensor id
						sam.send.mini = (float)(host_com2.receive.set_value);
						sam_sign_send(&com1, 0x00, cmd_h, sizeof(sam_t));
						sam_send(&com1, '#', REAL, cmd_h);
						host_sign_send_com2(&com2, MPM_ID, ACK_SEND, UOF_SET);
						variable_save(OFFSET_BASE, cmd_h, sam.send.mini);
						break;
			case 'N' :	host_sign_send_com2(&com2, MPM_ID, SIZE_SEND, sizeof(mpm_cfg_t));
						mpm_cfg_send(&com2);
						break;
		}
	} else	if(host_com2.receive.schedule == 0x0000        )	{		// Real data poll
		host_sign_send_com2(&com2, MPM_ID, SIZE_SEND, weather_size());
		data_send(&com2, host_com2.receive.schedule, REAL);
		if((rtc.year != host_com2.receive.year ) || (rtc.month != host_com2.receive.month) ||
		   (rtc.day  != host_com2.receive.day  ) || (rtc.hour  != host_com2.receive.hour ))	rtc_set();
	} else	if(host_com2.receive.schedule == i_data.call_time)	{	// Call data poll
		host_sign_send_com2(&com2, MPM_ID, SIZE_SEND, weather_size());
		data_send(&com2, host_com2.receive.schedule, CALL);
	} else	if(host_com2.receive.schedule == i_data.save_time)	{	// Save data poll
		host_sign_send_com2(&com2, MPM_ID, SIZE_SEND, weather_size());
		count = load_data(SEND);
		data_send(&com2, count, SEND);
	}
	WATCHDOG;														return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com2_cfg_catch(variable_t *var)
{
	byte chk_add = 0, chk_xor = 0, cmd_h, cmd_l;
	word count;
	idata byte pos, val;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		mpm_cfg.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
			WATCHDOG;
		}
	}
	if(mpm_cfg.send.rtu_id   != i_data.rtu_id)						return 1;
	if(mpm_cfg.send.board_id != MPM_ID )							return 1;
	if(mpm_cfg.send.chk_xor  != chk_xor)							return 1;
	if(mpm_cfg.send.chk_add  != chk_add)							return 1;
	cmd_h = var->buf[(var->p_sp + 14) % var->size];
	cmd_l = var->buf[(var->p_sp + 15) % var->size];
	if((cmd_h != 0xFF) || (cmd_l != 'N'))							return 1;
	for(pos=0; pos<200; pos++)	{
		val = mpm_cfg.send.id_code[pos];
		CFG_RAM(MPM_CFG);	output(ID_CODE + pos, val);	RAM_CS();
	}
	i_data.sensor_su  = mpm_cfg.send.sensor_su ; mpm_cfg_save(SENSOR_SU );
	i_data.call_time  = mpm_cfg.send.schedule_1; mpm_cfg_save(CALL_TIME );	// Call time
	i_data.save_time  = mpm_cfg.send.schedule_2; mpm_cfg_save(SAVE_TIME );	// 10 minute
	i_data.station_id = mpm_cfg.send.station_id; mpm_cfg_save(STATION_ID);
	value.config_receive_status = ON;
	WATCHDOG;														return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com3_sign_catch(variable_t *var)
{
	byte val, chk_xor = 0, chk_add = 0, command;
	word count;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		host_sign_com3.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(host_sign_com3.send.rtu_id   != i_data.rtu_id)				return 0;
	if(host_sign_com3.send.board_id != MPM_ID )						return 0;
	if(host_sign_com3.send.chk_xor  != chk_xor)						return 0;
	if(host_sign_com3.send.chk_add  != chk_add)						return 0;
	command = var->buf[(var->p_sp + 15) % var->size];
	if(command == 'D')	{
		for(count=0; count<com3.pcnt; count++)	{
			if(host_sign_com3.send.data_size == com3.p[count].var.size)	{
				set_protocol_flag(&com3, count, ON);
			}
		}
	}
	WATCHDOG;														return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com3_cmd_catch(variable_t *var)
{
	byte val, chk_add = 0, chk_xor = 0, cmd_h, cmd_l;
	word count;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		host_com3.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(host_com3.receive.rtu_id   != i_data.rtu_id)					return 1;
	if(host_com3.receive.board_id != MPM_ID )						return 1;
	if(host_com3.receive.chk_xor  != chk_xor)						return 1;
	if(host_com3.receive.chk_add  != chk_add)						return 1;
	cmd_h = var->buf[(var->p_sp + 14) % var->size];
	cmd_l = var->buf[(var->p_sp + 15) % var->size];
	WATCHDOG;
	if(cmd_h == 0xFF)	{
		switch(cmd_l)	{
			case 'v' :	host_sign_send_com3(&com3, MPM_ID, SIZE_SEND, sizeof(version_t));
						version_send(&com3, 0xFF76);
						break;
			case 'I' :  i_data.station_id = host_com3.receive.station_id;
						mpm_cfg_save(STATION_ID);
						host_sign_send_com3(&com3, MPM_ID, ACK_SEND, ID_SET);
						break;
			case 'P' :	i_data.password = host_com3.receive.set_value;
						mpm_cfg_save(PASSWORD);
						host_sign_send_com3(&com3, MPM_ID, ACK_SEND, PASS_SET);
						break;
			case 'T' :	disable(EA);
						rtc.year  = host_com3.receive.year;	rtc.month = host_com3.receive.month;
						rtc.day   = host_com3.receive.day;	rtc_date_write(&rtc);
						disable(EA);
						rtc.hour  = host_com3.receive.hour;	rtc.min   = host_com3.receive.min;
						rtc.sec   = host_com3.receive.sec;	rtc_time_write(&rtc);
						RAM_CS();
						host_sign_send_com3(&com3, MPM_ID, ACK_SEND, DT_SET);
						break;
			case 'C' :	buffer_clear();
						host_sign_send_com3(&com3, MPM_ID, ACK_SEND, BUFF_CLEAR);
						while(com3.tx_sp != com3.tx_ep)	{
							transmit_check__1();	receive_check__1();
							transmit_check_23();	receive_check_23();
						}
						while(1)	disable(EA);
			case 'B' :	// Baud rate set function call
						if     (host_com3.receive.set_value == 1200)	sport[COM3].baud = BPS1200;
						else if(host_com3.receive.set_value == 2400)	sport[COM3].baud = BPS2400;
						else if(host_com3.receive.set_value == 4800)	sport[COM3].baud = BPS4800;
						else if(host_com3.receive.set_value == 9600)	sport[COM3].baud = BPS9600;
						else											sport[COM3].baud = BPS2400;
						host_sign_send_com3(&com3, MPM_ID, ACK_SEND, BUAD_SET);
						while(com3.tx_sp != com3.tx_ep)	transmit_check_23();
						sport_cfg_save(COM3);
						initiliaze();								// Serial port & rx Q initialize
						break;
			case '*' :	// SAM Board auto offset set function
						cmd_h = host_com3.receive.year;			// Sensor id
						sam_sign_send(&com1, 0x00, cmd_h, sizeof(sam_t));
						sam_send(&com1, '*', REAL, cmd_h);
						host_sign_send_com3(&com3, MPM_ID, ACK_SEND, SOF_SET);
						variable_save(SPECIAL_BASE, cmd_h, 0.0);
						variable_save(OFFSET_BASE, cmd_h, 0.0);
						break;
			case '#' :	// User define offset set function
						cmd_h = host_com3.receive.year;			// Sensor id
						sam.send.mini = (float)(host_com3.receive.set_value);
						sam_sign_send(&com1, 0x00, cmd_h, sizeof(sam_t));
						sam_send(&com1, '#', REAL, cmd_h);
						host_sign_send_com3(&com3, MPM_ID, ACK_SEND, UOF_SET);
						variable_save(OFFSET_BASE, cmd_h, sam.send.mini);
						break;
			case 'N' :	host_sign_send_com3(&com3, MPM_ID, SIZE_SEND, sizeof(mpm_cfg_t));
						mpm_cfg_send(&com3);
						break;
		}
	} else	if(host_com3.receive.schedule == 0x0000        )	{	// Real data poll
		host_sign_send_com3(&com3, MPM_ID, SIZE_SEND, weather_size());
		data_send(&com3, host_com3.receive.schedule, REAL);
	} else	if(host_com3.receive.schedule == i_data.call_time)	{	// Call data poll
		host_sign_send_com3(&com3, MPM_ID, SIZE_SEND, weather_size());
		data_send(&com3, host_com3.receive.schedule, CALL);
	} else	if(host_com3.receive.schedule == i_data.save_time)	{	// Save data poll
		host_sign_send_com3(&com3, MPM_ID, SIZE_SEND, weather_size());
		count = load_data(SEND);
		data_send(&com3, count, SEND);
	}
	WATCHDOG;														return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com3_cfg_catch(variable_t *var)
{
	byte chk_add = 0, chk_xor = 0, cmd_h, cmd_l;
	word count;
	idata byte pos, val;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		mpm_cfg.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
			WATCHDOG;
		}
	}
	if(mpm_cfg.send.rtu_id   != i_data.rtu_id)						return 1;
	if(mpm_cfg.send.board_id != MPM_ID )							return 1;
	if(mpm_cfg.send.chk_xor  != chk_xor)							return 1;
	if(mpm_cfg.send.chk_add  != chk_add)							return 1;
	cmd_h = var->buf[(var->p_sp + 14) % var->size];
	cmd_l = var->buf[(var->p_sp + 15) % var->size];
	if((cmd_h != 0xFF) || (cmd_l != 'N'))							return 1;
	for(pos=0; pos<200; pos++)	{
		val = mpm_cfg.send.id_code[pos];
		CFG_RAM(MPM_CFG);	output(ID_CODE + pos, val);	RAM_CS();
	}
	i_data.sensor_su  = mpm_cfg.send.sensor_su ; mpm_cfg_save(SENSOR_SU );
	i_data.call_time  = mpm_cfg.send.schedule_1; mpm_cfg_save(CALL_TIME );	// Call time
	i_data.save_time  = mpm_cfg.send.schedule_2; mpm_cfg_save(SAVE_TIME );	// 10 minute
	i_data.station_id = mpm_cfg.send.station_id; mpm_cfg_save(STATION_ID);
	value.config_receive_status = ON;
	WATCHDOG;														return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void transmit_check__1(void)
{
	if(com1.tx_sp != com1.tx_ep)	{
		if(com1.tx_flag)	{
			disable(EA);
			SBUF = com1.tx_buf[com1.tx_sp++];		RAM_CS();
			if(com1.tx_sp >= BUF_SIZE)	com1.tx_sp = 0;
			led_con(COM1, TX, ON);
			com1.tx_flag = OFF;
		}
	} else	led_con(COM1,TX,OFF);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void transmit_check_23(void)
{
	idata static byte io_data;	  				// p1_port;
	idata static word io_addr;

	if(com2.tx_sp != com2.tx_ep)	{
		io_addr = LSR(com2.base);
		SI2_CS();	io_data = input(io_addr);		RAM_CS();
		if(0x20 & io_data)	{
			led_con(COM2,TX,ON);
			io_data = com2.tx_buf[com2.tx_sp++];
			io_addr = THR(com2.base);
			SI2_CS();	output(io_addr,io_data);	RAM_CS();
			if(com2.tx_sp >= BUF_SIZE)	com2.tx_sp = 0;
		}
	}	else	{
		led_con(COM2,TX,OFF);
		RAM_CS();
	}
	if(com3.tx_sp != com3.tx_ep)	{
		io_addr = LSR(com3.base);
		SI3_CS();	io_data = input(io_addr);		RAM_CS();
		if(0x20 & io_data)	{
			led_con(COM3,TX,ON);
			io_data = com3.tx_buf[com3.tx_sp++];
			io_addr = THR(com3.base);
			SI3_CS();	output(io_addr,io_data);	RAM_CS();
			if(com3.tx_sp >= BUF_SIZE)	com3.tx_sp = 0;
		}
	}	else	{
		led_con(COM3,TX,OFF);
		RAM_CS();
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_data(protocol_grp_t *comx, byte sdata)
{
	WATCHDOG;
	comx->tx_buf[comx->tx_ep++] = sdata;
	if(comx->tx_ep >= BUF_SIZE)	comx->tx_ep = 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sam_sign_send(protocol_grp_t *comx, byte board_id, byte sensor_id, word size)
{
	byte i;

	sam_sign.send.header		= 0xFFFFFFFF;
	sam_sign.send.board_id		= board_id;
	sam_sign.send.sensor_id		= sensor_id;
	sam_sign.send.command		= 'D';
	sam_sign.send.data_size		= size;
	sam_sign.send.chk_xor		= 0x00;
	sam_sign.send.chk_add		= 0x00;
	sam_sign.send.tail			= 0xFEFEFEFE;
	for(i=0; i<sizeof(sam_sign_u); i++)	{
		if(i>3 && i<sizeof(sam_sign_u) - 6)	{
			sam_sign.send.chk_xor ^= sam_sign.bytes[i];
			sam_sign.send.chk_add += sam_sign.bytes[i];
		}
		send_data(comx,sam_sign.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sam_send(protocol_grp_t *comx, byte cmd, word schedule, byte sensor_id)
{
	byte i;

	sam.send.header			= 0xFFFFFFFF;
	sam.send.command		= cmd;
	sam.send.schedule		= schedule;
	sam.send.sensor_id		= sensor_id;
	sam.send.chk_xor		= 0x00;
	sam.send.chk_add		= 0x00;
	sam.send.tail			= 0xFEFEFEFE;
	for(i=0; i<sizeof(sam_u); i++)	{
		if(i>3 && i<sizeof(sam_u) - 6)	{
			sam.send.chk_xor ^= sam.bytes[i];
			sam.send.chk_add += sam.bytes[i];
		}
		send_data(comx,sam.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rcm_send(protocol_grp_t *comx, word cmd)
{
	byte i;

	rcm.send.header			= 0xFFFFFFFF;
	rcm.send.year			= rtc.year;
	rcm.send.month			= rtc.month;
	rcm.send.day			= rtc.day;
	rcm.send.hour			= rtc.hour;
	rcm.send.min			= rtc.min;
	rcm.send.sec			= rtc.sec;
	rcm.send.rtu_id			= i_data.rtu_id;
	rcm.send.station_id		= i_data.station_id;
	rcm.send.board_id		= RCM_ID;
	rcm.send.schedule		= cmd;
	rcm.send.chk_xor		= 0x00;
	rcm.send.chk_add		= 0x00;
	rcm.send.tail			= 0xFEFEFEFE;
	for(i=0; i<sizeof(rcm_u); i++)  {
		if(i>3 && i<sizeof(rcm_u) - 6)  {
			rcm.send.chk_xor ^= rcm.bytes[i];
			rcm.send.chk_add += rcm.bytes[i];
		}
		send_data(comx,rcm.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void host_sign_send_com2(protocol_grp_t *comx, byte bod_id, word schedule, word size)
{
	byte i;

	host_sign_com2.send.header		= 0xFFFFFFFF;
	host_sign_com2.send.year		= rtc.year;
	host_sign_com2.send.month		= rtc.month;
	host_sign_com2.send.day			= rtc.day;
	host_sign_com2.send.hour		= rtc.hour;
	host_sign_com2.send.min			= rtc.min;
	host_sign_com2.send.sec			= rtc.sec;
	host_sign_com2.send.rtu_id		= i_data.rtu_id;
	host_sign_com2.send.station_id	= i_data.station_id;
	host_sign_com2.send.board_id	= bod_id;
	host_sign_com2.send.schedule	= schedule;
	host_sign_com2.send.data_size	= size;
	host_sign_com2.send.chk_xor		= 0x00;
	host_sign_com2.send.chk_add		= 0x00;
	host_sign_com2.send.tail		= 0xFEFEFEFE;
	for(i=0; i<sizeof(com2_sign_u); i++)	{
		if(i>3 && i<sizeof(com2_sign_u) - 6)	{
			host_sign_com2.send.chk_xor ^= host_sign_com2.bytes[i];
			host_sign_com2.send.chk_add += host_sign_com2.bytes[i];
		}
		send_data(comx,host_sign_com2.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void host_sign_send_com3(protocol_grp_t *comx, byte bod_id, word schedule, word size)
{
	byte i;

	host_sign_com3.send.header		= 0xFFFFFFFF;
	host_sign_com3.send.year		= rtc.year;
	host_sign_com3.send.month		= rtc.month;
	host_sign_com3.send.day			= rtc.day;
	host_sign_com3.send.hour		= rtc.hour;
	host_sign_com3.send.min			= rtc.min;
	host_sign_com3.send.sec			= rtc.sec;
	host_sign_com3.send.rtu_id		= i_data.rtu_id;
	host_sign_com3.send.station_id	= i_data.station_id;
	host_sign_com3.send.board_id	= bod_id;
	host_sign_com3.send.schedule	= schedule;
	host_sign_com3.send.data_size	= size;
	host_sign_com3.send.chk_xor		= 0x00;
	host_sign_com3.send.chk_add		= 0x00;
	host_sign_com3.send.tail		= 0xFEFEFEFE;
	for(i=0; i<sizeof(com3_sign_u); i++)	{
		if(i>3 && i<sizeof(com2_sign_u) - 6)	{
			host_sign_com3.send.chk_xor ^= host_sign_com3.bytes[i];
			host_sign_com3.send.chk_add += host_sign_com3.bytes[i];
		}
		send_data(comx,host_sign_com3.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void version_send(protocol_grp_t *comx, word schedule)
{
	byte i;

	for(i=0; i<30; i++)	version.send.version[i] = 0x00;
	strcpy(version.send.version,"JY-AWS Ver : 9708.25");
	version.send.header		= 0xFFFFFFFF;
	version.send.year		= rtc.year;
	version.send.month		= rtc.month;
	version.send.day		= rtc.day;
	version.send.hour		= rtc.hour;
	version.send.min		= rtc.min;
	version.send.sec		= rtc.sec;
	version.send.rtu_id		= i_data.rtu_id;
	version.send.station_id	= i_data.station_id;
	version.send.board_id	= MPM_ID;
	version.send.schedule	= schedule;
	version.send.chk_xor	= 0x00;
	version.send.chk_add	= 0x00;
	version.send.tail		= 0xFEFEFEFE;
	for(i=0; i<sizeof(version_u); i++)	{
		if(i>3 && i<sizeof(version_u) - 6)	{
			version.send.chk_xor ^= version.bytes[i];
			version.send.chk_add += version.bytes[i];
		}
		send_data(comx,version.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void dialup_modem_init(void)
{
	send_data(&com2,'A');
	send_data(&com2,'T');
	send_data(&com2,'S');
	send_data(&com2,'0');
	send_data(&com2,'=');
	send_data(&com2,'3');

	send_data(&com3,'A');
	send_data(&com3,'T');
	send_data(&com3,'S');
	send_data(&com3,'0');
	send_data(&com3,'=');
	send_data(&com3,'3');
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void data_send(protocol_grp_t *comx, word schedule, byte pos)
{
	word i;
	char *w = (char *)(&weather[pos]);
	char *p = (char *)( weather[pos].sensor);

	weather[pos].header		= 0xFFFFFFFF;
	weather[pos].rtu_id		= i_data.rtu_id;
	weather[pos].station_id	= i_data.station_id;
	weather[pos].board_id	= MPM_ID;
	weather[pos].schedule	= schedule;
	weather[pos].chk_xor 	= 0x00;
	weather[pos].chk_add 	= 0x00;
	for(i=0; i<sizeof(weather_t) - sizeof(sensor_field_t *) - 6; i++)	{
		if(i>3)	{
			weather[pos].chk_xor ^= w[i];
			weather[pos].chk_add += w[i];
		}
		send_data(comx,w[i]);
	}
	for(i=0; i<i_data.sensor_su * sizeof(sensor_field_t); i++)	{
		weather[pos].chk_xor ^= p[i];
		weather[pos].chk_add += p[i];
		send_data(comx,p[i]);
	}
	send_data(comx,weather[pos].chk_xor);
	send_data(comx,weather[pos].chk_add);
	send_data(comx,0xFE);	send_data(comx,0xFE);
	send_data(comx,0xFE);	send_data(comx,0xFE);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void mpm_cfg_send(protocol_grp_t *comx)
{
	idata byte pos, val;
	word  i;

	for(pos=0; pos<200; pos++)	{
		WATCHDOG;
		CFG_RAM(MPM_CFG);	val = input(ID_CODE + pos);
		RAM_CS();			mpm_cfg.send.id_code[pos] = val;
	}
	mpm_cfg_load(SENSOR_SU);
	mpm_cfg_load(CALL_TIME);
	mpm_cfg_load(SAVE_TIME);

	mpm_cfg.send.header		= 0xFFFFFFFF;
	mpm_cfg.send.year		= rtc.year;
	mpm_cfg.send.month		= rtc.month;
	mpm_cfg.send.day		= rtc.day;
	mpm_cfg.send.hour		= rtc.hour;
	mpm_cfg.send.min		= rtc.min;
	mpm_cfg.send.sec		= rtc.sec;
	mpm_cfg.send.rtu_id		= i_data.rtu_id;
	mpm_cfg.send.station_id	= i_data.station_id;
	mpm_cfg.send.board_id	= MPM_ID;
	mpm_cfg.send.command	= 0xFF4E;			// 0x4E = 'N'
	mpm_cfg.send.sensor_su	= i_data.sensor_su;
	mpm_cfg.send.schedule_1	= i_data.call_time;
	mpm_cfg.send.schedule_2	= i_data.save_time;
	mpm_cfg.send.chk_xor	= 0x00;
	mpm_cfg.send.chk_add	= 0x00;
	mpm_cfg.send.tail		= 0xFEFEFEFE;
	for(i=0; i<sizeof(mpm_cfg_u); i++)	{
		if(i>3 && i<sizeof(mpm_cfg_u) - 6)	{
			mpm_cfg.send.chk_xor ^= mpm_cfg.bytes[i];
			mpm_cfg.send.chk_add += mpm_cfg.bytes[i];
		}
		send_data(comx,mpm_cfg.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_led_data(protocol_grp_t *comx)
{
	word date_value, i;
	byte su, *p;

	if(rtc.year < 90) date_value  = 2000;
	else              date_value  = 1900;

	led_table[0].header      = 0xFFFF;
	led_table[0].cmd         = 'N';				// Date command
	led_table[0].status      = 0;
	led_table[0].sensor_code = 0;
	led_table[0].sensor_id   = 0;
	led_table[0].aver        = rtc.year + date_value;
	led_table[0].spec        = rtc.month;
	led_table[0].mini        = rtc.day;
	led_table[0].maxi        = i_data.station_id;
	led_table[0].tail        = 0xFFFE;

	led_table[1].header      = 0xFFFF;
	led_table[1].cmd         = 'S';				// Time command
	led_table[1].status      = 0;
	led_table[1].sensor_code = 0;
	led_table[1].sensor_id   = 0;
	led_table[1].aver        = rtc.hour;
	led_table[1].spec        = rtc.min;
	led_table[1].mini        = rtc.sec;
	led_table[1].maxi        = i_data.station_id;
	led_table[1].tail        = 0xFFFE;

	for(su=0; su<i_data.sensor_su + 2; su++)	{
		led_table[su].chk_xor = 0;
		p = (char *)(&led_table[su]);
		for(i=0; i<sizeof(led_t); i++)	{
			if(i>1 && i<sizeof(led_t)-3)	led_table[su].chk_xor ^= p[i];
			send_data(comx, p[i]);
		}
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt void T0_int(void)         // Timer 0 interrupt vector
{
	idata byte io_data, io_addr, in_key;
	idata byte yy, mm, dd, hh, bb, ss, dt_flag;

	disable(EA);	disable(TR0);
	if(lcd.lcd_redisplay_flag)	{
		if(lcd.lcd_pos >= 40)	lcd.lcd_pos = 0;
		if(lcd.buf_pos >= 80)	{
			lcd.buf_pos = 0;
			if(++lcd.lcd_redisplay_flag > 5)	lcd.lcd_redisplay_flag = 0;
		}
		io_data = DISP_BUFFER[lcd.buf_pos];
		if(lcd.buf_pos < 40)	io_addr  = 0x80 + lcd.lcd_pos;
		else					io_addr  = 0xC0 + lcd.lcd_pos;
		LCD_CS();
		 output(LCD_CON, io_addr);	LCD_BUSY(0x00);
		 output(LCD_DATA,io_data);
		iRAM_CS();
		lcd.lcd_pos++, lcd.buf_pos++;
	}
	WATCHDOG;
	if((++key.key_chtr) > key.key_cmp)	{
		KEY_CS();	in_key = input(0x00);	iRAM_CS();	// Keypad data search
		key.key_chtr = 0;
		if(in_key & 0x80)	{
			if(key.key_up_flag)	key.key_down_flag = 1;
			if(++key.key_del > 10)	{
				key.key_down_flag =  1;	key.key_del = 15;
				key.key_cmp       = 55;
			}
			if(key.key_down_flag)	{
				BUZZER_ON;
				value.buzzer_cnt = 40;	key.key_up_flag = 0;
				key.key_down_flag =  0;
				key.key_data[key.key_pos_st++] = in_key & 0x0F;
				if(key.key_pos_st > 20)	key.key_pos_st = 0;
			}
		} else	{
			key.key_up_flag	= 1;	key.key_del	= 0;	key.key_cmp	= 20;
		}
	}
	if(value.buzzer_cnt-- <= 1)	BUZZER_OFF;
	WATCHDOG;
	if(rtc.delay++  > 80)	{
		rtc.delay = 0;	dt_flag = 1;
		RTC_CS();
		 while(input(RTC_ADD_A) & 0x80);
		 yy = input(RTC_YEA);	mm = input(RTC_MON);	dd = input(RTC_DAY);
		 hh = input(RTC_HOU);	bb = input(RTC_MIN);	ss = input(RTC_SEC);
		iRAM_CS();
		if((yy > 99)||(mm > 12)||(mm < 1)||(dd > 31)||(dd < 1))	dt_flag = 0;
		if((hh > 24)||(bb > 59)||(ss > 59))						dt_flag = 0;
		if(dt_flag)	{
			rtc.year  = yy;	rtc.month = mm;	rtc.day   = dd;
			rtc.hour  = hh;	rtc.min   = bb;	rtc.sec   = ss;
		}

		if(real.sec != rtc.sec)	{
			value.delay_count++;
			real.year = rtc.year;	real.month  = rtc.month;
			real.day  = rtc.day ;	real.hour   = rtc.hour ;
			real.min  = rtc.min ;	real.sec    = rtc.sec  ;
			if(real.sec % 10 == 0)	real.flag   = ON;	// real processing
		}
		if(i_data.call_time != 0)	{
			if(i_data.call_time < 60)	{
				if(((real.sec % call.call_sec)==0)&&(call.sec != real.sec)) {
					call.year = real.year;	call.month = real.month;
					call.day  = real.day ;	call.hour  = real.hour ;
					call.min  = real.min ;	call.sec   = real.sec  ;
					call.flag = ON;						// call processing
				}
			} else	if(((real.min % call.call_min)==0)&&(call.min != real.min)) {
				call.year = real.year;	call.month = real.month;
				call.day  = real.day ;	call.hour  = real.hour ;
				call.min  = real.min ;	call.sec   = 0;
				call.flag = ON;							// call processing
			}
		}
		if(i_data.save_time != 0)	{
			if(i_data.save_time < 60)	{
				if(((real.sec % save.save_sec)==0)&&(save.sec != real.sec)) {
					save.year = real.year;	save.month = real.month;
					save.day  = real.day ;	save.hour  = real.hour ;
					save.min  = real.min ;	save.sec   = real.sec  ;
					save.flag = ON;						// save processing
				}
			} else	if(((real.min % save.save_min)==0)&&(save.min != real.min)) {
				save.year = real.year;	save.month = real.month;
				save.day  = real.day ;	save.hour  = real.hour ;
				save.min  = real.min ;	save.sec   = 0;
				save.flag = ON;							// save processing
			}
		}
	}
	WATCHDOG;
	real.delay_count    ++;	call.delay_count++;	save.delay_count++;
	rcm_poll.delay_count++;	lcd.cursor_cnt  ++;

	TH0 = 0xF0;		TL0 = 0x00;
	enable(TR0);	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	iRAM_CS();
	if(RI)	{
		disable(RI);
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUF_SIZE)		com1.rx_ep = 0;
	}	else	if(TI)	{
		disable(TI);
		if(com1.tx_sp != com1.tx_ep)	{
			SBUF = com1.tx_buf[com1.tx_sp++];
			if(com1.tx_sp >= BUF_SIZE)	com1.tx_sp = 0;
		} else	com1.tx_flag = ON;
	}
	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt [0x03] void EX0_int (void)
{
	idata byte io_data, io_addr;

	disable(EA);
	iRAM_CS();	io_addr = IIR(com2.base);
	SI2_CS();	io_data = input(io_addr) & 0x6;			iRAM_CS();
	if(0x4 & io_data)	{
		io_addr = RBR(com2.base);
		SI2_CS();	io_data = input(io_addr);			iRAM_CS();
		com2.rx_buf[com2.rx_ep++] = io_data;
		if(com2.rx_ep >= BUF_SIZE)	com2.rx_ep = 0;
	}
	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt [0x13] void EX1_int (void)
{
	idata byte io_data, io_addr;

	disable(EA);
	iRAM_CS();	io_addr = IIR(com3.base);
	SI3_CS();	io_data = input(io_addr) & 0x6;			iRAM_CS();
	if(0x4 & io_data)	{
		io_addr = RBR(com3.base);
		SI3_CS();	io_data = input(io_addr);			iRAM_CS();
		com3.rx_buf[com3.rx_ep++] = io_data;
		if(com3.rx_ep >= BUF_SIZE)	com3.rx_ep = 0;
	}
	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
