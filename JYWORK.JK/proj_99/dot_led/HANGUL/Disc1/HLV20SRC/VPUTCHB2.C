/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             VGA Byte Putch             */
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

void vputche_byte_vga(int x,int y,char *buff,char color)
{
        int xwidth=xbyte;

        asm push ds

        asm lds si,buff                 /* load */

	asm mov dx,3ceh			/* writing mode */
        asm mov ax,0305h
	asm out dx,ax
	asm mov al,0
	asm mov ah,color
	asm out dx,ax
        asm mov ax,0ff08h
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

	asm mov cx,16
	asm mov dx,03ceh
	asm mov di,bx

eloop:  asm mov ah,ds:[si]
        asm mov al,es:[di]              /* output left part */
	asm mov es:[di],ah
        asm inc si
        asm add di,xwidth
	asm loop eloop

        asm mov ax,0005h
        asm out dx,ax

	asm pop ds
}

void vputchh_byte_vga(int x,int y,char *buff,char color)
{
        int xwidth=xbyte;

        asm push ds

        asm lds si,buff                 /* load */

	asm mov dx,3ceh			/* writing mode */
        asm mov ax,0305h
	asm out dx,ax
	asm mov al,0
	asm mov ah,color
	asm out dx,ax
        asm mov ax,0ff08h
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

	asm mov cx,16
	asm mov dx,03ceh
	asm mov di,bx

hloop:  asm mov ah,ds:[si]
        asm mov al,es:[di]              /* output left part */
	asm mov es:[di],ah
        asm inc si
        asm mov ah,ds:[si]
        asm mov al,es:[di+1]
        asm mov es:[di+1],ah
        asm inc si
        asm add di,xwidth
	asm loop hloop

        asm mov ax,0005h
        asm out dx,ax

	asm pop ds
}
