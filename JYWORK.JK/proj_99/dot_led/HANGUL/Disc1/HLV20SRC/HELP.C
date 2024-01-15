/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Help Management             */
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

#include "hgquick.h"
#include "hghlib.h"
#include "hginit.h"
#include "hwindow.h"
#include "key.h"

extern int ret_maxnumber(char *text[]);

HHELP *hgHHELP_Load(char *i[],int xc,int yl)
{
	HHELP *p;
	int    pointer=0;

        p=(HHELP *)malloc(sizeof(HHELP));

        while (strcmp(i[pointer],"")) {
                p->items[pointer]=(char *)malloc((size_t)(strlen(i[pointer])+2));
                strcpy(p->items[pointer],i[pointer]);
                pointer++;
	}

        p->items[pointer]=(char *)malloc((size_t)3);
        strcpy(p->items[pointer],"");

	p->xchars=xc;
	p->ylines=yl;

	return p;
}
void hgHHELP_Free(HHELP **p)
{
	int pos=0;
        while (strcmp((*p)->items[pos],""))
                free((*p)->items[pos++]);
        if ((*p)->items[pos]) free((*p)->items[pos]);
        free(*p);
}
void print_help_sub(int x,int y,char *buff[],int max,int sline,int ylines,int xsize)
{
	int i,j;

	for (i=0;i<ylines;i++) {
                hgBoxFill(x,y,x+xsize,y+16,WHITE);
		hgOutTextXy(x,y,buff[sline]);
		sline++;
		y+=16;
		if (sline>=max) break;
	}
	for (j=i+1;j<ylines;j++) {
		hgBoxFill(x,y,x+xsize,y+16,WHITE);
		y+=16;
	}
}
void hgHHELP_Draw(HHELP *p,int x,int y,char *title)
{
	int   bpos=0;
	FILE *idx,*hlp;
	int   xsize,ysize;
	int   prvs;
	int   sline;
	int   ch;
	int   i;
	int   ps,diff;		/* for scroll bar */

	HSCRLBAR *sbar;
	WIDTH     sbarw;

	bpos=ret_number(p->items)/16/hgGetYFactor();
	xsize=p->xchars*8+28;
	ysize=p->ylines*16+32+12+4;

	hgWIDTH_Load(&sbarw,16,p->ylines*16+3);
	sbar=hgHSCRLBAR_Load(bpos,p->ylines,VERT,sbarw);
	hgHideMouse();
        hgPrtWindowXy(x,y,x+xsize+16,y+ysize,title);
	hgHideMouse();
	hgHSCRLBAR_Draw(sbar,x+xsize-12,y+33);
	hgHideMouse();
	prvs=sline=0;
        print_help_sub(x+14,y+34,p->items,bpos,sline,p->ylines,xsize-28);
	while (1) {
		hgShowMouse();
		do {
			ch=windelay(x,y,x+xsize+16,y+ysize);
		} while (ch==NOKEY);
		prvs=sline;
		if (ch==MOUSE_LEFT)
			if (hgHSCRLBAR_Area(sbar,x+xsize-12,y+33)) {
				ps=sbar->pos;
				hgHSCRLBAR_Choose(sbar,x+xsize-12,y+33);
				diff=sbar->pos-ps;
                                if (diff!=0) {
					hgHideMouse();
                                        if (diff==1) ch=DOWN;
                                        else if (diff==-1) ch=UP;
                                        else sline=sbar->pos;
				}
				while (hgLeftMouse());
			}
		if (ch==PgDn) {
			sline+=p->ylines;
			if (sline>=bpos) sline-=p->ylines;
		}
		if (ch==PgUp) {
			sline-=p->ylines;
			if (sline<0) sline=0;
		}
                if (ch==CTRL_PgDn) {
                        sline=bpos-p->ylines;
                        if (sline<0) sline=0;
                }
                if (ch==CTRL_PgUp) sline=0;
		if (ch==ESC || ch==RETURN || ch==LEFTMARK || ch==RIGHTMARK) break;
                if (prvs!=sline) {
                        hgHideMouse();
                        print_help_sub(x+14,y+34,p->items,bpos,sline,p->ylines,xsize-48);
			sbar->pos=sline;
			hgHSCRLBAR_Update(sbar,x+xsize-12,y+33);
		}
		if (ch==UP) {
			sline--;
			if (sline<0) sline=0;
			else {
                                hgHideMouse();
                                hgScrDownWindowXy(x,y,x+xsize-13,y+ysize,16);
                                hgOutTextXy(x+14,y+34,p->items[sline]);
				sbar->pos=sline;
				hgHSCRLBAR_Update(sbar,x+xsize-12,y+33);
			}
		}
		if (ch==DOWN) {
			sline++;
			if ((sline+p->ylines-1)>=bpos) sline--;
			else {
                                hgHideMouse();
				hgScrUpWindowXy(x,y,x+xsize-13,y+ysize,16);
                                hgOutTextXy(x+14,y+(p->ylines-1)*16+34,p->items[sline+p->ylines-1]);
				sbar->pos=sline;
				hgHSCRLBAR_Update(sbar,x+xsize-12,y+33);
			}
		}
	};
	hgHSCRLBAR_Free(&sbar);
	hgShowMouse();
}
