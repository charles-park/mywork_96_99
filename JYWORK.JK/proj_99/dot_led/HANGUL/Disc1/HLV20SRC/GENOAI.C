/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           GENOA PageRoutine            */
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
extern void genoa();
extern int  ModeSet[];

void genoainit()
{
        int mode[]={ 0x12,0x79,-1,0x10,0x0e,0,0,0x5c,0x5e };
        int i;

        hgSetPageInit(&genoa);

        for (i=0;i<9;i++)
                ModeSet[i]=mode[i];
}
