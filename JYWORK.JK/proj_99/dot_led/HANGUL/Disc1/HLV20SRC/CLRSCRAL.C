/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              Clear All                 */
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

void (*clr)();

void hgEnableClearScreen(void (*p)())
{
        clr=p;
}
void hgClearScreen()
{
        (*clr)();
}
