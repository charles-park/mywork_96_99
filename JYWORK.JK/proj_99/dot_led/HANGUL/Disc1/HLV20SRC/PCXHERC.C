/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         PCX Hercules Decoding          */
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

extern int ret_herc_offset(int y);
extern void writepcxlineh(unsigned offset,char *temp,int n,unsigned char lmask,unsigned char rmask);
extern int readpcxline(char *s,FILE *fpt,int bytes);
extern void getpcxhdrfpt(PCXHDR *p,FILE *fpt);
extern int  getpcxmodefpt(FILE *fpt);

void hgPCXFileDisplayH(int x,int y,char *fname)
{
	FILE   *fpt;
	PCXHDR *p;
	char   *temp;
	int     i;
	unsigned offset;
	int     n;
        int     bytes;
        unsigned char lmask,rmask;

	fpt=fopen(fname,"rb");
	if (fpt==NULL) return;

        if (getpcxmodefpt(fpt)!=PCX_HERCULES) return;
	p=(PCXHDR *)malloc(sizeof(PCXHDR));
        getpcxhdrfpt(p,fpt);

	temp=(char *)malloc((size_t)p->BytesperLine);

        offset=ret_herc_offset(y)+x/8;

        lmask=0xff;             /* for later update */
        rmask=0;

        bytes=p->BytesperLine;
        fseek(fpt,128,0);               /* pass header */
        for (i=y;i<y+p->y2-p->y1+1;i++) {
                n=readpcxline(temp,fpt,bytes);
                writepcxlineh(offset,temp,n-2,lmask,rmask);
		offset+=0x2000;
		if (offset>=0x8000) offset-=0x7fb0;
                if (i>=hgGety2r()) break;
	}
	fclose(fpt);
        free(temp);
        free(p);
}
