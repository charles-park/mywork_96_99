/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           256 color output             */
/*               MxN Module               */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#pragma inline

extern int  xbyte;
extern void setpage();

void vputch256mn_out(int x,int y,char *buff,int m,int n,char color)
{
        int xw=xbyte;

        asm push ds
	asm mov ax,y
        asm mov bx,xw
	asm mul bx
	asm add ax,x
        asm adc dl,0             /* add carry            */
        asm mov bx,ax            /* bx is offset         */
        asm call far ptr setpage
pass:   asm lds si,buff          /* ds:[si] is font data */
	asm mov ax,0a000h
	asm mov es,ax
	asm mov al,color
        asm mov cx,n             /* vertical loop */
mn:     asm push cx
	asm push bx
        asm mov di,bx            /* use di instead of bx */
        asm mov cx,m
lp0:    asm push cx
        asm mov bl,ds:[si]
        asm mov cx,8
lp1:    asm shl bl,1
	asm jnc next
        asm mov es:[di],al
next:   asm inc di
        asm cmp di,0
	asm jnz next2
	asm inc dl
        asm call far ptr setpage
next2:  asm loop lp1
        asm pop cx
        asm inc si
        asm loop lp0
	asm pop bx
        asm add bx,xw
        asm cmp bx,di
        asm jae next3
        asm inc dl
        asm call far ptr setpage
next3:  asm pop cx
        asm loop mn
	asm pop ds
}
