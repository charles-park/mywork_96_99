/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          Hercules subroutine           */
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

void hercadd()
{
        asm add bx,02000h
	asm cmp bx,08000h
	asm jb  noadd
        asm sub bx,07fb0h
noadd:  return;
}
void hercsub()
{
        asm sub bx,02000h
	asm cmp bx,08000h
	asm jb  noadd
        asm add bx,07fb0h
noadd:  return;
}
void hercadd_dx()
{
        asm add dx,02000h
	asm cmp dx,08000h
	asm jb  noadd
        asm sub dx,07fb0h
noadd:  return;
}
void hercsub_dx()
{
        asm sub dx,02000h
	asm cmp dx,08000h
	asm jb  noadd
        asm add dx,07fb0h
noadd:  return;
}

void ypat(int xpat)
{
	asm rol dl,1
	asm jc noad3
	asm mov al,0
	return;
noad3:	asm mov al,BYTE PTR xpat
	return;
}
