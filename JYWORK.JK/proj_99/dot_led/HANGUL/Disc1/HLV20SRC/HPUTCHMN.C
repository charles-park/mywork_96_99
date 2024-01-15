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

#define ZERO 0     /* special define */

#include <dos.h>

extern int ret_herc_offset(int y);

void hputchmn_out(int x,int y,char *buff,int m,int n,char color)
{
	int x_width=m;
	int y_width=n;
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
	asm adc bh,0
        asm and al,07h
	asm mov cl,al
nosign: asm mov ah,0ffh
        asm shr ah,cl                   /* ah = bitmask */

	asm mov di,bx
	asm mov bx,ax			/* bx <- ax */

	asm mov cx,WORD PTR y_width
nloop:  asm push cx
	asm push di
	asm mov cx,WORD PTR x_width
mloop:  asm push cx
	asm mov cl,al
	asm mov al,ds:[si]
	asm ror al,cl
	asm mov ch,al
        asm cmp BYTE PTR color,ZERO
	asm jnz enext2

	asm not al
        asm not ah
        asm or  ah,al
        asm and es:[di],ah
        asm jmp en1

enext2:	asm and ah,al
       	asm or es:[di],ah
en1:    asm mov ax,bx
	asm mov al,ch
	asm not ah
        asm cmp BYTE PTR color,ZERO
	asm jnz enext3

        asm not al
        asm not ah
        asm or  ah,al
        asm and es:[di+1],ah
        asm jmp en2

enext3:	asm and ah,al
	asm or es:[di+1],ah
en2:    asm mov ax,bx
	asm inc si
	asm inc di
        asm pop cx
	asm loop mloop
	asm pop di
	asm pop cx
	asm add di,2000h
	asm cmp di,8000h
	asm jb enext4
        asm sub di,07fb0h
enext4:	asm loop nloop

	asm pop ds
}
