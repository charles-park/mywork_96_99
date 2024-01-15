/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           AHEAD PageRoutine            */
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
extern void ahead();
extern int  ModeSet[];

void aheadinit()
{
        int mode[]={ 0x12,0x6a,0x63,0x10,0x0e,0,0,0x61,0x62 };
        int i;

        hgSetPageInit(&ahead);

        for (i=0;i<9;i++)
                ModeSet[i]=mode[i];
}
