/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*       Scan Image for 256 color         */
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

extern int  xbyte;
extern void setpage();

void hgVGetImageScan(int x1,int y1,int x2,int y2,char *buff,char scan)
{
        int max=hgGetMaxPages();
        int ywidth=(y2-y1+max-scan)/max;        /* for max scan */
        int xwidth=x2-x1+1;
        int xw=xbyte;

        asm push ds

        asm mov ax,y1
        asm add ax,scan
        asm mov bx,xw
        asm mul bx
        asm add ax,x1
        asm adc dl,0
        asm mov bx,ax
        asm call far ptr setpage
        asm les di,buff
        asm mov ax,0a000h
        asm mov ds,ax

        asm xor ax,ax
        asm mov cx,max
lp1:    asm add ax,xw
        asm loop lp1

lp2:    asm mov cx,ywidth
lp0:    asm push cx
        asm mov si,bx
        asm cld
        asm add si,xwidth
        asm jc  proc2
        asm sub si,xwidth
        asm mov cx,xwidth
        asm REP movsb
        asm jmp next1

proc2:  asm push si
        asm sub si,xwidth
        asm mov cx,si
        asm neg cx
        asm REP movsb
        asm inc dl
        asm call far ptr setpage
        asm pop si
        asm mov cx,si
        asm xor si,si
        asm REP movsb

next1:  asm add bx,ax
        asm cmp bx,si
        asm jae next2
        asm inc dl
        asm call far ptr setpage
next2:  asm pop cx
        asm loop lp0

        asm pop ds
}

void hgVPutImageScan(int x1,int y1,int x2,int y2,char *buff,char scan)
{
        int max=hgGetMaxPages();
        int ywidth=(y2-y1+max-scan)/max;
        int xwidth=x2-x1+1;
        int xw=xbyte;

        asm push ds

        asm mov ax,y1
        asm add ax,scan
        asm mov bx,xw
        asm mul bx
        asm add ax,x1
        asm adc dl,0
        asm mov bx,ax
        asm call far ptr setpage
        asm lds si,buff
        asm mov ax,0a000h
        asm mov es,ax

        asm xor ax,ax
        asm mov cx,max
lp1:    asm add ax,xw
        asm loop lp1

lp2:    asm mov cx,ywidth
lp0:    asm push cx
        asm mov cx,xwidth
        asm mov di,bx
        asm cld

        asm add di,xwidth
        asm jc  proc2

        asm sub di,xwidth
        asm mov cx,xwidth
        asm REP movsb
        asm jmp next1

proc2:  asm push di
        asm sub di,xwidth
        asm mov cx,di
        asm neg cx
        asm REP movsb
        asm inc dl
        asm call far ptr setpage
        asm pop di
        asm mov cx,di
        asm xor di,di
        asm REP movsb

next1:  asm add bx,ax
        asm cmp bx,di
        asm jae next2
        asm inc dl
        asm call far ptr setpage
next2:  asm pop cx
        asm loop lp0

        asm pop ds
}
