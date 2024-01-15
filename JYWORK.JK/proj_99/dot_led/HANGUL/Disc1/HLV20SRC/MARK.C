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

#include "hginit.h"
#include "hghlib.h"
#include "hwindow.h"
#include "hgicon.h"

HICON *mark;
HICON *mend;
HICON *markr;
HICON *mendr;

int mark_enable=hgFAIL;              /* mark enable */

void hgMarkOn()
{
        mark_enable=1;
}
void hgMarkOff()
{
        mark_enable=0;
}
void hgMarkEnd()
{
        hgHICON_Free(&mark);
        hgHICON_Free(&mend);
}
void putmark(int x,int y)
{
        if (!mark_enable) return;
        hgHICON_Draw(mark,x,y);
}
void putmend(int x,int y)
{
        if (!mark_enable) return;
        hgHICON_Draw(mend,x,y);
}
void putmark_R(int x,int y)
{
        if (!mark_enable) return;
        hgHICON_Draw(markr,x,y);
}
void putmend_R(int x,int y)
{
        if (!mark_enable) return;
        hgHICON_Draw(mendr,x,y);
}
