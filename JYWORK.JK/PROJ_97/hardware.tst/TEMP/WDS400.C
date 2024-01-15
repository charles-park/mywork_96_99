#include <c:\cci_hdsl\include\io51.h>
#include <c:\cci_hdsl\include\math.h>
#include <wds400.h>

#pragma memory=code
char Title[10][65]={
"/ /// /////////////////////////////////////////////// /// /",
"/ /// /////////////////////////////////////////////// /// /",
"/ /// ///        Wind Speed & Direction Board     /// /// /",
"/ /// ///             (General,Unit,Vector)       /// /// /",
"/ /// ///       JINYANG  INDUSTRIAL.,CO LTD       /// /// /",
"/ /// ///       Program By  Hae-suk. JIN          /// /// /",
"/ /// ///     Start :  1996.10. 25                /// /// /",
"/ /// ///       End :  1996.11.     Ver 4.0       /// /// /",
"/ /// /////////////////////////////////////////////// /// /",
"/ /// /////////////////////////////////////////////// /// /"
};
//               Memory Default Set
//  Adirection Default Value   Aspeed Default Value
//   254 ;  22.5          45 ;  3 m/s * 1.9444 = knot
//   509 ;  45            80 ;  5 m/s
//   763 ;  67.5         160 ; 10 m/s
//  1018 ;  90           248 ; 15 m/s
//  1272 ; 112.5         324 ; 20 m/s
//  1526 ; 135           410 ; 25 m/s
//  1781 ; 157.5         495 ; 30 m/s
//  2035 ; 180           579 ; 35 m/s
//  2289 ; 202.5         663 ; 40 m/s
//  2544 ; 225           748 ; 45 m/s
//  2798 ; 247.5         833 ; 50 m/s
//  3053 ; 270           918 ; 55 m/s
//  3307 ; 292.5        1003 ; 60 m/s
//  3561 ; 315          1088 ; 65 m/s
//  3816 ; 337.5        1172 ; 70 m/s
//  4070 ; 360          1257 ; 75 m/s

static  unsigned    int Adirection[16] = {
 254,509, 763,1018,1272,1526,1781,2035,2289,2544,2798,3053,3307,3561,3816,4070};
// 0   1   2   3    4    5    6    7    8    9   10   11   12   13   14   15
static  unsigned    int Aspeed[16] = {
//   3   5   10   15   20   25   30   35   40   45   50   55   60   65   70   75
    38,  72, 152, 227, 303, 382, 460, 544, 627, 706, 785, 864,944 ,1022,1101,1181};
//  45,  80, 160, 248, 324, 410, 495, 579, 663, 748, 833, 918,1003,1088,1172,1257};
// 0   1    2    3    4    5    6    7    8    9   10   11  12   13   14   15
#pragma memory=default

#define     CS      P1.3
#define     CLK     P1.2
#define     DI      P1.1
#define     DO      P1.0
#define     RedL    P1.7        // Transm
#define     BlueL   P1.6        // Receve
#define     Wdt     P1.5
#define     LightL  0x800

interrupt   void    EX0_int(void);      // 4 Hz (Time Process)
interrupt   void    T0_int(void);       // Wind Speed Buffer
interrupt   void    T2_int(void);       // 4 Hz Watch
interrupt   void    SCON_int(void);     // Internal Serial Communication
			void    Sec_pros(void);     // One Sec Processing
            void    Tec_pros(void);     // Ten Sec Processing
            void    One_pros(void);     // One Minute Processing
            void    Ten_pros(void);     // Ten Minute Processing
            void    Call_pros(void);    // Data Calling Processing
            void    Checks(void);       // Make it Checksum
            void    Initialize(void);

void Initialize(void)
{
    unsigned int i;

    IE=0x80;
    PCON=0x80;          // Baudrate double set
    P2=0xff;

    TMOD = 0x25;        // T0 = counter 16Bit no,Prescaler,

    TH1 = 0xf4;
    TL1 = 0xf4;        // 23Mhz = F3, 22Mhz = F4,18Mhz = F6, 11Mhz = fa Set
    TH0 = 0;
    TL0 = 0;

    TH2=0;
    TL2=0;
    RCAP2L=0x00;
    RCAP2H=0x00;

    SCON=0x50;

    Rpoint=0;
    Spoint=0;
    Comon=0;
    Quad=2;
    Second=1;
    Minute=0;
    Nws=0;
    Ows=0;

    WsEcount=0;
    WdEcount=0;
    WdsEcount=0;
    TEcount=0;

    Ws_sec.Head = Ws_tec.Head = Ws_one.Head = Ws_ten.Head = 0xffff;
    Wd_sec.Head = Wd_tec.Head = Wd_one.Head = Wd_ten.Head = 0xffff;

    Ws_sec.Kind = Wd_sec.Kind = 'M';
    Ws_tec.Kind = Wd_tec.Kind = 't';
    Ws_one.Kind = Wd_one.Kind = 'O';
    Ws_ten.Kind = Wd_ten.Kind = 'T';

    Wd_sec.Scode = Wd_tec.Scode = Wd_one.Scode = Wd_ten.Scode = 0x00;
    Ws_sec.Scode = Ws_tec.Scode = Ws_one.Scode = Ws_ten.Scode = 0x01;
    
    Wd_sec.SID   = Wd_tec.SID   = Wd_one.SID   = Wd_ten.SID   = 0x00;
    Ws_sec.SID   = Ws_tec.SID   = Ws_one.SID   = Ws_ten.SID   = 0x01;
    
    Ws_sec.End   = Ws_tec.End   = Ws_one.End   = Ws_ten.End   = 0xfffe;
    Wd_sec.End   = Wd_tec.End   = Wd_one.End   = Wd_ten.End   = 0xfffe;

    // Bit Default Set
    Init=1;
    Aderr=0;
    MeasQ=0;
    Terror=0;

/* Hard Ware Set up ID READ */
    do {
        ID=read_XDATA(VID);
        for(i=10; i != 0; i--)  {
            if(ID != read_XDATA(VID))
                break;
        }
    } while(i != 0);

    ID=~ID;

    Unit=0;                     // Unit is "m/s"
    if((ID & 0x40) == 0x40)
        Unit=1;                 // Unit is "kont"

    Amos=0;                     // None A-MOS General Processing
    if((ID & 0x20) == 0x20)
        Amos=1;                 // A-MOS Processing(Vector)

//    ID &= 0x0f;

    BlueL=0;
    RedL=0;

    TR0=1;      // External Counting Run
    TR1=1;      // Serial Source Clock Run
    TR2=1;      // Watch Quad Time "RUN
    IT0=1;      // EX0 Edge Trigger
    ES =1;      // Serial Interrupt Enable
    ET2=1;      // Quad Time Watcher Enable
    PX0=1;      // External Interrupt Priority High Set
    EX0=1;      // External Interrupt Enable 4hz
}

void main(void)
{
    unsigned int i;

    Initialize();
    while(1)
    {
        Wbit=~Wbit;
        Wdt=Wbit;

        if(Receve) {
            if(!Blueb--) {
                BlueL=0;
                Receve=0;
            }
        }
        if(Transm)  {
            if(!Redb--) {
                RedL=0;
                Transm=0;
            }
        }

        if(WdsEcount >= 43200 || WsEcount >= 21600)
            Ws_sec.Status = Ws_tec.Status = Ws_one.Status = Ws_ten.Status |= 2;         // 010
        else
            Ws_sec.Status = Ws_tec.Status = Ws_one.Status = Ws_ten.Status &= 5;         // 101

        if(WdsEcount >= 43200 || WdEcount >= 21600)
            Wd_sec.Status = Wd_tec.Status = Wd_one.Status = Wd_ten.Status |= 2;         // 010
        else
            Wd_sec.Status = Wd_tec.Status = Wd_one.Status = Wd_ten.Status &= 5;         // 101

        if(Aderr)
            Wd_sec.Status = Wd_tec.Status = Wd_one.Status = Wd_ten.Status |= 4;         // 100
        else
            Wd_sec.Status = Wd_tec.Status = Wd_one.Status = Wd_ten.Status &= 3;         // 011
        
        if(Terror) {
            Ws_sec.Status = Ws_tec.Status = Ws_one.Status = Ws_ten.Status |= 1;         // 001
            Wd_sec.Status = Wd_tec.Status = Wd_one.Status = Wd_ten.Status |= 1;         // 001
        }
        else    {
            Ws_sec.Status = Ws_tec.Status = Ws_one.Status = Ws_ten.Status &= 6;         // 110
            Wd_sec.Status = Wd_tec.Status = Wd_one.Status = Wd_ten.Status &= 6;         // 110
        }

        Call_pros();
        if(MeasQ == 1)  {
            MeasQ=0;
            Sec_pros();
            if(Second%10 == 0)
                Tec_pros();
            if(Second == 0) {
                One_pros();
                if(Minute == 0)
                    Ten_pros();
            }
        }
    }
}

void Sec_pros(void)
{
    unsigned int i;

    if(Unit)    {
        EX0=0;
        if(Wsv_sec.Sum == 0)
            Ws_sec.Ave = 0;
        else if(Wsv_sec.Sum <= Aspeed[0])    {
            value=5.835/(float)Aspeed[0];
            Ws_sec.Ave = value*Wsv_sec.Sum*10;
        }
        else if(Wsv_sec.Sum > Aspeed[0] && Wsv_sec.Sum <= Aspeed[1])  {
            newv=Aspeed[1];
            oldv=Aspeed[0];
            value=3.890/(float)(newv-oldv);
            Ws_sec.Ave = ((value*Wsv_sec.Sum)-(value*oldv)+5.835)*10.;
        }
        else    {
            oldv=Aspeed[1];
            flag=9.725;
            for(i=2; i != 16; i++)  {
                flag += 9.725;
                newv=Aspeed[i];
                if( Wsv_sec.Sum < newv && Wsv_sec.Sum >= oldv)    {
                    value=(float)(flag-(flag-9.725))/(newv-oldv);
                    Ws_sec.Ave = ((value*Wsv_sec.Sum) - (value*oldv) + ( flag-9.725))*10.;
                    break;
                }
                else
                    oldv=newv;
            }
        }
        EX0=1;
    }
    else    {
        EX0=0;
        if(Wsv_sec.Sum == 0)
            Ws_sec.Ave = 0;
        else if(Wsv_sec.Sum <= Aspeed[0])    {
            value=3./(float)Aspeed[0];
            Ws_sec.Ave = value*Wsv_sec.Sum*10;
        }
        else if(Wsv_sec.Sum > Aspeed[0] && Wsv_sec.Sum <= Aspeed[1])  {
            newv=Aspeed[1];
            oldv=Aspeed[0];
            value=2./(float)(newv-oldv);
            Ws_sec.Ave = ((value*Wsv_sec.Sum)-(value*oldv)+3)*10.;
        }
        else    {
            oldv=Aspeed[1];
            flag=5.;
            for(i=2; i != 16; i++)  {
                flag += 5.;
                newv=Aspeed[i];
                if( Wsv_sec.Sum < newv && Wsv_sec.Sum >= oldv)    {
                    value=(float)(flag-(flag-5.))/(newv-oldv);
                    Ws_sec.Ave = ((value*Wsv_sec.Sum) - (value*oldv) + ( flag-5.))*10.;
                    break;
                }
                else
                    oldv=newv;
            }
        }
        EX0=1;
    }
    if(Ws_sec.Ave > 600)
        Ws_sec.Ave=600;
    Wsv_tec.Sum+=Ws_sec.Ave;
    Wsv_one.Sum+=Ws_sec.Ave;
    Wsv_ten.Sum+=Ws_sec.Ave;

// ***** Wind Direction Calculation *****
    oldv=0;
    flag=0;
    Sens=0;
    EX0=0;
    for(i=0; i != 16; i++)  {
        flag += 22.5;
        newv=Adirection[i];
        if( Wdv_sec.Sum < newv && Wdv_sec.Sum >= oldv)    {
            Sens=1;
            value=(flag-(flag-22.5))/(newv-oldv);
            Wd_sec.Ave=(float)((value*Wdv_sec.Sum)-(value*oldv)+(flag-22.5)) * 10.;
            break;
        }
        else
            oldv=newv;
    }
    if(!Sens)
        Wd_sec.Ave=3599;
    EX0=1;

    if(Amos && Ws_sec.Ave == 0)
        Wd_sec.Ave=0;


// ********* Wds Error Processing *********
    if(WdEvalue == Wd_sec.Ave && WsEvalue != Ws_sec.Ave) {
        if(WdEcount < 21600)        // ***** 6 Hour
            WdEcount++;
    }
    else
        WdEcount=0;

    if(WsEvalue == Ws_sec.Ave && WdEvalue != Wd_sec.Ave) {
        if(WsEcount < 21600)        // ***** 6 Hour
            WsEcount++;
    }
    else
        WsEcount=0;

    if(WsEvalue == Ws_sec.Ave && WdEvalue == Wd_sec.Ave) {
        if(WdsEcount < 43200)       // **** 12 Hour
            WdsEcount++;
    }
    else
        WdsEcount=0;

    WdEvalue = Wd_sec.Ave;
    WsEvalue = Ws_sec.Ave;

// ********* Maximum & Minimum Processing *********

    if(Ws_sec.Max < Ws_sec.Ave) {       // One Day
        Ws_sec.Max=Ws_sec.Ave;
        Wd_sec.Max=Wd_sec.Ave;
    }
    else if(Ws_sec.Min > Ws_sec.Ave) {
        Ws_sec.Min=Ws_sec.Ave;
        Wd_sec.Min=Wd_sec.Ave;
    }
    // ********
    if(Wsv_tec.Max < Ws_sec.Ave) {      // Tec
        Wsv_tec.Max=Ws_sec.Ave;
        Wdv_tec.Max=Wd_sec.Ave;
    }
    else if(Wsv_tec.Min > Ws_sec.Ave) {
        Wsv_tec.Min=Ws_sec.Ave;
        Wdv_tec.Min=Wd_sec.Ave;
    }
    // ********
    if(Wsv_one.Max < Ws_sec.Ave) {       // One
        Wsv_one.Max=Ws_sec.Ave;
        Wdv_one.Max=Wd_sec.Ave;
    }
    else if(Wsv_one.Min > Ws_sec.Ave) {
        Wsv_one.Min=Ws_sec.Ave;
        Wdv_one.Min=Wd_sec.Ave;
    }
    // ********
    if(Wsv_ten.Max < Ws_sec.Ave) {       // Ten
        Wsv_ten.Max=Ws_sec.Ave;
        Wdv_ten.Max=Wd_sec.Ave;
    }
    else if(Wsv_ten.Min > Ws_sec.Ave) {
        Wsv_ten.Min=Ws_sec.Ave;
        Wdv_ten.Min=Wd_sec.Ave;
    }
    // ********
    Calv1=(double)(Wd_sec.Ave/10.)*(3.141592/180.);
    Calv2=sin(Calv1);
    if(Amos)    Calv2+=Ws_sec.Ave/10.;
    Wdv_tec.Sin+=Calv2;
    Wdv_one.Sin+=Calv2;
    Wdv_ten.Sin+=Calv2;

    Calv2=cos(Calv1);
    if(Amos)    Calv2+=Ws_sec.Ave/10.;
    Wdv_tec.Cos+=Calv2;
    Wdv_one.Cos+=Calv2;
    Wdv_ten.Cos+=Calv2;

    if(Init) {
        Init=0;
        Wd_tec.Ave=Wd_sec.Ave;
        Wd_one.Ave=Wd_sec.Ave;
        Wd_ten.Ave=Wd_sec.Ave;
        
        Wd_sec.Max=Wd_sec.Ave;
        Wd_tec.Max=Wd_sec.Ave;
        Wd_one.Max=Wd_sec.Ave;
        Wd_ten.Max=Wd_sec.Ave;
        
        Wd_sec.Min=Wd_sec.Ave;
        Wd_tec.Min=Wd_sec.Ave;
        Wd_one.Min=Wd_sec.Ave;
        Wd_ten.Min=Wd_sec.Ave;

        Ws_tec.Ave=Ws_sec.Ave;
        Ws_one.Ave=Ws_sec.Ave;
        Ws_ten.Ave=Ws_sec.Ave;
        
        Ws_sec.Max=Ws_sec.Ave;
        Ws_tec.Max=Ws_sec.Ave;
        Ws_one.Max=Ws_sec.Ave;
        Ws_ten.Max=Ws_sec.Ave;
        
        Ws_sec.Min=Ws_sec.Ave;
        Ws_tec.Min=Ws_sec.Ave;
        Ws_one.Min=Ws_sec.Ave;
        Ws_ten.Min=Ws_sec.Ave;
    }
    TEcount=0;
}

void Tec_pros(void)
{
    Ws_tec.Ave=Wsv_tec.Sum/10.;
    Wsv_tec.Sum=0;
    Ws_tec.Max=Wsv_tec.Max;
    Wd_tec.Max=Wdv_tec.Max;
    Ws_tec.Min=Wsv_tec.Min;
    Wd_tec.Min=Wdv_tec.Min;

    Wsv_tec.Max=Ws_sec.Ave;
    Wsv_tec.Min=Ws_sec.Ave;
    Wdv_tec.Max=Wd_sec.Ave;
    Wdv_tec.Min=Wd_sec.Ave;

    Calv1=(double)(atan(Wdv_tec.Sin/Wdv_tec.Cos))*(180./3.141592);
    if(Wdv_tec.Sin < 0 && Wdv_tec.Cos > 0)
        Calv1+=360.;
    else if((Wdv_tec.Sin > 0 && Wdv_tec.Cos < 0) || (Wdv_tec.Sin < 0 && Wdv_tec.Cos < 0))
        Calv1+=180.;
    Wd_tec.Ave=(double)Calv1*10.;
}

void One_pros(void)
{
    Ws_one.Ave=Wsv_one.Sum/60.;
    Wsv_one.Sum=0;
    Ws_one.Max=Wsv_one.Max;
    Wd_one.Max=Wdv_one.Max;
    Ws_one.Min=Wsv_one.Min;
    Wd_one.Min=Wdv_one.Min;

    Wsv_one.Max=Ws_sec.Ave;
    Wsv_one.Min=Ws_sec.Ave;
    Wdv_one.Max=Wd_sec.Ave;
    Wdv_one.Min=Wd_sec.Ave;

    Calv1=(double)(atan(Wdv_one.Sin/Wdv_one.Cos))*(180./3.141592);
    if(Wdv_one.Sin < 0 && Wdv_one.Cos > 0)
        Calv1+=360.;
    else if((Wdv_one.Sin > 0 && Wdv_one.Cos < 0) || (Wdv_one.Sin < 0 && Wdv_one.Cos < 0))
        Calv1+=180.;
    Wd_one.Ave=(double)Calv1*10.;
}

void Ten_pros(void)
{
    Ws_ten.Ave=Wsv_ten.Sum/600.;
    Wsv_ten.Sum=0;
    Ws_ten.Max=Wsv_ten.Max;
    Wd_ten.Max=Wdv_ten.Max;
    Ws_ten.Min=Wsv_ten.Min;
    Wd_ten.Min=Wdv_ten.Min;

    Wsv_ten.Max=Ws_sec.Ave;
    Wsv_ten.Min=Ws_sec.Ave;
    Wdv_ten.Max=Wd_sec.Ave;
    Wdv_ten.Min=Wd_sec.Ave;

    Calv1=(double)(atan(Wdv_ten.Sin/Wdv_ten.Cos))*(180./3.141592);
    if(Wdv_ten.Sin < 0 && Wdv_ten.Cos > 0)
        Calv1+=360.;
    else if((Wdv_ten.Sin > 0 && Wdv_ten.Cos < 0) || (Wdv_ten.Sin < 0 && Wdv_ten.Cos < 0))
        Calv1+=180.;
    Wd_ten.Ave=(double)Calv1*10.;
}

interrupt void T2_int(void)
{
    TR2=0;          /* Timer 2 Stop */
    TH2=0x00;
    TL2=0x00;

    if(TEcount > 40)
        Terror=1;
    else {
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

    
    switch(Quad++)
    {
        case 0x00:
            ES=0;
            if(++Second == 60) {
                Second=0;
                Minute=++Minute%10;
            }
            ES=1;
            break;

        case 0x01:
            TR0=0;                                  // Timer Stop
            Nws  = TH0 << 8;                        // Capturing Wind Speed
            Nws |= TL0;
            TR0=1;                                  // Timer Run
            if(Ows > Nws)
                Wsv_sec.Sum=(long)(Nws+0x10000)-Ows;
            else
                Wsv_sec.Sum=(long)Nws-Ows;
            Ows=Nws;
            Ws_sec.Spe=Nws;
            MeasQ=1;
            break;

        case 0x03:
            Quad=0;
            P3.5 = 0;                               // Holding
            for(i=0; i < 15; i++) {
                i++;
                i--;
            }                                       // Delay
            write_XDATA(ADCMSB,0xff);               // Converting Start
            for(i=0; i < 10; i++) {
                i++;
                i--;
            }
            i=0;                                    // Delay
            while(P3.3 == 1) {                      // Converting Delay
                if(i++ > 100)
                    break;
            }
            if(i > 100)
                Aderr=1;
            else
                Aderr=0;

            Wdv_sec.Sum  = read_XDATA(ADCLSB) >> 4;
            Wdv_sec.Sum |= read_XDATA(ADCMSB) << 4; // Measuring Wind Direction
            P3.5 = 1;                               // Sampling
            break;
    }
}

void Call_pros(void)
{
    unsigned char i,j;

    ES=0;
    if( Rbuf[Rpoint-1]  == 0xfe && Rbuf[Rpoint-2]  == 0xff
     && Rbuf[Rpoint-11] == 0xff && Rbuf[Rpoint-10] == 0xff && Comon == 0) {
        j=0;
        for(i=9; i != 3; i--)
            j ^= Rbuf[Rpoint-i];
        if(j == Rbuf[Rpoint-3]) {
            switch(Rbuf[Rpoint- 8]) {        // Call Sensor ID
                case WsID:
                    switch(Rbuf[Rpoint-9]) {
                        case 'M':
                            Send.buf= Ws_sec;
                            Checks();
                            break;

                        case 't':
                            Send.buf= Ws_tec;
                            Checks();
                            break;

                        case 'O':
                            Send.buf= Ws_one;
                            Checks();
                            break;

                        case 'T':
                            Send.buf= Ws_ten;
                            Checks();
                            break;
                    }
                break;

                case WdID:
                    switch(Rbuf[Rpoint-9]) {
                        case 'M':
                            Send.buf= Wd_sec;
                            Checks();
                            break;

                        case 't':
                            Send.buf= Wd_tec;
                            Checks();
                            break;

                        case 'O':
                            Send.buf= Wd_one;
                            Checks();
                            break;

                        case 'T':
                            Send.buf= Wd_ten;
                            Checks();
                            break;
                    }
                break;
            }
        }
    }
    ES=1;
}

void Checks(void)
{
    unsigned char i;
    
    for(i=2; i!=14;i++) Send.Sbuf[14] ^= Send.Sbuf[i];
    Spoint=0;
    Sepoint=16;
    Rpoint=1;
    Rbuf[0]=0;
    Comon=1;
    SBUF=Send.Sbuf[0];
}

interrupt void SCON_int(void)
{
    if(RI == 1) {
        RI=0;
        Receve=1;
        BlueL=1;
        Blueb=LightL;
        Rbuf[Rpoint++]=SBUF;
    }

    if(TI == 1) {
        TI=0;
        if(Comon == 1) {
            if( Spoint++ < Sepoint ) {
                SBUF=Send.Sbuf[Spoint];
                Transm=1;
                RedL=1;
                Redb=LightL;
            }
            else
                Comon=0;
        }
    }
}

