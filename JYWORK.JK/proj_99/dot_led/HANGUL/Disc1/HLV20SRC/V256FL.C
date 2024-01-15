/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          BoxFill in 256 color          */
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

extern int  xbyte;
extern void setpage();

void hgVBoxFill256(int x1,int y1,int x2,int y2,char color)
{
        int xwidth=x2-x1+1;
        int ywidth=y2-y1+1;
        int xw=xbyte;

        asm mov ax,y1
        asm mov bx,xw
        asm mul bx
        asm add ax,x1
        asm adc dl,0             /* add with carry */
        asm mov bx,ax            /* bx is offset   */
        asm call far ptr setpage

pass:   asm mov ax,0a000h
        asm mov es,ax
        asm mov al,color        /* fill color     */

        asm mov cx,ywidth
lp2:    asm push cx
        asm mov di,bx
        asm add di,xwidth
        asm jc  proc2

        asm mov cx,xwidth
        asm sub di,xwidth
        asm cld
        asm REP stosb
        asm jmp next1

proc2:  asm sub di,xwidth
        asm mov cx,di
        asm neg cx
        asm cld
        asm REP stosb
        asm mov di,bx
        asm add di,xwidth
        asm inc dl
        asm call far ptr setpage
        asm mov cx,di
        asm xor di,di
        asm REP stosb

next1:  asm add bx,xw
        asm cmp bx,di
	asm jae next2
	asm inc dl
        asm call far ptr setpage
next2:  asm pop cx
	asm loop lp2
}
