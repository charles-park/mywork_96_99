/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*       VGA Wonder PageRoutine           */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

/* dl is page number */

void wonder1(void)
{
             asm push dx
             asm push ax
             asm mov ah,dl
             asm mov dx,1ceh
             asm mov al,0b2h
             asm out dx,al
             asm inc dx
             asm and ah,07h
             asm shl ah,1
             asm in al,dx
             asm dec dx
             asm and al,0e1h
             asm or ah,al
             asm mov al,0b2h
             asm out dx,ax
             asm pop ax
             asm pop dx
}
int iswonder()
{
        char far *p=(char far *)0xc0000000;
        char *signature=" 761295520";
        int  i;

        for (i=0;i<10;i++)
                if (p[i+0x30]!=signature[i]) return 0;

        return 1;
}
void wonder_special()
{
        asm mov dx,01ceh
        asm mov al,0beh
        asm out dx,al
        asm inc dx
        asm in  al,dx
        asm and al,0f7h
        asm mov ah,al
        asm dec dx
        asm mov al,0beh
        asm out dx,ax
}
