/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            VGA MxN PutCh               */
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

extern int xbyte;

void vputchmn_out(int x,int y,char *buff,int m,int n,char color)
{
	int y_width=n;
	int x_width=m;
        int xwidth=xbyte;

	asm push ds

        asm lds si,buff                 /* load address */
	asm mov dx,3ceh			/* writing mode */
	asm mov ax,0005h
	asm out dx,ax
	asm mov al,0
	asm mov ah,color
	asm out dx,ax
	asm mov ax,0f01h
	asm out dx,ax

	asm mov ax,0a000h		/* segment setting */
	asm mov es,ax

	asm mov ax,WORD PTR y		/* offset,bitmask */
        asm mov bx,xwidth
	asm mul bx
	asm mov bx,ax
	asm mov dx,WORD PTR x
	asm mov ax,dx
	asm shr dx,1
	asm shr dx,1
	asm shr dx,1
	asm add bl,dl
	asm adc bh,0   			/* bx = offset */
        asm and al,07h
	asm mov cl,al
nosign: asm mov ah,0ffh
        asm shr ah,cl                   /* ah = bitmask */

	asm mov dx,03ceh
	asm mov di,bx
	asm mov bx,ax			/* bx <- ax */

	asm mov cx,WORD PTR y_width
nloop :	asm push cx
	asm push di			/* for screen offset's recovery */
	asm mov cx,WORD PTR x_width
mloop : asm push cx
	asm mov cl,al
	asm mov al,ds:[si]
	asm ror al,cl
	asm and ah,al
	asm mov ch,al
	asm mov al,8
	asm out dx,ax
	asm mov ah,es:[di]		/* output left part */
	asm mov es:[di],ah
	asm mov ax,bx
	asm mov al,ch
	asm not ah
	asm and ah,al
	asm mov al,8
	asm out dx,ax
	asm mov ah,es:[di+1]		/* output right part */
	asm mov es:[di+1],ah
	asm mov ax,bx
	asm inc si
	asm inc di
	asm pop cx
	asm loop mloop
	asm pop di
	asm pop cx
        asm add di,xwidth
	asm loop nloop

	asm mov ax,1		/* initializing */
	asm out dx,ax
	asm mov ax,0ff08h
	asm out dx,ax

	asm pop ds
}
