// 1997. 1. 11 ~
// Program : Park Jong Kil
// Program Link file : Main.c + Lcd.c + Rtc.c + Key.c
// Sheet : Project1.sch     JED File : Project1.PDS, Project1.JED

#include <io51.h>
#include <stdio.h>
#include <lcd.h>
#include <rtc.h>

#define     MAX_X   20
#define     MAX_Y   3
#define     On      1
#define     Off     0

static Byte Update_Level ;   // default Value
static Byte KeyOn,test;
static Byte *DAYWEEK[8] = { "ErrorDay","Sunday   ","Monday   ","Tuesday  ","Wednesday","Thursday ","Friday   ","Saturday "};
static Byte Sbuf[20];
static Byte Rbuf[20];
static Byte Rcount=0,Scount=0;
static bit  Comin;
static bit  Comout;

void initiliaze(void)       // interrupt initliaze Roution
{

    IE=0x80;
    PCON=0x80;          // Baudrate double set

    TMOD = 0x25;        // T0 = counter 16Bit no,Prescaler,

    TH1 = 0xfa;
    TL1 = 0xfa;        // 23Mhz = F3, 22Mhz = F4,18Mhz = F6, 11Mhz = fa Set

    SCON=0x50;
    TR1=1;

    PS =1;      // Serial Priority
    ES =1;      // Serial Interrupt Enable
    
    // IE = 0x80;              // External interrupt 0 Enable (Edge)
    // TCON = 0x0f;
    // EX0 = On;
}

void main(void)
{
    Byte BUF[20];
    Byte i;
    Word count;

    initiliaze();   // interrupt initiliaze
    init_lcd();     // initiliaze LCD(20 Chars * 4 Line)
    init_rtc();     // RTC initiliaze (MC146818)

    Update_Level = 0xff;
    
    P1 = 0xf0;
    KeyOn = 0xf0;
    Comin = 0;
    Comout = 0;

    L_clr();

    //  Day of Week : 1 = Sunday, 2 = Monday ...

    RTC_Set(97, 1, 21, 3); // Default Year, Month, Day of Month, Day of Week Setup
    Time_Set(16,10,0);       // Default Time Setup : Hour,Min,Sec
    
    Time[0].Year      = Time[1].Year      = Ret_data(RTC_YY);
    Time[0].Month     = Time[1].Month     = Ret_data(RTC_MM);
    Time[0].Day_Month = Time[1].Day_Month = Ret_data(RTC_DM);
    Time[0].Day_Week  = Time[1].Day_Week  = Ret_data(RTC_DW);
    Time[0].Hour      = Time[1].Hour      = Ret_data(RTC_H);
    Time[0].Min       = Time[1].Min       = Ret_data(RTC_M);
    Time[0].Sec       = Time[1].Sec       = Ret_data(RTC_S);

    // Test Program -----------------------------------------------
    Sbuf[0] = 0xff;     Sbuf[1] = 0xff;     Sbuf[2] = 'R';
    Sbuf[3] = 0x0;      Sbuf[4] = 0x0;      Sbuf[5] = 0;
    Sbuf[6] = 0;        Sbuf[7] = 0;        Sbuf[8] = 0;
    Sbuf[9] = 0xff;     Sbuf[10] = 0xfe;

    for(i=2;i != 8; i++)    Sbuf[8] ^= Sbuf[i];

    i=0;

    //-------------------------------------------------------------
    
    while(1)
    {
        if(Update_Level != 0)
        {
            // for(i=0;i<=20;i++)  BUF[i] = ' ';

            switch(Update_Level)
            {
                case    RTC_YY:
                case    RTC_MM:
                case    RTC_DM:
                      //  sprintf(BUF,"Year : 19%d, %d, %d   ",Time[0].Year, Time[0].Month, Time[0].Day_Month);
                      //  L_printf(0,0,BUF,0);

                      //  sprintf(BUF,"Time : %d : %d : %d ",Time[0].Hour, Time[0].Min, Time[0].Sec);
                      //  L_printf(0,2,BUF,0);
                        break;

                case    RTC_DW:
                      //  sprintf(BUF,"Day/Week : %s",DAYWEEK[Time[0].Day_Week]);
                      //  L_printf(0,1,BUF,0);

                      //  sprintf(BUF,"Time : %d : %d : %d ",Time[0].Hour, Time[0].Min, Time[0].Sec);
                      //  L_printf(0,2,BUF,0);
                        break;

                case    RTC_S:
                      //  sprintf(BUF,"Time : %d : %d : %d ",Time[0].Hour, Time[0].Min, Time[0].Sec);
                      //  L_printf(0,2,BUF,0);
                        break;

                default   :
                      //  sprintf(BUF,"Year : %d, %d, %d   ",Time[0].Year, Time[0].Month, Time[0].Day_Month);
                      //  L_printf(0,0,BUF,0);

                      //  sprintf(BUF,"Day/Week : %s",DAYWEEK[Time[0].Day_Week]);
                      //  L_printf(0,1,BUF,0);

                      //  sprintf(BUF,"Time : %d : %d : %d ",Time[0].Hour, Time[0].Min, Time[0].Sec);
                      //  L_printf(0,2,BUF,0);

                      //  L_printf(0,3,"Good - Morning!!!   ",0);
                        break;
            }
            Update_Level = 0;   // if Loop Out Command
        }

        if(KeyOn != 0xf0 && Comout )
        {
            P1 = 0xf0;
            /* sprintf(BUF,"Year : %d, %d, %d   ",Time[0].Year, Time[0].Month, Time[0].Day_Month);
            L_printf(0,0,BUF,0);

            sprintf(BUF,"Day/Week : %s",DAYWEEK[Time[0].Day_Week]);
            L_printf(0,1,BUF,0);

            sprintf(BUF,"Time : %d : %d : %d ",Time[0].Hour, Time[0].Min, Time[0].Sec);
            L_printf(0,2,BUF,0);

            L_printf(0,3,"Time is a Gold..!!!!",0); */
            L_clr();

            Scount = 0;
            Rcount = 0;
            SBUF = Sbuf[0];
        }
        
        if(Comin)
        {
            sprintf(BUF,"%x%x%x%x%x",(char)Rbuf[0],(char)Rbuf[1],(char)Rbuf[2],(char)Rbuf[3],(char)Rbuf[4]);
            L_printf(0,0,BUF,0);

            sprintf(BUF,"%x%x%x%x%x",(char)Rbuf[5],(char)Rbuf[6],(char)Rbuf[7],(char)Rbuf[8],(char)Rbuf[9]);
            L_printf(0,1,BUF,0);

            sprintf(BUF,"%x%x%x%x%x",(char)Rbuf[10],(char)Rbuf[11],(char)Rbuf[12],(char)Rbuf[13],(char)Rbuf[14]);
            L_printf(0,2,BUF,0);

            sprintf(BUF,"%x%x",(char)Rbuf[15],(char)Rbuf[16]);
            L_printf(0,3,BUF,0);

            Comin = 0;
        }
        KeyOn = P1;
    }
}

/* interrupt [0x03] void EX0_int (void)
{
    EA = Off;
    Time[0].Hour       = Ret_data(RTC_H);
    Time[0].Min        = Ret_data(RTC_M);
    Time[0].Sec        = Ret_data(RTC_S);
    Time[0].Year       = Ret_data(RTC_YY);
    Time[0].Month      = Ret_data(RTC_MM);
    Time[0].Day_Week   = Ret_data(RTC_DW);
    Time[0].Day_Month  = Ret_data(RTC_DM);

    if     (Time[0].Year      != Time[1].Year     ) { Time[0].Year      = Time[1].Year;      Update_Level = RTC_YY;  }   // Year Update
    else if(Time[0].Month     != Time[1].Month    ) { Time[0].Month     = Time[1].Month;     Update_Level = RTC_MM;  }   // Month Update
    else if(Time[0].Day_Week  != Time[1].Day_Week ) { Time[0].Day_Week  = Time[1].Day_Week;  Update_Level = RTC_DW;  }   // Day_Week Update
    else if(Time[0].Day_Month != Time[1].Day_Month) { Time[0].Day_Month = Time[1].Day_Month; Update_Level = RTC_DM;  }   // Day_Month Update
    else                                            Update_Level = RTC_S ; // Sec Update

    test = input(REG_C);
    EA = On;
}  */

interrupt [0x23] void SCON_int (void)    /* Serial Port */
{
    Byte cks=0;
    Byte i=0;
    
    if(TI == 1)
    {
        TI = 0;
        if(Scount++ <= 11)
        {
            SBUF = Sbuf[Scount];
        }
        else       Comout = 1;
    }
    
    else if(RI == 1)
    {
        RI = 0;
        
        Rbuf[Rcount++] = SBUF;
        
        if(Rcount == 17)
        {
            cks = 0;
            for(i=2;i<14;i++)    cks ^= Rbuf[i];
            
            if(cks == Rbuf[14])     Comin = 1;      // check sum
            else                    Comin = 0;
        
            Rcount = 0;
        }
    }
}


