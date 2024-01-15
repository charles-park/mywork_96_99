/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Hangul Byte Output           */
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

#define ZERO 0         /* 0 is BLACK */

void hputche_byte(int x,int y,char *buff,char color)
{
        unsigned off=ret_herc_offset(y);

        asm push ds

        asm mov bx,off
        asm lds si,buff
        asm mov ax,0b000h               /* segment setting */
	asm mov es,ax
	asm mov dx,WORD PTR x
	asm mov ax,dx
	asm shr dx,1
	asm shr dx,1
	asm shr dx,1
	asm add bl,dl
	asm adc bh,0

	asm mov cx,16
	asm mov di,bx

eloop:  asm mov al,ds:[si]
        asm cmp BYTE PTR color,ZERO
        asm jnz enext2
        asm not al
        asm and es:[di],al
        asm jmp jp1
enext2: asm or es:[di],al
jp1:    asm inc si
	asm add di,2000h
	asm cmp di,8000h
	asm jb enext
        asm sub di,7fb0h
enext:  asm loop eloop

	asm pop ds
}

void hputchh_byte(int x,int y,char *buff,char color)
{
        unsigned off=ret_herc_offset(y);

        asm push ds

        asm mov bx,off
        asm lds si,buff
	asm mov ax,0b000h		/* segment setting */
	asm mov es,ax
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

hloop:  asm mov ax,ds:[si]
        asm cmp BYTE PTR color,ZERO
	asm jnz hnext2
        asm not ax
        asm and es:[di],ax
        asm jmp jp1
hnext2: asm or es:[di],ax
jp1:    asm inc si
        asm inc si
	asm add di,2000h
	asm cmp di,8000h
	asm jb hnext
        asm sub di,7fb0h
hnext:  asm loop hloop

        asm pop ds
}
