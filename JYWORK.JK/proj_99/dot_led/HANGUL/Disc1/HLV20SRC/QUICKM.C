/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             Init Mouse                 */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "mouse.h"
#include "hginit.h"

extern char mouse_enable;
extern int hgx1r,hgx2r,hgy1r,hgy2r;

void hgSetHMouseInit()
{
        int flag;

        flag=init_mouse(HERC_MOUSE);
        if (flag) hgSetMouse();

        mouse_enable=hgSUCCESS;
        hgMoveMouse((hgx1r+hgx2r)/2,(hgy1r+hgy2r)/2);
        hgShowMouse();
}
void hgSetVMouseInit()
{
        int flag;

        flag=init_mouse(VGA16_MOUSE);
        if (flag) hgSetMouse();

        mouse_enable=hgSUCCESS;
        hgMoveMouse((hgx1r+hgx2r)/2,(hgy1r+hgy2r)/2);
        hgShowMouse();
}
void hgSetSMouseInit()
{
        int flag;

        flag=init_mouse(VGA256_MOUSE);
        if (flag) hgSetMouse();

        mouse_enable=hgSUCCESS;
        hgMoveMouse((hgx1r+hgx2r)/2,(hgy1r+hgy2r)/2);
        hgShowMouse();
}
void hgSetMouseInit()
{
        if (hgIsHerc()) hgSetHMouseInit();
        else hgSetVMouseInit();
}
