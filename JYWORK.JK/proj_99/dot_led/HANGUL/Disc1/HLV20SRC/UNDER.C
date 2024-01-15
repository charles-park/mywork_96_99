/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Under Convert               */
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

void Under_Img(char *dest,int bytes)
{
        _SI=bytes;
        _BX=FP_OFF(dest);
	_ES=FP_SEG(dest);

        asm mov BYTE PTR es:[bx+si-1],0xff;
        asm cmp si,32
        asm jne undernext
        asm mov BYTE PTR es:[bx+si-2],0xff;
 undernext:
        return;
}

