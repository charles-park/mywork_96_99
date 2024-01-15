/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             "Header File"              */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

/* header define */

#ifndef __HGHLIB_H
#define __HGHLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>	/* for file process */
#include "hgvir.h"      /* for virtual system */

/* extern font type */

#define MAXFONTQUEUE 500
#define MAXUSERCHAR   10

typedef struct extfont {
        FILE *fpt;
        char *p[MAXFONTQUEUE];
        int   n[MAXFONTQUEUE];
        int   qh,qt;            /* queue header,queue tail */
        int   xsize,ysize;
        int   qsize;
        char *buff;
} ExtFont;

/* hangul type & english type */

typedef struct htype {
	int firs;
	int mids;
	int lass;
} HTYPE;

typedef struct etype {
	int engs;
} ETYPE;

/* hangul byte & code form */

typedef struct {
                        unsigned lasc :5;
                        unsigned midc :5;       /* HAN CODE FORM */
                        unsigned firc :5;
                        unsigned dummy:1;
} hcode;

typedef struct {
                        unsigned char schar;    /* HAN BYTE FORM */
                        unsigned char fchar;
} hchar;

/* cursor blinking */

#define NOBLINK         0
#define DEFAULTBLINK    7

/* attribute */

#define NORMAL          1
#define REVERSE         2
#define DIM             4
#define SHADOW          8
#define THREED          16
#define OUTLINE         32
#define UNDER           64
#define BOLD            128

/* font format */

#define HAN_8x4x4       0
#define HAN_6x2x1       1
#define HAN_10x4x4      2
#define HAN_2x1x2       3
#define ENG_256         0
#define SPECIAL_CODE    212

/* 9 english font define ... */

#define BIG_ENG         &big_eng    ,ENG_256
#define ITALIC_ENG      &italic_eng ,ENG_256
#define SMALL_ENG       &small_eng  ,ENG_256
#define PILGI_ENG       &pilgi_eng  ,ENG_256
#define INBOLD_ENG      &inbold_eng ,ENG_256
#define BOLD_ENG        &bold_eng   ,ENG_256
#define EART_ENG        &eart_eng   ,ENG_256
#define AD24_ENG        &ad24_eng   ,ENG_256
#define ROMAN_ENG       &roman_eng  ,ENG_256

/* 15 hangul font define ... */

#define GODIK1_HAN      &hanchg2_han  ,HAN_8x4x4
#define GODIK2_HAN      &hanchg4_han  ,HAN_8x4x4
#define HANIYG_HAN      &haniyg_han   ,HAN_8x4x4
#define HANSOFT_HAN     &hansoft_han  ,HAN_8x4x4
#define HWP_HAN         &hwp_han      ,HAN_10x4x4
#define MYUNGJO_HAN     &han_m_han    ,HAN_8x4x4
#define PILGI_HAN       &pilhan_han   ,HAN_6x2x1
#define PPALAE_HAN      &ppalae_han   ,HAN_2x1x2
#define AD24_HAN        &ad24_han     ,HAN_8x4x4
#define SAM_HAN         &sam_han      ,HAN_8x4x4
#define HOULD_HAN       &hould_han    ,HAN_8x4x4
#define HMAPPLE_HAN     &hmapple_han  ,HAN_8x4x4
#define HANSON_HAN      &hanson_han   ,HAN_8x4x4
#define HANPIL_HAN      &hanpil_han   ,HAN_8x4x4
#define HANBOOT_HAN     &hanboot_han  ,HAN_8x4x4

/* 1 special character */

#define SPECIAL_FNT     &special_fnt,SPECIAL_CODE

/* attribute enable function */

#define fNORMAL         &attr_null  ,0
#define fREVERSE        &Reverse_Img,0
#define fDIM            &Dim_Img    ,1
#define fSHADOW         &Shadow_Img ,2
#define fTHREED         &ThreeD_Img ,3
#define fOUTLINE        &Outline_Img,4
#define fBOLD           &Bold_Img   ,5
#define fUNDER          &Under_Img  ,6

/* hangul code type */

#define SANGYONG        &sangyong_code
#define SAMSUNG         &samsung_code
#define GOLDSTAR        &goldstar_code

/* for hangul input routine */

#define _SPECIAL_KEYCODE         0
#define _NORMAL_KEYCODE         -1
#define _EXIT_KEYCODE            1
#define _DISABLE_KEYCODE        -2

/* prototype */

/*========================================================*/
/*  initial - functions                                   */
/*========================================================*/

void hgSetFore(char color);
void hgSetBack(char color);
void hgSetTFcolor(char color);
void hgSetTBcolor(char color);
char hgGetFore(void);
char hgGetBack(void);
char hgGetTFcolor(void);
char hgGetTBcolor(void);
void hgSetDisplay(int disp);
void hgSetHercDisplay(int disp);
void hgSetColorDisplay(int disp);
void hgSetScreenWindow(int x1,int y1,int x2,int y2);
void hgSetRealWindow(int x1,int y1,int x2,int y2);
int  hgGetx1s(void);
int  hgGety1s(void);
int  hgGetx2s(void);
int  hgGety2s(void);
int  hgGetx1r(void);
int  hgGety1r(void);
int  hgGetx2r(void);
int  hgGety2r(void);
void hgSetMode(int mode);
int  hgIsHerc(void);
void hgSetHercMode(void);
void hgSetColorMode(void);
void hgSetHerc720(void);    /* this mode is not supported. */
void hgSetHerc640(void);
int  hgGetMode(void);
int  hgGetDisplay(void);
int  hgGetTextMode(void);
void hgSetRGBpalette(int palnum,int r,int g,int b);
void hgGetRGBpalette(int palnum,int *r,int *g,int *b);
void hgSetRGBpalette256(int color,int r,int g,int b);
void hgGetRGBpalette256(int color,int *r,int *g,int *b);
void hgSetGRAYpalette(int palnum);
void hgSetGRAYpalette256(int color);
void hgAutoSetDisplay(void);
void hgClearScreen(void);
void hgEnableClearScreen(void (*clr)(void));
int  inkey(int mode);        /* TWO mode.. WAIT & NO_WAIT */
void hgSetMouse(void);
char hgGetMouse(void);
void hgSetMouseInit(void);       /* mouse init */
void hgSetMouseEnd(void);        /* mouse end  */
char hgIfClose(void);
int  hgGetMaxColors(void);
int  hgIsEGAVGA(void);           /* EGA or VGA present */
int  hgIsVGA(void);              /* VGA present        */
int  hgIsEGA(void);              /* EGA present        */
int  hgGetMaxPages(void);
void hgSetMaxPages(int num);
int  hgDetectCard(void);         /* video card detect */
void hgSVGASpecial(void);        /* SuperVGA special  */

/* mark init */

void hgMarkOn(void);
void hgMarkOff(void);
void hgMarkEnd(void);
void hgMarkInit(void);

/* svga init */

void hgSetChipSet(void (*p)(void));
void hgAutoChipSet(void);           /* Super VGA AutoDetection */
int  hgGetChipSet(void);
void hgSetPageInit(void (*func)(void));
void setpage(void);

/* timer function */

void hgTimerInstall(void);
void hgTimerUninstall(void);
void hgSetTimerZero(void);
long hgGetTimerTicks(void);
double hgGetElapsedTime(void);

/*========================================================*/
/*  output - functions                                    */
/*========================================================*/

void hgSetHanOut(void (*hout)(int x,int y,char *buff,char color));
void hgSetEngOut(void (*eout)(int x,int y,char *buff,char color));
void hgSetMagOut(void (*eout)(int x,int y,char *buff,int m,int n,char color));
void puthan(int x,int y,char *buff,char color);
void puteng(int x,int y,char *buff,char color);
void putchmn(int x,int y,char *buff,char color);
void putmag(int x,int y,char *buff,int m,int n,char color);
void hgSetHanFontRoom(int htype);
void hgSetEngFontRoom(int etype);
void hgSetSpecialFontRoom(int stype);
void hgSetSpecialCharOn(void);
void hgSetSpecialCharOff(void);
void hgFreeHanRoom(void);
void hgFreeEngRoom(void);
void hgFreeSpecialRoom(void);
void hgLoadHanFont(char *hname,int htype);
void hgLoadEngFont(char *ename,int htype);
void hgLoadSpecialFont(char *sname,int stype);
void hgSetHanFont(void (*hf)(void),int htype);
void hgSetEngFont(void (*ef)(void),int etype);
void hgSetSpecialFont(void (*sf)(void),int stype);
void hgSetCtable(int method);
void hgSetCodeTable(void (*p)(void));
void hgResetCodeTable(void);
void hgOutTextXy(int x,int y,char *str);
void hgOutText(char *string);
void hgPrintfXy(int x,int y,char *fmt,...);
void hgPrintf(char *fmt,...);
void hgEngOutTextXy(int x,int y,char *str);
void hgEngPutch(int x,int y,char num);
void hgEngPrintfXy(int x,int y,char *fmt,...);
void hgForeTextXy(int x,int y,char *str,int fore);
void hgBackTextXy(int x,int y,char *str,int back);
void hgSpecTextXy(int x,int y,char *str,int color1,int color2,int deep);
void hgOverTextXy(int x,int y,char *str);
void hgFBTextXy(int x,int y,char *str,int fcolor,int scolor);
void hgGotoXy(int x,int y);
int  hgGetOutX(void);
int  hgGetOutY(void);
void hgTextAttrInit(void);
void hgSetTextAttrOn(void);
void hgSetTextAttrOff(void);

/* extern font */

ExtFont *hgOpenExtFont(char *path,int x,int y,int qsize);
void     hgGetExtFont(ExtFont *p,int n);
void     hgPutExtFont(ExtFont *p,int x,int y,int n);
void     hgCloseExtFont(ExtFont *p);
int      hgGetExtFontNum(ExtFont *p);
int      hgGetExtFontXsize(ExtFont *p);
int      hgGetExtFontYsize(ExtFont *p);

/* hanja enable */

void hgHanjaInit(char *fname,int qsize);
void hgHanjaEnd(void);
void hgHanjaInInit(int keyvalue);
void hgHanjaToHangulSub(unsigned char  h1 ,unsigned char  h2,
                        unsigned char *hh1,unsigned char *hh2);
void hgHanjaToHangul(char *src,char *dest);

/* hanjadictionary enable */

void hgRegisterKeyFunction(int value,void (*p)(void));
void hgHanjaDicInit(char *fname,int keyvalue);
void hgHanjaDicEnd(void);

/* Graphic Character */

void hgGraphicCharInit(char *fname,int qsize);
void hgGraphicCharEnd(void);
void hgGraphicCharInInit(int keyvalue1,int keyvalue2);

/*========================================================*/
/*  attribute - functions                                 */
/*========================================================*/

void hgSetEAttr(char attr);
char hgGetEAttr(void);
void hgSetHAttr(char attr);
char hgGetHAttr(void);
void hgSetExtAttr(char attr);
char hgGetExtAttr(void);
void hgDoAttr(char *dest,int bytes,int n);
void Reverse_Img(char *dest,int bytes);
void Dim_Img(char *dest, int bytes);
void Shadow_Img(char *dest, int bytes);
void ThreeD_Img(char *dest,int bytes);
void Outline_Img(char *dest,int bytes);
void Bold_Img(char *dest, int bytes);
void Under_Img(char *dest,int bytes);
void hgEnableAttr(void (*func)(char *dest,int bytes),int bytes);

/*========================================================*/
/*  input - functions                                     */
/*========================================================*/

void hg2bulInit(void);
void hgHanInInit(void);
void hg3bulInit(void);
void hgSetCursAttr(char attr);
char hgGetCursAttr(void);
void hgSetCursColor(char color);
char hgGetCursColor(void);
int  hgInTextXy(int x,int y,char *retn_str,int max);
void hgSetEngMode(void);
void hgSetHanMode(void);
void hgSetInMethod(int ch);
int  hgGetInMethod(void);
int  hgGetInStatus(void);
void hgInitStr(char *str,int n);
void hgSetInternal(void (*func)(void));
int  hgGetPos(void);
int  hgGetCh(void);
void hgSetInsertOn(void);
void hgSetInsertOff(void);
int  hgIsInsertOn(void);
void hgSetCursorRate(long rate);     /* cursor blinking */
void hgSpecInModeOn(void);               /* special input mode */
void hgSpecInModeOff(void);
void hgEngInModeOn(void);                /* english input mode */
void hgEngInModeOff(void);
void hgDigitInModeOn(void);              /* digit input mode */
void hgDigitInModeOff(void);

/* exit key */

void hgSetExtKey(int *a);
int *hgGetExtKey(void);
void hgFreeExtKey(void);
int  hgGetNumExtKey(void);
void hgSetExtKeyOn(void);
void hgSetExtKeyOff(void);

/* disable key */

void hgSetDisableKey(int *a);
int *hgGetDisableKey(void);
void hgFreeDisableKey(void);
int  hgGetNumDisableKey(void);
void hgSetDisableKeyOn(void);
void hgSetDisableKeyOff(void);

/* special input function */

int  windelay(int x1,int y1,int x2,int y2);
void hgMouseInFlagOff(void);
void hgMouseInFlagOn(void);
void hgRetMSize(char *text[],int *x1,int *y1,int *x2,int *y2);

/*========================================================*/
/*  Graphic - functions                                   */
/*========================================================*/

unsigned hgImgSize(int x1,int y1,int x2,int y2);
unsigned hgImgSize4(int x1,int y1,int x2,int y2);
unsigned char get_lmask(int x);
unsigned char get_rmask(int x);
unsigned char get_vmask(int x);

void hgGetImage(int x1,int y1,int x2,int y2,char *buff);
void hgPutImage(int x1,int y1,int x2,int y2,char *buff);
void hgGetImage4(int x1,int y1,int x2,int y2,CIMAGE **p);
void hgPutImage4(int x1,int y1,int x2,int y2,CIMAGE *p);
void hgGetImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE **p);
void hgPutImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE *p);

void hgEnableImage(void (*g)(int x1,int y1,int x2,int y2,char *buff),
                   void (*p)(int x1,int y1,int x2,int y2,char *buff) );
void hgEnableImgSize(unsigned (*i)(int x1,int y1,int x2,int y2));
void hgEnableImage4(void (*g)(int x1,int y1,int x2,int y2,CIMAGE **p),
                   void (*p)(int x1,int y1,int x2,int y2,CIMAGE *p));
void hgEnableImgSize4(unsigned (*i)(int x1,int y1,int x2,int y2));
void hgEnableImageVIRTUAL(void (*g)(int x1,int y1,int x2,int y2,VIMAGE **p),
                   void (*p)(int x1,int y1,int x2,int y2,VIMAGE *p));

void hgBox(int x1,int y1,int x2,int y2,int color);
void hgHline(int x1,int x2,int y,char color);
void hgVline(int x,int y1,int y2,char color);
void hgDHline(int x1,int x2,int y);
void hgDVline(int x,int y1,int y2);
void hgEnableHVline(void (*h)(int x1,int x2,int y,char color),
                    void (*v)(int x,int y1,int y2,char color));

void hgEnableBoxFill(void (*b)(int x1,int y1,int x2,int y2,int color));
void hgBoxFill(int x1,int y1,int x2,int y2,int color);

void hgEnableScrUp(void (*b)(int x1,int y1,int x2,int y2,int lines,int color));
void hgScrUp(int x1,int y1,int x2,int y2,int lines,int color);
void hgEnableScrDown(void (*b)(int x1,int y1,int x2,int y2,int lines,int color));
void hgScrDown(int x1,int y1,int x2,int y2,int lines,int color);
void hgEnablePlot(void (*p)(int x,int y,char color));
void hgEnableGetPlot(char (*p)(int x,int y));
void hgPlotXy(int x,int y,char color);
char hgGetPlotXy(int x,int y);
void hgEnableLine(void (*p)(int x1,int y1,int x2,int y2,char color));
void hgLine(int x1,int y1,int x2,int y2,char color);
void hgCircle(int x,int y,int radius,char color);
void hgEllipse(int x,int y,int a,int b,char color);

unsigned char hgGetHFillXPattern(int color);
unsigned char hgGetHFillYPattern(int color);
void          hgSetHFillXPattern(int color,unsigned char pattern);
void          hgSetHFillYPattern(int color,unsigned char pattern);
void          hgSetHFillDPattern(void);

unsigned char hgGetHlinePattern(char color);
unsigned char hgGetVlinePattern(char color);
void          hgSetHlinePattern(char color,unsigned char pat);
void          hgSetVlinePattern(char color,unsigned char pat);
void          hgSetHVlineDefault(void);

void hgSetMagnify(void);
void hgSetVMagnify(void);
void hgSetHMagnify(void);
void hgSetSMagnify(void);
void hgSetXFactor(int n);
void hgSetYFactor(int n);
int  hgGetXFactor(void);
int  hgGetYFactor(void);

/* ================================================================  */
/*  don't call this!                                                 */
/* ----------------------------------------------------------------  */

int  ret_herc_offset(int y); /* for hercules offset */
void hercadd(void);              /* please,don't use this function! */
void hercadd_dx(void);
void hercsub(void);
void hercsub_dx(void);
void ypat(int t);            /* and this ! */
int  pw(int a,int b);

/* ================================================================ */

#ifdef __cplusplus
}
#endif

/* for external prototype */

#include "extern.h"

#endif
