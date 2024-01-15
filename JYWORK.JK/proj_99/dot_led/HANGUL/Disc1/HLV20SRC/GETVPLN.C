/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Plane Reading              */
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

void vgagetbyteplane(int x1,int y1,int x2,int y2,int pnum,unsigned char *buff)
{
        int y_width;
        int x_width;

        asm push ds

        asm mov ax,y2
        asm sub ax,y1
        asm inc ax
        asm mov y_width,ax
        asm mov ax,x2
        asm sub ax,x1
        asm inc ax
        asm mov x_width,ax
        asm mov ax,80
        asm mov bx,y1
        asm mul bx
        asm mov bx,ax
        asm add bx,x1
        asm les di,buff
        asm mov ax,0a000h
        asm mov ds,ax
        asm mov cx,y_width

        asm mov dx,3ceh

        asm mov ah,BYTE PTR pnum
	asm mov al,4
	asm out dx,ax

g1:     asm push cx
        asm mov si,bx
        asm mov cx,x_width
        asm cld
        asm REP movsb
        asm add bx,80
        asm pop cx
        asm loop g1

        asm mov ax,0f04h
        asm out dx,ax

        asm pop ds
}
