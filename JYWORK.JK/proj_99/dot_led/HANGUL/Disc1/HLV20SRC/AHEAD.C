/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          AHEAD PageRoutine             */
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

void ahead(void)  /* for AHEAD */
{
        asm push dx
        asm push ax
        asm mov al,dl
	asm shl al,1
	asm shl al,1
	asm shl al,1
	asm shl al,1
	asm or  al,dl
        asm mov ah,al
        asm mov al,0dh
        asm mov dx,3ceh
        asm out dx,ax
        asm pop ax
        asm pop dx
}
int isahead()
{
        char far *p;
        char *s="AHEAD";
        int  i;

        p=(char far *)MK_FP(0xc000,0x0025);
        for (i=0;i<5;i++)
                if (s[i]!=p[i]) return 0;
        return 1;
}
