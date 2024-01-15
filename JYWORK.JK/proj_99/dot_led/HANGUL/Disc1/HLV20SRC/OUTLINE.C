/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           OverLine Convert             */
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

#include <mem.h>
#include <dos.h>

void Outline_Img(char *dest,int bytes)
{
	int     i;
        char    temp1[32];
        char    temp2[32];

        memcpy(temp1,dest,bytes);
        memcpy(temp2,dest,bytes);

	asm push ds

	_ES=FP_SEG(temp2);_DS=FP_SEG(temp1);

	       _CX=bytes;_BX=FP_OFF(temp1);_SI=FP_OFF(temp2);
               asm shr cx,1
outlp1:        asm mov ax,es:[si]
               asm shr ax,1
               asm or ds:[bx],ax
               asm shl ax,1
               asm shl ax,1
               asm or ds:[bx],ax
               asm inc bx
	       asm inc bx
               asm inc si
               asm inc si
               asm loop outlp1

	if (bytes == 16) {
                _CX=15; _BX=FP_OFF(temp1);_SI=FP_OFF(temp2);
        outlp2: asm mov ah,es:[si]
                asm or ds:[bx+1],ah
                asm mov ah,es:[si+1]
                asm or ds:[bx],ah
                asm mov ah,es:[si]
                asm xor ds:[bx],ah
                asm inc bx
                asm inc si
                asm loop outlp2
        }
        else {
                _CX=15; _BX=FP_OFF(temp1);_SI=FP_OFF(temp2);
        outlp4: asm mov ax,es:[si]
		asm or ds:[bx+2],ax
                asm mov ax,es:[si+2]
                asm or ds:[bx],ax
                asm mov ax,es:[si]
                asm xor ds:[bx],ax
                asm inc bx
                asm inc bx
                asm inc si
                asm inc si
               asm loop outlp4
        }
        memcpy(dest,temp1,bytes);

	asm pop ds
}
