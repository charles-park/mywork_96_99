/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           "Magnify Output"             */
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

extern char engflag;     /* is english process ? */

void putchmn(int x,int y,char *buff,char color)
{
        int m=hgGetXFactor();
        int n=hgGetYFactor();
        char eng=engflag;
	int xx;

        char *mbuff;             /* magnified character buffer1 */
        char *nbuff;             /*            ""             2 */

        int i;
        int fac=1;
        int eh=1;

        if (!eng) eh=2;

        for (i=1;i<m;i++)        /* cal. x factor */
                fac*=2;

        if (!eng) fac*=2;

        mbuff=(char *)malloc((size_t)16*fac);
        nbuff=(char *)malloc((size_t)16*fac*n);

        if (m!=1) widen(buff,mbuff,16*eh);
        else      memcpy(mbuff,buff,16*eh);
        if (n!=1) lengthen(mbuff,nbuff,fac,16);
        else      memcpy(nbuff,mbuff,16*fac);

        putmag(x,y,nbuff,fac,n<<4,color);

	xx=hgGetOutX();

        xx-=( (eng) ? 8 : 16 );        /* control */
        xx+=fac*8;
	hgGotoXy(xx,hgGetOutY());

        free(mbuff);
        free(nbuff);
}
