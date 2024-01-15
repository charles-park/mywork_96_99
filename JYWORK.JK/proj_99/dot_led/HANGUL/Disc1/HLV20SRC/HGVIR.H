/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         Virtual Memory System          */
/*                                        */
/*            "Sub Header.."              */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#ifndef __HGVIR_H
#define __HGVIR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;

/* Virtuam Memory Type */

typedef struct vmem {
        int logichandle;
        int type;
} VMEM;

typedef struct phy {
	unsigned int segment;
	unsigned int pnum;
} PHYSICAL_PAGE;

/* graphic vimage type */

#define MAXPAGES 12

typedef struct vimage {
	VMEM *buff[MAXPAGES];
} VIMAGE;

typedef struct cimage {
	char *buff[MAXPAGES];
} CIMAGE;

#define CON     1
#define EMM     2
#define HARD    3

extern byte    *vgbuff;            /* virtual global buff */
extern unsigned vgseg;             /* buff segment        */
extern unsigned vgoff;             /* buff offset         */
extern int      emmflag;
extern int      hardflag;
extern int      conflag;
extern unsigned long hardsize;
extern unsigned long consize;
extern unsigned ftbuff;            /* expanded segment    */
extern byte     vfmode;

/* prototype */

VMEM *hgVMallocCON(unsigned size);
void  hgVFreeCON(VMEM **p);
void  hgVSetBuffCON(VMEM *p);
void  hgVSaveBuffCON(VMEM *p);
VMEM *hgVMallocHARD(unsigned size);
void  hgVFreeHARD(VMEM **p);
void  hgVSetBuffHARD(VMEM *p);
void  hgVSaveBuffHARD(VMEM *p);
VMEM *hgVMallocEMM(unsigned size);
void  hgVFreeEMM(VMEM **p);
void  hgVSetBuffEMM(VMEM *p);
void  hgVSaveBuffEMM(VMEM *p);

/* automatic function */

VMEM *hgVMalloc(unsigned size);         /* size ( in Byte ) */
void  hgVFree(VMEM **p);
void  hgVSetBuff(VMEM *p);
void  hgVSaveBuff(VMEM *p);

/* initial & end function */

void  hgVMEMInit(unsigned size);        /* size ( in KB ) */
void  hgVMEMEnd(void);
void  hgFreeCIMAGE(CIMAGE **p);
void  hgFreeVIMAGE(VIMAGE **p);

/* special function */

unsigned long hgVGetFreeHARD(void);
unsigned long hgVGetFreeCON(void);
void          hgSetSwapDir(char *fname);

void hgSetEMMOff(void);
void hgSetEMMOn(void);
void hgSetHARDOff(void);
void hgSetHARDOn(void);
void hgSetCONOff(void);
void hgSetCONOn(void);
void hgSetCONLimit(unsigned long limit);
void hgForceCON(void);
void hgAutoCON(void);

/* emm function */

char hgEMM_IsInstalled(void);
char hgEMM_IsEnoughPages(int pages);
int  hgEMM_GetFreePages(void);
int  hgEMM_GetTotalPages(void);
char hgEMM_AllocatePages(int pages,unsigned int *emm_handle);
char hgEMM_MapPages(unsigned int handle,int ppage,int lpage);
char hgEMM_GetAddress(char **pf_ptr);
char hgEMM_FreeHandle(unsigned int emm_handle);
char hgEMM_GetVersion(char *s);
int  hgEMM_GetHandleCount(void);
int  hgEMM_GetHandlePages(unsigned int handle);
int  hgEMM_GetPhysicalNumber(void);
int  hgEMM_GetPhysicalTable(PHYSICAL_PAGE *p);
unsigned hgEMM_GetSeg(void);

#ifdef __cplusplus
}
#endif

#endif
