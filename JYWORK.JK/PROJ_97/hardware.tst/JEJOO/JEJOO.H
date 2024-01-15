
/**************************  Bit's Configuration  **************************/
static  bit     Comon;
static  bit     Receve;
static  bit     Transm;
static  bit     MeasQ;
static  bit     Init;
static  bit     Adcerr;

/***********************  Variable Configuration  **************************/
static  signed      char    Quad;             // 1/4 Second counter
static  unsigned    char    Second;           // Second Value
static  unsigned    char    Minute;           // Minute Value
static  unsigned    char    ID;
static  unsigned    char    TempID;
static  unsigned    char    HumiID;
static  unsigned    char    Rbuf[256];
static  unsigned    char    Sbuf[256];
static  unsigned    char    Rpoint;
static  unsigned    char    Spoint;
static  unsigned    int     Redb;
static  unsigned    int     Blueb;

static  unsigned    char    Tempsta;
static  unsigned    char    Humista;

static  unsigned    char    Templ;          // About Temp
static  unsigned    char    Temph;
static  unsigned    int     Tempreal;
static  unsigned    char    Humil;
static  unsigned    char    Humih;
static  unsigned    int     Humireal;

static  unsigned    int     TempSec;        // Moment
static  unsigned    int     HumiSec;

static  unsigned    int     TempMin;        // Minute
static  unsigned    long    TempMsum;
static  unsigned    int     HumiMin;
static  unsigned    long    HumiMsum;

static  unsigned    int     TempTen;        // Ten Minute
static  unsigned    long    TempTsum;
static  unsigned    int     HumiTen;
static  unsigned    long    HumiTsum;

static  unsigned    int     TempOmin;       // Minimum of Day
static  unsigned    int     TempOmax;       // Maximum of Day
static  unsigned    int     TempMmin;       // Minimum of Day
static  unsigned    int     TempMmax;       // Maximum of Day
static  unsigned    int     TempTmin;       // Minimum of Day
static  unsigned    int     TempTmax;       // Maximum of Day

static  unsigned    int     HumiOmin;
static  unsigned    int     HumiOmax;
static  unsigned    int     HumiMmin;
static  unsigned    int     HumiMmax;
static  unsigned    int     HumiTmin;
static  unsigned    int     HumiTmax;

static  unsigned    int     TempMsmin;       // Minimum of Day
static  unsigned    int     TempMsmax;       // Maximum of Day
static  unsigned    int     TempTsmin;       // Minimum of Day
static  unsigned    int     TempTsmax;       // Maximum of Day

static  unsigned    int     HumiMsmin;
static  unsigned    int     HumiMsmax;
static  unsigned    int     HumiTsmin;
static  unsigned    int     HumiTsmax;

static  unsigned    int     OldTemp;
static  unsigned    int     OldHumi;

static  unsigned    int     PrecSTR;        // Special Data
static  float               Tempv;

static  unsigned    int     Tcount;
static  unsigned    int     TempEcount;
static  unsigned    int     HumiEcount;

static  unsigned    int     Tempd[30];
static  unsigned    int     Humid[30];

#define     VID    0x4000          // View ID Number
#define     ADCMSB 0x2000          // A/D Converter High Byte
#define     ADCLSB 0x2001          // A/D Converter Low  Byte
#define     THR    0x6000          // DLB = 0
#define     RBR    0x6000          // DLB = 0
#define     DLL    0x6000          // DLB = 1
#define     DLM    0x6001          // DLB = 1
#define     IER    0x6001          // DLB = 0
#define     IIR    0x6002          // DLB = 0
#define     LCR    0x6003          // DLB = 0
#define     MCR    0x6004          // DLB = 0
#define     LSR    0x6005          // DLB = 0
#define     MSR    0x6006          // DLB = 0
