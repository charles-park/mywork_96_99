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

#define ZERO 0

void hgHLineG(int x1,int y1,int x2,int y2,char color)
{
        int vdx,vdy,vix,viy,vinc;
        unsigned off=ret_herc_offset(y1);

        asm mov  ax,0b000h       /* segment */
        asm mov  es,ax

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

next2:  asm mov bx,off
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

        asm cmp BYTE PTR color,ZERO
        asm jz n4
        asm or es:[bx],ah
        asm jmp n5
n4:     asm not ah
        asm and es:[bx],ah
        asm not ah

n5:     asm mov  si,0                   /* si is X       */
	asm mov  di,0                   /* di is Y       */
        asm mov  cx,vinc                 /* loop counter  */
        asm inc  cx

loops:  asm mov  al,0                   /* al is plot    */
	asm add  si,vix                 /* x+=ix;        */
	asm add  di,viy                 /* y+=iy;        */
        asm cmp  si,vinc                 /* si > inc ?    */
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

        asm add  bx,02000h              /* ploty++       */
        asm cmp  bx,08000h
        asm jb   noadd
        asm sub  bx,07fb0h

noadd:  asm jmp  lpend

next4:  asm sub  bx,02000h
        asm cmp  bx,08000h
        asm jb   lpend
        asm add  bx,07fb0h

lpend:  asm cmp  al,1
	asm jnz  end

        asm cmp BYTE PTR color,ZERO
        asm jz n6
        asm or es:[bx],ah
        asm jmp end
n6:     asm not ah
        asm and es:[bx],ah
        asm not ah

end  :  asm loop loops
}
