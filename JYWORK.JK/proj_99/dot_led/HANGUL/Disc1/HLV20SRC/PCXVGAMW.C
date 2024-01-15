/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        PCX VGA-Hercules Subroutine     */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

void writepcxlinem(unsigned off,char *buff,int n,char color)
{
        asm push ds
        asm mov ax,0a000h
        asm mov es,ax
        asm lds si,buff
        asm mov di,off
        asm mov dx,03ceh
        asm mov ax,0205h
        asm out dx,ax
        asm cld
        asm mov al,8
        asm out dx,al
        asm inc dx
        asm mov al,color

        asm mov cx,n
lp1:    asm mov ah,ds:[si]
        asm xchg al,ah
        asm out dx,al
        asm xchg al,ah
        asm mov ah,es:[di]      /* for latch load */
        asm stosb
        asm inc si
        asm loop lp1

        asm dec dx
        asm mov ax,0ff08h
        asm out dx,ax
        asm mov ax,0005h
        asm out dx,ax
        asm pop ds
}
