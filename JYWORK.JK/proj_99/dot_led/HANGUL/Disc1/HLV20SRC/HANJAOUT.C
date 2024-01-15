/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Hanja Output               */
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

void hanja_real_out(char *str,int pos);
extern void (*hjps)(char *str,int pos);
extern int _xpos,_ypos;                     /* for output */
extern void attribute_process(char *dest,int bytes);

/* for hanja queue routine */

ExtFont *hanja;
extern char hanjaokflag;

void hgHanjaInit(char *fname,int qsize)
{
        if (qsize>500) qsize=500;
        hanja=hgOpenExtFont(fname,2,16,qsize);
        if (hanja!=NULL) {
                hjps=&hanja_real_out;
                hanjaokflag=1;
        }
}

void hanja_real_out(char *str,int pos)
{
        unsigned char hc1,hc2;
        unsigned int off;
        char     sattr;
        int      i;
        int      fac=1;

        hc1=str[pos];
        hc2=str[pos+1];

        if ((hc1>=0xe0 && hc1<=0xfa) &&
           ( (hc2>=0x31 && hc2<=0x7e) ||
             (hc2>=0x91 && hc2<=0xfe) ) )
        {
	    if (hc2>0x90) hc2-=0x12;
	    hc2-=0x31;
            off=((hc1-0xe0)*188+hc2);
        }
        sattr=hgGetExtAttr();
        hgSetExtAttr(hgGetHAttr());
        hgPutExtFont(hanja,_xpos,_ypos,off);
        hgSetExtAttr(sattr);

        for (i=1;i<hgGetXFactor();i++)
                fac*=2;

        _xpos+=(16*fac);
}
void hgHanjaEnd()
{
        if (hanjaokflag) hgCloseExtFont(hanja);
}
