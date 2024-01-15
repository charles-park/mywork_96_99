/* ==================================== */
/*                                      */
/*      THIS IS H-Window Library        */
/*                                      */
/*                 by                   */
/*                                      */
/*        Hurumi hangul library         */
/*                 &                    */
/*            graphic library           */
/*                                      */
/*       programmed by KIT e900519      */
/*                                      */
/*              Han Woo-jin             */
/*                                      */
/* ==================================== */

#ifndef __HWINDOW_H
#define __HWINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

/* include file */

#include <string.h>
#include "hgvir.h"      /* for virtual system */

/* usable define */

#define DRAW      	1
#define ERASE     	0

/* don't touch this */

#define MAXITEM  	15
#define MAXPOPUP	15
#define _MAXDIR 	200
#define MAXHELPLINE 	100
#define MAXSTYLE        20

#define HORIZ    0
#define VERT     1

#define FILL     1
#define NOFILL   0

#define NOPUSH   1
#define PUSH     0

#define ZERO     BLACK          /* special color code */

#define hgEnable  1             /* used by pop up & pull down */
#define hgDisable 0

/* used by hgDrawBorder */

#define INSIDE          dft_bstyle[0]
#define INSIDE2         dft_bstyle[1]
#define BOXNORMAL       dft_bstyle[2]
#define BOXREVERSE      dft_bstyle[3]
#define BLUEBOX         dft_bstyle[4]
#define BARNORMAL       dft_bstyle[5]
#define BARREVERSE      dft_bstyle[6]
#define ERASER          dft_bstyle[7]

/* used by WCOLOR */

#define wcINSIDE          0
#define wcINSIDE2         1
#define wcBOXNORMAL       2
#define wcBOXREVERSE      3
#define wcBLUEBOX         4
#define wcBARNORMAL       5
#define wcBARREVERSE      6
#define wcERASER          7

/* used by hgSetDither */

#define DITHER_GRAY     &dither_gray
#define DITHER_PATTERN  &dither_pattern

/* system define.. not usable */

#define _PLANES  4

/* extern variable */

extern int dft_bstyle[MAXSTYLE][4];

/* fundamental structure(sub) define */

typedef struct bstyle {
	int back;
	int color1;
	int color2;
	int deep;
} BSTYLE;

typedef struct wcolor {
        int border;
        int tfore;
        int tback;
        int back;
} WCOLOR;

typedef struct bcolor {
	int border;
	int back;
} BCOLOR;

typedef struct width {
        int xwidth;
        int ywidth;
} WIDTH;

/* object define */

typedef struct hwindow {
	char  *titl;
        WIDTH  size;
        WCOLOR color;
} HWINDOW;

typedef struct hbox {
	char  *items[MAXITEM];
        WCOLOR color;
        WIDTH  size;
        VIMAGE *buff;
} HBOX;

typedef struct hbar {
        BCOLOR color;
        WIDTH  size;
        CIMAGE *buff;
} HBAR;

typedef struct hbutton {
        char  *item;
        WIDTH  size;
        WCOLOR color;
} HBUTTON;

typedef struct hpopup {
        char *items[MAXITEM];
        WCOLOR color;
        WIDTH  size;
        VIMAGE *buff;
        int    start;
        int    code[MAXITEM];
} HPOPUP;

typedef struct hpulldown {
        char   *items[MAXITEM];
        int     xpos[MAXITEM];
        WIDTH  size;
        HPOPUP *p[MAXPOPUP];
        int     code[MAXITEM];
        int     start;
} HPULLDOWN;

typedef struct hdirbox {
        char   *items[_MAXDIR];
        WIDTH  size;
        int    xnum;
        int    ynum;
} HDIRBOX;

typedef struct hicon {
	WIDTH size;
        char *buff;
} HICON;

typedef struct hscrlbar {
        WIDTH size;
	int   total;
	int   screen;
	int   pos;
	int   direc;
} HSCRLBAR;

typedef struct hhelp {
	char *items[MAXHELPLINE];
	int   xchars;
	int   ylines;
} HHELP;

/* prototype */

void hgDrawBorder(int x1,int y1,int x2,int y2,int style[4],int fill);

/* style adding function */

int hgAddStyle(int back,int color1,int color2,int deep);

/* style convert in 256 color mode */

void hgAdjustStyle(void);
void hgAdjustPalette256(void);
void hgSavePalette256(void);
void hgRestorePalette256(void);

/* subfunction in DirBox */

void hgHercBarOn(void);
void hgHercBarOff(void);
void hgSetRecPosOn(void);
void hgSetRecPosOff(void);

/* load function */

void hgBSTYLE_Load(BSTYLE *p,int back,int color1,int color2,int deep);
void hgWCOLOR_Load(WCOLOR *p,int border,int tfore,int tback,int back);
void hgBCOLOR_Load(BCOLOR *p,int border,int back);
void hgWIDTH_Load (WIDTH  *p,int xwidth,int ywidth);

HWINDOW  *hgHWINDOW_Load(char *title,WCOLOR c,WIDTH w);
HBOX     *hgHBOX_Load(char *i[],WCOLOR c,WIDTH w);
HBAR     *hgHBAR_Load(BCOLOR c,WIDTH w);
HBUTTON  *hgHBUTTON_Load(char *i,WCOLOR c,WIDTH w);
HDIRBOX  *hgHDIRBOX_Load(char *i[],WIDTH w,int xnum,int ynum);
HICON    *hgHICON_Load(char *fname);
HICON    *hgHICON_Set(void (*icon)(HICON *p));
HHELP    *hgHHELP_Load(char *i[],int xc,int yl);
HSCRLBAR *hgHSCRLBAR_Load(int total,int screen,int direc,WIDTH w);

/* draw function */

void  hgHWINDOW_Draw(HWINDOW *p,int x,int y);
void  hgHBOX_Draw(HBOX *p,int x,int y);
void  hgHBAR_Draw(HBAR *p,int x,int y,int mode);
void  hgHBUTTON_Draw(HBUTTON *p,int x,int y,int mode);
void  hgHDIRBOX_Choose(HDIRBOX *p,int x,int y,char *title,char *ret);
void  hgHICON_Draw(HICON *p,int x,int y);
void  hgHHELP_Draw(HHELP *p,int x,int y,char *title);

void  hgHSCRLBAR_Draw(HSCRLBAR *p,int x,int y);
void  hgHSCRLBAR_Update(HSCRLBAR *p,int x,int y);
void  hgHSCRLBAR_Choose(HSCRLBAR *p,int x,int y);
int   hgHSCRLBAR_Area(HSCRLBAR *p,int x,int y);

/* free function */

void hgHWINDOW_Free(HWINDOW **p);
void hgHBOX_Free(HBOX **p);
void hgHBUTTON_Free(HBUTTON **p);
void hgHBAR_Free(HBAR **p);
void hgHDIRBOX_Free(HDIRBOX **p);
void hgHICON_Free(HICON **p);

void hgHSCRLBAR_Free(HSCRLBAR **p);

/* window special */

void hgHWINDOW_Clrscr(HWINDOW *p,int x,int y);
void hgHWINDOW_ScrUp(HWINDOW *p,int x,int y,int n);
void hgHWINDOW_ScrDown(HWINDOW *p,int x,int y,int n);

/* icon special */

void hgSetDither(void (*p)(void));
void hgDoDither(HICON *p);
int  hgIsIcon(char *fname);

/* box special */

void hgHBOX_Save(HBOX *p,int x,int y);
void hgHBOX_Restore(HBOX *p,int x,int y);

/* popup */

HPOPUP *hgHPOPUP_Load(char *i[],WCOLOR c,WIDTH w);
void    hgHPOPUP_Loadcode(HPOPUP *p,int c[MAXITEM]);
void    hgHPOPUP_Free(HPOPUP **p);
void    hgHPULLDOWN_Loadcode(HPULLDOWN *p,int c[MAXITEM]);
void    hgHPULLDOWN_Free(HPULLDOWN **p);
int     hgHPOPUP_Choose(HPOPUP *p,int x,int y,int start);
void    hgHPULLDOWN_Choose(HPULLDOWN *p,int x,int y,int start,int *xx,int *yy);
HPULLDOWN *hgHPULLDOWN_Load(char *i[],int x[MAXITEM],WIDTH w,
			    HPOPUP *pop[MAXPOPUP]);

/* quick function */

void hgPrtPullWindowXy(HPULLDOWN *p,char *title,int x,int y1,int y2);
void hgPrtPullDownXy(HPULLDOWN *p,int x,int y,int start,int *xx,int *yy);
void hgPrtTextIconXy(HICON *p,int x,int y,char *str);
void hgPrtIconBarXy(HICON *p,int x,int y);
void hgPrtIconBarSaveXy(HICON *p,int x,int y,int mode);

/* extern dither function */

extern void dither_gray(HICON *p);
extern void dither_pattern(HICON *p);

#ifdef __cplusplus
}
#endif

#endif
