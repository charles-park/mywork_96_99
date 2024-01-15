/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             "Make Bold"                */
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

void Bold_Img(char *dest, int bytes)
{
        asm mov si,bytes
        asm les bx,dest
bldlp1: asm mov ax,es:[bx+si-2]
        asm shr ax,1
        asm or es:[bx+si-2],ax
        asm dec si
        asm dec si
        asm jne bldlp1
}
