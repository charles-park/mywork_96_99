/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         Virtual Memory System          */
/*                                        */
/*          "Virtual Management"          */
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
#include <alloc.h>
#include <dir.h>
#include <stdio.h>

#define MAX_LOGICHANDLE         500

byte     logic[MAX_LOGICHANDLE] = {0, };
byte    *vgbuff;
unsigned vgseg,vgoff;
int      emmflag;
int      hardflag;
int      conflag;
int      xmsflag;
unsigned long hardsize=0L;
unsigned long consize=0L;
unsigned ftbuff;
byte     vfmode=0;

void hgVMEMInit(unsigned size)
{
	if (size<1 || size>64) size=64;
	if (size!=64) size=(unsigned)(size*1024);
	else size=0xffff;
	vgbuff=(byte *)malloc((size_t)size);
	vgseg=FP_SEG(vgbuff);
	vgoff=FP_OFF(vgbuff);
	hardflag=getdisk()-1;
	consize=(unsigned long)128*1024L;
	if (hardflag>0) hardsize=hgVGetFreeHARD();
	emmflag=hgEMM_IsInstalled();
	xmsflag=hgXMS_IsInstalled();
	if (emmflag || xmsflag) {
		ftbuff=hgEMM_GetSeg();
		consize=(unsigned long)64*1024L;
	}
	conflag=1;      /* enable */
	if (consize>(unsigned long)coreleft())
		consize=(unsigned long)coreleft();
}
void hgVMEMEnd()
{
	free(vgbuff);
}
VMEM *hgVMalloc(unsigned size)
{
	VMEM *p;

	p=hgVMallocCON(size);
	if (p!=NULL) return p;
	p=hgVMallocEMM(size);
	if (p!=NULL) return p;
	p=hgVMallocXMS(size);
	if (p!=NULL) return p;
	p=hgVMallocHARD(size);
	if (p!=NULL) return p;
	return NULL;
}
void  hgVFree(VMEM **p)
{
	switch((*p)->type) {
		case CON :
			hgVFreeCON(p);
			break;
		case EMM :
			hgVFreeEMM(p);
			break;
		case XMS :
			hgVFreeXMS(p);
			break;
		case HARD:
			hgVFreeHARD(p);
			break;
	};
}
void  hgVSetBuff(VMEM *p)
{
	switch(p->type) {
		case CON :
			hgVSetBuffCON(p);
			break;
		case EMM :
			hgVSetBuffEMM(p);
			break;
		case XMS :
			hgVSetBuffXMS(p);
			break;
		case HARD:
			hgVSetBuffHARD(p);
			break;
	};
}
void  hgVSaveBuff(VMEM *p)
{
	switch(p->type) {
		case CON :
			hgVSaveBuffCON(p);
			break;
		case EMM :
			hgVSaveBuffEMM(p);
			break;
		case XMS :
			hgVSaveBuffXMS(p);
		case HARD:
			hgVSaveBuffHARD(p);
			break;
	};
}
void hgSetEMMOff()
{
	emmflag=0;
}
void hgSetEMMOn()
{
	emmflag=1;
}
void hgSetXMSOff()
{
	xmsflag=0;
}
void hgSetXMSOn()
{
	xmsflag=1;
}
void hgSetHARDOff()
{
	hardflag=0;
}
void hgSetHARDOn()
{
	hardflag=1;
}
void hgSetCONOff()
{
	conflag=0;
}
void hgSetCONOn()
{
	conflag=1;
}
void hgSetCONLimit(unsigned long limit)
{
	consize=(unsigned long)limit;
}
void hgForceCON()
{
	vfmode=1;
}
void hgAutoCON()
{
	vfmode=0;
}
int findlogichandle()
{
	int i;

	for (i=0;i<MAX_LOGICHANDLE;i++)
		if (logic[i]==0) break;
	if (i==MAX_LOGICHANDLE) return 0;
	logic[i]=1;	/* logic handle */
	return i+1;     /* logic handle starts from 1 */
}
void deletelogichandle(int logichandle)
{
	logic[logichandle-1]=0;
}
