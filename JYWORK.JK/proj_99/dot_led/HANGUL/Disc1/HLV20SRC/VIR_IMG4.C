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

void hgVGetImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE **p)
{
        int i;

        (*p)=(VIMAGE *)malloc(sizeof(VIMAGE));
        for (i=0;i<4;i++) {
                (*p)->buff[i]=(VMEM *)hgVMalloc((size_t)hgImgSize4(x1,y1,x2,y2));
                hgVGetImagePlane(x1,y1,x2,y2,vgbuff,i);
                hgVSaveBuff((*p)->buff[i]);
        }
}
void hgVPutImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE *p)
{
        int i;
        int k=1;

        for (i=0;i<4;i++) {
                hgVSetBuff(p->buff[i]);
                hgVPutImagePlane(x1,y1,x2,y2,vgbuff,k);
                k*=2;
        }
}
void hgHGetImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE **p)
{
        (*p)=(VIMAGE *)malloc(sizeof(VIMAGE));
        (*p)->buff[0]=(VMEM *)hgVMalloc((size_t)hgImgSize4(x1,y1,x2,y2));
        hgHGetImage(x1,y1,x2,y2,vgbuff);
        hgVSaveBuff((*p)->buff[0]);
}
void hgHPutImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE *p)
{
        hgVSetBuff(p->buff[0]);
        hgHPutImage(x1,y1,x2,y2,vgbuff);
}
