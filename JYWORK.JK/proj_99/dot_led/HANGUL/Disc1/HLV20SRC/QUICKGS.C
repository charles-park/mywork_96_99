/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            256-graph-init              */
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

void hgQuickSGraphInit()
{
        hgEnableBoxFill(&hgVBoxFill256);
        hgEnableImage4(&hgVGetImage256,&hgVPutImage256);
        hgEnableImageVIRTUAL(&hgVGetImageVIRTUAL256,&hgVPutImageVIRTUAL256);
        hgEnableImgSize4(&hgVImgSize256);
        hgEnableHVline(&hgVhline256,&hgVvline256);
        hgEnableScrUp(&hgVScrUp256);
        hgEnableScrDown(&hgVScrDown256);
        hgEnableLine(&hgVLine256);
        hgEnablePlot(&hgVPlotXy256);
}
