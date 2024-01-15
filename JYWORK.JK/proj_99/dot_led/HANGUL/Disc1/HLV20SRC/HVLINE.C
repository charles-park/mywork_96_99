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

#include "hginit.h"

void (*hline)(int x1,int x2,int y,char color);
void (*vline)(int x,int y1,int y2,char color);

void hgEnableHVline(void (*h)(int x1,int x2,int y,char color),
                    void (*v)(int x,int y1,int y2,char color))
{
        hline=h;
        vline=v;
}

void hgHline(int x1,int x2,int y,char color)
{
        (*hline)(x1,x2,y,color);
}
void hgVline(int x,int y1,int y2,char color)
{
        (*vline)(x,y1,y2,color);
}
void hgDHline(int x1,int x2,int y)
{
        (*hline)(x1,x2,y,DARKGRAY);
        (*hline)(x1,x2,y+1,WHITE);
}
void hgDVline(int x,int y1,int y2)
{
        (*vline)(x,y1,y2,WHITE);
        (*vline)(x+1,y1,y2,DARKGRAY);
}
