#include <io51.h>
#include <lcd.h>
#include <rtc.h>

#define     MAX_X   20
#define     MAX_Y   3

void main(void)
{
    unsigned char BUF[20];

    init_lcd();     // initiliaze LCD(20 Chars * 4 Line)
    init_rtc();     // RTC initiliaze (MC146818)

    L_clr();
    
    Time.Year      = Ret_data(RTC_YY);
    Time.Month     = Ret_data(RTC_MM);
    Time.Day_Month = Ret_data(RTC_DM);
    Time.Day_Week  = Ret_data(RTC_DW);
    Time.Hour      = Ret_data(RTC_H);
    Time.Min       = Ret_data(RTC_M);
    Time.Sec       = Ret_data(RTC_S);
    
    sprintf(BUF, "Year : 19%d, %d, %d ",Time.Year, Time.Month, Time.Day_Month);
    L_printf(0,0,BUF,0);
    
    sprintf(BUF, "Day of Week : %d    ",Time.Day_Week);
    L_printf(0,1,BUF,0);

    sprintf(BUF, "TIME : %d : %d : %d ",Time.Hour, Time.Min, Time.Sec);
    L_printf(0,2,BUF,0);

    L_printf(0,3,"Sample program..!   ",0);
    
    while(1)
    {
        Time.Hour  = Ret_data(RTC_H);
        Time.Min   = Ret_data(RTC_M);
        Time.Sec   = Ret_data(RTC_S);

        sprintf(BUF,"TIME : %d : %d : %d ", Time.Hour, Time.Min, Time.Sec);
        L_printf(0,2,BUF,0);

        long_delay(1);
    }
}

