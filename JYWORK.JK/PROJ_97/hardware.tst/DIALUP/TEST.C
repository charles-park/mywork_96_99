//[*]----------------------------------------------------------------------[*]
#include 	"test.h"
//[*]----------------------------------------------------------------------[*]
//[*]------ Start Program -------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
signed int Point(signed int Hear, signed int Step)    // Q Buffer Point Set
{
	Hear += Step;
	if(Hear < 0)  	Hear += SIZE;
	return	Hear;
}
//[*]----------------------------------------------------------------------[*]
void interrupt SERIAL(void)
{
	disable();
	Com.RxBuf[Com.RxEp++] = inportb(RBR);
	if(Com.RxEp >= SIZE)	Com.RxEp = 0;
	outportb(PICEOI,0x20);
	enable();
}
//[*]----------------------------------------------------------------------[*]
Byte CominData(void)
{
	Byte	data;
	data = Com.RxBuf[Com.RxSp];		Com.RxSp = Point(Com.RxSp, +1);
	printf("%c",data);
	return 	data;
	// return Com.RxBuf[Point(Com.RxSp, +1)];	origial source code
}
//[*]----------------------------------------------------------------------[*]
int CominREADY(void)
{
	return	((Com.RxSp == Com.RxEp) ? 0:1);
}
//[*]----------------------------------------------------------------------[*]
Byte CominCH(void)
{
	while(!CominREADY());
	return	CominData();
}
//[*]----------------------------------------------------------------------[*]
void ComoutCH(Byte ch)
{
	while((inportb(LSR) & 0x20) == 0x00)
	{
	}
	outportb(THR,ch);
	printf("OUTPUT SERIAL DATA -> BINARY:%02X ASCII:%c\n",ch, ch);
}
//[*]----------------------------------------------------------------------[*]
void ComoutStr(void)
{
	if(LineCount++ > 24)	{	clrscr();	LineCount = 0;	}
	while(Com.TxSp != Com.TxEp)		ComoutCH(Com.TxBuf[Com.TxSp++]);
	printf("\n");
}
//[*]----------------------------------------------------------------------[*]
void init_Serial(Word MODE)
{
	switch(Com.Port)
	{
		case	COM1:	Com.Base = C1BASE;	break;
		case	COM2:	Com.Base = C2BASE;	break;
		case	COM3:   Com.Base = C3BASE;	break;
		case	COM4:   Com.Base = C4BASE;	break;
	}
	if(MODE == START)	{
		outportb(LCR,inportb(LCR) | 0x80);
		outportb(DLL,B9600);		// Baudrate Setting
		outportb(DLM,0);
		outportb(LCR,inportb(LCR) & 0x7f);

		switch(Com.Port)
		{
			case COM1:	case COM3:
				old_vect = getvect(IRQ4);	setvect(IRQ4,SERIAL);
				outportb( PICMSK, inportb(PICMSK) & I4ENA );
				break;
			case COM2:	case COM4:
				old_vect = getvect(IRQ3);	setvect(IRQ3,SERIAL);
				outportb( PICMSK, inportb(PICMSK) & I3ENA );
				break;
		}
		outportb(IER,0x01);
		outportb(MCR,0x0b);
	}	else	{
		outportb(MCR,0x0B);
		outportb(IER,0x00);
		switch(Com.Port)
		{
			case COM1:	case COM3:
				outportb(PICMSK,inportb(PICMSK) | I4DIS);
				setvect(IRQ4,SERIAL);
				break;
			case COM2:	case COM4:
				outportb(PICMSK,inportb(PICMSK) | I3DIS);
				setvect(IRQ3,SERIAL);
				break;
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void ok_send(void)
{
	Com.TxBuf[Com.TxEp++] = 'R';
	Com.TxBuf[Com.TxEp++] = 'S';
	Com.TxBuf[Com.TxEp++] = 'O';
	Com.TxBuf[Com.TxEp++] = 'K';
	printf("RESET RECEIVE CMD OK! RESET OK DATA SEND TO PC\n");
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void main(int argc,char **argv)
{
	int ch, start;
	unsigned char	flag = 0;
	if (argc != 2)
	{
		puts("Using COMM com_port[1..4]");
		exit(0);
	}
	else	Com.Port = argv[1][0] - '0';

	if (Com.Port < 1 || Com.Port > 4)
	{
		puts("Using COMM com_port[1..4]");
		exit(0);
	}
	clrscr();
	init_Serial(START);

	start = 1;
	Com.RxSp = Com.RxEp;
	while(start)
	{
		if (kbhit())
		{
			ch = getch();
			if (ch == 0x1b)		start = 0;
			if (ch == '1' )		ok_send();
			if (ch == '2' )		{
				unsigned char j;
				Com.RxBuf[Com.RxEp++] = 'R';
				Com.RxBuf[Com.RxEp++] = 'S';
				for(j = 0; j<23; j++)	Com.RxBuf[Com.RxEp++] = '9';
				Com.RxBuf[Com.RxEp++] = 0x0D;
			}
		}
		while(Com.RxSp != Com.RxEp)	{
			unsigned char i;
			if('R'  != CominCH())							break;
			if('S'  != CominCH())							break;
			for(i=0; i<23; i++)		if('9' != CominCH())	break;
			if(0x0D != CominCH())							break;
			printf("\n");
			ok_send();
		}
	}
	init_Serial(END);
}
//[*]----------------------------------------------------------------------[*]
