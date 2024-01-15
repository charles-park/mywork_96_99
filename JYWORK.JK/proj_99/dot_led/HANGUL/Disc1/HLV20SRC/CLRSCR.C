/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Clear Screen                */
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

#include "hghlib.h"
#include "hginit.h"

extern int hgx2r;
extern int hgy2r;

void herc_clear_screen(unsigned char mask);
void vga_clear_screen(char color);

void hgVClearScreen()
{
        int back=hgGetBack();

        hgHideMouse();

        vga_clear_screen(back);

        hgShowMouse();
}
void vga_clear_screen(char color)
{
        asm mov dx,03ceh
        asm mov ax,0205h
        asm out dx,ax
        asm mov ax,0ff08h
        asm out dx,ax

        asm mov ax,0a000h
        asm mov es,ax
        asm xor di,di           /* di is 0 */

        asm mov ax,hgx2r
        asm inc ax              /* ax is hgx2r+1 */
        asm shr ax,1
        asm shr ax,1
        asm shr ax,1            /* (hgx2r+1)/8 */
        asm mov bx,hgy2r
        asm inc bx              /* bx is hgy2r+1 */
        asm mul bx              /* ((hgx2r+1)/8)*(hgy2r+1) */
        asm cld
        asm mov cx,ax
        asm mov al,BYTE PTR color
        asm mov ah,al
        asm shr cx,1
        asm REP stosw
        asm adc cx,0
        asm REP stosb

        asm mov ax,0005h
        asm out dx,ax
}
void hgHClearScreen()
{
        int back=hgGetBack();
        unsigned char mask=0;

        if (back!=ZERO) mask=0xff;

        hgHideMouse();
        herc_clear_screen(mask);
        hgShowMouse();
}
void herc_clear_screen(unsigned char mask)
{
        asm mov ax,0b000h
        asm mov es,ax
        asm mov di,0
        asm mov cx,16384
        asm mov ah,BYTE PTR mask
        asm mov al,ah
        asm cld
        asm REP stosw
}
