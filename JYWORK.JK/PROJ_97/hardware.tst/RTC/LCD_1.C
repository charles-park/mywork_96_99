#include <io51.h>
#include <lcd_def.h>

void init_lcd(void)
{
    /* LCD Enable ---> 0 */

    delay(300);
    func_set();      /* First Config DATA */

    delay(200);
    func_set();

    delay(100);
    func_set();

    Display_OnOff(Dis_On_Off);    /* Display On/Off Control */

    L_rethome();                  /* LCD Cursor Return Home */

    Entry_mode(Ent_Mode);         /* Entry Mode Setup */

    L_clr();                      /* LCD CLEAR Function */
}

void func_set(void)      /* First Config data write to LCD  */
{
    /* 8 Bit interface, 2 Line display, 5*7 font config */
    /* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
    /* 0   0    0   0   1   DL  N   F   *   *           */
    /* DL = 8/4 bit, N = 2/1 Line, F = 5*10 / 5*7 dot   */
    
    output(LCD_CON,SETUP);
    delay(100);
}

/* Display ON/OFF Control DATA Write to LCD         */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   1   D   C   B           */
/* D = All Display ON/OFF, C = Cursor ON/OFF, B = Cursor Position BLINK ON/OFF */

void Display_OnOff(unsigned char DCB)
{
    delay(100);
    output(LCD_CON,DCB & 0xf);
}

/* Cursor & DD RAM Address return HOME(Address 0,0) */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   0   0   1   *           */

void L_rethome(void)
{
    delay(100);
    output(LCD_CON,0x02);
}

/* Cursor Entry MODE Write data to LCD              */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   0   1   I/D S           */
/* I/D = Increment/Decrement, S = Display Shift/Cursor Shift */

void Entry_mode(unsigned char IDS)
{
    delay(100);
    output(LCD_CON,IDS & 0x7);
}

/* CLEAR Display data                               */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   0   0   0   1           */

void L_clr(void)      /* LCD Clear Function */
{
    output(LCD_CON,0x1);
    delay(100);
}

/* Charicter String Processing Roution & Delay */
void L_printf(unsigned char x, unsigned char y, unsigned char *str, unsigned char Dly)
{
    lcd_disp(x,y);     
    
    while(*str != 0x00)
    {
        write_char(*str++);
        if(Dly != 0)    long_delay(Dly);
    }
}

/* LCD Charicter Write Position Config */
void lcd_disp(unsigned char x, unsigned char y)
{
    /* LCD display NUMBER discription (DDRAM)  2 Line LCD
    | 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f .....|
    | 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f .....|
    */ 
    /* 4 Line LCD (if 20 Chars * 4 Line)
    | 00 01 02 ...
    | 40 41 42 ...
    | 20 21 22 ...
    | 60 61 62 ...
    */
    
    /* SET LCD Display data RAM address                 */
    /* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
    /* 0   0    1   DDRAM Address data(0 - 7f)          */

    if(y == 1)  output(LCD_CON,0xC0+x);   /* 0x80 | 0x40 => Second Line Position */
    else if(y == 2) output(LCD_CON,0x80+x+COLUM); // First Line - 1
    else if(y ==3) output(LCD_CON,0xC0+x+COLUM);  // Second Line -1
    else output(LCD_CON,0x80+x);          /* 0x80 | 0x00 => First Line Position  */

    delay(100);
}

void write_char(unsigned char s)   /* One charicter write to LCD */
{
    output(LCD_DATA,s);
    delay(100);
}

void L_shift(unsigned char move,unsigned char rot,unsigned char dly)
{
    unsigned char count;

    for(count=0;count<rot;count++)
    {
        output(LCD_CON,move);
        long_delay(dly);
    }
}
           
void LEFT_clr(unsigned char line)
{
    unsigned char i;
    for(i=COLUM;i>0;--i)
    {
        L_printf(i,line," ",0);
        long_delay(1);
    }
    L_printf(0,line," ",0);
}

void RIGHT_clr(unsigned char line1)
{
    unsigned char ii;
    for(ii=0;ii<=COLUM;ii++)
    {
        L_printf(ii,line1," ",0);
        long_delay(1);
    }
}

void delay(unsigned int count)
{
    /* delay control */
    
    unsigned int comp;
    for(comp=0;comp<count;comp++)   ;
}

/* Long Time Delay */
void long_delay(unsigned char i)
{
    unsigned char j;
    unsigned int comp;

    for(j=0;j<i;j++)
    {
        for(comp=0;comp<L_DLYTIME;comp++)    ;
    }
} 
