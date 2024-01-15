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

void hercscrup(int x1,int y1,int x2,int y2,int lines)
{
	unsigned x_width=x2-x1+1;
        unsigned off=ret_herc_offset(y1);

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
uplp3:  hercadd_dx();
        asm loop uplp3
        asm pop cx

uplp2:	asm push cx
	asm mov di,bx
        asm mov si,dx
        asm mov cx,x_width
        asm cld
        asm REP movsb
	hercadd();
        hercadd_dx();
	asm pop cx
	asm loop uplp2

	asm pop es
	asm pop ds
}
void hercsideup(int x,int y1,int y2,int lines,unsigned char mask)
{
        unsigned off=ret_herc_offset(y1);

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
        asm mov cx,WORD PTR lines
        asm mov al,BYTE PTR mask

uplp3:  hercadd_dx();
        asm loop uplp3
        asm pop cx

uplp2:  asm push cx
	asm mov di,bx
        asm mov si,dx
        asm mov ah,ds:[si]
        asm and ah,al
        asm not al
        asm and es:[di],al
        asm not al
	asm or  es:[di],ah
	hercadd();
        hercadd_dx();
	asm pop cx
	asm loop uplp2

	asm pop es
	asm pop ds
}

void hgHScrUp(int x1,int y1,int x2,int y2,int lines,int bkcolor)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        unsigned char rmask=get_rmask(x2);
        unsigned char lmask=get_lmask(x1);

        hercscrup(xs,y1,xe,y2,lines);
        hercsideup(xs-1,y1,y2,lines,lmask);
        hercsideup(xe+1,y1,y2,lines,rmask);
        hgHBoxFill(x1,y2-lines+1,x2,y2,bkcolor);
}

void hgHScrUpFast(int x1,int y1,int x2,int y2,int lines,int bkcolor)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        hercscrup(xs,y1,xe,y2,lines);
        hgHBoxFill(xs*8,y2-lines+1,xe*8+7,y2,bkcolor);
}

