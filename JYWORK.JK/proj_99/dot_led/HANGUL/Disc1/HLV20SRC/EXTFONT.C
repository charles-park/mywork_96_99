/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            External Font               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#define MAXFONTQUEUE 500

#include <stdio.h>
#include <alloc.h>
#include "hginit.h"

typedef struct extfont {
        FILE *fpt;
        char *p[MAXFONTQUEUE];
        int   n[MAXFONTQUEUE];
        int   qh,qt;            /* queue header,queue tail */
        int   xsize,ysize;
        int   qsize;
        int   fnum;             /* font number */
        char *buff;
} ExtFont;

extern void attribute_process(char *dest,int bytes);

ExtFont *hgOpenExtFont(char *path,int x,int y,int qsize)
{
        ExtFont *p;
        FILE *fpt;
        int   i;
        unsigned size;
        long     fsize;

        fpt=fopen(path,"rb");
        if (fpt==NULL) {
                fclose(fpt);
                return NULL;
        }

        fseek(fpt,0,2);         /* move to file end */
        fsize=ftell(fpt);
        fseek(fpt,0,0);         /* move to file start */

        fsize=fsize/(long)x;
        fsize=fsize/(long)y;

        p=(ExtFont *)malloc(sizeof(ExtFont));
        p->fpt=fpt;
        p->qsize=qsize;
        p->xsize=x;
        p->ysize=y;
        p->qh=p->qt=0;  /* queue is empty */
        p->fnum=fsize;

        size=x*y;

        for (i=0;i<qsize;i++)
                p->p[i]=(char *)malloc((size_t)size);

	p->buff=p->p[0];        /* for safe */

	return p;
}

void hgGetExtFont(ExtFont *p,int n)
{
        int i=p->qh;
        int flag=hgFAIL;
        char *temp;
        int m;
        long offset=0L;         /* file pointer offset */

        do {
                if (p->qh==p->qt) break;
                i++;
		if (i>=p->qsize) i=0;
                if (p->n[i]==n) {
                        flag=hgSUCCESS;
                        break;
                }
        } while (i!=p->qt);

        if (flag) {
		p->buff=p->p[i];
                return;
        }
        m=p->qt+1;
        if (m>=p->qsize) m=0;

        if (m==p->qh) {
                p->qh++;
		if (p->qh>=p->qsize) p->qh=0;
        }
        p->qt++;
	if (p->qt>=p->qsize) p->qt=0;

        offset=(long)n*(long)p->xsize*(long)p->ysize;
        fseek(p->fpt,offset,0);

	fread(p->p[p->qt],(size_t)(p->xsize*p->ysize),1,p->fpt);
        p->buff=p->p[p->qt];
        p->n[p->qt]=n;
}

void hgPutExtFont(ExtFont *p,int x,int y,int num)
{
        int m=hgGetXFactor();
        int n=hgGetYFactor();
        int bytes=p->xsize*p->ysize;
        int i;
        int fac=1;               /* factor */
        int xx;

        char *mbuff;             /* magnified character buffer1 */
        char *nbuff;             /*            ""             2 */
        char *buff ;             /* real external font buffer   */

        for (i=1;i<m;i++)        /* cal. x factor */
                fac*=2;

        mbuff=(char *)malloc((size_t)bytes*fac);
        nbuff=(char *)malloc((size_t)bytes*fac*n);
        buff =(char *)malloc((size_t)bytes);

        hgGetExtFont(p,num);      /* fetch */
        memcpy(buff,p->buff,bytes);
        attribute_process(buff,-bytes);

        if (m!=1) widen(buff,mbuff,bytes);
        else      memcpy(mbuff,buff,bytes);
        if (n!=1) lengthen(mbuff,nbuff,p->xsize*fac,p->ysize);
        else      memcpy(nbuff,mbuff,bytes*fac);

        putmag(x,y,nbuff,p->xsize*fac,p->ysize*n,hgGetTFcolor());

        free(mbuff);
        free(nbuff);
        free(buff);
}

int hgGetExtFontNum(ExtFont *p)
{
        return p->fnum;
}

void hgCloseExtFont(ExtFont *p)
{
        int i;

        for (i=0;i<p->qsize;i++)
                free(p->p[i]);
        fclose(p->fpt);
        free(p);
}
int hgGetExtFontXsize(ExtFont *p)
{
        return p->xsize;
}
int hgGetExtFontYsize(ExtFont *p)
{
        return p->ysize;
}
