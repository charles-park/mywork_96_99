#include <c:\cci_hdsl\include\io51.h>
#include <c:\cci_hdsl\include\math.h>
#include <wdupg.h>

#pragma memory=code
char Title[10][65]={
"/ /// /////////////////////////////////////////////// /// /",
"/ /// /////////////////////////////////////////////// /// /",
"/ /// ///        Wind Speed & Direction Board     /// /// /",
"/ /// ///                        (Up-Grade)       /// /// /",
"/ /// ///       JINYANG  INDUSTRIAL.,CO LTD       /// /// /",
"/ /// ///       Program By  Hae-suk. JIN          /// /// /",
"/ /// ///     Start :  1995.12. 17                /// /// /",
"/ /// ///       End :  1996. 3. 11  Ver 1.1       /// /// /",
"/ /// /////////////////////////////////////////////// /// /",
"/ /// /////////////////////////////////////////////// /// /"
};
//               Memory Default Set
//  Adirection Default Value   Aspeed Default Value
//   254 ;  22.5       //   45 ;  3 m/s
//   509 ;  45         //   80 ;  5 m/s
//   763 ;  67.5       //  160 ; 10 m/s
//  1018 ;  90         //  248 ; 15 m/s
//  1272 ; 112.5       //  333 ; 20 m/s
//  1526 ; 135         //  420 ; 25 m/s
//  1781 ; 157.5       //  507 ; 30 m/s
//  2035 ; 180         //  593 ; 35 m/s
//  2289 ; 202.5       //  680 ; 40 m/s
//  2544 ; 225         //  766 ; 45 m/s
//  2798 ; 247.5       //  853 ; 50 m/s
//  3053 ; 270         //  940 ; 55 m/s
//  3307 ; 292.5       // 1027 ; 60 m/s
//  3561 ; 315         // 1113 ; 65 m/s
//  3816 ; 337.5       // 1200 ; 70 m/s
//  4070 ; 360         // 1287 ; 75 m/s

static  unsigned    int Adirection[16] = {
 254,509,763,1018,1272,1526,1781,2035,2289,2544,2798,3053,3307,3561,3816,4070};
// 0   1   2   3    4    5    6    7    8    9   10   11   12   13   14   15
static  unsigned    int Aspeed[16] = {
  45, 80,160, 248, 333, 420, 507, 593, 680, 766, 853, 940,1027,1113,1200,1287};
// 0   1   2   3    4    5    6    7    8    9   10   11   12   13   14   15
#pragma memory=default

#define     CS      P1.3
#define     CLK     P1.2
#define     DI      P1.1
#define     DO      P1.0
#define     RedL    P1.7        // Transm
#define     BlueL   P1.6        // Receve
#define     Wdt     P1.5
#define     LightL  0x1000

interrupt   void    EX0_int(void);       // 4 Hz (Time Process)
interrupt   void    T0_int(void);        // Wind Speed Buffer
interrupt   void    T2_int(void);
interrupt   void    SCON_int(void);      // Internal Serial Communication
            void    Quad_pros(void);
            void    Sec_pros(void);
            void    One_pros(void);
            void    Ten_pros(void);
            void    initialize(void);
            int     Ws_calc(unsigned int);

void initialize(void)
{
    unsigned int i;

    IE=0x80;
    PCON=0x80;          // Baudrate double set

    TMOD = 0x25;        // T0 = counter 16Bit no,Prescaler,

    TH1 = 0xf4;
    TL1 = 0xf4;        // 23Mhz = F3, 22Mhz = F4,18Mhz = F6, 11Mhz = fa Set

    TH0=0;
    TL0=0;

    TH2=0;
    TL2=0;
    RCAP2L=0x00;
    RCAP2H=0x00;

    SCON=0x50;
    TR1=1;
    TR0=0;      // External Counting Stop

    Rpoint=0;
    Spoint=0;
    Comon=0;
    Quad=0;
    Second=1;
    Minute=0;
    Wssta=0;
    Wdsta=0;
    Wpoint=0;
    Nws=0;
    Ows=0;

    Sbuf[4]=0;
    Sbuf[5]=0;
    Sbuf[6]=0;
    Sbuf[7]=0;
    for(i=0; i != 60;i++)
    {
        WdSecd[i]=0;
        WsSecd[i]=0;
    }               // 1 Sec, Avarage Dim Clear
    for(i=0; i != 12;i++)
    {
        Wddata[i]=0;
        Wsdata[i]=0;
    }               // 3 Sec, Avarage Dim Clear
    for(i=0; i != 10;i++)
    {
        WdMind[i]=0;
        WsMind[i]=0;
    }               // Minute Dim Clear
    WdMin=0;
    WsMin=0;
    WsMin  = 0;
    WsTen  = 0;
    WsGmax = 0;
    WsMmax = 0;
    WsTmax = 0;
    WsEcount=0;
    WdEcount=0;
    WdsEcount=0;
    TEcount=0;
    sinOne=0;
    sinTen=0;
    cosOne=0;
    cosTen=0;
    sumOne=0;
    sumTen=0;

    WdEvalue=0;
    WsEvalue=0;

    // Bit Default Set
    Send=0;
    Sendd=0;
    Init=1;
    Aderr=0;
    MeasQ=0;
    Terror=0;

    ID  = 0;    // ID Number Complemently (Fix)
    BlueL=0;
    RedL=0;

    PS =1;      // Serial Priority
    TR2=1;      // Quad Time Watcher "RUN
    IT0=1;      // Edge Trigger
    ES =1;      // Serial Interrupt Enable
    ET2=1;      // Quad Time Watcher Enable
    EX0=1;      // External Interrupt Enable 4hz
}

void main(void)
{
    unsigned int i;

    initialize();
    while(1)
    {
        if(Wbit)
        {
            Wdt=0;
            Wbit=0;
        }
        else
        {
            Wdt=1;
            Wbit=1;
        }

        if(Receve)
        {
            if(!Blueb--)
            {
                BlueL=0;     // Receive LED off
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

        if(WdsEcount == 43200 || WsEcount == 21600)
        {
            ES=0;
            Wssta |= 2;         // 010
            ES=1;
        }
        else
        {
            ES=0;
            Wssta &= 5;         // 101
            ES=1;
        }

        if(WdsEcount == 43200 || WdEcount == 21600)
        {
            ES=0;
            Wdsta |= 2;         // 010
            ES=1;
        }
        else
        {
            ES=0;
            Wdsta &= 5;         // 101
            ES=1;
        }

        if(Aderr)
        {
            ES=0;
            Wdsta |= 4;         // 100
            ES=1;
        }
        else
        {
            ES=0;
            Wdsta &= 3;         // 011
            ES=1;
        }
        
        if(Terror)
        {
            ES=0;
            Wssta |= 1;         // 001
            Wdsta |= 1;
            ES=1;
        }
        else
        {
            ES=0;
            Wssta &= 6;         // 110
            Wdsta &= 6;
            ES=1;
        }

        if(MeasQ == 1)
        {
            EX0=0;
            MeasQ=0;
            Quad_pros();
            if(Quad == 0)
            {
                Sec_pros();
                if(Second == 0)
                {
                    One_pros();
                    if(Minute == 0)
                        Ten_pros();
                }
            }
            EX0=1;
        }

    }
}


void Quad_pros(void)
{
    unsigned int i,wssum;
    double sumsin,sumcos,ave;
                //  Wind Speed Guest Average Calculate
    wssum=0;
    for(i=0;i != 12;i++)
        wssum+=Wsdata[i];
    wssum = wssum/3.;
    wssum = Ws_calc(wssum);
    if(wssum > WsGmax)
    {
        ES=0;
        WsGmax = wssum;
        ES=1;
        sumcos=0;
        sumsin=0;
        for(i=0;i != 12; i++)
        {
            ave=(double)(Wddata[i]/10.)*(3.141592/180.);
            sumsin += sin(ave);
            sumcos += cos(ave);
        }
        ave=(double)(atan(sumsin/sumcos))*(180./3.141592);
        if(sumsin < 0 && sumcos > 0)
            ave+=360.;
        else if((sumsin > 0 && sumcos < 0) || (sumsin < 0 && sumcos < 0))
            ave+=180.;
        ES=0;
        WdGmax=(double)ave*10.;
        ES=1;
    }
    TEcount=0;
}

void Sec_pros(void)
{
    unsigned int i,wssum;
    double ave,ave2;

    sinSec=0;
    cosSec=0;
    sumSec=0;
    for(i=9; i != 13; i++)
    {
        wssum   =  Wsdata[(Wpoint+i)%12];      // Wind Speed Sum
        sumSec +=  wssum;
        sumOne +=  wssum;
        sumTen +=  wssum;
                                               // Wind Direction Sum
        ave     = (double)(Wddata[(Wpoint+i)%12]/10.)*(3.141592/180.);
        ave2    = sin(ave);
        sinSec += ave2;
        sinOne += ave2;
        sinTen += ave2;
        ave2    = cos(ave);
        cosSec += ave2;
        cosOne += ave2;
        cosTen += ave2;
    }
    ave=(double)(atan(sinSec/cosSec))*(180./3.141592);
    if(sinSec < 0 && cosSec > 0)
        ave+=360.;
    else if((sinSec > 0 && cosSec < 0) || (sinSec < 0 && cosSec < 0))
        ave+=180.;

    Wsreal=sumSec;
    i=Ws_calc(sumSec);
    ES=0;
    WsSec=i;
    WdSec=(double)ave*10.;
    WsSecd[Second]=WsSec;
    WdSecd[Second]=WdSec;
    ES=1;
    if(Init)
    {
        Init=0;
        ES=0;
        WdMin  = WdSec;
        WdTen  = WdMin;
        WdGmax = WdSec;
        WdMmax = WdSec;
        WdTmax = WdSec;
        WdThvalue=WdSec;

        WsSec  = WsSec*1.33;
        WsMin  = WsSec;
        WsTen  = WsSec;
        WsGmax = WsSec;
        WsMmax = WsSec;
        WsTmax = WsSec;
        WsThvalue=WsSec;
        ES=1;
    }

    if(WsSec != WsEvalue)
    {
        if(WdSec == WdEvalue)
        {
            if(WdEcount < 21600)    /* Wd = 6 Hour Fix Then Error */
                WdEcount++;
        }
        else
            WdEcount=0;
    }
    if(WdSec != WdEvalue)
    {
        if(WsSec == WsEvalue)
        {
            if(WsEcount < 21600)
                WsEcount++;
        }
        else
            WsEcount=0;
    }

    if((WdSec == WdEvalue) && (WsSec == WsEvalue))
    {
        if(WdsEcount < 43200)       /* All Stop Then 12 Hour */
            WdsEcount++;
    }
    else
        WdsEcount=0;
        
    WdEvalue=WdSec;
    WsEvalue=WsSec;
}

void One_pros(void)
{
    unsigned int i;
    double ave;

    ave=(double)(atan(sinOne/cosOne))*(180/3.141592);
    if(sinOne < 0 && cosOne > 0)
        ave+=360.;
    else if((sinOne > 0 && cosOne < 0) || (sinOne < 0 && cosOne < 0))
        ave+=180.;
    i=Ws_calc(sumOne/60);
    ES=0;
    WsMin=i;
    WdMin=(double)ave*10.;
    ES=1;

    WsMind[Minute]=WsMin;
    WdMind[Minute]=WdMin;

    ES=0;
    WsMmax=0;
    WdMmax=0;
    for(i=0;i != 60; i++)
    {
        if(WsSecd[i] > WsMmax)
        {
            WdMmax=WdSecd[i];
            WsMmax=WsSecd[i];
        }
        WsSecd[i]=0;
    }
    ES=1;

    sumOne=0;
    sinOne=0;
    cosOne=0;
    for(i=8;i != 11; i++)
    {
        sumOne+=WsMind[(Minute+i)%10];
        ave=(double)(WdMind[(Minute+i)%10]/10.)*(3.141592/180.);
        sinOne += sin(ave);
        cosOne += cos(ave);
    }
    ave=(double)(atan(sinOne/cosOne))*(180./3.141592);
    if(sinOne < 0 && cosOne > 0)
        ave+=360.;
    else if((sinOne > 0 && cosOne < 0) || (sinOne < 0 && cosOne < 0))
        ave+=180.;

    ES=0;
    WsThvalue=sumOne/3.;
    WdThvalue=(double)ave*10.;
    ES=1;

    sumOne=0;
    sinOne=0;
    cosOne=0;
}

void Ten_pros(void)
{
    unsigned int i;
    double ave;

    ave=(atan(sinTen/cosTen))*(180./3.141592);
    if(sinTen < 0 && cosTen > 0)
        ave+=360.;
    else if((sinTen > 0 && cosTen < 0) || (sinTen < 0 && cosTen < 0))
        ave+=180.;
    i=Ws_calc(sumTen/600.);
    ES=0;
    WsTen=i;
    WdTen=(double)ave*10.;

    WsTmax=0;
    WdTmax=0;
    for(i=0;i != 10; i++)
    {
        if(WsMind[i] > WsTmax)
        {
            WdTmax=WdMind[i];
            WsTmax=WsMind[i];
        }
    }
    ES=1;
    sumTen=0;
    sinTen=0;
    cosTen=0;
}


interrupt void T0_int(void)
{

}

interrupt void T2_int(void)
{
    TR2=0;          /* Timer 2 Stop */
    TH2=0x00;
    TL2=0x00;

    if(TEcount > 20)
        Terror=1;
    else
    {
        Terror=0;
        TEcount++;
    }

    RCAP2L=0x00;
    RCAP2H=0x00;
    TR2=1;          /* Timer 2 Run */
    TF2=0;
}

interrupt void EX0_int(void)
{
    unsigned int i;

    if(Quad++ == 3)
    {
        Quad=0;
        ES=0;
        if(++Second == 60)
        {
            Second=0;
            Minute=++Minute%10;
        }
        ES=1;
    }
    P3.5 = 0;                               // Holding
    for(i=0; i < 15; i++)
    {
        i++;
        i--;
    }                                       // Delay
    write_XDATA(ADCMSB,0xff);               // Converting Start
    for(i=0; i < 10; i++)
    {
        i++;
        i--;
    }
    i=0;                                    // Delay
    while(P3.3 == 1)                        // Converting Delay
    {
        if(i++ > 100)
            break;
    }
    if(i > 100)
        Aderr=1;
    else
        Aderr=0;

    ES=0;
    Wdreal  = read_XDATA(ADCLSB) >> 4;
    Wdreal |= read_XDATA(ADCMSB) << 4;      // Measuring Wind Direction
    ES=1;
    P3.5 = 1;                               // Sampling

    TR0=0;                                  // Timer Stop
    Nws  = TH0 << 8;                        // Capturing Wind Speed
    Nws |= TL0;
    TR0=1;                                  // Timer Run
    if(Ows > Nws)
        Wsabs=(Nws+0x10000)-Ows;
    else
        Wsabs=Nws-Ows;
    Ows=Nws;
                                            // Calculation W_D
    oldv=0;
    flag=0;
    Sens=0;
    for(i=0; i != 16; i++)
    {
        flag += 22.5;
        newv=Adirection[i];
        if( Wdreal < newv && Wdreal >= oldv)
        {
            Sens=1;
            value=(flag-(flag-22.5))/(newv-oldv);
            Wdvalue=(float)((value*Wdreal)-(value*oldv)+(flag-22.5)) * 10.;
            break;
        }
        else
            oldv=newv;
    }
    if(!Sens)
        Wdvalue=3599;
                            // Data Store
    Wpoint = ++Wpoint%12;
    Wddata[Wpoint]=Wdvalue;
    Wsdata[Wpoint]=Wsabs;
    MeasQ=1;
}

interrupt void SCON_int(void)
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
                j ^= Rbuf[Rpoint-i];                // Calculation Check_sum
            if(j == Rbuf[Rpoint-3])
            {
                if(Rbuf[Rpoint-8] == ID)            // Wind Direction Call
                {
                    switch(Rbuf[Rpoint-9])
                    {
                        case 'M':
                            Rpoint=0;
                            Sbuf[16]= 0xff;          // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'M';
                            Sbuf[13]= Wdsta;
                            Sbuf[12]= 0;            // Sensor Code
                            Sbuf[11]= ID;           // sensor code & ID
                            Sbuf[10]= WdSec >> 8;
                            Sbuf[9] = WdSec;
                            Sbuf[8] = 0;
                            Sbuf[7] = 0;
                            Sbuf[6] = 0;
                            Sbuf[5] = 0;
                            Sbuf[4] = WdGmax >> 8;
                            Sbuf[3] = WdGmax;
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
                            Sbuf[16]= 0xff;      // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'O';
                            Sbuf[13]= Wdsta;
                            Sbuf[12]= 0;        // Sensor Code
                            Sbuf[11]= ID;       // Sensor ID
                            Sbuf[10]= WdMin >> 8;
                            Sbuf[9] = WdMin;
                            Sbuf[8] = WdThvalue >> 8;
                            Sbuf[7] = WdThvalue;
                            Sbuf[6] = 0;
                            Sbuf[5] = 0;
                            Sbuf[4]=WdGmax >> 8;
                            Sbuf[3]=WdGmax;
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
                            Sbuf[16]= 0xff;      /* Head */
                            Sbuf[15]=0xff;
                            Sbuf[14]= 'T';
                            Sbuf[13]= 0;
                            Sbuf[12]= 0;        // Sensor Code
                            Sbuf[11]= ID;       // Sensor ID
                            Sbuf[10]= WdTen >> 8;
                            Sbuf[9] = WdTen;
                            Sbuf[8] = 0;
                            Sbuf[7] = 0;
                            Sbuf[6] = 0;
                            Sbuf[5] = 0;
                            Sbuf[4]=WdGmax >> 8;
                            Sbuf[3]=WdGmax;
                            Sbuf[2] = 0;
                            for(j=14;j != 2;j--)
                                Sbuf[2] ^= Sbuf[j];
                            Sbuf[1] = 0xff;
                            Sbuf[0] = 0xfe;
                            Spoint=16;
                            Comon=1;
                            SBUF=Sbuf[16];
                            break;
                        case 'R':                   // Call Real Value
                            Rpoint=0;
                            Sbuf[16]= 0xff;         // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'R';
                            Sbuf[13]= Wdsta;
                            Sbuf[12]= 0;            // Sensor Code
                            Sbuf[11]= ID;           // Sensor ID
                            Sbuf[10]= Wdreal >> 8;
                            Sbuf[9] = Wdreal;
                            Sbuf[8] = WdSec >> 8;
                            Sbuf[7] = WdSec;
                            Sbuf[6] = 0;
                            Sbuf[5] = 0;
                            Sbuf[4] = WdGmax >> 8;
                            Sbuf[3] = WdGmax;
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
                            WdGmax=Rbuf[Rpoint-5] << 8 | Rbuf[Rpoint-4];
                            Rpoint=0;
                            break;
                    }
                }
                if(Rbuf[Rpoint-8] == ID+1)          // Wind Speed Call
                {
                    switch(Rbuf[Rpoint-9])
                    {
                        case 'M':
                            Rpoint=0;
                            Sbuf[16]= 0xff;          // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'M';
                            Sbuf[13]= Wssta;
                            Sbuf[12]= 0x01;         // Sensor Code
                            Sbuf[11]= ID+1;         // Sensor ID
                            Sbuf[10]= WsSec >> 8;
                            Sbuf[9] = WsSec;
                            Sbuf[8] = 0;
                            Sbuf[7] = 0;
                            Sbuf[6] = 0;
                            Sbuf[5] = 0;
                            Sbuf[4] = WsGmax >> 8;
                            Sbuf[3] = WsGmax;
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
                            Sbuf[13]= Wssta;
                            Sbuf[12]= 0x01;             // Sensor Code
                            Sbuf[11]= ID+1;             // Sensor ID
                            Sbuf[10]= WsMin >> 8;
                            Sbuf[9] = WsMin;
                            Sbuf[8] = WsThvalue >> 8;
                            Sbuf[7] = WsThvalue;
                            Sbuf[6] = 0;
                            Sbuf[5] = 0;
                            Sbuf[4]=WsGmax >> 8;
                            Sbuf[3]=WsGmax;
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
                            Sbuf[16]= 0xff;         // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'T';
                            Sbuf[13]= 0;
                            Sbuf[12]= 0x01;         // Sensor Code
                            Sbuf[11]= ID+1;         // Sensor ID
                            Sbuf[10]= WsTen >> 8;
                            Sbuf[9] = WsTen;
                            Sbuf[8] = 0;
                            Sbuf[7] = 0;
                            Sbuf[6] = 0;
                            Sbuf[5] = 0;
                            Sbuf[4]=WsGmax >> 8;
                            Sbuf[3]=WsGmax;
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
                            Sbuf[16]= 0xff;         // Head
                            Sbuf[15]= 0xff;
                            Sbuf[14]= 'R';
                            Sbuf[13]= Wssta;
                            Sbuf[12]= 0x01;         // Sensor Code
                            Sbuf[11]= ID+1;         // Sensor ID
                            Sbuf[10]= Wsreal >> 8;
                            Sbuf[9] = Wsreal;
                            Sbuf[8] = WsSec >> 8;
                            Sbuf[7] = WsSec;
                            Sbuf[6] = 0;
                            Sbuf[5] = 0;
                            Sbuf[4] = WsGmax >> 8;
                            Sbuf[3] = WsGmax;
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
                            WsGmax=Rbuf[Rpoint-5] << 8 | Rbuf[Rpoint-4];
                            Rpoint=0;
                            break;
                    }
                }

                else if(Rbuf[Rpoint-9] == 'I')
                {
                    Rpoint=0;
                    WdGmax=WdSec;
                    WsGmax=WsSec;
                }
                Rpoint = 0;
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

int Ws_calc(unsigned int Wssum)
{

    unsigned int newv,oldv,i,flag;
    float value;
                        /* Calculation W_S  */
    if(Wssum == 0)
        Wssum = 0;
    else if(Wssum <= Aspeed[0])
    {
        value=3./(float)Aspeed[0];
        Wssum = value*Wssum*10;
    }
    else if(Wssum > Aspeed[0] && Wssum <= Aspeed[1])
    {
        newv=Aspeed[1];
        oldv=Aspeed[0];
        value=2./(float)(newv-oldv);
        Wssum = ((value*Wssum)-(value*oldv)+3)*10.;
    }
    else
    {
        oldv=Aspeed[1];
        flag=5.;
        for(i=2; i != 16; i++)
        {
            flag += 5.;
            newv=Aspeed[i];
            if( Wssum < newv && Wssum >= oldv)
            {
                value=(float)(flag-(flag-5.))/(newv-oldv);
                Wssum = ((value*Wssum) - (value*oldv) + ( flag-5.))*10.;
                break;
            }
            else
                oldv=newv;
        }
    }
    if(Wssum > 600)
        return 601;
    else
        return Wssum;
}
