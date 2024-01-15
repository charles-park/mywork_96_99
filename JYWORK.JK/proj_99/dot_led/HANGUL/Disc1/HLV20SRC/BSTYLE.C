/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Border Drawing              */
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
#include "hginit.h"
#include "hwindow.h"

void hgDrawBorder(int x1,int y1,int x2,int y2,int sty[4],int fill)
{
        int i;

        if (fill) hgBoxFill(x1,y1,x2,y2,sty[0]);

        for (i=0;i<sty[3];i++) {
                hgHline(x1,x2,y1,sty[1]);
                hgVline(x1,y1,y2,sty[1]);
                hgVline(x2,y1,y2,sty[2]);
                hgHline(x1,x2,y2,sty[2]);
                x1++;y1++;
                x2--;y2--;
        }
}
