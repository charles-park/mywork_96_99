/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         Horiz/Vert Line in 256         */
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

void hgVhline256(int x1,int x2,int y,char color)
{
        int xwidth=x2-x1+1;
        int xw=xbyte;

        asm mov ax,y
        asm mov bx,xw
        asm mul bx
        asm add ax,x1
        asm adc dl,0             /* add with carry */
        asm mov di,ax            /* di is offset   */
        asm call far ptr setpage

pass:   asm mov ax,0a000h
        asm mov es,ax
        asm mov al,color        /* fill color     */

        asm add di,xwidth
        asm jc  proc2

        asm sub di,xwidth
        asm mov cx,xwidth
        asm cld
        asm REP stosb

        return;

proc2:  asm cld
        asm push di
        asm sub di,xwidth
        asm mov cx,di
        asm neg cx
        asm REP stosb
        asm inc dl
        asm call far ptr setpage
        asm pop di
        asm mov cx,di
        asm xor di,di
        asm REP stosb
}
void hgVvline256(int x,int y1,int y2,char color)
{
        int ywidth=y2-y1+1;
        int xw=xbyte;

        asm mov ax,y1
        asm mov bx,xw
        asm mul bx
        asm add ax,x
        asm adc dl,0             /* add with carry */
        asm mov bx,ax            /* di is offset   */
        asm call far ptr setpage

pass:   asm mov ax,0a000h
        asm mov es,ax
        asm mov al,color        /* fill color     */

        asm mov cx,ywidth

lp1:    asm mov di,bx
        asm stosb
        asm add bx,xw
        asm jnc next2
        asm inc dl
        asm call far ptr setpage
next2:  asm loop lp1
}
