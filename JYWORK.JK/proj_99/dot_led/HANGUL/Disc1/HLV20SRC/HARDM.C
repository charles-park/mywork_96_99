/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         Virtual Memory System          */
/*                                        */
/*             "Hard Disk"                */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

/* include */

#include "hgvir.h"
#include <alloc.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>

/* buff number limit */

#define MAX_HARD_BUFF 100

byte hardfile[MAX_HARD_BUFF][12];
int  hardpos=0;                 /* init pos         */
byte swapdir[50]={ 0, };        /* swap directory   */

struct hard {
	int logichandle;
        unsigned size;
} hard_table[MAX_HARD_BUFF] = { {0,0}, };

int  findhardtable(int logichandle)
{
        int i;

	for (i=0;i<MAX_HARD_BUFF;i++)
                if (hard_table[i].logichandle==logichandle) return i;
        return -1;
}

VMEM *hgVMallocHARD(unsigned size)
{
        VMEM *p;
        int   i;
        char  temp[30];
        int   l;

        if (hardflag<1) return NULL;

        if (hardsize<(unsigned long)size) return NULL;
        hardsize-=(unsigned long)size;

        l=findlogichandle();
        if (l==0) {
                hardsize+=(unsigned long)size;
                return NULL;          /* error */
        }

        p=(VMEM *)malloc(sizeof(VMEM));
        p->type=HARD;
        p->logichandle=l;

	for (i=0;i<MAX_HARD_BUFF;i++)
                if (hard_table[i].logichandle==0) break;

	if (i==MAX_HARD_BUFF) {
                free(p);
                deletelogichandle(l);
                hardsize+=(unsigned long)size;
                return NULL;
        }
        hardpos=i;
        sprintf(hardfile[hardpos],"$%d.$$$",hardpos);
        hard_table[hardpos].logichandle=l;
        hard_table[hardpos].size       =(unsigned) size;

        return p;
}
void hgVFreeHARD(VMEM **p)
{
        int i;
        char temp[50];

        i=findhardtable((*p)->logichandle);
	if (i==-1) return;
        deletelogichandle((*p)->logichandle);
        hard_table[i].logichandle=0;     /* free */

        if (!swapdir[0]) strcpy(temp,hardfile[i]);
        else {
                strcpy(temp,swapdir);
                strcat(temp,hardfile[i]);
        }

        unlink(temp);             /* delete swap file */
        hardsize+=(unsigned long)hard_table[i].size;
        free(*p);
}
void hgVSetBuffHARD(VMEM *p)
{
        int  i;
        FILE *fpt;
        char temp[50];

        i=findhardtable(p->logichandle);
        if (!swapdir[0]) strcpy(temp,hardfile[i]);
        else {
                strcpy(temp,swapdir);
                strcat(temp,hardfile[i]);
        }
        fpt=fopen(temp,"rb");
	fread(vgbuff,hard_table[i].size,1,fpt);
        fclose(fpt);
}
void hgVSaveBuffHARD(VMEM *p)
{
	int i;
        FILE *fpt;
        char temp[50];

        i=findhardtable(p->logichandle);
        if (!swapdir[0]) strcpy(temp,hardfile[i]);
        else {
                strcpy(temp,swapdir);
                strcat(temp,hardfile[i]);
        }
        fpt=fopen(temp,"wb");
	fwrite(vgbuff,hard_table[i].size,1,fpt);
        fclose(fpt);
}

unsigned long hgVGetFreeHARD()
{
        struct dfree dsk;
        unsigned long size;

	getdfree(hardflag+2,&dsk);
	size=(unsigned long)dsk.df_avail*dsk.df_sclus*dsk.df_bsec;
        return size;
}

void hgSetSwapDir(char *direc)
{
        FILE *fpt;
        char temp[50];
	char  drive[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];

        strcpy(temp,direc);
        strcat(temp,"$$$.$$$");       /* test file */

        fpt=fopen(temp,"wb");
        if (fpt==NULL) return;
        fclose(fpt);
        unlink(temp);
        strcpy(swapdir,direc);

        fnsplit(direc,drive,dir,name,ext);

        if (strcmp(drive,"")) {
                hardflag=toupper(drive[0])-'B';
                hardsize=hgVGetFreeHARD();
        }
}
