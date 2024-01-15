/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Quick Header                */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#ifndef __HGQUICK_H
#define __HGQUICK_H

#ifdef __cplusplus
extern "C" {
#endif

/* quick function prototype */

void hgPrtWindowXy(int x1,int y1,int x2,int y2,char *title);
void hgPrtWTextXyQ(int x,int y,char *title,char *text[]);
void hgPrtWTextXyM(char *title,char *text[]);
void hgPrtWTextXy(int x1,int y1,int x2,int y2,char *title,char *text[]);
int  hgSelectXy(int x1,int y1,int x2,int y2,char *items[],int start);
int  hgSelectXyQ(int x,int y,char *items[],int start);
int  hgSelectXyM(char *items[],int start);
void hgClearWindowXy(int x1,int y1,int x2,int y2);
void hgColorWindowXy(int x1,int y1,int x2,int y2,int color);
void hgScrUpWindowXy(int x1,int y1,int x2,int y2,int n);
void hgScrDownWindowXy(int x1,int y1,int x2,int y2,int n);
void hgSetTitleFore(int color);
void hgSetTitleBack(int color);
int  hgGetTitleFore(void);
int  hgGetTitleBack(void);
void hgSetWinBack(int style);
void hgSetWinBorder(int style);
int  hgGetWinBack(void);
int  hgGetWinBorder(void);
void hgSetSaveOn(void);
void hgSetSaveOff(void);
int  hgInWTextXy(int x,int y,char *title,char *str,int n);
int  hgInWTextXyM(char *title,char *str,int n);
void hgPrtBoxXy(int x1,int y1,int x2,int y2,char *items[]);
void hgPrtBoxXyQ(int x,int y,char *items[]);
void hgPrtBoxXyM(char *items[]);
void hgPrtBarXy(int x1,int x2,int y);
void hgPrtBarSaveXy(int x1,int x2,int y,int mode);
void hgPrtButtonXy(int x,int y,char *text,int mode);
int  hgPrtButtonBarXy(int x1,int y1,int x2,int y2,char *msg[],char *items[]);
int  hgPrtButtonBarXyQ(int x,int y,char *msg[],char *items[]);
int  hgPrtButtonBarXyM(char *msg[],char *items[]);
void hgRestore(void);
void hgDelete(void);

/* directory box */

void    hgSelectDirBoxXy(int x,int y,char *items[],char *title,char *ret);
int     hgFileDirBoxXy(int x,int y,char *fname,char *ret);
void    hgSelectDirBoxXyM(char *items[],char *title,char *ret);
int     hgFileDirBoxXyM(char *fname,char *ret);

int     hgFindFirstDir(char *mask,char attrib,char *s);
int     hgFindNextDir(char attrib,char *s);

/* help box */

void hgPrtHelpBoxXy(int x,int y,char *items[],int xw,int yw,char *title);
int  hgFileHelpBoxXy(char *h,char *i,int x,int y,int xw,int yw,char *key);
void hgPrtHelpBoxXyM(char *items[],int xw,int yw,char *title);
int  hgFileHelpBoxXyM(char *h,char *i,int xw,int yw,char *title);

/* quick init function */

/* auto-detect function */

void hgQuickGraphInit(void);
void hgQuickInInit(void);
void hgQuickOutInit(void);

/* non-auto-detect function */

void hgQuickHGraphInit(void);
void hgQuickVGraphInit(void);
void hgQuickSGraphInit(void);
void hgQuickHOutInit(void);
void hgQuickVOutInit(void);
void hgQuickSOutInit(void);

/* mouse routine */

void hgGetMousePos(int *x,int *y);
int  hgLeftMouse(void);
int  hgRightMouse(void);
int  hgMiddleMouse(void);
void hgShowMouse(void);
void hgHideMouse(void);
void hgMoveMouse(int x,int y);
void hgSetMouseCursor(void (*p)(void));

#ifdef __cplusplus
}
#endif

#endif
