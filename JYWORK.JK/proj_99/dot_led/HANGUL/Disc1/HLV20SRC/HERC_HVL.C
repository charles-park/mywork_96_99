/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          Herc Horiz-Vert line          */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#pragma inline

#include "hghlib.h"

unsigned char hlxpat[16];
unsigned char hlypat[16];

unsigned char hgGetHlinePattern(char color)
{
	return hlxpat[color];
}
unsigned char hgGetVlinePattern(char color)
{
	return hlypat[color];
}
void hgSetHlinePattern(char color,unsigned char pat)
{
	hlxpat[color]=pat;
}
void hgSetVlinePattern(char color,unsigned char pat)
{
	hlypat[color]=pat;
}
void hgSetHVlineDefault()
{
        unsigned char xx[16]={ 0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0x00,
                               0xff,0xff,0xff,0xff,0xff,0xff,0xff };
        unsigned char yy[16]={ 0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0x00,
                               0xff,0xff,0xff,0xff,0xff,0xff,0xff };
	int i;

	for (i=0;i<16;i++) {
		hgSetHlinePattern(i,xx[i]);
		hgSetVlinePattern(i,yy[i]);
	}
}

#pragma inline

void hbyteline(int x1,int x2,int y,char color)
{
	unsigned char xpattern=hgGetHlinePattern(color);
        unsigned int  off=ret_herc_offset(y);

        asm mov di,off
        asm add di,x1
        asm mov cx,x2
        asm sub cx,x1
        asm inc cx
	asm mov ax,0b000h
	asm mov es,ax
	asm mov al,BYTE PTR xpattern;
blop:	asm mov es:[di],al
	asm inc di
	asm loop blop
}

void hsideline(int x,int y,char color,unsigned char mask)
{
	unsigned char xpattern=hgGetHlinePattern(color);
        unsigned int  off=ret_herc_offset(y);

        asm mov bx,off
        asm add bx,x
        asm mov ax,0b000h
        asm mov es,ax
        asm mov ah,BYTE PTR xpattern
        asm mov al,BYTE PTR mask
        asm and ah,al
        asm not al
        asm and es:[bx],al
        asm or  es:[bx],ah
}

void hvertline(int x,int y1,int y2,char color,unsigned char mask)
{
	unsigned char ypattern=hgGetVlinePattern(color);
        unsigned int  off=ret_herc_offset(y1);

        asm mov bx,off
        asm add bx,x
        asm mov cx,y2
        asm sub cx,y1
        asm inc cx
        asm mov ax,0b000h
	asm mov es,ax
	asm mov dl,BYTE PTR ypattern
	asm mov al,BYTE PTR mask

bfill:  asm cmp dl,0
        asm jnz bnext

        asm not al
        asm and es:[bx],al
        asm not al
        hercadd();
        asm jmp bnext2

bnext:  asm or  es:[bx],al
	hercadd();
	asm push cx
	ypat(mask);
	asm pop  cx

bnext2: asm loop bfill
}

void hgHhline(int x1,int x2,int y,char color)
{
	int xs;
	int xe;

	xs=x1/8+1;
	xe=x2/8-1;

	hbyteline(xs,xe,y,color);
        hsideline(xs-1,y,color,get_lmask(x1));
        hsideline(xe+1,y,color,get_rmask(x2));
}
void hgHvline(int x,int y1,int y2,char color)
{
	hvertline(x>>3,y1,y2,color,get_vmask(x));
}

