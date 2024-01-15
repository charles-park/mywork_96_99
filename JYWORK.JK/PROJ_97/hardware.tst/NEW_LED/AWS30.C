#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>
#include "serial.h"

#define	TIME	8

#define	START	0
#define  END   1

#define	MAX	1024

#define B300L   0x80
#define B300H   0x01
#define B600    0xC0
#define B1200   0x60
#define B2400   0x30
#define B4800   0x18
#define B9600   0x0c
#define B19200  0x06
#define B38400  0x04

char COM_BUFFER[MAX];
WORD front=0,rear=0,r_count=0;			/* CIRCLE Q point */
WORD COM_PORT;
void interrupt (*old_vect)();
void interrupt intsr(void)
{
	disable();
	rear %= MAX;
	COM_BUFFER[rear++] = inportb(RBR);
	outportb(PICEOI,0x20);
	enable();
}

BYTE cominDATA(void)
{
	front %= MAX;
	return COM_BUFFER[front++];
}

int cominREADY(void)
{
       return ((front == rear) ? 0:1);
}

BYTE cominCH(void)
{
	while(!cominREADY());
	return cominDATA();
}

void comoutCH(char ch)
{
	while((inportb(LSR) & 0x20) == 0x00)
	{
	}
	outportb(THR,ch);
}

void Setting(int MODE)
{
	switch(COM_PORT)
	{
		case 1:	BASE = C1BASE;	break;
		case 2:	BASE = C2BASE;	break;
		case 3:	BASE = C3BASE;	break;
		case 4:	BASE = C4BASE;	break;
	}
	if (MODE == START)
	{
		outportb(LCR,0x83);
		outportb(DLL,B2400);
		outportb(DLM,0);
		outportb(LCR,inportb(LCR) & 0x7f);

		switch(COM_PORT)
		{
			case 1:	case 3:
				old_vect = getvect(IRQ4);	setvect(IRQ4,intsr);
				outportb(PICMSK,inportb(PICMSK) & I4ENA);
				break;
			case 2:	case 4:
				old_vect = getvect(IRQ3);	setvect(IRQ3,intsr);
				outportb(PICMSK,inportb(PICMSK) & I3ENA);
				break;
		}
		outportb(IER,0x01);
		outportb(MCR,0x0b);
	}
	else if (MODE == END)
	{
		outportb(MCR,0x0B);
		outportb(IER,0x00);
		switch(COM_PORT)
		{
			case 1:	case 3:
				outportb(PICMSK,inportb(PICMSK) | I4DIS);
				setvect(IRQ4,old_vect);
				break;
			case 2:	case 4:
				outportb(PICMSK,inportb(PICMSK) | I3DIS);
				setvect(IRQ3,old_vect);
				break;
		}
	}
}

void main(int argc,char **argv)
{
	int ch,i;
	unsigned char outd[102],subv=0;

	if (argc != 2)
	{
		puts("Using COMM com_port[1..4]");
		exit(0);
	}
	else	COM_PORT = argv[1][0] - '0';
	if (COM_PORT < 1 || COM_PORT > 4)
	{
		puts("Using COMM com_port[1..4]");
		exit(0);
	}
	clrscr();
	Setting(START);
	while(1)
	{
		if (kbhit())
		{
			ch = getch();
			if (ch == 0x1b)	break;
			if (ch == 0x00)
				getch();
			if(ch == 0x0d)
			{
				outd[0]=0xff;
				outd[1]=0xff;
				outd[2] ='P';       /* Data : Kubun ,  Program : "P" */
				outd[3] ='M';		/* Data : Status, Program : Kubun */
				outd[4] =3;			/* Sensor code  , Sensor ID     pred 4, w/d 0*/
				outd[5] =0;			/* Sensor ID    , Jumper ID    , */

				outd[6] =0x04;		/* Data 0 High  , Climit */
				outd[7] =0x03;      /* Data 0 Low   , Dot Point */
				outd[8] =0x02;		/* Data 1 High  , Display Kind */
				outd[9] =0xf7;		/* 0xfa;       Data 1 Low   , Perh */
				outd[10]=0xff;		/* Data 2 High  , Perl */
				outd[11]=0x03;		/* Data 2 Low   , Baudrate */
				outd[12]=0;			/* Data 3 High  , None */
				outd[13]=0;      	/* Data 3 Low   , None */
				outd[14]=0;
				outd[15]=0xff;
				outd[16]=0xfe;
		/***************************/
				outd[17]=0xff;
				outd[18]=0xff;
				outd[19] ='P';       /* Data : Kubun ,  Program : "P" */
				outd[20] ='M';		/* Data : Status, Program : Kubun */
				outd[21] =0x2;		/* Sensor code  , Sensor ID     pred 4, w/d 0*/
				outd[22] =0x1;		/* Sensor ID    , Jumper ID    , */

				outd[23]=0x04;		/* Data 0 High  , Climit */
				outd[24]=0x03;      /* Data 0 Low   , Dot Point */
				outd[25]=0x00;		/* Data 1 High  , Display Kind */
				outd[26]=0xf7;		/* 0xfa;       Data 1 Low   , Perh */
				outd[27]=0xff;		/* Data 2 High  , Perl */
				outd[28]=0x03;		/* Data 2 Low   , Baudrate */
				outd[29]=0;			/* Data 3 High  , None */
				outd[30]=0;      	/* Data 3 Low   , None */
				outd[31]=0;
				outd[32]=0xff;
				outd[33]=0xfe;
		/*************************/
				outd[34]=0xff;
				outd[35]=0xff;
				outd[36] ='P';       /* Data : Kubun ,  Program : "P" */
				outd[37] ='M';		/* Data : Status, Program : Kubun */
				outd[38] =0x1;		/* Sensor code  , Sensor ID     pred 4, w/d 0*/
				outd[39] =0x2;		/* Sensor ID    , Jumper ID    , */

				outd[40]=0x04;		/* Data 0 High  , Climit */
				outd[41]=0x03;      /* Data 0 Low   , Dot Point */
				outd[42]=0x00;		/* Data 1 High  , Display Kind */
				outd[43]=0xf7;		/* 0xfa;       Data 1 Low   , Perh */
				outd[44]=0xff;		/* Data 2 High  , Perl */
				outd[45]=0x03;		/* Data 2 Low   , Baudrate */
				outd[46]=0;			/* Data 3 High  , None */
				outd[47]=0;      	/* Data 3 Low   , None */
				outd[48]=0;
				outd[49]=0xff;
				outd[50]=0xfe;
				/****************/
				outd[51]=0xff;
				outd[52]=0xff;
				outd[53]='P';       /* Data : Kubun ,  Program : "P" */
				outd[54]='M';		/* Data : Status, Program : Kubun */
				outd[55]=0x0;		/* Sensor code  , Sensor ID     pred 4, w/d 0*/
				outd[56]=0x3;		/* Sensor ID    , Jumper ID    , */

				outd[57]=0x0f;		/* Data 0 High  , Climit */
				outd[58]=0x80;      /* Data 0 Low   , Dot Point */
				outd[59]=0x00;		/* Data 1 High  , Display Kind */
				outd[60]=0xfb;		/* 0xfa;       Data 1 Low   , Perh */
				outd[61]=0xff;		/* Data 2 High  , Perl */
				outd[62]=0x03;		/* Data 2 Low   , Baudrate */
				outd[63]=0x00;		/* Data 3 High  , None */
				outd[64]=0x00;      /* Data 3 Low   , None */
				outd[65]=0;
				outd[66]=0xff;
				outd[67]=0xfe;
		/********************************/
				outd[68]=0xff;
				outd[69]=0xff;
				outd[70]='P';       /* Data : Kubun ,  Program : "P" */
				outd[71]='M';		/* Data : Status, Program : Kubun */
				outd[72]=0x4;		/* Sensor code  , Sensor ID     pred 4, w/d 0*/
				outd[73]=0x4;		/* Sensor ID    , Jumper ID    , */

				outd[74]=0x0f;		/* Data 0 High  , Climit */
				outd[75]=0x80;      /* Data 0 Low   , Dot Point */
				outd[76]=0x00;		/* Data 1 High  , Display Kind */
				outd[77]=0xfb;		/* 0xfa;       Data 1 Low   , Perh */
				outd[78]=0xff;		/* Data 2 High  , Perl */
				outd[79]=0x03;		/* Data 2 Low   , Baudrate */
				outd[80]=0x00;		/* Data 3 High  , None */
				outd[81]=0x00;      /* Data 3 Low   , None */
				outd[82]=0;
				outd[83]=0xff;
				outd[84]=0xfe;
			/***************************/
				outd[85]=0xff;
				outd[86]=0xff;
				outd[87]='P';       /* Data : Kubun ,  Program : "P" */
				outd[88]='M';		/* Data : Status, Program : Kubun */
				outd[89]=0x3;		/* Sensor code  , Sensor ID     pred 4, w/d 0*/
				outd[90]=0x5;		/* Sensor ID    , Jumper ID    , */

				outd[91]=0x04;		/* Data 0 High  , Climit */
				outd[92]=0x03;      /* Data 0 Low   , Dot Point */
				outd[93]=0x03;		/* Data 1 High  , Display Kind */
				outd[94]=0xf7;		/* 0xfa;       Data 1 Low   , Perh */
				outd[95]=0xff;		/* Data 2 High  , Perl */
				outd[96]=0x03;		/* Data 2 Low   , Baudrate */
				outd[97]=0x00;		/* Data 3 High  , None */
				outd[98]=0x00;      /* Data 3 Low   , None */
				outd[99]=0;
				outd[100]=0xff;
				outd[101]=0xfe;

				for(i=2; i != 14;i++)
					outd[14]^=outd[i];
				for(i=19; i != 31;i++)
					outd[31]^=outd[i];
				for(i=36; i != 48;i++)
					outd[48]^=outd[i];
				for(i=53; i != 65;i++)
					outd[65]^=outd[i];
				for(i=70; i != 82;i++)
					outd[82]^=outd[i];
				for(i=87; i != 99;i++)
					outd[99]^=outd[i];

				for(i=0; i != 102;i++)
				{
					comoutCH(outd[i]);
					printf(" %02X",outd[i]);
					if(outd[i] == 0xfe)
					{
						printf(" Led Setting ,%02X\n",i);
						delay(500);
					}
				}
				puts("");
			}
			if(ch == 0x20)
			{
                if(subv==0)
                {
                    subv=1;
                    outd[0]=0xff;
                    outd[1]=0xff;
                    outd[2] ='M';       /* Data : Kubun ,  Program : "P" */
                    outd[3] =0x0;       /* Data : Status, Program : Kubun */
                    outd[4] =0;         /* Sensor code  , Sensor ID     pred 4, w/d 0*/
                    outd[5] =0;         /* Sensor ID    , Jumper ID    , */
                    i=2130;
                    outd[6] =i>>8;      /* Data 0 High  , Climit */
                    outd[7] =i;      /* Data 0 Low   , Dot Point */
                    outd[8] =0x00;      /* Data 1 High  , Display Kind */
                    outd[9] =0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    outd[10]=0x00;      /* Data 2 High  , Perl */
                    outd[11]=0x00;      /* Data 2 Low   , Baudrate */
                    outd[12]=0;         /* Data 3 High  , None */
                    outd[13]=0;         /* Data 3 Low   , None */
                    outd[14]=0;
                    outd[15]=0xff;
                    outd[16]=0xfe;
            /***************************/
                    outd[17]=0xff;
                    outd[18]=0xff;
                    outd[19] ='M';       /* Data : Kubun ,  Program : "P" */
                    outd[20] =0x0;      /* Data : Status, Program : Kubun */
                    outd[21] =0x1;      /* Sensor code  , Sensor ID     pred 4, w/d 0*/
                    outd[22] =0x1;      /* Sensor ID    , Jumper ID    , */

					i=252;
                    outd[23]=i>>8;      /* Data 0 High  , Climit */
                    outd[24]=i;         /* Data 0 Low   , Dot Point */
                    outd[25]=0x00;      /* Data 1 High  , Display Kind */
                    outd[26]=0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    outd[27]=0x00;      /* Data 2 High  , Perl */
                    outd[28]=0x00;      /* Data 2 Low   , Baudrate */
                    outd[29]=0;         /* Data 3 High  , None */
                    outd[30]=0;         /* Data 3 Low   , None */
                    outd[31]=0;
                    outd[32]=0xff;
                    outd[33]=0xfe;
            /*************************/
                    outd[34]=0xff;
                    outd[35]=0xff;
                    outd[36] ='M';       /* Data : Kubun ,  Program : "P" */
                    outd[37] =0x0;      /* Data : Status, Program : Kubun */
                    outd[38] =0x2;      /* Sensor code  , Sensor ID     pred 4, w/d 0*/
                    outd[39] =0x2;      /* Sensor ID    , Jumper ID    , */
                    i=1525;
                    outd[40]=i>>8;      /* Data 0 High  , Climit */
                    outd[41]=i;      /* Data 0 Low   , Dot Point */
                    outd[42]=0x00;      /* Data 1 High  , Display Kind */
                    outd[43]=0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    outd[44]=0x00;      /* Data 2 High  , Perl */
                    outd[45]=0x00;      /* Data 2 Low   , Baudrate */
                    outd[46]=0;         /* Data 3 High  , None */
                    outd[47]=0;         /* Data 3 Low   , None */
                    outd[48]=0;
                    outd[49]=0xff;
                    outd[50]=0xfe;
                    /****************/
                    outd[51]=0xff;
                    outd[52]=0xff;
                    outd[53]='M';       /* Data : Kubun ,  Program : "P" */
                    outd[54]=0x00;      /* Data : Status, Program : Kubun */
                    outd[55]=0x3;       /* Sensor code  , Sensor ID     pred 4, w/d 0*/
                    outd[56]=0x3;       /* Sensor ID    , Jumper ID    , */
                    outd[57]=0;     /* Data 0 High  , Climit */
                    outd[58]=0;         /* Data 0 Low   , Dot Point */
                    outd[59]=0x00;      /* Data 1 High  , Display Kind */
                    outd[60]=0x00;      /* 0xfa;       Data 1 Low   , Perh */
					i=5252;
					outd[61]=i>>8;      /* Data 2 High  , Perl */
                    outd[62]=i;         /* Data 2 Low   , Baudrate */
					i=2525;
                    outd[63]=i>>8;      /* Data 3 High  , None */
                    outd[64]=i;         /* Data 3 Low   , None */
                    outd[65]=0;
                    outd[66]=0xff;
                    outd[67]=0xfe;
            /********************************/
                    outd[68]=0xff;
                    outd[69]=0xff;
                    outd[70]='M';       /* Data : Kubun ,  Program : "P" */
                    outd[71]=0x0;       /* Data : Status, Program : Kubun */
					outd[72]=0x4;       /* Sensor code  , Sensor ID     pred 4, w/d 0*/
					outd[73]=0x4;       /* Sensor ID    , Jumper ID    , */

                    outd[74]=0x00;      /* Data 0 High  , Climit */
                    outd[75]=0x00;      /* Data 0 Low   , Dot Point */
                    outd[76]=0x00;      /* Data 1 High  , Display Kind */
                    outd[77]=0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    outd[78]=0x00;      /* Data 2 High  , Perl */
                    outd[79]=0x00;      /* Data 2 Low   , Baudrate */
                    outd[80]=0x00;      /* Data 3 High  , None */
                    outd[81]=0x00;      /* Data 3 Low   , None */
                    outd[82]=0;
                    outd[83]=0xff;
                    outd[84]=0xfe;
                /***************************/

                    for(i=2; i != 14;i++)
                        outd[14]^=outd[i];
                    for(i=19; i != 31;i++)
                        outd[31]^=outd[i];
                    for(i=36; i != 48;i++)
                        outd[48]^=outd[i];
                    for(i=53; i != 65;i++)
                        outd[65]^=outd[i];
                    for(i=70; i != 82;i++)
                        outd[82]^=outd[i];

                    for(i=0; i != 85;i++)
                    {
                        comoutCH(outd[i]);
                        printf(" %02X",outd[i]);
                        if(outd[i] == 0xfe)
                        {
                            printf(" Led Sample Data#1 Out ,%02X\n",i);
                        }
                    }
                    puts("");
                }
                else
                {
                    subv=0;
                    outd[1]=0xff;
                    outd[2] ='M';       /* Data : Kubun ,  Program : "P" */
                    outd[3] =0x0;       /* Data : Status, Program : Kubun */
                    outd[4] =0;         /* Sensor code  , Sensor ID     pred 4, w/d 0*/
                    outd[5] =0;         /* Sensor ID    , Jumper ID    , */
                    i=3030;
                    outd[6] =i>>8;      /* Data 0 High  , Climit */
                    outd[7] =i;      /* Data 0 Low   , Dot Point */
                    outd[8] =0x00;      /* Data 1 High  , Display Kind */
                    outd[9] =0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    outd[10]=0x00;      /* Data 2 High  , Perl */
                    outd[11]=0x00;      /* Data 2 Low   , Baudrate */
                    outd[12]=0;         /* Data 3 High  , None */
                    outd[13]=0;         /* Data 3 Low   , None */
                    outd[14]=0;
                    outd[15]=0xff;
                    outd[16]=0xfe;
            /***************************/
                    outd[17]=0xff;
                    outd[18]=0xff;
                    outd[19] ='M';       /* Data : Kubun ,  Program : "P" */
                    outd[20] =0x0;      /* Data : Status, Program : Kubun */
                    outd[21] =0x1;      /* Sensor code  , Sensor ID     pred 4, w/d 0*/
                    outd[22] =0x1;      /* Sensor ID    , Jumper ID    , */

					i=525;
                    outd[23]=i>>8;      /* Data 0 High  , Climit */
                    outd[24]=i;         /* Data 0 Low   , Dot Point */
                    outd[25]=0x00;      /* Data 1 High  , Display Kind */
                    outd[26]=0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    outd[27]=0x00;      /* Data 2 High  , Perl */
                    outd[28]=0x00;      /* Data 2 Low   , Baudrate */
                    outd[29]=0;         /* Data 3 High  , None */
                    outd[30]=0;         /* Data 3 Low   , None */
                    outd[31]=0;
                    outd[32]=0xff;
                    outd[33]=0xfe;
            /*************************/
                    outd[34]=0xff;
                    outd[35]=0xff;
                    outd[36] ='M';       /* Data : Kubun ,  Program : "P" */
                    outd[37] =0x0;      /* Data : Status, Program : Kubun */
                    outd[38] =0x2;      /* Sensor code  , Sensor ID     pred 4, w/d 0*/
                    outd[39] =0x2;      /* Sensor ID    , Jumper ID    , */
                    i=748;
                    outd[40]=i>>8;      /* Data 0 High  , Climit */
                    outd[41]=i;      /* Data 0 Low   , Dot Point */
                    outd[42]=0x00;      /* Data 1 High  , Display Kind */
                    outd[43]=0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    outd[44]=0x00;      /* Data 2 High  , Perl */
                    outd[45]=0x00;      /* Data 2 Low   , Baudrate */
                    outd[46]=0;         /* Data 3 High  , None */
                    outd[47]=0;         /* Data 3 Low   , None */
                    outd[48]=0;
                    outd[49]=0xff;
                    outd[50]=0xfe;
                    /****************/
                    outd[51]=0xff;
                    outd[52]=0xff;
                    outd[53]='M';       /* Data : Kubun ,  Program : "P" */
                    outd[54]=0x00;      /* Data : Status, Program : Kubun */
                    outd[55]=0x3;       /* Sensor code  , Sensor ID     pred 4, w/d 0*/
                    outd[56]=0x3;       /* Sensor ID    , Jumper ID    , */
                    outd[57]=0;     /* Data 0 High  , Climit */
                    outd[58]=0;         /* Data 0 Low   , Dot Point */
                    outd[59]=0x00;      /* Data 1 High  , Display Kind */
					outd[60]=0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    i=2525;
                    outd[61]=i>>8;      /* Data 2 High  , Perl */
					outd[62]=i;         /* Data 2 Low   , Baudrate */
					i=5252;
                    outd[63]=i>>8;      /* Data 3 High  , None */
                    outd[64]=i;         /* Data 3 Low   , None */
                    outd[65]=0;
                    outd[66]=0xff;
                    outd[67]=0xfe;
            /********************************/
                    outd[68]=0xff;
                    outd[69]=0xff;
                    outd[70]='M';       /* Data : Kubun ,  Program : "P" */
                    outd[71]=0x0;       /* Data : Status, Program : Kubun */
					outd[72]=0x4;       /* Sensor code  , Sensor ID     pred 4, w/d 0*/
					outd[73]=0x4;       /* Sensor ID    , Jumper ID    , */

                    outd[74]=0x00;      /* Data 0 High  , Climit */
                    outd[75]=0x01;      /* Data 0 Low   , Dot Point */
                    outd[76]=0x00;      /* Data 1 High  , Display Kind */
                    outd[77]=0x00;      /* 0xfa;       Data 1 Low   , Perh */
                    outd[78]=0x00;      /* Data 2 High  , Perl */
                    outd[79]=0x00;      /* Data 2 Low   , Baudrate */
                    outd[80]=0x00;      /* Data 3 High  , None */
                    outd[81]=0x00;      /* Data 3 Low   , None */
                    outd[82]=0;
                    outd[83]=0xff;
                    outd[84]=0xfe;
                /***************************/

                    for(i=2; i != 14;i++)
                        outd[14]^=outd[i];
                    for(i=19; i != 31;i++)
                        outd[31]^=outd[i];
                    for(i=36; i != 48;i++)
                        outd[48]^=outd[i];
                    for(i=53; i != 65;i++)
                        outd[65]^=outd[i];
                    for(i=70; i != 82;i++)
                        outd[82]^=outd[i];

                    for(i=0; i != 85;i++)
                    {
                        comoutCH(outd[i]);
                        printf(" %02X",outd[i]);
                        if(outd[i] == 0xfe)
                        {
                            printf(" Led Sample Data#2 out ,%02X\n",i);
                        }
                    }
                    puts("");
                }
            }
		}
		if (cominREADY())
		{
			ch=cominCH();
/*			printf("%02X ",ch);
			if(ch == 0xfe)
				puts("");
*/		}
	}
	Setting(END);
}
