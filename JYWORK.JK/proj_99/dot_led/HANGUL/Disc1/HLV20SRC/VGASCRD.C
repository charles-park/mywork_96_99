/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            VGA scroll down             */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <dos.h>
#include "hghlib.h"

#pragma inline

extern int xbyte;

void vgascrdown(int x1,int y1,int x2,int y2,int lines)
{
        int      xw=xbyte;
        unsigned x_width=x2-x1+1;
        unsigned y_width=xw*lines;

	asm push ds
	asm push es

        asm mov cx,y2
        asm sub cx,y1
        asm sub cx,lines
        asm inc cx

	asm mov ax,0a000h
	asm mov es,ax
	asm mov ds,ax

        asm mov ax,WORD PTR y2
        asm mov dx,xw
	asm mul dx
	asm mov bx,ax
        asm add bx,WORD PTR x1

	asm mov dx,3ceh
        asm mov ax,0105h
	asm out dx,ax

dnlp2:  asm push cx
        asm mov cx,x_width
        asm mov di,bx
        asm mov si,di
        asm sub si,y_width
        asm cld
        asm REP movsb
        asm sub bx,xw
        asm pop cx
        asm loop dnlp2

        asm mov ax,0005h
        asm out dx,ax

        asm pop es
        asm pop ds
}
void vgasidedown(int x,int y1,int y2,int lines,unsigned char mask)
{
        int      xw=xbyte;
        unsigned y_width=xw*lines;

	asm push ds
	asm push es

        asm mov cx,y2
        asm sub cx,y1
        asm sub cx,lines
        asm inc cx

	asm mov ax,0a000h
	asm mov es,ax
	asm mov ds,ax

        asm mov ax,WORD PTR y2
        asm mov dx,xw
	asm mul dx
	asm mov bx,ax
        asm add bx,WORD PTR x

        asm mov dx,3ceh                 /* write mode 0 */
        asm mov ax,0005h
	asm out dx,ax
        asm mov ax,0000h
        asm out dx,ax

        asm mov al,8
        asm mov ah,BYTE PTR mask        /* bit mask */
        asm out dx,ax

        asm mov di,bx
        asm mov si,di
        asm sub si,y_width
        asm sub dx,10                   /* just cal. */

uplp2:  asm push cx

        asm mov cx,3    /* counter & read */
        asm mov bl,8    /* write */

uplp3:  asm add dx,10           /* GC */
        asm mov ah,cl           /* read counter */
        asm mov al,4            /* func 4 */
        asm out dx,ax
        asm mov bh,ds:[si]      /* read */
        asm sub dx,10           /* sequencer */
        asm mov al,2            /* func 2 */
        asm mov ah,bl           /* write counter */
        asm out dx,ax
        asm mov ch,es:[di]      /* fill latch */
        asm mov es:[di],bh      /* write */
        asm shr bl,1            /* rotate write counter */
        asm mov ch,0            /* high byte of CX must be 0 */
        asm dec cx
        asm cmp cx,-1
        asm jne uplp3

        asm sub si,xw
        asm sub di,xw

        asm pop cx
        asm loop uplp2

        asm mov ax,0ff02h       /* sequencer init */
        asm out dx,ax

        asm mov dx,03ceh        /* GC init */
        asm mov ax,0ff08h
        asm out dx,ax
        asm mov ax,0ff04h
        asm out dx,ax

        asm pop es
	asm pop ds
}

void hgVScrDown(int x1,int y1,int x2,int y2,int lines,int bkcolor)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        unsigned char rmask=get_rmask(x2);
        unsigned char lmask=get_lmask(x1);

        vgascrdown(xs,y1,xe,y2,lines);
        vgasidedown(xs-1,y1,y2,lines,lmask);
        vgasidedown(xe+1,y1,y2,lines,rmask);
        hgVBoxFill(x1,y1,x2,y1+lines-1,bkcolor);
}
void hgVScrDownFast(int x1,int y1,int x2,int y2,int lines,int bkcolor)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        vgascrdown(xs,y1,xe,y2,lines);
        hgVBoxFill(xs*8,y1,xe*8+7,y1+lines-1,bkcolor);
}
