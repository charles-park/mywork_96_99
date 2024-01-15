/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          ET3000 PageRoutine            */
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
extern void et3000();
extern int  ModeSet[];

void et3000init()
{
        int mode[]={ 0x12,0x29,-1,0x10,0x0e,0,0,0x2e,0x30 };
        int i;

        hgSetPageInit(&et3000);

        for (i=0;i<9;i++)
                ModeSet[i]=mode[i];
}
