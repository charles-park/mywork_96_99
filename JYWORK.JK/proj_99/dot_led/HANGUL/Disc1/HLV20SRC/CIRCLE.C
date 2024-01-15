/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Circle Draw                */
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

void symmetry8(int xx,int yy,int x,int y,char color);

void hgCircle(int xx,int yy,int radius,char color)
{
	int y=radius;
	int d=3-y*2;
	int x=0;

	while (x<y) {
        hgPlotXy(xx+x,yy+y,color);
                symmetry8(xx,yy,x,y,color);
		if (d<0) d+=(x*4+6);
		else {
			d+=((x-y)*4+10);
			y--;
		}
		x++;
	}
        if (x==y) hgPlotXy(xx+x,yy+y,color);
}

void symmetry8(int xx,int yy,int x,int y,char color)
{
    hgPlotXy(xx-x,yy+y,color);
    hgPlotXy(xx+x,yy-y,color);
    hgPlotXy(xx-x,yy-y,color);
    hgPlotXy(xx-y,yy+x,color);
    hgPlotXy(xx+y,yy-x,color);
    hgPlotXy(xx-y,yy-x,color);
    hgPlotXy(xx+y,yy+x,color);
}
