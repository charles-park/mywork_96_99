/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        PCX Hercules Subroutine         */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

void writepcxlineh(unsigned off,char *buff,int n,unsigned char lmask,unsigned char rmask)
{
        asm push ds
        asm mov ax,0b000h
        asm mov es,ax
        asm lds si,buff

        asm mov di,off
        asm mov ah,ds:[si]
        asm and ah,lmask
        asm mov es:[di],ah
        asm mov cx,n
        asm cld
        asm REP movsb
        asm mov ah,ds:[si]
        asm and ah,rmask
        asm or  es:[di],ah
        asm pop ds
}
