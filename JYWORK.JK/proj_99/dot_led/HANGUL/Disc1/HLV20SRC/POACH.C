/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          POACH PageRoutine             */
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

#include <dos.h>

void poach(void)  /* for ZyMOS Poach51 */
{
        asm push dx
        asm push ax
        asm mov al,dl
        asm and al,7
        asm mov ah,al
        asm xor ah,02h
        asm mov dx,03c4h
        asm mov al,0eh
        asm out dx,al
        asm inc dx
        asm in  al,dx
        asm and al,0f0h
        asm or  al,ah
        asm out dx,al
        asm pop ax
        asm pop dx
}
int ispoach()
{
        asm mov dx,3c4h
        asm mov al,0bh
        asm out dx,al
        asm inc dx
        asm in  al,dx
        asm and al,0fh
        asm cmp al,2
        asm jne next
        return 1;
next:   return 0;
}
void poach_special()
{
        asm mov dx,03c4h
        asm mov al,0bh
        asm out dx,al
        asm inc dx
        asm in al,dx            /* force Version 2 Mode */

        asm mov dx,03ceh
        asm mov al,6            /* select Miscelaneous register */
        asm out dx,al
        asm inc dx
        asm in al,dx
        asm and al,0f3h
        asm or al,04h
        asm out dx,al
}
