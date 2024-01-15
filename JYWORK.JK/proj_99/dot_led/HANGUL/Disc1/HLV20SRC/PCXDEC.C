/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         PCX Decoding routine           */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

/* this routine is in Bit-Mapped Graphics,Steve Rimmer */
/* thank you for this good routine.                    */

#include <stdio.h>

int readpcxline(char *s,FILE *fpt,int bytes)
{
	int c,i;
	int n=0;

	do {
		c=(fgetc(fpt) & 0xff);
		if ((c&0xc0)==0xc0) {
			i=c&0x3f;
			c=fgetc(fpt);
                        while (i--) s[n++]=c;
		}
		else s[n++]=c;
	} while (n<bytes);
	return n;
}
