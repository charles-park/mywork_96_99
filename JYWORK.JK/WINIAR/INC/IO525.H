/*			- IO525.H -

   Special header for the 80C525 processor.
	   
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
sfr   P4	= 0xE8;
sfr   P5	= 0xF8;
sfr   P6	= 0xD8;
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
sfr   WDS	= 0xA9;
sfr   WDK	= 0xAA;
sfr   IP	= 0xB8;
sfr   SCON	= 0x98;
sfr   SBUF	= 0x99;
sfr   ARR	= 0xAB;
sfr   OFE	= 0xAC;
sfr   AIP     	= 0xAD;
sfr   AIE     	= 0xAE;
sfr   AIF     	= 0xAF;
sfr   ID0     	= 0xC0;
sfr   ID1     	= 0xC1;
sfr   ID2     	= 0xC2;
sfr   ID3     	= 0xC3;
sfr   ID4     	= 0xC4;
sfr   ID5     	= 0xC5;
sfr   ID6     	= 0xC6;
sfr   ID7     	= 0xC7;
sfr   ID8     	= 0xC8;
sfr   ID9     	= 0xC9;
sfr   ID10    	= 0xCA;
sfr   ID11    	= 0xCB;
sfr   ID12    	= 0xCC;
sfr   ID13    	= 0xCD;
sfr   ID14    	= 0xCE;
sfr   ID15    	= 0xCF;
sfr   GPIC    	= 0xF9;
sfr   IFS     	= 0xFA;
sfr   FES     	= 0xFB;
sfr   IT      	= 0xFC;
sfr   ET      	= 0xFD;
sfr   IFBUF   	= 0xFE;
sfr   EFBUF   	= 0xFF;

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
bit   ET2	= 0xAD;
bit   ES	= 0xAC;
bit   ET1	= 0xAB;
bit   EX1	= 0xAA;
bit   ET0	= 0xA9;
bit   EX0	= 0xA8;

/*========IP=========*/

bit   PT2	= 0xBD;
bit   PS	= 0xBC;
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

/*========SCON=======*/

bit   SM0	= 0x9F;
bit   SM1	= 0x9E;
bit   SM2	= 0x9D;
bit   REN	= 0x9C;
bit   TB8	= 0x9B;
bit   RB8	= 0x9A;
bit   TI	= 0x99;
bit   RI	= 0x98;

/*========ID1========*/

bit   ID1_7	= 0xCF;
bit   ID1_6	= 0xCE;
bit   ID1_5	= 0xCD;
bit   ID1_4	= 0xCC;
bit   ID1_3	= 0xCB;
bit   ID1_2	= 0xCA;
bit   ID1_1	= 0xC9;
bit   ID1_0	= 0xC8;

/*========ID0========*/

bit   ID0_7	= 0xC7;
bit   ID0_6	= 0xC6;
bit   ID0_5	= 0xC5;
bit   ID0_4	= 0xC4;
bit   ID0_3	= 0xC3;
bit   ID0_2	= 0xC2;
bit   ID0_1	= 0xC1;
bit   ID0_0	= 0xC0;

/*================================*/
/* Pre-defined external registers */
/*================================*/

#define   DPR0    0x00
#define   DPR1    0x01
#define   DPR2    0x02
#define   DPR3    0x03
#define   DPR4    0x04
#define   DPR5    0x05
#define   DPR6    0x06
#define   DPR7    0x07
#define   DPR8    0x08
#define   DPR9    0x09
#define   DPR10   0x0A
#define   DPR11   0x0B
#define   DPR12   0x0C
#define   DPR13   0x0D
#define   DPR14   0x0E
#define   DPR15   0x0F
#define   IBUF    0x10
#define   EBUF    0x11
#define   EFS     0x12
#define   EIE     0x13
#define   RS      0x14

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

