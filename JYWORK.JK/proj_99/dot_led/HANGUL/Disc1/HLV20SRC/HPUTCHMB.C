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

void hputchmn_out_byte(int x,int y,char *buff,int m,int n,char color)
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
	asm shr dx,1
	asm shr dx,1
	asm shr dx,1
	asm add bl,dl
	asm adc bh,0

	asm mov cx,WORD PTR y_width
nloop:  asm push cx
	asm mov cx,WORD PTR x_width
        asm mov di,bx
mloop:  asm mov al,ds:[si]
        asm cmp BYTE PTR color,ZERO
	asm jnz enext2
	asm not al
        asm and es:[di],al
        asm jmp enext3
enext2: asm or es:[di],al
enext3: asm inc si
	asm inc di
	asm loop mloop
	asm pop cx
        asm add bx,2000h
        asm cmp bx,8000h
	asm jb enext4
        asm sub bx,07fb0h
enext4:	asm loop nloop

	asm pop ds
}
