//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]              97' AWS and AMOS RTU(Remote Terminal Unit)             [*]
//[*]                          rtcc io program                            [*]
//[*]                Copyright Jinyang industrial co.,ltd                 [*]
//[*]                      Program by Jinmin - Mok                        [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]	Use under header with rtc program .....                           [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define	RTC_ADD			0x0000				// RTC Base address
#define	RTC_SEC			RTC_ADD + 0			// Second address
#define	RTC_MIN			RTC_ADD + 2			// Minute address
#define	RTC_HOU			RTC_ADD + 4			// Hour address
#define	RTC_DAY			RTC_ADD + 7			// Day address
#define	RTC_MON			RTC_ADD + 8			// Month address
#define	RTC_YEA			RTC_ADD + 9			// Year address
#define	RTC_ADD_A		RTC_ADD + 10		// ADD_A address
#define	RTC_ADD_B		RTC_ADD + 11		// ADD_B address
#define	RTC_ADD_D		RTC_ADD + 13		// ADD_D address
#define	RTC_SQW			0x0D				// 0x0E = 4.194304MHz / 8 Hz
#define	RTC_OUT			0x1E				// 0x1E = Binary Output
//[*]---------------------------------------------------------------------[*]
typedef struct	rtc__t	{
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
	unsigned char	delay;
}	rtc_t;

static idata rtc_t	rtc;

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rtc_init(void);
void rtc_date_write(idata rtc_t *date);
void rtc_time_write(idata rtc_t *time);
//void rtc_read(idata rtc_t *time);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rtc_init(void)
{
	idata byte yy, mm, dd, hh, bb, ss, rtc_flag = 0;

	RTC_CS();
	 output(RTC_ADD_A,RTC_SQW);
	 output(RTC_ADD_B,RTC_OUT);
	 if ((input(RTC_ADD_D) & 0x80) == 0)	rtc_flag = 1;
	 yy = input(RTC_YEA);
	 mm = input(RTC_MON);
	 dd = input(RTC_DAY);
	 hh = input(RTC_HOU);
	 bb = input(RTC_MIN);
	 ss = input(RTC_SEC);
	 if((yy > 99)||(mm > 12)||(mm < 1)||(dd > 31)||(dd < 1)) rtc_flag = 1;
	 if((hh > 24)||(bb > 59)||(ss > 59))				  	 rtc_flag = 1;
	 if(rtc_flag)	{
		output(RTC_YEA,97);	output(RTC_MON, 3);	output(RTC_DAY,23);
		output(RTC_HOU, 0);	output(RTC_MIN, 4);	output(RTC_SEC, 4);
	 }
	iRAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rtc_date_write(idata rtc_t *date)
{
	if((date->year > 99) || (date->month > 12) || (date->month < 1)
						 || (date->day   > 31) || (date->day   < 1))	{
		RAM_CS();	return;
	}
	RTC_CS();
	 while(input(RTC_ADD_A) & 0x80);
	 output(RTC_YEA,date->year );
	 output(RTC_MON,date->month);
	 output(RTC_DAY,date->day  );
	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void rtc_time_write(idata rtc_t *time)
{
	if((time->hour > 23) || (time->min > 59) || (time->sec > 59))	{
		RAM_CS();	return;
	}
	RTC_CS();
	 while(input(RTC_ADD_A) & 0x80);
	 output(RTC_HOU,time->hour );
	 output(RTC_MIN,time->min  );
	 output(RTC_SEC,time->sec  );
	RAM_CS();
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
/*
void rtc_read(idata rtc_t *time)
{
	idata byte yy, mm, dd, hh, bb, ss, rtc_flag = 1;
	RTC_CS();
	 while(input(RTC_ADD_A) & 0x80);
	 yy = input(RTC_YEA);
	 mm = input(RTC_MON);
	 dd = input(RTC_DAY);
	 hh = input(RTC_HOU);
	 bb = input(RTC_MIN);
	 ss = input(RTC_SEC);
	 if((yy > 99)||(mm > 12)||(mm < 1)||(dd > 31)||(dd < 1))	rtc_flag = 0;
	 if((hh > 24)||(bb > 59)||(ss > 59))						rtc_flag = 0;
	 if(rtc_flag)	{
		rtc.year  = yy;
		rtc.month = mm;
		rtc.day   = dd;
		rtc.hour  = hh;
		rtc.min   = bb;
		rtc.sec   = ss;
	}
	RAM_CS();
}
*/
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
