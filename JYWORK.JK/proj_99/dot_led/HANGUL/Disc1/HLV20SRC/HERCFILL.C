/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Hercules Fill               */
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

unsigned char hgHXP[16];
unsigned char hgYXP[16];

unsigned char hgGetHFillXPattern(int color)
{
        return hgHXP[color];
}
unsigned char hgGetHFillYPattern(int color)
{
	return hgYXP[color];
}
void hgSetHFillXPattern(int color,unsigned char pattern)
{
	hgHXP[color]=pattern;
}
void hgSetHFillYPattern(int color,unsigned char pattern)
{
	hgYXP[color]=pattern;
}
void hgSetHFillDPattern()
{
	int i;
	unsigned char xx[16]=
                   { 0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xff,
                     0xff,0xff,0xff,0xff,0xff,0xff };
	unsigned char yy[16]=
                   { 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0xff,
		     0xff,0xff,0xff,0xff,0xff,0xff };

	for (i=0;i<16;i++) {
                hgSetHFillXPattern(i,xx[i]);
                hgSetHFillYPattern(i,yy[i]);
	}
}

void hercfill(int x1,int y1,int x2,int y2,int color)
{
	unsigned x_width=x2-x1+1;
	unsigned y_width=y2-y1+1;
        unsigned char xpattern=hgGetHFillXPattern(color);
        unsigned char ypattern=hgGetHFillYPattern(color);
        unsigned off=ret_herc_offset(y1);

        asm mov bx,off
        asm add bx,x1
	asm push ds
	asm push es
	asm mov ax,0b000h
	asm mov ds,ax
	asm mov es,ax
	asm mov cx,y_width
	asm mov al,BYTE PTR xpattern
	asm mov dl,BYTE PTR ypattern

        asm rol dl,1            /* first bit must be 0,so pass. */
        asm mov di,WORD PTR y1
        asm shr di,1
        asm jnc boxlp1
        asm rol al,1            /* rotate one more time */

boxlp1: asm push cx
	asm mov cx,x_width
	asm mov di,bx
        asm REP stosb
        hercadd();
noad:   asm rol al,1
	asm cmp al,0
	asm je  nost
	asm mov BYTE PTR xpattern,al
nost:	asm pop cx
        asm push cx
        ypat(xpattern);
        asm pop cx
boxn1:  asm loop boxlp1

	asm pop es
	asm pop ds
}

void hsidefill(int x,int y1,int y2,unsigned char color,unsigned char mask)
{
        unsigned char xpattern=hgGetHFillXPattern(color);
        unsigned char ypattern=hgGetHFillYPattern(color);
        unsigned off=ret_herc_offset(y1);

        asm mov bx,off
        asm add bx,x
        asm mov cx,y2
        asm sub cx,y1
        asm inc cx
	asm mov ax,0b000h
	asm mov es,ax
	asm mov al,BYTE PTR xpattern
        asm mov di,WORD PTR y1
        asm shr di,1
        asm jnc sf1
        asm rol al,1            /* rotate one more time */

sf1:    asm mov ah,al
	asm mov dh,BYTE PTR mask
	asm and ah,dh
	asm mov dl,BYTE PTR ypattern
	asm rol dl,1

sfill:  asm not dh
        asm and es:[bx],dh
        asm not dh
	asm or  es:[bx],ah
	asm rol al,1
	asm cmp al,0
	asm je  nost
	asm mov BYTE PTR xpattern,al
nost:	hercadd();
        asm push cx
	ypat(xpattern);
        asm pop cx
	asm mov ah,al
        asm and ah,dh
	asm loop sfill
}

void hgHBoxFill(int x1,int y1,int x2,int y2,int color)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        unsigned char rmask=get_rmask(x2);
        unsigned char lmask=get_lmask(x1);

        hercfill(xs,y1,xe,y2,color);
        hsidefill(xs-1,y1,y2,color,lmask);
        hsidefill(xe+1,y1,y2,color,rmask);
}
void hgHBoxFillFast(int x1,int y1,int x2,int y2,int color)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        hercfill(xs,y1,xe,y2,color);
}
