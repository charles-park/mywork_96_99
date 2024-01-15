/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Quick Button               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#define  MAXBUTTON 10

#include <string.h>
#include <alloc.h>

#include "hginit.h"
#include "hghlib.h"
#include "hgquick.h"
#include "hwindow.h"
#include "key.h"
#include "mouse.h"

extern int  saveflag;
extern int dft_bstyle[MAXSTYLE][4];
extern void savearea(int x1,int y1,int x2,int y2);
extern int ret_number(char *text[]);
extern int ret_maxlength(char *text[]);

void hgPrtButtonXy(int x,int y,char *text,int mode)
{
        WCOLOR   btc;
        WIDTH    btw;
        HBUTTON *btb;

        int max=strlen(text)*pw(2,hgGetXFactor()-1);

        hgWCOLOR_Load(&btc,wcBARNORMAL,ZERO,LIGHTGRAY,wcBARREVERSE);
        hgWIDTH_Load(&btw,max*8+10,16*hgGetYFactor()+4);
        btb=hgHBUTTON_Load(text,btc,btw);
        hgHBUTTON_Draw(btb,x,y,mode);
        hgHBUTTON_Free(&btb);
}
void hgPrtButtonXyM(char *text,int mode)
{
        WCOLOR   btc;
        WIDTH    btw;
        HBUTTON *btb;

        int xmid=(hgGetx2r()+hgGetx1r())/2;
        int ymid=(hgGety2r()+hgGety1r())/2;

        int max=strlen(text)*pw(2,hgGetXFactor()-1);

        hgWCOLOR_Load(&btc,wcBARNORMAL,ZERO,LIGHTGRAY,wcBARREVERSE);
        hgWIDTH_Load(&btw,max*8+10,16*hgGetYFactor()+4);
        btb=hgHBUTTON_Load(text,btc,btw);

        hgHBUTTON_Draw(btb,xmid-max*4-5,ymid-8*hgGetYFactor()-2,mode);
        hgHBUTTON_Free(&btb);
}
int hgPrtButtonBarXy(int x1,int y1,int x2,int y2,char *msg[],char *items[])
{
        int fore,back;
        int pos=y1+10;
        int ch;
	int length;
        CIMAGE *buff;
        int num,i,j,prvs,prvi;
        int xincre,xpos;
        int x[MAXBUTTON];
        int xw[MAXBUTTON];

        hgHideMouse();

        if (saveflag) savearea(x1,y1,x2,y2);

        hgDrawBorder(x1,y1,x2,y2,dft_bstyle[2],FILL);
        hgDrawBorder(x1+4,y1+4,x2-4,y2-4,dft_bstyle[3],NOFILL);

	fore=hgGetTFcolor();
	back=hgGetTBcolor();

        hgSetTFcolor(ZERO);
	hgSetTBcolor(LIGHTGRAY);

        num=ret_number(msg)/16/hgGetYFactor();

        for (i=0;i<num;i++) {
                hgOutTextXy(x1+10,pos,msg[i]);
                pos+=16*hgGetYFactor();
	}
        pos+=4;
	hgDHline(x1+6,x2-6,pos);
        pos+=16;

        num=ret_number(items)/hgGetYFactor()/16;

        xincre=(x2-x1+1-32)/num;
        xpos=11+x1+xincre/2;

	for (i=0;i<num;i++) {
                length=strlen(items[i])*pw(2,hgGetXFactor()-1);
		hgPrtButtonXy(xpos-length*4,pos,items[i],NOPUSH);
                x[i]=xpos-length*4;
                xw[i]=length*8+10;
		xpos+=xincre;
	}

        xpos=11+x1+xincre/2;
	i=0;
        prvs=xpos;
        prvi=i;

        hgHideMouse();

        length=strlen(items[i])*pw(2,hgGetXFactor()-1);
        hgGetImage4(xpos-length*4-5,pos-5,xpos+length*4+15,pos+26+16*(hgGetYFactor()-1),&buff);
        hgDrawBorder(xpos-length*4-5,pos-5,xpos+length*4+15,pos+26+16*(hgGetYFactor()-1),dft_bstyle[3],NOFILL);

        hgShowMouse();

	do {
                do {
                        ch=inkey(NOWAIT);
                        if (ch==MOUSE_RIGHT || ch==MOUSE_MIDDLE) ch=NOKEY;
                } while (ch==NOKEY);

                if (ch==MOUSE_LEFT) {
                        j=get_xwpos(x,pos-1,xw,20+16*(hgGetYFactor()-1),num);
                        if (j>=0 && j<num) {
				i=j;
                                hgHideMouse();
                                length=strlen(items[prvi])*pw(2,hgGetXFactor()-1);
                                hgPutImage4(prvs-length*4-5,pos-5,prvs+length*4+15,pos+26+16*(hgGetYFactor()-1),buff);
                                hgFreeCIMAGE(&buff);
                                length=strlen(items[i])*pw(2,hgGetXFactor()-1);
                                xpos=11+x1+xincre/2+i*xincre;
                                hgDrawBorder(xpos-length*4-5,pos-5,xpos+length*4+15,pos+26+16*(hgGetYFactor()-1),dft_bstyle[3],NOFILL);
                                break;
                        }
                }

		if (ch==RIGHT || ch==TAB) { xpos+=xincre;i++; }
		if (ch==LEFT ) { xpos-=xincre;i--; }

		if (i<0) { xpos+=xincre*num;i=num-1; }
                if (i>num-1) { xpos=11+xincre/2+x1;i=0; }

                if (xpos!=prvs) {
                        length=strlen(items[prvi])*pw(2,hgGetXFactor()-1);
                        hgHideMouse();
                        hgPutImage4(prvs-length*4-5,pos-5,prvs+length*4+15,pos+26+16*(hgGetYFactor()-1),buff);
                        hgFreeCIMAGE(&buff);
                        length=strlen(items[i])*pw(2,hgGetXFactor()-1);
                        hgGetImage4(xpos-length*4-5,pos-5,xpos+length*4+15,pos+26+16*(hgGetYFactor()-1),&buff);
			hgDrawBorder(xpos-length*4-5,pos-5,xpos+length*4+15,pos+26+16*(hgGetYFactor()-1),dft_bstyle[3],NOFILL);
                        prvs=xpos;
                        prvi=i;
                        hgShowMouse();
                }
                if (ch==RETURN) {
                        length=strlen(items[prvi])*pw(2,hgGetXFactor()-1);
                        hgHideMouse();
                        hgPutImage4(prvs-length*4-5,pos-5,prvs+length*4+15,pos+26+16*(hgGetYFactor()-1),buff);
                        hgFreeCIMAGE(&buff);
                        break;
                }
	} while (1);
	hgSetTFcolor(fore);
	hgSetTBcolor(back);
        length=strlen(items[i])*pw(2,hgGetXFactor()-1);
        hgHideMouse();
	hgPrtButtonXy(xpos-length*4,pos,items[i],PUSH);
        hgShowMouse();
        while (hgLeftMouse());
	return i;
}

int hgPrtButtonBarXyQ(int x,int y,char *msg[],char *items[])
{
	int max=ret_maxlength(msg);
	int num=ret_number(msg);

        int flag;
        int x2=x+max+32;
        int y2=y+num+54+16*hgGetYFactor();

	flag=hgPrtButtonBarXy(x,y,x2,y2,msg,items);

	return flag;
}

int hgPrtButtonBarXyM(char *msg[],char *items[])
{
	int max=ret_maxlength(msg);
	int num=ret_number(msg);

        int flag;
        int xwidth=max+33;
        int ywidth=num+55+16*hgGetYFactor();
        int xmid=(hgGetx1r()+hgGetx2r())/2;
        int ymid=(hgGety1r()+hgGety2r())/2;

        flag=hgPrtButtonBarXy(xmid-xwidth/2,ymid-ywidth/2,xmid+xwidth/2,ymid+ywidth/2,msg,items);

	return flag;
}

