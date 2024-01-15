/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              Quick PCX                 */
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

#include "hwindow.h"
#include "hghlib.h"
#include "hgpcx.h"
#include "hginit.h"

extern int dft_bstyle[MAXSTYLE][4];

void (*pcxq)(int x,int y,char *fname);

void hgEnablePCXFileDisplay(void (*p)(int x,int y,char *fname))
{
        pcxq=p;
}
void hgPCXFileDisplay(int x,int y,char *fname)
{
        (*pcxq)(x,y,fname);
}
void hgPrtPCXWindowXy(int x,int y,char *titl,char *fname)
{
        PCXHDR *p;
        int x1,y1,x2,y2;

        if (access(fname,0)) return;

        p=(PCXHDR *)malloc(sizeof(PCXHDR));
        hgGetPCXHdr(p,fname);

        x1=x/8*8;y1=y;
        x2=x+p->x2-p->x1;y2=y+p->y2-p->y1;
        hgPrtWindowXy(x1-13,y1-33,x2+13,y2+13,titl);
        hgPCXFileDisplay(x1,y1,fname);
        free(p);
}
void hgPrtPCXBorderXy(int x,int y,char *fname,int style)
{
        PCXHDR *p;
        int x1,y1,x2,y2;

        if (access(fname,0)) return;

        p=(PCXHDR *)malloc(sizeof(PCXHDR));
        hgGetPCXHdr(p,fname);
        x1=x/8*8;y1=y;
        x2=x+p->x2-p->x1;y2=y+p->y2-p->y1;
        hgDrawBorder(x1-5,y1-5,x2+5,y2+5,dft_bstyle[style],FILL);
        hgPCXFileDisplay(x1,y1,fname);
        free(p);
}
void hgPrtPCXWindowXyM(char *titl,char *fname)
{
        PCXHDR *p;
        int x1,y1,x2,y2;

        if (access(fname,0)) return;

        p=(PCXHDR *)malloc(sizeof(PCXHDR));
        hgGetPCXHdr(p,fname);

        x1=p->x1/8*8;y1=p->y1;x2=p->x2;y2=p->y2;
        x1=(hgGetx2r()+hgGetx1r())/2-(x2-x1)/2;
        y1=(hgGety2r()+hgGety1r())/2-(y2-y1)/2;
        x2=x1+p->x2-p->x1;
        y2=y1+p->y2-p->y1;
        hgPrtWindowXy(x1-13,y1-33,x2+13,y2+13,titl);
        hgPCXFileDisplay(x1,y1,fname);
        free(p);
}
void hgPrtPCXBorderXyM(char *fname,int style)
{
        PCXHDR *p;
        int x1,y1,x2,y2;

        if (access(fname,0)) return;

        p=(PCXHDR *)malloc(sizeof(PCXHDR));
        hgGetPCXHdr(p,fname);

        x1=p->x1/8*8;y1=p->y1;x2=p->x2;y2=p->y2;
        x1=(hgGetx2r()+hgGetx1r())/2-(x2-x1)/2;
        y1=(hgGety2r()+hgGety1r())/2-(y2-y1)/2;
        x2=x1+p->x2-p->x1;
        y2=y1+p->y2-p->y1;
        hgDrawBorder(x1-5,y1-5,x2+5,y2+5,dft_bstyle[style],FILL);
        hgPCXFileDisplay(x1,y1,fname);
        free(p);
}
