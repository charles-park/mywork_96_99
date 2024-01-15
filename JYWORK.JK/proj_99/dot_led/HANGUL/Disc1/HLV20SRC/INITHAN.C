/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Hangul Output               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

/* include file */

#include <stdio.h>
#include <dos.h>

#include "hghlib.h"
#include "hginit.h"

/* Color Define for 256 color */

char c256[16]={ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

/* 4 color */

char hgFore=WHITE;
char hgBack=ZERO;
char hgTF=ZERO;
char hgTB=WHITE;

/* some global variable */

int hgx1s;               /* screen x minimum     */
int hgx2s;               /* screen x maximum     */
int hgy1s;               /* screen y minimum     */
int hgy2s;               /* screen y maximum     */
int hgx1r;               /* real   x minimum     */
int hgx2r;               /* real   x maximum     */
int hgy1r;               /* real   y minimum     */
int hgy2r;               /* real   y maximum     */

int  xbyte;

int hgMode;              /* graphic or text      */
int hgDisplay;           /* various graphic card */
int hgColors;            /* physical max colors  */

int _hgH=hgFAIL;         /* if Hercules, this is hgSUCCESS */
char mouse_ok=hgFAIL;    /* mouse exist ?                  */
char mouse_enable=hgFAIL;

/* SuperVGA special */

void svga_null_process();
void (*svgaspec)()=&svga_null_process;
int  specflag=hgFAIL;           /* default is hgFAIL */

/* mode setting number for ET4000 */

int ModeSet[]={ 0x12,0x29,0x38,0x10,0x0e,0,0,0x2e,0x30 };

/* some prototype */

void call_int10_0(int);

void (*magh)(int x,int y,char *buff,char color);
void (*mage)(int x,int y,char *buff,char color);

void svga_null_process()
{
	specflag&=0xff;         /* dummy statement */
}

void hgSetDisplay(int disp)
{
	if (disp==hgHERC640x400) hgSetHercDisplay(disp);
	else hgSetColorDisplay(disp);
}
void hgSetScreenWindow(int x1,int y1,int x2,int y2)
{
	hgx1s=x1;hgx2s=x2;hgy1s=y1;hgy2s=y2;
}

void hgSetRealWindow(int x1,int y1,int x2,int y2)
{
	hgx1r=x1;hgx2r=x2;hgy1r=y1;hgy2r=y2;
}

void hgSetHercDisplay(int disp)
{
	_hgH=hgSUCCESS;
	hgDisplay=disp;

	hgSetMaxPages(1);     /* just One buffer */

	if (hgGetTextMode()!=hgMonoText) {
		printf("\nError: incorrect video setting occured.\n");
		printf("- made by Han Woo-jin. Hurumi Library 2.0.\n\n");
		exit(1);
	}
	hgSetRealWindow(0,0,639,399);
	hgSetScreenWindow(0,0,639,399);
	hgColors=2;
	hgEnableClearScreen(&hgHClearScreen);
}

void hgSetColorDisplay(int disp)
{
	_hgH=hgFAIL;
	hgDisplay=disp;


	if (!hgIsEGAVGA() || (hgIsEGA() && disp!=hgEGA640x350x16 && disp!=hgEGA640x200x16)) {
		printf("\nError: incorrect video setting occured.\n");
		printf("- made by Han Woo-jin. Hurumi Library 2.0.\n\n");
		exit(1);
	}

	switch(disp) {
		case hgVGA640x480x16 :
			hgSetRealWindow(0,0,639,479);
			hgSetScreenWindow(0,0,639,479);
			hgSetMaxPages(4);
			break;
		case hgVGA800x600x16 :
			hgSetRealWindow(0,0,799,599);
			hgSetScreenWindow(0,0,799,599);
			hgSetMaxPages(4);
			break;
		case hgEGA640x350x16 :
			hgSetRealWindow(0,0,639,349);
			hgSetScreenWindow(0,0,639,349);
			hgSetMaxPages(4);
			break;
		case hgEGA640x200x16 :
			hgSetRealWindow(0,0,639,199);
			hgSetScreenWindow(0,0,639,199);
			hgSetMaxPages(4);
			break;
		case hgVGA640x480x256 :
			hgSetRealWindow(0,0,639,479);
			hgSetScreenWindow(0,0,639,479);
			hgSetMaxPages(4);
			break;
		case hgVGA800x600x256 :
			hgSetRealWindow(0,0,799,599);
			hgSetScreenWindow(0,0,799,599);
			hgSetMaxPages(4);
			break;
		 case hgVGA1024x768x256:
			hgSetRealWindow(0,0,1023,767);
			hgSetScreenWindow(0,0,1023,767);
			hgSetMaxPages(4);
			break;
	}

	hgEnableClearScreen(&hgVClearScreen);

	xbyte=(hgx2r+1)/8;
	hgColors=16;

	switch(disp) {
		case hgVGA640x480x256 :
		case hgVGA800x600x256 :
		case hgVGA1024x768x256:
		xbyte*=8;
		hgColors=256;
		hgEnableClearScreen(&hgClearScreen256);
	}
}

int hgGetx1s()
{
	return hgx1s;
}

int hgGety1s()
{
	return hgy1s;
}
int hgGetx2s()
{
	return hgx2s;
}

int hgGety2s()
{
	return hgy2s;
}
int hgGetx1r()
{
	return hgx1r;
}

int hgGety1r()
{
	return hgy1r;
}
int hgGetx2r()
{
	return hgx2r;
}

int hgGety2r()
{
	return hgy2r;
}

void hgSetMode(int mode)
{
	hgMode=mode;
	switch(mode) {
		case hgTEXT:
			if (_hgH) call_int10_0(7);
			else      call_int10_0(3);
			break;
		case hgGRAPHICS:
			if (_hgH) hgSetHercMode();
			else      hgSetColorMode();
			break;
	}
	hgSVGASpecial();    /* for svga special */
}

void call_int10_0(int mode)
{
	union REGS in;

	if (mode==-1 || mode==0) {
		printf("\nError: can't set video mode.\n");
		printf("- made by Han Woo-jin. Hurumi Library 2.0.\n\n");
		exit(1);
	}

	in.h.ah=0;
	in.h.al=mode;

	int86(0x10,&in,&in);
}

int hgIsHerc()          /* if then, hgSUCCESS or hgFAIL */
{
	return _hgH;
}
void hgSetHercMode()
{
	if (hgGetTextMode()!=hgMonoText) {
		printf("\nError: incorrect video setting occured.\n");
		printf("- made by Han Woo-jin. Hurumi Library 2.0.\n\n");
		exit(1);
	}
	hgSetHerc640();
}
void hgSetColorMode()
{
	call_int10_0(ModeSet[hgDisplay]);
}
void hgSetHerc640()
{
	char data[8]={ 0x31,0x28,0x29,0x08,0x68,0x02,0x64,0x65 };
	int i;

	outportb(0x3b8,0);              /* disable */

	for (i=0;i<8;i++){
		outportb(0x3b4,i);
		outportb(0x3b5,data[i]);
	}
	outportb(0x3b4,9);outportb(0x3b5,3);

	outportb(0x3bf,3);              /* for Full Mode */
	outportb(0x3b8,0x1e);           /* able */

	hgHClearScreen();
}

int hgGetMode()
{
	return hgMode;
}

int hgGetDisplay()
{
	return hgDisplay;
}

int hgGetTextMode()
{
	union REGS in,out;

	in.h.ah=0x0f;

	int86(0x10,&in,&out);

	return(out.h.al);
}

void hgSetRGBpalette(int palnum,int r,int g,int b)
{
	int pal[]={ 0,1,2,3,4,5,20,7,56,57,58,59,60,61,62,63 };
	int reg=pal[palnum];

	outportb(0x3c8,reg);
	outportb(0x3c9,r);
	outportb(0x3c9,g);
	outportb(0x3c9,b);
}

void hgGetRGBpalette(int color,int *r,int *g,int *b)
{
	int pal[]={ 0,1,2,3,4,5,20,7,56,57,58,59,60,61,62,63 };
	int reg=pal[color];

	outportb(0x3c7,reg);
	*r=inportb(0x3c9);
	*g=inportb(0x3c9);
	*b=inportb(0x3c9);
}
void hgSetRGBpalette256(int palnum,int r,int g,int b)
{
	outportb(0x3c8,palnum);
	outportb(0x3c9,r);
	outportb(0x3c9,g);
	outportb(0x3c9,b);
}
void hgGetRGBpalette256(int color,int *r,int *g,int *b)
{
	outportb(0x3c7,color);
	*r=inportb(0x3c9);
	*g=inportb(0x3c9);
	*b=inportb(0x3c9);
}
void hgSetGRAYpalette(int palnum)
{
	int r,g,b;
	float rate;
	int value;

	hgGetRGBpalette(palnum,&r,&g,&b);
	rate=(float)r*0.3+(float)g*0.59+(float)b*0.11;
	value=(int)rate%64;
	hgSetRGBpalette(palnum,value,value,value);
}
void hgSetGRAYpalette256(int palnum)
{
	int r,g,b;
	float rate;
	int value;

	hgGetRGBpalette256(palnum,&r,&g,&b);
	rate=(float)r*0.3+(float)g*0.59+(float)b*0.11;
	value=(int)rate%64;
	hgSetRGBpalette256(palnum,value,value,value);
}

void hgAutoSetDisplay()
{
	if (hgGetTextMode()!=hgMonoText) {
		if (!hgIsEGAVGA()) {
			printf("\nHurumi Library 2.0 doesn't support this video card.\n");
			printf("- made by Han Woo-jin. Hurumi Library 2.0.\n\n");
			exit(1);
		}
		if (hgIsVGA()) hgSetDisplay(hgVGA640x480x16);
		else hgSetDisplay(hgEGA640x350x16);
	}
	else
		hgSetDisplay(hgHERC640x400);
}

void hgSetFore(char color)
{
	hgFore=color;
}
void hgSetBack(char color)
{
	hgBack=color;
}
void hgSetTFcolor(char color)
{
	hgTF=color;
}
void hgSetTBcolor(char color)
{
	hgTB=color;
}
char hgGetFore()
{
	return hgFore;
}
char hgGetBack()
{
	return hgBack;
}
char hgGetTFcolor()
{
	return hgTF;
}
char hgGetTBcolor()
{
	return hgTB;
}
void hgSetMouse()
{
	mouse_ok=hgSUCCESS;
}
char hgIfClose()
{
	return mouse_ok;
}
char hgGetMouse()
{
	if (mouse_ok && mouse_enable) return hgSUCCESS;
	else return hgFAIL;
}
void hgSetChipSet(void (*setfunc)())
{
	(*setfunc)();
}
int  hgGetMaxColors()
{
	return hgColors;
}
int  hgIsEGAVGA()
{
	union REGS in,out;

	in.h.ah=0x12;
	in.h.bl=0x10;
	in.h.bh=0x55;

	int86(0x10,&in,&out);

	if (_BH==0x55) return 0;
	else return 1;
}
int  hgIsVGA()
{
	union REGS in,out;

	in.h.ah=0x1a;
	in.h.al=0;

	int86(0x10,&in,&out);

	if (_AL==0x1a && (_BL==7 || _BL==8)) return 1;
	else return 0;
}
int  hgIsEGA()
{
	union REGS in,out;

	in.h.ah=0x1a;
	in.h.al=0;

	int86(0x10,&in,&out);

	if (_AL==0x1a && (_BL==3 || _BL==4 || _BL==5)) return 1;
	else return 0;
}
void hgSVGASpecial()
{
	if (specflag) (*svgaspec)();
}
