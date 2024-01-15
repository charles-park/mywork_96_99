/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*       ClearScreen in 256 color         */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <dos.h>

extern int  hgBack;
extern void setpage();

void hgClearScreen256()
{
        int xmax=hgGetx2r()+1;
        int ymax=hgGety2r()+1;

        asm mov ax,0a000h
        asm mov es,ax
        asm xor di,di

        asm mov ax,xmax
        asm mov bx,ymax
        asm mul bx                      /* xmax * ymax */
        asm mov bx,ax                   /* save ax     */
        asm mov al,BYTE PTR hgBack      /* hgBack is background */
        asm mov ah,al
        asm cld
        asm mov cx,dx
        asm mov dl,0
        asm call far ptr setpage
lp1:    asm push cx
        asm mov cx,32768
        asm REP stosw
        asm xor di,di
        asm pop cx
        asm inc dl
        asm call far ptr setpage
        asm loop lp1
next:   asm mov cx,bx
        asm REP stosb
}
