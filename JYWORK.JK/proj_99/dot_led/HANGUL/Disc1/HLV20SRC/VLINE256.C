/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*       Line Routine in 256 color        */
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

void hgVLine256(int x1,int y1,int x2,int y2,char color)
{
        int vdx,vdy,vix,viy,vinc;
        int xw=xbyte;

	asm mov  ax,x2           /* calculate vdx,vdy,idx,idy */
	asm sub  ax,x1
	asm mov  vdx,ax
	asm test ax,0x8000
	asm jz   n1
	asm neg  ax
n1:     asm mov  vix,ax
	asm mov  ax,y2
	asm sub  ax,y1
	asm mov  vdy,ax
	asm test ax,0x8000
	asm jz   n2
	asm neg  ax
n2:     asm mov  viy,ax
        asm mov  ax,vix                  /* vinc is MAX of vix & viy */
        asm cmp  ax,viy
        asm jc   n3
        asm mov  vinc,ax
        asm jmp  next2
n3:     asm mov  ax,viy
        asm mov  vinc,ax
next2:  asm mov  ax,WORD PTR y1          /* calculate bx */
        asm mov  bx,xw
        asm mul  bx
        asm add  ax,WORD PTR x1
        asm mov  bx,ax
        asm adc  dl,0
        asm call far ptr setpage
pass:   asm mov  ax,0a000h
        asm mov  es,ax
        asm mov  es:[bx],ah

        asm mov  si,0                   /* si is X       */
	asm mov  di,0                   /* di is Y       */

        asm mov  cx,vinc                /* loop counter  */
        asm inc  cx

        asm mov  ah,color               /* color load    */
loops:  asm mov  al,0                   /* al is plot    */
	asm add  si,vix                 /* x+=ix;        */
	asm add  di,viy                 /* y+=iy;        */
        asm cmp  si,vinc                /* si > inc ?    */
	asm jle  yproc
	asm mov  al,1                   /* plot is 1     */
        asm sub  si,vinc
        asm cmp  WORD PTR vdx,0
	asm jle  next3
        asm inc  bx                     /* plotx++       */
        asm cmp  bx,0
        asm jnz  yproc
        asm inc  dl
        asm call far ptr setpage
        asm jmp  yproc
next3:  asm dec  bx                     /* plotx--       */
        asm cmp  bx,0ffffh
        asm jnz  yproc
        asm dec  dl
        asm call far ptr setpage
yproc:  asm cmp  di,vinc
	asm jle  lpend
	asm mov  al,1                   /* plot is 1     */
        asm sub  di,vinc
	asm cmp  WORD PTR vdy,0
	asm jle  next4
        asm add  bx,xw                 /* ploty++       */
        asm jnc  lpend
        asm inc  dl
        asm call far ptr setpage
	asm jmp  lpend
next4:  asm sub  bx,xw
        asm jnc  lpend
        asm dec  dl
        asm call far ptr setpage
lpend:  asm cmp  al,1
	asm jnz  end
        asm mov  es:[bx],ah             /* plot !        */
end  :  asm loop loops
}

