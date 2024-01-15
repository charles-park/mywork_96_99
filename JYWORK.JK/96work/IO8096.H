/* 			- IO8096.H -

   This file #defines the Special Function Registers

   Version: 3.00 [A.R.]

*/

/* Jong Kil Update Header File --> 196Kc */

	/* A/D converter */

#define AD_RESULT_LO 	(* (unsigned char *)	( 0x02 ))
#define AD_RESULT_HI 	(* (unsigned char *)	( 0x03 ))
#define AD_RESULT       (* (unsigned int  *)    ( 0x02 ))   /* JK 196 */
#define AD_COMMAND      (* (unsigned char *)    ( 0x02 ))


	/* High Speed Input */

#define HSI_MODE        (* (unsigned char *)    ( 0x03 ))
#define HSI_TIME        (* (unsigned int  *)    ( 0x04 ))
#define HSI_TIME_LO     (* (unsigned char *)    ( 0x04 ))
#define HSI_TIME_HI     (* (unsigned char *)    ( 0x05 ))
#define HSI_STATUS      (* (unsigned char *)    ( 0x06 ))


	/* High Speed Output */

#define HSO_TIME_LO     (* (unsigned char *)    ( 0x04 ))
#define HSO_TIME_HI     (* (unsigned char *)    ( 0x05 ))
#define HSO_TIME        (* (unsigned int  *)    ( 0x04 ))
#define HSO_COMMAND 	(* (unsigned char *)	( 0x06 ))


	/* Serial port receive/transmit Buffer	*/
 
#define SBUF            (* (unsigned char *)    ( 0x07 ))
#define SBUF_RX         (* (unsigned char *)    ( 0x07 ))
#define SBUF_TX         (* (unsigned char *)    ( 0x07 ))


	/* Interrupt Mask */

#define INT_MASK        (* (unsigned char *)    ( 0x08 ))


	/* Interrupt Pending */

#define INT_PEND        (* (unsigned char *)    ( 0x09 ))


	/* Watchdog Timer */

#define WATCHDOG        (* (unsigned char *)    ( 0x0A ))

    /* I/O Control Register 2 */

#define IOC2            (* (unsigned char *)    ( 0x0B ))


	/* Timer 1 and Timer 2 */

#define TIMER1_LO       (* (unsigned char *)    ( 0x0A ))
#define TIMER1_HI       (* (unsigned char *)    ( 0x0B ))
#define TIMER1          (* (unsigned int  *)    ( 0x0A ))

#define TIMER2_LO       (* (unsigned char *)    ( 0x0C ))
#define TIMER2_HI       (* (unsigned char *)    ( 0x0D ))
#define TIMER2          (* (unsigned int  *)    ( 0x0C ))


	/* Baud Rate */

#define BAUD_RATE       (* (unsigned char *)    ( 0x0E ))


	/* I/O ports */

#define P0              (* (unsigned char *)    ( 0x0E ))
#define P1              (* (unsigned char *)    ( 0x0F ))
#define P2              (* (unsigned char *)    ( 0x10 ))


	/* Serial Port Status */

#define SP_STAT         (* (unsigned char *)    ( 0x11 ))


	/* Serial Port Control */

#define SP_CON          (* (unsigned char *)    ( 0x11 ))


    /* Interrupt Pending 1 Register */

#define INT_PEND1       (* (unsigned char *)    ( 0x12 ))


    /* Interrupt Mask 1 */

#define INT_MASK1       (* (unsigned char *)    ( 0x13 ))


    /* Window Select Register */

#define WSR             (* (unsigned char *)    ( 0x14 ))


	/* I/O status, HSO */
	
#define IOS0            (* (unsigned char *)    ( 0x15 ))


    /* I/O Control Register 0 */

#define IOC0            (* (unsigned char *)    ( 0x15 ))


	/* I/O status, HSI */

#define IOS1            (* (unsigned char *)    ( 0x16 ))


    /* I/O Control Register 1 */

#define IOC1            (* (unsigned char *)    ( 0x16 ))


    /* I/O status. HS2 */

#define IOS2            (* (unsigned char *)    ( 0x17 ))


	/* Pulse Width Modulation Control */
	
#define PWM_CONTROL 	(* (unsigned char *)	( 0x17 ))
#define PWM0_CONTROL    (* (unsigned char *)    ( 0x17 ))

