/*  ------------------------------------------------------------------ 
*/
//
// TITLE      : General Mouse Graphic Routine
//
// SUBTITLE   :
// FILENAME   : mousegra.c
// Programmer : KIT e900519 Han Woo-jin (hitel ID:hwjketel)
//
/* ------------------------------------------------------------------ */

#include <mem.h>
#include <dos.h>
#include <alloc.h>

#include "hghlib.h"

void v256get_image(int x1,int y1,int x2,int y2,char *buff)
{
        int xsize,ysize;
        int xgraph=hgGetx2r()+1;

        asm     push ds

        asm     mov  ax,x2
        asm     sub  ax,x1
        asm     inc  ax                  // xsize=x2-x1+1
        asm     mov  xsize,ax

        asm     mov  ax,y2
        asm     sub  ax,y1              // ysize=y2-y1+1
        asm     inc  ax
        asm     mov  ysize,ax

        asm     mov  ax,y1
        asm     mov  bx,xgraph
        asm     mul  bx
        asm     add  ax,x1
        asm     adc  dl,0
        asm     mov  bx,ax              // bx is offset
        asm     call far ptr setpage

        asm     les  di,buff
        asm     mov  ax,0a000h          // video segmtment
        asm     mov  ds,ax

        asm     mov  cx,ysize

Y_loop_get: 

        asm     push cx
        asm     mov  si,bx
        asm     cld
        asm     add  si,xsize
        asm     jc   Divide_x_get

        asm     sub  si,xsize
        asm     mov  cx,xsize
        asm     REP  movsb
        asm     jmp  Divide_x_exit_get

Divide_x_get:

        asm     push si
        asm     sub  si,xsize
        asm     mov  cx,si
        asm     neg  cx
        asm     REP  movsb
        asm     inc  dl
        asm     call far ptr setpage
        asm     pop  si
        asm     mov  cx,si
        asm     xor  si,si
        asm     REP  movsb

Divide_x_exit_get:

        asm     add  bx,xgraph
        asm     cmp  bx,si
        asm     jae  No_setpage_get
        asm     inc  dl
        asm     call far ptr setpage

No_setpage_get:

        asm     pop  cx
        asm     loop Y_loop_get

        asm     pop ds
}

void v256put_image(int x1,int y1,int x2,int y2,char *buff)
{
        int xsize,ysize;
        int xgraph=hgGetx2r()+1;

        asm     push ds

        asm     mov  ax,x2
        asm     sub  ax,x1              // xsize=x2-x1+1
        asm     inc  ax
        asm     mov  xsize,ax

        asm     mov  ax,y2
        asm     sub  ax,y1              // ysize=y2-y1+1
        asm     inc  ax
        asm     mov  ysize,ax

        asm     mov  ax,y1
        asm     mov  bx,xgraph
        asm     mul  bx
        asm     add  ax,x1
        asm     adc  dl,0
        asm     mov  bx,ax              // bx is offset
        asm     call far ptr setpage

        asm     lds  si,buff
        asm     mov  ax,0a000h          // video segmtment
        asm     mov  es,ax
        asm     mov  cx,ysize

Y_loop_put:

        asm     push cx
        asm     mov  di,bx
        asm     cld
        asm     add  di,xsize
        asm     jc   Divide_x_put

        asm     mov  cx,xsize
        asm     sub  di,xsize
        asm     REP  movsb
        asm     jmp  Divide_x_exit_put

Divide_x_put:

        asm     push di
        asm     sub  di,xsize
        asm     mov  cx,di
        asm     neg  cx
        asm     REP  movsb
        asm     inc  dl
        asm     call far ptr setpage
        asm     pop  di
        asm     mov  cx,di
        asm     xor  di,di
        asm     REP  movsb

Divide_x_exit_put:

        asm     add  bx,xgraph
        asm     cmp  bx,di
        asm     jae  No_setpage_put
        asm     inc  dl
        asm     call far ptr setpage

No_setpage_put:

        asm     pop  cx
        asm     loop Y_loop_put

        asm     pop ds
}
