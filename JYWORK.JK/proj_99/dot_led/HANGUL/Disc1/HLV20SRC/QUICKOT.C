/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Quick Output Init            */
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

void hgQuickOutInit()
{
        if (hgGetTextMode()==hgMonoText || hgIsHerc()) hgQuickHOutInit();
        else hgQuickVOutInit();
}


