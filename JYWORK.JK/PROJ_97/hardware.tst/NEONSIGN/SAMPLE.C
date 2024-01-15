#include "io51.h"
#include "sample.h"

data    static  unsigned char Led_Data[20];
data    static  unsigned char Led_Count=0;
data    static  unsigned char Num = 0;
static  bit     Key_Flag;
data    unsigned    char    i;

void Mode1_Led(void)
{
    unsigned char i=0;
    unsigned char Led;
    unsigned char Count;

    for(Count=0;Count<5;Count++)
    {
        Led = 0x1;
        for(i=0;i<8;i++)
        {
            P1 = ~Led;
            Led = Led<<1;
            delay(2);
        }

        Led = 0x80;

        for(i=0;i<8;i++)
        {
            P1 = ~Led;
            Led = Led>>1;
            delay(2);
        }
    }
}

void Mode2_Led(void)
{
    unsigned char Value1;
    unsigned char Value2;
    unsigned char Value3=0;

    unsigned char i,j;

    for(i=0;i<5;i++)
    {
        Value1 = 0x1;
        Value2 = 0x80;

        for(j=0;j<8;j++)
        {
            Value3 = (Value1 | Value2);
            P1 = ~Value3;
            Value1 = Value1<<1;
            Value2 = Value2>>1;
            delay(2);
        }

        Value1 = 0x80;
        Value2 = 0x1;

        for(j=0;j<8;j++)
        {
            Value3 = (Value1 | Value2);
            P1 = ~Value3;
            Value1 = Value1>>1;
            Value2 = Value2<<1;
            delay(2);
        }
    }
}

void Mode3_Led(void)
{
    unsigned char Value1;
    unsigned char Value2;

    unsigned char i,k;

    for(i=0;i<5;i++)
    {
        Value1 = 0xff;
        Value2 = 0xfe;

        for(k=0;k<8;k++)
        {
            P1 = Value1;
            Value1 = Value1 & Value2;
            Value2 = Value2<<1;
            delay(2);
        }

        Value1 = 0;
        Value2 = 0x80;

        for(k=0;k<8;k++)
        {
            P1 = Value1;
            Value1 = Value1 | Value2;
            Value2 = Value2>>1;
            delay(2);
        }
    }
}

void Mode4_Led(void)
{
    unsigned char i=0;

    for(i=0 ; i<5 ; i++)
    {
        P1 = On;
        delay(3);
        P1 = ~On;
        delay(3);
    }
}

void delay(data unsigned char Count)
{
    data unsigned int Comp;
    data unsigned char i;

    for(i=0;i<Count;i++)
        for(Comp=0;Comp<5000;Comp++) ;
}

void initiliaze(void)
{
    Run_Led = Off;      /* Set & Stop Status */
    Stop_Led = On;
    Set_Led = On;
    P1 = 0xff;
    Key_Flag = Stop;

    Mode4_Led();
}

void Key_Scan(void)
{
    Run_Led = Off;
    Stop_Led = On;
    Set_Led = On;

    while(Key_Flag)
    {
        P1 = 0xff;
        if(Mode1_Key == On)     Led_Data[Led_Count++]=Mode1;
        else if(Mode2_Key == On)    Led_Data[Led_Count++]=Mode2;
        else if(Mode3_Key == On)    Led_Data[Led_Count++]=Mode3;
        else if(Mode4_Key == On)    Led_Data[Led_Count++]=Mode4;

        if(Led_Count == 20)    Key_Flag = Run;
        if(Stop_Key == On)   Led_Count=0;

        if(Run_Key == On)
        {
            Key_Flag = Run;
            Run_Led = On;
            Stop_Led =Off;
            Set_Led = Off;
        }
        else    Key_Flag = Stop;

        Mode1_Key = Mode2_Key = Mode3_Key = Mode4_Key = Stop_Key = Run_Key = Off;

        delay(3);
    }
    P1 = 0xff;
}

void main(void)
{
    initiliaze();

    while(1)
    {
        if(Key_Flag)    Key_Scan();

        else
        {
            if(Led_Count == 0)  Key_Flag = Stop;

            else
            {
                for(i=0; i != Led_Count; i++)
                {
                    Num = Led_Data[i];

                    switch(Num)
                    {
                        case    Mode1:  Mode1_Led();    break;
                        case    Mode2:  Mode2_Led();    break;
                        case    Mode3:  Mode3_Led();    break;
                        case    Mode4:  Mode4_Led();    break;
                        default :
                                Key_Flag = Stop;
                                Led_Count = 0;
                                break;
                    }
                }
                Led_Count = 0;
                P1 = 0xff;
            }
        }
    }
}
