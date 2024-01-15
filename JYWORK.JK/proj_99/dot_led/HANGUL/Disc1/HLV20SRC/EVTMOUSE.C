/* ------------------------------------------------------------------ */
//
// TITLE      : General Mouse Routine
//
// SUBTITLE   :
// FILENAME   : evtmouse.c
// DATE       : 1992/9/5 AM 12:45
// Programmer : KIT e900519 Han Woo-jin (hitel ID:hwjketel)
//
/* ------------------------------------------------------------------ */

#pragma inline                          // for DGROUP directive

#include <mem.h>
#include <dos.h>
#include <alloc.h>

#include "mouse.h"
#include "hginit.h"

/* Static Variable */

static int mouse_x,mouse_y;                             // Mouse Position
static int mouse_color1,mouse_color2;                   // Mouse Color
static int left_button,right_button,middle_button;      // Mouse Button Flag
static char cursor_save_area[256];                      // Cursor Save Area
static char cursor_shape1[32],cursor_shape2[32];        // Cusror Shape
static int mouse_spotx,mouse_spoty;                     // Cursor Spot
static int is_show;                                     // Show ?
static int limit_x1,limit_y1,limit_x2,limit_y2;         // limit box
static int mouse_x_grid,mouse_y_grid;                   // mouse x/y grid
static int maxx,maxy;                                   // for speed
static int save_mode;
static int show_save;

/* global mouse install flag */

int    mouse_install_flag=hgFAIL;

/* Old Mouse Routine Save Area */

MOUSE_FUNC *old_save_area;

/* Function Prototype */

void far mouse_handler();

/* Standard Cursor Shape */

char arrow_cursor1[]={
	  0,  0, 24,  0, 20,  0, 18,  0, 17,  0, 16,128, 16, 64, 16, 32,
	 16, 16, 16,  8, 30, 56,  3, 32,  1,160,  0,144,  0,240,  0,  0
};
char arrow_cursor2[]={
	  0,  0,  0,  0,  8,  0, 12,  0, 14,  0, 15,  0, 15,128, 15,192,
	 15,224, 15,240,  1,128,  0,192,  0, 64,  0, 96,  0,  0,  0,  0
};

/* extern functions */

void hputchh(int x,int y,char *buff,int color);
void vputchh(int x,int y,char *buff,int color);
void vputch256h(int x,int y,char *buff,int color);
void hgHGetImage(int x1,int y1,int x2,int y2,char *buff);
void hgHPutImage(int x1,int y1,int x2,int y2,char *buff);
void hgVGetImage(int x1,int y1,int x2,int y2,char *buff);
void hgVPutImage(int x1,int y1,int x2,int y2,char *buff);

void v256get_image(int x1,int y1,int x2,int y2,char *buff);
void v256put_image(int x1,int y1,int x2,int y2,char *buff);

/* Function Pointer */

void (*putch_16)(int x,int y,char *buff,int color);
void (*get_image)(int x1,int y1,int x2,int y2,char *buff);
void (*put_image)(int x1,int y1,int x2,int y2,char *buff);

/* Function Detail */

MOUSE_FUNC *swap_mouse_rtn(word mask,word user_segment,word user_offset)
{
	MOUSE_FUNC *p;
	word prvs_mask,prvs_seg,prvs_off;

	asm {
		mov cx,mask
		mov ax,user_segment
		mov es,ax
		mov dx,user_offset
                mov ax,014h
		int 33h
		mov prvs_mask,cx
		mov ax,es
		mov prvs_seg,ax
		mov prvs_off,dx
	};

        p=(MOUSE_FUNC *)malloc(sizeof(MOUSE_FUNC));
	p->mask=prvs_mask;
	p->segment=prvs_seg;
	p->offset=prvs_off;

	return p;
}

void set_mouse_rtn(MOUSE_FUNC *p)
{
	word mask,mseg,moff;

	mask=p->mask;
	mseg=p->segment;
	moff=p->offset;

	asm {
		mov cx,mask
		mov ax,mseg
		mov es,ax
		mov dx,moff
                mov ax,0ch
		int 33h
	};
}

int init_mouse(int vmode)
{
	int  is_installed;
        int  x2,y2;
	word offset,segment;
	MOUSE_FUNC mfunc;

        maxx=hgGetx2r()+1;
        maxy=hgGety2r()+1;
        x2=maxx;y2=maxy;

        save_mode=peekb(0x40,0x49);
        pokeb(0x40,0x49,6);

	asm {
		mov ax,00h
		int 33h
		mov is_installed,ax
	}

	if (is_installed==0) return MOUSE_NOT_INSTALLED;
        mouse_install_flag=hgSUCCESS;   /* mouse driver exist */

        asm {
                mov cx,0
                mov dx,x2
                mov ax,7
                int 33h
                mov cx,0
                mov dx,y2
                mov ax,8
                int 33h
        }

        // mouse initializing

        switch(vmode) {
                case HERC_MOUSE:
                        putch_16=hputchh;
                        get_image=hgHGetImage;
                        put_image=hgHPutImage;
                        break;
                case VGA16_MOUSE:
                        putch_16=vputchh;
                        get_image=hgVGetImage;
                        put_image=hgVPutImage;
                        break;
                case VGA256_MOUSE:
                        putch_16=vputch256h;
                        get_image=v256get_image;
                        put_image=v256put_image;
                        break;
                default:
                        return MOUSE_NOT_INSTALLED;
        }

	// variable initialize

        mouse_x=(hgGetx2r()+1)/2-mouse_spotx;
        mouse_y=(hgGety2r()+1)/2-mouse_spoty;
        limit_x1=limit_y1=0;
        limit_x2=(hgGetx2r()+1)-1;limit_y2=(hgGety2r()+1)-1;

	is_show=OFF;
	left_button=right_button=middle_button=RELEASED;
        mouse_color1=BLACK;mouse_color2=WHITE;

        set_mouse_cursor(ARROW_CURSOR);
        set_mouse_position(mouse_x-mouse_spotx,mouse_y-mouse_spoty);
        hide_mouse();

	offset =FP_OFF(mouse_handler);
	segment=FP_SEG(mouse_handler);

	old_save_area=swap_mouse_rtn(0x1f,segment,offset);

	return MOUSE_INSTALLED;
}

void end_mouse()
{
        pokeb(0x40,0x49,save_mode);

        if (!mouse_install_flag) return; /* if mouse driver not exist,
                                            just return */
        set_mouse_rtn(old_save_area);
        free(old_save_area);
}

void show_mouse()
{
	if (is_show) return;

        asm cli

        (*get_image)(mouse_x,mouse_y,mouse_x+15,mouse_y+15,cursor_save_area);
        (*putch_16)(mouse_x,mouse_y,cursor_shape1,mouse_color1);
        (*putch_16)(mouse_x,mouse_y,cursor_shape2,mouse_color2);
	is_show=ON;

        asm sti
}

void hide_mouse()
{
	if (!is_show) return;

        asm cli

        (*put_image)(mouse_x,mouse_y,mouse_x+15,mouse_y+15,cursor_save_area);
	is_show=OFF;

        asm sti
}

bool is_left_button()
{
        return left_button;
}

bool is_right_button()
{
        return right_button;
}

bool is_middle_button()
{
        return middle_button;
}

int get_mouse_x()
{
	return mouse_x+mouse_spotx;
}

int get_mouse_y()
{
        return mouse_y+mouse_spoty;
}
void set_mouse_position(int x,int y)
{
        save_mouse();
        mouse_x=x-mouse_spotx;mouse_y=y-mouse_spoty;

        asm {
                mov ax,04h
                mov cx,mouse_x;
                mov dx,mouse_y;
                int 33h
        };
        restore_mouse();
}

void set_mouse_color(int color1,int color2)
{
        hide_mouse();
        mouse_color1=color1;
        mouse_color2=color2;
        show_mouse();
}

int get_mouse_color1()
{
        return mouse_color1;
}

int get_mouse_color2()
{
        return mouse_color2;
}

void set_mouse_cursor(int spotx,int spoty,char btmap1[32],char btmap2[32])
{
	mouse_spotx=spotx;
	mouse_spoty=spoty;
        memcpy(cursor_shape1,btmap1,32);
        memcpy(cursor_shape2,btmap2,32);
}

void set_limit_area(int x1,int y1,int x2,int y2)
{
        limit_x1=x1;limit_y1=y1;
        limit_x2=x2;limit_y2=y2;
}

int get_limit_x1()
{
        return limit_x1;
}
int get_limit_y1()
{
        return limit_y1;
}
int get_limit_x2()
{
        return limit_x2;
}
int get_limit_y2()
{
        return limit_y2;
}
void save_mouse()
{
        show_save=is_show;
        hide_mouse();
}
void restore_mouse()
{
        if (show_save) show_mouse();
        else hide_mouse();
}

/* mouse handler routine */

void far mouse_handler()
{
	int  new_x,new_y;
	int  diff_x,diff_y;
	int  x1,y1,x2,y2;
	word btn_status;

	asm {
		cli
		push ds
		mov ax,DGROUP
		mov ds,ax
		mov new_x,cx
		mov new_y,dx
		mov btn_status,bx
	};

        diff_x=new_x-mouse_x;
        diff_y=new_y-mouse_y;

	if ((btn_status & 1) == 1) left_button=PRESSED;
	else left_button=RELEASED;

	if ((btn_status & 2) == 2) right_button=PRESSED;
	else right_button=RELEASED;

	if ((btn_status & 3) == 3) middle_button=PRESSED;
	else middle_button=RELEASED;

	if ((diff_x!=0 || diff_y!=0) && is_show==ON) {

                (*put_image)(mouse_x,mouse_y,mouse_x+15,mouse_y+15,cursor_save_area);

                mouse_x+=diff_x;
                mouse_y+=diff_y;

                if (mouse_x<limit_x1) mouse_x=limit_x1;
                if (mouse_x+15>limit_x2) mouse_x=limit_x2-15;
                if (mouse_y<limit_y1) mouse_y=limit_y1;
                if (mouse_y+15>limit_y2) mouse_y=limit_y2-15;

                (*get_image)(mouse_x,mouse_y,mouse_x+15,mouse_y+15,cursor_save_area);
                (*putch_16)(mouse_x,mouse_y,cursor_shape1,mouse_color1);
                (*putch_16)(mouse_x,mouse_y,cursor_shape2,mouse_color2);
	}

	asm {
		pop ds
		sti
	};
}
