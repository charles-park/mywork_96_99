//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		DLL(x)				(x+0)
#define		DLM(x)				(x+1)
#define		THR(x)				(x+0)
#define		RBR(x)				(x+0)
#define		IER(x)				(x+1)
#define		IIR(x)				(x+2)
#define		LCR(x)				(x+3)
#define		MCR(x)				(x+4)
#define		LSR(x)				(x+5)
#define		MSR(x)				(x+6)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		INTERNAL			0
#define		EXTERNAL			1
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		DATA_BIT_7			0x2
#define		DATA_BIT_8			0x3
#define		STOP_BIT_1			0x0
#define		STOP_BIT_2			0x4
#define		PARITY_NO			0x0
#define		PARITY_ODD			0x8
#define		PARITY_EVEN			0x18
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		BUF_SIZE			2000
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
#define		disable(y)			y = 0
#define		enable(z)			z = 1
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		Error		   	   -1
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct	variable__t	{
	Word	p_sp;
	Word	p_ep;
	Word	size;
	Word	dsize;
	Byte	flag;
	Byte	*buf;
}	variable_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct	protocol__t	{
	variable_t	var;
	Byte	(*pcheck)(variable_t *p);
	Byte	(*pcatch)(variable_t *p);
}	protocol_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct	protocol_grp__t	{
	Word	rx_sp,rx_ep;
	Word	tx_sp,tx_ep;
	Byte	tx_flag;
	Byte	tx_buf[BUF_SIZE];
	Byte	rx_buf[BUF_SIZE];
	Byte	pcnt;
	Word	base;
	protocol_t	*p;
}	protocol_grp_t;

static	protocol_grp_t	Com1,Com2,Com3,Com4;

//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	Byte	INTER_BAUD[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	Byte	EXTER_BAUD[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
	static 	Byte	Log_id ;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x03] 	void 	EX0_int (void);
interrupt [0x0B]	void	T0_int(void);
interrupt [0x13] 	void 	EX1_int (void);
interrupt [0x23] 	void 	SCON_int (void);

void 	init_serial_grp(protocol_grp_t *Comx,Byte mode,Word b_addr,Word speed,
						 Byte clock,Byte S_MOD,Byte sta,Byte Pcount_Rx);
void 	init_serial_pro(protocol_grp_t	*Comx, Byte Pcount, Byte BufSize, Byte (*chk)(variable_t *FuncP1),
						 Byte (*cat)(variable_t *FuncP2));
void 	set_protocol_flag(protocol_grp_t *Comx, Byte Pcount, Byte sta);
void 	protocol_event(protocol_grp_t *Comx, Byte RecData);
void 	protocol_q(protocol_grp_t *Comx, Byte Pcount, Byte RecData);
void 	initiliaze(void);
Byte 	rx_chk(variable_t *var);    // var->buf == p_buf
Byte	rx1_cat0(variable_t *var);
Byte	rx2_cat0(variable_t *var);
Byte	rx2_cat1(variable_t *var);
Byte	rx2_cat2(variable_t *var);
Byte	rx3_cat0(variable_t *var);
void 	receive_check(void);
void 	transmit_check(void);

void	key_send(protocol_grp_t *Comx, Byte comd, Byte gb, Byte gb_id);
void	send_data(protocol_grp_t *Comx, Byte sdata);

//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt void T0_int(void)         			// Timer 0 interrupt vector
{
	disable(EA);
	if(LCD_REDISPLAY_FLAG)	{
		if(Buf_pos >= 16)	{
			Buf_pos = 0;
//			if(++LCD_REDISPLAY_FLAG > 5)	LCD_REDISPLAY_FLAG = 0;
		}
		output(LCD_DATA,LCD_BUFFER[Buf_pos]);
		Lcd_pos++, Buf_pos++;
		if(Lcd_pos >= 16)	Lcd_pos = 0;
		if(Lcd_pos == 0)	output(LCD_ADD,0x80);
		if(Lcd_pos == 8)	output(LCD_ADD,0xC0);
	}
	if(Buzzer_cnt++ > 20)	BUZZER(reoff);
	Delay_cnt++;	Cursor_cnt++;
	Key_del++;		Lcd_cnt++;
	TH0 = 0xEA;	TL0 = 0x00;
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)	{
		disable(RI);
		Com1.rx_buf[Com1.rx_ep++] = SBUF;
		if(Com1.rx_ep >= BUF_SIZE)		Com1.rx_ep = 0;
	} else	if(TI)	{
		disable(TI);
		if(Com1.tx_sp != Com1.tx_ep)	{
			SBUF = Com1.tx_buf[Com1.tx_sp++];
			if(Com1.tx_sp >= BUF_SIZE)	Com1.tx_sp = 0;
		} else	Com1.tx_flag = OFF;
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x03] void EX0_int (void)
{
	disable(EA);
	if(0x04 & input(IIR(Com3.base)))	{
		Com3.rx_buf[Com3.rx_ep++] = input(RBR(Com3.base));
		if(Com3.rx_ep >= BUF_SIZE)	Com3.rx_ep = 0;
	} else	if(input(IIR(Com4.base)) & 0x4)	{
		Com4.rx_buf[Com4.rx_ep++] = input(RBR(Com4.base));
		if(Com4.rx_ep >= BUF_SIZE)	Com4.rx_ep = 0;
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x13] void EX1_int (void)
{
	disable(EA);
	if(0x04 & input(IIR(Com2.base)))	{
		Com2.rx_buf[Com2.rx_ep++] = input(RBR(Com2.base));
		if(Com2.rx_ep >= BUF_SIZE)	Com2.rx_ep = 0;
	} else	if(Key_del >= 20)	{
		KEY_BUF[Key_pos_ed++] = input(KEY_ADD) & 0x0F;	// Keypad data read
		if(Key_pos_ed >= 10)	Key_pos_ed = 0;
		BUZZER(reon);	Buzzer_cnt = 0;
		LCD(reon);		Lcd_cnt    = 0;
		Key_del  = 0;
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_serial_grp(protocol_grp_t *Comx,Byte mode,Word b_addr,Word speed,
					 Byte clock,Byte S_MOD,Byte sta,Byte Pcount_Rx)
{
	Byte baud_value	= 0;
	Word baud		= 0;

	Comx->base		= b_addr;
	Comx->pcnt		= Pcount_Rx;
	Comx->tx_flag	= OFF;

	Comx->tx_ep	= Comx->rx_ep = Comx->tx_sp = Comx->rx_sp = 0x00;
	Comx->p     = (protocol_t *)(malloc( sizeof(protocol_t) * Comx->pcnt));

	disable(EA);
	if(mode == INTERNAL)	{
		baud_value += ((clock == M22) ? 1:0);
		if(S_MOD)	{	PCON |= 0x80;	baud_value += 1; }
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
		TH0 = 0xDC;		TL0 = 0x00;				// 100[Hz],10[ms]
		// Interrupt enable register
		enable ( ES);
		disable(ET1);
		enable (ET0);
		enable (EX1);
		enable (EX0);
		// TCON Register initial
		enable (TR1);
		enable (TR0);
		disable(IT1);							// 0 = level, 1 = edge trigger
		disable(IT0);

	} else	{
		baud_value += ((clock == M3) ? 4:0);
		switch(speed)	{
			case	BPS9600:    baud_value += 0;
								baud = EXTER_BAUD[baud_value];	break;
			case	BPS4800:    baud_value += 1;
								baud = EXTER_BAUD[baud_value];	break;
			case	BPS2400:    baud_value += 2;
								baud = EXTER_BAUD[baud_value];	break;
			case	BPS1200:    baud_value += 3;
								baud = EXTER_BAUD[baud_value];	break;
		}
		output(LCR(b_addr),(0x80 | sta));
		output(DLL(b_addr),        baud);
		output(DLM(b_addr),(0x00      ));
		output(LCR(b_addr),(0x7F & sta));
		output(IER(b_addr),(0x01      ));
		output(MCR(b_addr),(0x0B      ));
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_serial_pro(protocol_grp_t	*Comx, Byte Pcount, Byte BufSize, Byte (*chk)(variable_t *FuncP1),
					 Byte (*cat)(variable_t *FuncP2))
{
	Comx->p[Pcount].var.p_sp	= 0;
	Comx->p[Pcount].var.p_ep	= 0;
	Comx->p[Pcount].var.flag	= 0;
	Comx->p[Pcount].var.dsize	= 0;
	Comx->p[Pcount].var.size	= BufSize;
	Comx->p[Pcount].var.buf		= (Byte *)(malloc(Comx->p[Pcount].var.size));
	Comx->p[Pcount].pcheck		= chk;
	Comx->p[Pcount].pcatch		= cat;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void protocol_event(protocol_grp_t *Comx, Byte RecData)
{
	Byte	pos;
	for(pos = 0; pos < Comx->pcnt; pos++)	{
		if(Comx->p[pos].var.flag)	{
			protocol_q(Comx, pos, RecData);
			if(Comx->p[pos].pcheck(&Comx->p[pos].var))	{
				if(Comx->base == 0xA000)	led_con(COM2,RX,OFF);
				if(Comx->p[pos].pcatch(&Comx->p[pos].var))	{
					set_protocol_flag(Comx,pos,OFF);
				}
			}
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void protocol_q(protocol_grp_t *Comx, Byte Pcount, Byte RecData)
{
	Comx->p[Pcount].var.dsize++;
	Comx->p[Pcount].var.p_ep %= Comx->p[Pcount].var.size;
	if(Comx->p[Pcount].var.p_ep == Comx->p[Pcount].var.p_sp)	{
		Comx->p[Pcount].var.p_sp++;
		Comx->p[Pcount].var.p_sp %= Comx->p[Pcount].var.size;
	}
	Comx->p[Pcount].var.buf[Comx->p[Pcount].var.p_ep++] = RecData;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void set_protocol_flag(protocol_grp_t *Comx, Byte Pcount, Byte sta)
{
	Comx->p[Pcount].var.flag = sta;
	if(Comx->p[Pcount].var.flag == ON)	Comx->p[Pcount].var.dsize = 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte rx_chk(variable_t *var)					// var->buf == p_buf
{
	if(var->buf[(var->p_sp + var->size - 1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size - 2) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size - 3) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size - 4) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp +             3) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp +             2) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp +             1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp  	          ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void receive_check(void)
{
	if(Com1.rx_sp != Com1.rx_ep)	{
		led_con(COM1,RX,ON);
		Com3.tx_buf[Com3.tx_ep++] = Com1.rx_buf[Com1.rx_sp++];
		if(Com1.rx_sp >= BUF_SIZE)	Com1.rx_sp = 0;
		if(Com3.tx_ep >= BUF_SIZE)	Com3.tx_ep = 0;
	} else	led_con(COM1,RX,OFF);
	if(Com3.rx_sp != Com3.rx_ep)	{
		led_con(COM3,RX,ON);
		Com1.tx_buf[Com1.tx_ep++] = Com3.rx_buf[Com3.rx_sp++];
		if(Com3.rx_sp >= BUF_SIZE)	Com3.rx_sp = 0;
		if(Com1.tx_ep >= BUF_SIZE)	Com1.tx_ep = 0;
	} else	led_con(COM3,RX,OFF);

	if(Com2.rx_sp != Com2.rx_ep)	{
		led_con(COM2,RX,ON);
		protocol_event(&Com2,Com2.rx_buf[Com2.rx_sp++]);
		if(Com2.rx_sp >= BUF_SIZE)	Com2.rx_sp = 0;
	}
	if(Com4.rx_sp != Com4.rx_ep)	{
		led_con(COM4,RX,ON);
		protocol_event(&Com4,Com4.rx_buf[Com4.rx_sp++]);
		if(Com4.rx_sp >= BUF_SIZE)	Com4.rx_sp = 0;
	} else	led_con(COM4,RX,OFF);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte rx1_cat0(variable_t *var)
{
	return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte rx2_cat0(variable_t *var)
{
	Byte *Value_p = (char *)(&Value);
	Byte ck_xor	 = 0;
	Word count;

	for(count=4; count<var->size-5; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size-5) % var->size])	return 0;
	if(var->buf[(var->p_sp + 4) % var->size] != 'l')				return 0;
	switch(var->buf[(var->p_sp + 5) % var->size])	{
		case  1	:								// Sensor status display
		case  2	:								// Module status display
			for(count=0; count<8; count++)
				ASCII_BUF[count] = var->buf[(var->p_sp + count + 9) % var->size];
			ASCII_BUF[count] = NUL;
			return 0;
		case  4	:								// Memory test
		case 'A':								// Ascii 16 byte
			for(count=0; count<16; count++)	{
				ASCII_BUF[count] = var->buf[(var->p_sp + count + 9) % var->size];
				if(ASCII_BUF[0] != 'M')	Run_up = 0;
			}
			ASCII_BUF[count] = NUL;
			lcd_printf(0,0,1,"%s",ASCII_BUF);
			return 0;
		case  0	:								// Real display
			Sensor_cd = var->buf[(var->p_sp + 7) % var->size];
		case  3	:								// Volt display
			for(count=0; count<4; count++)
				Value_p[count]   = var->buf[(var->p_sp + count + 9) % var->size];
			return 0;
		case  5	:								// Setup code
			switch(var->buf[(var->p_sp + 6) % var->size])	{
				case  0	:						// Date data
				case  1	:						// Time data
					kd.send.yy_hh = var->buf[(var->p_sp +  9) % var->size];
					kd.send.mm_mm = var->buf[(var->p_sp + 10) % var->size];
					kd.send.dd_ss = var->buf[(var->p_sp + 11) % var->size];
					return 0;
				case  2	:						// Station ID data
					for(count=0; count<4; count++)
						Value_p[count]   = var->buf[(var->p_sp + count + 9) % var->size];
					Station_id = (int)(Value);
					return 0;
				case  3	:						// Passeord data
					for(count=0; count<4; count++)
						Value_p[count]   = var->buf[(var->p_sp + count + 9) % var->size];
					Password   = (int)(Value);
					return 0;
				default	:
					for(count=0; count<16; count++)
						ASCII_BUF[count] = var->buf[(var->p_sp + count + 9) % var->size];
					ASCII_BUF[count] = NUL;
					return 0;
			}
		case 'B':								// Buzzer on
			BUZZER(reon);
			Buzzer_cnt = -80;
			return 0;
		case 'R':								// RTU Run
			BUZZER(reon);
			Buzzer_cnt = -130;
			lcd_printf(0,0,1," Collection ... ");
	}
	return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte rx2_cat1(variable_t *var)
{
	Byte	ck_xor = 0;
	Byte	ck_add = 0;
	Word	count, i;

	if(var->buf[(var->p_sp + 4) % var->size] != 'C')				return 0;
	for(count=4; count<var->size-6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size-6) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size-5) % var->size])	return 0;

	Sensor_suu = var->buf[(var->p_sp + 5) % var->size];

	i = 7;
	for(count=0; count<Sensor_suu; count++, i+=2)
		SENSOR_CODE[count] = var->buf[(var->p_sp + i) % var->size];

	Led_size = 10 + ((Sensor_suu + 2) * 17);
	init_serial_pro(&Com2,2,Led_size,rx_chk,rx2_cat2);
	set_protocol_flag(&Com2,2,ON);
	Start_up = 0;
																	return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte rx2_cat2(variable_t *var)
{
	Byte	ck_xor = 0;
	Word	count;

	if(var->buf[(var->p_sp + 4) % var->size] != 'L')				return 0;
	for(count=4; count<var->size-5; count++)
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
	if(ck_xor != var->buf[(var->p_sp + var->size-5) % var->size])	return 0;
	for(count=5; count<var->size-5; count++)
		send_data(&Com4,var->buf[(var->p_sp + count) % var->size]);	return 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte rx3_cat0(variable_t *var)
{
	return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void initiliaze(void)
{
	//[*]------------------------------------------------------------------[*]
	init_serial_grp(&Com1,INTERNAL,     0,BPS9600,M11,SMOD_OFF,NOT_USED,1);
	init_serial_pro(&Com1,0,10,rx_chk,rx1_cat0);
	set_protocol_flag(&Com1,0,OFF);
	//[*]------------------------------------------------------------------[*]
	init_serial_grp(&Com2,EXTERNAL,0xA000,BPS9600, M1,NOT_USED,
									  (DATA_BIT_8|STOP_BIT_1|PARITY_NO),3);
	init_serial_pro(&Com2,0, 30,rx_chk,rx2_cat0);
	init_serial_pro(&Com2,1,212,rx_chk,rx2_cat1);
	set_protocol_flag(&Com2,0,ON);
	set_protocol_flag(&Com2,1,ON);
	//[*]------------------------------------------------------------------[*]
	init_serial_grp(&Com3,EXTERNAL,0x8000,BPS2400, M1,NOT_USED,
									  (DATA_BIT_8|STOP_BIT_1|PARITY_NO),1);
	init_serial_pro(&Com3,0,10,rx_chk,rx3_cat0);
	set_protocol_flag(&Com3,0,OFF);
	//[*]------------------------------------------------------------------[*]
	init_serial_grp(&Com4,EXTERNAL,0x9000,BPS2400, M1,NOT_USED,
									  (DATA_BIT_8|STOP_BIT_1|PARITY_NO),0);
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void transmit_check(void)
{
	if(Com1.tx_sp != Com1.tx_ep)	{
		if(!Com1.tx_flag)	{
			led_con(COM1,TX,ON);
			disable(EA);
			SBUF = Com1.tx_buf[Com1.tx_sp++];
			if(Com1.tx_sp >= BUF_SIZE)	Com1.tx_sp = 0;
			Com1.tx_flag = ON;
			enable(EA);
		}
	} else	led_con(COM1,TX,OFF);

	if(Com2.tx_sp != Com2.tx_ep)	{
		if(0x20 & input(LSR(Com2.base)))	{
			led_con(COM2,TX,ON);
			disable(EA);
			output(THR(Com2.base),Com2.tx_buf[Com2.tx_sp++]);
			if(Com2.tx_sp >= BUF_SIZE)	Com2.tx_sp = 0;
			enable(EA);
		}
	} else	led_con(COM2,TX,OFF);

	if(Com3.tx_sp != Com3.tx_ep)	{
		if(0x20 & input(LSR(Com3.base)))	{
			led_con(COM3,TX,ON);
			disable(EA);
			output(THR(Com3.base),Com3.tx_buf[Com3.tx_sp++]);
			if(Com3.tx_sp >= BUF_SIZE)	Com3.tx_sp = 0;
			enable(EA);
		}
	} else	led_con(COM3,TX,OFF);

	if(Com4.tx_sp != Com4.tx_ep)	{
		if(0x20 & input(LSR(Com4.base)))	{
			led_con(COM4,TX,ON);
			disable(EA);
			output(THR(Com4.base),Com4.tx_buf[Com4.tx_sp++]);
			if(Com4.tx_sp >= BUF_SIZE)	Com4.tx_sp = 0;
			enable(EA);
		}
	} else	led_con(COM4,TX,OFF);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
