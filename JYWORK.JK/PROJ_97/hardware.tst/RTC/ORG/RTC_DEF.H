#define     RTC_CS          0x8000
#define     Sec             RTC_CS
#define     Sec_Alram       RTC_CS+1
#define     Min             RTC_CS+2
#define     Min_Alram       RTC_CS+3
#define     Hour            RTC_CS+4
#define     Hour_Alram      RTC_CS+5
#define     Day_Week        RTC_CS+6
#define     Day_Month       RTC_CS+7
#define     Month           RTC_CS+8
#define     Year            RTC_CS+9
#define     REG_A           RTC_CS+10
#define     REG_B           RTC_CS+11
#define     REG_C           RTC_CS+12
#define     REG_D           RTC_CS+13

#define RTC_YY  1
#define RTC_MM  2
#define RTC_DM  3
#define RTC_DW  4
#define RTC_H   5
#define RTC_M   6
#define RTC_S   7

/* Crystal Setup : 4.194304Mhz = 1, 1.048576Mhz = 2, 32.768Khz = 3 */

#define     XTAL            1

//  Bit7  Bit6  Bit5  Bit4  Bit3  Bit2  Bit1  Bit0
//  UIP   DV2   DV1   DV0   RS3   RS2   RS1   RS0
//  RTC A Register Setup(REG_A_SET)
//  Default Configuration

#define     REG_A_SET       0x0f

//  Bit7  Bit6  Bit5  Bit4  Bit3  Bit2  Bit1  Bit0
//  SET   PIE   AIE   UIE   SQWE  DM    24/12 DSE
//  RTC B Register Setup(REG_B_SET)
//  Default configuration = Update interrupt ,Binary , 24Hour
//  DM = 1 Binary, 0 = BCD Count

#define     REG_B_SET       0x1e

void Alram_Set(unsigned char H_Alram, unsigned char M_Alram, unsigned char S_Alram);
void Mode_Set(unsigned char Mode);
void RTC_Set(unsigned char YY, unsigned char MM, unsigned char DM, unsigned char DW);
void Time_Set(unsigned char H_Time, unsigned char M_Time, unsigned char S_Time);
unsigned char Ret_data(unsigned char view);
void init_rtc(void);
