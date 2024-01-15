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

#define ZERO    0

int ret_herc_offset(int y)
{
	int m,n;

	n=y>>2;
        m=y-(n<<2);
        return (0x2000*m+80*n);
}

void hputche(int x,int y,char *buff,char color)
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
	asm and al,07h
	asm mov cl,al
nosign: asm mov ah,0ffh
        asm shr ah,cl                   /* ah = bitmask */

	asm mov cx,16
	asm mov di,bx
	asm mov bx,ax			/* bx <- ax */

eloop:  asm push cx
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
        asm jmp jp1

enext2: asm and ah,al
       	asm or es:[di],ah
jp1:    asm mov ax,bx
	asm mov al,ch
	asm not ah
        asm cmp BYTE PTR color,ZERO
	asm jnz enext3

        asm not al
        asm not ah
        asm or ah,al
        asm and es:[di+1],ah
        asm jmp jp2

enext3: asm and ah,al
	asm or es:[di+1],ah
jp2:    asm mov ax,bx
	asm inc si
	asm add di,2000h
	asm cmp di,8000h
	asm jb enext
        asm sub di,7fb0h
enext:	asm pop cx
	asm loop eloop

	asm pop ds
}

void hputchh(int x,int y,char *buff,char color)
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
	asm and al,07h
	asm mov cl,al
nosign: asm mov ah,0ffh
        asm shr ah,cl                   /* ah = bitmask */

	asm mov cx,16
	asm mov dx,03ceh
	asm mov di,bx
	asm mov bx,ax			/* bx <- ax */

hloop:  asm push cx
	asm mov cl,al
	asm mov al,ds:[si]
	asm ror al,cl
	asm mov ch,al
        asm cmp BYTE PTR color,ZERO
	asm jnz hnext2

        asm not al
        asm not ah
        asm or ah,al
        asm and es:[di],ah
        asm jmp jp1

hnext2: asm and ah,al
	asm or es:[di],ah
jp1:    asm mov ax,bx
	asm mov al,ch
	asm not ah
        asm cmp BYTE PTR color,ZERO
	asm jnz hnext5

        asm not al
        asm not ah
        asm or ah,al
        asm and es:[di+1],ah
        asm jmp jp2

hnext5: asm and ah,al
	asm or es:[di+1],ah
jp2:    asm mov ax,bx
	asm inc si
	asm mov al,ds:[si]
	asm ror al,cl
	asm mov ch,al
	asm and al,ah
        asm cmp BYTE PTR color,ZERO
	asm jnz hnext3

        asm not al
        asm not ah
        asm or ah,al
        asm and es:[di+1],ah
        asm jmp jp3

hnext3: asm and ah,al
	asm or es:[di+1],ah
jp3:    asm mov ax,bx
	asm mov al,ch
	asm not ah
        asm cmp BYTE PTR color,ZERO
	asm jnz hnext4

        asm not al
        asm not ah
        asm or ah,al
        asm and es:[di+2],ah
        asm jmp jp4

hnext4: asm and ah,al
	asm or es:[di+2],ah
jp4:    asm mov ax,bx
	asm inc si
	asm add di,2000h
	asm cmp di,8000h
	asm jb hnext
        asm sub di,7fb0h
hnext:	asm pop cx
        asm dec cx
        asm cmp cx,0
        asm je  final
        asm jmp hloop

final:  asm pop ds
}
