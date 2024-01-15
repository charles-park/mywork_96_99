//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define		INTERNAL			0
#define		EXTERNAL			1
//[*]---------------------------------------------------------------------[*]
#define		DATABIT7			0x2
#define		DATABIT8			0x3
//[*]---------------------------------------------------------------------[*]
#define		STOP1				0x0
#define		STOP2				0x4
//[*]---------------------------------------------------------------------[*]
#define		PARITY_NO			0x0
#define		PARITY_ODD			0x0
#define		PARITY_EVEN			0x0
//[*]---------------------------------------------------------------------[*]
#define		DLL(x)				(x+0)		// 16C450 DLL Register
#define		DLM(x)				(x+1)       // 16C450 DLM Register
#define		THR(x)				(x+0)       // 16C450 THR Register
#define		RBR(x)				(x+0)       // 16C450 RBR Register
#define		IER(x)				(x+1)       // 16C450 IER Register
#define		IIR(x)				(x+2)       // 16C450 IIR Register
#define		LCR(x)				(x+3)       // 16C450 LCR Register
#define		MCR(x)				(x+4)       // 16C450 MCR Register
#define		LSR(x)				(x+5)       // 16C450 LSR Register
#define		MSR(x)				(x+6)       // 16C450 MSR Register
//[*]---------------------------------------------------------------------[*]
#define		BUF_SIZE			2000
//[*]---------------------------------------------------------------------[*]
#define		M11					0
#define		M22					1
#define		M3					2
#define		M1					3
//[*]---------------------------------------------------------------------[*]
#define		BPS9600				0
#define		BPS4800				1
#define		BPS2400				2
#define		BPS1200				3
//[*]---------------------------------------------------------------------[*]
#define		SMOD_ON				1
#define		SMOD_OFF			0
#define		NOT_USED			0
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
typedef struct send_to_sam__t	{
	word			header;
	byte			cmd;
	byte			cid;
	word			mini;
	word			maxi;
	byte			chk_xor;
	word			tail;
}	send_to_sam_t;

typedef union send_to_sam__u	{
	send_to_sam_t	send;
	byte			bytes[sizeof(send_to_sam_t)];
}	send_to_sam_u;

static	send_to_sam_u	sam_tx;

//[*]---------------------------------------------------------------------[*]
typedef struct receive_sam__t	{
	word			header;
	byte			cmd;
	byte			sensor_status;
	byte			sensor_code;
	byte			sensor_id;
	word			aver;
	word			spec;
	word			mini;
	word			maxi;
	byte			chk_xor;
	word			tail;
}	receive_sam_t;

typedef union receive_sam__u	{
	receive_sam_t	rece;
	byte			bytes[sizeof(receive_sam_t)];
}	receive_sam_u;

static	receive_sam_u	sam_rx;

//[*]---------------------------------------------------------------------[*]
typedef struct	host__t	{
	unsigned int	header;
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned int	station_id;
	unsigned int	password;
	unsigned char	command;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned int	tail;
}	host_t;

typedef union host__u	{
	host_t			receive;
	unsigned char	bytes[sizeof(host_t)];
}	host_u;

static host_u		host;

//[*]---------------------------------------------------------------------[*]
typedef struct mpm_cfg__t	{
	unsigned long	header;
	unsigned char	rtu_id;
	unsigned char	board_id;
	unsigned char	command;
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
typedef struct ack_mpm_to_host__t	{
	word			header;
	word			logid;
	byte			cmd;
	byte			year;
	byte			month;
	byte			day;
	byte			hour;
	byte			min;
	byte			ack[4];
	byte			chk_xor;
	byte			chk_add;
	word			tail;
}	ack_mpm_to_host_t;

typedef union ack_mpm_to_host__u	{
	ack_mpm_to_host_t		send;
	byte                    bytes[sizeof(ack_mpm_to_host_t)];
}	ack_mpm_to_host_u;

static ack_mpm_to_host_u	ack_mpm_to_host;

//[*]---------------------------------------------------------------------[*]
typedef struct rom_version__t	{
	word			header;
	word			log_id;
	byte			gubun;
	byte			year;
	byte			month;
	byte			day;
	byte			hour;
	byte			min;
	byte			str[20];
	byte			chk_xor;
	byte			chk_add;
	word			tail;
}	rom_version_t;

typedef union rom_version__u	{
	rom_version_t		send;
	byte				bytes[sizeof(rom_version_t)];
}	rom_version_u;

static rom_version_u	rom_version;

//[*]---------------------------------------------------------------------[*]
typedef struct volt_bit__t	{
	word			pv5_1 	:1;
	word			pv5_2  	:1;
	word			pv12_1 	:1;
	word			pv12_2 	:1;
	word			mv12_1 	:1;
	word			mv12_2 	:1;
	word			battery	:1;
	word			ac_volt	:1;
	word			fan    	:1;
	word			door   	:1;
	word			wd     	:1;
	word			ws     	:1;
	word			ped    	:1;
	word			prs    	:1;
	word			rh     	:1;
	word			tsp    	:1;
}	volt_bit_t;

typedef union volt_bit__u	{
	volt_bit_t		bits;
	word  			word_data;
}	volt_bit_u;
//[*]---------------------------------------[*]
typedef struct board_bit__t	{
	byte			rcm		:1;
	byte			mpm		:1;
	byte			mem		:1;
	byte			cpm		:1;
	byte			nc		:4;
}	board_bit_t;

typedef union board_bit__u	{
	board_bit_t		bits;
	byte			byte_data;
}	board_bit_u;
//[*]---------------------------------------[*]
typedef struct volt_data__t	{
	word			pv5_1;
	word			pv5_2;
	word			pv12_1;
	word			pv12_2;
	word			mv12_1;
	word			mv12_2;
	word			battery;
	word			ac_volt;
	word			fan;
	word			door;
	word			wd;
	word			ws;
	word			ped;
	word			prs;
	word			rh;
	word			tsp;	// 32 Byte
}	volt_data_t;

typedef union volt_data__u	{
	volt_data_t		detail;
	byte			bytes[sizeof(volt_data_t)];
}	volt_data_u;
//[*]---------------------------------------[*]
typedef struct board_status__t	{
	volt_bit_u		volt_sta;
	byte			sen_board[4];
	board_bit_u		cnt_board;
	byte			sen_sta[4];
	volt_data_u		volt;
}	board_status_t;

typedef struct board_status__u	{
	board_status_t	rtu;
	byte			bytes[sizeof(board_status_t)];
}	board_status_u;

static board_status_u	status;

//[*]---------------------------------------------------------------------[*]
typedef struct status__t	{
	volt_bit_u		volt_sta;
	byte			sen_board[4];
	board_bit_u		cnt_board;
	byte			sen_sta[4];
}	status_t;

typedef struct send_to_rcm__t	{
	word				header;
	byte				cmd;
	byte				sen_code;
	word				log_id;
	byte				year;
	byte				month;
	byte				day;
	byte				hour;
	byte				min;
	byte				sec;
	status_t			sta;
	byte				chk_xor;
	word				tail;
}	send_to_rcm_t;

typedef union send_to_rcm__u	{
	send_to_rcm_t		send;
	byte				bytes[sizeof(send_to_rcm_t)];
}	send_to_rcm_u;

static	send_to_rcm_u	rcm;

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

static led_t *led_table;

//[*]---------------------------------------------------------------------[*]
typedef struct data_field__t	{
	float		d1;
	float		d2;
	float		d3;
	float		d4;
}	data_field_t;

typedef union data_field__u		{
	data_field_t	float_data;
	byte		    bytes[sizeof(data_field_t)];
}	data_field_u;

typedef struct cpm_real_send__t	{
	unsigned long	header;
	byte			cmd;
	byte			jr;
	byte			gubun;
	byte			scode;
	byte            hung;
	data_field_u    d;
	byte			chk_xor;
	unsigned long	tail;
}	cpm_real_send_t;

typedef union cpm_real_send__u	{
	cpm_real_send_t		field;
	byte				bytes[sizeof(cpm_real_send_t)];
}	cpm_real_send_u;

static cpm_real_send_u	cpm;

//[*]---------------------------------------------------------------------[*]
typedef struct cpm_cfg__t	{
	unsigned long	header;
	byte			cmd;
	byte			sensor_su;
	byte			code_id[200];
	byte			chk_xor;
	byte			chk_add;
	unsigned long	tail;
}	cpm_cfg_t;

typedef union cpm_cfg__u	{
	cpm_cfg_t		send;
	byte			bytes[sizeof(cpm_cfg_t)];
}	cpm_cfg_u;

static cpm_cfg_u	cpm_cfg;

//[*]---------------------------------------------------------------------[*]
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
	byte	tx_buf[BUF_SIZE];
	byte	rx_buf[BUF_SIZE];
	byte	pcnt;
	byte	type;
	word	base;
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	com1, com2, com3;

//[*]---------------------------------------------------------------------[*]
static	byte	inter_baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	byte	exter_baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt		void T0_int   (void);
interrupt[0x03]	void EX0_int  (void);	// External Interrupt 0
interrupt[0x13]	void EX1_int  (void);	// External Interrupt 1
interrupt[0x23]	void SCON_int (void);

void initiliaze(void);
void init_serial_grp(protocol_grp_t *comx, byte mode, word b_addr, word speed,
					 byte crystal, byte smod, byte sta, byte protocol_count);
void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
					 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
void protocol_event(protocol_grp_t *comx, byte com_data);
void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);
void receive_check__1(void);
void receive_check_23(void);
byte rx1_rcm_sam_check(variable_t *var);
byte rx1_sam_catch(variable_t *var);
void sam_data_catch(void);
byte rx1_rcm_catch(variable_t *var);
byte rx2_cmd_check(variable_t *var);
byte rx2_cmd_catch(variable_t *var);
byte rx2_cfg_check(variable_t *var);
byte rx2_cfg_catch(variable_t *var);
byte rx3_cpm_check(variable_t *var);
byte rx3_cpm_catch(variable_t *var);
void transmit_check__1(void);
void transmit_check_23(void);
void status_send_to_rcm(protocol_grp_t *comx);
void send_to_sam_call(protocol_grp_t *comx, byte call_id, byte gubun);
void send_to_host_ack(protocol_grp_t *comx, byte bunru);
void sam_data_send(protocol_grp_t *comx, byte gubun, byte comd);
void rom_version_send(protocol_grp_t *comx, byte cmd);
void send_to_led_data(protocol_grp_t *comx);
void send_to_cpm_cfg(protocol_grp_t *comx);
void message_send(protocol_grp_t *comx, char *buf);
void cpm_run_send(void);
void real_send_to_cpm(byte sen_id, byte sen_code, word value);
void cpm_sensor_sta_send(byte idx);
void cpm_module_sta_send(byte gubun);
void cpm_chk_voltage_send(byte gubun);
void cpm_setup_send(byte gubun);
void cfg_send_to_pc(void);
void send_data(protocol_grp_t *comx, byte move_data);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	Port initial funtion
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void initiliaze(void)
{
	init_serial_grp(&com1, INTERNAL, 0x0000, BPS9600, M11, SMOD_ON, NOT_USED, 2);
	init_serial_pro(&com1, 0, 49, rx1_rcm_sam_check, rx1_rcm_catch);
	init_serial_pro(&com1, 1, 17, rx1_rcm_sam_check, rx1_sam_catch);
	set_protocol_flag(&com1, 0, ON);
	set_protocol_flag(&com1, 1, ON);

	init_serial_grp(&com2, EXTERNAL, 0x8000, BPS9600, M1, NOT_USED, (STOP1|PARITY_NO|DATABIT8), 2);
	init_serial_pro(&com2, 0,  17, rx2_cmd_check, rx2_cmd_catch);
	init_serial_pro(&com2, 1, 218, rx2_cfg_check, rx2_cfg_catch);
	set_protocol_flag(&com2, 0, ON);
	set_protocol_flag(&com2, 1, ON);

	init_serial_grp(&com3, EXTERNAL, 0x9000, BPS9600, M1, NOT_USED, (STOP1|PARITY_NO|DATABIT8), 1);
	init_serial_pro(&com3, 0, 19, rx3_cpm_check, rx3_cpm_catch);
	set_protocol_flag(&com3, 0, ON);

	enable(EA);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
					  byte crystal, byte smod, byte sta, byte protocol_count)
{
	idata byte	baud_value = 0, status = 0;
	idata word	baud	   = 0, base_addr = 0;

	disable(EA);
	comx->base    = b_addr;
	comx->type    = mode;
	comx->pcnt    = protocol_count;
	comx->rx_sp   = comx->rx_ep = comx->tx_sp = comx->tx_ep = 0;
	comx->p       = (protocol_t *)(malloc( sizeof(protocol_t) * comx->pcnt));
	comx->tx_flag = ON;

	if(comx->type == INTERNAL)	{
		baud_value += ((crystal == M22) ? 1:0);

		if(smod)	{	PCON |= 0x80;	baud_value += 1; }
		else		{	PCON &= 0x7F;	baud_value += 0; }
		switch(speed)	{
			case	BPS9600:    baud_value += 0;
								TH1 = inter_baud[baud_value];	break;
			case	BPS4800:    baud_value += 1;
								TH1 = inter_baud[baud_value];	break;
			case	BPS2400:    baud_value += 2;
								TH1 = inter_baud[baud_value];	break;
			case	BPS1200:    baud_value += 3;
								TH1 = inter_baud[baud_value];	break;
		}
		TMOD |= 0x21;
		SCON |= 0x52;
		// Interrupt enable register(IE Register)
		enable (ES );
		disable(ET1);
		enable (ET0);
		enable (EX1);
		enable (EX0);
		// TCON Register initial
		enable (TR1);
		enable (TR0);
		disable(IT1);							// 1 = Low edge triger int.
		enable (IT0);                           // 0 = Low lebel int.

		enable (PS );

	} else	{
		baud_value += ((crystal == M3) ? 4:0);
		switch(speed)	{
			case	BPS9600:    baud_value += 0;
								baud = exter_baud[baud_value];	break;
			case	BPS4800:    baud_value += 1;
								baud = exter_baud[baud_value];	break;
			case	BPS2400:    baud_value += 2;
								baud = exter_baud[baud_value];	break;
			case	BPS1200:    baud_value += 3;
								baud = exter_baud[baud_value];	break;
		}
		base_addr = comx->base;					// move xdata->data (local)
		status    = sta       ;					// move xdata->data (local)

		if(comx->base == 0x8000)	{
			sio2_cs();						// external com2 port select
			  output( LCR(base_addr),(0x80 | status));
			  output( DLL(base_addr),        baud   );
			  output( DLM(base_addr), 0x00          );
			  output( LCR(base_addr),(0x7F & status));
			  output( IER(base_addr), 0x01          );
			  output( MCR(base_addr), 0x0b          );
			iram_cs();						// base ram select (0~0x7fff)
		} else	if(comx->base == 0x9000)	{
			sio3_cs();						// external com2 port select
			  output( LCR(base_addr),(0x80 | status));
			  output( DLL(base_addr),        baud   );
			  output( DLM(base_addr), 0x00          );
			  output( LCR(base_addr),(0x7F & status));
			  output( IER(base_addr), 0x01          );
			  output( MCR(base_addr), 0x0b          );
			iram_cs();						// base ram select (0~0x7fff)
		}
	}
	iram_cs();
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
	comx->p[protocol_number].var.p_ep %= comx->p[protocol_number].var.size;
	if(comx->p[protocol_number].var.p_ep == comx->p[protocol_number].var.p_sp)
	{
		comx->p[protocol_number].var.p_sp++;
		comx->p[protocol_number].var.p_sp %= comx->p[protocol_number].var.size;
	}
	comx->p[protocol_number].var.buf[comx->p[protocol_number].var.p_ep++] = com_data;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	Receive check funtion.
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void receive_check__1(void)
{
	if(com1.rx_sp != com1.rx_ep)	{
		led_disp(COM1,RXD,ON);
		protocol_event(&com1,com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= BUF_SIZE)	com1.rx_sp = 0;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void receive_check_23(void)
{
	if(com2.rx_sp != com2.rx_ep)	{
		led_disp(COM2,RXD,ON);
		protocol_event(&com2,com2.rx_buf[com2.rx_sp++]);
		if(com2.rx_sp >= BUF_SIZE)	com2.rx_sp = 0;
	}
	if(com3.rx_sp != com3.rx_ep)	{
		led_disp(COM3,RXD,ON);
		protocol_event(&com3,com3.rx_buf[com3.rx_sp++]);
		if(com3.rx_sp >= BUF_SIZE)	com3.rx_sp = 0;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx1_rcm_sam_check(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)    return 0;
	led_disp(COM1, RXD, OFF);
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx1_sam_catch(variable_t *var)
{
	byte val, count, id, chk_xor = 0;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		sam_rx.bytes[count] = val;
		if(count > 1 && count < var->size - 3)
			chk_xor ^= val;
	}
	if(sam_rx.rece.chk_xor != chk_xor)								return 0;
	if(sam_rx.rece.sensor_id >= i_data.sensor_su)					return 0;
	id = sam_rx.rece.sensor_id;
	switch(sam_rx.rece.cmd)	{
		case '$'	: max_min_load(id);
					  send_to_sam_call(&com1, id, '@');				break;
		default		: sam_data_catch();								break;
	}
																	return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sam_data_catch(void)
{
	byte p, id;

	id = sam_rx.rece.sensor_id;
	if(sam_rx.rece.cmd == real.real_command)	{			// Real data
		led_table[id + 2].cmd         = sam_rx.rece.cmd;
		led_table[id + 2].status      = sam_rx.rece.sensor_status;
		led_table[id + 2].sensor_code = sam_rx.rece.sensor_code;
		led_table[id + 2].sensor_id   = sam_rx.rece.sensor_id;
		led_table[id + 2].aver		  = sam_rx.rece.aver;
		led_table[id + 2].spec		  = sam_rx.rece.spec;
		led_table[id + 2].mini		  = sam_rx.rece.mini;
		led_table[id + 2].maxi		  = sam_rx.rece.maxi;
		p = REAL;
		sensor_status_set(sam_rx.rece.sensor_id, sam_rx.rece.sensor_status);
		real.receive_status = ON;
		if((cpm_st.flag == ON) && (id == cpm_st.call_id))	{
			real_send_to_cpm(id, sam_rx.rece.sensor_code, sam_rx.rece.aver);
			cpm_st.receive_status = ON;
		}
	} else	if(sam_rx.rece.cmd == call.call_command)	{	// Call data
		p = CALL;
		call.receive_status = ON;
	} else	if(sam_rx.rece.cmd == save.save_command)	{	// Save data
		p = SAVE;
		save.receive_status = ON;
	}
	weather[p].year  = rtc.year;	weather[p].month = rtc.month;
	weather[p].day   = rtc.day ;	weather[p].hour  = rtc.hour ;
	weather[p].min   = rtc.min ;	weather[p].sec   = rtc.sec  ;
	weather[p].sensor[id].sensor_status	= sam_rx.rece.sensor_status;
	weather[p].sensor[id].sensor_code	= sam_rx.rece.sensor_code;
	weather[p].sensor[id].sensor_id		= sam_rx.rece.sensor_id;
	weather[p].sensor[id].aver			= sam_rx.rece.aver;
	weather[p].sensor[id].spec			= sam_rx.rece.spec;
	weather[p].sensor[id].mini			= sam_rx.rece.mini;
	weather[p].sensor[id].maxi			= sam_rx.rece.maxi;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx1_rcm_catch(variable_t *var)
{
	byte	ck_xor = 0;
	word	count  = 0;

	if(var->buf[(var->p_sp + 2) % var->size] != 'L')				return 0;

	for(count = 0; count<var->size; count++)	{
		if(count > 1 && count < (var->size -3))
			ck_xor ^= var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -3) % var->size])	return 0;

	status.rtu.volt_sta.word_data  = var->buf[(var->p_sp +3) % var->size] <<8;
	status.rtu.volt_sta.word_data |= var->buf[(var->p_sp +4) % var->size]    ;

	for(count = 0; count < 4; count++)	{
		status.rtu.sen_board[count] = var->buf[(var->p_sp + 5 +count) % var->size];
		status.rtu.sen_sta[count]   = var->buf[(var->p_sp +10 +count) % var->size];
	}
	for(count=0; count<32; count++)
		status.rtu.volt.bytes[count] = var->buf[(var->p_sp +14 +count) % var->size];

	rcm_poll.receive_status = ON;
	status.rtu.cnt_board.byte_data = var->buf[(var->p_sp +9          ) % var->size];
	return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx2_cmd_check(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)    return 0;
	led_disp(COM2, RXD, OFF);
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx2_cmd_catch(variable_t *var)
{
	byte val, cmd, chk_add = 0, chk_xor = 0;
	word count;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		host.bytes[count] = val;
		if(count > 1 && count < var->size - 4)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(host.receive.chk_xor  != chk_xor)							return 0;
	if(host.receive.chk_add  != chk_add)							return 0;
	switch(host.receive.command)	{
		case 'I'  :	if(rtc.year  != host.receive.year )	rtc_set();
					if(rtc.month != host.receive.month)	rtc_set();
					if(rtc.day   != host.receive.day  )	rtc_set();
					if(rtc.hour  != host.receive.hour )	rtc_set();
					if(rtc.min   != host.receive.min  )	rtc_set();
					sam_data_send(&com2, REAL, 'M');				break;
		case 'B'  :	sam_data_send(&com2, CALL, 'O');				break;
		case 'A'  :	cmd = load_data(SEND);
					sam_data_send(&com2, SEND, cmd);				break;
		case 'V'  :	rom_version_send(&com2, 'V');					break;
		case 'D'  :	i_data.station_id = host.receive.station_id;
					mpm_cfg_save(STATION_ID);
					send_to_host_ack(&com2, 'D');					break;
		case 'W'  :	i_data.password = host.receive.password;
					mpm_cfg_save(PASSWORD);
					send_to_host_ack(&com2, 'W');					break;
		case 'T' :  rtc_set();
					send_to_host_ack(&com2,'T');					break;
		case 'C'  :	value.buffer_clr = ON;							break;
		case 'N'  :	cfg_send_to_pc();								break;
		case '#'  :	// offset_user_set();							break;
		case '*'  :	// offset_auto_set();							break;
		default	  :													break;
	}
																	return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx2_cfg_check(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFE)    return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != 0xFE)    return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != 0xFE)    return 0;
	led_disp(COM2, RXD, OFF);
	if(var->buf[(var->p_sp             +3) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp             +2) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx2_cfg_catch(variable_t *var)
{
	byte  cmd_h, cmd_l, chk_add = 0, chk_xor = 0;
	word  count;
	idata byte pos, val;

	for(count=0; count<var->size; count++)	{
		val = var->buf[(var->p_sp + count) % var->size];
		mpm_cfg.bytes[count] = val;
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= val;
			chk_add += val;
		}
	}
	if(mpm_cfg.send.chk_xor  != chk_xor)							return 0;
	if(mpm_cfg.send.chk_add  != chk_add)							return 0;
	if(mpm_cfg.send.command  != 'N')								return 0;
	for(pos=0; pos<200; pos++)	{
		val = mpm_cfg.send.id_code[pos];
		cfg_ram_cs(MPM_CFG);	output(ID_CODE + pos, val);	ram_cs();
	}
	i_data.sensor_su  = mpm_cfg.send.sensor_su ; mpm_cfg_save(SENSOR_SU );
	i_data.call_time  = mpm_cfg.send.schedule_1; mpm_cfg_save(CALL_TIME );	// Call time
	i_data.save_time  = mpm_cfg.send.schedule_2; mpm_cfg_save(SAVE_TIME );	// Save time
	value.config_receive_status = ON;
																	return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx3_cpm_check(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFE)    return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != 0xFE)    return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != 0xFE)    return 0;
	led_disp(COM3, RXD, OFF);
	if(var->buf[(var->p_sp             +3) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp             +2) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte rx3_cpm_catch(variable_t *var)
{
	byte	ck_xor = 0, yh, mm, ds;
	word	count,	vals;
	float	f_data = 0.;
	byte	*p = (byte *)(&f_data);

	for(count = 0; count<var->size; count++)	{
		if(count > 3 && count < (var->size -5))
			ck_xor ^= var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;

	if(var->buf[(var->p_sp + 4) % var->size] == 'X')	{
		value.cancel = ON;											return 0;
	}

	yh = var->buf[(var->p_sp + 7) % var->size];
	mm = var->buf[(var->p_sp + 8) % var->size];
	ds = var->buf[(var->p_sp + 9) % var->size];

	for(count=0; count<4; count++)	{
		p[count] = var->buf[(var->p_sp + 10 + count) % var->size];
	}
	vals = (word)(f_data);
	if(value.config_receive_status != ON)							return 0;
	if(var->buf[(var->p_sp + 4) % var->size] == 'C')	{
		switch(var->buf[(var->p_sp + 5) % var->size])	{
			case 0 :	cpm_st.call_id = var->buf[(var->p_sp + 6) % var->size];
						cpm_st.flag = ON;
						return 0;
			case 1 :	cpm_sensor_sta_send(var->buf[(var->p_sp + 6) % var->size]);
						return 0;
			case 2 :	cpm_module_sta_send(var->buf[(var->p_sp + 6) % var->size]);
						return 0;
			case 3 :	cpm_chk_voltage_send(var->buf[(var->p_sp + 6) % var->size]);
						return 0;
			case 4 :	value.memory_tst = ON;
						return 0;
			case 5 :	cpm_setup_send(var->buf[(var->p_sp + 6) % var->size]);
						return 0;
			case 6 :	send_to_cpm_cfg(&com3);
			default:	return 0;
		}
	}
	if(var->buf[(var->p_sp + 4) % var->size] == 'S')	{
		if(var->buf[(var->p_sp + 5) % var->size] != 0x5)			return 0;
		else	{
			switch(var->buf[(var->p_sp + 6) % var->size])	{
				case 0:	disable(EA);
						rtc.year  = yh;	rtc.month = mm;
						rtc.day   = ds;	rtc_date_write(&rtc);
						ram_cs();									return 0;
				case 1:	disable(EA);
						rtc.hour  = yh;	rtc.min   = mm;
						rtc.sec   = ds;	rtc_time_write(&rtc);
						ram_cs();									return 0;
				case 2:	i_data.station_id = vals;
						mpm_cfg_save(STATION_ID);					return 0;
				case 3:	i_data.password = vals;
						mpm_cfg_save(PASSWORD);						return 0;
				case 4:	// offset function
																	return 0;
				case 5:	value.buffer_clr = ON;						return 0;
				default:											return 0;
			}
		}
	}
	return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	Send funtion.
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void transmit_check__1(void)
{
	if(com1.tx_sp != com1.tx_ep)	{
		if(com1.tx_flag)	{
			disable(EA);
			SBUF = com1.tx_buf[com1.tx_sp++];		ram_cs();
			if(com1.tx_sp >= BUF_SIZE)	com1.tx_sp = 0;
			led_disp(COM1, TXD, ON);
			com1.tx_flag = OFF;
		}
	} else	led_disp(COM1,TXD,OFF);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void transmit_check_23(void)
{
	idata static byte io_data;	  				// p1_port;
	idata static word io_addr;

	if(com2.tx_sp != com2.tx_ep)	{
		io_addr = LSR(com2.base);
		sio2_cs();	io_data = input(io_addr);		ram_cs();
		if(0x20 & io_data)	{
			led_disp(COM2,TXD,ON);
			io_data = com2.tx_buf[com2.tx_sp++];
			io_addr = THR(com2.base);
			sio2_cs();	output(io_addr,io_data);	ram_cs();
			if(com2.tx_sp >= BUF_SIZE)	com2.tx_sp = 0;
		}
	}	else	{
		led_disp(COM2,TXD,OFF);
		ram_cs();
	}
	if(com3.tx_sp != com3.tx_ep)	{
		io_addr = LSR(com3.base);
		sio3_cs();	io_data = input(io_addr);		ram_cs();
		if(0x20 & io_data)	{
			led_disp(COM3,TXD,ON);
			io_data = com3.tx_buf[com3.tx_sp++];
			io_addr = THR(com3.base);
			sio3_cs();	output(io_addr,io_data);	ram_cs();
			if(com3.tx_sp >= BUF_SIZE)	com3.tx_sp = 0;
		}
	}	else	{
		led_disp(COM3,TXD,OFF);
		ram_cs();
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_data(protocol_grp_t *comx, byte move_data)
{
	comx->tx_buf[comx->tx_ep++] = move_data;
	if(comx->tx_ep >= BUF_SIZE)	comx->tx_ep = 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_to_sam_call(protocol_grp_t *comx, byte call_id, byte gubun)
{
	word i;

	sam_tx.send.header  = 0xFFFF;	sam_tx.send.tail = 0xFFFE;
	sam_tx.send.cmd	    = gubun ; 	sam_tx.send.cid  = call_id;
	sam_tx.send.chk_xor = 0;
	for(i=0; i<sizeof(send_to_sam_u); i++)	{
		if(i>1 && i<sizeof(send_to_sam_u)-3)	{
			sam_tx.send.chk_xor ^= sam_tx.bytes[i];
		}
		send_data(comx, sam_tx.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void status_send_to_rcm(protocol_grp_t *comx)
{
	word i;

	rcm.send.header	 = 0xFFFF;		rcm.send.tail	  = 0xFFFE;
	rcm.send.cmd	 = 'L';			rcm.send.sen_code = 0xFA;
	rcm.send.year 	 = rtc.year;	rcm.send.month	  = rtc.month;
	rcm.send.day	 = rtc.day ;	rcm.send.hour	  = rtc.hour;
	rcm.send.min	 = rtc.min ;	rcm.send.sec	  = rtc.sec;
	rcm.send.chk_xor = 0;			rcm.send.log_id   = i_data.station_id;

	rcm.send.sta.volt_sta.word_data = status.rtu.volt_sta.word_data;
	for(i=0; i<4; i++)	{
		rcm.send.sta.sen_board[i] = status.rtu.sen_board[i];
		rcm.send.sta.sen_sta[i]   = status.rtu.sen_sta[i];
	}
	rcm.send.sta.cnt_board.byte_data = status.rtu.cnt_board.byte_data;
	for(i=0; i<sizeof(send_to_rcm_u); i++)	{
		if(i>1 && i<sizeof(send_to_rcm_u)-3)	{
			rcm.send.chk_xor ^= rcm.bytes[i];
		}
		send_data(comx, rcm.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sam_data_send(protocol_grp_t *comx, byte gubun, byte comd)
{
	word i;
	byte *w = (byte	*)(&weather[gubun]);
	byte *s = (byte	*)( weather[gubun].sensor);

	if(gubun == REAL)	{
		weather[gubun].year   = real.year;
		weather[gubun].month  = real.month;
		weather[gubun].day    = real.day;
		weather[gubun].hour   = real.hour;
		weather[gubun].min    = real.min;
		weather[gubun].sec    = real.sec;
	}
	weather[gubun].header 	  = 0xFFFF;
	weather[gubun].station_id = i_data.station_id;
	weather[gubun].cmd		  = comd;
	weather[gubun].chk_xor	  = 0x00;
	weather[gubun].chk_add	  = 0x00;
	for(i=0; i<sizeof(weather_t)-sizeof(sensor_field_t *) -4; i++)	{
		if(i>1)	{
			weather[gubun].chk_xor ^= w[i];
			weather[gubun].chk_add += w[i];
		}
		send_data(comx, w[i]);
	}
	for(i=0; i<i_data.sensor_su * sizeof(sensor_field_t);i++)	{
		weather[gubun].chk_xor ^= s[i];
		weather[gubun].chk_add += s[i];
		send_data(comx, s[i]);
	}
	send_data(comx, weather[gubun].chk_xor);
	send_data(comx, weather[gubun].chk_add);
	send_data(comx, 0xFF);
	send_data(comx, 0xFE);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_to_host_ack(protocol_grp_t *comx, byte bunru)
{
	word i;

	ack_mpm_to_host.send.header  = 0xFFFF;
	ack_mpm_to_host.send.logid   = i_data.station_id;
	ack_mpm_to_host.send.cmd     = bunru;
	ack_mpm_to_host.send.year    = rtc.year;
	ack_mpm_to_host.send.month   = rtc.month;
	ack_mpm_to_host.send.day     = rtc.day;
	ack_mpm_to_host.send.hour    = rtc.hour;
	ack_mpm_to_host.send.min     = rtc.min;
	ack_mpm_to_host.send.chk_xor = 0;
	ack_mpm_to_host.send.chk_add = 0;
	ack_mpm_to_host.send.tail    = 0xFFFE;
	strcpy(ack_mpm_to_host.send.ack, "OKAY");

	if		  (bunru == 'N')	strcpy(ack_mpm_to_host.send.ack, "FAIL");
	else	if(bunru == 'n')	ack_mpm_to_host.send.cmd = 'N';

	for(i=0; i<sizeof(ack_mpm_to_host_u); i++)	{
		if(i>1 && i<sizeof(ack_mpm_to_host_u) -4)	{
			ack_mpm_to_host.send.chk_xor ^= ack_mpm_to_host.bytes[i];
			ack_mpm_to_host.send.chk_add += ack_mpm_to_host.bytes[i];
		}
		send_data(comx, ack_mpm_to_host.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rom_version_send(protocol_grp_t *comx, byte cmd)
{
	word i;

	rom_version.send.header	 = 0xFFFF;
	rom_version.send.log_id  = i_data.station_id;
	rom_version.send.gubun   = cmd;
	rom_version.send.year	 = rtc.year;
	rom_version.send.month	 = rtc.month;
	rom_version.send.day	 = rtc.day;
	rom_version.send.hour	 = rtc.hour;
	rom_version.send.min	 = rtc.min;
	rom_version.send.chk_xor = 0x00;
	rom_version.send.chk_add = 0x00;
	rom_version.send.tail	 = 0xFFFE;

	strcpy(rom_version.send.str,"JY-AWS VER : 9706.12");
	for(i=0; i<sizeof(rom_version_u); i++)	{
		if(i>1 && i<(sizeof(rom_version_u) -4))	{
			rom_version.send.chk_xor ^= rom_version.bytes[i];
			rom_version.send.chk_add += rom_version.bytes[i];
		}
		send_data(comx, rom_version.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void cfg_send_to_pc(void)
{
	idata byte read_data;
	idata word i;

	mpm_cfg_load(SENSOR_SU);
	mpm_cfg_load(CALL_TIME);
	mpm_cfg_load(SAVE_TIME);

	mpm_cfg.send.header		= 0xFFFFFFFF;
	mpm_cfg.send.rtu_id		= 0;
	mpm_cfg.send.board_id	= 0;
	mpm_cfg.send.command	= 'N';

	mpm_cfg.send.sensor_su	= i_data.sensor_su;
	mpm_cfg.send.schedule_1	= i_data.call_time;
	mpm_cfg.send.schedule_2	= i_data.save_time;
	mpm_cfg.send.chk_xor	= 0;
	mpm_cfg.send.chk_add	= 0;
	mpm_cfg.send.tail		= 0xFEFEFEFE;

	for(i=0; i<200; i++)	{
		cfg_ram_cs(MPM_CFG);	read_data = input(ID_CODE + i);
		ram_cs();				mpm_cfg.send.id_code[i] = read_data;
	}
	for(i=0; i<sizeof(mpm_cfg_u); i++)	{
		if(i>3 && i<sizeof(mpm_cfg_u)-6)	{
			mpm_cfg.send.chk_xor ^= mpm_cfg.bytes[i];
			mpm_cfg.send.chk_add += mpm_cfg.bytes[i];
		}
		send_data(&com2, mpm_cfg.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_to_led_data(protocol_grp_t *comx)
{
	word date_value, i;
	byte *p, su, chk_xor = 0;

	if(rtc.year < 90) date_value  = 2000;
	else              date_value  = 1900;

	led_table[0].cmd         = 'N';				// Date command
	led_table[0].status      = 0;
	led_table[0].sensor_code = 0;
	led_table[0].sensor_id   = 0;
	led_table[0].aver        = rtc.year + date_value;
	led_table[0].spec        = rtc.month;
	led_table[0].mini        = rtc.day;
	led_table[0].maxi        = i_data.station_id;

	led_table[1].cmd         = 'S';				// Time command
	led_table[1].status      = 0;
	led_table[1].sensor_code = 0;
	led_table[1].sensor_id   = 0;
	led_table[1].aver        = rtc.hour;
	led_table[1].spec        = rtc.min;
	led_table[1].mini        = rtc.sec;
	led_table[1].maxi        = i_data.station_id;

	for(su=0; su<4; su++)	send_data(comx, 0xFF);
	send_data(comx, 'L');
	chk_xor ^= 'L';
	for(su=0; su<i_data.sensor_su + 2; su++)	{
		led_table[su].header  = 0xFFFF;
		led_table[su].tail    = 0xFFFE;
		led_table[su].chk_xor = 0;
		p = (char *)(&led_table[su]);
		for(i=0; i<sizeof(led_t); i++)	{
			if(i>1 && i<sizeof(led_t)-3)	led_table[su].chk_xor ^= p[i];
			chk_xor ^= p[i];
			send_data(comx, p[i]);
		}
	}
	send_data(comx, chk_xor);
	for(su=0; su<4; su++)	send_data(comx, 0xFE);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_to_cpm_cfg(protocol_grp_t *comx)
{
	idata word i;
	idata byte read_data;

	cpm_cfg.send.header	   = 0xFFFFFFFF;
	cpm_cfg.send.cmd	   = 'C';
	cpm_cfg.send.sensor_su = i_data.sensor_su;
	cpm_cfg.send.chk_xor   = 0;
	cpm_cfg.send.chk_add   = 0;
	cpm_cfg.send.tail      = 0xFEFEFEFE;

	for(i=0; i<200; i++)	{
		cfg_ram_cs(MPM_CFG);	read_data = input(ID_CODE + i);
		ram_cs();				cpm_cfg.send.code_id[i] = read_data;
	}
	for(i=0; i<sizeof(cpm_cfg_t); i++)	{
		if(i>3 && i<sizeof(cpm_cfg_t) -6)	{
			cpm_cfg.send.chk_xor ^= cpm_cfg.bytes[i];
			cpm_cfg.send.chk_add += cpm_cfg.bytes[i];
		}
		send_data(comx, cpm_cfg.bytes[i]);
	}
	while(com3.tx_sp != com3.tx_ep)	transmit_check_23();
	transmit_check_23();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void message_send(protocol_grp_t *comx, char *buf)
{
	word i;

	cpm.field.header = 0xFFFFFFFF;	cpm.field.tail    = 0xFEFEFEFE;
	cpm.field.cmd    = 'l';			cpm.field.jr      = 'A';
	cpm.field.gubun  = 0;			cpm.field.hung    = 0;
	cpm.field.scode  = 0;			cpm.field.chk_xor = 0;

	for(i=0; i<16; i++)	cpm.field.d.bytes[i] = buf[i];
	for(i=0; i<sizeof(cpm_real_send_u); i++)	{
		if(i>3 && i<sizeof(cpm_real_send_u) -5)	cpm.field.chk_xor ^= cpm.bytes[i];
		send_data(comx, cpm.bytes[i]);
	}
	while(com3.tx_sp != com3.tx_ep)	transmit_check_23();
	transmit_check_23();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void cpm_run_send(void)
{
	word	i;

	cpm.field.header = 0xFFFFFFFF ;	cpm.field.tail   = 0xFEFEFEFE ;
	cpm.field.cmd    = 'l'		  ;	cpm.field.jr	 = 'R'		  ;
	cpm.field.gubun  = 0          ;	cpm.field.hung   = 0          ;
	cpm.field.scode  = 0          ; cpm.field.chk_xor    = 0		  ;

	for(i=0; i<sizeof(data_field_u); i++)	cpm.field.d.bytes[i] = 0;

	for(i=0; i<sizeof(cpm_real_send_u); i++)	{
		if(i>3 && i<sizeof(cpm_real_send_u) -6)	cpm.field.chk_xor ^= cpm.bytes[i];
		send_data(&com3, cpm.bytes[i]);
	}
	while(com3.tx_sp != com3.tx_ep)	transmit_check_23();
	transmit_check_23();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void real_send_to_cpm(byte sen_id, byte sen_code, word value)
{
	word i;

	for(i=0; i<sizeof(data_field_u); i++)	cpm.field.d.bytes[i] = 0;

	cpm.field.header = 0xFFFFFFFF;	cpm.field.tail    = 0xFEFEFEFE;
	cpm.field.cmd    = 'l';			cpm.field.jr	  = 0;
	cpm.field.gubun  = sen_id;		cpm.field.hung    = 'F';
	cpm.field.scode  = sen_code;	cpm.field.chk_xor = 0;

	cpm.field.d.float_data.d1 = (float)(value);

	for(i=0; i<sizeof(cpm_real_send_u); i++)	{
		if(i>3 && i<sizeof(cpm_real_send_u) -6)	cpm.field.chk_xor ^= cpm.bytes[i];
		send_data(&com3, cpm.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void cpm_sensor_sta_send(byte idx)
{
	word i;
	byte sta_check = 0x01;
	byte arrary_mok, arrary_na, a_data;

	for(i=0; i<sizeof(data_field_u); i++)	cpm.field.d.bytes[i] = 0;
	cpm.field.header  = 0xFFFFFFFF;	cpm.field.tail = 0xFEFEFEFE;
	cpm.field.cmd     = 'l';		cpm.field.jr   = 1;
	cpm.field.gubun   = idx;		cpm.field.hung = 'a';
	cpm.field.scode   = 0;
	cpm.field.chk_xor = 0;
	if(idx == 0)	{ arrary_mok = 0;	arrary_na = 0;			}
	else			{
		arrary_mok = idx / 8;
		arrary_na = idx % 8;
		sta_check = sta_check << arrary_na;
	}
	a_data = status.rtu.sen_sta[arrary_mok];
	if(!(a_data & sta_check))	strcpy(cpm.field.d.bytes,"NORMAL  ");
	else						strcpy(cpm.field.d.bytes,"FAIL    ");
	for(i=0; i<sizeof(cpm_real_send_u); i++)	{
		if(i>3 && i<sizeof(cpm_real_send_u) -6)	cpm.field.chk_xor ^= cpm.bytes[i];
		send_data(&com3, cpm.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void cpm_module_sta_send(byte gubun)
{
	word i;
	byte sta_check  = 0x01;
	byte arrary_mok = 0, arrary_na = 0, a_data = 0;

	cpm.field.header  = 0xFFFFFFFF;	cpm.field.tail    = 0xFEFEFEFE;
	cpm.field.cmd     = 'l';		cpm.field.jr	  = 2;
	cpm.field.gubun   = gubun;		cpm.field.hung    = 'a';
	cpm.field.scode   = 0;			cpm.field.chk_xor = 0;
	for(i=0; i<sizeof(data_field_u); i++)	cpm.field.d.bytes[i] = 0;
	if(gubun < 4)	{
		switch(gubun)	{
			case 0:	if(status.rtu.cnt_board.bits.mpm == NORMAL)
							strcpy(cpm.field.d.bytes,"NORMAL  ");
					else	strcpy(cpm.field.d.bytes,"FAIL    ");	break;
			case 1:	if(status.rtu.cnt_board.bits.mem == NORMAL)
							strcpy(cpm.field.d.bytes,"NORMAL  ");
					else	strcpy(cpm.field.d.bytes,"FAIL    ");	break;
			case 2:	strcpy(cpm.field.d.bytes,"NORMAL  ");			break;
			case 3:	if(status.rtu.cnt_board.bits.rcm == NORMAL)
							strcpy(cpm.field.d.bytes,"NORMAL  ");
					else	strcpy(cpm.field.d.bytes,"FAIL    ");	break;
			default:		strcpy(cpm.field.d.bytes,"NORMAL  ");	break;
		}
	} else	{
		gubun = gubun - 4;
		if(gubun == 0)	{ arrary_mok = 0;	arrary_na = 0;}
		else	{
			arrary_mok = gubun / 8;	arrary_na = gubun % 8;
			sta_check = (sta_check << arrary_na);
		}
		a_data = status.rtu.sen_board[arrary_mok];
		if(!(a_data & sta_check))		strcpy(cpm.field.d.bytes,"NORMAL  ");
		else							strcpy(cpm.field.d.bytes,"FAIL    ");
	}
	for(i=0; i<sizeof(cpm_real_send_u); i++)	{
		if(i>3 && i<sizeof(cpm_real_send_u) -6)	cpm.field.chk_xor ^= cpm.bytes[i];
		send_data(&com3, cpm.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void cpm_chk_voltage_send(byte gubun)
{
	word i;

	cpm.field.header = 0xFFFFFFFF;	cpm.field.tail    = 0xFEFEFEFE;
	cpm.field.cmd    = 'l';			cpm.field.jr      = 3;
	cpm.field.gubun  = gubun;		cpm.field.hung    = 'F';
	cpm.field.scode  = 0;			cpm.field.chk_xor = 0;

	for(i=0; i<sizeof(data_field_u); i++)	cpm.field.d.bytes[i] = 0;
	switch(gubun)	{
		case 0:	cpm.field.d.float_data.d1 = (float)(status.rtu.volt.detail.pv5_1);
				break;
		case 1:	cpm.field.d.float_data.d1 = (float)(status.rtu.volt.detail.pv12_1);
				break;
		case 2:	cpm.field.d.float_data.d1 = (float)(status.rtu.volt.detail.mv12_1);
				break;
		case 3:	cpm.field.d.float_data.d1 = (float)(status.rtu.volt.detail.ac_volt);
				break;
		case 4:	cpm.field.d.float_data.d1 = (float)(status.rtu.volt.detail.battery);
				break;
	}
	for(i=0; i<sizeof(cpm_real_send_u); i++)	{
		if(i>3 && i<sizeof(cpm_real_send_u) -6)	cpm.field.chk_xor ^= cpm.bytes[i];
		send_data(&com3, cpm.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void cpm_setup_send(byte gubun)
{
	word  i;

	cpm.field.header = 0xFFFFFFFF;	cpm.field.tail    = 0xFEFEFEFE;
	cpm.field.cmd    = 'l';			cpm.field.jr	  = 5;
	cpm.field.gubun  = gubun;		cpm.field.hung    = 'F';
	cpm.field.scode  = 0; 			cpm.field.chk_xor = 0;

	for(i=0; i<sizeof(data_field_u); i++)	cpm.field.d.bytes[i] = 0;
	switch(gubun)	{
		case 0:	cpm.field.d.bytes[0] = rtc.year;
				cpm.field.d.bytes[1] = rtc.month;
				cpm.field.d.bytes[2] = rtc.day;
				break;
		case 1:	cpm.field.d.bytes[0] = rtc.hour;
				cpm.field.d.bytes[1] = rtc.min;
				cpm.field.d.bytes[2] = rtc.sec;
				break;
		case 2:	cpm.field.d.float_data.d1 = (float)(i_data.station_id);
				break;
		case 3:	cpm.field.d.float_data.d1 = (float)(i_data.password);
				break;
	}
	for(i=0; i<sizeof(cpm_real_send_u); i++)	{
		if(i>3 && i<sizeof(cpm_real_send_u) -6)	cpm.field.chk_xor ^= cpm.bytes[i];
		send_data(&com3, cpm.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	Timer 0 interrupt
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt void T0_int(void)         // Timer 0 interrupt vector
{
	idata byte yy, mm, dd, hh, bb, ss, dt_flag;

	disable(EA);	disable(TR0);
	dt_flag = 1;
	rtc_cs();
	 while(input(RTC_ADD_A) & 0x80);
	 yy = input(RTC_YEA);	mm = input(RTC_MON);	dd = input(RTC_DAY);
	 hh = input(RTC_HOU);	bb = input(RTC_MIN);	ss = input(RTC_SEC);
	iram_cs();
	if((yy > 99)||(mm > 12)||(mm < 1)||(dd > 31)||(dd < 1))	dt_flag = 0;
	if((hh > 24)||(bb > 59)||(ss > 59))						dt_flag = 0;
	if(dt_flag)	{
		rtc.year = yy;	rtc.month = mm;	rtc.day = dd;
		rtc.hour = hh;	rtc.min   = bb;	rtc.sec = ss;
	}
	if(real.sec != rtc.sec)	{
		rtc.check_count = 0;
		real.year = rtc.year;		real.month  = rtc.month;
		real.day  = rtc.day ;		real.hour   = rtc.hour ;
		real.min  = rtc.min ;		real.sec    = rtc.sec  ;
		if((real.sec % 10) == 5)	real.flag   = ON;	// real processing
	}
	if(i_data.call_time != 0)	{
		if(i_data.call_time < 60)	{
			if(((real.sec % call.call_sec)==0)&&(call.sec != real.sec))	{
				call.year = real.year;	call.month = real.month;
				call.day  = real.day ;	call.hour  = real.hour ;
				call.min  = real.min ;	call.sec   = real.sec  ;
				call.flag = ON;							// call processing
			}
		} else	if(((real.min % call.call_min)==0)&&(call.min != real.min))	{
			call.year = real.year;	call.month = real.month;
			call.day  = real.day ;	call.hour  = real.hour ;
			call.min  = real.min ;	call.sec   = 0;
			call.flag = ON;								// call processing
		}
	}
	if(i_data.save_time != 0)	{
		if(i_data.save_time < 60)	{
			if(((real.sec % save.save_sec)==0)&&(real.sec != save.sec))	{
				save.year = real.year;	save.month = real.month;
				save.day  = real.day ;	save.hour  = real.hour ;
				save.min  = real.min ;	save.sec   = real.sec  ;
				save.flag = ON;							// save processing
			}
		} else	if(((real.min % save.save_min)==0)&&(save.min != real.min))	{
			save.year = real.year;	save.month = real.month;
			save.day  = real.day ;	save.hour  = real.hour ;
			save.min  = real.min ;	save.sec   = 0;
			save.flag = ON;								// save processing
		}
	}
	real.delay_count  ++;	call.delay_count    ++;
	save.delay_count  ++;	rcm_poll.delay_count++;
	cpm_st.delay_count++;	rtc.sqw_status_count++;
	rtc.check_count   ++;

	enable(TR0);	ram_cs();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	4Hz Processing function
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt [0x03] void EX0_int (void)    /* External Interrupt 0 */
{
	disable(EA);
		rtc.sqw_status_count = 0;
	ram_cs();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt [0x13] void EX1_int (void)
{
	idata byte	base_addr = 0, com_data = 0;

	disable(EA);
	base_addr = IIR(com2.base);
	sio2_cs();	com_data  = input(base_addr);			iram_cs();
	if(0x4 & (com_data & 0x6))	{
		base_addr = RBR(com2.base);
		sio2_cs();	com_data  =	input(base_addr);		iram_cs();
		com2.rx_buf[com2.rx_ep++] =	com_data;
		if(com2.rx_ep >= BUF_SIZE)		com2.rx_ep = 0;
	} else 	{
		base_addr = IIR(com3.base);
		sio3_cs();	com_data  = input(base_addr);		iram_cs();
		if(0x4 & (com_data & 0x6))	{
			base_addr = RBR(com3.base);
			sio3_cs();	com_data  = input(base_addr);	iram_cs();
			com3.rx_buf[com3.rx_ep++] = com_data;
			if(com3.rx_ep >= BUF_SIZE)	com3.rx_ep = 0;
		}
	}
	ram_cs();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)	{
		disable(RI);
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUF_SIZE)		com1.rx_ep = 0;
	} else if(TI)	{
		disable(TI);
		if(com1.tx_sp != com1.tx_ep)	{
			SBUF = com1.tx_buf[com1.tx_sp++];
			if(com1.tx_sp >= BUF_SIZE)	com1.tx_sp = 0;
		} else	com1.tx_flag = ON;
	}
	ram_cs();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
