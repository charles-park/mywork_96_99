/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Quick Icon                 */
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
#include <string.h>

#include "hghlib.h"
#include "hwindow.h"

/* global buff for icon bar */

CIMAGE *ibuff;

void hgPrtTextIconXy(HICON *p,int x,int y,char *str)
{
        int xpos;
        int ypos;

        hgHICON_Draw(p,x,y);

	xpos=x+p->size.xwidth*4-strlen(str)*4;
        ypos=y+p->size.ywidth+5;

        hgOutTextXy(xpos,ypos,str);
}
void hgPrtIconBarXy(HICON *p,int x,int y)
{
        int x1,x2;
        int y1,y2;

        x1=x-5;
        x2=x+p->size.xwidth*8+5;
        y1=y-5;
        y2=y+p->size.ywidth+5;

        hgDrawBorder(x1,y1,x2,y2,BARREVERSE,NOFILL);
}
void hgPrtIconBarSaveXy(HICON *p,int x,int y,int mode)
{
        int x1,x2;
        int y1,y2;

        x1=x-3;
        x2=x+p->size.xwidth*8+2;
        y1=y-3;
        y2=y+p->size.ywidth+2;

        if (mode==DRAW) {
                hgGetImage4(x1,y1,x2,y2,&ibuff);
                hgDrawBorder(x1,y1,x2,y2,BARREVERSE,NOFILL);
        } else {
                hgPutImage4(x1,y1,x2,y2,ibuff);
                hgFreeCIMAGE(&ibuff);
        }
}
