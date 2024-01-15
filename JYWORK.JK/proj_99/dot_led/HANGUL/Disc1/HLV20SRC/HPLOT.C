/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              HERC PLOT                 */
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
#pragma warn -rvl               /* remove warning */

#define ZERO 0

void hgHPlotXy(int x,int y,char color)
{
        unsigned off=ret_herc_offset(y);

        asm mov ax,0b000h
        asm mov es,ax

        asm mov bx,off
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

        asm mov al,BYTE PTR color
        asm cmp al,ZERO
        asm jz  zerop
        asm or  es:[bx],ah              /* not ZERO */

        return;

zerop:  asm not ah
        asm and es:[bx],ah              /* ZERO     */

        return;
}

char hgHGetPlotXy(int x,int y)
{
        unsigned off=ret_herc_offset(y);

        asm mov ax,0b000h
        asm mov es,ax

        asm mov bx,off
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

        asm mov al,15       /* this is WHITE */
        asm and ah,es:[bx]
        asm jnz next
        asm mov al,0        /* this is ZERO  */
next:   asm mov ah,0
}
