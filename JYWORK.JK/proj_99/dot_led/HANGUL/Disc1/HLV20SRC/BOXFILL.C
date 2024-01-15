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

void (*boxfl)(int x1,int y1,int x2,int y2,int color);

void hgEnableBoxFill(void (*b)(int x1,int y1,int x2,int y2,int color))
{
        boxfl=b;
}

void hgBoxFill(int x1,int y1,int x2,int y2,int color)
{
        (*boxfl)(x1,y1,x2,y2,color);
}
