/*			- IO515.H -

   Special header for the 80C515A and 80C535A processors.

   Version 5.00 [IMAF]
	   
*/

#pragma language=extended

/*===============================*/
/* Predefined SFR Byte Addresses */
/*===============================*/

sfr   ADDATL	= 0xDA;
sfr   ADDATH	= 0xD9;
sfr   ADCON0   	= 0xD8;
sfr   ADCON1	= 0xDC;
sfr   P0	= 0x80;
sfr   P1	= 0x90;
sfr   P2	= 0xA0;
sfr   P3	= 0xB0;
sfr   P4	= 0xE8;
sfr   P5	= 0xF8;
sfr   P6	= 0xDB;
sfr   PSW	= 0xD0;
sfr   ACC	= 0xE0;
sfr   B	   	= 0xF0;
sfr   SP	= 0x81;
sfr   DPL	= 0x82;
sfr   DPH	= 0x83;
sfr   PCON	= 0x87;
sfr   TCON	= 0x88;
sfr   TMOD	= 0x89;
sfr   TL0	= 0x8A;
sfr   TL1	= 0x8B;
sfr   TL2	= 0xCC;
sfr   TH0	= 0x8C;
sfr   TH1	= 0x8D;
sfr   TH2	= 0xCD;
sfr   IEN0	= 0xA8;
sfr   IP0	= 0xA9;
sfr   IEN1	= 0xB8;
sfr   IP1	= 0xB9;
sfr   IRCON	= 0xC0;
sfr   CCEN	= 0xC1;
sfr   CCL1	= 0xC2;
sfr   CCH1	= 0xC3;
sfr   CCL2	= 0xC4;
sfr   CCH2	= 0xC5;
sfr   CCL3	= 0xC6;
sfr   CCH3	= 0xC7;
sfr   CRCL	= 0xCA;
sfr   CRCH	= 0xCB;
sfr   T2CON    	= 0xC8;
sfr   SCON	= 0x98;
sfr   SBUF	= 0x99;
sfr   XPAGE	= 0x91;
sfr   SRELL	= 0xAA;
sfr   SRELH	= 0xBA;
sfr   SYSCON	= 0xB1;
  
/*==============================*/
/* Predefined SFR Bit Addresses */
/*==============================*/

/*========ADCON0=======*/

bit   BD	= 0xDF;
bit   BSY	= 0xDC;
bit   ADM	= 0xDB;
bit   MX2	= 0xDA;
bit   MX1	= 0xD9;
bit   MX0	= 0xD8;

/*========IRCON=======*/

bit   EXF2	= 0xC7;
bit   TF2	= 0xC6;
bit   IEX6	= 0xC5;
bit   IEX5	= 0xC4;
bit   IEX4	= 0xC3;
bit   IEX3	= 0xC2;
bit   IEX2	= 0xC1;
bit   IADC  	= 0xC0;

/*========T2CON=======*/

bit   T2PS	= 0xCF;
bit   I3FR	= 0xCE;
bit   I2FR	= 0xCD;
bit   T2R1	= 0xCC;
bit   T2R0	= 0xCB;
bit   T2CM	= 0xCA;
bit   T2I1	= 0xC9;
bit   T2I0	= 0xC8;

/*========PSW=========*/

bit   CY	= 0xD7;
bit   AC	= 0xD6;
bit   F0	= 0xD5;
bit   RS1	= 0xD4;
bit   RS0	= 0xD3;
bit   OV	= 0xD2;
bit   F1	= 0xD1;
bit   P	  	= 0xD0;

/*========TCON========*/

bit   TF1	= 0x8F;
bit   TR1	= 0x8E;
bit   TF0	= 0x8D;
bit   TR0	= 0x8C;
bit   IE1	= 0x8B;
bit   IT1	= 0x8A;
bit   IE0	= 0x89;
bit   IT0	= 0x88;

/*========IEN0========*/

bit   EAL	= 0xAF;
bit   WDT	= 0xAE;
bit   ET2	= 0xAD;
bit   ES	= 0xAC;
bit   ET1	= 0xAB;
bit   EX1	= 0xAA;
bit   ET0	= 0xA9;
bit   EX0	= 0xA8;

/*========IEN1========*/

bit   EXEN2	= 0xBF;
bit   SWDT	= 0xBE;
bit   EX6	= 0xBD;
bit   EX5	= 0xBC;
bit   EX4	= 0xBB;
bit   EX3	= 0xBA;
bit   EX2	= 0xB9;
bit   EADC	= 0xB8;

/*========P3=========*/

bit   RD	= 0xB7;
bit   WR	= 0xB6;
bit   T1	= 0xB5;
bit   T0	= 0xB4;
bit   INT1	= 0xB3;
bit   INT0	= 0xB2;
bit   TXD	= 0xB1;
bit   RXD	= 0xB0;

/*========SCON========*/

bit   SM0	= 0x9F;
bit   SM1	= 0x9E;
bit   SM2	= 0x9D;
bit   REN	= 0x9C;
bit   TB8	= 0x9B;
bit   RB8	= 0x9A;
bit   TI	= 0x99;
bit   RI	= 0x98;

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

interrupt [0x03] void EX0_int (void);	  /* External Interrupt 0 */

interrupt [0x0B] void T0_int (void);      /* Timer 0 Overflow */

interrupt [0x13] void EX1_int (void);	  /* External Interrupt 1 */

interrupt [0x1B] void T1_int (void);      /* Timer 1 Overflow */

interrupt [0x23] void SCON_int (void);    /* Serial Port */

interrupt [0x2B] void T2_int (void);      /* Timer 2 Overflow */

interrupt [0x2B] void T2EX_int (void);    /* Negative Transition on T2EX */

interrupt [0x43] void ADC_int (void);     /* ADC Interrupt */

interrupt [0x4B] void EX2_int (void);	  /* External Interrupt 2 */

interrupt [0x53] void EX3_int (void);	  /* External Interrupt 3 */

interrupt [0x5B] void EX4_int (void);	  /* External Interrupt 4 */

interrupt [0x63] void EX5_int (void);	  /* External Interrupt 5 */

interrupt [0x6B] void EX6_int (void);	  /* External Interrupt 6 */



