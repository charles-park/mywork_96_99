/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           EMM management               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hgvir.h"
#include <dos.h>
#include <stdio.h>
#include <alloc.h>

/* some definition */

#define hgSUCCESS     1
#define hgFAIL        0

#define MAX_EMM_HANDLE 255

/* emm structure */

struct emm {
        int logichandle;
        int logicalpage;
        unsigned int emmhandle;
} emm_table[MAX_EMM_HANDLE] = { {0,0,0}, };
int  emmpos=0;

int   findemmhandle(int logichandle)
{
        int i;

	for (i=0;i<MAX_EMM_HANDLE;i++)
		if (emm_table[i].logichandle==logichandle) return i;
        return 0;
}
void  mappingpages(VMEM *p,int logic)
{
	int j;

        j=findemmhandle(p->logichandle);
        hgEMM_MapPages(emm_table[j].emmhandle,0,logic);
}
VMEM *hgVMallocEMM(unsigned size)
{
        int i,l;
        unsigned pages;
        unsigned emmhandle;
        VMEM *p;
        unsigned fpages;

        if (!emmflag) return NULL;

        pages=(size/16/1024)+1;
        if (!hgEMM_IsEnoughPages(pages)) return NULL;

	l=findlogichandle();
        if (l==0) return NULL;

        for (i=0;i<MAX_EMM_HANDLE;i++)
                if (emm_table[i].logichandle==0) break;
	if (i==MAX_EMM_HANDLE) {
		deletelogichandle(l);
		return NULL;
	}
        emmpos=i;
	if (!hgEMM_AllocatePages(pages,&emmhandle)) {
		deletelogichandle(l);
		return NULL;
        }

        p=(VMEM *)malloc(sizeof(VMEM));

        p->type=EMM;
        p->logichandle=l;

        emm_table[emmpos].logichandle=l;
	emm_table[emmpos].logicalpage=pages-1;
	emm_table[emmpos].emmhandle  =emmhandle;

        return (VMEM *)p;
}

void  hgVFreeEMM(VMEM **p)
{
        int i;

        i=findemmhandle((*p)->logichandle);
        hgEMM_FreeHandle(emm_table[i].emmhandle);
        deletelogichandle((*p)->logichandle);
        emm_table[i].logichandle=0;
        free(*p);
}
void  hgVSetBuffEMM(VMEM *p)
{
	int i,j;
        unsigned offset=vgoff;

	i=findemmhandle(p->logichandle);

        for (j=0;j<=emm_table[i].logicalpage;j++) {
                mappingpages(p,j);
		movedata(ftbuff,0,vgseg,offset,0x4000);
                offset+=0x4000;
	}
}
void  hgVSaveBuffEMM(VMEM *p)
{
	int i,j;
        unsigned offset=vgoff;

	i=findemmhandle(p->logichandle);

        for (j=0;j<=emm_table[i].logicalpage;j++) {
                mappingpages(p,j);
		movedata(vgseg,offset,ftbuff,0,0x4000);
                offset+=0x4000;
        }
}
