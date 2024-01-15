/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           File Find Module             */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <dos.h>

int hgFindFirstDir(char *mask,char attr,char *s)
{
	char far *p;
	int  i=0;

	asm push ds
	asm lds  si,mask
	asm mov  dx,si
	asm mov  ah,04eh
        asm mov  cl,attr
	asm xor  ch,ch
        asm int  21h
        asm pop  ds
        asm jc   error

	p=getdta();
	p+=30;

	while (*p) { s[i++]=*p++; }

	s[i]=0;
	return 1;
error:	return 0;
}

int hgFindNextDir(char attrib,char *s)
{
	char far *p;
	int  i=0;

        do {
                asm mov  ah,04fh        /* find next     */
		asm int  21h
		asm jc   error

		p=getdta();
        } while (!(p[21]&attrib));      /* for sure */

        p+=30;                          /* for find name */

	while (*p) { s[i++]=*p++; }

	s[i]=0;
	return 1;
error:	return 0;
}
