/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Palette PCX                */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <stdio.h>

#include "hginit.h"
#include "hghlib.h"
#include "hgpcx.h"

int  palettepcx=hgSUCCESS;              /* palette option */

void hgPCXpaletteOn()
{
        palettepcx=hgSUCCESS;
}
void hgPCXpaletteOff()
{
        palettepcx=hgFAIL;
}
void hgPCXpaletteSet(char *fname)
{
        PCXHDR p;
        FILE  *fpt;

        int    i;
        int    type;
        int    r,g,b;

        hgGetPCXHdr(&p,fname);
        type=hgGetPCXMode(fname);

        switch(type) {
                case PCX_VGA16:
                        for (i=0;i<16;i++)
                                hgSetRGBpalette(i,p.Colormap[i][0]>>2,p.Colormap[i][1]>>2,p.Colormap[i][2]>>2);
                        break;
                case PCX_VGA256:
                        fpt=fopen(fname,"rb");
                        if (fpt==NULL) return;
                        fseek(fpt,-768L,2);
                        for (i=0;i<256;i++) {
                                r=fgetc(fpt)>>2;
                                g=fgetc(fpt)>>2;
                                b=fgetc(fpt)>>2;
                                hgSetRGBpalette256(i,r,g,b);
                        }
                        fclose(fpt);
                        break;
        };
}
