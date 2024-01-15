/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        PCX VGA 16 color Subroutine     */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

void writepcxlinev(unsigned off,char *buff,int n,unsigned char lmask,unsigned char rmask)
{
        asm push ds
        asm mov dx,03ceh
        asm mov ax,0005h
        asm out dx,ax
        asm mov ax,0a000h
        asm mov es,ax
        asm lds si,buff
        asm mov di,off
        asm mov bx,di
        asm mov dx,03c4h
        asm mov al,2
        asm out dx,al
        asm inc dx
        asm mov al,1
        asm cld

        asm mov cx,4
lp1:    asm push cx

        asm push ax
        asm mov al,8
        asm mov ah,lmask
        asm mov dx,03ceh                /* GC */
        asm out dx,ax
        asm mov al,es:[di]              /* latch load */
        asm movsb                       /* left mask  */
        asm mov ax,0ff08h
        asm out dx,ax
        asm pop ax

        asm mov cx,n
lp2:    asm mov ah,es:[di]
        asm movsb
        asm loop lp2

        asm push ax
        asm mov al,8
        asm mov ah,rmask
        asm out dx,ax
        asm mov al,es:[di]              /* latch load */
        asm movsb                       /* left mask  */
        asm mov dx,03c5h
        asm pop ax

        asm shl al,1
        asm out dx,al
        asm mov di,bx
        asm pop cx
        asm loop lp1

        asm dec dx
        asm mov ax,0f02h
        asm out dx,ax
        asm pop ds
}
