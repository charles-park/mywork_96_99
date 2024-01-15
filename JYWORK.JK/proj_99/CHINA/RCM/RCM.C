//[*]----------------------------------------------------------------------[*]
#include <io51.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rcm.h"
#include "send.hpp"
#include "serial.hpp"
#include "chk&cat.hpp"
//[*]----------------------------------------------------------------------[*]
#pragma memory=code
Byte	NAME[20][60] = {
   "[*]-----------------------------------------------------[*]",
   "[*]	 JIN-YANG  INDUSTRIAL.,CO LTD                       [*]",
   "[*]  Program : PARK. JongKil    Date : 1999. 1. 10~     [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  Project NAME : CHINA                               [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  BOARD : RCM BOARD                                  [*]",
   "[*]-----------------------------------------------------[*]",
   "[*]  VERSION : 1.01                                     [*]",
   "[*]-----------------------------------------------------[*]",
};
#pragma memory=default
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
//   Com Port Initiliaze ( Baud & Serial Group & Protocol Size .. ETC)
//[*]----------------------------------------------------------------------[*]
void com_port_initiliaze(void)
{
	init_serial_grp(&com_int, INTERNAL, 0, BPS9600, M11, SMOD_OFF, NOT_USED, 1);
	init_serial_pro(&com_int, 0, 26, rx_check, rx_catch_int);
	set_protocol_flag(&com_int, 0, ON);

	init_serial_grp(&com_ext, EXTERNAL, EXT_COM, BPS9600, M1, NOT_USED,
											(STOP1|PARITY_NO|DATABIT8), 1);
	init_serial_pro(&com_ext, 0, 17, rx_check, rx_catch_ext);
	set_protocol_flag(&com_ext, 0, ON);
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
//   Transmit Data Ready??
//[*]----------------------------------------------------------------------[*]
void transmit_data_check(void)
{
	if(com_int.tx_sp != com_int.tx_ep)	{
		if(com_int.tx_flag)		{
			disable(EA);
			TX_LED1(ON);
			SBUF = com_int.tx_buf[com_int.tx_sp++];
			if(com_int.tx_sp >= BUFFER_SIZE)	com_int.tx_sp = 0;
			com_int.tx_flag = OFF;
			enable(EA);
		}
	} else	TX_LED1(OFF);

	if(com_ext.tx_sp != com_ext.tx_ep)	{
		disable(EA);
		TX_LED2(ON);
		if(0x20 & (input(LSR(com_ext.base))))	{
			output(THR(com_ext.base), com_ext.tx_buf[com_ext.tx_sp++]);
			if(com_ext.tx_sp >= BUFFER_SIZE)	com_ext.tx_sp = 0;
		}
		enable(EA);
	} else	TX_LED2(OFF);
}
//[*]----------------------------------------------------------------------[*]
//   Receive Data Ready??
//[*]----------------------------------------------------------------------[*]
void receive_data_check(void)
{
	if(com_int.rx_sp != com_int.rx_ep)	{
		protocol_event(&com_int, com_int.rx_buf[com_int.rx_sp++]);
		if(com_int.rx_sp >= BUFFER_SIZE)	com_int.rx_sp = 0;
	}
	if(com_ext.rx_sp != com_ext.rx_ep)	{
		protocol_event(&com_ext, com_ext.rx_buf[com_ext.rx_sp++]);
		if(com_ext.rx_sp >= BUFFER_SIZE)	com_ext.rx_sp = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
//   Processing Voltage Function
//[*]----------------------------------------------------------------------[*]
void pros_voltage(void)
{
	float 	value, cmp_value;

	value     = read_volt_ararry[pros_volt_ch];
	value     = (float)(0.196 * value);
	cmp_value = (float)(0.196 * 153. );					// cmp value is 3V

	if     (pros_volt_ch == 0)	mpm.send.volt.pv5_1   = (word)(value*2.);
	else if(pros_volt_ch == 1)	mpm.send.volt.pv5_2   = (word)(value*2.);
	else if(pros_volt_ch == 2)	mpm.send.volt.pv12_1  = (word)(value*3.);
	else if(pros_volt_ch == 3)	mpm.send.volt.pv12_2  = (word)(value*3.);
	else if(pros_volt_ch == 4)	mpm.send.volt.mv12_1  = (word)(value*4.);
	else if(pros_volt_ch == 5)	mpm.send.volt.mv12_2  = (word)(value*4.);
	else if(pros_volt_ch == 6)	mpm.send.volt.battery = (word)(value*3.);
	else if(pros_volt_ch == 7)	{
		if     (value>10 && value<cmp_value)	mpm.send.volt.ac_volt = 1100;
		else if(cmp_value < value)				mpm.send.volt.ac_volt = 2200;
		else     								mpm.send.volt.ac_volt = 0;
	}
	else if(pros_volt_ch == 8)	mpm.send.volt.fan     = (word)(value*3.);
	else if(pros_volt_ch == 9)	mpm.send.volt.door    = (word)(value*2.);
	else if(pros_volt_ch == 10)	mpm.send.volt.wd      = (word)(value*3.);
	else if(pros_volt_ch == 11)	mpm.send.volt.ws      = (word)(value*3.);
	else if(pros_volt_ch == 12)	mpm.send.volt.ped     = (word)(value*3.);
	else if(pros_volt_ch == 13)	mpm.send.volt.prs     = (word)(value*3.);
	else if(pros_volt_ch == 14)	mpm.send.volt.rh      = (word)(value*3.);
	else if(pros_volt_ch == 15)	mpm.send.volt.tsp     = (word)(value*3.);

	if(++pros_volt_ch >= 16)		pros_volt_ch = 0;
	volts_status_check(&mpm.send.sta.volt.bits);
}
//[*]----------------------------------------------------------------------[*]
//   Voltage Status Check Function
//[*]----------------------------------------------------------------------[*]
void volts_status_check(volt_bit_t *d)
{
	d->pv5_1   = (mpm.send.volt.pv5_1   >   40) ? NORMAL : FAIL ;
	d->pv5_2   = (mpm.send.volt.pv5_2   >   40) ? NORMAL : FAIL ;
	d->pv12_1  = (mpm.send.volt.pv12_1  >  110) ? NORMAL : FAIL ;
	d->pv12_2  = (mpm.send.volt.pv12_2  >  110) ? NORMAL : FAIL ;
	d->mv12_1  = (mpm.send.volt.mv12_1  >  110) ? NORMAL : FAIL ;
	d->mv12_2  = (mpm.send.volt.mv12_2  >  110) ? NORMAL : FAIL ;
	d->battery = (mpm.send.volt.battery >  110) ? NORMAL : FAIL ;
	d->ac_volt = (mpm.send.volt.ac_volt > 1000) ? NORMAL : FAIL ;
	d->fan     = (mpm.send.volt.fan     >  110) ? NORMAL : FAIL ;
	d->door    = (mpm.send.volt.door    >   40) ? NORMAL : FAIL ;
	d->wd      = (mpm.send.volt.wd      >  110) ? NORMAL : FAIL ;
	d->ws      = (mpm.send.volt.ws      >  110) ? NORMAL : FAIL ;
	d->ped     = (mpm.send.volt.ped     >  110) ? NORMAL : FAIL ;
	d->prs     = (mpm.send.volt.prs     >  110) ? NORMAL : FAIL ;
	d->rh      = (mpm.send.volt.rh      >  110) ? NORMAL : FAIL ;
	d->tsp     = (mpm.send.volt.tsp     >  110) ? NORMAL : FAIL ;
}
//[*]----------------------------------------------------------------------[*]
//   MPM Reset Function(RTU All RESET)
//[*]----------------------------------------------------------------------[*]
void mpm_reset(void)
{
	word	i;
	MPM_RESET(ON);
	for(i=0; i < 1000; i++)	;
	mpm_status = 0;			call_count = 0;
	MPM_RESET(OFF);
}
//[*]----------------------------------------------------------------------[*]
//   Interrupt Initiliaze & Variable Initiliaze
//[*]----------------------------------------------------------------------[*]
void variable_initiliaze(void)
{
	word 	i;

	for(i=0; i<20; i++)		read_volt_ararry[i] = 0;
	msb_volt_ch  = 0;		lsb_volt_ch  = 0;
	mpm_status   = 0;		pros_volt_ch = 0;		call_count = 0;
	LOG_ID       = 0;
	RX_LED1(OFF);			RX_LED2(OFF);
	TX_LED1(OFF);			TX_LED2(OFF);
	enable(IT0);			enable(IT1);
	enable(EX0);			enable(EX1);
	MPM_RESET(OFF);			channel_select = LSB;
}
//[*]----------------------------------------------------------------------[*]
//   4 Hz Processing Function ( Check MPM Communication Time )
//[*]----------------------------------------------------------------------[*]
interrupt [0x03] void EX0_int (void)    /* External Interrupt 0 */
{
	disable(EX0);	mpm_status++;	enable(EX0);
}
//[*]----------------------------------------------------------------------[*]
//   External Serial Interrupt Processing Function
//[*]----------------------------------------------------------------------[*]
interrupt [0x13] void EX1_int (void)     /* External Interrupt 1 */
{
	disable(EA);
	RX_LED2(ON);
	if(0x4 & (input(IIR(com_ext.base)) & 0x6))	{
		com_ext.rx_buf[com_ext.rx_ep++] = input(RBR(com_ext.base));
		if(com_ext.rx_ep >= BUFFER_SIZE)	com_ext.rx_ep = 0;
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
//   Internal Serial Interrupt Processing Function
//[*]----------------------------------------------------------------------[*]
interrupt [0x23] void SCON_int (void)    /* Serial Port */
{
	disable(EA);
	if(RI)	{
		disable(RI);
		RX_LED1(ON);
		com_int.rx_buf[com_int.rx_ep++] = SBUF;
		if(com_int.rx_ep >= BUFFER_SIZE)		com_int.rx_ep = 0;
	} else	if(TI)	{
		disable(TI);
		if(com_int.tx_sp != com_int.tx_ep)	{
			SBUF = com_int.tx_buf[com_int.tx_sp++];
			if(com_int.tx_sp >= BUFFER_SIZE)	com_int.tx_sp = 0;
		} else	com_int.tx_flag = ON;
	}
	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
//   Voltage Read (ADC READ)
//[*]----------------------------------------------------------------------[*]
void read_voltage(word ad_select)
{
	if(ad_select == ADC_LSB)	{
		read_volt_ararry[lsb_volt_ch] = input(ad_select + lsb_volt_ch);
		if(lsb_volt_ch >= 7)	lsb_volt_ch = 0;
		else					lsb_volt_ch++  ;
		output(ADC_MSB+msb_volt_ch,0x00);
	}
	else if(ad_select == ADC_MSB)	{
	//  msb_volt_ch + ararry offset
		read_volt_ararry[msb_volt_ch + 8] = input(ad_select + msb_volt_ch);
		if(msb_volt_ch >= 7)	msb_volt_ch = 0;
		else					msb_volt_ch++  ;
		output(ADC_LSB+lsb_volt_ch, 0x0);	// Converting Start Command
	}
}
//[*]----------------------------------------------------------------------[*]
//   Start Program!!!
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	variable_initiliaze();				com_port_initiliaze();

	while(1)	{
		receive_data_check();			transmit_data_check();
		if(ADC_LSB_EOC && ADC_MSB_EOC)	{
			if(channel_select == LSB)	{
				read_voltage(ADC_LSB);
				channel_select = MSB;
			} else	if(channel_select == MSB)	{
				read_voltage(ADC_MSB);
				channel_select = LSB;
			}
		}
		receive_data_check();			transmit_data_check();
		if(mpm_status >= ERROR_TIME)	mpm_reset();
		receive_data_check();			transmit_data_check();
		pros_voltage();
	}
}
//[*]----------------------------------------------------------------------[*]
