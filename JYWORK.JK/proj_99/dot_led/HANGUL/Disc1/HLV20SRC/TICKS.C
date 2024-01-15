/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Timer Interrupt             */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include<dos.h>
#include<stdlib.h>

void interrupt (*oldvec)();
volatile long ticks;

void   interrupt tickisr(void);

void   hgTimerInstall(void);
void   hgTimerUninstall(void);
void   hgSetTimerZero(void);
long   hgGetTimerTicks(void);
double hgGetElapsedTime(void);

void interrupt tickisr(void)
{
	ticks++;
	if (oldvec)
		(*oldvec)();
}

void hgTimerInstall(void)
{
	oldvec=getvect(0x1c);
	setvect(0x1c,tickisr);
        ticks=0L;
}
void hgTimerUninstall(void)
{
        setvect(0x1c,oldvec);
}
double hgGetElapsedTime(void)
{
	return((double)ticks/18.2044);
}
long hgGetTimerTicks(void)
{
        return ticks;
}
void hgSetTimerZero(void)
{
        ticks=0L;
}
