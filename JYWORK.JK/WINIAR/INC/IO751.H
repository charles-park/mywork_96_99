/*			- IO751.H -

   Special header for the 80C751 processors.
	   
   Version 5.00 [IMAF]

*/

#pragma language=extended

/*===============================*/
/* Predefined SFR Byte Addresses */
/*===============================*/

sfr   P0	= 0x80;
sfr   P1	= 0x90;
sfr   P3	= 0xB0;
sfr   PSW	= 0xD0;
sfr   ACC	= 0xE0;
sfr   B		= 0xF0;
sfr   SP	= 0x81;
sfr   DPL	= 0x82;
sfr   DPH	= 0x83;
sfr   PCON	= 0x87;
sfr   TCON	= 0x88;
sfr   TL	= 0x8A;
sfr   RTL	= 0x8B;
sfr   TH	= 0x8C;
sfr   RTH	= 0x8D;
sfr   IE	= 0xA8;
sfr   I2CFG	= 0xD8;
sfr   I2CON	= 0x98;
sfr   I2DAT	= 0x99;
sfr   I2STA	= 0xF8;

/*==============================*/
/* Predefined SFR Bit Addresses */
/*==============================*/
  
/*========PSW========*/

bit   CY	= 0xD7;
bit   AC	= 0xD6;
bit   F0	= 0xD5;
bit   RS1	= 0xD4;
bit   RS0	= 0xD3;
bit   OV	= 0xD2;
bit   P		= 0xD0;

/*========TCON=======*/

bit   GATE	= 0x8F;
bit   CT	= 0x8E;
bit   TF	= 0x8D;
bit   TR	= 0x8C;
bit   IE0	= 0x8B;
bit   IT0	= 0x8A;
bit   IE1	= 0x89;
bit   IT1	= 0x88;

/*========IE=========*/

bit   EA	= 0xAF;
bit   EI2	= 0xAC;
bit   ET1	= 0xAB;
bit   EX1	= 0xAA;
bit   ET0	= 0xA9;
bit   EX0	= 0xA8;

/*========P0=========*/

bit   SDA	= 0x81;
bit   SCL	= 0x80;

/*========P1=========*/

bit   T0	= 0x97;
bit   INT1	= 0x96;
bit   INT0	= 0x95;

/*========I2CFG=========*/

bit   SLAVEN	= 0xDF;
bit   MASTRQ	= 0xDE;
bit   CLRTI	= 0xDD;
bit   TIRUN	= 0xDC;
bit   CT1	= 0xD9;
bit   CT0	= 0xD8;

/*========I2CON=========*/

bit   RDAT_C	= 0x9F;
bit   ATN	= 0x9E;
bit   DRDY	= 0x9D;
bit   ARL	= 0x9C;
bit   STR	= 0x9B;
bit   STP	= 0x9A;
bit   MASTER	= 0x99;

bit   CXA	= 0x9F;
bit   IDLE_C	= 0x9E;
bit   CDR	= 0x9D;
bit   CARL	= 0x9C;
bit   CSTR	= 0x9B;
bit   CSTP	= 0x9A;
bit   XSTR_C	= 0x99;
bit   XSTP_C	= 0x98;

/*========I2DAT=========*/

bit   RDAT_D	= 0x87;

bit   XDAT	= 0x87;

/*========I2STA=========*/

bit   IDLE_S	= 0xFE;
bit   XDATA	= 0xFD;
bit   XACTV	= 0xFC;
bit   MAKSTR	= 0xFB;
bit   MAKSTP	= 0xFA;
bit   XSTR_S	= 0xF9;
bit   XSTP_S	= 0xF8;

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

interrupt [0x03] void EX0_int (void);	  /* External Interrupt 0 */

interrupt [0x0B] void T0_int (void);      /* Timer 0 Overflow */

interrupt [0x13] void EX1_int (void);	  /* External Interrupt 1 */

interrupt [0x1B] void T1_int (void);      /* Timer 1 Overflow */

interrupt [0x23] void I2C_int (void);     /* I2C Interrupt */


