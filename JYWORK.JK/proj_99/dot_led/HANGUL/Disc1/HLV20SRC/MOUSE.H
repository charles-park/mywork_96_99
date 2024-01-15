/* ------------------------------------------------------------------ */
//
// TITLE      : General Mouse Routine Header
//
// SUBTITLE   :
// FILENAME   : mouse.h
// DATE       : 1992/9/5 AM 12:50
// Programmer : KIT e900519 Han Woo-jin (hitel ID:hwjketel)
//
/* ------------------------------------------------------------------ */

#ifndef __MOUSE_H
#define __MOUSE_H

#ifndef __LARGE__
#error Hangul GUI v1.0 only supports Large Model
#endif

// Some Constants Define

#define         MOUSE_INSTALLED         1
#define         MOUSE_NOT_INSTALLED     0

#define         PRESSED                 1
#define         RELEASED                0

#define         ON                      1
#define         OFF                     0

// Video Mode

#define         HERC_MOUSE              0
#define         VGA16_MOUSE             1
#define         VGA256_MOUSE            2

// Mouse Cursor

#define         ARROW_CURSOR            5,3,arrow_cursor1,arrow_cursor2

// Mouse Cusor External Define

extern char arrow_cursor1[32],arrow_cursor2[32];

// Mouse Function Save Area Structure Define

typedef unsigned int word;
typedef unsigned char byte;
typedef int bool;

typedef struct mouse_func {
        word mask;
        word segment;
        word offset;
} MOUSE_FUNC;

// function prototype

int  init_mouse(int vmode);
void end_mouse(void);
void show_mouse(void);
void hide_mouse(void);
bool is_left_button(void);
bool is_right_button(void);
bool is_middle_button(void);
int  get_mouse_x(void);
int  get_mouse_y(void);
void set_mouse_position(int x,int y);
void set_mouse_color(int color1,int color2);
int  get_mouse_color1(void);
int  get_mouse_color2(void);
void set_mouse_cursor(int spotx,int spoty,char btmap1[32],char btmap2[32]);
void set_limit_area(int x1,int y1,int x2,int y2);
int  get_limit_x1(void);
int  get_limit_y1(void);
int  get_limit_x2(void);
int  get_limit_y2(void);
void save_mouse(void);
void restore_mouse(void);
int  get_mouse_x_grid(void);
int  get_mouse_y_grid(void);

#endif
