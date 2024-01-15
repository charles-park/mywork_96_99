//[*]----------------------------------------------------------------------[*]
// Ultra Edit Used
//[*]----------------------------------------------------------------------[*]
#include <io51.h>
#include <stdio.h>
#include <stdlib.h>
#include "LED.H"
#include "SERIAL.HPP"
#include "CHK&CAT.HPP"
#include "N93C46.HPP"
#include "DOT_DISP.HPP"
//[*]----------------------------------------------------------------------[*]
byte read_jumper(void)
{
	byte	value1, value2, count;

	watch();
	value1 = input(JUMPER);

	while(1)	{
		value2 = input(JUMPER);
		if(value1 == value2)	count++;
		else				{	count = 0;	value1 = input(JUMPER);	}
		if(count > 10)			break;
	}
	return	(byte)(~value1);
}
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	word	i;

	watch();
	// Serial EEPROM Control Pin CLEAR
//	CHIP_CS  = 0;	CLK_IN   = 0;	DATA_IN  = 0;	DATA_OUT = 0;
	CHIP_CS  = 0;	CLK_IN   = 0;	DATA_IN  = 0;

	// Serial EEPROM READ
	if(config_read())		factory_setup();

	// Hangul Code Table define
	code_table[0] = &sangyongtable[0][0];
	code_table[1] = &sangyongtable[1][0];
	code_table[2] = &sangyongtable[2][0];

	Start_addr     = 0x0000;	Hi_lo  = 0;	Sunsor = 0;
	comm.byte_data = 0xE0  ;

	for(i=0; i<100; i++)	{	hbuffer[i] = 0x00;	watch();	}
}
//[*]----------------------------------------------------------------------[*]
void serial_init(byte baud)
{
	watch();
	switch(baud)	{
		case	0x00:	init_serial_grp(&com, INTERNAL, 0,   300L, M11, SMOD_OFF, NOT_USED, 1);	break;
		case	0x01:   init_serial_grp(&com, INTERNAL, 0,   600L, M11, SMOD_OFF, NOT_USED, 1);	break;
		case	0x02:   init_serial_grp(&com, INTERNAL, 0,  1200L, M11, SMOD_OFF, NOT_USED, 1);	break;
		case	0x03:   init_serial_grp(&com, INTERNAL, 0,  2400L, M11, SMOD_OFF, NOT_USED, 1);	break;
		case	0x04:   init_serial_grp(&com, INTERNAL, 0,  4800L, M11, SMOD_OFF, NOT_USED, 1);	break;
		case	0x05:   init_serial_grp(&com, INTERNAL, 0,  9600L, M11, SMOD_OFF, NOT_USED, 1);	break;
		case	0x06:   init_serial_grp(&com, INTERNAL, 0, 19200L, M11, SMOD_OFF, NOT_USED, 1);	break;
		case	0x07:   init_serial_grp(&com, INTERNAL, 0,  2400L, M11, SMOD_OFF, NOT_USED, 1);	break;
		default	 	:   init_serial_grp(&com, INTERNAL, 0,  2400L, M11, SMOD_OFF, NOT_USED, 1);	break;
	}
	init_serial_pro  (&com, 0, 17, disp_chk, disp_cat);
	set_protocol_flag(&com, 0, ON);
}
//[*]----------------------------------------------------------------------[*]
void interrupt_init(void)
{
	watch();
	TMOD |= 0x01;	TH0  = 0xFF;	TL0  = 0xE0;
	enable(ET0);	enable(TR0);	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void receive_check(void)
{
	watch();
	// Receive data from MPM
	if(com.rx_sp != com.rx_ep)    {
		protocol_event(&com, com.rx_buf[com.rx_sp++]);
		if(com.rx_sp >= BUFFER_SIZE)   com.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int(void)
{
	disable(TR0);	disable(TF0);
	TH0 = 0xFF;		TL0 = 0xE0;

	switch(Sunsor)	{
		case	0:
			output(COMMON, 0xFF);				Sunsor++;
												break;
		case	1:
			output(HI_DATA, ~hbuffer[Hi_lo++]);
			output(LO_DATA, ~hbuffer[Hi_lo++]);
			output(COMMON , comm.byte_data);    Sunsor++;

			if     (0  <= Hi_lo && Hi_lo < 32)	comm.bits.cs  = 6;
			else if(32 <= Hi_lo && Hi_lo < 64)	comm.bits.cs  = 5;
			else 								comm.bits.cs  = 3;
			if(++comm.bits.cnt >= 16)			comm.bits.cnt = 0;
			if(Hi_lo  >= 96)        			Hi_lo  = 0;
												break;
		default	 :			Sunsor = 0;			break;
	}
	enable(TR0);
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)
{
	disable(ES);
	if(RI)  {
		disable(RI);
		com.rx_buf[com.rx_ep++] = SBUF;
		if(com.rx_ep >= BUFFER_SIZE)       com.rx_ep = 0;
	}   else if(TI) {
		disable(TI);
		if(com.tx_sp != com.tx_ep)    {
			SBUF = com.tx_buf[com.tx_sp++];
			if(com.tx_sp >= BUFFER_SIZE)   com.tx_sp = 0;
		}
	}
	enable (ES);
}
//[*]---------------------------------------------------------------------[*]
void main(void)
{
	word	i;
	byte	j = 0;
	write_mode(ENABLE_CMD   , EWEN_ADDR, 0x0, EWEN_OP);

/*	set_byte(0,0xFF);
	set_byte(1,0xFF);
	set_byte(2,0xFF);
	set_byte(3,0xFE);
	output(0x3000, get_byte(0));
	output(0x3001, get_byte(1));
	output(0x3002, get_byte(2));
	output(0x3003, get_byte(3));	while(1);
*/
	for(i=0; i<sizeof(chk_cfg_u); i++)	{
		set_byte(i,0xFE);			watch();
	}
//	for(i=0; i<sizeof(chk_cfg_u); i++)	{
//		output(0x6000+i, get_byte(i));			watch();
//	}


	variable_init();	interrupt_init();
	while(1)			receive_check();
}
//[*]----------------------------------------------------------------------[*]
//	dot_puts("•·  ¦‚",  0, 1);



