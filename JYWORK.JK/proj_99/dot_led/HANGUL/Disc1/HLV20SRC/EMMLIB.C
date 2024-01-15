/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           EMM management               */
/*               Library                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <dos.h>
#include <stdio.h>
#include <alloc.h>
#include "hgvir.h"

/* global register define */

union  REGS  inr,outr;
struct SREGS segr;

/* some definition */

#define hgSUCCESS     1
#define hgFAIL        0

#define MAX_EMM_HANDLE 255

char hgEMM_IsInstalled()
{
        char *device="EMMXXXX0";
        char far *int67name;
        char temp[8];
	int  i;

        inr.h.ah=0x35;
        inr.h.al=0x67;
        intdosx(&inr,&outr,&segr);

	int67name=MK_FP(segr.es,0x0a);

        for (i=0;i<8;i++)
		temp[i]=int67name[i];

        if (memcmp(device,temp,8)==0)
                return (hgSUCCESS);
	else
                return (hgFAIL);
}

char hgEMM_IsEnoughPages(int pages)
{
        inr.h.ah=0x42;
        int86(0x67,&inr,&outr);
        if (outr.h.ah!=0 || pages>outr.x.bx)
                return (hgFAIL);
	else
                return (hgSUCCESS);
}
int  hgEMM_GetFreePages()
{
        inr.h.ah=0x42;
        int86(0x67,&inr,&outr);
        if (outr.h.ah!=0) return hgFAIL;        /* 0 is fail */
        return (outr.x.bx);
}
int  hgEMM_GetTotalPages()
{
        inr.h.ah=0x42;
        int86(0x67,&inr,&outr);
        if (outr.h.ah!=0) return hgFAIL;        /* 0 is fail */
        return (outr.x.dx);
}
char hgEMM_AllocatePages(int pages,unsigned int *emm_handle)
{
	inr.h.ah=0x43;
        inr.x.bx=pages;
        int86(0x67,&inr,&outr);
        if (outr.h.ah==0) {
		*emm_handle=outr.x.dx;
                return (hgSUCCESS);
	}
	else
                return (hgFAIL);
}

char hgEMM_MapPages(unsigned int handle,int ppage,int lpage)
{
        inr.h.ah=0x44;
        inr.h.al=ppage;
        inr.x.bx=lpage;
        inr.x.dx=handle;
        int86(0x67,&inr,&outr);
        if (outr.h.ah==0)
                return (hgSUCCESS);
	else
                return (hgFAIL);
}

char hgEMM_GetAddress(char **pf_ptr)
{
        inr.h.ah=0x41;
        int86(0x67,&inr,&outr);
        if (outr.h.ah!=0)
                return (hgFAIL);
	else
                *pf_ptr=MK_FP(outr.x.bx,0);
        return (hgSUCCESS);
}
unsigned hgEMM_GetSeg()
{
        inr.h.ah=0x41;
        int86(0x67,&inr,&outr);
        if (outr.h.ah!=0)
                return (hgFAIL);
	else
                return (unsigned)outr.x.bx;
}

char hgEMM_FreeHandle(unsigned int emm_handle)
{
        inr.h.ah=0x45;
        inr.x.dx=emm_handle;
	int86(0x67,&inr,&outr);
        if (outr.h.ah==0)
                return (hgSUCCESS);
	else
                return (hgFAIL);
}
char hgEMM_GetVersion(char *s)
{
	inr.h.ah=0x46;
	int86(0x67,&inr,&outr);
	if (outr.h.ah!=0) return hgFAIL;
	sprintf(s,"%d.%d\0",(outr.h.al&0xf0)>>4,outr.h.al&0x0f);
	return hgSUCCESS;
}
int  hgEMM_GetHandleCount()
{
	inr.h.ah=0x4b;
	int86(0x67,&inr,&outr);
	if (!outr.h.ah) return outr.x.bx;
	return hgFAIL;
}
int  hgEMM_GetHandlePages(unsigned int handle)
{
	inr.h.ah=0x4c;
	inr.x.dx=handle;
	int86(0x67,&inr,&outr);
	if (!outr.h.ah) return outr.x.bx;
	return hgFAIL;
}

int  hgEMM_GetPhysicalNumber()
{
	inr.x.ax=0x5801;
	int86x(0x67,&inr,&outr,&segr);

	if (!outr.h.ah) return outr.x.cx;
	return hgFAIL;
}
int  hgEMM_GetPhysicalTable(PHYSICAL_PAGE *p)
{
	inr.x.ax=0x5800;
	segr.es=FP_SEG(p);
	inr.x.di=FP_OFF(p);
	int86x(0x67,&inr,&outr,&segr);

	if (!outr.h.ah) return outr.x.cx;
	return hgFAIL;
}

