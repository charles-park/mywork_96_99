/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        Scroll Up in 256 color!         */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <alloc.h>

#pragma inline

extern int  xbyte;
extern void setpage();

void getline256(int xwidth,char *buff)
{
        asm push dx
        asm push ds

        asm call far ptr setpage
pass:   asm mov ax,0a000h
	asm mov ds,ax
	asm les di,buff

	asm add si,xwidth
	asm jnc pro1

	asm push si
	asm sub si,xwidth
	asm mov cx,si
	asm neg cx
	asm cld
	asm REP movsb
	asm inc dl
        asm call far ptr setpage
	asm pop si
	asm mov cx,si
	asm xor si,si
	asm REP movsb

        asm pop ds
        asm pop dx

	return;

pro1:   asm sub si,xwidth
	asm mov cx,xwidth
	asm cld
	asm REP movsb

        asm pop ds
        asm pop dx
}

void putline256(int xwidth,char *buff)
{
        asm push dx
        asm push ds

        asm call far ptr setpage
pass:   asm mov ax,0a000h
	asm mov es,ax
	asm lds si,buff

	asm add di,xwidth
	asm jnc pro1

	asm push di
	asm sub di,xwidth
	asm mov cx,di
	asm neg cx
	asm cld
	asm REP movsb
	asm inc dl
        asm call far ptr setpage
	asm pop di
	asm mov cx,di
	asm xor di,di
	asm REP movsb

        asm pop ds
        asm pop dx

	return;

pro1:   asm sub di,xwidth
	asm mov cx,xwidth
	asm cld
	asm REP movsb

        asm pop ds
        asm pop dx
}

void hgVScrUp256(int x1,int y1,int x2,int y2,int lines,char bkcolor)
{
	int  i;
	int  xwidth=x2-x1+1;
	int  count=y2-y1-lines+1;
	char *buff;
        int  xw=xbyte;

        buff=(char *)malloc((size_t)(x2-x1+1));

        asm mov ax,y1
        asm mov bx,xw
        asm mul bx
        asm add ax,x1
        asm adc dl,0
        asm mov di,ax
        asm mov si,ax
        asm mov dh,dl

        asm mov cx,lines
lp0:    asm add si,xw
        asm jnc next
        asm inc dh           /* si page increment */
next:   asm loop lp0

        asm mov cx,count
lp1:    asm push cx
        asm xchg dh,dl

        getline256(xwidth,buff);

	asm xchg dh,dl

	putline256(xwidth,buff);

        asm add  si,xw
        asm jnc  next2
        asm inc  dh
next2:  asm add  di,xw
        asm jnc  next3
        asm inc  dl
next3:  asm pop  cx
        asm loop lp1

        free(buff);
        hgVBoxFill256(x1,y2-lines+1,x2,y2,bkcolor);
}
void hgVScrDown256(int x1,int y1,int x2,int y2,int lines,char bkcolor)
{
        int  i;
        int  xwidth=x2-x1+1;
        int  count=y2-y1-lines+1;
        char *buff;
        int  xw=xbyte;

        buff=(char *)malloc((size_t)(x2-x1+1));

        asm mov ax,y2
        asm mov bx,xw
        asm mul bx
        asm add ax,x1
        asm adc dl,0
        asm mov di,ax
        asm mov si,ax
        asm mov dh,dl

        asm mov cx,lines
lp0:    asm sub si,xw
        asm jnc next
        asm dec dh           /* si page increment */
next:   asm loop lp0

        asm mov cx,count
lp1:    asm push cx
        asm xchg dh,dl

        getline256(xwidth,buff);

        asm xchg dh,dl

        putline256(xwidth,buff);

        asm sub  si,xw
        asm jnc  next2
        asm dec  dh
next2:  asm sub  di,xw
        asm jnc  next3
        asm dec  dl
next3:  asm pop  cx
        asm loop lp1

        free(buff);
        hgVBoxFill256(x1,y1,x2,y1+lines-1,bkcolor);
}
