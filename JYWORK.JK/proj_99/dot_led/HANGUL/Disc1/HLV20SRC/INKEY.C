/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              Key Input                 */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "key.h"
#include "hghlib.h"

#include <bios.h>

#define NOKEY   -1
#define WAIT     0
#define NOWAIT   1

int minflag=1;          /* mouse in flag */

int inkey(int mode)     /* TWO mode.. WAIT & NO_WAIT */
{
    int ch;

    if (mode==WAIT) {
		while(!bioskey(1));
		if ((ch=getch())==0) return (256+getch());
		return(ch);
    }
    if (minflag) {
            ch=minkey();
            if (ch!=NOKEY) return ch;
    }
    if (!bioskey(1)) return NOKEY;
    if ((ch=getch())==0) return (256+getch());
    return(ch);
}
int minkey()
{
        if (!hgGetMouse()) return NOKEY;

        if (hgLeftMouse()) return MOUSE_LEFT;
        if (hgRightMouse()) return MOUSE_RIGHT;
        if (hgMiddleMouse()) return MOUSE_MIDDLE;

        return NOKEY;
}
int windelay(int x1,int y1,int x2,int y2)
{
        int ch;
        int mx,my;

        y2&=0xff;       /* remove waring */

        if (hgLeftMouse()) {
                hgGetMousePos(&mx,&my);
                if (mx>x1+8 && mx<x1+24 && my>y1+8 && my<y1+28) {
                        hgHideMouse();
                        putmend(x1+8,y1+8);
                        hgShowMouse();
                        while (hgLeftMouse());
                        hgHideMouse();
                        putmark(x1+8,y1+8);
                        hgShowMouse();
                        return LEFTMARK;
                }
                else if (mx>x2-24 && mx<x2-8 && my>y1+8 && my<y1+28) {
                        hgHideMouse();
                        putmend_R(x2-24,y1+8);
                        hgShowMouse();
                        while (hgLeftMouse());
                        hgHideMouse();
                        putmark_R(x2-24,y1+8);
                        hgShowMouse();
                        return RIGHTMARK;
                }
        }
        ch=inkey(NOWAIT);
        return ch;
}
void hgMouseInFlagOff()
{
        minflag=0;
}
void hgMouseInFlagOn()
{
        minflag=1;
}
