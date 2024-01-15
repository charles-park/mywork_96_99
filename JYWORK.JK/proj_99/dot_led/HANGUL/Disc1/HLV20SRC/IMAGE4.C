/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Image4 Routine              */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <dos.h>
#include "hghlib.h"

void (*getimg4)(int x1,int y1,int x2,int y2,CIMAGE **p);
void (*putimg4)(int x1,int y1,int x2,int y2,CIMAGE *p);

void hgEnableImage4(void (*g)(int x1,int y1,int x2,int y2,CIMAGE **p),
                   void (*p)(int x1,int y1,int x2,int y2,CIMAGE *p))
{
        getimg4=g;
        putimg4=p;
}

void hgGetImage4(int x1,int y1,int x2,int y2,CIMAGE **p)
{
        (*getimg4)(x1,y1,x2,y2,p);
}
void hgPutImage4(int x1,int y1,int x2,int y2,CIMAGE *p)
{
        (*putimg4)(x1,y1,x2,y2,p);
}
