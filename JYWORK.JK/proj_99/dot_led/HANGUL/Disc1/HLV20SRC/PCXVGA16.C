/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        PCX 16 Color VGA decoding       */
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

extern void writepcxlinev(unsigned offset,char *temp,int n,unsigned char lmask,unsigned char rmask);
extern int  readpcxline(char *s,FILE *fpt,int bytes);
extern void getpcxhdrfpt(PCXHDR *p,FILE *fpt);
extern int  getpcxmodefpt(FILE *fpt);
extern int  xbyte;      /* screen width */
extern int  palettepcx;

void hgPCXFileDisplayV(int x,int y,char *fname)
{
	FILE   *fpt;
	PCXHDR *p;
	char   *temp;
	int     i;
	unsigned offset;
	int     n;
	int     bytes;
        int     xwidth=xbyte;
        unsigned char lmask,rmask;

	fpt=fopen(fname,"rb");
	if (fpt==NULL) return;

	if (getpcxmodefpt(fpt)!=PCX_VGA16) return;
	p=(PCXHDR *)malloc(sizeof(PCXHDR));
	getpcxhdrfpt(p,fpt);

        offset=y*xwidth+x/8;

        lmask=0xff;             /* for later update */
        rmask=0;

        if (palettepcx) for (i=0;i<16;i++)
		hgSetRGBpalette(i,p->Colormap[i][0]>>2,p->Colormap[i][1]>>2,p->Colormap[i][2]>>2);

	bytes=p->BytesperLine*4;
	temp=(char *)malloc((size_t)bytes);

	for (i=y;i<y+p->y2-p->y1+1;i++) {
		n=readpcxline(temp,fpt,bytes);
		n>>=2;                          /* n=n/4 */
                writepcxlinev(offset,temp,n-2,lmask,rmask);
                offset+=xwidth;
                if (i>=hgGety2r()) break;
	}
	fclose(fpt);
	free(temp);
	free(p);
}
