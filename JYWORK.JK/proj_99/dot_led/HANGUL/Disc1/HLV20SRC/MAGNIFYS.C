/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            "Magnify Set"               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

int hgYFAC=1;
int hgXFAC=1;

/* extern function */

extern void (*magh)(int x,int y,char *buff,char color);
extern void (*mage)(int x,int y,char *buff,char color);
extern void (*puth)(int x,int y,char *buff,char color);
extern void (*pute)(int x,int y,char *buff,char color);
extern void putchmn(int x,int y,char *buff,char color);

void hgSetXFactor(int n)
{
        if (n>3) return;

        hgXFAC=n;

        if ((hgXFAC+hgYFAC)>2) {
                puth=&putchmn;
                pute=&putchmn;
        } else {
                puth=magh;
                pute=mage;
        }
}
void hgSetYFactor(int n)
{
        if (n>4) return;

        hgYFAC=n;

        if ((hgXFAC+hgYFAC)>2) {
                puth=&putchmn;
                pute=&putchmn;
        } else {
                puth=magh;
                pute=mage;
        }
}
int  hgGetXFactor()
{
        return hgXFAC;
}
int  hgGetYFactor()
{
        return hgYFAC;
}
