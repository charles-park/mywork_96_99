/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            VGA-graph-init              */
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

void hgQuickVGraphInit()
{
        hgEnableBoxFill(&hgVBoxFill);
        hgEnableImage4(&hgVGetImage4,&hgVPutImage4);
        hgEnableImageVIRTUAL(&hgVGetImageVIRTUAL,&hgVPutImageVIRTUAL);
        hgEnableImgSize4(&hgVImgSize4);
	hgEnableHVline(&hgVhline,&hgVvline);
        hgEnableScrUp(&hgVScrUp);
        hgEnableScrDown(&hgVScrDown);
        hgEnableLine(&hgVLineG);
        hgEnablePlot(&hgVPlotXy);
}
