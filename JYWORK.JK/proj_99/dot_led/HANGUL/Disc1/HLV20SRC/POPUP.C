/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          "Popup & Pulldown "           */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <stdio.h>
#include <alloc.h>

#include "hghlib.h"
#include "hwindow.h"
#include "key.h"
#include "hginit.h"
#include "hgquick.h"
#include "mouse.h"

int xb[MAXPOPUP];
int xw[MAXPOPUP];
int xn;                 /* counter */
int xp;                 /* pointer */
int maxlen=-1;
int xe[MAXPOPUP];

HPOPUP *hgHPOPUP_Load(char *i[],WCOLOR c,WIDTH w)
{
        HPOPUP *p;
	int pointer=0;
	int ii;

        p=(HPOPUP *) malloc(sizeof(HPOPUP));

        while (strcmp(i[pointer],"")) {
                p->items[pointer]=(char *)malloc((size_t)(strlen(i[pointer])+2));
                strcpy(p->items[pointer],i[pointer]);
                pointer++;
	}

	for (ii=0;ii<pointer;ii++)
		p->code[ii]=hgEnable;

        p->items[pointer]=(char *)malloc((size_t)3);
        strcpy(p->items[pointer],"");

        p->color=c;
        p->size=w;
        p->start=0;

        return p;
}

void hgHPOPUP_Loadcode(HPOPUP *p,int c[MAXITEM])
{
        int i;

        for (i=0;i<MAXITEM;i++) {
                if (!strcmp(p->items[i],"")) break;
                p->code[i]=c[i];
        }
}

void hgHPOPUP_Free(HPOPUP **p)
{
	int pos=0;
        while (strcmp((*p)->items[pos],""))
                free((*p)->items[pos++]);
        if ((*p)->items[pos]) free((*p)->items[pos]);
        free (*p);
}

HPULLDOWN *hgHPULLDOWN_Load(char *i[],int x[MAXITEM],WIDTH w,HPOPUP *pop[MAXPOPUP])
{
        HPULLDOWN *p;
	int pointer=0;
	int ii;

        p=(HPULLDOWN *) malloc(sizeof(HPULLDOWN));

        /* items init */

        while (strcmp(i[pointer],"")) {
                p->items[pointer]=(char *)malloc((size_t)(strlen(i[pointer])+2));
                strcpy(p->items[pointer],i[pointer]);
                pointer++;
	}

        /* xpos & popup menu init */

	for (ii=0;ii<pointer;ii++) {
		p->xpos[ii]=x[ii];
		p->p[ii]=pop[ii];
		p->code[ii]=hgEnable;
        }

        p->items[pointer]=(char *)malloc((size_t)3);
        strcpy(p->items[pointer],"");

        /* color and width init */

        p->start=0;
	p->size=w;

        return p;
}

void hgHPULLDOWN_Loadcode(HPULLDOWN *p,int c[MAXITEM])
{
        int i;

        for (i=0;i<MAXITEM;i++) {
                if (!strcmp(p->items[i],"")) break;
                p->code[i]=c[i];
        }
}

void hgHPULLDOWN_Free(HPULLDOWN **p)
{
	int pos=0;
        while (strcmp((*p)->items[pos],""))
                free((*p)->items[pos++]);
        if ((*p)->items[pos]) free((*p)->items[pos]);
        free (*p);
}

int hgHPOPUP_Choose(HPOPUP *p,int x,int y,int start)
{
        HBAR  *bar;
        BCOLOR bc;
	WIDTH  bw;
        int    ch;
        int    xx,yy;
        int    num;
	int    i;
        int    j;
        int    prvs;
        int    mousedrag=0;
        int    x1,y1,xwidth,ywidth;
        int    save;
        char   temp[30];

        if (start<0) start=p->start;    /* if start<0.use past value */
        num=ret_number(p->items)/16/hgGetYFactor();

        while (p->code[start]==hgDisable) {
                start++;
                if (start<0) start=num-1;
                if (start>num-1) start=0;
        }
        prvs=start;

        hgWIDTH_Load(&bw,p->size.xwidth-4,20*hgGetYFactor());
        hgBCOLOR_Load(&bc,wcBARNORMAL,wcBARNORMAL);
        bar=hgHBAR_Load(bc,bw);         /* bar define */

        hgHideMouse();

        hgGetImageVIRTUAL(x,y,x+p->size.xwidth,y+p->size.ywidth,&p->buff);

        hgDrawBorder(x,y,x+p->size.xwidth,y+p->size.ywidth,dft_bstyle[p->color.border],FILL);

        for (i=0;i<num;i++) {
                if (p->code[i]==hgDisable) {
                        if (!hgIsHerc()) {
                                hgSetHAttr(DIM);
                                hgSetEAttr(DIM);
                        }
                }
                hgOutTextXy(x+6,y+4+i*20*hgGetYFactor(),p->items[i]);
                if (p->code[i]==hgDisable) {
                        if (!hgIsHerc()) {
                                hgSetEAttr(NORMAL);
                                hgSetHAttr(NORMAL);
                        }
                }
        }
        xx=x+2;
        yy=y+2+start*20*hgGetYFactor();
        hgHBAR_Draw(bar,xx,yy,DRAW);

        while (1) {
                hgShowMouse();
                ch=inkey(NOWAIT);
                if (ch==MOUSE_LEFT) {
                        start=get_mpos(x+2,y+2,p->size.xwidth-4,20*hgGetYFactor(),num);
                        if (start!=-1)
                                if (p->code[start]!=hgDisable && !mousedrag) {
                                        yy=y+2+prvs*20*hgGetYFactor();
                                        hgHBAR_Draw(bar,xx,yy,ERASE);
                                        yy=y+2+start*20*hgGetYFactor();
                                        hgHBAR_Draw(bar,xx,yy,DRAW);
                                        delay(100);
                                        hgHBAR_Draw(bar,xx,yy,ERASE);
                                        break;
                                }
                                else  {
                                        if (p->code[start]==hgDisable)
                                                start=prvs;
                                        mousedrag=1;
                                }
                        else mousedrag=1;
                        if (start==-1) start=prvs;
                        if (maxlen!=-1) {
                                save=get_xwpos(xb,y+2-14-16*hgGetYFactor()+5,xw,4+16*hgGetYFactor(),xn);
                                if (save!=-1)
                                        if (xp!=save && xe[save]!=hgDisable) {
                                                xp=save;
                                                ch=-2;     /* for special */
                                                hgHideMouse();
                                                xx=x+2;
                                                yy=y+2+prvs*20*hgGetYFactor();
                                                hgHBAR_Draw(bar,xx,yy,ERASE);
                                                break;
                                        } else mousedrag=1;
                        }
                }
                if (ch!=MOUSE_LEFT && mousedrag) {
                        start=get_mpos(x+2,y+2,p->size.xwidth-4,20*hgGetYFactor(),num);
                        if (start!=-1) {
                                hgHBAR_Draw(bar,x+2,y+prvs*20*hgGetYFactor()+2,ERASE);
                                hgHBAR_Draw(bar,x+2,y+start*20*hgGetYFactor()+2,DRAW);
                                delay(100);
                                hgHBAR_Draw(bar,x+2,y+start*20*hgGetYFactor()+2,ERASE);
                                prvs=start;
                                break;
                        } else  {
                                start=prvs;
                                if (maxlen!=-1) {
                                        x1=x+2;
                                        xwidth=maxlen;
                                        y1=y+2-14-16*hgGetYFactor()+5;
                                        ywidth=4+16*hgGetYFactor();
                                        if (get_mpos(x1,y1,xwidth,ywidth,1)==-1)
                                                ch=ESC;
                                }
                        }
                        mousedrag=0;
                }

                if (ch==ESC || ch==RETURN || ch==MOUSE_RIGHT) {
                        xx=x+2;
                        yy=y+2+prvs*20*hgGetYFactor();
                        hgHBAR_Draw(bar,xx,yy,ERASE);
                        break;
                }
                if (ch==UP) start--;
                if (ch==DOWN) start++;
                if (ch==LEFT || ch==RIGHT) {
                        xx=x+2;
                        yy=y+2+prvs*20*hgGetYFactor();
                        hgHBAR_Draw(bar,xx,yy,ERASE);
                        break;
                }
                if (start<0) start=num-1;
                if (start>num-1) start=0;
		while (p->code[start]==hgDisable) {
                        if (ch==UP) start--;
                        else start++;
                        if (start<0) start=num-1;
                        if (start>num-1) start=0;
                }
                if (prvs!=start) {
                        yy=y+2+prvs*20*hgGetYFactor();
                        hgHBAR_Draw(bar,xx,yy,ERASE);
                        yy=y+2+start*20*hgGetYFactor();
                        hgHBAR_Draw(bar,xx,yy,DRAW);
                        prvs=start;
                }
        }
        hgHideMouse();
        hgPutImageVIRTUAL(x,y,x+p->size.xwidth,y+p->size.ywidth,p->buff);
        hgFreeVIMAGE(&p->buff);

        hgHBAR_Free(&bar);

	p->start=start;

        if (ch==-2)  return -2;
        if (ch==ESC) return -1;
        if (ch==LEFT) return -LEFT;
        if (ch==RIGHT) return -RIGHT;

        hgShowMouse();
        return start;           /* case 'press RETURN' */
}
void hgHPULLDOWN_Choose(HPULLDOWN *p,int x,int y,int start,int *xx,int *yy)
{
        HBAR  *bar;

        BCOLOR bc;
        WIDTH  bw;

        int    ch;
        int    num;
        int    i;
        int    ret=0;
        int    prvs;

        if (start<0) start=p->start;
        num=ret_number(p->items)/16/hgGetYFactor();

        while (p->code[start]==hgDisable) {
                start++;
                if (start<0) start=num-1;
                if (start>num-1) start=0;
        }

        prvs=start;

        hgHideMouse();

        hgBCOLOR_Load(&bc,wcBARNORMAL,wcBARNORMAL);
        hgDrawBorder(x,y,x+p->size.xwidth,y+14+16*hgGetYFactor(),BOXNORMAL,FILL);
        maxlen=ret_maxlength(p->items)+10;
        hgWIDTH_Load(&bw,maxlen,4+16*hgGetYFactor());
        bar=hgHBAR_Load(bc,bw);

        for (i=0;i<num;i++) {
                if (p->code[i]==hgDisable) {
                        if (!hgIsHerc()) {
                                hgSetEAttr(DIM);
                                hgSetHAttr(DIM);
                        }
                }
                hgOutTextXy(x+p->xpos[i],y+8,p->items[i]);
                if (p->code[i]==hgDisable) {
                        if (!hgIsHerc()) {
                                hgSetEAttr(NORMAL);
                                hgSetHAttr(NORMAL);
                        }
                }
                xb[i]=x+p->xpos[i]-5;
                xw[i]=maxlen;
                xe[i]=p->code[i];
        }

        xn=num;

        do {
                hgHBAR_Draw(bar,x+p->xpos[start]-5,y+5,DRAW);
                prvs=start;
		while (1) {
                        hgShowMouse();
                        do {
                                ch=inkey(NOWAIT);
                        } while (ch==NOKEY);
                        if (ch==MOUSE_LEFT) {
                                start=get_xwpos(xb,y+5,xw,20,num);
                                if (start!=-1 && p->code[start]!=hgDisable) {
                                        hgHideMouse();
                                        hgHBAR_Draw(bar,x+p->xpos[prvs]-5,y+5,ERASE);
                                        break;
                                }
                                start=prvs;
                        }
                        if (ch==ESC || ch==RETURN || ch==DOWN) {
                                hgHideMouse();
                                hgHBAR_Draw(bar,x+p->xpos[prvs]-5,y+5,ERASE);
                                break;
                        }
			if (ch==LEFT) start--;
			if (ch==RIGHT) start++;
                        if (start<0) start=num-1;
                        if (start>num-1) start=0;
			while (p->code[start]==hgDisable) {
				if (ch==LEFT) start--;
                                else start++;
                                if (start<0) start=num-1;
                                if (start>num-1) start=0;
                        }
                        if (start!=prvs) {
                                hgHideMouse();
                                hgHBAR_Draw(bar,x+p->xpos[prvs]-5,y+5,ERASE);
                                hgHBAR_Draw(bar,x+p->xpos[start]-5,y+5,DRAW);
                                prvs=start;
                                hgShowMouse();
                        }
                }
                if (ch==ESC) {
                        p->start=start;
                        hgHBAR_Free(&bar);
                        *xx=-1;
                        *yy=-1;
                        return;
                }

                do {
                        xp=start;       /* global pointer */
                        hgHBAR_Draw(bar,x+p->xpos[start]-5,y+5,DRAW);
                        ret=hgHPOPUP_Choose(p->p[start],x+p->xpos[start]-5,y+14+16*hgGetYFactor(),-1);
                        hgHBAR_Draw(bar,x+p->xpos[start]-5,y+5,ERASE);
                        if (ret==-LEFT) start--;
                        else if (ret==-RIGHT) start++;
                        else if (ret==-1) break;        /* for ESC */
                        else if (ret==-2) start=xp;
			else break;                     /* for adequate value */
			if (start<0) start=num-1;
			if (start>num-1) start=0;
                        while (p->code[start]==hgDisable) {
                                if (ret==-LEFT) start--;
                                else start++;
                                if (start<0) start=num-1;
                                if (start>num-1) start=0;
                        }
		} while (1);
		p->start=start;
        } while (ret==-1);

	*xx=start;
	*yy=ret;

        hgHBAR_Free(&bar);

        maxlen=-1;              /* re-initialize */

        hgShowMouse();

        if (ch==ESC) {
                *xx=-1;
                *yy=-1;
                return;
        }
        return;
}
