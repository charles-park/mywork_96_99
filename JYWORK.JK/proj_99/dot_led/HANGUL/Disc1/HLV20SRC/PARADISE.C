/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*       Western Digital PARADISE         */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

/* dl is page number */

void paradise(void)
{
        asm push dx
        asm push ax
        asm mov ah,dl
        asm shl ah,1
        asm shl ah,1
        asm shl ah,1
        asm shl ah,1
        asm mov al,09h
        asm mov dx,3ceh
        asm out dx,ax
        asm pop ax
        asm pop dx
}
int isparadise()
{
        asm push ds
        asm mov ax,0c000h
        asm mov ds,ax
        asm mov si,7dh
        asm cmp WORD PTR [si],4756h
        asm jne notnext
        asm cmp WORD PTR [si+2],3d41h
        asm jne notnext
        asm pop ds
        return 1;
notnext:asm pop ds
        return 0;
}
void paradise_special()
{
        asm mov dx,03ceh
        asm mov ax,050fh
        asm out dx,ax
        asm mov al,0bh
        asm out dx,al
        asm inc dx
        asm in al,dx
        asm and al,0f7h
        asm out dx,al
}
