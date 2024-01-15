/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            ThreeD convert              */
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
#include <mem.h>

void ThreeD_Img(char *dest,int bytes)
{
	int     i;
	char    temp1[32];
	char    temp2[32];

        memcpy(temp1,dest,bytes);
        memcpy(temp2,dest,bytes);

	asm push ds

	_DS=FP_SEG(temp1);_ES=FP_SEG(temp2);

	if (bytes == 16) {
		_CX=15; _BX=FP_OFF(temp1);_SI=FP_OFF(temp2);
	thlp1:  asm mov ah,es:[si]
		asm or ds:[bx+1],ah
                asm shr ah,1
		asm or ds:[bx],ah
		asm shl ah,1
		asm xor ds:[bx],ah
                asm inc bx
                asm inc si
                asm loop thlp1
        }
	else {
		_CX=16; _BX=FP_OFF(temp1);_SI=FP_OFF(temp2);
	thlp4:  asm mov ax,es:[si]
		asm or ds:[bx+2],ax
		asm shr ax,1
		asm or ds:[bx],ax
		asm shl ax,1
		asm xor ds:[bx],ax
		asm inc bx
		asm inc bx
		asm inc si
                asm inc si
                asm loop thlp4
        }
        memcpy(dest,temp1,bytes);

	asm pop ds
}
