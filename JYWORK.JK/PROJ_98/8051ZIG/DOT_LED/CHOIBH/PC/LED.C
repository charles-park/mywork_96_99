//[*]----------------------------------------------------------------------[*]
//#include <io51.h>

#include <conio.h>
#include <stdio.h>
#include "FONT.HPP"
//[*]----------------------------------------------------------------------[*]
void Putch_EngFont(int index);
void Putch_HanFont(char *str);
void complete_hangul(char mode, char *dest, char *src);
//[*]----------------------------------------------------------------------[*]
typedef	struct	{
	unsigned int	lasc	:	5;
	unsigned int	midc	:	5;
	unsigned int	firc	:	5;
	unsigned int	dummy	:	1;
}	hcode;
//[*]----------------------------------------------------------------------[*]
typedef	struct	{
	unsigned char	schar;
	unsigned char	fchar;
}	hchar;
//[*]----------------------------------------------------------------------[*]
union	{
	hcode c;
	hchar b;
}	han;


unsigned char hbuffer[32];
//[*]----------------------------------------------------------------------[*]
unsigned char	sangyongtable[3][33] = {
	{	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
		0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01	},
	{	0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
		0x00, 0x00, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
		0x00, 0x00, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
		0x00, 0x00, 0x12, 0x13, 0x14, 0x15, 0x00, 0x00, 0x02	},
	{	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
		0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x00, 0x11, 0x12, 0x13, 0x14, 0x15,
		0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x00, 0x00, 0x01	}
};
//[*]----------------------------------------------------------------------[*]
char Dml[22] = { 0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1 																	};
char Dfm[40] = { 1, 3, 0, 2, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 0, 2, 1, 3, 1, 3, 1, 3 			};
char Dmf[44] = { 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 1, 6, 3, 7, 3, 7, 3, 7, 1, 6, 2, 6, 4, 7, 4, 7, 4, 7, 2, 6, 1, 6, 3, 7, 0, 5 };
//[*]----------------------------------------------------------------------[*]
char *code_table[3];     // hangul code	pointer
//[*]----------------------------------------------------------------------[*]
void complete_hangul(char mode, unsigned char *dest, const unsigned char *src)
{
	int i;
	if(mode)	for(i=0; i<32; i++)	dest[i]  = src[i];
	else		for(i=0; i<32; i++)	dest[i] |= src[i];
}
//[*]----------------------------------------------------------------------[*]
void	Putch_HanFont(char *str)
{
	int fcode,mcode,lcode;
	int f1=0,f2=0,f3=0,i,j,k;
	char hflag = 1;

	han.b.fchar=str[0];
	han.b.schar=str[1];

	fcode  = code_table[0][han.c.firc];
	mcode  = code_table[1][han.c.midc];
	lcode  = code_table[2][han.c.lasc];

	f3 = Dml[mcode];
	f2 = Dfm[fcode*2+(lcode!=0)];
	f1 = Dmf[mcode*2+(lcode!=0)];

	if(fcode){	complete_hangul(    1, hbuffer, HANFONT1+(f1*16+f1*4+fcode)*32);	hflag = 0;	}
	if(mcode){	complete_hangul(hflag, hbuffer, HANFONT2+(f2*22     +mcode)*32);	hflag = 0;	}
	if(lcode){	complete_hangul(hflag, hbuffer, HANFONT3+(f3*32-f3*4+lcode)*32);	hflag = 0;	}
	for(i=0, k=0; i<16; i++)	{
		for(j=0;j<2;j++)	{
			gotoxy(10+1+j*8, i+5);	if(hbuffer[k] & 0x80)	printf("#");	else	printf(" ");
			gotoxy(10+2+j*8, i+5);	if(hbuffer[k] & 0x40)	printf("#");	else	printf(" ");
			gotoxy(10+3+j*8, i+5);	if(hbuffer[k] & 0x20)	printf("#");	else	printf(" ");
			gotoxy(10+4+j*8, i+5);	if(hbuffer[k] & 0x10)	printf("#");	else	printf(" ");
			gotoxy(10+5+j*8, i+5);	if(hbuffer[k] & 0x08)	printf("#");	else	printf(" ");
			gotoxy(10+6+j*8, i+5);	if(hbuffer[k] & 0x04)	printf("#");	else	printf(" ");
			gotoxy(10+7+j*8, i+5);	if(hbuffer[k] & 0x02)	printf("#");	else	printf(" ");
			gotoxy(10+8+j*8, i+5);	if(hbuffer[k] & 0x01)	printf("#");	else	printf(" ");
			k++;
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void	Putch_EngFont(int index)
{
	int	i;
	for(i = 0; i<16; i++)	{
		gotoxy(10+1, i+5);	if(ENGFONT[index][i] & 0x80)	printf("#");	else	printf(" ");
		gotoxy(10+2, i+5);	if(ENGFONT[index][i] & 0x40)	printf("#");	else	printf(" ");
		gotoxy(10+3, i+5);	if(ENGFONT[index][i] & 0x20)	printf("#");	else	printf(" ");
		gotoxy(10+4, i+5);	if(ENGFONT[index][i] & 0x10)	printf("#");	else	printf(" ");
		gotoxy(10+5, i+5);	if(ENGFONT[index][i] & 0x08)	printf("#");	else	printf(" ");
		gotoxy(10+6, i+5);	if(ENGFONT[index][i] & 0x04)	printf("#");	else	printf(" ");
		gotoxy(10+7, i+5);	if(ENGFONT[index][i] & 0x02)	printf("#");	else	printf(" ");
		gotoxy(10+8, i+5);	if(ENGFONT[index][i] & 0x01)	printf("#");	else	printf(" ");
	}
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	code_table[0] = &sangyongtable[0][0];
	code_table[1] = &sangyongtable[1][0];
	code_table[2] = &sangyongtable[2][0];

	Putch_EngFont('A');     getch();

    Putch_HanFont("¦‚");    getch();
    Putch_HanFont("Ò—");    getch();
    Putch_HanFont("ÓW");    getch();
    Putch_HanFont("•·");    getch();
    Putch_HanFont("¬á");    getch();
    Putch_HanFont("q");    getch();
    Putch_HanFont("¦‚");    getch();
    Putch_HanFont("Ów");    getch();
    Putch_HanFont("ª‚");    getch();
    Putch_HanFont("ª‚");    getch();
    Putch_HanFont("ª—");    getch();
    Putch_HanFont("ªw");    getch();
}
//[*]----------------------------------------------------------------------[*]
