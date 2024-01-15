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

#include "hginit.h"
#include "hghlib.h"

int  hgDetectCard()
{
        if (hgGetTextMode()==7) return hgHERCULES;
        if (hgIsEGAVGA() && !hgIsVGA()) return hgEGA;
        if (hgIsEGAVGA())
                if (hgGetChipSet()!=CHIP_UNKNOWN) return hgSVGA;
                else return hgVGA;
        else return hgUNKNOWN;
}
