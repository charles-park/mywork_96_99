/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Graphic Output              */
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

void graph_real_out(char *str,int pos);
extern void (*grps)(char *str,int pos);
extern int _xpos,_ypos;                     /* for output */

/* for hanja queue routine */

ExtFont *graph;
extern char graphokflag;

void hgGraphicCharInit(char *fname,int qsize)
{
        if (qsize>500) qsize=500;
        graph=hgOpenExtFont(fname,2,16,qsize);
        if (graph!=NULL) {
                grps=&graph_real_out;
                graphokflag=1;
        }
}

void graph_real_out(char *str,int pos)
{
        unsigned fontID[]={ 0xd480,0xd931,0xda31,0xdb31,0xdc31,0xdd31,0xde31 };
        unsigned addoff[]={ 125,162,188,162,188,177,175 };
        unsigned sblank[]={   1,  1,  0,  0,  0,  0,  0 };
        unsigned char hc1,hc2,hc3;
        unsigned off;
        char     sattr;
        int      i,j;
        char     temp[5];
        int      fac=1;

        for (i=0;i<7;i++) {
                hc1=(fontID[i]>>8) & 0xff;
                hc2=(fontID[i] & 0xff);
                if ((unsigned char)str[pos]==hc1 && (unsigned char)str[pos+1]>=hc2)
                        break;
        }
        hc3=(unsigned char)str[pos+1];
        off=(unsigned)hc3-(unsigned)hc2;

        if (off<sblank[i] || (hc3>=0x78 && hc3<=0x89) || hc3>=(addoff[i]+0x31+18+sblank[i]))
                 off=486;    /* special process */
        if (hc1==0xd4) {
                off=(unsigned)hc3-(unsigned)hc2;
                if (off>addoff[i] || off==0) off=486;
        }
        if (off!=486) {
                if (hc1!=0xd4 && hc3>=0x8a) off-=18;
                off-=sblank[i];
                for (j=0;j<i;j++)
                        off+=addoff[j];
        }

        sattr=hgGetExtAttr();
        hgSetExtAttr(hgGetHAttr());
        hgPutExtFont(graph,_xpos,_ypos,off);
        hgSetExtAttr(sattr);

        for (i=1;i<hgGetXFactor();i++)
                fac*=2;

        _xpos+=(16*fac);
}
void hgGraphicCharEnd()
{
        if (graphokflag) hgCloseExtFont(graph);
}

