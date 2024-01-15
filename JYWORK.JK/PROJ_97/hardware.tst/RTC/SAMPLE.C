#include <io51.h>
#include "sample.h"

void initialize(void)
{
    P1=0xff;

    if(read_XDATA(REGD) & 0x80 == 0x80)
    {
        write_XDATA(REGA,0x8f);
        /* write_XDATA(REGB,0x0e);       SQW 2Hz out, Binary, 24Hour Mode */
        write_XDATA(REGB,0x1a);      /* SQW 2Hz out, BCD, 24Hour Mode */
        
    }
    write_XDATA(YEAR,90);
    write_XDATA(MONTH,1);
    write_XDATA(DAY_M,1);
    write_XDATA(DAY_W,1);
    write_XDATA(HOUR,0);
    write_XDATA(MIN,1);
    write_XDATA(SEC,1);

    write_XDATA(REGA,0x0f);      /* 4.1943 Mhz set */
    write_XDATA(REGB,0x1a);      /* SQW 2Hz out, BCD, 24Hour Mode */
}

void delay(unsigned int count)
{
    unsigned int comp;
    for(comp=0;comp<count;comp++)   ;
}

void main(void)
{
    unsigned char Sec, Min, Hour, Month, Day_W, Day_M, Year;
    unsigned char value1,value2;

    initialize();

    while(1)
    {
        Sec = read_XDATA(SEC);
        Min = read_XDATA(MIN);
        Hour = read_XDATA(HOUR);
        Month = read_XDATA(MONTH);
        Day_W = read_XDATA(DAY_W);
        Day_M = read_XDATA(DAY_M);
        Year = read_XDATA(Year);

        if(REGC == 0x90)
        {
            write_XDATA(REGC,0);
            P1 = Sec;
        }
        else if(REGC == 0x10)
        {
            write_XDATA(REGC,0);
            P1 = 0xaa;
        }
        else P1 = 0x55;
    }
}

