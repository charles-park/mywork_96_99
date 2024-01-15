#define RTC_YY  1
#define RTC_MM  2
#define RTC_DM  3
#define RTC_DW  4
#define RTC_H   5
#define RTC_M   6
#define RTC_S   7

extern void init_rtc(void);
extern void Alram_Set(unsigned char H_Alram, unsigned char M_Alram, unsigned char S_Alram);
extern void Mode_Set(unsigned char Mode);
extern void RTC_Set(unsigned char YY, unsigned char MM, unsigned char DM, unsigned char DW);
extern void Time_Set(unsigned char H_Time, unsigned char M_Time, unsigned char S_Time);
extern unsigned char Ret_data(unsigned char view);

struct RTC {
    unsigned char Year;
    unsigned char Month;
    unsigned char Day_Month;
    unsigned char Day_Week;
    unsigned char Hour;
    unsigned char Min;
    unsigned char Sec;
}   Time;

