/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Palette Adjust              */
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
#include "hginit.h"

char savepal[256][3];

#define ABS(x) ( ((x)>0) ? (x): -(x) )

void hgAdjustPalette256()
{
        int i,j;
        int r,g,b;
        int diff;
        int min;

        char spal[16][3]={ {  0, 0, 0 },{  0, 0,42 },{  0,42, 0 },{  0,42,42 },
                           { 42, 0, 0 },{ 42, 0,42 },{ 42,21, 0 },{ 42,42,42 },
                           { 21,21,21 },{ 21,21,63 },{ 21,63,21 },{ 21,63,63 },
                           { 63,21,21 },{ 63,21,63 },{ 63,63,21 },{ 63,63,63 } };

        if (hgGetMaxColors()!=256) return;

        for (i=0;i<16;i++) {
                min=1000;
                c256[i]=i;
                for (j=0;j<256;j++) {
                        hgGetRGBpalette256(j,&r,&g,&b);
                        diff=ABS(r-spal[i][0])+ABS(g-spal[i][1])+ABS(b-spal[i][2]);
                        if (min>diff) min=diff,c256[i]=j;
                }
        }
}

void hgSavePalette256()
{
        int i;
        int r,g,b;

        for (i=0;i<256;i++) {
                hgGetRGBpalette256(i,&r,&g,&b);
                savepal[i][0]=r;
                savepal[i][1]=g;
                savepal[i][2]=b;
        }
}

void hgRestorePalette256()
{
        int i;

        for (i=0;i<256;i++)
                hgSetRGBpalette256(i,savepal[i][0],savepal[i][1],savepal[i][2]);
}
