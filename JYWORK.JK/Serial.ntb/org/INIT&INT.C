/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]                       This is serial emulator                      [*]*/
/*[*]                                and                                 [*]*/
/*[*]                      protocal search program                       [*]*/
/*[*]                Copyright Jinyang industrial co.,ltd                [*]*/
/*[*]                      Program by Jinmin - Mok                       [*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#include	"main.h"
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
protocol_grp_t	com1, com2;
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void interrupt( *com1_old_vector)(__CPPARGS);
void interrupt( *com2_old_vector)(__CPPARGS);
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void interrupt handler1(__CPPARGS)
{
	if(inportb(IIR(com1.base)) & 0x04)	{
		com1.rx_buf[com1.rx_ep++] = inportb(RBR(com1.base));
		if(com1.rx_ep >= BUF_SIZE)	com1.rx_ep = 0;
	}
	outportb(OCW2, EOI);
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void interrupt handler2(__CPPARGS)
{
	if(inportb(IIR(com2.base)) & 0x04)	{
		com2.rx_buf[com2.rx_ep++] = inportb(RBR(com2.base));
		if(com2.rx_ep >= BUF_SIZE)	com2.rx_ep = 0;
	}
	outportb(OCW2, EOI);
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void initiliaze(void)
{
	byte sta;
	long speed;

	if(value.comport == COM1)	{
		sta = DATA8 | STOP1 | PRT_NONE;
//		speed = 2400L;
		speed = 1200L;
		init_serial_grp(&com1, COM1_ADDR, speed, Y18432, sta, 2);
		init_serial_pro(&com1, 0,  24, header_tail_check, com1_sign_catch);
		init_serial_pro(&com1, 1,  26, header_tail_check, com1_cmd_catch);
		set_protocol_flag(&com1, 0, ON );
		set_protocol_flag(&com1, 1, OFF);
	} else	if(value.comport == COM2)	{
		sta = DATA8 | STOP1 | PRT_NONE;
//		speed = 2400L;
		speed = 1200L;
		init_serial_grp(&com2, COM2_ADDR, speed, Y18432, sta, 0);
	}
	init_serial_vector(value.comport);
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void init_serial_vector(byte port)
{
	value.old_20 = inportb(0x20);
	value.old_21 = inportb(0x21);
	if(port == COM1)	{
		com1_old_vector = getvect(COM1_IRQ);	// save the old interrupt vector
		setvect(COM1_IRQ, handler1);			// install the new interrupt handler
		outportb(IMR, (inportb(IMR) & 0xEF));	// Com1 interrupt enable
	} else	if(port == COM2)	{
		com2_old_vector = getvect(COM2_IRQ);	// save the old interrupt vector
		setvect(COM2_IRQ, handler2);			// install the new interrupt handler
		outportb(IMR, (inportb(IMR) & 0xF7));	// Com2 interrupt enable
	}
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void return_serial_vector(byte port)
{
	outportb(0x20, value.old_20);
	outportb(0x21, value.old_21);
	if(port == COM1)	{
		setvect(COM1_IRQ, com1_old_vector);
		outportb(IER(com1.base),(        0x00));
		outportb(LCR(com1.base),(        0x80));
		outportb(DLL(com1.base),(com1.old_bdl));
		outportb(DLM(com1.base),(com1.old_bdh));
		outportb(LCR(com1.base),(com1.old_lcr));
		outportb(MCR(com1.base),(com1.old_mcr));
		outportb(IER(com1.base),(com1.old_ier));
	} else	if(port == COM2)	{
		setvect(COM2_IRQ, com2_old_vector);
		outportb(IER(com2.base),(        0x00));
		outportb(LCR(com2.base),(        0x80));
		outportb(DLL(com2.base),(com2.old_bdl));
		outportb(DLM(com2.base),(com2.old_bdh));
		outportb(LCR(com2.base),(com2.old_lcr));
		outportb(MCR(com2.base),(com2.old_mcr));
		outportb(IER(com2.base),(com2.old_ier));
	}
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void init_serial_grp(protocol_grp_t *comx, word b_addr, long speed,
					 long crystal, byte status, byte protocol_count)
{
	word baud;

	comx->base    = b_addr;
	comx->pcnt    = protocol_count;
	comx->rx_sp   = comx->rx_ep = comx->tx_sp = comx->tx_ep = 0;
	comx->p       = (protocol_t *)(malloc( sizeof(protocol_t) * comx->pcnt));
	comx->tx_flag = ON;

	baud = (word)(crystal / (speed * 16L));
	comx->old_ier = inportb(IER(comx->base ));
	outportb(IER(comx->base),(         0x00));
	comx->old_lcr = inportb(LCR(comx->base ));
	outportb(LCR(comx->base),(         0x80));
	comx->old_bdl = inportb(DLL(comx->base ));
	outportb(DLL(comx->base),(byte)(   baud));
	comx->old_bdh = inportb(DLM(comx->base ));
	outportb(DLM(comx->base),(byte)(baud>>8));
	outportb(LCR(comx->base),(0x7F & status));
	comx->old_mcr = inportb(MCR(comx->base ));
	outportb(MCR(comx->base),(         0x0B));
	outportb(IER(comx->base),(         0x01));
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
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
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void set_protocol_flag(protocol_grp_t *comx, byte protocol_number, byte status)
{
	comx->p[protocol_number].var.flag = status;
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void protocol_event(protocol_grp_t *comx, byte com_data)
{
	byte position;

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
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
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

/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void comport_event(void)
{
	if(com1.rx_sp != com1.rx_ep)	{

if(com1.rx_buf[com1.rx_sp] == 0x0D)	printf("\n");
else								printf("%c",com1.rx_buf[com1.rx_sp]);

		protocol_event(&com1,com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= BUF_SIZE)	com1.rx_sp = 0;
	}
	if(com2.rx_sp != com2.rx_ep)	{

if(com2.rx_buf[com2.rx_sp] == 0x0D)	printf("\n");
else								printf("%c",com2.rx_buf[com2.rx_sp]);

		protocol_event(&com2,com2.rx_buf[com2.rx_sp++]);
		if(com2.rx_sp >= BUF_SIZE)	com2.rx_sp = 0;
	}

	if(com1.tx_sp != com1.tx_ep)	{
		if(0x20 & inportb(LSR(com1.base)))	{
			outportb(THR(com1.base), com1.tx_buf[com1.tx_sp++]);
			if(com1.tx_sp >= BUF_SIZE)	com1.tx_sp = 0;
		}
	}
	if(com2.tx_sp != com2.tx_ep)	{
		if(0x20 & inportb(LSR(com2.base)))	{
			outportb(THR(com2.base), com2.tx_buf[com2.tx_sp++]);
			if(com2.tx_sp >= BUF_SIZE)	com2.tx_sp = 0;
		}
	}
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
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
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
byte com1_sign_catch(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
																	return 0;
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
byte com1_cmd_catch(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 1;
																	return 1;
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
byte com2_sign_catch(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
																	return 0;
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
byte com2_cmd_catch(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 1;
																	return 1;
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void send_data(byte port, byte sdata)
{
	if(port == COM1)	{
		com1.tx_buf[com1.tx_ep++] = sdata;
		if(com1.tx_ep >= BUF_SIZE)	com1.tx_ep = 0;
	} else	if(port == COM2)	{
		com2.tx_buf[com2.tx_ep++] = sdata;
		if(com2.tx_ep >= BUF_SIZE)	com2.tx_ep = 0;
	}
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void send_led_data(byte port, byte cmd, byte sta, byte cd, byte id)
{
	byte i;
	led.rtx.header      = 0xFFFF;
	led.rtx.command     = cmd;
	led.rtx.status      = sta;
	led.rtx.sensor_code = cd;
	led.rtx.sensor_id   = id;
	led.rtx.chk_xor     = 0x00;
	led.rtx.tail		= 0xFEFF;
	for(i=2; i<sizeof(led_u)-3; i++)	led.rtx.chk_xor ^= led.bytes[i];
	for(i=0; i<sizeof(led_u)  ; i++)	{
		send_data(port, led.bytes[i]);
		printf("%02X",  led.bytes[i]);
	}
	printf("\n");
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]
