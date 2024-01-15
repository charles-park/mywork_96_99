//[*]----------------------------------------------------------------------[*]
void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, word speed,
					  byte crystal, byte smod, byte status, byte protocol_count)
{
	byte	baud_value = 0;
	word	baud	   = 0;

	comx->base    = b_addr;
	comx->type    = mode;
	comx->pcnt    = protocol_count;
	comx->rx_sp   = comx->rx_ep = comx->tx_sp = comx->tx_ep = 0;
	comx->p       = (protocol_t *)(malloc( sizeof(protocol_t) * comx->pcnt));
	comx->tx_flag = ON ;
	comx->cat_flag= ON ;
	comx->xtal    = crystal;

	if(comx->type == INTERNAL)	{
		disable(EA);

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
		SCON = 0x50;		TMOD = 0x20;		enable(TR1);
		enable(ES);			enable(PS);

	}	else	{

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
		output( LCR(comx->base),(0x80 | status));
		output( DLL(comx->base),        baud   );
		output( DLM(comx->base), 0x00          );
		output( LCR(comx->base),(0x7F & status));
		output( IER(comx->base), 0x01          );
		output( MCR(comx->base), 0x0b          );
	}
}
//[*]----------------------------------------------------------------------[*]
void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
					 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2))
{
	comx->p[protocol_number].var.p_sp   = 0  ;
	comx->p[protocol_number].var.p_ep   = 0  ;
	comx->p[protocol_number].var.flag   = 0  ;
	comx->p[protocol_number].var.size   = buffer_size;
	comx->p[protocol_number].var.buf    = (byte *)(malloc(comx->p[protocol_number].var.size));
	comx->p[protocol_number].pcheck = chk;
	comx->p[protocol_number].pcatch = cat;
}
//[*]----------------------------------------------------------------------[*]
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status)
{
	comx->p[protocol_number].var.flag = status;
}
//[*]----------------------------------------------------------------------[*]
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
//[*]----------------------------------------------------------------------[*]
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
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]


