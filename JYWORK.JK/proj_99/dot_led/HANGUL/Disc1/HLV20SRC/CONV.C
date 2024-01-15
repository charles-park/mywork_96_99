/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         Virtual Memory System          */
/*                                        */
/*         "Conventional Memory"          */
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

/* buff number limit */

#define MAX_CON_BUFF 50

byte *conbuff[MAX_CON_BUFF];
int   conpos=0;                 /* init pos 1       */

struct con {
	int logichandle;
	unsigned segment;
	unsigned offset;
	unsigned size;
} con_table[MAX_CON_BUFF] = { {0,0,0,0}, };

int  findcontable(int logichandle)
{
        int i;

        for (i=0;i<MAX_CON_BUFF;i++)
                if (con_table[i].logichandle==logichandle) return i;
        return -1;
}

VMEM *hgVMallocCON(unsigned size)
{
        VMEM *p;
        int   i;
        int   l;
	unsigned seg;
	unsigned off;

        if (!conflag) return NULL;
        l=findlogichandle();
        if (l==0) return NULL;          /* error */

        if (!vfmode)
                if (consize< (unsigned long)size) {
                        deletelogichandle(l);
                        return NULL;
                }

        consize-=(unsigned long)size;

        p=(VMEM *)malloc(sizeof(VMEM));
        p->type=CON;
        p->logichandle=l;

	for (i=0;i<MAX_CON_BUFF;i++)
		if (con_table[i].logichandle==0) break;

	if (i==MAX_CON_BUFF) {
		free(p);
		deletelogichandle(l);
                consize+=(unsigned long)size;
                return NULL;
	}
	conpos=i;
        conbuff[conpos]=(byte *)malloc((size_t)size);
        if (conbuff[conpos]==NULL) {
                deletelogichandle(l);
                free(p);
                consize+=(unsigned long)size;
                return NULL;
        }

        seg=FP_SEG(conbuff[conpos]);
        off=FP_OFF(conbuff[conpos]);

        con_table[conpos].logichandle=l;
	con_table[conpos].segment=(unsigned) seg;
	con_table[conpos].offset =(unsigned) off;
	con_table[conpos].size   =(unsigned) size;

        return p;
}
void hgVFreeCON(VMEM **p)
{
        int i;

	i=findcontable((*p)->logichandle);
	if (i==-1) return;
        deletelogichandle(con_table[i].logichandle);
	con_table[i].logichandle=0;     /* free */
        free(conbuff[i]);
        free(*p);
        consize+=(unsigned long)con_table[i].size;
}
void hgVSetBuffCON(VMEM *p)
{
        int i;

        i=findcontable(p->logichandle);
	movedata(con_table[i].segment,con_table[i].offset,vgseg,vgoff,con_table[i].size);
}
void hgVSaveBuffCON(VMEM *p)
{
	int i;

	i=findcontable(p->logichandle);
	movedata(vgseg,vgoff,con_table[i].segment,con_table[i].offset,con_table[i].size);
}
unsigned long hgVGetFreeCON()
{
	return (coreleft());
}
