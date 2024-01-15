/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          VGA-horiz,vert line           */
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

void vbyteline(int x1,int x2,int y,char color)
{
        int xw=xbyte;

        asm mov cx,x2
        asm sub cx,x1
        asm inc cx

        asm mov bx,xw
        asm mov ax,WORD PTR y
        asm mul bx
        asm add ax,WORD PTR x1
        asm mov di,ax

        asm mov ax,0a000h
        asm mov es,ax

        asm mov dx,03ceh
        asm mov ax,0205h
        asm out dx,ax
	asm mov ax,0ff08h
	asm out dx,ax

        asm mov al,BYTE PTR color
        asm REP stosb

        asm mov ax,0005h
        asm out dx,ax
}

void vsideline(int x,int y,char color,unsigned char mask)
{
        int xw=xbyte;

        asm mov bx,xw
        asm mov ax,WORD PTR y
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

        asm mov al,es:[di]
        asm mov es:[di],ah

        asm mov ax,0005h
        asm out dx,ax
        asm mov ax,0ff08h
        asm out dx,ax
}

void vvertline(int x,int y1,int y2,char color,unsigned char mask)
{
        int xw=xbyte;

        asm mov cx,y2
        asm sub cx,y1
        asm inc cx

        asm mov bx,xw
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

vloop:  asm mov al,es:[di]
        asm mov es:[di],ah
        asm add di,xw
	asm loop vloop

        asm mov ax,0005h
        asm out dx,ax
        asm mov ax,0ff08h
        asm out dx,ax
}

void hgVhline(int x1,int x2,int y,char color)
{
	int xs;
	int xe;
	unsigned char rmask=get_rmask(x2);
	unsigned char lmask=get_lmask(x1);

	xs=x1/8+1;
	xe=x2/8-1;

        vbyteline(xs,xe,y,color);
	vsideline(xs-1,y,color,lmask);
	vsideline(xe+1,y,color,rmask);
}
void hgVvline(int x,int y1,int y2,char color)
{
        vvertline(x>>3,y1,y2,color,get_vmask(x));
}
