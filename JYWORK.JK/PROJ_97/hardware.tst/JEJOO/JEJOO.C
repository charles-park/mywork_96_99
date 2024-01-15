#include <c:\cci_hdsl\include\io51.h>
#include <c:\cci_hdsl\include\math.h>
#include <jejoo.h>

#pragma memory=code
char Title[20][65]={
//          1         2         3         4         5
// 123456789012345678901234567890123456789012345678901234567890
  "/ /// /////////////////////////////////////////////// /// /",
  "/ /// /////////////////////////////////////////////// /// /",
  "/ /// ///       JIN-YANG  INDUSTRIAL.,CO LTD      /// /// /",
  "/ /// ///       Program By  Hae-suk. Jin          /// /// /",
  "/ /// ///     Start :  1997. 2. 19.               /// /// /",
  "/ /// ///       End :  1996.        Ver 1.0       /// /// /",
  "/ /// ///                                         /// /// /",    // Dew Point Value <= Temp
  "/ /// ///    1.Temperature   : 1 EA  Analog Ch=0  /// /// /",
  "/ /// ///    2.Humidity      : 1 EA  Analog Ch=1  /// /// /",
  "/ /// /////////////////////////////////////////////// /// /",
  "/ /// /////////////////////////////////////////////// /// /"
};
#pragma memory=default

#define BD9600L 0x0c
#define BD9600H 0x00
#define BPS96   0xFA // FA=11MHz, F4=22Mhz, F6=18 MHz

/**************************  Sensor Code's  ********************************/
#define TEMPCODE    2   // ID 2
#define HUMICODE    6   // ID 4

/**************************  Multiplexer  **********************************/
#define     Mux0    P1.3        // Multiplexer Select Pin 0
#define     Mux1    P1.4        // Multiplexer Select Pin 1
#define     Mux2    P1.5        // Multiplexer Select Pin 2

//      1 : Temperature
//      2 : Humidity

/***************************  ETC.  ****************************************/
#define     SRH     P1.2        // Sample/Hold
#define     RedL    P1.1        // Transm
#define     BlueL   P1.0        // Receve
#define     STS     P1.7        // AD Status
#define     LightL  0x300

/**************************  Function's   **********************************/
interrupt void T0_int       (void);         // Timer 0
interrupt void EX0_int      (void);         // Clock_check Interrupt
interrupt void EX1_int      (void);         // Precipi-Detect Interrupt
interrupt void SCON_int     (void);         // Serial Interrupt

          void Initialize   (void);
          void Time_pros    (void);
          void One_pros     (void);
          void Ten_pros     (void);
          void S0_pros      (void);         // Temperature    Analog
          void S1_pros      (void);         // Humidity       Calc

/*************************** Main ******************************************/
void main(void)
{
    Initialize();
    while(1)
    {
        if(Receve)
        {
            if(!Blueb--)
            {
                BlueL=0;     /*Receive LED off  */
                Receve=0;
            }
        }
        if(Transm)
        {
            if(!Redb--)
            {
                RedL=0;
                Transm=0;
            }
        }

        if(Adcerr)
        {
            ES=0;
            Tempsta |= 4;         // 100
            Humista |= 4;         // 100
            ES=1;
        }
        else
        {
            ES=0;
            Tempsta &= 3;         // 011
            Humista &= 3;         // 011
            ES=1;
        }

        if(TempEcount > 14400)
        {
            ES=0;
            Tempsta |= 2;         // 010
            ES=1;
        }
        else
        {
            ES=0;
            Tempsta &= 5;         // 101
            ES=1;
        }

        if(HumiEcount > 14400)
        {
            ES=0;
            Humista |= 2;         // 010
            ES=1;
        }
        else
        {
            ES=0;
            Humista &= 5;         // 101
            ES=1;
        }

        if(Tcount > 10000)
        {
            ES=0;
            Tempsta |= 1;         // 001
            Humista |= 1;         // 001
            ES=1;
        }
        else
        {
            Tcount++;
            ES=0;
            Tempsta &= 6;         // 110
            Humista &= 6;         // 110
            ES=1;
        }

        if(MeasQ == 1)
        {
            MeasQ=0;
            if(Quad == 0)
                S0_pros();          // Temperature      A
            else if(Quad == 2)
                S1_pros();          // Humidity      Calculation
            else if(Quad == 3)
                Time_pros();
        }
    }
}

/*************************** Initialize ************************************/
void Initialize(void)
{
    unsigned int i,j;

    IE=0x80;
    PCON=0x80;          // Baudrate Double set

    TMOD = 0x25;        // T0 = Internal Timer 16Bit no,Prescaler,
                        // T1 = Using to Serial
    TH1 = 0xfa;
    TL1 = 0xfa;         // 23Mhz = F3, 22Mhz = F4,18Mhz = F6, 11Mhz = fa Set
    TH0=0xff;
    TL0=0xff;
    SCON=0x50;

    Rpoint=0;
    Spoint=0;
    Comon=0;
    Quad=1;
    Second=1;
    Minute=0;
    Tcount=0;

    Tcount=0;
    TempEcount= 0;
    HumiEcount= 0;

    TempMsum  = 500;
    TempTsum  = 500;
    TempMin   = 500;
    TempTen   = 500;

    TempOmax = 500;
    TempMmax = 500;
    TempTmax = 500;
    TempOmin = 500;
    TempMmin = 500;
    TempTmin = 500;

    TempMsmax= 500;
    TempMsmin= 500;
    TempTsmax= 500;
    TempTsmin= 500;
// ************************
    HumiMsum=0;
    HumiTsum=0;

    HumiMin=0;
    HumiTen=0;

    HumiOmax=0;
    HumiMmax=0;
    HumiTmax=0;
    HumiOmin=0;
    HumiMmin=0;
    HumiTmin=0;

    HumiMsmax=0;
    HumiMsmin=0;
    HumiTsmax=0;
    HumiTsmin=0;

            // Bit Default Set
    Init=1;
    MeasQ=1;
    Adcerr=0;
            // Hard Ware Set up ID READ
    do
    {
        ID=read_XDATA(VID);
        for(i=10; i != 0; i--)
        {
            if(ID != read_XDATA(VID))
                break;
        }
    } while(i != 0);

    ID  = (0xff-ID) & 0xff;     /* ID Number Complemently   */

    TempID=2;
    HumiID=5;

    BlueL=0;
    RedL=0;
    Receve=0;
    Transm=0;

    Mux0=1;     // Humidity Set
    Mux1=0;
    Mux2=0;

    TR1=1;
    PS =1;
    IT0=1;      /* Edge Trigger */
    EX0=1;      /* 4 Hz Clock Enalble */
    ES =1;      /* Serial Interrupt Enable  */
}

/***************************  Temperature  *********************************/
void S0_pros      (void)
{
    unsigned int i,j;

    SRH=0;                          // Holding
    for(i=0; i < 15; i++)
    {
        i++;
        i--;
    }                               // Delay
    write_XDATA(ADCMSB,0xff);       // Converting Start
    for(i=0; i < 10; i++)
    {
        i++;
        i--;
    }
    i=0;                            // Delay
    while(STS == 1)                 // Converting Delay
    {
        if(i++ > 100)
            break;
    }
    if(i > 100)
        Adcerr=1;
    else
        Adcerr=0;

    ES=0;
    Tempreal = read_XDATA(ADCLSB) >> 4;
    Tempreal|= read_XDATA(ADCMSB) << 4;      // Measuring Wind Direction
    ES=1;
    SRH=1;                          // Sampling

    i=(Tempreal/4.095)+500.;        // Scale Calculate

    Tempd[Second%10]=i;
    i=0;
    for(j=0;j < 10;j++)
        i+=Tempd[j];
    i=i/10.;

    ES=0;
    TempSec=i;
    ES=1;
    TempMsum+=i;
    TempTsum+=i;

    if(Init)
    {
        for(j=0;j<10;j++)
            Tempd[j]=i;
        TempMin = i;
        TempTen = i;
        TempOmax= i;
        TempOmin= i;

        TempMsmax=i;
        TempMsmin=i;
        TempTsmax=i;
        TempTsmin=i;
    }
    else if(TempOmax < i)           // One Day
    {
        ES=0;
        TempOmax=i;
        ES=1;
    }
    else if(TempOmin > i)
    {
        ES=0;
        TempOmin=i;
        ES=1;
    }

    if(TempMsmax < i)           // One Minute
        TempMsmax=i;
    else if(TempMsmin > i)
        TempMsmin=i;

    if(TempTsmax < i)           // Ten Minute
        TempTsmax=i;
    else if(TempTsmin > i)
        TempTsmin=i;

    if(OldTemp == i)
    {
        if(TempEcount < 14401)
            TempEcount++;
    }
    else
        TempEcount=0;
    OldTemp=i;

    Mux0=1;             // Humidity Set
    Mux1=0;
    Mux2=0;

    Init=0;
}

/***************************  Humidity     *********************************/
void S1_pros      (void)
{
    unsigned int i,j;

    SRH=0;                          // Holding
    for(i=0; i < 15; i++)
    {
        i++;
        i--;
    }                               // Delay
    write_XDATA(ADCMSB,0xff);       // Converting Start
    for(i=0; i < 10; i++)
    {
        i++;
        i--;
    }
    i=0;                            // Delay
    while(STS == 1)                 // Converting Delay
    {
        if(i++ > 100)
            break;
    }
    if(i > 100)
        Adcerr=1;
    else
        Adcerr=0;

    ES=0;
    Humireal  = read_XDATA(ADCLSB) >> 4;
    Humireal |= read_XDATA(ADCMSB) << 4;      // Measuring Wind Direction
    ES=1;
    SRH=1;                          // Sampling

    i=Humireal/4.095;        // Scale Calculate

    Humid[Second%10]=i;
    i=0;
    for(j=0;j < 10;j++)
        i+=Humid[j];
    i=i/10.;

    if(i > 999)
        i=999;
    ES=0;
    HumiSec=i;
    ES=1;
    HumiMsum+=i;
    HumiTsum+=i;

    if(Init)
    {
        for(j=0;j<10;j++)
            Humid[j]=i;
        HumiMin=i;
        HumiTen=i;
        HumiOmax=i;
        HumiOmin=i;

        HumiMsmax=i;
        HumiMsmin=i;
        HumiTsmax=i;
        HumiTsmin=i;
    }
    else if(HumiOmax < i)
    {
        ES=0;
        HumiOmax=i;
        ES=1;
    }
    else if(HumiOmin > i)
    {
        ES=0;
        HumiOmin=i;
        ES=1;
    }
    
    if(HumiMsmax < i)           // One Minute
        HumiMsmax=i;
    else if(HumiMsmin > i)
        HumiMsmin=i;

    if(HumiTsmax < i)           // Ten Minute
        HumiTsmax=i;
    else if(HumiTsmin > i)
        HumiTsmin=i;

    if(OldHumi == i)
    {
        if(HumiEcount < 14401)
            HumiEcount++;
    }
    else
        HumiEcount=0;
    OldHumi=i;

    Mux0=0;             // Temperature Set
    Mux1=0;
    Mux2=0;
}

/*********************** External Interrupt 0 ******************************/
interrupt void EX0_int(void)
{
    if(Quad++ == 3)
        Quad=0;
    MeasQ=1;
}
/*********************** One, Ten Data Processing **************************/
void Time_pros(void)
{
    if(++Second == 60)
    {
        Second=0;
        Minute=++Minute%10;
        One_pros();
        if(Minute == 0)
            Ten_pros();
    }
    Tcount=0;
}

/*********************** One Minute Processing *****************************/
void One_pros(void)
{
    ES=0;
    TempMin = TempMsum/60.;
    HumiMin = HumiMsum/60.;

    TempMmax= TempMsmax;
    TempMmin= TempMsmin;
    HumiMmax= HumiMsmax;
    HumiMmin= HumiMsmin;
    ES=1;

    TempMsmax=TempSec;
    TempMsmin=TempSec;
    HumiMsmax=HumiSec;
    HumiMsmin=HumiSec;

    TempMsum = 0;
    HumiMsum = 0;
}

/*********************** Ten Minute Processing *****************************/
void Ten_pros(void)
{
    ES=0;
    TempTen = TempTsum/600.;
    HumiTen = HumiTsum/600.;

    TempTmax= TempTsmax;
    TempTmin= TempTsmin;
    HumiTmax= HumiTsmax;
    HumiTmin= HumiTsmin;
    ES=1;

    TempTsmax=TempSec;
    TempTsmin=TempSec;
    HumiTsmax=HumiSec;
    HumiTsmin=HumiSec;

    TempTsum = 0;
    HumiTsum = 0;
}

/*********************** Serial Interrupt **********************************/
interrupt void  SCON_int (void)          /* Serial Port */
{
    unsigned char i,j;

    if(RI == 1)
    {
        RI=0;
        Receve=1;
        BlueL=1;
        Blueb=LightL;
        Rbuf[Rpoint++]=SBUF;

        if( Rbuf[Rpoint-1]  == 0xfe && Rbuf[Rpoint-2]  == 0xff
         && Rbuf[Rpoint-10] == 0xff && Rbuf[Rpoint-11] == 0xff)
        {
            j=0;
            for(i=9; i != 3; i--)
                j ^= Rbuf[Rpoint-i];
            if(j == Rbuf[Rpoint-3])
            {
                if(Rbuf[Rpoint-8] == TempID)            // Temp 1 Call
                {
                    switch(Rbuf[Rpoint-9])
                    {
                        case 'M':
                            Rpoint=0;
                            Sbuf[16]= 0xff;             // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'M';
                            Sbuf[13]= Tempsta;
                            Sbuf[12]= TEMPCODE;         // Sensor Code
                            Sbuf[11]= TempID;           // Sensor ID
                            Sbuf[10]= TempSec >> 8;
                            Sbuf[9] = TempSec;
                            Sbuf[8] = 0;                // Special Data
                            Sbuf[7] = 0;
                            Sbuf[6] = TempOmin >> 8;
                            Sbuf[5] = TempOmin;
                            Sbuf[4] = TempOmax >> 8;
                            Sbuf[3] = TempOmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case 'O':
                            Rpoint=0;
                            Sbuf[16]= 0xff;             // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'O';
                            Sbuf[13]= Tempsta;
                            Sbuf[12]= TEMPCODE;         // Sensor Code
                            Sbuf[11]= TempID;           // Sensor ID
                            Sbuf[10]= TempMin >> 8;
                            Sbuf[9] = TempMin;
                            Sbuf[8] = 0;            // Special Data
                            Sbuf[7] = 0;
                            Sbuf[6] = TempMmin >> 8;
                            Sbuf[5] = TempMmin;
                            Sbuf[4] = TempMmax >> 8;
                            Sbuf[3] = TempMmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case 'T':
                            Rpoint=0;
                            Sbuf[16]= 0xff;             // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'T';
                            Sbuf[13]= Tempsta ;
                            Sbuf[12]= TEMPCODE;         // Sensor Code
                            Sbuf[11]= TempID;           // Sensor ID
                            Sbuf[10]= TempTen >> 8;
                            Sbuf[9] = TempTen;
                            Sbuf[8] = 0;            // Special Data
                            Sbuf[7] = 0;
                            Sbuf[6] = TempTmin >> 8;
                            Sbuf[5] = TempTmin;
                            Sbuf[4] = TempTmax >> 8;
                            Sbuf[3] = TempTmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case 'R':
                            Rpoint=0;
                            Sbuf[16]= 0xff;             // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'R';
                            Sbuf[13]= Tempsta;
                            Sbuf[12]= TEMPCODE;         // Sensor Code
                            Sbuf[11]= TempID;           // Sensor ID
                            Sbuf[10]= Tempreal >> 8;
                            Sbuf[9] = Tempreal;
                            Sbuf[8] = 0;            // Special Data
                            Sbuf[7] = 0;
                            Sbuf[6] = TempOmin >> 8;
                            Sbuf[5] = TempOmin;
                            Sbuf[4] = TempOmax >> 8;
                            Sbuf[3] = TempOmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case '@':
                            TempOmax=Rbuf[Rpoint-5] << 8 | Rbuf[Rpoint-4];
                            TempOmin=Rbuf[Rpoint-7] << 8 | Rbuf[Rpoint-6];
                            Rpoint=0;
                            break;
                    }
                }
                else if(Rbuf[Rpoint-8] == HumiID)          // Humidity 1 Call
                {
                    switch(Rbuf[Rpoint-9])
                    {
                        case 'M':
                            Rpoint=0;
                            Sbuf[16]= 0xff;             // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]='M';
                            Sbuf[13]= Humista;
                            Sbuf[12]= HUMICODE;          // Sensor Code
                            Sbuf[11]= HumiID;            // sensor code & ID
                            Sbuf[10]= HumiSec >> 8;
                            Sbuf[9] = HumiSec;
                            Sbuf[8] = 0;                // Special Data
                            Sbuf[7] = 0;
                            Sbuf[6] = HumiOmin >> 8;
                            Sbuf[5] = HumiOmin;
                            Sbuf[4] = HumiOmax >> 8;
                            Sbuf[3] = HumiOmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case 'O':
                            Rpoint=0;
                            Sbuf[16]= 0xff;             // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]='O';
                            Sbuf[13]= Humista;
                            Sbuf[12]= HUMICODE;          // Sensor Code
                            Sbuf[11]= HumiID;            // sensor code & ID
                            Sbuf[10]= HumiMin >> 8;
                            Sbuf[9] = HumiMin;
                            Sbuf[8] = 0;            // Special Data
                            Sbuf[7] = 0;
                            Sbuf[6] = HumiMmin >> 8;
                            Sbuf[5] = HumiMmin;
                            Sbuf[4] = HumiMmax >> 8;
                            Sbuf[3] = HumiMmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case 'T':
                            Rpoint=0;
                            Sbuf[16]= 0xff;             // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]='T';
                            Sbuf[13]= Humista;
                            Sbuf[12]= HUMICODE;          // Sensor Code
                            Sbuf[11]= HumiID;            // sensor ID
                            Sbuf[10]= HumiTen >> 8;
                            Sbuf[9] = HumiTen;
                            Sbuf[8] = 0;                // Special Data
                            Sbuf[7] = 0;
                            Sbuf[6] = HumiTmin >> 8;
                            Sbuf[5] = HumiTmin;
                            Sbuf[4] = HumiTmax >> 8;
                            Sbuf[3] = HumiTmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case 'R':
                            Rpoint=0;
                            Sbuf[16]= 0xff;             // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]='R';
                            Sbuf[13]= Humista;
                            Sbuf[12]= HUMICODE;          // Sensor Code
                            Sbuf[11]= HumiID;            // sensor code & ID
                            Sbuf[10]= HumiSec >> 8;
                            Sbuf[9] = HumiSec;
                            Sbuf[8] = 0;            // Special Data
                            Sbuf[7] = 0;
                            Sbuf[6] = HumiOmin >> 8;
                            Sbuf[5] = HumiOmin;
                            Sbuf[4] = HumiOmax >> 8;
                            Sbuf[3] = HumiOmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case '@':
                            HumiOmax=Rbuf[Rpoint-5] << 8 | Rbuf[Rpoint-4];
                            HumiOmin=Rbuf[Rpoint-7] << 8 | Rbuf[Rpoint-6];
                            Rpoint=0;
                            break;
                    }
                }
                else if(Rbuf[Rpoint-9] == 'I')
                {
                    Rpoint=0;
                    TempMin   = TempSec;
                    TempTen   = TempSec;
                    TempOmin  = TempSec;
                    TempOmax  = TempSec;

                    HumiMin   = HumiSec;
                    HumiTen   = HumiSec;
                    HumiOmin  = HumiSec;
                    HumiOmax  = HumiSec;
                }
            }
        }
        if(Rpoint > 90)
            Rpoint=0;
    }

    if(TI == 1)
    {
        TI=0;
        if(Comon == 1)
        {
            if( Spoint-- != 0 )
            {
                SBUF=Sbuf[Spoint];
                Transm=1;
                RedL=1;
                Redb=LightL;
            }
            else
                Comon=0;
        }
    }
}
