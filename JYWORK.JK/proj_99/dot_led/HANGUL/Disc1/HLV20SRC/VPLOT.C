/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*               VGA PLOT                 */
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
#pragma warn -rvl               /* for remove warning */

extern int xbyte;

void hgVPlotXy(int x,int y,char color)
{
        int xw=xbyte;

        asm mov dx,3ceh
	asm mov ax,0205h
	asm out dx,ax
														 	
	asm mov ax,WORD PTR y
        asm mov bx,xw
	asm mul bx
	asm mov bx,ax
	asm mov dx,WORD PTR x
	asm mov ax,dx
	asm shr dx,1
	asm shr dx,1
	asm shr dx,1
	asm add bl,dl
	asm adc bh,0
        asm and al,07h
	asm mov cl,al
	asm mov ah,080h
	asm shr ah,cl

        asm mov dx,03ceh
	asm mov al,8
	asm out dx,ax

	asm mov ax,0a000h
	asm mov es,ax
	asm mov ah,BYTE PTR es:[bx]
	asm mov ah,BYTE PTR color
	asm mov es:[bx],ah

	asm mov ax,0005h
	asm out dx,ax
	asm mov ax,0ff08h
	asm out dx,ax
}
char hgVGetPlotXy(int x,int y)
{
        int xw=xbyte;

        asm mov ax,WORD PTR y
        asm mov bx,xw
	asm mul bx
	asm mov bx,ax
	asm mov dx,WORD PTR x
	asm mov ax,dx
	asm shr dx,1
	asm shr dx,1
	asm shr dx,1
	asm add bl,dl
	asm adc bh,0
        asm and al,07h
	asm mov cl,al
	asm mov ah,080h
	asm shr ah,cl
	asm mov si,bx           /* si is offset  */
	asm mov bh,ah           /* dl is bitmask */
	asm mov bl,0            /* bl is return value */

        asm mov dx,03ceh
        asm mov ax,0ff08h
        asm out dx,ax
        asm mov ax,0a000h
        asm mov es,ax

        asm mov cx,4
lp:     asm shl bl,1
        asm mov al,4
        asm mov ah,cl
        asm dec ah                      /* plane num */
        asm out dx,ax
        asm mov al,BYTE PTR es:[si]     /* reading */
        asm and al,bh                   /* masking */
        asm jz  next
        asm inc bl
next:   asm loop lp

        asm xor ax,ax
        asm mov al,bl                   /* return value */
}
