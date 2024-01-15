/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Scroll Left/Right            */
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

void (*scrll)(int x1,int y1,int x2,int y2,int lines,int color);
void (*scrlr)(int x1,int y1,int x2,int y2,int lines,int color);

void hgEnableScrLeft(void (*b)(int x1,int y1,int x2,int y2,int lines,int color))
{
        scrll=b;
}

void hgScrLeft(int x1,int y1,int x2,int y2,int lines,int color)
{
        (*scrll)(x1,y1,x2,y2,lines,color);
}

void hgEnableScrRight(void (*b)(int x1,int y1,int x2,int y2,int lines,int color))
{
        scrlr=b;
}

void hgScrRight(int x1,int y1,int x2,int y2,int lines,int color)
{
        (*scrlr)(x1,y1,x2,y2,lines,color);
}

