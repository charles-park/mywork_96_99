/*[-]********************************************************************[-]*/
/*[-]   KO     AWS                                                       [-]*/
/*[-]   LED DISPLAY                                                      [-]*/
/*[-]   1996.2                                     DESIGNED BY LEE.      [-]*/
/*[-]********************************************************************[-]*/

#include <io51.h> 

/*	#define BPS96   0xFA     
	#define BPS48   0xF4   
	#define BPS24   0xE8
	#define BPS12   0xD0
	#define BPS6    0x80
	#define BPS3    0x40			*/

#define   REC_SIZE  146

static    char DATA[REC_SIZE];
static    char DAT[REC_SIZE];
static    char DSP = 0;     
static    char DEP = 0;     
static    char full =0;
static    char bull =0;
static    int  counter;
static    char SEN_CODE=0;
static    char TWO_ONE=0;
static    char BB=0;
static    unsigned char count[5];
static    unsigned int  PUN;
static    unsigned int  PUN1;
static    unsigned char buf1;
static    unsigned char buf2;

/***************************************************************************/
interrupt   void SCON_int         (void);
interrupt   void T0_int           (void);
            void initial          (void);
            void delay            (void);
            void recive_data_comp (void);
			void display_0        (void);
            void display_8        (void);
            void display_clear    (void);
            void bin2bcd          (void);
            void bin2bcd1         (void);
            void put_number       (unsigned int addr);
// test function
void ram_clear(void);

/***************************************************************************/
#pragma memory=code     /* rom */
/*                    -         0 1  2  3   4   5   6 7   8   9 */
/* char    string[]={ 0x40,0,0x7F,63,6,91,79,102,109,125,7,127,111 }; */

/*                  0 1  2  3   4   5   6 7   8   9 */
char    string[]={ 63,6,91,79,102,109,125,7,127,111 };
/***************************************************************************/
char LEE1[]={"1996   2 25 DESIGNED BY KYUNG WOO LEE --"};
char LED1[]={"JIN YANG INDUSTRIAL CO., LTD. TEL (0342) 41-7741"};
char LED2[]={"223-39 SANGDAEWONDONG JUNGWONGU SUNGNAM CITY KYUNGGIDO KOREA"};

/***************************************************************************/
#pragma memory=default  /* ram */

/***************************************************************************/
void initial(void)
{
    EA   = 0;
    P1   = 0xFF;
    PCON = 0x80;
    SCON = 0x50;
    TMOD = 0x21; /*  50mS TIMER */
    TH1  = 0xE8; /*  2400 bps   */
    TH0  = 0x4B;
    TL0  = 0xFE;
    TR0  = 1;
    TR1  = 1;
    ET0  = 1;
    ET1  = 0;
    ET2  = 1;
    EX0  = 0;
    EX1  = 0;
    PS   = 1;
    ES   = 1;
    DSP  = 0;
    DEP  = 0;
    EA   = 1;
}

/***************************************************************************/
interrupt void SCON_int(void)
{

    int          l;
    int    CHK = 1;

    unsigned char check_xor=0x00;
    unsigned char check_or =0x00;

    full       = 0;
	counter    = 0;

    DEP %= REC_SIZE;                        /* THIS IS Q ROUTINE */
    if(DSP == DEP)  {  DSP++;  DSP %= REC_SIZE; }

    DATA[DEP++] = SBUF;

    /*          1         2         3         4         5      ^+FE*/
    /*0123456789012345678901234567890123456789012345678901234567890*/
    /*FFIDMymdhmM0          M0         M0          M0          ^+  */
    /*          VV  VV  VV  VV VV  VV  VV  VV  VV  VV  VV  VV  VV  */
    /*  M       M0  __  __  M0 __  __  M0  __  __  M0  __  __      */
    if(CHK == 1 && DATA[(DSP    )%REC_SIZE] !=0xFF) CHK=0;
    if(CHK == 1 && DATA[(DSP+  4)%REC_SIZE] !='M' ) CHK=0;
    if(CHK == 1 && DATA[(DSP+144)%REC_SIZE] !=0xFF) CHK=0;

    if(CHK == 1) {

        check_xor = 0x00;
        check_or  = 0x00;
        bull = 1;
        for(l=2;l<REC_SIZE;l++) {
            DAT[l] = DATA[(DSP+l)%REC_SIZE];
            if(l <142)  {
                check_xor^=DAT[l];
                check_or +=DAT[l];
            }
        }

        if(DATA[(DSP+142)%REC_SIZE] != check_xor) bull = 0;
        if(DATA[(DSP+143)%REC_SIZE] != check_or ) bull = 0;
    }

	TI   = 0;
	RI   = 0;

}
/***************************************************************************/
void display_0(void)
{
	int    i;

	for(i=0;i<16;i++)   {   write_XDATA(0x8000+i,0x00);
							write_XDATA(0x9000+i,0x00);    }
	for(i=0;i<16;i=i+4) {   write_XDATA(0x8003+i,0x3f);
							write_XDATA(0x9003+i,0x3f);    }
}

/***************************************************************************/
void display_8(void)
{
	int    i;

	for(i=0;i<16;i++)   {   write_XDATA(0x8000+i,0x00);
							write_XDATA(0x9000+i,0x00);    }
	for(i=0;i<16;i++)   {   write_XDATA(0x8000+i,0x7f);
							write_XDATA(0x9000+i,0x7f);    }
}

/****************************************************************************/
void display_clear(void)
{
	int    i;
	for(i=0;i<16;i++)   {   write_XDATA(0x8000+i,0x00);
							write_XDATA(0x9000+i,0x00);    }
}

/***************************************************************************/
void delay(void)
{
	int i;  for (i = 0;i<30000;i++);
}

/***************************************************************************/
/* -------------------------  recvie data routine -----------------------  */
void recive_data_comp(void)
{
	int j;

for(j=10;j<REC_SIZE-10;j=j+12)
	{
   if(DAT[j] == 'M'){  TWO_ONE=j; SEN_CODE=DAT[j+2];

	  switch(SEN_CODE) {

	   case  0:{bin2bcd(); put_number(0x8003);  }  break; /*  W/D   */
	   case  1:{bin2bcd(); put_number(0x8007);  }  break; /*  W/S   */
	   case  2:{if(DAT[j+3]==2 ) {                        /*    C   */
				bin2bcd1();put_number(0x800B);
                if(BB==1)  write_XDATA(0x8008,0x40);BB=0;

                           TWO_ONE=TWO_ONE+4;
				bin2bcd1();put_number(0x800F);
				if(BB==1)  write_XDATA(0x800C,0x40);BB=0;}  }  break;
       case  3:{bin2bcd(); put_number(0x9003); TWO_ONE=TWO_ONE+4;
                bin2bcd(); put_number(0x9007);  }  break; /*   mm   */
       case  6:{if(DAT[j+3]==4) {
				bin2bcd();put_number(0x900B);  } }  break; /*   %    */

                       }
               }
    }
}

/***************************************************************************/
interrupt void T0_int(void)
{

    TH0  = 0x4B;
    TL0  = 0xFE;
    counter++;
    if(counter >=3600)  {       /* counter ==> 100 ==> 5sec  1000 ==> 50sec */
		full++; 
		counter=0;
    }

}

/***************************************************************************/
void put_number(unsigned int addr)
{
    int i;
    for(i=0;i<4;i++)  {write_XDATA(addr-i,0               );}

    if(count[1] != 0) {write_XDATA(addr-3,string[count[1]]);
                       write_XDATA(addr-2,string[count[2]]);}

                 else if(count[2] != 0)
					   write_XDATA(addr-2,string[count[2]]);

	write_XDATA(addr-1,string[count[3]]+0x80);		// ponit view
//	write_XDATA(addr-1, string[count[3]]);
    write_XDATA(addr  ,string[count[4]]);
}

/***************************************************************************/
void bin2bcd(void)
{

	buf1=DAT[TWO_ONE+4];   /* max  */
	buf2=DAT[TWO_ONE+5];   /* min  */
	PUN = buf1;
	PUN = PUN<< 8;
	PUN = PUN|buf2;
    PUN1 = PUN;

    count[0] = PUN1/10000;
    PUN1 %= 10000;
    count[1] = PUN1/1000;
    PUN1 %= 1000;
    count[2] = PUN1/100;
    PUN1 %= 100;
    count[3] = PUN1/10;
    count[4] = PUN1%10;
}

/***************************************************************************/
void bin2bcd1(void)
{

    buf1=DAT[TWO_ONE+4];   /* max  */
    buf2=DAT[TWO_ONE+5];   /* min  */
    PUN = buf1;
    PUN = PUN<< 8;
    PUN = PUN|buf2;
    PUN1 = PUN;

    if(PUN1<1000) {PUN1=1000-PUN1; BB=1;}
            else  {PUN1=PUN1-1000; BB=0;}

    count[0] = PUN1/10000;
    PUN1 %= 10000;
    count[1] = PUN1/1000;
    PUN1 %= 1000;
    count[2] = PUN1/100;
    PUN1 %= 100;
    count[3] = PUN1/10;
    count[4] = PUN1%10;
}



// test function
void ram_clear(void)
{
	unsigned int	addr;
	for(addr = 0x0E; addr < 0x100; addr++)		write_XDATA(addr, 0x00);
}

/***************************************************************************/
void main(void)
{

	initial();
	display_8();       delay();
	display_0();

	ram_clear();
	BB = 0;
	while(1)
	{
		display_8();	delay();	delay();
		display_0();    delay();	delay();

		count[0] = 0;
		count[1] = 0;
		count[2] = 0;
		count[3] = 0;
		count[4] = 0;
		put_number(0x8003);	// wd
		put_number(0x8007);	// ws

		count[0] = 0;
		count[1] = 2;
		count[2] = 3;
		count[3] = 4;
		count[4] = 5;
		put_number(0x800B);		// temp
		write_XDATA(0x8008,0x40);		// '-'

		count[0] = 0;
		count[1] = 3;
		count[2] = 4;
		count[3] = 5;
		count[4] = 6;
		put_number(0x800F);		// ???	maximum temp(temp min value used)
		write_XDATA(0x800C,0x40);		// '-'


		count[0] = 0;
		count[1] = 4;
		count[2] = 5;
		count[3] = 6;
		count[4] = 7;			// pre
		put_number(0x9003);

		count[0] = 0;
		count[1] = 5;
		count[2] = 6;
		count[3] = 7;
		count[4] = 8;			// ypre
		put_number(0x9007);

		count[0] = 0;
		count[1] = 6;
		count[2] = 7;
		count[3] = 8;
		count[4] = 9;			// humi
		put_number(0x900B);

		delay();	delay();
	}

//	// test
//	while(1)	{
//	}
	while(1)            {
		if(full>=1)     { display_0();        full=0;}
		if(bull==1)     { recive_data_comp(); bull=0;}
						}
}

