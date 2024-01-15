/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Get PCX Header              */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <stdio.h>
#include <alloc.h>
#include "hgpcx.h"

int is256pcx(FILE *fpt)
{
        int  ch;

        fseek(fpt,-769L,2);
        ch=fgetc(fpt);
        if (ch!=12) return 0;
        else return 1;
}

void getpcxhdrfpt(PCXHDR *p,FILE *fpt)
{
        fseek(fpt,0,0);
        fread(p,128,1,fpt);
}
int getpcxmodefpt(FILE *fpt)
{
        PCXHDR *p;

        p=(PCXHDR *)malloc(sizeof(PCXHDR));

        getpcxhdrfpt(p,fpt);
        if (p->Manufacturer!=0x0a) { free(p);return NO_PCX; }
	if (p->HRes==720 && p->VRes==348) { free(p);return PCX_HERCULES; }
        if (p->NPlanes==4) { free(p);return PCX_VGA16; }
        if (is256pcx(fpt)) { free(p);return PCX_VGA256; }
        free(p);
        return NO_PCX;
}
int hgGetPCXMode(char *fname)
{
        FILE *fpt;
        int   value;

        fpt=fopen(fname,"rb");
        value=getpcxmodefpt(fpt);
        fclose(fpt);
        return value;
}
void hgGetPCXHdr(PCXHDR *p,char *fname)
{
        FILE *fpt;
        int   value;

        fpt=fopen(fname,"rb");
        getpcxhdrfpt(p,fpt);
        fclose(fpt);
}
