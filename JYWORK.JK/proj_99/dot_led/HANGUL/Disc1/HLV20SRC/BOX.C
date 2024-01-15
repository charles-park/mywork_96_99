/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*               Box Draw                 */
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

void hgBox(int x1,int y1,int x2,int y2,int color)
{
        hgHline(x1,x2,y1,color);
        hgHline(x1,x2,y2,color);
        hgVline(x1,y1,y2,color);
        hgVline(x2,y1,y2,color);
}
