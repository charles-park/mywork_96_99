/*   ==================================
          DATA STORAGE MEMORY MAP
     ================================== */
/*    ------------------------
          C O N S T A N T
      ------------------------  */
#define     OK          1
#define     NO          0

#define     WsID        1
#define     WdID        0
/*    ------------------------
          B I T   M A P
      ------------------------  */
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
static  bit     Amos;
static  bit     Unit;

/*    ------------------------
       X - D A T A   A R E A
      ------------------------  */
// ***** Using System Variable ******
static  unsigned    char    Quad;
static  unsigned    char    Minute;
static  unsigned    char    Second;
static  unsigned    char    Rbuf[256];
static  unsigned    char    Rpoint;
static  unsigned    char    Sbuf[256];
static  unsigned    int     Spoint;
static  unsigned    int     Sepoint;

static  unsigned    int     Blueb;
static  unsigned    int     Redb;
static  unsigned    int     TEcount;    // Quad Time Error Count
static  unsigned    char    ID;

// ***** Using Wind Speed ******
static  unsigned    int     Nws;
static  unsigned    int     Ows;
static  unsigned    int     WsEvalue;   // Compare To  Old value VS New value
static  unsigned    int     WsEcount;   // Error Count

// ***** Using Wind Direction ******
static  unsigned    int     WdEvalue;   // Compare To  Old value VS New value
static  unsigned    int     WdEcount;   // Error Count
static              double  Calv1;      //  For Wind Direction Cos, Sin Calculation
static              double  Calv2;      //  For Wind Direction Cos, Sin Calculation

// ***** Using Common Variable ******
static  unsigned    int     WdsEcount;  // Wds Error Count
static              float   newv;
static              float   oldv;
static              float   flag;
static              float   value;

struct Wind {
    unsigned int    Head;       //         17, 16
    unsigned char   Kind;       //         15
    unsigned char   Status;     //         14
    unsigned char   Scode;      //         13
    unsigned char   SID;        //         12
    unsigned int    Ave;        //         11, 10
    unsigned int    Spe;        //          9,  8
    unsigned int    Min;        //          7,  6
    unsigned int    Max;        //          5,  4
    unsigned char   Cks;        //          3
    unsigned int    End;        //          2,  1
} Ws_sec,Ws_tec,Ws_one,Ws_ten,Wd_sec,Wd_tec,Wd_one,Wd_ten;

union send_buf{
    struct Wind     buf;
    unsigned char   Sbuf[sizeof(struct Wind)];
} Send;
// ********
//          Head  2    ; 11, 10
//          Kind  1    ;  9
//          SID   1    ;  8
//          Min   2    ;  7,  6
//          Max   2    ;  5,  4
//          Chk   1    ;  3
//          Tail  2    ;  2,  1
//
//
//
// ********
struct Wdvalue {
    unsigned int    Max;
    unsigned int    Min;
             double Sin;    //  For Wind direction sum Sin
             double Cos;    //  For Wind direction sum Cos
    unsigned int    Sum;    //  Using Transmit to Data
} Wdv_sec,Wdv_tec,Wdv_one,Wdv_ten;
// -------
struct Wsvalue {
    unsigned int    Max;
    unsigned int    Min;
             long   Sum;    //  For Wind Speed Sum
} Wsv_sec,Wsv_tec,Wsv_one,Wsv_ten;
// **********
/*    ------------------------
           I / O   M A P
      ------------------------ */
#define     VID         0x8000      /* View ID Number */
#define     ADCMSB      0xC000      /* A/D Converter High Byte */
#define     ADCLSB      0xC001      /* A/D Converter Low  Byte */
