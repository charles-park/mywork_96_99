/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           VGA Mouse Routine            */
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
#include "mouse.h"

#include <dos.h>

#define PASSVALUE       if (!hgGetMouse()) return hgFAIL
#define PASS            if (!hgGetMouse()) return

int hgLeftMouse()
{
        PASSVALUE;
        return is_left_button();
}
int hgRightMouse()
{
        PASSVALUE;
        return is_right_button();
}
int hgMiddleMouse()
{
        PASSVALUE;
        return is_middle_button();
}
void hgGetMousePos(int *x,int *y)
{
        *x=get_mouse_x();
        *y=get_mouse_y();
}
void hgShowMouse()
{
        PASS;
        show_mouse();
}
void hgHideMouse()
{
        PASS;
        hide_mouse();
}
void hgMoveMouse(int x,int y)
{
        PASS;
        set_mouse_position(x,y);
}
