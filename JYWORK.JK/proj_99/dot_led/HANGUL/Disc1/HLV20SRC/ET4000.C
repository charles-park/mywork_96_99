/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          ET4000 PageRoutine            */
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

void et4000(void)  /* for ET4000 CHIPSET.. OPTIMA MEGA */
{
        asm push dx
        asm push ax
        asm mov al,dl
	asm shl al,1
	asm shl al,1
	asm shl al,1
	asm shl al,1
	asm or  al,dl
	asm mov dx,3cdh
	asm out dx,al
        asm pop ax
        asm pop dx
}
int iset4000()
{
        asm mov     dx,3D4h
        asm mov     al,33h
        asm out     dx,al
        asm inc     dx
        asm in      al,dx
        asm mov     cl,al
        asm dec     dx
        asm mov     ax,0A33h
        asm out     dx,ax       
        asm mov     al,33h      
        asm out     dx,al       
        asm inc     dx
        asm in      al,dx       
        asm and     al,0Fh
        asm cmp     al,0Ah
        asm jne     next
        asm dec     dx
        asm mov     ax,533h
        asm out     dx,ax       
        asm mov     al,33h      
        asm out     dx,al       
        asm inc     dx
        asm in      al,dx       
        asm and     al,0Fh
        asm cmp     al,5
        asm jne     next
        asm dec     dx
        asm mov     al,33h      
        asm mov     ah,cl
        asm out     dx,ax
        return 1;

next:   asm mov     ah,cl
        asm out     dx,ax
        return 0;
}
