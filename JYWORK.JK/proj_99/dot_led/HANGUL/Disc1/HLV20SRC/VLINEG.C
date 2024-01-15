/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Fast VGA Line               */
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

extern int xbyte;

void hgVLineG(int x1,int y1,int x2,int y2,char color)
{
        int vdx,vdy,vix,viy,vinc;
        int xw=xbyte;

	asm mov ax,0a000h       /* setment */
	asm mov es,ax
	asm mov dx,03ceh
        asm mov ax,0005h        /* write mode 2 */
	asm out dx,ax
        asm mov al,0
        asm mov ah,color
        asm out dx,ax           /* color */
        asm mov ax,0f01h
        asm out dx,ax           /* Set/Reset Enable = 0xf */

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

        asm mov ax,vix                  /* vinc is MAX of vix & viy */
	asm cmp ax,viy
        asm jc  n3
        asm mov vinc,ax
        asm jmp next2
n3:     asm mov ax,viy
        asm mov vinc,ax

next2:	asm mov ax,WORD PTR y1          /* calculate bx */
        asm mov bx,xw
	asm mul bx
	asm mov bx,ax
	asm mov dx,WORD PTR x1
	asm mov ax,dx
	asm shr dx,1
	asm shr dx,1
	asm shr dx,1
	asm add bl,dl
	asm adc bh,0
	asm and al,07h
	asm mov cl,al
	asm mov ah,080h
	asm shr ah,cl                   /* ah is bitmask */

	asm mov dx,03ceh
	asm mov al,8			/* first plot */
	asm out dx,ax
        asm mov al,es:[bx]              /* latch load */
        asm mov es:[bx],al              /* plot!      */

	asm mov  si,0                   /* si is X       */
	asm mov  di,0                   /* di is Y       */
        asm mov  cx,vinc                /* loop counter  */
        asm inc  cx

loops:  asm mov  al,0                   /* al is plot    */
	asm add  si,vix                 /* x+=ix;        */
	asm add  di,viy                 /* y+=iy;        */
        asm cmp  si,vinc                /* si > inc ?    */
	asm jle  yproc
	asm mov  al,1                   /* plot is 1     */
        asm sub  si,vinc
        asm cmp  WORD PTR vdx,0
	asm jle  next3
	asm shr  ah,1                   /* plotx++       */
	asm jnz  yproc
	asm mov  ah,080h                /* mask reset    */
        asm inc  bx                     /* incre offset  */
	asm jmp  yproc
next3:  asm shl  ah,1                   /* plotx--       */
	asm jnz  yproc
	asm mov  ah,01h
        asm dec  bx                     /* decre offset  */

yproc:  asm cmp  di,vinc
	asm jle  lpend
	asm mov  al,1                   /* plot is 1     */
        asm sub  di,vinc
	asm cmp  WORD PTR vdy,0
	asm jle  next4
        asm add  bx,xw                  /* ploty++       */
	asm jmp  lpend
next4:  asm sub  bx,xw

lpend:  asm cmp  al,1
	asm jnz  end

	asm mov  al,8			/* plot */
	asm out  dx,ax
        asm mov  al,es:[bx]             /* latch load */
        asm mov  es:[bx],al             /* plot !     */

end  :  asm loop loops

        asm mov  ax,0001h
        asm out  dx,ax
        asm mov  ax,0ff08h
        asm out  dx,ax
}
