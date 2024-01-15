/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Quick Input                */
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

#include "hgquick.h"
#include "hghlib.h"

int  hgInWTextXy(int x,int y,char *title,char *str,int n)
{
        int y2,x2;
        int flag;

        x2=x+n*8+40;    /* 40 is adding margin */
        y2=y+16+58;

        hgPrtWindowXy(x,y,x2,y2,title);
        flag=hgInTextXy(x+20,y+40,str,n);

        return flag;
}
int  hgInWTextXyM(char *title,char *str,int n)
{
        int y2,x2;
        int flag;
        int xmid=(hgGetx1r()+hgGetx2r())/2;
        int ymid=(hgGety1r()+hgGety2r())/2;
        int xwidth=n*8+41;
        int ywidth=58+16;

        hgPrtWindowXy(xmid-xwidth/2,ymid-ywidth/2,xmid+xwidth/2,ymid+ywidth/2,title);

        flag=hgInTextXy(xmid-xwidth/2+20,ymid-ywidth/2+40,str,n);

        return flag;
}
