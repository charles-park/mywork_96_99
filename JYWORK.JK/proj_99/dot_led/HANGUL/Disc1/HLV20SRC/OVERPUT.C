/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              Over-Output               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <string.h>

#include "hghlib.h"

void erase_hangul(int x,int y,int color)
{
	char font[32]={ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff };

        puthan(x,y,font,color);
}

void erase_english(int x,int y,int color)
{
	char font[16]={ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff };

        puteng(x,y,font,color);
}

void hgOverTextXy(int x,int y,char *str)
{
	int i;
	int t;
	char temp[3];

        t=hgGetTBcolor();

	for (i=0;i<strlen(str);) {
		if (str[i]&0x80) {
			temp[0]=str[i];
			temp[1]=str[i+1];
			temp[2]=0;
			erase_hangul(x+i*8,y,t);
                        hgOutTextXy(x+i*8,y,temp);
			i+=2;
		}
		else if (str[i]=='\n') {
			x=0;
			y+=16;
			i++;
		}
		else if (str[i]=='\t') {
			x+=(8-(x/8)%8)*8;
			i++;
		}
		else {
			temp[0]=str[i];
			temp[1]=0;
			erase_english(x+i*8,y,t);
                        hgOutTextXy(x+i*8,y,temp);
			i++;
		}
	}
}

void hgFBTextXy(int x,int y,char *str,int fcolor,int scolor)
{
        int ft=hgGetTFcolor(),st=hgGetTBcolor();

        hgSetTFcolor(fcolor);
        hgSetTBcolor(scolor);

        hgOverTextXy(x,y,str);

        hgSetTFcolor(ft);
        hgSetTBcolor(st);
}
