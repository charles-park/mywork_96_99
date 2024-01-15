/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            ScrollBar Area              */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hginit.h"
#include "hwindow.h"

int hgHSCRLBAR_Area(HSCRLBAR *p,int x,int y)
{
        int x1,y1,x2,y2;
        int xx,yy;

        if (!hgGetMouse()) return hgFAIL;

        hgGetMousePos(&xx,&yy);

        x1=x;x2=x+p->size.xwidth;
        y1=y;y2=y+p->size.ywidth;

        if (xx>=x1 && xx<=x2 && yy>=y1 && yy<=y2) return hgSUCCESS;
        return hgFAIL;
}
