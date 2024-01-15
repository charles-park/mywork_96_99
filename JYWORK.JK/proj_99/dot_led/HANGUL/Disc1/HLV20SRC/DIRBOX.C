/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Object Drawing              */
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

#include "hghlib.h"
#include "hginit.h"
#include "hwindow.h"
#include "key.h"

/* global flag */

int hercbar=hgFAIL;             /* hercules only bar mode */
int recpos=hgFAIL;

int dbpos=0;                    /* for special process */
int dobjt=0;
int dtpos=0;
int mbpos=0;
int mobjt=0;
int stpos=0;

/* some prototype */

void print_oneline(int x,int y,char *items[],int pos,int num,int xnum,int xincre);

void hgChangeDirBox(int a,int b,int c)
{
        mbpos=a;
        mobjt=b;
        stpos=c;
}

void hgSetDirBox(int a,int b,int c)
{
        dbpos=a;
        dobjt=b;
        dtpos=c;
}
void hgGetDirBox(int *a,int *b,int *c)
{
        *a=dbpos;
        *b=dobjt;
        *c=dtpos;
}

HDIRBOX *hgHDIRBOX_Load(char *i[MAXITEM],WIDTH w,int xnum,int ynum)
{
        HDIRBOX *p;
	int pointer=0;

        p=(HDIRBOX *) malloc(sizeof(HDIRBOX));

        while (strcmp(i[pointer],"")) {
                p->items[pointer]=(char *)malloc((size_t)(strlen(i[pointer])+2));
                strcpy(p->items[pointer],i[pointer]);
                pointer++;
	}

        p->items[pointer]=(char *)malloc((size_t)3);
        strcpy(p->items[pointer],"");

        p->size=w;
        p->xnum=xnum;
        p->ynum=ynum;

        return p;
}

void hgHDIRBOX_Free(HDIRBOX **p)
{
	int pos=0;
        while (strcmp((*p)->items[pos],""))
                free((*p)->items[pos++]);
        if ((*p)->items[pos]) free((*p)->items[pos]);
        free (*p);
}

#define HERCBAR(x,y) if (hgIsHerc() && hercbar) hgBox(x-2,y-2,x-2+save+16,y-2+20,ZERO)

void hgHDIRBOX_Choose(HDIRBOX *p,int x,int y,char *title,char *ret)
{
        int num;
        int i;
        int yfac,xfac;
        int xx,yy;
        int pos=0;
        int ch;
	int offset=0;
        int os=0;
        int ps=0;
        int px,py;
        int max;
        int scrmax;
        int save;
        int savetemp;           /* for pos saving */

        BCOLOR    barc={ wcBARNORMAL,wcINSIDE };
        WIDTH     barw;
	HBAR     *bar;
        HSCRLBAR *sbar;
        WIDTH     sbarw;
        int       mx,my;
        int       xc,yc;
        int       diff;
        int       fill=LIGHTGRAY;

        if (hgIsHerc() && hercbar) fill=WHITE;     /* for hercules */

        save=max=ret_maxlength(p->items);

        hgWIDTH_Load(&barw ,max+16,20);
        hgWIDTH_Load(&sbarw,16,p->size.ywidth-47);
	bar=hgHBAR_Load(barc,barw);

        hgHideMouse();
        hgPrtWindowXy(x,y,x+p->size.xwidth,y+p->size.ywidth,title);
        hgHideMouse();

        hgBoxFill(x+14,y+34,x+p->size.xwidth-13,y+p->size.ywidth-13,fill);

	xfac=hgGetXFactor();
	yfac=hgGetYFactor();
        hgSetXFactor(1);
        hgSetYFactor(1);

        num=ret_number(p->items)/16;
        max=(p->size.xwidth-24-12-13)/(p->xnum);

        scrmax=y+p->ynum*20+40;

        sbar=hgHSCRLBAR_Load((num-1)/p->xnum+1,p->ynum,VERT,sbarw);

        if (recpos) {
                offset=mobjt;
                dtpos=stpos;
        } else {
                offset=0;
                dtpos=0;
        }

        xx=17+x;
        yy=40+y;

        for (i=0;i<p->ynum;i++) {
                print_oneline(xx,yy+i*20,p->items,dtpos,num,p->xnum,max);
                dtpos+=p->xnum;
                if (dtpos>=num) break;
        }

        if (recpos) {
                dtpos=stpos;
                pos=mbpos;
                offset=mobjt;
        } else {
                pos=0;
                offset=0;
                dtpos=0;
        }

        sbar->pos=offset/(-20);

        px=xx=(pos%p->xnum)*max+17+x;
        py=yy=(pos/p->xnum)*20+40+y+offset;

        hgHSCRLBAR_Draw(sbar,x+p->size.xwidth-29,y+34);
        hgHideMouse();
        hgHBAR_Draw(bar,xx-2,yy-2,DRAW);
        HERCBAR(xx,yy);
        hgShowMouse();

        os=offset;

        do {
                if (xx!=px || yy!=py) {
                        hgHideMouse();
                        hgHBAR_Draw(bar,px-2,py-2,ERASE);
                        hgHBAR_Draw(bar,xx-2,yy-2,DRAW);
                        HERCBAR(xx,yy);
                        hgShowMouse();
                        px=xx;py=yy;
                }
                do {
                        ch=windelay(x,y,x+p->size.xwidth,y+p->size.ywidth);
                } while (ch==NOKEY);

                if (ch==MOUSE_LEFT)
                        if (hgHSCRLBAR_Area(sbar,x+p->size.xwidth-29,y+34)) {
                                ps=sbar->pos;
                                hgHSCRLBAR_Choose(sbar,x+p->size.xwidth-29,y+34);
                                diff=sbar->pos-ps;
                                ps=diff;
                                if (ps!=0) {
                                        pos+=(ps*p->xnum);
                                        ps=sbar->pos;
                                        offset=-ps*20;
                                        os=offset;
                                        hgHBAR_Draw(bar,xx-2,yy-2,ERASE);

                                        if (diff==1) {
                                               hgScrUp(x+14,y+40,x+p->size.xwidth-30,scrmax,20,fill);
                                               print_oneline(x+17,y+40+20*(p->ynum-1),p->items,ps*p->xnum+(p->ynum-1)*p->xnum,num,p->xnum,max);
                                               dtpos+=p->xnum;
                                        } else
                                        if (diff==-1) {
                                               hgScrDown(x+14,y+40,x+p->size.xwidth-30,scrmax,20,fill);
                                               print_oneline(x+17,y+40,p->items,ps*p->xnum,num,p->xnum,max);
                                               dtpos-=p->xnum;
                                        } else

                                        for (i=0;i<p->ynum;i++) {
                                                hgBoxFill(x+14,y+40+i*20,x+p->size.xwidth-30,y+60+i*20,fill);
                                                print_oneline(x+17,y+40+i*20,p->items,ps*p->xnum+i*p->xnum,num,p->xnum,max);
                                                dtpos=(offset/(-20))*p->xnum;
                                        }

                                        hgHBAR_Draw(bar,xx-2,yy-2,DRAW);
                                        HERCBAR(xx,yy);
                                        hgShowMouse();
                                }
                                while (hgLeftMouse());
                        } else {
                                hgGetMousePos(&mx,&my);
                                if (mx>x+17 && mx<x+p->size.xwidth-29
                                 && my>y+40 && my<y+p->size.ywidth-13) {
                                        xc=(mx-x-17)/max;
                                        yc=(my-y-40)/20;
                                        savetemp=-offset*p->xnum/20+xc+yc*p->xnum;
                                        if (savetemp<num) {
                                                pos=savetemp;
                                                xx=(pos%p->xnum)*max+17+x;
                                                yy=(pos/p->xnum)*20+40+y+offset;
                                                hgHideMouse();
                                                hgHBAR_Draw(bar,px-2,py-2,ERASE);
                                                hgHBAR_Draw(bar,xx-2,yy-2,DRAW);
                                                HERCBAR(xx,yy);
                                                hgShowMouse();
                                                while (hgLeftMouse());
                                                hgHideMouse();
                                                hgHBAR_Draw(bar,xx-2,yy-2,ERASE);
                                                hgShowMouse();
                                                break;
                                        }
                                }
                        }

                if (ch==RETURN) {
                        hgHBAR_Draw(bar,px-2,py-2,ERASE);
                        break;
                }

                if (ch==ESC || ch==LEFTMARK || ch==RIGHTMARK) {
                        hgHBAR_Draw(bar,px-2,py-2,ERASE);
                        break;
                }

                if (ch==RIGHT) pos++;
                if (ch==LEFT)  pos--;
                if (ch==UP)    pos-=p->xnum;
                if (ch==DOWN)  pos+=p->xnum;
                if (pos<0) pos=0;
                if (pos>=num) pos=num-1;

                xx=(pos%p->xnum)*max+17+x;
                yy=(pos/p->xnum)*20+40+y+offset;

                if ((yy+20)>(scrmax)) {
                       hgHBAR_Draw(bar,px-2,py-2,ERASE);
                       hgHideMouse();
                       hgScrUp(x+14,y+40,x+p->size.xwidth-30,scrmax,20,fill);
                       print_oneline(x+17,y+40+20*(p->ynum-1),p->items,pos,num,p->xnum,max);
                       hgHBAR_Draw(bar,px-2,py-2,DRAW);
                       HERCBAR(px,py);
		       offset-=20;
                       dtpos+=p->xnum;
                       yy=(pos/p->xnum)*20+40+y+offset;       /* recal. */
		}
		if (yy<(y+40)) {
                       hgHBAR_Draw(bar,px-2,py-2,ERASE);
                       hgHideMouse();
                       hgScrDown(x+14,y+40,x+p->size.xwidth-30,scrmax,20,fill);
                       print_oneline(x+17,y+40,p->items,pos,num,p->xnum,max);
                       hgHBAR_Draw(bar,px-2,py-2,DRAW);
                       HERCBAR(px,py);
		       offset+=20;
                       dtpos-=p->xnum;
                       yy=(pos/p->xnum)*20+40+y+offset;       /* recal. */
		}
                if (os!=offset) {
                        sbar->pos=sbar->pos-(offset-os)/20;
                        if (sbar->pos<0) sbar->pos=0;
                        if (sbar->pos+sbar->screen>=sbar->total)
                                sbar->pos=sbar->total-sbar->screen;
                        hgHSCRLBAR_Update(sbar,x+p->size.xwidth-29,y+34);
                        os=offset;
                }
        } while (1);

        if (recpos) hgSetDirBox(pos,offset,dtpos);

	hgSetXFactor(xfac);
	hgSetYFactor(yfac);

        hgHBAR_Free(&bar);
        hgHSCRLBAR_Free(&sbar);

        if (ch==ESC || ch==LEFTMARK || ch==RIGHTMARK) { strcpy(ret,"");return; }
        strcpy(ret,p->items[pos]);
        hgShowMouse();
}
void print_oneline(int x,int y,char *items[],int pos,int num,int xnum,int xw)
{
        int xpos=(pos/xnum)*xnum;
	int i;

        for (i=0;i<xnum;i++) {
		if ((xpos+i)>=num) return;
                hgOutTextXy(x+i*xw+6,y+1,items[xpos+i]);
	}
}
void hgHercBarOn()
{
        hercbar=hgSUCCESS;
}
void hgHercBarOff()
{
        hercbar=hgFAIL;
}
void hgSetRecPosOn()
{
        recpos=hgSUCCESS;
}
void hgSetRecPosOff()
{
        recpos=hgFAIL;
}
