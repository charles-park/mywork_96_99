/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Hangul Output               */
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

void hgForeTextXy(int x,int y,char *str,int fore)
{
        int fsav=hgGetTFcolor();

        hgSetTFcolor(fore);
        hgOutTextXy(x,y,str);
        hgSetTFcolor(fsav);
}
void hgBackTextXy(int x,int y,char *str,int back)
{
        int fore=hgGetTFcolor();

        hgFBTextXy(x,y,str,fore,back);
}
void hgSpecTextXy(int x,int y,char *str,int color1,int color2,int deep)
{
        hgForeTextXy(x+deep,y+deep,str,color2);
        hgForeTextXy(x,y,str,color1);
}
