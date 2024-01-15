/*                      - IO196_KC.H -

   This file defines the Special Function Registers for 80196KC

   Version: 5.10A [IMAF]

*/

#ifndef C196KC

#define C196KC

		/*=================*/
		/* SFR Definitions */
		/*=================*/


#pragma language=extended


        /* General */

sfrw		ZERO_REG 	 = 	  0x00;

	/* A/D converter */

const sfrb	AD_RESULT_LO	 =	  0x02;
const sfrb	AD_RESULT_HI 	 = 	  0x03;
const sfrw	AD_RESULT	 =	  0x02;
sfrb		AD_COMMAND 	 = 	  0x02;


	/* High Speed Input */

sfrb		HSI_MODE 	 = 	  0x03;
const sfrb	HSI_TIME_LO 	 = 	  0x04;
const sfrb	HSI_TIME_HI 	 = 	  0x05;
const sfrw	HSI_TIME 	 = 	  0x04;
const sfrb	HSI_STATUS 	 = 	  0x06;


	/* High Speed Output */

sfrb		HSO_TIME_LO 	 = 	  0x04;
sfrb		HSO_TIME_HI 	 = 	  0x05;
sfrw		HSO_TIME 	 = 	  0x04;
sfrb		HSO_COMMAND 	 = 	  0x06;


	/* Serial port receive/transmit Buffer	*/
 
sfrb		SBUF	 	 = 	  0x07;
const sfrb	SBUF_RX	 	 = 	  0x07;
sfrb		SBUF_TX	 	 = 	  0x07;


	/* Interrupt Mask */

sfrb		INT_MASK 	 = 	  0x08;


	/* Interrupt Pending */

sfrb		INT_PEND 	 = 	  0x09;


	/* Watchdog Timer */

sfrb		WATCHDOG 	 = 	  0x0A;


	/* I/O Control Register 2 */

sfrb		IOC2	 	 = 	  0x0B;


	/* Timer 1 and Timer 2 */

const sfrb	TIMER1_LO 	 = 	  0x0A;
const sfrb	TIMER1_HI 	 = 	  0x0B;
const sfrw	TIMER1	 	 = 	  0x0A;

sfrb		TIMER2_LO 	 = 	  0x0C;
sfrb		TIMER2_HI 	 = 	  0x0D;
sfrw		TIMER2	 	 = 	  0x0C;


	/* Baud Rate */

sfrb		BAUD_RATE 	 = 	  0x0E;

	/* I/O ports */

const sfrb	IO_PORT0 	 = 	  0x0E;
sfrb		IO_PORT1 	 = 	  0x0F;
sfrb		IO_PORT2 	 = 	  0x10;


	/* Serial Port Status */

const sfrb	SP_STAT	 	 = 	  0x11;


	/* Serial Port Control */

sfrb		SP_CON	 	 = 	  0x11;


	/* Interrupt Pending 1 */

sfrb		INT_PEND1 	 = 	  0x12;


	/* Interrupt Mask 1 */

sfrb		INT_MASK1 	 = 	  0x13;

        /* Window select register */

sfrb		WSR		 =	  0x14;

	/* I/O status, HSO */
	
const sfrb	IOS0	 	 = 	  0x15;


	/* I/O Control Register 0 */

sfrb		IOC0	 	 = 	  0x15;


        /* I/O status, HS1 */

const sfrb	IOS1	 	 = 	  0x16;



	/* I/O Control Register 1 */

sfrb		IOC1	 	 = 	  0x16;


        /* I/O status, HS2 */

const sfrb	IOS2	 	 = 	  0x17;


	/* Pulse Width Modulation Control */
	
sfrb		PWM_CONTROL 	 = 	  0x17;

sfrb		PWM0_CONTROL 	 = 	  0x17;




#ifdef __IAR_SYSTEMS_ICC

		/*=======================*/
		/* Interrupt Definitions */
		/*=======================*/


		/* Timer overflow */
interrupt [0x00] void TM_OVFL_interrupt(void);

		/* A/D conversion complete*/
interrupt [0x02] void AD_COMPL_interrupt(void);

		/* High speed input data available */
interrupt [0x04] void HSI_AV_interrupt(void);

		/* High speed output*/
interrupt [0x06] void HSO_interrupt(void);

		/* HSI.0 Pin*/
interrupt [0x08] void HSI0_interrupt(void);

		/* Software timers*/
interrupt [0x0A] void SW_TM_interrupt(void);

		/* Serial port */
interrupt [0x0C] void SIO_interrupt(void);

		/* EXTINT */
interrupt [0x0E] void EXTINT_interrupt(void);

		/* Trap (special interrupt)*/
interrupt [0x10] void TRAP_interrupt(void);

		/* unimplemented opcode (special interrupt) */
interrupt [0x12] void UNIMP_OPC_interrupt(void);

		/* TI */
interrupt [0x30] void TI_interrupt(void);

		/* RI*/
interrupt [0x32] void RI_interrupt(void);

		/* Fourth entry into HSI_FIFO*/
interrupt [0x34] void HSI_FIFO_4_interrupt(void);

		/* Timer2 capture*/
interrupt [0x36] void TM2_CAP_interrupt(void);

		/* Timer2 overflow */
interrupt [0x38] void TM2_OVFL_interrupt(void);

		/* EXTINT Pin */
interrupt [0x3A] void EXTINT_PIN_interrupt(void);

		/* HSI FIFO full */
interrupt [0x3C] void HSI_FIFO_FULL_interrupt(void);

		/* Non maskable interrupt */
interrupt [0x3E] void NMI_interrupt(void);




		/*===========================*/
		/* PTS interrupt Definitions */
		/*===========================*/


		/* Timer overflow */
interrupt [0x40] void TM_OVFL_PTS_interrupt(void);

		/* A/D conversion complete*/
interrupt [0x42] void AD_COMPL_PTS_interrupt(void);

		/* High speed input data available */
interrupt [0x44] void HSI_AV_PTS_interrupt(void);

		/* High speed output*/
interrupt [0x46] void HSO_PTS_interrupt(void);

		/* HSI.0 Pin*/
interrupt [0x48] void HSI0_PTS_interrupt(void);

		/* Software timers*/
interrupt [0x4A] void SW_TM_PTS_interrupt(void);

		/* Serial port */
interrupt [0x4C] void SIO_PTS_interrupt(void);

		/* EXTINT */
interrupt [0x4E] void EXTINT_PTS_interrupt(void);

		/* TI */
interrupt [0x50] void TI_PTS_interrupt(void);

		/* RI*/
interrupt [0x52] void RI_PTS_interrupt(void);

		/* Fourth entry into HSI_FIFO*/
interrupt [0x54] void HSI_FIFO_4_PTS_interrupt(void);

		/* Timer2 capture*/
interrupt [0x56] void TM2_CAP_PTS_interrupt(void);

		/* Timer2 overflow */
interrupt [0x58] void TM2_OVFL_PTS_interrupt(void);

		/* EXTINT Pin */
interrupt [0x5A] void EXTINT_PIN_PTS_interrupt(void);

		/* HSI FIFO full */
interrupt [0x5C] void HSI_FIFO_FULL_PTS_interrupt(void);



#endif

#endif
