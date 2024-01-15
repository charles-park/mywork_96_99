/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           VGA-image routine            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#pragma inline

#include<alloc.h>
#include<dos.h>
#include "hghlib.h"
#include "hginit.h"

void hgVGetImage4(int x1,int y1,int x2,int y2,CIMAGE **p)
{
        int i;

        (*p)=(CIMAGE *)malloc(sizeof(CIMAGE));
        for (i=0;i<4;i++) {
                (*p)->buff[i]=(char *)malloc((size_t)hgImgSize4(x1,y1,x2,y2));
                hgVGetImagePlane(x1,y1,x2,y2,(*p)->buff[i],i);
        }
}
void hgVPutImage4(int x1,int y1,int x2,int y2,CIMAGE *p)
{
        int i;
        int k=1;

        for (i=0;i<4;i++) {
                hgVPutImagePlane(x1,y1,x2,y2,p->buff[i],k);
                k*=2;
        }
}
void hgHGetImage4(int x1,int y1,int x2,int y2,CIMAGE **p)
{
        (*p)=(CIMAGE *)malloc(sizeof(CIMAGE));
        (*p)->buff[0]=(char *)malloc((size_t)hgImgSize4(x1,y1,x2,y2));
        hgHGetImage(x1,y1,x2,y2,(*p)->buff[0]);
}
void hgHPutImage4(int x1,int y1,int x2,int y2,CIMAGE *p)
{
        hgHPutImage(x1,y1,x2,y2,p->buff[0]);
}
