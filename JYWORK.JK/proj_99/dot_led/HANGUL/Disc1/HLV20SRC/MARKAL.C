/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*                 Mark                   */
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
#include "hwindow.h"
#include "hgicon.h"

extern HICON *mark;
extern HICON *mend;
extern HICON *markr;
extern HICON *mendr;

void hgMarkInit()
{
        mark=hgHICON_Set(WINCLOSE_ICON);
        mend=hgHICON_Set(WINEND_ICON);
        markr=hgHICON_Set(WINCLOSER_ICON);
        mendr=hgHICON_Set(WINENDR_ICON);

        hgMarkOn();
}
