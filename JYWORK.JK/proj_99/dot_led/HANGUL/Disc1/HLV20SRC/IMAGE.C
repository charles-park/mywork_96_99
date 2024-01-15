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

#include <dos.h>
#include "hghlib.h"

void (*getimg)(int x1,int y1,int x2,int y2,char *buff);
void (*putimg)(int x1,int y1,int x2,int y2,char *buff);

void hgEnableImage(void (*g)(int x1,int y1,int x2,int y2,char *buff),
                   void (*p)(int x1,int y1,int x2,int y2,char *buff))
{
        getimg=g;
        putimg=p;
}

void hgGetImage(int x1,int y1,int x2,int y2,char *buff)
{
        (*getimg)(x1,y1,x2,y2,buff);
}
void hgPutImage(int x1,int y1,int x2,int y2,char *buff)
{
        (*putimg)(x1,y1,x2,y2,buff);
}
