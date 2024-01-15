/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Hangul Output               */
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

#include <dos.h>
#include "hghlib.h"

void hercscrdown(int x1,int y1,int x2,int y2,int lines)
{
	unsigned x_width=x2-x1+1;
        unsigned off=ret_herc_offset(y2);

        asm mov bx,off
        asm add bx,x1
        asm mov cx,y2
        asm sub cx,y1
        asm sub cx,lines
        asm inc cx

	asm push ds
	asm push es
        asm push cx

	asm mov ax,0b000h
	asm mov es,ax
	asm mov ds,ax
        asm mov dx,bx
        asm mov cx,WORD PTR lines
dnlp3:  hercsub_dx();
        asm loop dnlp3
        asm pop cx

dnlp2:	asm push cx
	asm mov di,bx
        asm mov si,dx
        asm mov cx,x_width
        asm cld
        asm REP movsb
        hercsub();
        hercsub_dx();
        asm pop cx
	asm loop dnlp2

	asm pop es
	asm pop ds
}
void hercsidedown(int x,int y1,int y2,int lines,unsigned char mask)
{
        unsigned off=ret_herc_offset(y2);

        asm mov bx,off
        asm add bx,x
        asm mov cx,y2
        asm sub cx,y1
        asm sub cx,lines
        asm inc cx

	asm push ds
	asm push es
        asm push cx

	asm mov ax,0b000h
	asm mov es,ax
	asm mov ds,ax
        asm mov dx,bx
        asm mov al,BYTE PTR mask

        asm mov cx,WORD PTR lines
dnlp3:  hercsub_dx();
        asm loop dnlp3
        asm pop cx

dnlp2:	asm push cx
	asm mov di,bx
        asm mov si,dx
        asm mov ah,ds:[si]
        asm and ah,al
        asm not al
        asm and es:[di],al
        asm not al
	asm or  es:[di],ah
        hercsub();
        hercsub_dx();
        asm pop cx
	asm loop dnlp2

	asm pop es
	asm pop ds
}

void hgHScrDown(int x1,int y1,int x2,int y2,int lines,int bkcolor)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        unsigned char rmask=get_rmask(x2);
        unsigned char lmask=get_lmask(x1);

        hercscrdown(xs,y1,xe,y2,lines);
        hercsidedown(xe+1,y1,y2,lines,rmask);
        hercsidedown(xs-1,y1,y2,lines,lmask);
        hgHBoxFill(x1,y1,x2,y1+lines-1,bkcolor);
}
void hgHScrDownFast(int x1,int y1,int x2,int y2,int lines,int bkcolor)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        hercscrdown(xs,y1,xe,y2,lines);
        hgHBoxFill(xs*8,y1,xe*8+7,y1+lines-1,bkcolor);
}

