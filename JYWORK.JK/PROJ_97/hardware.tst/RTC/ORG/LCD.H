/* LCD Function List . LCD Module Used Link this file (ex) #inlcude <LCD_FUNC.H> */
#define LEFT    0x18
#define RIGHT   0x1C

extern void init_lcd(void);
extern void func_set(void);      /* First Config data write to LCD  */
extern void Display_OnOff(unsigned char DCB);
extern void L_rethome(void);
extern void Entry_mode(unsigned char IDS);
extern void L_clr(void);      /* LCD Clear Function */
extern void L_printf(unsigned char x, unsigned char y, unsigned char *str, unsigned char Dly);
extern void lcd_disp(unsigned char x, unsigned char y);
extern void write_char(unsigned char s);   /* One charicter write to LCD */
extern void L_shift(unsigned char move,unsigned char rot,unsigned char dly);
extern void LEFT_clr(unsigned char line);
extern void RIGHT_clr(unsigned char line1);
extern void delay(unsigned int count);
extern void long_delay(unsigned char i);
extern void L_clrLine(unsigned char Line);

