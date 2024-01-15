/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Virtual Image              */
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

void (*vgetimg4)(int x1,int y1,int x2,int y2,VIMAGE **p);
void (*vputimg4)(int x1,int y1,int x2,int y2,VIMAGE *p);

void hgEnableImageVIRTUAL(void (*g)(int x1,int y1,int x2,int y2,VIMAGE **p),
                   void (*p)(int x1,int y1,int x2,int y2,VIMAGE *p))
{
        vgetimg4=g;
        vputimg4=p;
}

void hgGetImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE **p)
{
        (*vgetimg4)(x1,y1,x2,y2,p);
}
void hgPutImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE *p)
{
        (*vputimg4)(x1,y1,x2,y2,p);
}
