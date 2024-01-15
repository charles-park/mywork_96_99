/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           256 color output             */
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

void vputch256e(int x,int y,char *buff,char color)
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
        asm mov cx,16
emain:  asm push cx
	asm push bx
        asm mov di,bx           /* use di instead of bx */
        asm mov bl,ds:[si]
	asm mov cx,8
eloop:  asm shl bl,1
	asm jnc next
        asm mov es:[di],al
next:   asm inc di
        asm cmp di,0
	asm jnz next2
	asm inc dl
        asm call far ptr setpage
next2: 	asm loop eloop
	asm inc si
        asm pop bx
        asm add bx,xw
        asm cmp bx,di
        asm jae next3
        asm inc dl
        asm call far ptr setpage
next3:  asm pop cx
	asm loop emain
	asm pop ds
}
void vputch256h(int x,int y,char *buff,char color)
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
	asm mov cx,16
hmain:  asm push cx
	asm push bx
	asm mov di,bx           /* use di instead of bx */
        asm mov bh,ds:[si]
        asm mov bl,ds:[si+1]
	asm mov cx,16
hloop:  asm shl bx,1
	asm jnc next
        asm mov es:[di],al
next:   asm inc di
        asm cmp di,0
	asm jnz next2
	asm inc dl
        asm call far ptr setpage
next2:	asm loop hloop
	asm inc si
	asm inc si
	asm pop bx
        asm add bx,xw
        asm cmp bx,di
        asm jae next3
        asm inc dl
        asm call far ptr setpage
next3:  asm pop cx
	asm loop hmain
	asm pop ds
}
