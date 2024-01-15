/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           ICON Management              */
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
#include "hghlib.h"
#include "hginit.h"
#include "hwindow.h"

/* function prototype */

int  get_pos(HICON *p,int x,int y);

HICON *hgHICON_Load(char *fname)
{
        char hdr[40];
        unsigned size;
        FILE *fpt;
        int  xs,ys;
        HICON *p;
        int i,j;
        int pos;
        char data1,data2;

        fpt=fopen(fname,"rb");
        if (fpt==NULL) return NULL;

        p=(HICON *)malloc(sizeof(HICON));

        fseek(fpt,31,0);
        xs=getc(fpt);                           /* get Xsize   */
        ys=getc(fpt);                           /* get Ysize   */
        fseek(fpt,0,0);
        fread(hdr,33,1,fpt);                    /* read header */
        size=(unsigned) ( xs*4*ys );
        p->buff=(char *)malloc((size_t)size);
        fread(p->buff,size,1,fpt);
        fclose(fpt);
        p->size.xwidth=xs;
        p->size.ywidth=ys;
        if (hgIsHerc()) hgDoDither(p);
        return p;
}

void hgHICON_Free(HICON **p)
{
        free((*p)->buff);
        free(*p);
}
void hgHICON_Draw(HICON *p,int x,int y)
{
        int i,j;
        int pos;

        for (i=0;i<p->size.ywidth;i++)
                for (j=0;j<p->size.xwidth*4;j++) {
                        pos=get_pos(p,j,i);
                        hgPlotXy(x+j*2,y+i,c256[(p->buff[pos] & 0xf0)>>4]);
                        hgPlotXy(x+j*2+1,y+i,c256[(p->buff[pos] & 0x0f)]);
                }
}
HICON *hgHICON_Set(void (*iconset)(HICON *p))
{
        HICON *p;

        p=(HICON *)malloc(sizeof(HICON));
        (*iconset)(p);

        if (hgIsHerc()) hgDoDither(p);
        return p;
}

int hgIsIcon(char *fname)
{
    long offset,size;
    int  xs,ys;
    FILE *f;

    f=fopen(fname,"rb");
    if (f==NULL) return -1;           /* for error */

    fseek(f,31,0);
    xs=getc(f);
    ys=getc(f);
    fseek(f,0,2);
    size=xs*ys*4+33;
    offset=ftell(f);
    fclose(f);
    if (size!=offset) return 0;
    else return 1;
}
int get_pos(HICON *p,int x,int y)
{
        int pos;

        pos=p->size.xwidth*y*4+x;
        return pos;
}

