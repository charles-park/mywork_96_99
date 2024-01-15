/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        PCX 256 Color VGA decoding      */
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
#include <dos.h>

#include "hgpcx.h"

/* this is defined in 'PCXDEC.C' module.etc */

extern void writepcxline256(unsigned offset,char *temp,int n,char page);
extern int  readpcxline(char *s,FILE *fpt,int bytes);
extern void getpcxhdrfpt(PCXHDR *p,FILE *fpt);
extern int  getpcxmodefpt(FILE *fpt);
extern int  xbyte;      /* screen width */
extern int  palettepcx;

void hgPCXFileDisplay256(int x,int y,char *fname)
{
        FILE    *fpt;
        PCXHDR  *p;
        char    *temp;
        int      i,j;
	unsigned offset;
        long     tmp;
        int      page;
        int      n;
        int      bytes;
        int      r,g,b;
	unsigned xwidth=xbyte;
        int      wid;

	fpt=fopen(fname,"rb");
	if (fpt==NULL) return;

	if (getpcxmodefpt(fpt)!=PCX_VGA256) return;
	p=(PCXHDR *)malloc(sizeof(PCXHDR));
	getpcxhdrfpt(p,fpt);
        bytes=p->BytesperLine;

	temp=(char *)malloc((size_t)bytes);

        tmp=(long)y*xwidth+x;
	page=tmp>>16;
	offset=(unsigned)tmp;
        wid=p->x2-p->x1+1;

        _DL=page;
        setpage();

        fseek(fpt,-768L,2);

        if (palettepcx) for (i=0;i<256;i++) {
		r=fgetc(fpt)>>2;
		g=fgetc(fpt)>>2;
		b=fgetc(fpt)>>2;
		hgSetRGBpalette256(i,r,g,b);
	}

	fseek(fpt,128,0);
	for (i=y;i<y+p->y2-p->y1+1;i++) {
		n=readpcxline(temp,fpt,bytes);
                if (n>wid) n=wid;
                writepcxline256(offset,temp,n,(char)page);
                offset+=xwidth;
                if (offset<xwidth) {
                        page++;
                        _DL=page;
                        setpage();
                }
                if (i>=hgGety2r()) break;
	}
	fclose(fpt);
	free(temp);
	free(p);
}
