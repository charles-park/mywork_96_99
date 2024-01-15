//[*]----------------------------------------------------------------------[*]
// Ultra Edit Used
//[*]----------------------------------------------------------------------[*]
#include <io51.h>
#include <stdio.h>
#include "LED.H"
//[*]----------------------------------------------------------------------[*]
void variable_init(void)
{
	word	i;

	watch();
	code_table[0] = &sangyongtable[0][0];
	code_table[1] = &sangyongtable[1][0];
	code_table[2] = &sangyongtable[2][0];
	Common = 0;		Hi_lo  = 0;		Sunsor = 0;

	for(i=0; i<100; i++)	{	hbuffer[i] = 0x00;	watch();	}
}
//[*]----------------------------------------------------------------------[*]
void interrupt_init(void)
{
	watch();
	disable(EA);
	TMOD = 0x01;	TH0  = 0xFF;	TL0  = 0x00;
	enable(ET0);	enable(TR0);	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void putch_engfont(char attrib, word index, byte hi_lo)
{
	word i,k;

	byte *src;
	src = Eng_Font[index];

	switch(attrib)	{
		case	0:
			if(hi_lo)	{
				for(i=0 ; i<16; i++)			{
					hbuffer[i]  = 0x00;			watch();
					for(i=0, k=0; i<32; i++)	if(  i%2 )	hbuffer[i]  = src[k++];
				}
			}	else	{
				for(i=16; i<32; i++)			{
					hbuffer[i]  = 0x00;         watch();
					for(i=0, k=0; i<32; i++)    if(!(i%2))	hbuffer[i]  = src[k++];
				}
			}													break;
		case	1:
			if(hi_lo)	{
				for(i=0 ; i<16; i++)			{
					hbuffer[i]  = 0x00;			watch();
					for(i=0, k=0; i<32; i++)	if(  i%2 )	hbuffer[i]  = ~src[k++];
				}
			}	else	{
				for(i=16; i<32; i++)			{
					hbuffer[i]  = 0x00;         watch();
					for(i=0, k=0; i<32; i++)    if(!(i%2))	hbuffer[i]  = ~src[k++];
				}
			}     												break;
		default	 :												break;
	}
	/*
	if(hi_lo)	{
		for(i=0 ; i<16; i++)			{
			hbuffer[i]  = 0x00;			watch();
			for(i=0, k=0; i<32; i++)	if(  i%2 )	hbuffer[i]  = src[k++];
		}
	}	else	{
		for(i=16; i<32; i++)			{
			hbuffer[i]  = 0x00;         watch();
			for(i=0, k=0; i<32; i++)    if(!(i%2))	hbuffer[i]  = src[k++];
		}
	}
	*/
}
//[*]----------------------------------------------------------------------[*]
void complete_hangul(char attrib, char mode, unsigned char *dest, const unsigned char *src)
{
	int i;

	switch(attrib)	{
		case	0:	if(mode)	for(i=0; i<32; i++)	dest[i]  =  src[i];
					else		for(i=0; i<32; i++)	dest[i] |=  src[i];	break;
		case	1:	if(mode)	for(i=0; i<32; i++)	dest[i]  = ~src[i];
					else		for(i=0; i<32; i++)	dest[i] &= ~src[i];	break;
		default	 :														break;
	}

	/*
	if(mode)	for(i=0; i<32; i++)	dest[i]  = src[i];
	else		for(i=0; i<32; i++)	dest[i] |= src[i];
	*/
}
//[*]----------------------------------------------------------------------[*]
/*
void clear_buffer(void)
{
	word	i;
	for(i=0; i<100; i++)	{	hbuffer[i] = 0x00;	watch();	}
}
*/
//[*]----------------------------------------------------------------------[*]
void putch_hanfont(char attrib, char *str)
{
	int fcode,mcode,lcode;
	int f1=0,f2=0,f3=0;
	char hflag = 1;

	watch();
	han.b.fchar=str[0];
	han.b.schar=str[1];

	fcode  = code_table[0][han.c.firc];
	mcode  = code_table[1][han.c.midc];
	lcode  = code_table[2][han.c.lasc];

	f3 = Dml[mcode];					
	f2 = Dfm[fcode*2+(lcode!=0)];		
	f1 = Dmf[mcode*2+(lcode!=0)];

	if(han.c.dummy)	{
		if(fcode){	complete_hangul(attrib,	    1, hbuffer, Font_Addr1+(f1*16+fcode+f1*4)*32);	hflag = 0;	}
		if(mcode){	complete_hangul(attrib, hflag, hbuffer, Font_Addr2+(f2*22+mcode     )*32);	hflag = 0;	}
		if(lcode){	complete_hangul(attrib, hflag, hbuffer, Font_Addr3+(f3*32+lcode-f3*4)*32);	hflag = 0;	}
	}	else		{
//		putch_engfont(str[0], 0);	putch_engfont(str[1], 1);
		putch_engfont(attrib, str[0], 0);	putch_engfont(attrib, str[1], 1);
//		clear_buffer();
	}
}
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] void T0_int(void)
{
	disable(EA);	disable(TR0);	disable(TF0);

	switch(Sunsor)	{
		case	0:
			output(COMMON, 0xFF);		Sunsor++;
											break;
		case	1:
			output(HI_DATA, ~hbuffer[Hi_lo++]);
			output(LO_DATA, ~hbuffer[Hi_lo++]);
			output(COMMON, Common++);	Sunsor++;
			if(Hi_lo  >= 96)        	Hi_lo  = 0;
			if(Common >= 48)			Common = 0;
											break;
		default	 :			Sunsor = 0;		break;
	}
	TH0 = 0xFF;		TL0 = 0xA0;
	enable(TR0);	enable(EA);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	word	count = 0;
	word	font  = 0;

	variable_init();	interrupt_init();

	putch_hanfont(0,"¶ä");	while(1)	watch();

	while(1)            {
		watch();
		if(count > 20000)	{
			switch(font)	{
				case	0:  putch_hanfont(0, "ï∑");	font++;		break;
				case	1:	putch_hanfont(0, "¨·");	font++;		break;
				case	2:	putch_hanfont(0, "êq");	font++;		break;
				case	3:	putch_hanfont(0, "¶Ç");	font++;		break;
				case	4:	putch_hanfont(0, "AB"); font++;		break;
				case	5:	putch_hanfont(0, " B");	font++;		break;
				case	6:	putch_hanfont(0, "A ");	font++;		break;
				case	7:	putch_hanfont(0, "  ");	font++;		break;
				case	8:	putch_hanfont(0, "Z ");	font++;		break;
				case	9:	putch_hanfont(0, " Z");	font++;		break;
				case	10:	putch_hanfont(0, "  ");	font++;		break;
				case	11:	putch_hanfont(0, "a ");	font++;		break;
				case	12:	putch_hanfont(0, " b");	font++;		break;
				case	13:	putch_hanfont(0, "ab");	font++;		break;
				case	14:	putch_hanfont(0, " z");	font++;		break;
				case	15:	putch_hanfont(0, "gj");	font++;		break;
				case    16: putch_hanfont(0, "¶ä"); font++;     break;
				case	17:	putch_hanfont(0, "99");	font++;		break;
				default	 :  putch_hanfont(0, "  "); font = 0;	break;
			}		count = 0;
		}	else	count++;
	}
}
//[*]----------------------------------------------------------------------[*]


