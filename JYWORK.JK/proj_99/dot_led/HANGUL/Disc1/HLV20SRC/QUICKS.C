/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Quick Select               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <string.h>

#include "hginit.h"
#include "hghlib.h"
#include "hgquick.h"
#include "hwindow.h"
#include "key.h"
#include "mouse.h"

extern int  saveflag;
extern void savearea(int x1,int y1,int x2,int y2);
extern int ret_number(char *text[]);
extern int ret_maxlength(char *text[]);

int hgSelectXy(int x1,int y1,int x2,int y2,char *items[],int start)
{
	int xwidth=x2-x1+1;
	int ywidth=y2-y1+1;

	/* message box define */

        WIDTH  mesgs;
        WCOLOR mesgc;
        HBOX  *mesgb;

	/* message bar define */

        WIDTH  mbars;
        BCOLOR mbarc;
        HBAR  *mbarb;

	int pos=0;
	int ch;
	int max;
        int prvs;
        int mousedrag=0;
        int *ext;
        int num;
        int i;

        ext=hgGetExtKey();
        num=hgGetNumExtKey();

        hgWIDTH_Load(&mbars,xwidth-4,20*hgGetYFactor());
        hgBCOLOR_Load(&mbarc,wcBARNORMAL,wcBARNORMAL);
        mbarb=hgHBAR_Load(mbarc,mbars);

        hgWIDTH_Load(&mesgs,xwidth,ywidth);
        hgWCOLOR_Load(&mesgc,wcBOXNORMAL,ZERO,LIGHTGRAY,wcINSIDE);
        mesgb=hgHBOX_Load(items,mesgc,mesgs);

	while (strcmp(items[pos],"")) pos++;

	max=pos;
	pos=start;
        prvs=pos;        /* for no match */

        if (saveflag) hgHBOX_Save(mesgb,x1,y1);
        hgHBOX_Draw(mesgb,x1,y1);
        hgHideMouse();
        hgHBAR_Draw(mbarb,x1+2,y1+prvs*20*hgGetYFactor()+2,DRAW);
        hgShowMouse();

	while (1) {
                ch=inkey(NOWAIT);
                if (ch==MOUSE_LEFT) {
                        pos=get_mpos(x1+2,y1+2,xwidth-4,20*hgGetYFactor(),max);
                        if (pos>=0 && pos<max && !mousedrag) {
                                hgHideMouse();
                                hgHBAR_Draw(mbarb,x1+2,y1+prvs*20*hgGetYFactor()+2,ERASE);
                                hgHBAR_Draw(mbarb,x1+2,y1+pos*20*hgGetYFactor()+2,DRAW);
                                delay(100);
                                hgHBAR_Draw(mbarb,x1+2,y1+pos*20*hgGetYFactor()+2,ERASE);
                                prvs=pos;
                                break;
                        }
                        if (pos<0 || pos>=max) {
                                mousedrag=1;
                                pos=prvs;
                        }
                }
                if (ch!=MOUSE_LEFT && mousedrag) {
                        pos=get_mpos(x1+2,y1+2,xwidth-4,20*hgGetYFactor(),max);
                        if (pos>=0 && pos<max) {
                                if (prvs!=pos) {
                                        hgHideMouse();
                                        hgHBAR_Draw(mbarb,x1+2,y1+prvs*20*hgGetYFactor()+2,ERASE);
                                        hgHBAR_Draw(mbarb,x1+2,y1+pos*20*hgGetYFactor()+2,DRAW);
                                        delay(100);
                                        hgHBAR_Draw(mbarb,x1+2,y1+pos*20*hgGetYFactor()+2,ERASE);
                                        prvs=pos;
                                }
                                break;
                        } else  pos=prvs;
                        mousedrag=0;
                }

		if (ch==UP) pos--;
		if (ch==DOWN) pos++;
		if (pos<0) pos=max-1;
		if (pos>=max) pos=0;

                for (i=0;i<num;i++)
                        if (ext[i]==ch) {
                                pos=i;
                                hgHideMouse();
                                hgHBAR_Draw(mbarb,x1+2,y1+prvs*20*hgGetYFactor()+2,ERASE);
                                hgHBAR_Draw(mbarb,x1+2,y1+pos*20*hgGetYFactor()+2,DRAW);
                                delay(100);
                                prvs=pos;
                                ch=RETURN;      /* for break */
                        }

                if (prvs!=pos) {
                        hgHideMouse();
                        hgHBAR_Draw(mbarb,x1+2,y1+prvs*20*hgGetYFactor()+2,ERASE);
                        hgHBAR_Draw(mbarb,x1+2,y1+pos*20*hgGetYFactor()+2,DRAW);
                        hgShowMouse();
                        prvs=pos;
                }

                if (ch==MOUSE_RIGHT || ch==ESC || ch==RETURN) {
                        hgHideMouse();
                        hgHBAR_Draw(mbarb,x1+2,y1+prvs*20*hgGetYFactor()+2,ERASE);
                        hgShowMouse();
                        break;
                }
	};

        hgHideMouse();
        if (saveflag) hgHBOX_Restore(mesgb,x1,y1);
        hgShowMouse();

        hgHBOX_Free(&mesgb);
        hgHBAR_Free(&mbarb);

        if (ch==ESC || ch==MOUSE_RIGHT)  return -1;
        return pos;
}

int hgSelectXyQ(int x,int y,char *items[],int start)
{
	int flag;

	int max=ret_maxlength(items);
	int num=ret_number(items);

        num=num*20;
        num=num/16;

	flag=hgSelectXy(x,y,x+max+12,y+num+3,items,start);
	return flag;
}

int hgSelectXyM(char *items[],int start)
{
	int flag;

	int max=ret_maxlength(items);
	int num=ret_number(items);
        int xmid=(hgGetx2r()+hgGetx1r())/2;
        int ymid=(hgGety2r()+hgGety1r())/2;
        int xwidth=max+13;
        int ywidth;

        num=num*20;
        num=num/16;

        ywidth=num+4;

        flag=hgSelectXy(xmid-xwidth/2,ymid-ywidth/2,xmid+xwidth/2,ymid+ywidth/2,items,start);
	return flag;
}

