/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Quick Graph Init            */
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

void hgQuickGraphInit()
{
        if (hgGetTextMode()==hgMonoText || hgIsHerc()) hgQuickHGraphInit();
        else hgQuickVGraphInit();
}
