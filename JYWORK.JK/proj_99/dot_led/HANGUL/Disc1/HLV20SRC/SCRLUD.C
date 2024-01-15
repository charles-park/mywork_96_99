/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Scroll Up/Down              */
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

void (*scrlu)(int x1,int y1,int x2,int y2,int lines,int color);
void (*scrld)(int x1,int y1,int x2,int y2,int lines,int color);

void hgEnableScrUp(void (*b)(int x1,int y1,int x2,int y2,int lines,int color))
{
        scrlu=b;
}

void hgScrUp(int x1,int y1,int x2,int y2,int lines,int color)
{
        (*scrlu)(x1,y1,x2,y2,lines,color);
}

void hgEnableScrDown(void (*b)(int x1,int y1,int x2,int y2,int lines,int color))
{
        scrld=b;
}

void hgScrDown(int x1,int y1,int x2,int y2,int lines,int color)
{
        (*scrld)(x1,y1,x2,y2,lines,color);
}
