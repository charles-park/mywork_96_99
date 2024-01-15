/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*    VGA-image routine in 256 color      */
/*               Module                   */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#pragma inline

#include <alloc.h>
#include <dos.h>

#include "hghlib.h"

void hgVGetImage256(int x1,int y1,int x2,int y2,CIMAGE **p)
{
        int i;
        int max=hgGetMaxPages();

        (*p)=(CIMAGE *)malloc(sizeof(CIMAGE));
        for (i=0;i<max;i++) {
                (*p)->buff[i]=(char *)malloc((size_t)hgVImgSize256(x1,y1+i,x2,y2));
                hgVGetImageScan(x1,y1,x2,y2,(*p)->buff[i],i);
        }
}
void hgVPutImage256(int x1,int y1,int x2,int y2,CIMAGE *p)
{
        int i;
        int max=hgGetMaxPages();

        for (i=0;i<max;i++)
                hgVPutImageScan(x1,y1,x2,y2,p->buff[i],i);
}
