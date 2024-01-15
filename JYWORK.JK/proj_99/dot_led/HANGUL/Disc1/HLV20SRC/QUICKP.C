/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        "Quick Windowed Pulldown "      */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hwindow.h"

void hgPrtPullWindowXy(HPULLDOWN *p,char *title,int x,int y1,int y2)
{
        int pxsize=p->size.xwidth;

        hgPrtWindowXy(x-12,y1,x+pxsize+12,y2,title);
}
void hgPrtPullDownXy(HPULLDOWN *p,int x,int y,int start,int *xx,int *yy)
{
        hgHPULLDOWN_Choose(p,x,y+32,start,xx,yy);
}
