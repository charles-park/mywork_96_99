/*			- IO552.H -

   Special header for the 80C552/562 processors.
	   
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
sfr   P4	= 0xC0;
sfr   P5	= 0xC4;
sfr   PSW	= 0xD0;
sfr   ACC	= 0xE0;
sfr   B		= 0xF0;
sfr   SP	= 0x81;
sfr   DPL	= 0x82;
sfr   DPH	= 0x83;
sfr   PCON	= 0x87;
sfr   TCON	= 0x88;
sfr   TMOD	= 0x89;
sfr   TL0	= 0x8A;
sfr   TL1	= 0x8B;
sfr   TH0	= 0x8C;
sfr   TH1	= 0x8D;
sfr   S0CON	= 0x98;
sfr   S0BUF	= 0x99;
sfr   IEN0	= 0xA8;
sfr   CML0	= 0xA9;
sfr   CML1	= 0xAA;
sfr   CML2	= 0xAB;
sfr   CTL0	= 0xAC;
sfr   CTL1	= 0xAD;
sfr   CTL2	= 0xAE;
sfr   CTL3	= 0xAF;
sfr   IP0	= 0xB8;
sfr   ADCON	= 0xC5;
sfr   ADCH	= 0xC6;
sfr   TM2IR	= 0xC8;
sfr   CMH0	= 0xC9;
sfr   CMH1	= 0xCA;
sfr   CMH2	= 0xCB;
sfr   CTH0	= 0xCC;
sfr   CTH1	= 0xCD;
sfr   CTH2	= 0xCE;
sfr   CTH3	= 0xCF;
sfr   S1CON	= 0xD8;
sfr   S1STA	= 0xD9;
sfr   S1DAT	= 0xDA;
sfr   S1ADR	= 0xDB;
sfr   IEN1	= 0xE8;
sfr   TM2CON	= 0xEA;
sfr   CTCON	= 0xEB;
sfr   TML2	= 0xEC;
sfr   TMH2	= 0xED;
sfr   STE	= 0xEE;
sfr   RTE	= 0xEF;
sfr   IP1	= 0xF8;
sfr   PWM0	= 0xFC;
sfr   PWM1	= 0xFD;
sfr   PWMP	= 0xFE;
sfr   T3	= 0xFF;

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
bit   F1	= 0xD1;
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

/*========IEN0=========*/

bit   EA	= 0xAF;
bit   EAD	= 0xAE;
bit   ES1	= 0xAD;
bit   ES0	= 0xAC;
bit   ET1	= 0xAB;
bit   EX1	= 0xAA;
bit   ET0	= 0xA9;
bit   EX0	= 0xA8;

/*========IEN1=========*/

bit   ET2	= 0xEF;
bit   ECM2	= 0xEE;
bit   ECM1	= 0xED;
bit   ECM0	= 0xEC;
bit   ECT3	= 0xEB;
bit   ECT2	= 0xEA;
bit   ECT1	= 0xE9;
bit   ECT0	= 0xE8;

/*========P0=========*/

bit   AD7	= 0x87;
bit   AD6	= 0x86;
bit   AD5	= 0x85;
bit   AD4	= 0x84;
bit   AD3	= 0x83;
bit   AD2	= 0x82;
bit   AD1	= 0x81;
bit   AD0	= 0x80;

/*========P1=========*/

bit   SDA	= 0x97;
bit   SCL	= 0x96;
bit   RT2	= 0x95;
bit   T2	= 0x94;
bit   CT3I	= 0x93;
bit   CT2I	= 0x92;
bit   CT1I	= 0x91;
bit   CT0I	= 0x90;

/*========P3=========*/

bit   RD	= 0xB7;
bit   WR	= 0xB6;
bit   P_T1	= 0xB5;
bit   T0	= 0xB4;
bit   INT1	= 0xB3;
bit   INT0	= 0xB2;
bit   TXD	= 0xB1;
bit   RXD	= 0xB0;

/*========IP0=========*/

bit   PAD	= 0xBE;
bit   PS1	= 0xBD;
bit   PS0	= 0xBC;
bit   PT1	= 0xBB;
bit   PX1	= 0xBA;
bit   PT0	= 0xB9;
bit   PX0	= 0xB8;

/*========IP1=========*/

bit   PT2	= 0xFF;
bit   PCM2	= 0xFE;
bit   PCM1	= 0xFD;
bit   PCM0	= 0xFC;
bit   PCT3	= 0xFB;
bit   PCT2	= 0xFA;
bit   PCT1	= 0xF9;
bit   PCT0	= 0xF8;

/*========S0CON=========*/

bit   SM0	= 0x9F;
bit   SM1	= 0x9E;
bit   SM2	= 0x9D;
bit   REN	= 0x9C;
bit   TB8	= 0x9B;
bit   RB8	= 0x9A;
bit   S_T1	= 0x99;
bit   R1	= 0x98;

/*========S1CON=========*/

bit   CR2	= 0xDF;
bit   ENS1	= 0xDE;
bit   STA	= 0xDD;
bit   ST0	= 0xDC;
bit   SI	= 0xDB;
bit   AA	= 0xDA;
bit   CR1	= 0xD9;
bit   CR0	= 0xD8;

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

interrupt [0x03] void EX0_int (void);	  /* External Interrupt 0 */

interrupt [0x0B] void T0_int (void);      /* Timer 0 Overflow */

interrupt [0x13] void EX1_int (void);	  /* External Interrupt 1 */

interrupt [0x1B] void T1_int (void);      /* Timer 1 Overflow */

interrupt [0x23] void S0_int (void);     /* SIO0 (UART) Interrupt */

interrupt [0x2B] void S1_int (void);     /* SIO1 (I2C) Interrupt */

interrupt [0x33] void CT0_int (void);     /* T2 capture 0 Interrupt */

interrupt [0x3B] void CT1_int (void);     /* T2 capture 1 Interrupt */

interrupt [0x43] void CT2_int (void);     /* T2 capture 2 Interrupt */

interrupt [0x4B] void CT3_int (void);     /* T2 capture 3 Interrupt */

interrupt [0x53] void ADC_int (void);     /* ADC completion Interrupt */

interrupt [0x5B] void CM0_int (void);     /* T2 compare 0 Interrupt */

interrupt [0x63] void CM1_int (void);     /* T2 compare 1 Interrupt */

interrupt [0x6B] void CM2_int (void);     /* T2 compare 2 Interrupt */

interrupt [0x73] void T2_int (void);     /* T2 overflow Interrupt */
