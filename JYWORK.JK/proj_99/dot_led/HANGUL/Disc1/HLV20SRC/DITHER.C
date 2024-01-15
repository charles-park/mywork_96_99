/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              Dithering                 */
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
#include "hwindow.h"

void dither_gray(HICON *p);
void dither_pattern(HICON *p);

void (*dit)(HICON *p)=&dither_gray;

void hgSetDither(void (*p)())
{
        dit=p;
}
void hgDoDither(HICON *p)
{
        (*dit)(p);
}

void dither_gray(HICON *p)
{
        char paltable[16]={ 0,4,24,29,12,17,24,42,21,25,45,50,33,38,58,63 };
        char data1,data2;
        int i;
        char clr;

        int  xs=p->size.xwidth*4;
        int  ys=p->size.ywidth;
        unsigned size=xs*ys;

        for (i=0;i<size;i++) {
                clr=p->buff[i];
                data1=(clr & 0xf0)>>4;
                data2=(clr & 0x0f);
                if (paltable[data1]>31) data1=WHITE;
                else data1=ZERO;
                if (paltable[data2]>31) data2=WHITE;
                else data2=ZERO;
                p->buff[i]=(data1<<4) + data2;
         }
}
void dither_pattern(HICON *p)
{
        char paltable[16]={ 0,4,24,29,12,17,24,42,21,25,45,50,33,38,58,63 };
        int dtable[8][8] = {
            { 0, 32,  8, 40,  2, 34, 10, 42},
            {48, 16, 56, 24, 50, 18, 58, 26},
            {12, 44,  4, 36, 14, 46,  6, 38},
            {60, 28, 52, 20, 62, 30, 54, 22},
            { 3, 35, 11, 43,  1, 33,  9, 41},
            {51, 19, 59, 27, 49, 17, 57, 25},
            {15, 47,  7, 39, 13, 45,  5, 37},
            {63, 31, 55, 23, 61, 29, 53, 21} };

        char data1,data2;
        int i,j,k,l,pos;
        char comp1,comp2,clr;

        int  xs=p->size.xwidth*4;
        int  ys=p->size.ywidth;

        for (i=0;i<ys;i+=8)
           for (j=0;j<xs;j+=4)
              for (k=0;k<8;k++)
                 for (l=0;l<4;l++) {
                     pos=get_pos(p,l+j,k+i);
                     clr=p->buff[pos];
                     comp1=dtable[k][l*2];
                     comp2=dtable[k][l*2+1];
                     data1=(clr & 0xf0)>>4;
                     data2=(clr & 0x0f);
                     if (paltable[data1]>comp1) data1=WHITE;
                     else data1=ZERO;
                     if (paltable[data2]>comp2) data2=WHITE;
                     else data2=ZERO;
                     p->buff[pos]=(data1<<4) + data2;
                 }
}
