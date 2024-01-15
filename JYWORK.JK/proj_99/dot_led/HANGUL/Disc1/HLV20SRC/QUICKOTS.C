/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Quick Out - 256             */
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

void hgQuickSOutInit()
{
        hgSetHanOut(&vputch256h);
        hgSetEngOut(&vputch256e);
        hgSetMagOut(&vputch256mn_out);
        hgSetCodeTable(SANGYONG);
        hgEnableAttr(fDIM);
        hgEnableAttr(fREVERSE);
        hgEnableAttr(fUNDER);
        hgEnableAttr(fOUTLINE);
        hgEnableAttr(fTHREED);
        hgEnableAttr(fBOLD);
        hgEnableAttr(fSHADOW);
}
