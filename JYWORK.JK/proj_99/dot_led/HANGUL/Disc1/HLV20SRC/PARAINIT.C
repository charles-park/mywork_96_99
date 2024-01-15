/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        PARADISE PageRoutine            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

extern void hgSetPageInit(void (*func)());
extern void paradise();
extern void paradise_special();
extern void (*svgaspec)();
extern int  ModeSet[];
extern int  specflag;

void paradiseinit()
{
        int mode[]={ 0x12,0x58,-1,0x10,0x0e,0,0,0x5e,0x5c };
        int i;

        hgSetPageInit(&paradise);

        for (i=0;i<9;i++)
                ModeSet[i]=mode[i];

        svgaspec=&paradise_special;
        specflag=1;                     /* special process on */
}
