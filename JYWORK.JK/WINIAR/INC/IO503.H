/*                      - IO503.H -

   Special header for the Siemens SAB 80C503 processor.

   Version 5.00 [IAMR]

*/

#pragma language=extended

/*===============================*/
/* Predefined SFR Byte Addresses */
/*===============================*/

sfr   P0        = 0x80;
sfr   P1        = 0x90;
sfr   P2        = 0xA0;
sfr   P3        = 0xB0;
sfr   PSW       = 0xD0;
sfr   ACC       = 0xE0;
sfr   B         = 0xF0;
sfr   SP        = 0x81;
sfr   DPL       = 0x82;
sfr   DPH       = 0x83;
sfr   PCON      = 0x87;
sfr   TCON      = 0x88;
sfr   TMOD      = 0x89;
sfr   TL0       = 0x8A;
sfr   TL1       = 0x8B;
sfr   TH0       = 0x8C;
sfr   TH1       = 0x8D;
sfr   IE        = 0xA8;
sfr   IP        = 0xB8;
sfr   SCON      = 0x98;
sfr   SBUF      = 0x99;
sfr   T2CON     = 0xC8;
sfr   T2MOD     = 0xC9;
sfr   RC2L      = 0xCA;
sfr   RC2H      = 0xCB;
sfr   TL2       = 0xCC;
sfr   TH2       = 0xCD;
sfr   ADCON0    = 0xD8;
sfr   ADDATH    = 0xD9;
sfr   ADDATL    = 0xDA;
sfr   ADCON1    = 0xDC;
sfr   WDCON     = 0xC0;
sfr   WDTREL    = 0x86;

/*==============================*/
/* Predefined SFR Bit Addresses */
/*==============================*/

/*========PSW========*/

bit   CY        = 0xD7;
bit   AC        = 0xD6;
bit   F0        = 0xD5;
bit   RS1       = 0xD4;
bit   RS0       = 0xD3;
bit   OV        = 0xD2;
bit   F1        = 0xD1;
bit   P         = 0xD0;

/*========TCON=======*/

bit   TF1       = 0x8F;
bit   TR1       = 0x8E;
bit   TF0       = 0x8D;
bit   TR0       = 0x8C;
bit   IE1       = 0x8B;
bit   IT1       = 0x8A;
bit   IE0       = 0x89;
bit   IT0       = 0x88;

/*========IE=========*/

bit   EA        = 0xAF;
bit   EADC      = 0xAE;
bit   ET2       = 0xAD;
bit   ES        = 0xAC;
bit   ET1       = 0xAB;
bit   EX1       = 0xAA;
bit   ET0       = 0xA9;
bit   EX0       = 0xA8;

/*========IP=========*/

bit   PADC      = 0xBE;
bit   PT2       = 0xBD;
bit   PS        = 0xBC;
bit   PT1       = 0xBB;
bit   PX1       = 0xBA;
bit   PT0       = 0xB9;
bit   PX0       = 0xB8;

/*========P3=========*/

bit   RD        = 0xB7;
bit   WR        = 0xB6;
bit   T1        = 0xB5;
bit   T0        = 0xB4;
bit   INT1      = 0xB3;
bit   INT0      = 0xB2;
bit   TXD       = 0xB1;
bit   RXD       = 0xB0;

/*========SCON========*/

bit   SM0       = 0x9F;
bit   SM1       = 0x9E;
bit   SM2       = 0x9D;
bit   REN       = 0x9C;
bit   TB8       = 0x9B;
bit   RB8       = 0x9A;
bit   TI        = 0x99;
bit   RI        = 0x98;

/*========T2CON=======*/

bit   TF2       = 0xCF;
bit   EXF2      = 0xCE;
bit   RCLK      = 0xCD;
bit   TCLK      = 0xCC;
bit   EXEN2     = 0xCB;
bit   TR2       = 0xCA;
bit   C_T2      = 0xC9;
bit   CP_RL2    = 0xC8;

/*========ADCON0======*/

bit   BD        = 0xDF;
bit   CLK       = 0xDE;
bit   IADC      = 0xDD;
bit   BSY       = 0xDC;
bit   ADM       = 0xDB;
bit   MX2       = 0xDA;
bit   MX1       = 0xD9;
bit   MX0       = 0xD8;

/*========WDCON=======*/

bit   OWDS      = 0xC3;
bit   WDTS      = 0xC2;
bit   WDT       = 0xC1;
bit   SWDT      = 0xC0;

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

interrupt [0x03] void EX0_int (void);     /* External Interrupt 0 */

interrupt [0x0B] void T0_int (void);      /* Timer 0 Overflow */

interrupt [0x13] void EX1_int (void);     /* External Interrupt 1 */

interrupt [0x1B] void T1_int (void);      /* Timer 1 Overflow */

interrupt [0x23] void SCON_int (void);    /* Serial Port */

interrupt [0x2B] void T2_int (void);      /* Timer 2 Overflow */

interrupt [0x2B] void T2EX_int (void);    /* Negative Transition on T2EX */

interrupt [0x43] void ADC_int (void);     /* A/D Converter Interrupt */

