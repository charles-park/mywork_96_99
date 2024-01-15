/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Hercules Image               */
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

void hercgetimage(int x1,int y1,int x2,int y2,char *buff)
{
		int y_width=y2-y1+1;
		int x_width=x2-x1+1;
                unsigned off=ret_herc_offset(y1);

                asm push ds

                asm mov bx,off
                asm add bx,x1
                asm les di,buff
		asm mov ax,0b000h
		asm mov ds,ax
                asm mov cx,y_width

get_loop1:      asm push cx
		asm mov si,bx
                asm mov cx,x_width
		asm cld
		asm REP movsb
		hercadd();
		asm pop cx
		asm loop get_loop1

		asm pop ds
}

void hercputimage(int x1,int y1,int x2,int y2,char *buff,unsigned char mask)
{
		int x_width=x2-x1+1;
		int y_width=y2-y1+1;
		unsigned char tmask=mask;
                unsigned off=ret_herc_offset(y1);

		asm push ds

                asm mov bx,off
                asm add bx,x1
                asm lds si,buff
		asm mov ax,0b000h
		asm mov es,ax
                asm mov cx,y_width
		asm mov ah,BYTE PTR tmask

put_loop1:    	asm push cx
                asm mov cx,x_width
		asm mov di,bx
put_loop2:	asm mov al,ds:[si]
		asm not ah
		asm and es:[di],ah
		asm not ah
		asm and al,ah
		asm or  es:[di],al
		asm inc si
		asm inc di
		asm dec cx
		asm jnz put_loop2
		hercadd();
		asm pop cx
		asm loop put_loop1

		asm pop ds
}

void hgHGetImage(int x1,int y1,int x2,int y2,char *buff)
{
	int xs=x1/8+1;
	int xe=x2/8-1;
	unsigned offset;

        buff[0]=get_lmask(x1);
        buff[1]=get_rmask(x2);

        if (xs<=xe) {
                hercgetimage(xs,y1,xe,y2,&buff[2]);
                offset=(y2-y1+1)*(xe-xs+1)+2;
        } else offset=2;

	hercgetimage(xs-1,y1,xs-1,y2,&buff[offset]);
	offset+=(y2-y1+1);
	hercgetimage(xe+1,y1,xe+1,y2,&buff[offset]);
}

void hgHPutImage(int x1,int y1,int x2,int y2,char *buff)
{
	int xs=x1/8+1;
	int xe=x2/8-1;
	unsigned offset;
	unsigned char lmask,rmask;

	lmask=buff[0];
	rmask=buff[1];

        if (xs<=xe) {
                hercputimage(xs,y1,xe,y2,&buff[2],0xff);
                offset=(y2-y1+1)*(xe-xs+1)+2;
        } else offset=2;

	hercputimage(xs-1,y1,xs-1,y2,&buff[offset],lmask);
	offset+=(y2-y1+1);
	hercputimage(xe+1,y1,xe+1,y2,&buff[offset],rmask);
}
