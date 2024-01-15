#include <io51.h>
#include <stdio.h>

#define LCD_CON     0x8000
#define LCD_DATA    0x8001

#define LEFT        0x18
#define RIGHT       0x1C

#define output(addr,data)      ((*(char *)(0x010000+addr)) = data)
#define input(addr)            (*(char *)(0x010000+addr))

void delay(data unsigned int count) ;
void write_char(data unsigned char s) ;
void func_set(void) ;
void init_lcd(void) ;
void lcd_disp(data unsigned char x, data unsigned char y) ;
void L_printf(data unsigned char x, data unsigned char y, data unsigned char *str) ;
void L_slowprn(data unsigned char x, data unsigned char y, data unsigned char *str, data unsigned char v) ;
void long_delay(unsigned char i) ;
void L_display(unsigned char view) ;
void L_clr(void) ;     /* LCD Clear Function */
void L_shift(unsigned char move,unsigned char rot,unsigned char dly) ;
unsigned char get_key(void) ;
void LEFT_clr(unsigned char line) ;
void RIGHT_clr(unsigned char line1) ;
void init_serial(void);
void main(void) ;


void init_serial()
{
	EA = 0;
	TMOD = 0x20;
	PCON = 0;
	TH1 = 0xfd;
	TR1 = 1;
	SCON = 0x52;
}

void delay(data unsigned int count)
{
    /* delay control */
    
    data unsigned int comp;
    for(comp=0;comp<count;comp++)   ;
}

void write_char(data unsigned char s)   /* One charicter write to LCD */
{
    output(LCD_DATA,s);
    delay(500);
}

void func_set(void)      /* First Config data write to LCD  */
{
    /* 8 Bit interface, 2 Line display, 5*7 font config */
    /* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
    /* 0   0    0   0   1   DL  N   F   *   *           */
    /* DL = 8/4 bit, N = 2/1 Line, F = 5*10 / 5*7 dot   */
    
    output(LCD_CON,0x38);
    delay(300);
}

void init_lcd(void)
{
    /* LCD Enable ---> 0 */
    
    delay(500);
    func_set();      /* First Config DATA */
    
    delay(400);
    func_set();
    
    delay(300);
    func_set();
    
    /* Display ON/OFF Control DATA Write to LCD         */
    /* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
    /* 0   0    0   0   0   0   1   D   C   B           */
    /* D = All Display ON/OFF, C = Cursor ON/OFF, B = Cursor Position BLINK ON/OFF */

    delay(300);
    output(LCD_CON,0x0c);

    /* Cursor & DD RAM Address return HOME(Address 0,0) */
    /* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
    /* 0   0    0   0   0   0   0   0   1   *           */

    delay(300);
    output(LCD_CON,0x02);

    /* Cursor Entry MODE Write data to LCD              */
    /* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
    /* 0   0    0   0   0   0   0   1   I/D S           */
    /* I/D = Increment/Decrement, S = Display Shift/Cursor Shift */

    delay(300);
    output(LCD_CON,0x06);

    /* CLEAR Display data                               */
    /* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
    /* 0   0    0   0   0   0   0   0   0   1           */

    delay(300);
    output(LCD_CON,0x01);
}

/* LCD Charicter Write Position Config */
void lcd_disp(data unsigned char x, data unsigned char y)
{
    /* LCD display NUMBER discription (DDRAM)
    | 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f .....|
    | 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f .....|
    */ 
    
    /* SET LCD Display data RAM address                 */
    /* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
    /* 0   0    1   DDRAM Address data(0 - 7f)          */

    if(y == 1)  output(LCD_CON,0xC0+x);   /* 0x80 | 0x40 => Second Line Position */
    else output(LCD_CON,0x80+x);          /* 0x80 | 0x00 => Firsy Line Position  */
    delay(300);
    
}

/* Charicter String Processing Roution */
void L_printf(data unsigned char x, data unsigned char y, data unsigned char *str)
{
    lcd_disp(x,y);     
    
    while(*str != 0x00)        write_char(*str++);
        
}

/* Slow moving write */
void L_slowprn(data unsigned char x, data unsigned char y, data unsigned char *str, data unsigned char v)
{
    lcd_disp(x,y);     
    
    while(*str != 0x00)
    {
        write_char(*str++);
        long_delay(v);
    }
}

/* Long Time Delay */
void long_delay(unsigned char i)
{
    unsigned char j;
    unsigned int comp;

    for(j=0;j<i;j++)
    {
        for(comp=0;comp<5000;comp++)    ;
    }
}

/* Display Test point */
void L_display(unsigned char view)
{
    unsigned char KEY_BUFFER[25];
    sprintf(KEY_BUFFER,"P1.%c PushButton Enable!.",view);
    L_printf(0,1,KEY_BUFFER);
}

void L_clr(void)      /* LCD Clear Function */
{
    output(LCD_CON,0x1);
    delay(100);
}

/* Function Discription
   move is LEFT or RIGHT, rot is rotation value, dly is delay time

void L_shift(unsigned char move,unsigned char rot,unsigned char dly)
{
    unsigned char count;

    for(count=0;count<rot;count++)
    {
        output(LCD_CON,move);
        long_delay(dly);
    }
}

Not Used : DD RAM Space very small.
*/

unsigned char get_key(void)
{
    while(P1 == 0xff) ;
    if(P1 == 0x7f)  return '7';
    else if(P1 == 0xbf) return '6';
    else if(P1 == 0xdf) return '5';
    else if(P1 == 0xef) return '4';
    else if(P1 == 0xf7) return '3';
    else if(P1 == 0xfb) return '2';
    else if(P1 == 0xfd) return '1';
    else if(P1 == 0xfe) return '0';
}

void LEFT_clr(unsigned char line)
{
    unsigned char i;
    for(i=25;i>0;--i)
    {
        L_printf(i,line," ");
        long_delay(1);
    }
    L_printf(0,line," ");
}

void RIGHT_clr(unsigned char line1)
{
    unsigned char ii;
    for(ii=0;ii<=24;ii++)
    {
        L_printf(ii,line1," ");
        long_delay(1);
    }
}

void mem_test(void)
{
    unsigned int F_count=0x100f, L_count;
    unsigned char BUF[25],value;

    L_printf(0,0,"Memory 32K byte testing!");
    for(L_count=0x1000;L_count<=F_count;L_count++)
    {
        value = input(L_count);
        sprintf(BUF,"ADDR : %x DATA : %x",L_count,value);
        L_printf(0,1,BUF);
        long_delay(3);
    }
    L_printf(0,0,"TESTING COMPATIBLE!!!   ");
    long_delay(10);
}

void mem_test1(void)
{
    unsigned int F_count=0x100f, L_count;
    unsigned char BUF[25],value=0x55;

    L_printf(0,0,"Memory write testing!");
    for(L_count=0x1000;L_count<=F_count;L_count++)
    {
        output(L_count,value);
        /* sprintf(BUF,"ADDR : %x DATA : %x",L_count,value++); */
        sprintf(BUF,"ADDR : %x DATA : %x",L_count,value);
        L_printf(0,1,BUF);
        long_delay(3);
    }
    L_printf(0,0,"TESTING COMPATIBLE!!!   ");
    long_delay(10);
}

void CENT_clr(void)
{
    unsigned char i=11,j=12,comp;
    for(comp=0;comp<12;comp++)
    {
        L_printf(i,0," ");
        L_printf(i,1," ");
        L_printf(j,0," ");
        L_printf(j,1," ");
        long_delay(2);
        i--;
        j++;
    }
    L_printf(0,0," ");
    L_printf(0,1," ");
}

void main(void)  /* Program Start Function */
{
    unsigned char key=0;
    /* LCD init Routin */
    init_lcd();

    init_serial();

    L_slowprn(0,0," DAELIM UNIVERSTY  2-4  ",1);
    L_slowprn(0,1,"   ELECTRONICS PART     ",1);

    long_delay(20);

    CENT_clr();
     /* RIGHT_clr(0); */
    L_slowprn(0,0,"Who is Made this Kit?  ",1);

    RIGHT_clr(1);
    L_slowprn(0,1,"NUMBER 2 KIM MI JUNG.  ",1);
    long_delay(5);

    LEFT_clr(1);
    L_slowprn(0,1,"NUMBER 11 SON JUM DUCK.",1);
    long_delay(5);

    RIGHT_clr(1);
    L_slowprn(0,1,"NUMBER 17 LEE BYONG WON.",1);
    long_delay(5);

    LEFT_clr(1);
    L_slowprn(0,1,"NUMBER 25 PARK JONG KIL.",1);
    long_delay(5);

    CENT_clr();
    /* RIGHT_clr(0);
    LEFT_clr(1); */

    L_slowprn(0,0,"Program written by PARK.",1);
    L_slowprn(0,1,"DATE : 96. 9. 9 AM 5:00",1);
    long_delay(10);

    L_clr();

    /* mem_test1();
    long_delay(10);
    mem_test();  */

    long_delay(10);

    while(1)
    {
    CENT_clr();

    L_printf(0,0,"KEY BOARD TESTING!!!!!!");
    L_printf(0,1,"Input Key Borard(1-8)??");

    key = get_key();
    switch(key)
    {
        case '0' :
            CENT_clr();
            L_printf(0,0,"MOVING 1 Factory!!!!!...");
            L_printf(0,1,"Mouse Control test 1....");
            long_delay(30);

            break;
        case '1' :
            CENT_clr();
            L_printf(0,0,"MOVING 2 Factory!!!!!...");
            L_printf(0,1,"Mouse Control test 2....");
            long_delay(30);

            break;
        case '2' :
            CENT_clr();
            L_printf(0,0,"MOVING 3 Factory!!!!!...");
            L_printf(0,1,"Mouse Control test 3....");
            long_delay(30);

            break;

        case '3' :
            CENT_clr();
            L_printf(0,0,"MOVING 4 Factory!!!!!...");
            L_printf(0,1,"Mouse Control test 4....");
            long_delay(30);

            break;

        case '4' :
            CENT_clr();
            L_printf(0,0,"MOVING 5 Factory!!!!!...");
            L_printf(0,1,"Mouse Control test 5....");
            long_delay(30);

            break;

        case '5' :
            CENT_clr();
            L_printf(0,0,"SELF/TEST Function!!!...");
            L_printf(0,1,"Mouse Moving & ETC!!....");
            long_delay(30);

            break;

        case '6' :
            CENT_clr();
            L_printf(0,0,"Input RESET Button!!!...");
            L_printf(0,1,"Program RESTART Program!");
            long_delay(30);

            break;

        default:
            CENT_clr();
            L_printf(0,0,"Mouse RETURN HOME !!!!!!");
            L_printf(0,1,"TESTING Prgram! P1.7 KEY");
            long_delay(30);

            break;
    }
    /* while(1)
    {
        key = get_key();
        L_display(key);     /* P1 Port Testing */
        RIGHT_clr(1);

        L_slowprn(0,0,"Slow Moving Write test.",2);

        key = get_key();
        L_display(key);     /* P1 Port Testing */
        LEFT_clr(1);

        L_clr();
    }  */
    }
}

/* Interrupt Function test Recive */

unsigned char REC=0;
unsigned char SND=0;

interrupt [0x23] void SCON_int (void)     /* Serial Port */
{
    EA = 0;

    if(TI == 1)
    {
        TI = 0;
        SBUF = SND;
    }
    else   /* RI == 1 */
    {
        RI = 0;
        REC = SBUF;
    }

    EA = 1;
}
