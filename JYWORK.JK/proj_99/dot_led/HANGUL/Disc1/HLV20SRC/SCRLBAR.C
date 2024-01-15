/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              Scroll Bar                */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hghlib.h"
#include "hwindow.h"
#include "hginit.h"
#include "key.h"

#include <alloc.h>

HSCRLBAR *hgHSCRLBAR_Load(int total,int screen,int direc,WIDTH w)
{
	HSCRLBAR *p;

        p=(HSCRLBAR *)malloc(sizeof(HSCRLBAR));

        if (total<screen) total=screen;

        p->total=total;
        p->screen=screen;
        p->direc=direc;
        p->size=w;
        p->pos=0;

        return p;
}
void hgHSCRLBAR_Free(HSCRLBAR **p)
{
        free(*p);
}
void hgHSCRLBAR_Draw(HSCRLBAR *p,int x,int y)
{
        int x1,x2,y1,y2;
        int width;
        int bwidth;
        int start;

        hgHideMouse();

        x1=x;x2=x+p->size.xwidth;
        y1=y;y2=y+p->size.ywidth;

        if (p->direc==HORIZ) width=(x2-x1-44);
        else width=(y2-y1-44);

        if (p->total!=0) {
		start=(long)p->pos*width/p->total;
		bwidth=(long)p->screen*width/p->total;
        }
        else {
                bwidth=width;
                start=0;
        }

/*      if (p->direc==VERT) {
                hgBoxFill(x1+1,y1,x1+16,y2,LIGHTGRAY);
                hgBox(x1,y1,x1+16,y2,DARKGRAY);
                hgBoxFill(x1+1,y1+20,x1+15,y2-20,DARKGRAY);
                hgLine(x1+8,y1+3,x1+3,y1+16,WHITE);
                hgLine(x1+8,y1+3,x1+13,y1+16,ZERO);
                hgLine(x1+3,y1+16,x1+13,y1+16,ZERO);
                hgLine(x1+8,y2-3,x1+3,y2-16,WHITE);
                hgLine(x1+8,y2-3,x1+13,y2-16,ZERO);
                hgLine(x1+3,y2-16,x1+13,y2-16,ZERO);
                hgBoxFill(x1+2,y1+22+start,x1+14,y1+22+start+bwidth,LIGHTGRAY);
	} else {
                hgBoxFill(x1,y1+1,x2,y1+16,LIGHTGRAY);
                hgBox(x1,y1,x2,y1+16,DARKGRAY);
                hgBoxFill(x1+20,y1+1,x2-20,y1+15,DARKGRAY);
                hgLine(x1+3,y1+8,x1+16,y1+3,WHITE);
                hgLine(x1+3,y1+8,x1+16,y1+13,ZERO);
                hgLine(x1+16,y1+3,x1+16,y1+13,ZERO);
                hgLine(x2-3,y1+8,x2-16,y1+3,WHITE);
                hgLine(x2-3,y1+8,x2-16,y1+13,ZERO);
                hgLine(x2-16,y1+3,x2-16,y1+13,ZERO);
                hgBoxFill(x1+22+start,y1+2,x1+22+start+bwidth,y1+14,LIGHTGRAY);
        } */

        if (p->direc==VERT) {
                hgBoxFill(x1+1,y1,x1+16,y2,LIGHTGRAY);
                hgBox(x1,y1,x1+16,y2,DARKGRAY);
                hgLine(x1+1,y1+20,x1+15,y1+20,ZERO);
                hgLine(x1+1,y2-20,x1+16,y2-20,ZERO);

		hgLine(x1+8,y1+3,x1+3,y1+16,WHITE);
                hgLine(x1+8,y1+3,x1+13,y1+16,ZERO);
                hgLine(x1+3,y1+16,x1+13,y1+16,ZERO);

                hgLine(x1+8,y2-3,x1+3,y2-16,WHITE);
                hgLine(x1+8,y2-3,x1+13,y2-16,ZERO);
                hgLine(x1+3,y2-16,x1+13,y2-16,ZERO);

                hgLine(x1+2,y1+22+start,x1+14,y1+22+start,WHITE);
                hgVline(x1+2,y1+22+start,y1+22+start+bwidth,WHITE);
                hgLine(x1+2,y1+22+start+bwidth,x1+14,y1+22+start+bwidth,ZERO);
                hgVline(x1+14,y1+22+start,y1+22+start+bwidth,ZERO);
        } else {
                hgBoxFill(x1,y1+1,x2,y1+16,LIGHTGRAY);
                hgBox(x1,y1,x2,y1+16,DARKGRAY);
                hgVline(x1+20,y1+1,y1+15,ZERO);
                hgVline(x2-20,y1+1,y1+15,ZERO);

                hgLine(x1+3,y1+8,x1+16,y1+3,WHITE);
		hgLine(x1+3,y1+8,x1+16,y1+13,ZERO);
                hgLine(x1+16,y1+3,x1+16,y1+13,ZERO);

                hgLine(x2-3,y1+8,x2-16,y1+3,WHITE);
                hgLine(x2-3,y1+8,x2-16,y1+13,ZERO);
                hgLine(x2-16,y1+3,x2-16,y1+13,ZERO);

                hgHline(x1+22+start,x1+22+start+bwidth,y+2,WHITE);
                hgHline(x1+22+start,x1+22+start+bwidth,y+14,ZERO);
                hgVline(x1+22+start,y1+2,y1+14,WHITE);
                hgVline(x1+22+start+bwidth,y1+2,y1+14,ZERO);
        }
        hgShowMouse();
}

void hgHSCRLBAR_Update(HSCRLBAR *p,int x,int y)
{
        int x1,x2,y1,y2;
        int width;
	int bwidth;
        int start;
        int save;

        hgHideMouse();

        x1=x;x2=x+p->size.xwidth;
        y1=y;y2=y+p->size.ywidth;

        if (p->direc==HORIZ) width=(x2-x1-44);
        else width=(y2-y1-44);

        save=p->pos;
        if ((p->pos+p->screen) > p->total) p->pos=p->total-p->screen;

        if (p->total!=0) {
		start=(long)p->pos*width/p->total;
		bwidth=(long)p->screen*width/p->total;
	}
	else {
		bwidth=width;
		start=0;
	}

        if (p->direc==VERT) {
                hgBoxFill(x1+2,y1+21,x1+14,y2-21,LIGHTGRAY);
                hgLine(x1+2,y1+22+start,x1+14,y1+22+start,WHITE);
                hgVline(x1+2,y1+22+start,y1+22+start+bwidth,WHITE);
                hgLine(x1+2,y1+22+start+bwidth,x1+14,y1+22+start+bwidth,ZERO);
                hgVline(x1+14,y1+22+start,y1+22+start+bwidth,ZERO);
        } else {
                hgBoxFill(x1+22,y1+2,x2-22,y1+14,LIGHTGRAY);
                hgHline(x1+22+start,x1+22+start+bwidth,y+2,WHITE);
                hgHline(x1+22+start,x1+22+start+bwidth,y+14,ZERO);
                hgVline(x1+22+start,y1+2,y1+14,WHITE);
                hgVline(x1+22+start+bwidth,y1+2,y1+14,ZERO);
        }
        hgShowMouse();
	p->pos=save;
}
void hgHSCRLBAR_Choose(HSCRLBAR *p,int x,int y)
{
	int x1,x2,y1,y2;
        int width;
        int bwidth;
        int start;
        int mx,my;
        int mmx,mmy;
        int prvs=-1;
        int ch;
	int temp;

	hgGetMousePos(&mx,&my);

	x1=x;x2=x+p->size.xwidth;
	y1=y;y2=y+p->size.ywidth;

	if (p->direc==HORIZ) width=(x2-x1-44);
	else width=(y2-y1-44);

        if (p->total!=0) {
		start=(long)p->pos*width/p->total;
		bwidth=(long)p->screen*width/p->total;
	}
	else {
		bwidth=width;
		start=0;
	}
	if (p->direc==VERT) {
		if (mx>x1 && mx<x1+16 && my>y1 && my<y1+20) {
                        if (p->pos>0) p->pos--;
                        hgHideMouse();
                        hgLine(x1+8,y1+3,x1+3,y1+16,ZERO);
                        hgLine(x1+8,y1+3,x1+13,y1+16,WHITE);
                        hgLine(x1+3,y1+16,x1+13,y1+16,WHITE);
                        hgShowMouse();
			hgHSCRLBAR_Update(p,x,y);
                        while (hgLeftMouse());
			hgHideMouse();
			hgLine(x1+8,y1+3,x1+3,y1+16,WHITE);
			hgLine(x1+8,y1+3,x1+13,y1+16,ZERO);
			hgLine(x1+3,y1+16,x1+13,y1+16,ZERO);
			hgShowMouse();
			return;
		}
		if (mx>x1 && mx<x1+16 && my>y2-20 && my<y2) {
			if (p->pos+p->screen<p->total) p->pos++;
			hgHideMouse();
			hgLine(x1+8,y2-3,x1+3,y2-16,ZERO);
			hgLine(x1+8,y2-3,x1+13,y2-16,WHITE);
			hgLine(x1+3,y2-16,x1+13,y2-16,WHITE);
			hgShowMouse();
			hgHSCRLBAR_Update(p,x,y);
			while (hgLeftMouse());
			hgHideMouse();
			hgLine(x1+8,y2-3,x1+3,y2-16,WHITE);
			hgLine(x1+8,y2-3,x1+13,y2-16,ZERO);
			hgLine(x1+3,y2-16,x1+13,y2-16,ZERO);
			hgShowMouse();
			return;
		}
		if (mx>x1 && mx<x1+16 && my>y1+20 && my<y1+22+start) {
			p->pos=(long)( my-y1-20 )*p->total / width-p->screen/2;
			if (p->pos<0) p->pos=0;
			hgHSCRLBAR_Update(p,x,y);
			while (hgLeftMouse());
			return;
		}
		if (mx>x1 && mx<x1+16 && my>y1+22+start+bwidth && my<y2-20) {
			p->pos=(long)(my-y1-20)*p->total/width-p->screen/2;
			if ((p->pos+p->screen) > p->total)
				 p->pos=p->total-p->screen;
			hgHSCRLBAR_Update(p,x,y);
			while (hgLeftMouse());
			return;
		}
		if (mx>x1 && mx<x1+16 && my>y1+20 && my<y2-20) {
			temp=p->pos;
			do {
				hgGetMousePos(&mmx,&mmy);
				if (mmx>x1+2 && mmx<x1+14){
					temp=p->pos-(long)(my-mmy)*p->total/width;
					if (temp<0) temp=0;
					if (temp+p->screen>p->total)
						temp=p->total-p->screen;
					start=(long)temp*width/p->total;
				}
				if (prvs!=start) {
					hgHideMouse();
					hgBoxFill(x1+2,y1+21,x1+14,y2-21,LIGHTGRAY);
					hgLine(x1+2,y1+22+start,x1+14,y1+22+start,ZERO);
					hgVline(x1+2,y1+22+start,y1+22+start+bwidth,ZERO);
					hgLine(x1+2,y1+22+start+bwidth,x1+14,y1+22+start+bwidth,WHITE);
					hgVline(x1+14,y1+22+start,y1+22+start+bwidth,WHITE);
					prvs=start;
					hgShowMouse();
				}
				ch=hgLeftMouse();
			} while (ch);
			if (mmx>x1 && mmx<x1+16 && mmy>y1+20 && mmy<y2-20)
				p->pos=temp;
			hgHSCRLBAR_Update(p,x,y);
			return;
		}
	} else {
		if (mx>x1 && mx<x1+20 && my>y1 && my<y1+16) {
			if (p->pos>0) p->pos--;
			hgHideMouse();
			hgLine(x1+3,y1+8,x1+16,y1+3,ZERO);
			hgLine(x1+3,y1+8,x1+16,y1+13,WHITE);
			hgLine(x1+16,y1+3,x1+16,y1+13,WHITE);
			hgShowMouse();
			hgHSCRLBAR_Update(p,x,y);
			while (hgLeftMouse());
			hgHideMouse();
			hgLine(x1+3,y1+8,x1+16,y1+3,WHITE);
			hgLine(x1+3,y1+8,x1+16,y1+13,ZERO);
			hgLine(x1+16,y1+3,x1+16,y1+13,ZERO);
			hgShowMouse();
			return;
		}
		if (mx>x2-20 && mx<x2 && my>y1 && my<y1+16) {
			if (p->pos+p->screen<p->total) p->pos++;
			hgHideMouse();
			hgLine(x2-3,y1+8,x2-16,y1+3,ZERO);
			hgLine(x2-3,y1+8,x2-16,y1+13,WHITE);
			hgLine(x2-16,y1+3,x2-16,y1+13,WHITE);
			hgShowMouse();
			hgHSCRLBAR_Update(p,x,y);
			while (hgLeftMouse());
			hgHideMouse();
			hgLine(x2-3,y1+8,x2-16,y1+3,WHITE);
			hgLine(x2-3,y1+8,x2-16,y1+13,ZERO);
			hgLine(x2-16,y1+3,x2-16,y1+13,ZERO);
			hgShowMouse();
			return;
		}
		if (mx>x1+20 && mx<x1+22+start && my>y1 && my<y1+16) {
			p->pos=(long)( mx-x1-20 )*p->total / width-p->screen/2;
			if (p->pos<0) p->pos=0;
			hgHSCRLBAR_Update(p,x,y);
			while (hgLeftMouse());
			return;
		}
		if (mx>x1+22+start+bwidth && mx<x2-20 && my>y1 && my<y1+16) {
			p->pos=(long)(mx-x1-20)*p->total/width-p->screen/2;
			if ((p->pos+p->screen) > p->total)
				 p->pos=p->total-p->screen;
			hgHSCRLBAR_Update(p,x,y);
			while (hgLeftMouse());
			return;
		}
		if (mx>x1+20 && mx<x2-20 && my>y1 && my<y1+16) {
			temp=p->pos;
			do {
				hgGetMousePos(&mmx,&mmy);
				if (mmy>y1+2 && mmy<y1+14){
					temp=p->pos-(mx-mmx)*p->total/width;
					if (temp<0) temp=0;
					if (temp+p->screen>p->total)
						temp=p->total-p->screen;
					start=(long)temp*width/p->total;
				}
				if (prvs!=start) {
					hgHideMouse();
					hgBoxFill(x1+22,y1+2,x2-22,y1+14,LIGHTGRAY);
					hgHline(x1+22+start,x1+22+start+bwidth,y+2,ZERO);
					hgHline(x1+22+start,x1+22+start+bwidth,y+14,WHITE);
					hgVline(x1+22+start,y1+2,y1+14,ZERO);
					hgVline(x1+22+start+bwidth,y1+2,y1+14,WHITE);
					prvs=start;
					hgShowMouse();
				}
				ch=hgLeftMouse();
			} while (ch);
			if (mmy>y1 && mmy<y1+16 && mmx>x1+20 && mmx<x2-20)
				p->pos=temp;
			hgHSCRLBAR_Update(p,x,y);
			return;
		}
	}
}

