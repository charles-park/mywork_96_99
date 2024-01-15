/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Ellipse Draw               */
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

void symmetry4(int xx,int yy,int x,int y,char color);

void hgEllipse(int xx,int yy,int a0,int b0,char color)
{
	int x=0;
	int y=b0;

	long a=a0;
	long b=b0;

	long a2=a*a;
	long a22=2*a2;
	long b2=b*b;
	long b22=2*b2;

	long d;
	long dx,dy;

	d=b2-a2*b+a2/4L;
	dx=0L;
	dy=a22*b;

	while (dx<dy){
		symmetry4(xx,yy,x,y,color);
		if (d>0) {
			y--;
			dy-=a22;
			d-=dy;
		}
		x++;
		dx+=b22;
		d+=b2+dx;
	}
	d+=(3*(a2-b2)/2L-(dx+dy))/2L;

	while (y>=0) {
		symmetry4(xx,yy,x,y,color);
		if (d<0) {
			x++;
			dx+=b22;
			d+=dx;
		}
		y--;
		dy-=a22;
		d+=a2-dy;
	}
}

void symmetry4(int xx,int yy,int x,int y,char color)
{
    hgPlotXy(xx-x,yy+y,color);
    hgPlotXy(xx+x,yy-y,color);
    hgPlotXy(xx-x,yy-y,color);
    hgPlotXy(xx+x,yy+y,color);
}
