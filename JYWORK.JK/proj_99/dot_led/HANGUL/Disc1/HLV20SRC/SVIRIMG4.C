/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          Virtual Image Routine         */
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

void hgVGetImageVIRTUAL256(int x1,int y1,int x2,int y2,VIMAGE **p)
{
        int i;
        int max=hgGetMaxPages();

        (*p)=(VIMAGE *)malloc(sizeof(VIMAGE));
        for (i=0;i<max;i++) {
                (*p)->buff[i]=(VMEM *)hgVMalloc((size_t)hgVImgSize256(x1,y1+i,x2,y2));
                hgVGetImageScan(x1,y1,x2,y2,vgbuff,i);
                hgVSaveBuff((*p)->buff[i]);
        }
}
void hgVPutImageVIRTUAL256(int x1,int y1,int x2,int y2,VIMAGE *p)
{
        int i;
        int max=hgGetMaxPages();

        for (i=0;i<max;i++) {
                hgVSetBuff(p->buff[i]);
                hgVPutImageScan(x1,y1,x2,y2,vgbuff,i);
        }
}
