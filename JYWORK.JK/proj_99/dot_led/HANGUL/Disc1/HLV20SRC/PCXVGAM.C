/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          PCX B/W VGA decoding          */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <alloc.h>
#include <stdio.h>
#include "hgpcx.h"

/* this is defined in 'PCXDEC.C' module.etc */

extern void writepcxlinem(unsigned offset,char *temp,int n,char color);
extern int  readpcxline(char *s,FILE *fpt,int bytes);
extern void getpcxhdrfpt(PCXHDR *p,FILE *fpt);
extern int  getpcxmodefpt(FILE *fpt);
extern int  xbyte;      /* screen width */

int PCXMonoColor=15;

void hgPCXFileDisplayM(int x,int y,char *fname)
{
	FILE   *fpt;
	PCXHDR *p;
	char   *temp;
	int     i;
	unsigned offset;
	int     n;
        int     bytes;
        int     xwidth=xbyte;

	fpt=fopen(fname,"rb");
	if (fpt==NULL) return;

        if (getpcxmodefpt(fpt)!=PCX_HERCULES) return;
	p=(PCXHDR *)malloc(sizeof(PCXHDR));
        getpcxhdrfpt(p,fpt);

	temp=(char *)malloc((size_t)p->BytesperLine);

        offset=y*xwidth+x/8;
        bytes=p->BytesperLine;
        fseek(fpt,128,0);               /* pass header */

        for (i=y;i<y+p->y2-p->y1+1;i++) {
                n=readpcxline(temp,fpt,bytes);
                n--;
                writepcxlinem(offset,temp,n,PCXMonoColor);
                offset+=xwidth;
	}
	fclose(fpt);
        free(temp);
        free(p);
}

void hgSetPCXMonoColor(char color)
{
        PCXMonoColor=color;
}
