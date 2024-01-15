/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Reverse Convert              */
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

void Reverse_Img(char *dest,int bytes)
{
        _CX=bytes;_BX=FP_OFF(dest);_ES=FP_SEG(dest);

          asm shr cx,1
 revloop: asm not WORD PTR es:[bx]
          asm inc bx
          asm inc bx
          asm loop revloop
}

