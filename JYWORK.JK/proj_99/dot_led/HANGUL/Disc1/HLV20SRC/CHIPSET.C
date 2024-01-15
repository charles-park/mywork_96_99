/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*       Auto Setting of SVGA ChipSet     */
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

extern int iset4000(),iset3000(),isgenoa(),isparadise(),iswoder();
extern int isahead(),ispoach();

void hgAutoChipSet()
{
        
        if (isgenoa())    hgSetChipSet(GENOA);
        else if (isparadise()) hgSetChipSet(PARADISE);
        else if (iswonder())   hgSetChipSet(WONDER);
        else if (isahead())    hgSetChipSet(AHEAD);
        else if (ispoach())    hgSetChipSet(POACH);
        else if (iset4000())        hgSetChipSet(ET4000);
        else if (iset3000())   hgSetChipSet(ET3000);
        else {
                printf("\nError: incorrect SVGA chip occured.\n");
                printf("- made by Han Woo-jin. Hurumi Library 2.0.\n\n");
                exit(1);
        }
}
int hgGetChipSet()
{
        int current;

        if (isgenoa())    return CHIP_GENOA;
        else if (isparadise()) return CHIP_PARADISE;
        else if (iswonder())   return CHIP_WONDER;
        else if (isahead())    return CHIP_AHEAD;
        else if (ispoach())    return CHIP_POACH;
        else if (iset4000())        return CHIP_ET4000;
        else if (iset3000())   return CHIP_ET3000;

        return CHIP_UNKNOWN;
}
