/*   ==================================
          DATA STORAGE MEMORY MAP
     ================================== */
/*    ------------------------
          C O N S T A N T
      ------------------------  */
#define     OK          1
#define     NO          0
/*    ------------------------
          B I T   M A P
      ------------------------  */
static  bit     Sendd;
static  bit     Send;
static  bit     Comon;
static  bit     Receve;
static  bit     Transm;
static  bit     Sens;
static  bit     MeasQ;
static  bit     Init;
static  bit     Wbit;
static  bit     Aderr;
static  bit     Wderr;
static  bit     Wserr;
static  bit     Terror;

/*    ------------------------
       X - D A T A   A R E A
      ------------------------  */
static  unsigned    char    Wdsta;
static  unsigned    char    Wssta;
static  unsigned    long    Nws;
static  unsigned    long    Ows;
static  signed      char    Quad;
static  unsigned    char    Minute;
static  unsigned    char    Second;
static  unsigned    int     Wdreal;
static  unsigned    int     Wsreal;
static  unsigned    int     Wsabs;
static  unsigned    int     Wdvalue;
static  unsigned    int     Wsvalue;
static  unsigned    int     Wddata[12];
static  unsigned    int     Wsdata[12];
static  unsigned    char    Wpoint;
static  unsigned    int     WdSec;
static  unsigned    int     WdSecd[60];
static  unsigned    int     WsSec;
static  unsigned    int     WsSecd[60];
static  unsigned    int     WdMin;
static  unsigned    int     WdMind[10];
static  unsigned    int     WsMin;
static  unsigned    int     WsMind[10];
static  unsigned    int     WsTen;
static  unsigned    int     WdTen;
static  unsigned    int     WdGmax;
static  unsigned    int     WsGmax;
static  unsigned    int     WdMmax;
static  unsigned    int     WsMmax;
static  unsigned    int     WdTmax;
static  unsigned    int     WsTmax;
static  unsigned    int     WdEvalue;
static  unsigned    int     WsEvalue;
static  unsigned    int     WdEcount;
static  unsigned    int     WsEcount;
static  unsigned    int     WdThvalue;
static  unsigned    int     WsThvalue;

static  unsigned    int     WdsEcount;
static  unsigned    int     TEcount;
static  unsigned    char    Rbuf[100];
static  unsigned    int     Rpoint;
static  unsigned    char    Sbuf[100];
static  unsigned    int     Spoint;
static  unsigned    char    ID;
static  unsigned    int     Blueb;
static  unsigned    int     Redb;
static  unsigned    int     wdb;
static  unsigned    int     oldv;
static  unsigned    int     newv;
static  float   flag;
static  float   value;

static  double      sinSec;     /* / For Wind direction sum     */
static  double      sinOne;     /* / For Wind direction sum     */
static  double      sinTen;
static  double      cosSec;
static  double      cosOne;
static  double      cosTen;
static  unsigned    int sumSec;
static  long        sumOne;     /*  Wind Speed sum  */
static  long        sumTen;


//static  unsigned    int Aspeed[16];
/*  Wind Speed      3, 5,10,15,20,25,30,35,40,45,50,55,60,65,70,75 m/Sec */

//static  unsigned    int Adirection[16];     /* Adjust Direction */
/*  Wind Direction     15, 30, 45, 60, 75, 90,105,120,135,150,165,
                  180,195,210,225,240,255,270,285,300,315,330,345,360. */


/*    ------------------------
           I / O   M A P
      ------------------------ */
#define     VID         0x8000      /* View ID Number */
#define     ADCMSB      0xC000      /* A/D Converter High Byte */
#define     ADCLSB      0xC001      /* A/D Converter Low  Byte */
