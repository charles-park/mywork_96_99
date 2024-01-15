/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        Plot in 256 color mode          */
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
#pragma warn -rvl               /* for remove warning */

extern int  xbyte;
extern void setpage();

void hgVPlotXy256(int x,int y,char color)
{
        int xw=xbyte;

        asm mov ax,y
        asm mov bx,xw
        asm mul bx
        asm add ax,x
        asm adc dl,0             /* add with carry */
        asm mov bx,ax            /* di is offset   */
        asm call far ptr setpage

pass:   asm mov ax,0a000h
        asm mov es,ax
        asm mov al,color         /* fill color     */
        asm mov es:[bx],al
}
char hgVGetPlotXy256(int x,int y)
{
        int xw=xbyte;

        asm mov ax,y
        asm mov bx,xw
        asm mul bx
        asm add ax,x
        asm adc dl,0             /* add with carry */
        asm mov bx,ax            /* di is offset   */
        asm call far ptr setpage

pass:   asm mov ax,0a000h
        asm mov es,ax
        asm mov al,es:[bx]       /* al is return value */
        asm xor ah,ah
}
