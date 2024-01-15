#include <io51.h>
#include <rtc_def.h>

static  unsigned char Config ;

void init_rtc(void)
{
    Config = REG_B_SET;
    
    
    switch(XTAL)
    {
        case    1:
        case    2:
        case    3:
            output(REG_A, REG_A_SET | 0x80);
            output(REG_B, Config);
            break;

        default  :
            output(REG_A, 0xff);
            break;
    }

    output(Year,97);
    output(Month,1);
    output(Day_Month,18);
    output(Day_Week,7);   // 1 = Sunday 2 = Monday...
    output(Hour,16);
    output(Min,05);
    output(Sec,55);

    output(Hour_Alram,2);
    output(Min_Alram,2);
    output(Sec_Alram,2);
    
    output(REG_A, REG_A_SET & 0x7f);
    output(REG_B, Config);

}

void Alram_Set(unsigned char H_Alram, unsigned char M_Alram, unsigned char S_Alram)
{
    output(REG_B, Config | 0x80);

    output(Hour_Alram,H_Alram);
    output(Min_Alram,M_Alram);
    output(Sec_Alram,S_Alram);

    output(REG_B, Config & 0x7f);
}

void Mode_Set(unsigned char Mode)
{
}

void RTC_Set(unsigned char YY, unsigned char MM, unsigned char DM, unsigned char DW)
{
    output(REG_B, Config | 0x80);

    output(Year,      YY);
    output(Month,     MM);
    output(Day_Month, DM);
    output(Day_Week,  DW);

    output(REG_B, Config & 0x7f);
}

void Time_Set(unsigned char H_Time, unsigned char M_Time, unsigned char S_Time)
{
    output(REG_B, Config | 0x80);

    output(Hour, H_Time);
    output(Min,  M_Time);
    output(Sec,  S_Time);

    output(REG_B, Config & 0x7f);
}

unsigned char Ret_data(unsigned char view)
{
    unsigned char ret_value=0;

    switch(view)
    {
        case    RTC_YY:
                ret_value = input(Year);
                break;

        case    RTC_MM:
                ret_value = input(Month);
                break;

        case    RTC_DM:
                ret_value = input(Day_Month);
                break;

        case    RTC_DW:
                ret_value = input(Day_Week);
                break;

        case    RTC_H:
                ret_value = input(Hour);
                break;

        case    RTC_M:
                ret_value = input(Min);
                break;

        case    RTC_S:
                ret_value = input(Sec);
                break;

        default:
                ret_value = 0;;
                break;
    }
    return  ret_value;
}

