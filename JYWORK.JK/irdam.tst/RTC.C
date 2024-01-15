/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#include	"main.h"
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
rtc_t		rtc;
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void rtc_date_write(rtc_t *d)
{
	struct dosdate_t set_d;

	if((d->year > 99) || (d->month > 12) || (d->month < 1)
					  || (d->day   > 31) || (d->day   < 1))	return;

	set_d.year  = ((d->year > 80) ? (1900 + d->year) : (2000 + d->year));
	set_d.month =   d->month;
	set_d.day   =   d->day;

	_dos_setdate(&set_d);

	set_d.year = set_d.year % 100;
	outportb(0x349,(set_d.year   / 10 * 16 + set_d.year   % 10));
	outportb(0x347,(set_d.month  / 10 * 16 + set_d.month  % 10));
	outportb(0x346,(set_d.day    / 10 * 16 + set_d.day    % 10));
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void rtc_time_write(rtc_t *t)
{
	struct dostime_t set_t;

	if((t->hour > 23) || (t->min > 59) || (t->sec > 59))	return;

	set_t.hour   = t->hour;
	set_t.minute = t->min;
	set_t.second = t->sec;

	_dos_settime(&set_t);

	outportb(0x344,(set_t.hour   / 10 * 16 + set_t.hour   % 10));
	outportb(0x343,(set_t.minute / 10 * 16 + set_t.minute % 10));
	outportb(0x342,(set_t.second / 10 * 16 + set_t.second % 10));
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void rtc_read(void)
{
	struct	time	t;
	struct	date	d;
	static	oldsec = 70;
	gettime(&t);

	if(oldsec != t.ti_sec)	{
		oldsec = t.ti_sec;
		getdate(&d);

		rtc.year  = d.da_year % 100;
		rtc.month = d.da_mon;
		rtc.day   = d.da_day;
		rtc.hour  = t.ti_hour;
		rtc.min   = t.ti_min;
		rtc.sec   = t.ti_sec;
	}
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
