/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           GENOA PageRoutine            */
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

void genoa(void)
{
             asm push dx
             asm push ax
             asm mov al,dl
             asm and al,7
             asm mov ah,al
             asm shl al,1
             asm shl al,1
             asm shl al,1
             asm or ah,al
             asm mov dx,3c4h
             asm mov al,6
             asm out dx,al
             asm inc dx
             asm in al,dx
             asm and al,0c0h
             asm or al,ah
             asm out dx,al
             asm pop ax
             asm pop dx
}
int isgenoa()
{
        unsigned char far *p;
        p=(unsigned char far *)MK_FP(0xc000,0x00b4);

        if (p[0]==0x77 && p[1]==0x11 && p[2]==0x99 && p[3]==0x66)
                return 1;
        else
                return 0;
}
