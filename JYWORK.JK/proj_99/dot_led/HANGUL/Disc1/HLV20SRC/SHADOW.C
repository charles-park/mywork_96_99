/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Shadow Convert              */
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

void Shadow_Img(char *dest, int bytes)
{
        _BX=FP_OFF(dest);_SI=bytes;
        _ES=FP_SEG(dest);

	asm cmp si,32
	asm jz shan1
shlop1: asm or WORD PTR es:[bx+si-2],0aa55h
	asm dec si
	asm dec si
	asm jne shlop1
        return;

shan1 : asm or WORD PTR es:[bx+si-2],0aaaah
        asm dec si
        asm dec si
        asm or WORD PTR es:[bx+si-2],05555h
        asm dec si
        asm dec si
	asm jne shan1
        return;
}
