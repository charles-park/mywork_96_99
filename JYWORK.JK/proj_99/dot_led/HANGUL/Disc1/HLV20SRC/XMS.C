/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            XMS management              */
/*                                        */
/*        made by Jung Hyun-taek          */
/*                                        */
/*           HITEL ID : helpme            */
/*                                        */
/*  ; Thank you for this good routine     */
/*  ; - Han Woo-jin                       */
/*                                        */
/*========================================*/

#pragma inline

#include "hgvir.h"
#include <dos.h>
#include <alloc.h>
#include <stdio.h>

union REGS xmsinr;

#define hgSUCCESS 1
#define hgFAIL    0

#define MAX_XMS_HANDLE 255

int xmspos;
unsigned int xms_addr[2];

/* xms structure */
struct xms { int logichandle;
	     unsigned long size;
	     unsigned int  xmshandle;
	   } xms_table[MAX_XMS_HANDLE] = { {0,0,0}, };

void hgXMS_SetAddress()
{
    struct REGPACK dosr;

    dosr.r_ax = 0x4310;

    intr( 0x2f,&dosr );
    xms_addr[0] = dosr.r_bx;       /* offset  */
    xms_addr[1] = dosr.r_es;       /* segment */
}

void hgXMS_Call()
{
    unsigned int xms_jump[2];
    unsigned int t_ax,t_bx,t_dx;

    xms_jump[0] = xms_addr[0];   xms_jump[1] = xms_addr[1];

    asm {
	   push ax;   push bx;  push dx;
	   push si;   push ds;

	   mov ax,[xmsinr.x.ax]
	   mov bx,[xmsinr.x.bx]
	   mov dx,[xmsinr.x.dx]
	   mov si,[xmsinr.x.si]
	   mov ds,[xmsinr.x.cx]    /* for data move  : rcx -> rds */
	   call dword ptr [xms_jump]
	   pop ds;     pop si;
	   mov [t_ax],ax
	   mov [t_dx],dx
	   mov [t_bx],bx

	   pop dx;    pop bx;    pop ax;
	}

    xmsinr.x.ax = t_ax;    xmsinr.x.bx = t_bx;
    xmsinr.x.dx = t_dx;
}

char hgXMS_IsInstalled()
{
     xmsinr.x.ax = 0x4300;

     int86( 0x2f,&xmsinr,&xmsinr );

     if (xmsinr.h.al != 0x80) return (hgFAIL);

     hgXMS_SetAddress();
     return (hgSUCCESS);
}

unsigned int hgXMS_FreeSize(unsigned realsize)          /* return Kbyte  */
{
     xmsinr.h.ah = 0x08;

     hgXMS_Call();

     if (xmsinr.h.bl == 0x80 || xmsinr.h.bl == 0x81 || xmsinr.h.bl == 0x0a0)
	return hgFAIL;
     /*   80h   This function isn't used.
	  81h   find VDISK
	  a0h   EMB is FULL.  */

     if ( (unsigned)xmsinr.x.ax < realsize )  return hgFAIL;
     return xmsinr.x.ax;
}

char hgXMS_AllocateBuff(unsigned size,unsigned *handle)
{
     xmsinr.h.ah = 0x09;
     xmsinr.x.dx = size;

     hgXMS_Call();

     if (xmsinr.x.ax == 0x00 ) return hgFAIL;
     /*  ax:  0001h   success
	      0000h   fail
	 error code
	 bl:    80h   This function isn't used.
		81h   find VDISK
		a0h   EMB is FULL
		a1h   EMB Handle is FULL    */

     *handle = xmsinr.x.dx;

     return hgSUCCESS;
}

char hgXMS_FreeHandle ( unsigned handle )
{
     xmsinr.h.ah = 0x0a;
     xmsinr.x.dx = handle;

     hgXMS_Call();

     if ( xmsinr.x.ax == 0x00 )  return hgFAIL;
     /*  ax:  0001h   success
	      0000h   fail
	 error code
	 bl:    80h   This function isn't used.
		81h   find VDISK
		a0h   EMB is FULL
		a1h   EMB Handle is FULL
		a2h   Handle is fault
		abh   Handle is locked  */
     return hgSUCCESS;
}

char hgXMS_Lock (unsigned int handle,unsigned int *xms_seg,unsigned int *xms_offset)
{
     xmsinr.h.ah = 0x0c;
     xmsinr.x.dx = handle;

     hgXMS_Call();

     *xms_offset = xmsinr.x.dx;    *xms_seg = xmsinr.x.bx;

     if ( xmsinr.x.ax == 0x00 )  return hgFAIL;
     /*  ax:  0001h   success
	      0000h   fail
	 error code
	 bl:    80h   This function isn't used.
		81h   find VDISK
		a2h   Handle is fault
		ach   Block Number is FULL
		adh   Lock is Fail           */
     return hgSUCCESS;
}

char hgXMS_Unlock (unsigned int handle)
{
     xmsinr.h.ah = 0x0d;
     xmsinr.x.dx = handle;

     hgXMS_Call();

     if ( xmsinr.x.ax == 0x00 )  return hgFAIL;
     /*  ax:  0001h   success
	      0000h   fail
	 error code
	 bl:    80h   This function isn't used.
		81h   find VDISK
		a2h   Handle is fault
		aah   Block isn't Lock            */
     return hgSUCCESS;
}

char hgXMS_Move( unsigned long length,
		 unsigned int sh,unsigned int sseg,unsigned int soff,
		 unsigned int dh,unsigned int dseg,unsigned int doff)
{   struct { unsigned long length;
	     unsigned int  sh;
	     unsigned long soffset;
	     unsigned int  dh;
	     unsigned long doffset;
	   } xms_buff = { 0,0,0,0,0 };
    unsigned int p_off,p_seg;

     xms_buff.length = (unsigned long)length;
     xms_buff.sh     = sh;
     xms_buff.soffset= (unsigned long)sseg*0x10000 + (unsigned long)soff;
     xms_buff.dh     = dh;
     xms_buff.doffset= (unsigned long)dseg*0x10000 + (unsigned long)doff;

     p_off = FP_OFF( &xms_buff );  p_seg = FP_SEG( &xms_buff );

     xmsinr.h.ah = 0x0b;
     xmsinr.x.cx = p_seg;
     xmsinr.x.si = p_off;

     hgXMS_Call();

     if ( xmsinr.x.ax == 0x00 )  return hgFAIL;
     /*  ax:  0001h   success
	      0000h   fail
	 error code
	 bl:    80h   This function isn't used.
		81h   find VDISK
		82h   A20 error
		a3h   source handle fault
		a4h   source offset fault
		a5h   dest   handel fault
		a6h   dest   offset fault
		a7h   length        fault
		a8h   overlap
		a9h   parity error          */

     return hgSUCCESS;
}

int findxmshandle( int logichandle )
{
	int i;

	for (i=0;i<MAX_XMS_HANDLE;i++)
	    if ( xms_table[i].logichandle == logichandle ) return i;
	return 0;
}

VMEM *hgVMallocXMS( unsigned size )
{
      VMEM *p;
      unsigned int realsize,xmshandle;
      int l,i;
      unsigned int xms_seg,xms_off;

      if (!xmsflag) return NULL;

      if ( (size%2) == 1 ) size -= 1;
      realsize = (size/1024) + 1;
      if (!hgXMS_FreeSize(realsize)) return NULL;

      l = findlogichandle();
      if (l==0) return NULL;

      for (i=0;i<MAX_XMS_HANDLE;i++)
	  if ( xms_table[i].logichandle==0 ) break;
      if (i==MAX_XMS_HANDLE)
      {   deletelogichandle(l);
	  return NULL;
      }

      xmspos = i;
      if ( !hgXMS_AllocateBuff(realsize,&xmshandle) )
      {  deletelogichandle(l);
	 return NULL;
      }

      if ( !hgXMS_Lock( xmshandle,&xms_seg,&xms_off ) )
      {  hgXMS_FreeHandle ( xms_table[i].xmshandle );
	 return NULL;
      }

      p = (VMEM *)malloc(sizeof(VMEM));
      if (p==NULL) return NULL;
      p->type = XMS;
      p->logichandle = l;

      xms_table[xmspos].logichandle = l;
      xms_table[xmspos].size        = (unsigned long)size;
      xms_table[xmspos].xmshandle   = xmshandle;

      return (VMEM *)p;
}

void hgVFreeXMS( VMEM **p )
{
     int i;

     i = findxmshandle( (*p)->logichandle );

     hgXMS_Unlock ( xms_table[i].xmshandle );
     hgXMS_FreeHandle ( xms_table[i].xmshandle );


     deletelogichandle( (*p)->logichandle );
     xms_table[i].logichandle = 0;
     free (*p);
}

void hgVSetBuffXMS( VMEM *p )
{
     int i;

     i = findxmshandle( p->logichandle );

     hgXMS_Move( xms_table[i].size,
		 xms_table[i].xmshandle, 0,     0,
		 0,                      vgseg, vgoff);
}

void hgVSaveBuffXMS( VMEM *p )
{
    int i;

    i = findxmshandle( p->logichandle );

    hgXMS_Move( xms_table[i].size,
		0,                      vgseg, vgoff,
		xms_table[i].xmshandle, 0,     0      );
}
