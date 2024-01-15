//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_serial_grp( protocol_grp_t *comx, byte mode, word b_addr, unsigned long speed,
					  unsigned long crystal, byte smod, byte status, byte protocol_count)
{
	word	baud	   = 0;

	watch();
	disable(EA);
	comx->base    = b_addr;
	comx->type    = mode;
	comx->pcnt    = protocol_count;
	comx->rx_sp   = comx->rx_ep = comx->tx_sp = comx->tx_ep = 0;
	comx->p       = (protocol_t *)(malloc( sizeof(protocol_t) * comx->pcnt));
	comx->tx_flag = ON ;
	comx->cat_flag= ON ;
	comx->xtal    = (unsigned long)crystal;

	if(comx->type == INTERNAL)	{
		if(smod)		PCON |= 0x80;
		else			PCON &= 0x7F;

		TH1  = (byte)(256L-((1L+(unsigned long)smod) * crystal / 384L * speed));
		TL1  = TH1 = 0xF4;//
		SCON = 0x50;	TMOD = 0x20;	enable(PS);
		enable(TR1);	enable(ES);
	}	else	{
		baud = (word)(crystal / (speed * 16L));

		output( LCR(comx->base),(0x80 | status));
		output( DLL(comx->base),(byte)(baud   ));
		output( DLM(comx->base),(byte)(baud>>8));
		output( LCR(comx->base),(0x7F & status));
		output( IER(comx->base), 0x01          );
		output( MCR(comx->base), 0x0b          );
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void init_serial_pro(protocol_grp_t *comx, byte protocol_number, byte buffer_size,
					 byte (*chk)(variable_t *func1), byte (*cat)(variable_t *func2))
{
	watch();
	comx->p[protocol_number].var.p_sp   = 0  ;
	comx->p[protocol_number].var.p_ep   = 0  ;
	comx->p[protocol_number].var.flag   = 0  ;
	comx->p[protocol_number].var.size   = buffer_size;
	comx->p[protocol_number].var.buf    = (byte *)(malloc(comx->p[protocol_number].var.size));
	comx->p[protocol_number].pcheck = chk;
	comx->p[protocol_number].pcatch = cat;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status)
{
	watch();
	comx->p[protocol_number].var.flag = status;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void protocol_event(protocol_grp_t *comx, byte com_data)
{
	byte	position;

	watch();
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
	watch();
	comx->p[protocol_number].var.p_ep %= comx->p[protocol_number].var.size;
	if(comx->p[protocol_number].var.p_ep == comx->p[protocol_number].var.p_sp)
	{
		comx->p[protocol_number].var.p_sp++;
		comx->p[protocol_number].var.p_sp %= comx->p[protocol_number].var.size;
	}
	comx->p[protocol_number].var.buf[comx->p[protocol_number].var.p_ep++] = com_data;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]


