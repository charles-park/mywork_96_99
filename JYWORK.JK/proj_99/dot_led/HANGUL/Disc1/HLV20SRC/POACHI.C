/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           POACH PageRoutine            */
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
extern void poach();
extern void poach_special();
extern void (*svgaspec)();
extern int  specflag;

extern int  ModeSet[];

void poachinit()
{
        int mode[]={ 0x12,0x5b,-1,0x10,0x0e,0,0,0x5d,0x5e };
        int i;

        hgSetPageInit(&poach);

        for (i=0;i<9;i++)
                ModeSet[i]=mode[i];

        svgaspec=&poach_special;
        specflag=1;
}
