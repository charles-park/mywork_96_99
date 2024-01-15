/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          Hanja Input Module            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hwindow.h"
#include "hghlib.h"

#include <alloc.h>

/* extern varible from hanin.c */

extern char    *inbuff;
extern int      pos;
extern int      in_max;
extern int      Xpos,Ypos;
extern int      is_complete();
extern void     complete_2();
extern char     hanjaokflag;
extern int      ishanja(char *str,int pos);

/* hanjatable in CodeSegment */

extern unsigned far hanjatable[473][3];

/* prototype */

void hanjain_process();
int  choose_hanja();

/* byte define */

void hgHanjaToHangulSub(byte h1,byte h2,byte *hh1,byte *hh2)
{
        unsigned off;
        int      i;

        if ((h1>=0xe0 && h1<=0xfa) &&
           ( (h2>=0x31 && h2<=0x7e) ||
             (h2>=0x91 && h2<=0xfe) ) )
        {
            if (h2>0x90) h2-=0x12;
            h2-=0x31;
            off=((h1-0xe0)*188+h2);
        } else {
                *hh1=h1;
                *hh2=h2;
                return;
        }
        for (i=0;i<473;i++)
                if (hanjatable[i][2]>off) break;
        i--;
        *hh1=hanjatable[i][0]/256;
        *hh2=hanjatable[i][0]%256;
}

void hgHanjaToHangul(char *src,char *dest)
{
        int i;
        byte h1,h2;

        for (i=0;i<strlen(src);) {
                if (ishanja(src,i)) {
                        hgHanjaToHangulSub(src[i],src[i+1],&h1,&h2);
                        dest[i]=h1;
                        dest[i+1]=h2;
                        i+=2;
                } else  {
                        dest[i]=src[i];
                        i++;
                }
        }
        dest[i]=0;      /* for make string */
}

void hgHanjaInInit(int keyvalue)
{
        if (!hanjaokflag) return;
        hgRegisterKeyFunction(keyvalue,&hanjain_process);
}

void hanjain_process()
{
        unsigned h1,h2;
        unsigned comp;
        int      i;
        int      findflag=0;
        unsigned hc1,hc2;
        unsigned hanjacode;
        int      retflag;
        char    *items[_MAXDIR];
        char     ret[10];
        int      x,y;
        HDIRBOX *p;
        WIDTH    w={ 280,178 };
        byte     hb1,hb2;

        x=(hgGetx2r()+hgGetx1r())/2-w.xwidth/2;
        y=(hgGety2r()+hgGety1r())/2-w.ywidth/2;

	h1=(unsigned)(unsigned char)inbuff[pos];
	h2=(unsigned)(unsigned char )inbuff[pos+1];

	comp=h1*256+h2;

	for (i=0;i<473;i++)
		if (hanjatable[i][0]==comp) {
			findflag=1;
			break;
		}
	if (!findflag) {
		if (ishanja(inbuff,pos)) {
			hgHanjaToHangulSub(inbuff[pos],inbuff[pos+1],&hb1,&hb2);
			inbuff[pos]=hb1;
			inbuff[pos+1]=hb2;

			ret[0]=hb1;
			ret[1]=hb2;
			ret[2]=0;

			hgOverTextXy(Xpos+pos*8,Ypos,ret);
			pos+=2;
			if (pos>=in_max) pos=in_max-2;
		}
		return;
	}
	for (retflag=0;retflag<hanjatable[i][1];retflag++) {
                hanjacode=hanjatable[i][2]+retflag;
                hc1=(( hanjacode/188 ) + 0xe0);
                hc2=( hanjacode % 188);
                if (hc2 < 78) hc2+=0x31;
                else
                    hc2=hc2-78+0x91;
                items[retflag]=(char *)malloc(3);
                items[retflag][0]=(unsigned char) hc1;
                items[retflag][1]=(unsigned char) hc2;
                items[retflag][2]=0;
        }
        items[retflag]=(char *)malloc(3);

        items[retflag][0]=0;
        items[retflag][1]=0;
        items[retflag][2]=0;

        hgMouseInFlagOn();

        hgSetSaveOn();
        hgHercBarOn();
        p=hgHDIRBOX_Load(items,w,8,6);
        hgHDIRBOX_Choose(p,x,y,"Ðe¸a·³b",ret);
        hgHDIRBOX_Free(&p);
        hgHercBarOff();
        if (!is_complete()) {
                complete_2();
                pos-=2;
        }

        hgSetSaveOff();
        hgRestore();
        hgHideMouse();

        if (!strcmp(ret,"")) {
                pos+=2;
                if (pos>=in_max) pos=in_max-2;
                return;
        }

        inbuff[pos]=ret[0];
        inbuff[pos+1]=ret[1];

        for (retflag=0;retflag<=hanjatable[i][1];retflag++)
                free(items[retflag]);

        hgOverTextXy(Xpos+pos*8,Ypos,ret);
        pos+=2;
        if (pos>=in_max) pos=in_max-2;

        hgHideMouse();
        hgMouseInFlagOff();
}
