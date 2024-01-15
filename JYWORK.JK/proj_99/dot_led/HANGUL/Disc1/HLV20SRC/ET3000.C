/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          ET3000 PageRoutine            */
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

void et3000(void)
{
             asm push dx
             asm push ax
             asm mov ah,dl
             asm shl ah,1
             asm shl ah,1
             asm shl ah,1
             asm or  ah,dl
             asm mov dx,3cdh
             asm in  al,dx
             asm and al,0c0h
             asm or  al,ah
             asm out dx,al
             asm pop ax
             asm pop dx
}

int iset3000()
{
        asm mov dx,3cdh
        asm in  al,dx
        asm mov ah,al
        asm and al,0c0h
        asm or  al,55h
        asm out dx,al
        asm in  al,dx
        asm cmp al,55h
        asm jne notnext
        asm mov al,0aah
        asm out dx,al
        asm in  al,dx
        asm cmp al,0aah
        asm jne notnext
        asm mov al,ah
        asm out dx,al
        return 1;

notnext:asm mov al,ah
        asm out dx,al
        return 0;
}
