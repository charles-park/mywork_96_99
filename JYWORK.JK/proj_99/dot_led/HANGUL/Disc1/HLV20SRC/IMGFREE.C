/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             IMAGE FREE                 */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <alloc.h>

#include "hghlib.h"
#include "hwindow.h"

int maxpages=4;

void hgSetMaxPages(int num)
{
        maxpages=num;
}
int  hgGetMaxPages()
{
        return maxpages;
}
void hgFreeCIMAGE(CIMAGE **p)
{
        int i;

        for (i=0;i<maxpages;i++)
                free((*p)->buff[i]);
        free(*p);
}
void hgFreeVIMAGE(VIMAGE **p)
{
        int i;
        int max;

        for (i=0;i<maxpages;i++)
                hgVFree(&((*p)->buff[i]));
        free((*p));
}
