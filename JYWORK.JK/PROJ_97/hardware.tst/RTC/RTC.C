#include <io51.h>
#include <rtc_def.h>

void init_rtc(void)
{
    // Register B MSB 7 bit is Start & Stop Control
    // if MSB == 1 then Stop, other then RUN

    switch(XTAL)
    {
        case    1:      // Crystal is 4.194304 Mhz
            output(REG_B, REG_B_SET | Config | Stop);
            output(REG_A, REG_A_SET);
            break;

        case    2:      // Crystal is 1.048576 Mhz
            output(REG_B, REG_B_SET | Config | Stop);
            output(REG_A, REG_A_SET);
            break;

        case    3:      // Crystal is 32.768 Khz
            output(REG_B, REG_B_SET | Config | Stop);
            output(REG_A, REG_A_SET);
            break;

        default  :
            output(REG_A, 0xff);
            break;
    }

    output(REG_B,(REG_B_SET | Config) & Run );        //  Stop RTC(Stop = MSB 7 Bit is 1)
}

void Alram_Set(Byte H_Alram, Byte M_Alram, Byte S_Alram)
{
    output(REG_B, REG_B_SET | Config | Stop);

    output(Hour_Alram,H_Alram);
    output(Min_Alram,M_Alram);
    output(Sec_Alram,S_Alram);

    output(REG_B, (REG_B_SET | Config) & Run);
}

void Mode_Set(Byte Mode)
{
}

void RTC_Set(Byte YY, Byte MM, Byte DM, Byte DW)
{
    output(REG_B, REG_B_SET | Config | Stop);

    output(Year,      YY);
    output(Month,     MM);
    output(Day_Month, DM);
    output(Day_Week,  DW);

    output(REG_B, (REG_B_SET | Config) & Run);
}

void Time_Set(Byte H_Time, Byte M_Time, Byte S_Time)
{
    output(REG_B, REG_B_SET | Config | Stop);

    output(Hour, H_Time);
    output(Min,  M_Time);
    output(Sec,  S_Time);

    output(REG_B, (REG_B_SET | Config) & Run);
}

Byte Ret_data(Byte view)
{
    Byte ret_value=0;

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
