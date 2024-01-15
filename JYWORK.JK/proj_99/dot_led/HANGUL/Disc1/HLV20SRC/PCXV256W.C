/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        PCX VGA 256 color Subroutine    */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

extern void setpage();

void writepcxline256(unsigned off,char *buff,int n,char page)
{
        asm push ds
        asm mov ax,0a000h
        asm mov es,ax
        asm lds si,buff
        asm mov di,off
        asm add di,n
        asm jc proc2

        asm sub di,n
        asm cld
        asm mov cx,n
        asm REP movsb

        asm pop ds

        return;

proc2:
        asm push di
        asm sub di,n
        asm mov cx,di
        asm neg cx
        asm cld
        asm REP movsb
        asm pop di
        asm mov dl,BYTE PTR page
        asm inc dl
        asm call far ptr setpage
        asm mov cx,di
        asm xor di,di
        asm REP movsb

        asm pop ds
}
