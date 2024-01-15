/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*   VGA 256 color calculate imagesize    */
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

unsigned hgVImgSize256(int x1,int y1,int x2,int y2)
{
        long size;
        int  max;

        max=hgGetMaxPages();

        size=(long)(y2-y1+1L+(long)(max-1))/((long)max);
        size=size*(long)(x2-x1+1L);
        return (unsigned)size;
}
