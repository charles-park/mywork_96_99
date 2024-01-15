/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Herc-Graph-Init             */
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
#include "hwindow.h"

void hgQuickHGraphInit()
{
        hgSetHFillDPattern();
        hgSetHVlineDefault();
        hgEnableBoxFill(&hgHBoxFill);
        hgEnableImage4(&hgHGetImage4,&hgHPutImage4);
        hgEnableImageVIRTUAL(&hgHGetImageVIRTUAL,&hgHPutImageVIRTUAL);
        hgEnableImgSize4(&hgHImgSize4);
        hgEnableHVline(&hgHhline,&hgHvline);
        hgEnableScrUp(&hgHScrUp);
        hgEnableScrDown(&hgHScrDown);
        hgEnableLine(&hgHLineG);
        hgEnablePlot(&hgHPlotXy);
}
