//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void init_serial(word buffer_size, byte (*chk)(variable_t *func1),
								   byte (*cat)(variable_t *func2) )
{
	com.rx_sp = com.rx_ep = com.tx_sp = com.tx_ep = 0;
	com.tx_flag = ON;		com.cat_flag = ON;
	com.p = (protocol_t *)(malloc(sizeof(protocol_t)));

	SCON  = 0x50;	TMOD  = 0x20;	PCON |= 0x80;
	TH1   = TL1 = 0xF4;	// 22.1184Mhz->9600  Bps, 11.0592Mhz->0xFA
//	TH1   = TL1 = 0xFA;	// 22.1184Mhz->19200 Bps, 11.0592Mhz->0xFD

	com.pcnt 			= 1;
	com.p[0].var.p_sp   = 0;	com.p[0].var.p_ep   = 0;
	com.p[0].var.flag   = 0;	com.p[0].var.size   = buffer_size;
	com.p[0].pcheck     = chk;	com.p[0].pcatch     = cat;
	com.p[0].var.buf    = (byte *)(malloc(buffer_size));
	com.p[0].var.flag   = ON;

	enable(TR1);	enable(ES) ;	enable(PS) ;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status)
{
	comx->p[protocol_number].var.flag = status;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void protocol_event(protocol_grp_t *comx, byte com_data)
{
	byte	position;

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
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
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
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
byte rx_chk(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != 0xFE)	return 0;
	RX_LED(OFF);
	if(var->buf[(var->p_sp 			   +3) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp 			   +2) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp 			   +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp   			 ) % var->size] != 0xFF)	return 0;
	return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
byte rx_cat(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0;
	word	i = 0, j = 0;

	watch();
	for(i=4, j=0; i<var->size-6; i++, j++)	{
		ck_xor ^=  var->buf[(var->p_sp + i) % var->size];
		ck_add +=  var->buf[(var->p_sp + i) % var->size];
		disp_catch(var->buf[(var->p_sp + i) % var->size], j);
	}
	if(ck_add != var->buf[(var->p_sp + var->size -6) % var->size])	return 0;
	if(ck_xor != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;

	lcd_buffer.new_page = ON;
	output(VOLUME_H, var->buf[(var->p_sp + var->size -8) % var->size]);
	output(VOLUME_L, var->buf[(var->p_sp + var->size -7) % var->size]);
	return 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void disp_catch(byte d_data, word addr)
{
	byte	x, y, pg;

	watch();
	if(addr >= 80)					return;
	if(addr == 0)	{
		x = 0;			y = 0;
	}	else	{
		x = addr % LCD_COLUM;			y = addr / LCD_COLUM;
	}
	pg = lcd_buffer.page +1;
	if(pg >= PAGE)		pg = 0;
	lcd_buffer.disp[x][y][pg] = d_data;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void key_send_pc(byte key_data)
{
	word	i;
	watch();
	key_send.pc.header = 0xFFFFFFFF;	key_send.pc.tail = 0xFEFEFEFE;
	key_send.pc.xor    = 0x00      ;	key_send.pc.add  = 0x00;
	key_send.pc.key    = key_data  ;
	key_send.pc.xor   ^= key_send.pc.key;
	key_send.pc.add   += key_send.pc.key;

	for(i=0; i<sizeof(key_send_u); i++)		move_to_serial_buffer(&com, key_send.bytes[i]);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void move_to_serial_buffer(protocol_grp_t *comx, byte s_data)
{
	watch();
	comx->tx_buf[comx->tx_ep++] = s_data;
	if(comx->tx_ep >= BUFFER_SIZE)	comx->tx_ep = 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void transmit_check(void)
{
	watch();
	if(com.tx_sp != com.tx_ep)	{
		if(com.tx_flag)	{
			disable(EA);
			SBUF = com.tx_buf[com.tx_sp++];
			enable(EA);
			if(com.tx_sp >= BUFFER_SIZE)	com.tx_sp = 0;
			com.tx_flag = OFF;
		}
	}	else	com.cat_flag = ON;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void receive_check(void)
{
	watch();
	if(com.rx_sp != com.rx_ep)	{
		if(com.cat_flag == OFF)				return;
		protocol_event(&com, com.rx_buf[com.rx_sp++]);
		if(com.rx_sp >= BUFFER_SIZE)	com.rx_sp = 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)	{
		disable(RI);	RX_LED(ON);
		com.rx_buf[com.rx_ep++] = SBUF;
		if(com.rx_ep >= BUFFER_SIZE)		com.rx_ep = 0;
	} else if(TI)	{
		disable(TI);	TX_LED(ON);
		if(com.tx_sp != com.tx_ep)	{
			SBUF = com.tx_buf[com.tx_sp++];
			if(com.tx_sp >= BUFFER_SIZE)	com.tx_sp = 0;
		} else	{
			com.tx_flag = ON;	TX_LED(OFF);
		}
	}
	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]



