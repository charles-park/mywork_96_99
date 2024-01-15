/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Quick Box module            */
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
#include "hwindow.h"
#include "hgquick.h"
#include "key.h"

extern int  saveflag;
extern void savearea(int x1,int y1,int x2,int y2);
extern int ret_number(char *text[]);
extern int ret_maxlength(char *text[]);

/* global buff for bar */

CIMAGE *bbuff;

void hgPrtBoxXy(int x1,int y1,int x2,int y2,char *items[])
{
        WCOLOR boxc;
        WIDTH  boxw;
        HBOX  *boxb;

        hgWCOLOR_Load(&boxc,wcBOXNORMAL,ZERO,LIGHTGRAY,wcINSIDE);
        hgWIDTH_Load(&boxw,x2-x1+1,y2-y1+1);
        boxb=hgHBOX_Load(items,boxc,boxw);

        if (saveflag) savearea(x1,y1,x2+1,y2+1);

        hgHBOX_Draw(boxb,x1,y1);
        hgHBOX_Free(&boxb);
}
void hgPrtBoxXyQ(int x,int y,char *items[])
{
        int max=ret_maxlength(items);
        int num=ret_number(items);

        num*=20;
        num/=16;

        hgPrtBoxXy(x,y,x+max+12,y+num+3,items);
}
void hgPrtBoxXyM(char *items[])
{
        int max=ret_maxlength(items);
        int num=ret_number(items);
        int xw,yw;
        int xm=(hgGetx2r()+hgGetx1r())/2;
        int ym=(hgGety2r()+hgGety1r())/2;

        num*=20;
        num/=16;

        xw=max+13;
        yw=num+4;

        hgPrtBoxXy(xm-xw/2,ym-yw/2,xm+xw/2,ym+yw/2,items);
}
void hgPrtBarXy(int x1,int x2,int y)
{
        hgDrawBorder(x1,y,x2,y+20,BARNORMAL,NOFILL);
}
void hgPrtBarSaveXy(int x1,int x2,int y,int mode)
{
        int j;

        if (mode==DRAW) {
                hgGetImage4(x1,y,x2,y+20,&bbuff);
                hgDrawBorder(x1,y,x2,y+20,BARNORMAL,NOFILL);
        } else {
                hgPutImage4(x1,y,x2,y+20,bbuff);
                hgFreeCIMAGE(&bbuff);
        }
}
