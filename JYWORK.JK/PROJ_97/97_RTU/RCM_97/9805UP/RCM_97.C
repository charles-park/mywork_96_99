#include <io51.h>
#include <stdio.h>
#include <stdlib.h>
#include "rcm_97.h"
#include "send.hpp"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil    Date : 1997. 8. 15~     [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : SAN UP AWS                          [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD :  NEW RCM BOARD                             [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 2.xx                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]----------------------------------------------------------------------[*]
void com_port_init(void)
{
	watch();		Rtuid = read_rtuid();
	init_serial_grp(&com1, INTERNAL, 0x0, BPS9600, M11, SMOD_ON, NOT_USED, 2);
	init_serial_pro(&com1, 0, 15, rx_chk, rx1_cat1);
	init_serial_pro(&com1, 1, 52, rx_chk, rx1_cat2);
	set_protocol_flag(&com1, 0, ON);	set_protocol_flag(&com1, 1, OFF);

	init_serial_grp(&com2, EXTERNAL, 0x8000, BPS2400, M1, NOT_USED,
				   (STOP1|PARITY_NO|DATABIT8) ,2);
	init_serial_pro(&com2, 0, 24, rx_chk, rx2_cat1);
	init_serial_pro(&com2, 1, 26, rx_chk, rx2_cat2);
	set_protocol_flag(&com2, 0, ON);	set_protocol_flag(&com2, 1, OFF);

	// com3 port is debug port & normal communication port
	// DIP SW msb bit is high logic then a/d debug port used,
	// else then normal communication port used.
	// debug mode port used must RS-422 interface jumper setting.
	// debug mode baudrate is 9600 BPS

	Debug_bit = OFF;	// debug mode setting
	if(Debug_bit == ON)	{
		init_serial_grp  (&com3, EXTERNAL, 0x9000, BPS9600, M1, NOT_USED,
						 (STOP1|PARITY_NO|DATABIT8) ,1);
		init_serial_pro  (&com3, 0, 50, rx3_debug_chk, rx3_debug_cat);
		set_protocol_flag(&com3, 0, ON);
	}	else	{
		init_serial_grp  (&com3, EXTERNAL, 0x9000, BPS2400, M1, NOT_USED,
						 (STOP1|PARITY_NO|DATABIT8) ,2);
		init_serial_pro  (&com3, 0, 24, rx_chk, rx3_cat1);
		init_serial_pro  (&com3, 1, 26, rx_chk, rx3_cat2);
		set_protocol_flag(&com3, 0, ON);
		set_protocol_flag(&com3, 1, OFF);
	}
}
//[*]----------------------------------------------------------------------[*]
void interrupt_init(void)
{
	watch();
	TMOD |= 0x1;		enable(TR0);		enable(ET0);
	disable(IT0);		enable(IT1);
	enable(EX0);		enable(EX1);		enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	word	i;
	watch();

	ev_point.hear    = 0;	ev_point.trace   = 0;	ev_point.forward = 0;

	Ack_check_count  = 0;	Mpm_check_count  = 0;	Jijum            = 0;
	Receive_count    = 0;	Rtuid            = 0;

	Event_check_flag = OFF; 			Event_save_flag  = OFF;
	Event_send_flag  = OFF;				Event_run_flag   = OFF;

	Baud_set_flag    = OFF;				Baud_set_value   = 0  ;
	Baud_set_port    = 0  ;

	for(i=0; i<8; i++)	status.send.bd_sta[i] =	status.send.sen_sta[i]  = 0xFF;
	status.send.cnt_bd.word_data = 0;			status.send.bd_sta [0] &= 0xFD;

	volts_data.p12_sta = volts_data.p5_sta = volts_data.m12_sta = 0x0;
	volts_data.ch      = M12_DATA;
	volts_data.flag    = OFF;

	ext_signal_data.byte_data = 0;
	output(EXT_SIGNAL_ADD, ext_signal_data.byte_data);

	POWER_RESET(OFF);					MPM_RESET(OFF);
}
//[*]----------------------------------------------------------------------[*]
byte read_rtuid(void)
{
	byte	value1, value2, count = 0;

	watch();	value1 = P1;	// read jumper

	while(1)	{
		value2 = P1;
		if(value1 == value2)	count++;
		else	{	count = 0;	value1 = P1;}
		if(count > 10)			break;
	}
	value1 = ~(value1 & 0x0f);	return (value1);
}
//[*]----------------------------------------------------------------------[*]
void mpm_reset(void)
{
	word	i;
	MPM_RESET(ON);
	for(i=0; i<5000; i++)	{	watch();	i++;	i--;	}
	MPM_RESET(OFF);
}
//[*]----------------------------------------------------------------------[*]
void rtu_reset(void)
{
	word	i;
	POWER_RESET(ON);
	for(i=0; i<5000; i++)	{	watch();	i++;	i--;	}
	POWER_RESET(OFF);
}
//[*]----------------------------------------------------------------------[*]
void ext_signal_set(byte set_cmd, byte set_num)
{
	switch(set_num)	{
		case	0:	ext_signal_data.bits.sig0 = set_cmd;	break;
		case	1:  ext_signal_data.bits.sig1 = set_cmd;	break;
		case	2:  ext_signal_data.bits.sig2 = set_cmd;	break;
		case	3:  ext_signal_data.bits.sig3 = set_cmd;	break;
		case	4:  ext_signal_data.bits.sig4 = set_cmd;	break;
		case	5:  ext_signal_data.bits.sig5 = set_cmd;	break;
		default  :  										break;
	}	output(EXT_SIGNAL_ADD, ext_signal_data.byte_data);
}
//[*]----------------------------------------------------------------------[*]
void event_check(void)
{
	word	i;
	byte	cmp_num;

	watch();
	cmp_num = (ev_point.forward + EVENT_GETSU -1 ) % EVENT_GETSU;

	for(i=0; i<8; i++)	{
		if(event.num[cmp_num].bd_sta [i] != status.send.bd_sta [i])
			Event_save_flag = ON;
		if(event.num[cmp_num].sen_sta[i] != status.send.sen_sta[i])
			Event_save_flag = ON;
	}

	if(event.num[cmp_num].p12 != volts_data.p12_sta)	Event_save_flag = ON;
	if(event.num[cmp_num].p5  != volts_data.p5_sta )	Event_save_flag = ON;
	if(event.num[cmp_num].m12 != volts_data.m12_sta)	Event_save_flag = ON;

	if(Event_save_flag == ON)							event_save();
	Event_check_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void event_save(void)
{
	word	i;
	byte	*p = (byte	*)(&event.num[ev_point.forward]);

	watch();

	if(Event_run_flag != ON)	{	Event_save_flag = OFF;		return;	}

	for(i=0; i<4; i++)	{
		event.num[ev_point.forward].header[i] = 0xFF;
		event.num[ev_point.forward].tail  [i] = 0xFE;
	}
	event.num[ev_point.forward].xor = 0;
	event.num[ev_point.forward].add = 0;
	event.num[ev_point.forward].s_time.yy = status.send.s_time.yy;
	event.num[ev_point.forward].s_time.mm = status.send.s_time.mm;
	event.num[ev_point.forward].s_time.dd = status.send.s_time.dd;
	event.num[ev_point.forward].s_time.hh = status.send.s_time.hh;
	event.num[ev_point.forward].s_time.bb = status.send.s_time.bb;
	event.num[ev_point.forward].s_time.ss = status.send.s_time.ss;

	event.num[ev_point.forward].rtuid = Rtuid ;
	event.num[ev_point.forward].jijum = Jijum ;
	event.num[ev_point.forward].bdid  = RCM_ID;
	event.num[ev_point.forward].cmd[0]= 0xFF  ;
	event.num[ev_point.forward].cmd[1]= 'Q'   ;
	event.num[ev_point.forward].p12   = volts_data.p12_sta;
	event.num[ev_point.forward].p5    = volts_data.p5_sta ;
	event.num[ev_point.forward].m12   = volts_data.m12_sta;

	for(i=0; i<8; i++)	{
		event.num[ev_point.forward].bd_sta [i]   = status.send.bd_sta [i];
		event.num[ev_point.forward].sen_sta[i]   = status.send.sen_sta[i];
	}
	event.num[ev_point.forward].cnt_bd.word_data = status.send.cnt_bd.word_data;

	for(i=0; i<sizeof(status_u); i++)	{
		if(i>3 && i<sizeof(status_u) -6)	{
			event.num[ev_point.forward].xor ^= p[i];
			event.num[ev_point.forward].add += p[i];
		}
	}	status.send.cnt_bd.word_data = 0;	Event_save_flag = OFF;
		event_q(&ev_point);
}
//[*]----------------------------------------------------------------------[*]
void event_q(event_pros_t *ev_pros)
{
	watch();
	ev_pros->forward++;			ev_pros->forward %= EVENT_GETSU;
	if(ev_pros->forward == ev_pros->hear)	{
		ev_pros->hear++;		ev_pros->hear   %= EVENT_GETSU;
	}
}
//[*]----------------------------------------------------------------------[*]
void baud_set(void)
{
	switch(Baud_set_value)	{
		case	1200:	Baud_set_value = BPS1200;	break;
		case	2400:   Baud_set_value = BPS2400;   break;
		case	4800:   Baud_set_value = BPS4800;   break;
		case	9600:   Baud_set_value = BPS9600;   break;
		default     :                               return;
	}
	if     (Baud_set_port ==  2)		init_baud(&com2, Baud_set_value);
	else if(Baud_set_port ==  3)        init_baud(&com3, Baud_set_value);
}
//[*]----------------------------------------------------------------------[*]
void init_baud(protocol_grp_t *comx, word speed)
{
	byte	baud_value = 0;
	word	baud       = 0;

	disable(EA);
	// External comx Interrupt disable command output.
	output( IER(comx->base), 0x00);

	comx->rx_sp   = comx->rx_ep = comx->tx_sp = comx->tx_ep = 0;
	comx->tx_flag = OFF;
	comx->cat_flag= ON ;

	// baud set
	baud_value += ((comx->xtal == M3) ? 4:0);
	switch(speed)	{
		case	BPS9600:    baud_value += 0;
							baud = exter_baud[baud_value];	break;
		case	BPS4800:    baud_value += 1;
							baud = exter_baud[baud_value];	break;
		case	BPS2400:    baud_value += 2;
							baud = exter_baud[baud_value];	break;
		case	BPS1200:    baud_value += 3;
							baud = exter_baud[baud_value];	break;
		default        :									return;
	}
	output( LCR(comx->base),(0x80 | (STOP1|PARITY_NO|DATABIT8)));
	output( DLL(comx->base),        baud   );
	output( DLM(comx->base), 0x00          );
	output( LCR(comx->base),(0x7F & (STOP1|PARITY_NO|DATABIT8)));
	output( IER(comx->base), 0x01          );
	output( MCR(comx->base), 0x0b          );

	Baud_set_flag = OFF;
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void transmit_check(void)
{
	watch();
	if(com1.tx_sp != com1.tx_ep)	{
		if(com1.tx_flag)	{
			disable(EA);
			SBUF = com1.tx_buf[com1.tx_sp++];
			enable(EA);
			if(com1.tx_sp >= BUFFER_SIZE)	com1.tx_sp = 0;
			com1.tx_flag = OFF;
		}
	}	else	com1.cat_flag = ON;

	if(com2.tx_sp != com2.tx_ep)		{
		if(0x20 & (input(LSR(com2.base))))	{
			output(THR(com2.base), com2.tx_buf[com2.tx_sp++]);
			if(com2.tx_sp >= BUFFER_SIZE)		com2.tx_sp = 0;
		}
	}   else	com2.cat_flag = ON;

	if(com3.tx_sp != com3.tx_ep)		{
		if(0x20 & (input(LSR(com3.base))))	{
			output(THR(com3.base), com3.tx_buf[com3.tx_sp++]);
			if(com3.tx_sp >= BUFFER_SIZE)		com3.tx_sp = 0;
		}
	}   else	com3.cat_flag = ON;
}
//[*]----------------------------------------------------------------------[*]
void receive_check(void)
{
	watch();
	if(com1.rx_sp != com1.rx_ep)	{
		if(com1.cat_flag == OFF)				return;
		protocol_event(&com1, com1.rx_buf[com1.rx_sp++]);
		if(com1.rx_sp >= BUFFER_SIZE)	com1.rx_sp = 0;
	}

	if(com2.rx_sp != com2.rx_ep)	{
		if(com2.cat_flag == OFF)				return;
		protocol_event(&com2, com2.rx_buf[com2.rx_sp++]);
		if(com2.rx_sp >= BUFFER_SIZE)	com2.rx_sp = 0;
	}

		if(com3.cat_flag == OFF)				return;
	if(com3.rx_sp != com3.rx_ep)	{
		protocol_event(&com3, com3.rx_buf[com3.rx_sp++]);
		if(com3.rx_sp >= BUFFER_SIZE)	com3.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] void EX0_int (void)     /* External Interrupt 0 */
{
	disable(EA);
	if    (0x4 & (input(IIR(com2.base)) & 0x6))	{
		com2.rx_buf[com2.rx_ep++] = input(RBR(com2.base));
		if(com2.rx_ep >= BUFFER_SIZE)	com2.rx_ep = 0;
	}
	else if(0x4 & (input(IIR(com2.base)) & 0x6))	{
		com3.rx_buf[com3.rx_ep++] = input(RBR(com3.base));
		if(com3.rx_ep >= BUFFER_SIZE)	com3.rx_ep = 0;
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int (void)      /* Timer 0 Overflow */
{
	disable(TR0);
	if(volts_data.count++ > VOLT_WAIT )		{
		if(volts_data.event_count++ > 4)	{
			volts_data.event_count = 0;     Event_check_flag = ON;
		}
		else								volts_data.flag = OFF;
		volts_data.count = 0;
	}
	if(Mpm_check_count++  > MPM_WAIT  )		{
		status.send.bd_sta[0] |= 0x01;		status.send.cnt_bd.bits.reset = FAIL;
		Event_save_flag = ON;
		mpm_reset();						Mpm_check_count = 0 ;
	}
	enable(TR0);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x13] void EX1_int (void)     /* External Interrupt 1 */
{
	word	read_data;

	disable(EX1);
	switch(volts_data.ch)	{
		case	M12_DATA:
			read_data      = input(AD_ADD + volts_data.ch)           ;
			volts_data.m12 = (float)(read_data * 0.0195 * 4.8) * (-1.);
			if((volts_data.m12 > -10) && (volts_data.m12 < -14))	{
				volts_data.m12 =   0;	volts_data.m12_sta = 0x00;
			} 	else					volts_data.m12_sta = 0xAA;
			volts_data.ch = P12_DATA;	break;

		case	P12_DATA:
			read_data      = input(AD_ADD + volts_data.ch);
			volts_data.p12 = (float)((read_data) * 0.0195 * 5.5);
			if((volts_data.p12 < 10) && (volts_data.p12 > 14))		{
				volts_data.p12 = 0 ;	volts_data.p12_sta = 0x00;
			}	else					volts_data.p12_sta = 0xAA;
			volts_data.ch = P5_DATA;	break;

		case	P5_DATA :
			read_data      = input(AD_ADD + volts_data.ch);
			volts_data.p5  = (float)(read_data * 0.0195 * 1.7);
			if((volts_data.p5  <  4) && (volts_data.p5  >   6))		{
				volts_data.p5  = 0;		volts_data.p5_sta  = 0x00;
			}	else					volts_data.p5_sta  = 0xAA;
			volts_data.ch = M12_DATA;	break;

		default         :
			volts_data.ch = M12_DATA;	break;
	}
	enable(EX1);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)
{
	disable(EA);
	if(RI)	{
		disable(RI);
		com1.rx_buf[com1.rx_ep++] = SBUF;
		if(com1.rx_ep >= BUFFER_SIZE)		com1.rx_ep = 0;
	} else if(TI)	{
		disable(TI);
		if(com1.tx_sp != com1.tx_ep)	{
			SBUF = com1.tx_buf[com1.tx_sp++];
			if(com1.tx_sp >= BUFFER_SIZE)	com1.tx_sp = 0;
		} else	com1.tx_flag = ON;
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	com_port_init();	variable_init();	interrupt_init();

	while(1)
	{
		watch();
		receive_check();				transmit_check();

		if(Event_check_flag == ON )		event_check();
		if(Event_save_flag  == ON )		event_save();
		receive_check();				transmit_check();

		if(Event_send_flag  == ON  && com2.cat_flag == ON)		{
			send_event(&com2, ev_point.trace);		ev_point.trace++;
			if(ev_point.trace >= EVENT_GETSU)		ev_point.trace = 0;
			if(ev_point.trace == ev_point.forward)	Event_send_flag = OFF;
		}	receive_check();				transmit_check();

		if(volts_data.flag  == OFF)		{
			output(AD_ADD + volts_data.ch, 0x0);	volts_data.flag = ON;
		}	receive_check();			transmit_check();

		if(Baud_set_flag == ON)			baud_set();
		receive_check();				transmit_check();
	}
}
//[*]----------------------------------------------------------------------[*]

