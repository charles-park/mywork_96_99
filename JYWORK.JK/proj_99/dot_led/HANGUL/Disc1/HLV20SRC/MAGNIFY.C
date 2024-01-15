/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           "Magnified char"             */
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
#include <alloc.h>

extern int hgXFAC;
extern int hgYFAC;

void widen_sub(char *s1,char *s2,int bytes)
{
	asm push ds

	_CX=bytes;
	_DS=FP_SEG(s1);
	_SI=FP_OFF(s1);
	_ES=FP_SEG(s2);
	_DI=FP_OFF(s2);

lp0:	asm xor ax,ax
	asm mov dl,ds:[si]
	asm push cx
	asm mov cx,8
lp1:	asm shl ax,1
	asm xor bl,bl
	asm shl dl,1
	asm adc bl,0
	asm add al,bl
	asm shl ax,1
	asm add al,bl
   	asm loop lp1
   	asm mov es:[di],ah
   	asm inc di
   	asm mov es:[di],al
   	asm inc di
   	asm inc si
	asm pop cx
	asm loop lp0   	
   	
   	asm pop ds
}   	
void widen(char *s1,char *s2,int bytes)
{
        char *medi;
        int i,j;
 	int mul;
	unsigned size;

        /* size can't exceed 1024 */

        medi=(char *)malloc((size_t)pw(2,hgXFAC-1)*bytes);
	memcpy(medi,s1,bytes);
        for (i=1;i<hgXFAC;i++){
		mul=bytes*pw(2,i-1);
		widen_sub(medi,s2,mul);
                if (i==(hgXFAC-1)) break;
		memcpy(medi,s2,mul<<1);
	}
        free(medi);
}
void lengthen(char *s1,char *s2,int xbytes,int yscan)
{
        int count=hgYFAC;

	asm push ds

	_DS=FP_SEG(s1);
	_SI=FP_OFF(s1);
	_ES=FP_SEG(s2);
	_DI=FP_OFF(s2);			

	_CX=yscan;

wp0:	asm push cx
	asm mov cx,WORD PTR count
	asm mov bx,si
wp1:	asm push cx
	asm mov cx,WORD PTR xbytes
	asm REP movsb
	asm mov si,bx
	asm pop cx
	asm loop wp1
	asm add si,WORD PTR xbytes
	asm pop cx
	asm loop wp0

	asm pop ds
}

int pw(int a,int b)
{
	int i;

	if (b==0) return 1;
	a<<=(b-1);
	return a;
}

