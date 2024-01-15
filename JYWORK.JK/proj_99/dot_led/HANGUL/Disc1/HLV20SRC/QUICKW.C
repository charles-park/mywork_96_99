/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          Quick Window Routine          */
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
#include <alloc.h>

#include "hginit.h"
#include "hghlib.h"
#include "hgquick.h"
#include "hwindow.h"
#include "key.h"
#include "mouse.h"

#define MAXSTACK 50

int  wintitlefore=WHITE;
int  wintitleback=DARKGRAY;
int  winback=0;                         /* default style  */
int  winborder=2;                       /* default style  */

VIMAGE *gbuff[MAXSTACK];               /* global  buffer */
int  stacktop=0;                        /* first,empty    */
int  gx1[MAXSTACK],gy1[MAXSTACK];
int  gx2[MAXSTACK],gy2[MAXSTACK];
int  saveflag=0;                        /* default is off */

void savearea(int x1,int y1,int x2,int y2);

int ret_number(char *text[])
{
	int pos=0;

	while (strcmp(text[pos],"")) pos++;
        return pos*16*hgGetYFactor();
}
int ret_maxlength(char *text[])
{
	int pos=0;
	int length=-1;
	int slen;

	while (strcmp(text[pos],"")) {
		slen=strlen(text[pos]);
		if (slen>=length) length=slen;
		pos++;
	}
        return length*8*pw(2,hgGetXFactor()-1);
}
void hgPrtWindowXy(int x1,int y1,int x2,int y2,char *title)
{
	char *text[]={ "" };

	hgPrtWTextXy(x1,y1,x2,y2,title,text);
}
void hgPrtWTextXyQ(int x,int y,char *title,char *text[])
{
	int max=ret_maxlength(text);
	int num=ret_number(text);

        hgPrtWTextXy(x,y,x+max+32,y+num+54,title,text);
}
void hgRetMSize(char *text[],int *x1,int *y1,int *x2,int *y2)
{
        int max=ret_maxlength(text);
	int num=ret_number(text);
        int xmid=(hgGetx2r()+hgGetx1r())/2;
        int ymid=(hgGety2r()+20+hgGety1r())/2;
        int xwidth;
        int ywidth;

        xwidth=max+33;
        ywidth=num+55;

        *x1=xmid-xwidth/2;
        *y1=ymid-ywidth/2;
        *x2=xmid+xwidth/2;
        *y2=ymid+ywidth/2;
}
void hgPrtWTextXyM(char *title,char *text[])
{
	int max=ret_maxlength(text);
	int num=ret_number(text);
        int xmid=(hgGetx2r()+hgGetx1r())/2;
        int ymid=(hgGety2r()+20+hgGety1r())/2;
        int xwidth;
        int ywidth;

        xwidth=max+33;
        ywidth=num+55;

        hgPrtWTextXy(xmid-xwidth/2,ymid-ywidth/2,xmid+xwidth/2,ymid+ywidth/2,title,text);
}
void hgPrtWTextXy(int x1,int y1,int x2,int y2,char *title,char *text[])
{
	int xwidth=x2-x1+1;
	int ywidth=y2-y1+1;
	int ch;
        int pos=0;
        int xfac,yfac;

        WCOLOR   msgc;
        WIDTH    msgs;
        HWINDOW *msgw;

        xfac=hgGetXFactor();
        yfac=hgGetYFactor();
        hgSetXFactor(1);
        hgSetYFactor(1);

        hgWCOLOR_Load(&msgc,winborder,wintitlefore,wintitleback,winback);
        hgWIDTH_Load(&msgs,xwidth,ywidth);
        msgw=hgHWINDOW_Load(title,msgc,msgs);

        if (saveflag) savearea(x1,y1,x1+xwidth,y1+ywidth);

        hgHWINDOW_Draw(msgw,x1,y1);
        hgSetXFactor(xfac);
        hgSetYFactor(yfac);
        hgHideMouse();
	while (strcmp(text[pos],"")) {
                hgOutTextXy(x1+16,y1+38+pos*16*hgGetYFactor(),text[pos]);
		pos++;
	}
        hgShowMouse();
        hgHWINDOW_Free(&msgw);
}

void savearea(int x1,int y1,int x2,int y2)
{
        stacktop++;
        if (stacktop>=50) return;
        hgGetImageVIRTUAL(x1,y1,x2,y2,&gbuff[stacktop]);
        gx1[stacktop]=x1;gy1[stacktop]=y1;
        gx2[stacktop]=x2;gy2[stacktop]=y2;
}

void hgRestore()
{
        int j;

        hgHideMouse();
        if (stacktop==0) return;
        hgPutImageVIRTUAL(gx1[stacktop],gy1[stacktop],gx2[stacktop],gy2[stacktop],gbuff[stacktop]);
        hgFreeVIMAGE(&gbuff[stacktop]);
        stacktop--;
        hgShowMouse();
}

void hgDelete()
{
        hgFreeVIMAGE(&gbuff[stacktop]);
}
