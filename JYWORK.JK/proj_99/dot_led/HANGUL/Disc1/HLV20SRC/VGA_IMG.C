/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           VGA-image routine            */
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

#include<dos.h>
#include "hghlib.h"

extern int xbyte;

void vgagetimage(int x1,int y1,int x2,int y2,char *buff)
{
        int y_width=y2-y1+1;
        int x_width=x2-x1+1;
        int xw=xbyte;

        asm push ds

        asm mov ax,xw
        asm mov bx,y1
        asm mul bx
        asm mov bx,ax
        asm les di,buff
        asm mov ax,0a000h
        asm mov ds,ax
        asm mov cx,y_width
        asm mov dx,3ceh
        asm add bx,x1

g1:     asm push cx
        asm mov ah,3
g2:     asm mov al,4
        asm out dx,ax
        asm mov si,bx
        asm mov cx,x_width
        asm cld
        asm REP movsb
        asm dec ah
        asm jns g2
        asm add bx,xw
        asm pop cx
        asm loop g1

        asm mov al,4
        asm mov ah,0fh
        asm out dx,ax

        asm pop ds
}

void vgaputimage(int x1,int y1,int x2,int y2,char *buff)
{
        int x_width=x2-x1+1;
        int y_width=y2-y1+1;
        int xw=xbyte;

        asm push ds

        asm mov dx,03ceh
        asm mov ax,5
        asm out dx,ax

        asm mov ax,xw
        asm mov bx,y1
        asm mul bx
        asm mov bx,ax
        asm lds si,buff
        asm mov ax,0a000h
        asm mov es,ax
        asm mov dx,3c4h
        asm add bx,x1

        asm mov cx,y_width
p1:     asm mov ah,8
        asm push cx
p2:     asm mov al,2
        asm out dx,ax
        asm mov cx,x_width;
        asm mov di,bx
        asm cld
p3:     asm mov al,es:[di]
        asm movsb
        asm loop p3
        asm shr ah,1
        asm jnc p2
        asm add bx,xw
        asm pop cx
        asm loop p1

        asm mov ax,0f02h
        asm out dx,ax

        asm pop ds
}

void hgVGetImage(int x1,int y1,int x2,int y2,char *buff)
{
	int xs=x1/8+1;
	int xe=x2/8-1;
        unsigned offset;

        buff[0]=get_lmask(x1);
        buff[1]=get_rmask(x2);

        if (xs<=xe) {
                vgagetimage(xs,y1,xe,y2,&buff[2]);
                offset=(y2-y1+1)*(xe-xs+1)*4+2;
        } else offset=2;

        vgagetimage(xs-1,y1,xs-1,y2,&buff[offset]);
	offset+=(y2-y1+1)*4;
        vgagetimage(xe+1,y1,xe+1,y2,&buff[offset]);
}

void hgVPutImage(int x1,int y1,int x2,int y2,char *buff)
{
	int xs=x1/8+1;
	int xe=x2/8-1;
        unsigned offset;
	unsigned char lmask,rmask;

	lmask=buff[0];
	rmask=buff[1];

        if (xs<=xe) {
                vgaputimage(xs,y1,xe,y2,&buff[2]);
                offset=(y2-y1+1)*(xe-xs+1)*4+2;
        } else offset=2;

	outportb(0x3ce,8);
	outportb(0x3cf,lmask);

        vgaputimage(xs-1,y1,xs-1,y2,&buff[offset]);

	offset+=(y2-y1+1)*4;

	outportb(0x3ce,8);
	outportb(0x3cf,rmask);

        vgaputimage(xe+1,y1,xe+1,y2,&buff[offset]);

	outport(0x3ce,0x0ff08);
}

