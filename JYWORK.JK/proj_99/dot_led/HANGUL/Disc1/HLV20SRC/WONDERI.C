/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*        VGA Wonder PageRoutine          */
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
extern void wonder1();
extern void wonder_special();
extern int  ModeSet[];
extern void (*svgaspec)();
extern int  specflag;

void wonderinit()
{
        int mode[]={ 0x12,0x54,-1,0x10,0x0e,0,0,0x62,0x63 };
	int i;
	unsigned char ch;
        char far *Version=(char far *)0xc0000000;

        hgSetPageInit(&wonder1);

        for (i=0;i<9;i++)
		ModeSet[i]=mode[i];

        if (Version[0x43]!='1') {
                svgaspec=&wonder_special;
                specflag=1;
        }
}
