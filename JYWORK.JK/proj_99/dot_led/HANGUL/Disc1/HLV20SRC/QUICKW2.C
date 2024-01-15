/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Quick Window Set             */
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

#include "hginit.h"
#include "hghlib.h"
#include "hgquick.h"
#include "key.h"
#include "hwindow.h"

extern int  wintitlefore;
extern int  wintitleback;
extern int  winback;
extern int  winborder;
extern int  saveflag;

void hgClearWindowXy(int x1,int y1,int x2,int y2)
{
        hgHideMouse();
        hgBoxFill(x1+13,y1+33,x2-13,y2-13,dft_bstyle[winback][0]);
        hgShowMouse();
}
void hgColorWindowXy(int x1,int y1,int x2,int y2,int color)
{
        hgHideMouse();
        hgBoxFill(x1+13,y1+33,x2-13,y2-13,color);
        hgShowMouse();
}
void hgScrUpWindowXy(int x1,int y1,int x2,int y2,int n)
{
        hgHideMouse();
        hgScrUp(x1+13,y1+33,x2-13,y2-13,n,dft_bstyle[winback][0]);
        hgShowMouse();
}
void hgScrDownWindowXy(int x1,int y1,int x2,int y2,int n)
{
        hgHideMouse();
        hgScrDown(x1+13,y1+33,x2-13,y2-13,n,dft_bstyle[winback][0]);
        hgShowMouse();
}
void hgSetTitleFore(int color)
{
        wintitlefore=color;
}
void hgSetTitleBack(int color)
{
        wintitleback=color;
}
int  hgGetTitleFore()
{
        return wintitlefore;
}
int  hgGetTitleBack()
{
        return wintitleback;
}
void hgSetWinBack(int style)
{
        winback=style;
}
int  hgGetWinBack()
{
        return winback;
}
void hgSetWinBorder(int style)
{
        winborder=style;
}
int  hgGetWinBorder()
{
        return winborder;
}
void hgSetSaveOn()
{
        saveflag=1;
}
void hgSetSaveOff()
{
        saveflag=0;
}
