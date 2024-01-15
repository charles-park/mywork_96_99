/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            English Output              */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <stdarg.h>

extern void englishout_process(char *str,int pos);

void hgEngOutTextXy(int x,int y,char *str)
{
        int i;
        int xs,ys;

        xs=hgGetOutX();
        ys=hgGetOutY();

        hgGotoXy(x,y);

        for (i=0;i<strlen(str);i++)
                englishout_process(str,i);

        hgGotoXy(xs,ys);
}
void hgEngPutch(int x,int y,char num)
{
        int i;
        int xs,ys;
        char str[3];

        xs=hgGetOutX();
        ys=hgGetOutY();

        hgGotoXy(x,y);

        str[0]=num;str[1]=0;
        englishout_process(str,0);

        hgGotoXy(xs,ys);
}

void hgEngPrintfXy(int x,int y,char *fmt,...)
{
        char s[255];

        va_list argptr;
        va_start(argptr,fmt);
        vsprintf(s,fmt,argptr);

        hgEngOutTextXy(x,y,s);
}
