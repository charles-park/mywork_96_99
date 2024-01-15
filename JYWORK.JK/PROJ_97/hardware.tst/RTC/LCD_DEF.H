#define LCD_CON     0xC000    /* LCD Enable Address */
#define LCD_DATA    LCD_CON+1

/* LCD Module Colum Setup */
#define LINE      4
#define COLUM     20          /* LCD COLUM Value */

/* 8 Bit interface, 2 Line display, 5*7 font config     */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0              */
/* 0   0    0   0   1   DL  N   F   *   *               */
/* DL = 8/4 bit, N = 2/1 Line, F = 5*10 / 5*7 dot       */
/* Define Variable SETUP Configuration =>Function SET   */

#define SETUP       0x38

/* Cursor Entry MODE Write data to LCD              */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   0   1   I/D S           */
/* I/D = Increment/Decrement, S = Display Shift/Cursor Shift */

#define Ent_Mode    0x06

/* Display ON/OFF Control DATA Write to LCD         */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   1   D   C   B           */
/* D = All Display ON/OFF, C = Cursor ON/OFF, B = Cursor Position BLINK ON/OFF */

#define Dis_On_Off  0x0c            /* default setup */

#define LEFT        0x18
#define RIGHT       0x1C

#define L_DLYTIME   5000            /* Long Delay Value Configuration */

/* LCD Function define */
void init_lcd(void);
void func_set(void);      /* First Config data write to LCD  */
void Display_OnOff(unsigned char DCB);
void L_rethome(void);
void Entry_mode(unsigned char IDS);
void L_clr(void);      /* LCD Clear Function */
void L_printf(unsigned char x, unsigned char y, unsigned char *str, unsigned char Dly);
void lcd_disp(unsigned char x, unsigned char y);
void write_char(unsigned char s);   /* One charicter write to LCD */
void L_shift(unsigned char move,unsigned char rot,unsigned char dly);
void LEFT_clr(unsigned char line);
void RIGHT_clr(unsigned char line1);
void delay(unsigned int count);
void long_delay(unsigned char i);
void L_clrLine(unsigned char Line);
