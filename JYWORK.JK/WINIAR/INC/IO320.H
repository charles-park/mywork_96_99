/*			- IO320.H -

   Special header for the 80C320, 80C520, 80C530 processor.
	   
   Version 5.00 [IMAF]

*/

#pragma language=extended

/*===============================*/
/* Predefined SFR Byte Addresses */
/*===============================*/

sfr   P0	= 0x80;
sfr   P1	= 0x90;
sfr   P2	= 0xA0;
sfr   P3	= 0xB0;
sfr   PSW	= 0xD0;
sfr   ACC	= 0xE0;
sfr   B		= 0xF0;
sfr   SP	= 0x81;
sfr   DPL	= 0x82;
sfr   DPH	= 0x83;
sfr   DPL1	= 0x84;
sfr   DPH1	= 0x85;
sfr   DPS	= 0x86;
sfr   PCON	= 0x87;
sfr   TCON	= 0x88;
sfr   TMOD	= 0x89;
sfr   TL0	= 0x8A;
sfr   TL1	= 0x8B;
sfr   TH0	= 0x8C;
sfr   TH1	= 0x8D;
sfr   IE	= 0xA8;
sfr   IP	= 0xB8;
sfr   SCON	= 0x98;
sfr   SCON0	= 0x98;
sfr   SBUF	= 0x99;
sfr   SBUF0	= 0x99;
sfr   T2CON	= 0xC8;
sfr   RCAP2L    = 0xCA;
sfr   RCAP2H    = 0xCB;
sfr   TL2	= 0xCC;
sfr   TH2	= 0xCD;
sfr   CKCON	= 0x8E;
sfr   EXIF	= 0x91;
sfr   SADDR0	= 0xA9;
sfr   SADDR1	= 0xAA;
sfr   SADEN0	= 0xB9;
sfr   SADEN1	= 0xBA;
sfr   SCON1	= 0xC0;
sfr   SBUF1	= 0xC1;
sfr   TA	= 0xC7;
sfr   T2MOD	= 0xC9;
sfr   WDCON	= 0xD8;
sfr   EIE	= 0xE8;
sfr   EIP	= 0xF8;

/* Only for 80C520 and 80C530 */
sfr   PMR	= 0xC4;
sfr   STATUS	= 0xC5;

/* Only for 80C530 */
sfr   RTASS	= 0xF2;
sfr   RTAS	= 0xF3;
sfr   RTASM	= 0xF4;
sfr   RTASH	= 0xF5;
sfr   RTCC	= 0xF9;
sfr   RTCSS	= 0xFA;
sfr   RTCS	= 0xFB;
sfr   RTCM	= 0xFC;
sfr   RTCH	= 0xFD;
sfr   RTCD0	= 0xFE;
sfr   RTCD1	= 0xFF;

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
bit   FL	= 0xD1;
bit   P		= 0xD0;

/*========TCON=======*/

bit   TF1	= 0x8F;
bit   TR1	= 0x8E;
bit   TF0	= 0x8D;
bit   TR0	= 0x8C;
bit   IE1	= 0x8B;
bit   IT1	= 0x8A;
bit   IE0	= 0x89;
bit   IT0	= 0x88;

/*========IE=========*/

bit   EA	= 0xAF;
bit   ES1	= 0xAE;
bit   ET2	= 0xAD;
bit   ES	= 0xAC;
bit   ES0	= 0xAC;
bit   ET1	= 0xAB;
bit   EX1	= 0xAA;
bit   ET0	= 0xA9;
bit   EX0	= 0xA8;

/*========IP=========*/

bit   PS1	= 0xBE;
bit   PT2	= 0xBD;
bit   PS	= 0xBC;
bit   PS0	= 0xBC;
bit   PT1	= 0xBB;
bit   PX1	= 0xBA;
bit   PT0	= 0xB9;
bit   PX0	= 0xB8;

/*========P3=========*/

bit   RD	= 0xB7;
bit   WR	= 0xB6;
bit   T1	= 0xB5;
bit   T0	= 0xB4;
bit   INT1	= 0xB3;
bit   INT0	= 0xB2;
bit   TXD	= 0xB1;
bit   RXD	= 0xB0;

/*========SCON0=======*/

bit   SM0_0	= 0x9F;
bit   SM1_0	= 0x9E;
bit   SM2_0	= 0x9D;
bit   REN_0	= 0x9C;
bit   TB8_0	= 0x9B;
bit   RB8_0	= 0x9A;
bit   TI_0	= 0x99;
bit   RI_0	= 0x98;

/*========SCON1=======*/

bit   SM0_1	= 0xC7;
bit   SM1_1	= 0xC6;
bit   SM2_1	= 0xC5;
bit   REN_1	= 0xC4;
bit   TB8_1	= 0xC3;
bit   RB8_1	= 0xC2;
bit   TI_1	= 0xC1;
bit   RI_1	= 0xC0;

/*========T2CON=======*/

bit   TF2	= 0xCF;
bit   EXF2	= 0xCE;
bit   RCLK	= 0xCD;
bit   TCLK	= 0xCC;
bit   EXEN2	= 0xCB;
bit   TR2	= 0xCA;
bit   C_T2	= 0xC9;
bit   CP_RL2	= 0xC8;

/*========WDCON=======*/

bit   SMOD	= 0xDF;
bit   POR	= 0xDE;
bit   EPFI	= 0xDD;
bit   PFI	= 0xDC;
bit   WDIF	= 0xDB;
bit   WTRF	= 0xDA;
bit   EWT	= 0xD9;
bit   RWT	= 0xD8;

/*========EIE=======*/

bit   EWDI	= 0xEC;
bit   EX5	= 0xEB;
bit   EX4	= 0xEA;
bit   EX3	= 0xE9;
bit   EX2	= 0xE8;

/* Only for 80C530 */
bit   ERTCI	= 0xED;

/*========EIP=======*/

bit   PWDI	= 0xFC;
bit   PX5	= 0xFB;
bit   PX4	= 0xFA;
bit   PX3	= 0xF9;
bit   PX2	= 0xF8;

/* Only for 80C530 */
bit   PRTCI	= 0xFD;


/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

interrupt [0x03] void EX0_int (void);	  /* External Interrupt 0 */

interrupt [0x0B] void T0_int (void);      /* Timer 0 Overflow */

interrupt [0x13] void EX1_int (void);	  /* External Interrupt 1 */

interrupt [0x1B] void T1_int (void);      /* Timer 1 Overflow */

interrupt [0x23] void SCON0_int (void);   /* Serial Port 0 */

interrupt [0x2B] void T2_int (void);      /* Timer 2 Overflow */

interrupt [0x33] void PFI_int (void);     /* Power Fail Interrupt */

interrupt [0x3B] void SCON1_int (void);   /* Serial Port 1 */

interrupt [0x43] void EX2_int (void);     /* External Interrupt 2 */

interrupt [0x4B] void EX3_int (void);     /* External Interrupt 3 */

interrupt [0x53] void EX4_int (void);     /* External Interrupt 4 */

interrupt [0x5B] void EX5_int (void);     /* External Interrupt 5 */

interrupt [0x63] void WDTI_int (void);    /* Watchdog Time-out Interrupt */

/* Only 80C530 */
interrupt [0x6B] void RTCI_int (void);    /* Real-Time Clock Interrupt */

