/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            VGA fill Routine            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hghlib.h"

#pragma inline

extern int xbyte;

void vgafill(int x1,int y1,int x2,int y2,int color)
{
	unsigned x_width=x2-x1+1;
	unsigned y_width=y2-y1+1;
        int      xwidth=xbyte;

	asm push ds
	asm push es

	asm mov ax,0a000h
	asm mov ds,ax
	asm mov es,ax

	asm mov dx,3ceh
	asm mov ax,0205h
	asm out dx,ax
	asm mov ax,0ff08h
	asm out dx,ax
	asm mov cx,y_width
	asm mov ax,y1
        asm mov bx,xwidth
	asm mul bx
        asm add ax,x1
	asm mov bx,ax

        asm mov al,BYTE PTR color

boxlp1: asm push cx
        asm mov cx,x_width
        asm mov di,bx
boxlp2: asm REP stosb
        asm add bx,xwidth
        asm pop cx
        asm loop boxlp1

        asm mov dx,3ceh
        asm mov ax,5
        asm out dx,ax

        asm pop es
        asm pop ds
}

void vsidefill(int x,int y1,int y2,unsigned char color,unsigned char mask)
{
        int xwidth=xbyte;

        asm mov cx,y2
        asm sub cx,y1
        asm inc cx

        asm mov bx,xwidth
        asm mov ax,WORD PTR y1
        asm mul bx
        asm add ax,WORD PTR x
        asm mov di,ax

        asm mov ax,0a000h
        asm mov es,ax

        asm mov dx,03ceh
        asm mov ax,0205h
        asm out dx,ax

        asm mov al,8
        asm mov ah,BYTE PTR mask
	asm out dx,ax

	asm mov ah,BYTE PTR color

sfill:  asm mov al,es:[di]
	asm mov es:[di],ah
        asm add di,xwidth
        asm loop sfill

        asm mov ax,0005h
        asm out dx,ax
        asm mov ax,0ff08h
        asm out dx,ax
}

void hgVBoxFill(int x1,int y1,int x2,int y2,int color)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

        unsigned char rmask=get_rmask(x2);
        unsigned char lmask=get_lmask(x1);

	vgafill(xs,y1,xe,y2,color);
        vsidefill(xs-1,y1,y2,color,lmask);
        vsidefill(xe+1,y1,y2,color,rmask);
}
void hgVBoxFillFast(int x1,int y1,int x2,int y2,int color)
{
	int xs=x1/8+1;
	int xe=x2/8-1;

	vgafill(xs,y1,xe,y2,color);
}
