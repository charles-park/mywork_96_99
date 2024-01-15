/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          Text Attribute output         */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <ctype.h>

#include "hginit.h"
#include "hghlib.h"

void textattr_real_out(char *str,int *pos);
extern void (*atps)(char *str,int *pos);
extern void englishout_process(char *str,int pos);
extern char textattrflag;                         /* default is no */

void hgTextAttrInit()
{
        atps=&textattr_real_out;
        textattrflag=1;
}

void textattr_real_out(char *str,int *pos)
{
        char *color[16]= { "ZERO","BLUE","GREEN","CYAN","RED","MAGENTA",
			   "BROWN","LIGHTGRAY","DARKGRAY","LIGHTBLUE",
                           "LIGHTGREEN","LIGHTCYAN","LIGHTRED","LIGHTMAGENTA",
			   "YELLOW","WHITE" };
	char *attr[8]= { "NORMAL","REVERSE","DIM","SHADOW","THREED",
			 "OUTLINE","UNDER","BOLD" };
        char cmd[15];
        char opr;
        int  fac=1;
        int  i;
        int  save;

        if (!textattrflag) {
                englishout_process(str,*pos);
                (*pos)++;
                return;
        }
        if (str[*pos+1]==0) {
                (*pos)++;
                return;
        }
        if (str[*pos+1]=='/') {
                englishout_process(str,*pos);
                (*pos)+=2;
                return;
        }
        for (i=*pos+2;i<strlen(str);i++) {
                if (str[i]!='/') cmd[i-*pos-2]=str[i];
                else break;
                if ((i-*pos-2)>=14) break;
        }
        opr=str[*pos+1];
        cmd[i-*pos-2]=0;

        if (str[i]=='/') save=i+1;
        else {
                englishout_process(str,*pos);
                (*pos)++;
                return;
        }

        switch(opr) {
                case 'c' :
                case 'C' :
                        for (i=0;i<16;i++)
                                if (!strcmp(color[i],cmd)) break;
                        if (i==16) {
                                englishout_process(str,*pos);
                                (*pos)++;
                                return;
                        }
                        if (hgIsHerc()) hgSetTFcolor(ZERO);
                        else hgSetTFcolor(i);
                        break;
                case 'a' :
                case 'A' :
                        for (i=0;i<8;i++) {
                                if (!strcmp(attr[i],cmd)) break;
                                fac*=2;
                        }
                        if (i==8) {
                                englishout_process(str,*pos);
                                (*pos)++;
                                return;
                        }
                        hgSetHAttr(fac);
                        hgSetEAttr(fac);
                        break;
                case 's' :
                case 'S' :
                        hgSetXFactor(cmd[0]-'0');
                        hgSetYFactor(cmd[1]-'0');
                        break;
                default  :
                        englishout_process(str,*pos);
                        (*pos)++;
                        return;
        }
        *pos=save;      /* re-assign position */
}
void hgSetTextAttrOn()
{
        textattrflag=1;
}
void hgSetTextAttrOff()
{
        textattrflag=0;
}
