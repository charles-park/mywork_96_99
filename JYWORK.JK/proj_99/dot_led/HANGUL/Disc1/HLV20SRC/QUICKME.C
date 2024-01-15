/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              Mouse End                 */
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
#include "mouse.h"

void hgSetMouseEnd()
{
        if (hgIfClose()) {
                hgHideMouse();
                end_mouse();
        }
}
