//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]            97' AWS and AMOS RTU(Remote Terminal Unit)               [*]
//[*]                  variable & port initial program                    [*]
//[*]                Copyright Jinyang industrial co.,ltd                 [*]
//[*]                      Program by Jinmin - Mok                        [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]---------------------------------------------------------------------[*]
#define	INTERNAL	0
#define	EXTERNAL	1
//[*]---------------------------------------------------------------------[*]
#define	DATABIT7	0x02
#define	DATABIT8	0x03
//[*]---------------------------------------------------------------------[*]
#define	STOP1		0x00
#define	STOP2		0x04
//[*]---------------------------------------------------------------------[*]
#define	PARITY_NO	0x00
#define	PARITY_ODD	0x00
#define	PARITY_EVEN	0x00
//[*]---------------------------------------------------------------------[*]
#define	BUF_SIZE	1000
//[*]---------------------------------------------------------------------[*]
#define	M11			0
#define	M22			1
#define	M3			2
#define	M1			3
//[*]---------------------------------------------------------------------[*]
#define	BPS9600		0
#define	BPS4800		1
#define	BPS2400		2
#define	BPS1200		3
//[*]---------------------------------------------------------------------[*]
#define	SMOD_ON		1
#define	SMOD_OFF	0
#define	NOT_USED	0
//[*]---------------------------------------------------------------------[*]
#define Wind_Direc       0
#define Wind_Speed       1
#define Temper           2
#define Precipi          3
#define Prec_Detect      4
#define Pressure         5
#define Humidity         6
#define Dew_Temp         7
#define Sun_Rada         8
#define Sun_Detect       9
#define Evaporation     10
#define Gnd_Humidit     11
#define Snow            12
#define Dewdrop         13
#define Fog             14
#define Cloud           15
#define Visibility      16
#define Bak_Lumi        17
#define Sigma           18
#define Sky_Emiss       19
#define Gnd_Emiss       20
#define Ph              21
#define Conducte        22
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

static	protocol_grp_t	com1;

//[*]---------------------------------------------------------------------[*]
static	byte	INTER_BAUD[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
//[*]---------------------------------------------------------------------[*]
typedef struct		real__t	{
	unsigned int	header;
	unsigned char	command;
	unsigned char	status;
	unsigned char	sensor_code;
	unsigned char	sensor_id;
	unsigned int	aver;
	unsigned int	spec;
	unsigned int	mini;
	unsigned int	maxi;
	unsigned char	chk_xor;
	unsigned int	tail;
}	real_t;

typedef union		real__u	{
	real_t			real;
	unsigned char	bytes[sizeof(real_t)];
}	real_u;

static real_u		rxd;

//[*]---------------------------------------------------------------------[*]
interrupt void EX0_int (void);
interrupt void T0_int  (void);
interrupt void T2_int  (void);
interrupt void SCON_int(void);

void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
					  byte crystal, byte smod, byte sta, byte protocol_count);
void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
					 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2));
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status);
void protocol_event(protocol_grp_t *comx, byte com_data);
void protocol_q(protocol_grp_t *comx, byte protocol_number, byte com_data);
void initiliaze(void);

void receive_check(void);
byte mpm_header_tail(variable_t *var);
byte led_header_tail(variable_t *var);
byte com1_sign_catch(variable_t *var);
byte com1_mpm_catch(variable_t *var);
byte com1_led_catch(variable_t *var);

void transmit_check(void);
void send_data(protocol_grp_t *comx, byte sdata);
void sign_send(protocol_grp_t *comx, byte board_id, byte sensor_id, word size);
void weather_send(protocol_grp_t *comx, weather_u *prs_data);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
					  byte crystal, byte smod, byte sta, byte protocol_count)
{
	byte	baud_value = 0, status    = sta;
	word	baud	   = 0, base_addr =   0;

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
		TMOD  = 0x25;
		SCON  = 0x50;

		TH0   = 0x00;
		TL0   = 0x00;

		T2CON = 0x00;
		TH2   = 0xFA;
		TL2   = 0x00;
		// Interrupt enable register
		enable (ET2);
		enable (ES );
		enable (ET0);
		enable (EX0);
		// TCON Register initial
		enable (TR2);
		enable (TR1);
		enable (TR0);
		enable (IT1);
		enable (IT0);
		// Interrupt rangkig register
		enable (PS );
	}
	WATCHDOG;
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
	byte	position;

	WATCHDOG;
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
void initiliaze(void)
{
	WATCHDOG;
	disable(EA);								// All interrupt disable
	init_serial_grp(&com1, INTERNAL, 0, BPS9600, M22, SMOD_OFF, NOT_USED,3);
	init_serial_pro(&com1, 0, 15, mpm_header_tail, com1_sign_catch);
	init_serial_pro(&com1, 1, 32, mpm_header_tail, com1_mpm_catch);
	init_serial_pro(&com1, 2, 17, led_header_tail, com1_led_catch);
	set_protocol_flag(&com1, 0, ON );
	set_protocol_flag(&com1, 1, OFF);
	set_protocol_flag(&com1, 2, ON );
	enable (EA );								// All interrupt enable
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void receive_check(void)
{
	WATCHDOG;
	if(com1.rx_sp != com1.rx_ep)	{
		RX_LED_ON;
		protocol_event(&com1,com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= BUF_SIZE)	com1.rx_sp = 0;
	} else	RX_LED_OFF;
	WATCHDOG;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte mpm_header_tail(variable_t *var)
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
byte led_header_tail(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com1_sign_catch(variable_t *var)
{
	byte chk_add = 0, chk_xor = 0, sel;
	word count;

	for(count=0; count<var->size; count++)	{
		sign.bytes[count] = var->buf[(var->p_sp + count) % var->size];
		if(count > 3 && count < var->size - 6)	{
			chk_xor ^= var->buf[(var->p_sp + count) % var->size];
			chk_add += var->buf[(var->p_sp + count) % var->size];
			WATCHDOG;
		}
	}
	if(sign.sam.chk_xor   != chk_xor )								return 0;
	if(sign.sam.chk_add   != chk_add )								return 0;
	sel = 200;
	if     (real[WD].sam.sensor_id == sign.sam.sensor_id)	sel =  WD;
	else if(real[WS].sam.sensor_id == sign.sam.sensor_id)	sel =  WS;
	if(sel == 200)											return 0;
	if(sign.sam.command == 'D')	{
		for(count=0; count<com1.pcnt; count++)	{
			if(sign.sam.data_size == com1.p[count].var.size)	{
				set_protocol_flag(&com1, count, ON);
			}
		}
	}
	WATCHDOG;														return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com1_mpm_catch(variable_t *var)
{
	byte chk_xor = 0, chk_add = 0, sel;
	word count;

	for(count=0; count<var->size; count++)	{
		mpm.bytes[count] = var->buf[(var->p_sp + count) % var->size];
		if(count>3 && count<(var->size - 6))	{
			chk_xor ^= var->buf[(var->p_sp + count) % var->size];
			chk_add += var->buf[(var->p_sp + count) % var->size];
			WATCHDOG;
		}
	}
	if(mpm.receive.chk_xor != chk_xor)								return 1;
	if(mpm.receive.chk_add != chk_add)								return 1;

	if     (real[WD].sam.sensor_id == mpm.receive.sensor_id)	sel =  WD;
	else if(real[WS].sam.sensor_id == mpm.receive.sensor_id)	sel =  WS;
	else 														sel = 200;

	if(sel != 200)	{
		switch(mpm.receive.command)	{
			case 'W' 	:
				sign_send(&com1, 0, real[sel].sam.sensor_id, sizeof(weather_t));
				switch(mpm.receive.schedule)	{
					case REAL:	weather_send(&com1, &real[sel]);	break;
					case CALL:	weather_send(&com1, &call[sel]);	break;
					case SAVE:	weather_send(&com1, &save[sel]);	break;
				}
				WATCHDOG;											return 1;

			case INIT	:	variable_initial();
							WATCHDOG;								return 1;
			case RST	:	real[mpm.receive.sensor_id].sam.maxi =
							call[mpm.receive.sensor_id].sam.maxi =
							save[mpm.receive.sensor_id].sam.maxi = mpm.receive.maxi;
							real[mpm.receive.sensor_id].sam.mini =
							call[mpm.receive.sensor_id].sam.mini =
							save[mpm.receive.sensor_id].sam.mini = mpm.receive.mini;
		}
	}
	WATCHDOG;														return 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
byte com1_led_catch(variable_t *var)
{
	byte  chk_xor = 0, id;
	word  count;
	float vlaue, range, offset;

	for(count=0; count<var->size; count++)	{
		rxd.bytes[count] = var->buf[(var->p_sp + count) % var->size];
		if(count > 1 && count < var->size - 3)	{
			chk_xor ^= var->buf[(var->p_sp + count) % var->size];
			WATCHDOG;
		}
	}
	if(rxd.real.chk_xor != chk_xor)									return 0;
	if((~(input(JUMPER))) & 0x08)	{
		range  = 4096.0;					// 0 ~ 10V
		offset =    0.0;
	} else	{
		range  = 3276.8;					// 2 ~ 10V
		offset =  819.2;
	}
	if(rxd.real.command == 'M')	{
		id = rxd.real.sensor_id;
		switch(rxd.real.sensor_code)	{
			case Wind_Direc	:
						vlaue = (float)(rxd.real.aver / 10.);
						if(recoder.maxi[id] != rxd.real.maxi)	{
							recoder.maxi[id] = rxd.real.maxi;
							vlaue = (float)(rxd.real.maxi / 10.);
						}
						if((~(input(JUMPER))) & 0x10)	{
							range = range / 540.0;
							vlaue = main_540deg(vlaue);
						} else	range = range / 360.0;
						break;
			case Wind_Speed	:
						vlaue = (float)(rxd.real.aver / 10.);
						if(recoder.maxi[id] != rxd.real.maxi)	{
							recoder.maxi[id] = rxd.real.maxi;
							vlaue = (float)(rxd.real.maxi / 10.);
						}
						range = range / 60.0;
						break;
			case Temper	:
						vlaue = (float)((rxd.real.aver / 10.) - 50);
						if(recoder.mini[id] != rxd.real.mini)	{
							recoder.mini[id] = rxd.real.mini;
							vlaue = (float)((rxd.real.mini / 10.) - 50);
						}
						if(recoder.maxi[id] != rxd.real.maxi)	{
							recoder.maxi[id] = rxd.real.maxi;
							vlaue = (float)((rxd.real.maxi / 10.) - 50);
						}
						range = range / 100.0;
						break;
			case Precipi :	// 0 ~ 500mm
						vlaue = (float)(rxd.real.aver / 10.);
						if(recoder.maxi[id] != rxd.real.maxi)	{
							recoder.maxi[id] = rxd.real.maxi;
							vlaue = (float)(rxd.real.maxi / 10.);
						}
						range = range / 500.0;
						break;
			case Pressure :	// 800 ~ 1200 hPa
						vlaue = (float)(rxd.real.aver / 10.) - 800.0;
						if(recoder.maxi[id] != rxd.real.maxi)	{
							recoder.maxi[id] = rxd.real.maxi;
							vlaue = (float)(rxd.real.maxi / 10.) - 800.0;
						}
						range = range / 400.0;
						break;
			default	:
						vlaue = (float)(rxd.real.aver / 10.);
						if(recoder.mini[id] != rxd.real.mini)	{
							recoder.mini[id] = rxd.real.mini;
							vlaue = (float)(rxd.real.mini / 10.);
						}
						if(recoder.maxi[id] != rxd.real.maxi)	{
							recoder.maxi[id] = rxd.real.maxi;
							vlaue = (float)(rxd.real.maxi / 10.);
						}
						range = range / 100.0;
						break;
		}
		vlaue = (vlaue * range) + offset;
		disable(EA);
		recoder.aver[id] = ~(int)(vlaue);
		enable (EA);
	}
	WATCHDOG;														return 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void transmit_check(void)
{
	WATCHDOG;									// Transmit com port
	if(com1.tx_sp != com1.tx_ep)	{
		if(com1.tx_flag)	{
			disable(EA);
			SBUF = com1.tx_buf[com1.tx_sp++];
			enable(EA);
			if(com1.tx_sp >= BUF_SIZE)	com1.tx_sp = 0;
			TX_LED_ON;
			com1.tx_flag = OFF;
		}
	} else	TX_LED_OFF;
	WATCHDOG;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void send_data(protocol_grp_t *comx, byte sdata)
{
	WATCHDOG;
	comx->tx_buf[comx->tx_ep++] = sdata;
	if(comx->tx_ep >= BUF_SIZE)	comx->tx_ep = 0;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sign_send(protocol_grp_t *comx, byte board_id, byte sensor_id, word size)
{
	byte i;

	sign.sam.header		= 0xFFFFFFFF;
	sign.sam.board_id	= board_id;
	sign.sam.sensor_id	= sensor_id;
	sign.sam.command	= 'D';
	sign.sam.data_size	= size;
	sign.sam.chk_xor	= 0x00;
	sign.sam.chk_add	= 0x00;
	sign.sam.tail		= 0xFEFEFEFE;

	for(i=0; i<sizeof(sign_u); i++)	{
		if(i>3 && i<sizeof(sign_u) - 6)	{
			sign.sam.chk_xor ^= sign.bytes[i];
			sign.sam.chk_add += sign.bytes[i];
		}
		send_data(comx,sign.bytes[i]);
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void weather_send(protocol_grp_t *comx, weather_u *prs_data)
{
	byte i;

	prs_data->sam.chk_xor  = 0x00;
	prs_data->sam.chk_add  = 0x00;

	for(i=0; i<sizeof(weather_u); i++)	{
		if(i>3 && i<sizeof(weather_u) - 6)	{
			prs_data->sam.chk_xor ^= prs_data->bytes[i];
			prs_data->sam.chk_add += prs_data->bytes[i];
		}
		send_data(comx, prs_data->bytes[i]);
	}
	if(comx->tx_sp != comx->tx_ep)	comx->tx_flag = 1;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt void T0_int(void)         // Timer 0 interrupt vector
{
	disable(EA);
	enable (EA);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt void T2_int (void)
{
	disable(EA);
	disable(TF2);
	output(CHANNEL,0);
	output(DAH_ADDR,(char)(recoder.aver[recoder.pos] >> 8));
	output(DAL_ADDR,(char)(recoder.aver[recoder.pos]     ));
	output(CHANNEL,recoder.sel);
	recoder.sel <<= 1;
	if(++recoder.pos > 7)	{
		recoder.pos = 0;
		recoder.sel = 1;
	}
	TH2 = 0xF0;	TL2 = 0x00;
	enable (EA);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt void SCON_int (void)
{
	disable(EA);
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
	enable (EA);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
interrupt void EX0_int (void)
{
	disable(EA);
	if(clock.one_sec < 8)	clock.one_sec++;
	if(clock.one_sec == 8)	{
		TR0  = 0;								// Timer0 stop
		value.new_cnt  = TH0 << 8;				// Wind speed count
		value.new_cnt |= TL0;
		TR0  = 1;								// Timer0 run
		if(value.old_cnt > value.new_cnt)
				value.ws_cnt = (value.new_cnt + 0x10000) - value.old_cnt;
		else	value.ws_cnt =  value.new_cnt            - value.old_cnt;
		value.old_cnt = value.new_cnt;
	}
	value.delay_count++;
	enable (EA);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
