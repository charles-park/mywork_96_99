extern void delay(data unsigned int count) ;
extern void write_char(data unsigned char s) ;
extern void func_set(void) ;
extern void init_lcd(void) ;
extern void lcd_disp(data unsigned char x, data unsigned char y) ;
extern void L_printf(data unsigned char x, data unsigned char y, data unsigned char *str) ;
extern void L_slowprn(data unsigned char x, data unsigned char y, data unsigned char *str, data unsigned char v) ;
extern void long_delay(unsigned char i) ;
extern void L_clr(void) ;     /* LCD Clear Function */
extern void L_shift(unsigned char move,unsigned char rot,unsigned char dly) ;
extern void LEFT_clr(unsigned char line) ;
extern void RIGHT_clr(unsigned char line1) ;
void main(void);
