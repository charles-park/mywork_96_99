/*[-]********************************************************************[-]*/
/*[-]      AWS TEMP 1 EA,PRECIPI 1EA,PRECIPITATION-D,HUMI,SUN PROGRAM    [-]*/
/*[-]           U-GRADE PART TEMP       ver PP1.3                        [-]*/
/*[-]  1997.1.21       NAMYANG AWS                 DESIGNED BY PARK      [-]*/
/*[-]********************************************************************[-]*/

#include <io51.h>
#define BPS96   0xFA    /* serial  9600 bps  OSCILATER  FA=11 MHz F4=22 Mz   F6=18 MHz */

/*  DATA TRANSIMITER  SELECT (0-7) */

#define TEMPCODE  2                            /* code set */
#define PRECICODE 3                            /* code set */
#define PREDCODE  4                            /* code set */
#define PRESSCODE 5                            /* code set */
#define HUMICODE  6                            /* code set */
#define DEW       7                            /* code set */
#define SUNCODE   8                            /* code set */
#define SUNOFCODE 9                            /* code set */

static    unsigned  int   TEMP       =2;       /* id set 2 */
static    unsigned  int   PRECIPITION=4;       /* id set 3 */
static    unsigned  int   HUMI       =5;       /* id set 4 */
static    unsigned  int   SUNOF      =7;       /* id set 5 */
static    unsigned  int   SUN        =6;       /* id set 11*/

static    unsigned  int   PRED =299;              /* id set */
static    unsigned  int   PRESS=300;           /* id set */

static    unsigned  char  sum =10;             /*     SUM EA AVERLAGE  */
static    unsigned  char  ID  = 0;             /*     ID SETTING PLUS  */
static    unsigned  char  flag= 0;             /*     CALL             */

static    unsigned  char  clock_conter;
static    unsigned  char  Second;
static    unsigned  char  Minute;
static    unsigned  char  board_start=0;
static    unsigned  char  rain10min=0;
static    unsigned  char  pred_jy;
static    unsigned  char  pre_day;

static    unsigned  char  sensor_id;
static    unsigned  char  press_id;
static    unsigned  int   con,pred,ddd;
static    unsigned  char  Send_conter;
static    unsigned  char  CALL[11];
static    unsigned  char  CALLOK[300];
static    unsigned  char  SEND_DATA[17];

static    unsigned  char  send_check_sdata;
static    unsigned  char  send_check_sdata1;
static    unsigned  char  call_check_sdata;

static    unsigned  char  sensor_temp_status;           /* SENSOR STATUS  */
static    unsigned  char  sensor_precipi_status;        /* SENSOR STATUS  */
static    unsigned  char  sensor_pred_status;           /* SENSOR STATUS  */
static    unsigned  char  sensor_press_status;          /* SENSOR STATUS  */
static    unsigned  char  sensor_humi_status;           /* SENSOR STATUS  */
static    unsigned  char  sensor_sunof_status;          /* SENSOR STATUS  */
static    unsigned  char  sensor_sun_status;            /* SENSOR STATUS  */

static    unsigned  char  for_time=0;
static    unsigned  int   for_temp1=0;
static    unsigned  char  for_pre=0;
static    unsigned  int   for_humi=0;

static    unsigned  char  Temp1h1,Temp1l1;
static    unsigned  char  Temp1h,Temp1l;
static    unsigned  int   Temp1_data;
static    unsigned  int   Temp1S_data;
static    unsigned  int   Temp1M_data;
static    unsigned  int   Temp1T_data;
static    unsigned  long  Temp1Ms_data;
static    unsigned  long  Temp1Ts_data;
static    unsigned  int   Temp1D_max=0;
static    unsigned  int   Temp1D_min=0;
static    unsigned  int   Temp1M_max=0;
static    unsigned  int   Temp1M_min=0;
static    unsigned  int   Temp1T_max=0;
static    unsigned  int   Temp1T_min=0;
static    unsigned  int   Temp1M_smax=0;
static    unsigned  int   Temp1M_smin=0;
static    unsigned  int   Temp1T_smax=0;
static    unsigned  int   Temp1T_smin=0;
static    unsigned  int   Temp1T_count;
static    unsigned  int   Temp1M_count;
static    unsigned  int   Temp1A[10];

static    unsigned  char  Humi1h,Humi1l;
static    unsigned  int   Humi1_data;
static    unsigned  int   Humi1S_data;
static    unsigned  int   Humi1M_data;
static    unsigned  int   Humi1T_data;
static    unsigned  int   Humi1M_sdata;
static    unsigned  int   Humi1T_sdata;
static    unsigned  long  Humi1Ms_data;
static    unsigned  long  Humi1Ts_data;
static    unsigned  int   Humi1D_max=0;
static    unsigned  int   Humi1D_min=0;
static    unsigned  int   Humi1M_max=0;
static    unsigned  int   Humi1M_min=0;
static    unsigned  int   Humi1T_max=0;
static    unsigned  int   Humi1T_min=0;
static    unsigned  int   Humi1M_smax=0;
static    unsigned  int   Humi1M_smin=0;
static    unsigned  int   Humi1T_smax=0;
static    unsigned  int   Humi1T_smin=0;
static    unsigned  int   Humi1T_count;
static    unsigned  int   Humi1M_count;
static    unsigned  int   Humi1A[10];

static    unsigned  char  Pressh,Pressl;
static    unsigned  int   Press_data;
static    unsigned  int   PressS_data;
static    unsigned  int   PressM_data;
static    unsigned  int   PressT_data;
static    unsigned  int   PressM_sdata;
static    unsigned  int   PressT_sdata;
static    unsigned  long  PressMs_data;
static    unsigned  long  PressTs_data;
static    unsigned  int   PressD_max=0;
static    unsigned  int   PressD_min=0;
static    unsigned  int   PressM_max=0;
static    unsigned  int   PressM_min=0;
static    unsigned  int   PressT_max=0;
static    unsigned  int   PressT_min=0;
static    unsigned  int   PressM_smax=0;
static    unsigned  int   PressM_smin=0;
static    unsigned  int   PressT_smax=0;
static    unsigned  int   PressT_smin=0;
static    unsigned  int   PressT_count;
static    unsigned  int   PressM_count;
static    unsigned  int   PressA[10];


static    unsigned  char  Sunh,Sunl;
static    unsigned  int   Sun_data;
static    unsigned  long  SunS_data;
static    unsigned  long  SunS_data1;
static    unsigned  long  SunS_ddata;
static    unsigned  long  SunM_data;
static    unsigned  long  SunT_data;
static    unsigned  int   SunM_sdata;
static    unsigned  int   SunT_sdata;
static    unsigned  long  SunMs_data;
static    unsigned  long  SunTs_data;
static    unsigned  int   SunD_max=0;
static    unsigned  int   SunD_min=0;
static    unsigned  int   SunM_max=0;
static    unsigned  int   SunM_min=0;
static    unsigned  int   SunT_max=0;
static    unsigned  int   SunT_min=0;
static    unsigned  int   SunM_smax=0;
static    unsigned  int   SunM_smin=0;
static    unsigned  int   SunT_smax=0;
static    unsigned  int   SunT_smin=0;
static    unsigned  int   SunT_count;
static    unsigned  int   SunM_count;
static    unsigned  int   SunA[10];

static    unsigned  int   precipi_1sec_conter = 0;
static    unsigned  int   precipi_1min_conter = 0;
static    unsigned  int   precipi_10min_conter= 0;
static    unsigned  int   precipis_1min_conter = 0;
static    unsigned  int   precipis_10min_conter= 0;
static    unsigned  int   precipi_1day_conter = 0;
static    unsigned  int   precipis_1day_conter = 0;

static    unsigned  int   pre_10min_conter=0;
static    unsigned  int   pre_1ea_check=0;
static    unsigned  int   pre_segi_min=0;
static    unsigned  int   pre_1time_segi=0;
static    unsigned  char  pre_con,pre_con1=0;

static    unsigned  char  Pred;
static    unsigned  char  Pred_10min_con=0;

static    unsigned  char  Sunofh,Sunofl;
static    unsigned  int   Sunof_data;
static    unsigned  int   SunofS_data;
static    unsigned  int   SunofSD_data;
static    unsigned  int   SunofM_count;
static    unsigned  int   SunofMD_count;
static    unsigned  int   SunofT_count;
static    unsigned  int   SunofTD_count;
static    unsigned  int   SunofDD_count;

static    unsigned  int   Init_count;
static    unsigned  int   Init_count1;
static    unsigned  int   Humit;
static    unsigned  int   Tempt;
static    unsigned  char  send_conter1;
static    unsigned  int   adconter;
static    unsigned  char  io,DSP;

static    unsigned  int   clock_check_conter=0;
static    unsigned  int   clock_check=0;
static    unsigned  int   adc_check=0;
static    unsigned  int   ad_con=0;
static    unsigned  int   ad_call_data;

static    unsigned  int   A1temp[50];
static    unsigned  int   Stemp[50];
static    unsigned  int   A1Humi[50];
static    unsigned  int   SHumi[50];
static    unsigned  int   A1Sun[50];
static    unsigned  int   SSun[50];
static    unsigned  int   A1Press[50];
static    unsigned  int   SPress[50];
static    unsigned  char  adh,adl;


/***************************************************************************/
interrupt void T0_int           (void);            /* T0       Interrupt 0 */
interrupt void EX0_int          (void);            /* External Interrupt 0 */
interrupt void EX1_int          (void);            /* External Interrupt 1 */
interrupt void SCON_int         (void);            /* Serial Port */
          void initial          (void);
          void delay            (void);
          void delay1           (void);
          void send_check       (void);            /* serial send check */
          void call_check       (void);            /* serial call check */
          void sensor0_read     (void);            /* temp1     */
          void sensor1_read     (void);            /* Humi      */
          void sensor2_read     (void);            /* precipi-d */
          void sensor3_read     (void);            /* press     */
          void sensor4_read     (void);            /* sun       */
          void sensor5_read     (void);            /* sunof   detile  */
          void sensor6_read     (void);            /* sunof   on/off  */
          void Precipi_check    (void);            /* precipi   */
          void ad_read          (void);            /* a/d check */
          void data_read        (void);            /* a/d read  */

/***************************************************************************/

#pragma memory=code     /* rom */
char    string[]={ 1,2,3,4,5,6,7,8,9,0 };
char    LEE1[]={"--- PROGRAM BY ** PARK IN KYU ** --- "};
char    LED2[]={"JIN YANG INDUSTRIAL CO., LTD. TEL (0342) 41-7741 "};
char    LED3[]={" 223-39 SANGDAEWONDONG  JUNGWONGU  SUNGNAM CYTI  KUNGGIDO   KOREAN "};
char    string1[]={ 1,2,3,4,5,6,7,8,9,0 };

/***************************************************************************/
#pragma memory=default  /* ram */
/***************************************************************************/

void initial(void)
{
    char i,ii;
    EA=0;
    PCON=0x80;
    SCON=0x50;
    TMOD=0x21;   /*  50mS TIMER */
    TH1 =BPS96;
    TL1 =BPS96;
    TH0 =100;
    TL0 =255;
    TR0=1;
    TR1=1;

    ET0=1;
    ET1=0;

    Init_count=0;
    for(i=0; i != 10; i++)
    {
        Temp1A[i]=0;
    }

    IT0=1;      /* Edge Trigger */
    IT1=1;      /* Level Trigger */
    EX0=1;      /* Clock */
    EX1=1;      /* precipi */
    PS=1;
    ES=1;
    EA=1;
    P1.2=1;
}

/***************************************************************************/
interrupt void EX0_int (void)           /* External Interrupt 0 */
/***************************************************************************/
{
    clock_conter++;
    clock_check_conter++;
    ad_con=0;
    flag=1;
}

/***************************************************************************/
/*  timer interrupt  0                                                     */
/***************************************************************************/
interrupt void T0_int (void)
{
    TH0 =100;
    TL0 =255;
    if(++pre_con1 >=7)          /* 1 sec */
    {
        pre_con=0;
        pre_con1=0;
    }
    if(++ad_con > 30)            /* 3 sec */
        clock_check=1;
    else
    {
        clock_check = 0;
    }
}

/***************************************************************************/
void Precipi_check(void)
{
                /* clock 1 min conter  | time 10min counter*/

    if(pre_10min_conter > 600)   /* time 10 min conter | pulse start 10min counter*/
    {
        pre_10min_conter=0;
        pre_1time_segi=0;
        pre_1ea_check=0;
    }

    if(Second==0)
    {
        if(precipi_1min_conter==0 && precipi_10min_conter !=0)
        {
            pre_segi_min = 18000 / pre_10min_conter;
            if(pre_segi_min < pre_1time_segi )
            {
                pre_1time_segi = pre_segi_min;
            }
        }

        precipis_1min_conter   = precipi_1min_conter;            /* precipication 1 min check */
        precipi_1min_conter = 0;


        if(Minute==0)   /* clock 10 min conter | pulse start 10min counter*/
        {
            if(precipi_1day_conter > 0)
            {
                if(precipi_10min_conter <= precipi_1day_conter)
                {
                    precipis_10min_conter = precipi_1day_conter - precipi_10min_conter;
                    precipi_10min_conter  = precipi_1day_conter;
                    precipis_1day_conter  = precipi_1day_conter;
                }
            }
            else
            {
                     precipi_10min_conter= 0;
                     precipis_10min_conter= 0;
                     precipi_1day_conter = 0;
                     precipis_1day_conter = 0;
            }
        }
        sensor_precipi_status=0;
    }
}

/***************************************************************************/
interrupt void EX1_int (void)           /* External Interrupt 1 */
{
    sensor_precipi_status=0;

    if(pre_con == 0)
    {
        pre_con=1;
        precipi_1min_conter  += 5;
  /*      precipi_10min_conter += 5; */
        precipi_1day_conter  += 5;
        
        if(pre_10min_conter !=0)
        {
            if(pre_1ea_check == 0)
            {
                pre_1ea_check = 1;
                pre_10min_conter = 0;
            }
            else
            {
                pre_1time_segi = 18000 / pre_10min_conter;
                pre_10min_conter = 0;
            }
        }
    }
}

/***************************************************************************/
/***************************************************************************/
interrupt void  SCON_int (void)          /* Serial Port */
{
    if(RI == 1)
    {
        RI=0;
        TI=0;
        P1.1=1;                /* GREEN */
        con=0;

        TH1 = BPS96;
        TL1 = BPS96;
        io  = SBUF;
        CALLOK[DSP++] = io;

        if(DSP > 295) DSP=0;

        if(CALLOK[DSP-11]==0xff && CALLOK[DSP-10]==0xff && CALLOK[DSP-2]==0xff && CALLOK[DSP-1]==0xfe && io==0xfe)
        {
            CALL[0]=CALLOK[DSP-11];            /*  0  */
            CALL[1]=CALLOK[DSP-10];            /*  1  */
            CALL[2]=CALLOK[DSP-9];             /*  2  */
            CALL[3]=CALLOK[DSP-8];             /*  3  */
            CALL[4]=CALLOK[DSP-7];             /*  4  */
            CALL[5]=CALLOK[DSP-6];             /*  5  */
            CALL[6]=CALLOK[DSP-5];             /*  6  */
            CALL[7]=CALLOK[DSP-4];             /*  7  */
            CALL[8]=CALLOK[DSP-3];             /*  8  */
            CALL[9]=CALLOK[DSP-2];             /*  9  */
            CALL[10]=CALLOK[DSP-1];            /* 10  */

            DSP=0;
            call_check();
            if(call_check_sdata == CALL[8])
            {

/******************************************************************************/
/*              Temperature  1                                                */
/******************************************************************************/
                if(CALL[3] == TEMP +ID)
                {
                    if(CALL[2] == 'M')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'M';
                        SEND_DATA[3]  = (clock_check | adc_check | sensor_temp_status);
                        SEND_DATA[4]  = TEMPCODE;
                        SEND_DATA[5]  = TEMP +ID;
                        SEND_DATA[6]  = Temp1S_data >> 8;
                        SEND_DATA[7]  = Temp1S_data;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = Temp1D_min  >> 8;
                        SEND_DATA[11] = Temp1D_min ;
                        SEND_DATA[12] = Temp1D_max  >> 8;
                        SEND_DATA[13] = Temp1D_max ;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'O')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'O';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_temp_status;
                        SEND_DATA[4]  = TEMPCODE;
                        SEND_DATA[5]  = TEMP +ID;
                        SEND_DATA[6]  = Temp1M_data >> 8;
                        SEND_DATA[7]  = Temp1M_data;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = Temp1M_smin >> 8;
                        SEND_DATA[11] = Temp1M_smin;
                        SEND_DATA[12] = Temp1M_smax >> 8;
                        SEND_DATA[13] = Temp1M_smax;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'T')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'T';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_temp_status;
                        SEND_DATA[4]  = TEMPCODE;
                        SEND_DATA[5]  = TEMP +ID;
                        SEND_DATA[6]  = Temp1T_data >> 8;
                        SEND_DATA[7]  = Temp1T_data;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = Temp1T_smin >> 8;
                        SEND_DATA[11] = Temp1T_smin;
                        SEND_DATA[12] = Temp1T_smax >> 8;
                        SEND_DATA[13] = Temp1T_smax;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];
                    }
                    else if(CALL[2] == '@')
                    {

                        Second=1;
                        Minute=1;

                        Temp1D_min  = CALL[4];
                        Temp1D_min  = ((Temp1D_min << 8) & 0xff00) | CALL[5];
                        Temp1D_max  = CALL[6];
                        Temp1D_max  = ((Temp1D_max << 8) & 0xff00) | CALL[7];

                    }
                }

/******************************************************************************/
/*              precipication                         */
/******************************************************************************/

                else if(CALL[3] == PRECIPITION +ID)
                {
                    if(CALL[2] == 'M')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'M';
                        SEND_DATA[3]  = clock_check |sensor_precipi_status;
                        SEND_DATA[4]  = PRECICODE;
                        SEND_DATA[5]  = PRECIPITION +ID;
                        SEND_DATA[6]  = precipi_1day_conter >> 8;
                        SEND_DATA[7]  = precipi_1day_conter;
                        SEND_DATA[8]  = pre_1time_segi >> 8;
                        SEND_DATA[9]  = pre_1time_segi;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = precipi_1day_conter >> 8;
                        SEND_DATA[13] = precipi_1day_conter;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'O')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'O';
                        SEND_DATA[3]  = clock_check |sensor_precipi_status;
                        SEND_DATA[4]  = PRECICODE;
                        SEND_DATA[5]  = PRECIPITION +ID;
                        SEND_DATA[6]  = precipis_1min_conter >> 8;
                        SEND_DATA[7]  = precipis_1min_conter;
                        SEND_DATA[8]  = pre_1time_segi >> 8;
                        SEND_DATA[9]  = pre_1time_segi;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = precipi_1day_conter >> 8;
                        SEND_DATA[13] = precipi_1day_conter;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'T')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'T';
                        SEND_DATA[3]  = clock_check |sensor_precipi_status;
                        SEND_DATA[4]  = PRECICODE;
                        SEND_DATA[5]  = PRECIPITION +ID;
                        SEND_DATA[6]  = precipis_10min_conter >> 8;
                        SEND_DATA[7]  = precipis_10min_conter;
                        SEND_DATA[8]  = pre_1time_segi >> 8;
                        SEND_DATA[9]  = pre_1time_segi;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = precipis_1day_conter >> 8;
                        SEND_DATA[13] = precipis_1day_conter;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == '@')
                    {

                        Second=1;
                        Minute=1;
                        precipis_1day_conter = CALL[6];
                        precipis_1day_conter = ((precipis_1day_conter << 8) & 0xff00) | CALL[7];
                        precipi_10min_conter = precipis_1day_conter;
                        precipi_1day_conter  = precipis_1day_conter;
                    }
                }

/******************************************************************************/
/*          precipi detecter                                                   */
/******************************************************************************/
                else if(CALL[3]== PRED +ID)
                {
                    if(CALL[2] == 'M')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'M';
                        SEND_DATA[3]  = clock_check |sensor_pred_status;
                        SEND_DATA[4]  = PREDCODE;
                        SEND_DATA[5]  = PRED + ID;
                        SEND_DATA[6]  = 0;
                        SEND_DATA[7]  = pred;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = 0;
                        SEND_DATA[13] = pred;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'O')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'O';
                        SEND_DATA[3]  = clock_check |sensor_pred_status;
                        SEND_DATA[4]  = PREDCODE;
                        SEND_DATA[5]  = PRED + ID;
                        SEND_DATA[6]  = 0;
                        SEND_DATA[7]  = pred;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = 0;
                        SEND_DATA[13] = pred;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'T')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'T';
                        SEND_DATA[3]  = clock_check |sensor_pred_status;
                        SEND_DATA[4]  = PREDCODE;
                        SEND_DATA[5]  = PRED + ID;
                        SEND_DATA[6]  = 0;
                        SEND_DATA[7]  = pred;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = 0;
                        SEND_DATA[13] = pred;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == '@')
                    {

                        Second=1;
                        Minute=1;
                        pred = CALL[6];
                        pred = ((pred << 8) & 0xff00) | CALL[7];
                    }
                }

/******************************************************************************/
/*          humi                                                              */
/******************************************************************************/

                else if(CALL[3] == HUMI + ID)
                {
                    if(CALL[2] == 'M')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'M';
                        SEND_DATA[3]  = clock_check | adc_check | sensor_humi_status;
                        SEND_DATA[4]  = HUMICODE;
                        SEND_DATA[5]  = HUMI +ID;
                        SEND_DATA[6]  = Humi1S_data  >> 8;
                        SEND_DATA[7]  = Humi1S_data;
                        SEND_DATA[8]  = Humi1M_data >> 8;
                        SEND_DATA[9]  = Humi1M_data;
                        SEND_DATA[10] = Humi1D_min >> 8;
                        SEND_DATA[11] = Humi1D_min;
                        SEND_DATA[12] = Humi1D_max >> 8;
                        SEND_DATA[13] = Humi1D_max;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];
                    }

                    else if(CALL[2] == 'O')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'O';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_humi_status;
                        SEND_DATA[4]  = HUMICODE;
                        SEND_DATA[5]  = HUMI +ID;
                        SEND_DATA[6]  = Humi1M_sdata >> 8;
                        SEND_DATA[7]  = Humi1M_sdata;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = Humi1M_smin >> 8;
                        SEND_DATA[11] = Humi1M_smin;
                        SEND_DATA[12] = Humi1M_smax >> 8;
                        SEND_DATA[13] = Humi1M_smax;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'T')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'T';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_humi_status;
                        SEND_DATA[4]  = HUMICODE;
                        SEND_DATA[5]  = HUMI +ID;
                        SEND_DATA[6]  = Humi1T_sdata >> 8;
                        SEND_DATA[7]  = Humi1T_sdata;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = Humi1T_smin >> 8;
                        SEND_DATA[11] = Humi1T_smin;
                        SEND_DATA[12] = Humi1T_smax >> 8;
                        SEND_DATA[13] = Humi1T_smax;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];
                    }
                    else if(CALL[2] == '@')
                    {
                        Second=1;
                        Minute=1;
                        Humi1D_min  = CALL[4];
                        Humi1D_min  = ((Humi1D_min << 8) & 0xff00) | CALL[5];
                        Humi1D_max  = CALL[6];
                        Humi1D_max  = ((Humi1D_max << 8) & 0xff00) | CALL[7];

                    }
                }

/******************************************************************************/
/*          Press                                                              */
/******************************************************************************/

                else if(CALL[3] == PRESS + ID)
                {
                    if(CALL[2] == 'M')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'M';
                        SEND_DATA[3]  = clock_check | adc_check | sensor_press_status;
                        SEND_DATA[4]  = PRESSCODE;
                        SEND_DATA[5]  = PRESS +ID;
                        SEND_DATA[6]  = PressS_data  >> 8;
                        SEND_DATA[7]  = PressS_data;
                        SEND_DATA[8]  = PressM_data >> 8;
                        SEND_DATA[9]  = PressM_data;
                        SEND_DATA[10] = PressD_min >> 8;
                        SEND_DATA[11] = PressD_min;
                        SEND_DATA[12] = PressD_max >> 8;
                        SEND_DATA[13] = PressD_max;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];
                    }

                    else if(CALL[2] == 'O')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'O';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_press_status;
                        SEND_DATA[4]  = PRESSCODE;
                        SEND_DATA[5]  = PRESS +ID;
                        SEND_DATA[6]  = PressM_sdata >> 8;
                        SEND_DATA[7]  = PressM_sdata;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = PressM_smin >> 8;
                        SEND_DATA[11] = PressM_smin;
                        SEND_DATA[12] = PressM_smax >> 8;
                        SEND_DATA[13] = PressM_smax;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'T')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'T';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_press_status;
                        SEND_DATA[4]  = PRESSCODE;
                        SEND_DATA[5]  = PRESS +ID;
                        SEND_DATA[6]  = PressT_sdata >> 8;
                        SEND_DATA[7]  = PressT_sdata;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = PressT_smin >> 8;
                        SEND_DATA[11] = PressT_smin;
                        SEND_DATA[12] = PressT_smax >> 8;
                        SEND_DATA[13] = PressT_smax;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];
                    }
                    else if(CALL[2] == '@')
                    {
                        Second=1;
                        Minute=1;
                        PressD_min  = CALL[4];
                        PressD_min  = ((PressD_min << 8) & 0xff00) | CALL[5];
                        PressD_max  = CALL[6];
                        PressD_max  = ((PressD_max << 8) & 0xff00) | CALL[7];

                    }
                }


/******************************************************************************/
/*          Sun                                                              */
/******************************************************************************/

                else if(CALL[3] == SUN + ID)
                {

                    if(CALL[2] == '@')
                    {
                        SunD_max  = CALL[6];
                        SunD_max  = ((SunD_max << 8) & 0xff00) | CALL[7];
                    }
                    else if(CALL[2] == 'M')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'M';
                        SEND_DATA[3]  = clock_check | adc_check | sensor_sun_status;
                        SEND_DATA[4]  = SUNCODE;
                        SEND_DATA[5]  = SUN +ID;
                        SEND_DATA[6]  = SunS_data1 >> 8;
                        SEND_DATA[7]  = SunS_data1;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = SunD_max >> 8;
                        SEND_DATA[13] = SunD_max;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];
                    }

                    else if(CALL[2] == 'O')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'O';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_sun_status;
                        SEND_DATA[4]  = SUNCODE;
                        SEND_DATA[5]  = SUN +ID;
                        SEND_DATA[6]  = SunM_sdata >> 8;
                        SEND_DATA[7]  = SunM_sdata;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = SunS_ddata >> 8;
                        SEND_DATA[11] = SunS_ddata;
                        SEND_DATA[12] = SunD_max >> 8;
                        SEND_DATA[13] = SunD_max;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == 'T')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'T';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_sun_status;
                        SEND_DATA[4]  = SUNCODE;
                        SEND_DATA[5]  = SUN +ID;
                        SEND_DATA[6]  = SunT_sdata >> 8;
                        SEND_DATA[7]  = SunT_sdata;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = SunD_max >> 8;
                        SEND_DATA[13] = SunD_max;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];
                    }
                    else if(CALL[2] == '@')
                    {
                        Second=1;
                        Minute=1;
                        SunD_max  = CALL[6];
                        SunD_max  = ((SunD_max << 8) & 0xff00) | CALL[7];
                    }
                }

/******************************************************************************/
/*         sunof                                                                */
/******************************************************************************/

                else if(CALL[3]== SUNOF +ID)
                {
                    if(CALL[2] == 'M')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'M';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_sunof_status;
                        SEND_DATA[4]  = SUNOFCODE;
                        SEND_DATA[5]  = SUNOF +ID;
                        SEND_DATA[6]  = 0;
                        SEND_DATA[7]  = SunofSD_data;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = SunofDD_count >> 8;
                        SEND_DATA[13] = SunofDD_count;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    if(CALL[2] == 'O')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'O';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_sunof_status;
                        SEND_DATA[4]  = SUNOFCODE;
                        SEND_DATA[5]  = SUNOF +ID;
                        SEND_DATA[6]  = SunofMD_count >> 8;
                        SEND_DATA[7]  = SunofMD_count;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = SunofDD_count >> 8;
                        SEND_DATA[13] = SunofDD_count;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    if(CALL[2] == 'T')
                    {
                        SEND_DATA[0]  = 0xff;
                        SEND_DATA[1]  = 0xff;
                        SEND_DATA[2]  =  'T';
                        SEND_DATA[3]  = clock_check | adc_check |sensor_sunof_status;
                        SEND_DATA[4]  = SUNOFCODE;
                        SEND_DATA[5]  = SUNOF +ID;
                        SEND_DATA[6]  = SunofTD_count >> 8;
                        SEND_DATA[7]  = SunofTD_count;
                        SEND_DATA[8]  = 0;
                        SEND_DATA[9]  = 0;
                        SEND_DATA[10] = 0;
                        SEND_DATA[11] = 0;
                        SEND_DATA[12] = SunofDD_count >> 8;
                        SEND_DATA[13] = SunofDD_count;
                        send_check();
                        SEND_DATA[14] = send_check_sdata;
                        SEND_DATA[15] = 0xff;
                        SEND_DATA[16] = 0xfe;
                        SBUF=SEND_DATA[0];

                    }
                    else if(CALL[2] == '@')
                    {
                        Second=1;
                        Minute=1;
                        SunofDD_count  = CALL[6];
                        SunofDD_count  = ((SunofDD_count << 8) & 0xff00) | CALL[7];
                    }

                }
                if(CALL[2] == 'I')
                {

                    Second=1;
                    Minute=1;
                    Temp1S_data = Temp1S_data;
                    Temp1M_data = Temp1S_data;
                    Temp1T_data = Temp1S_data;

                    Temp1M_smax = Temp1S_data;
                    Temp1M_smin = Temp1S_data;
                    Temp1T_smax = Temp1S_data;
                    Temp1T_smin = Temp1S_data;

                    Temp1D_max  = Temp1S_data;
                    Temp1D_min  = Temp1S_data;
                    Temp1M_max  = Temp1S_data;
                    Temp1M_min  = Temp1S_data;
                    Temp1T_max  = Temp1S_data;
                    Temp1T_min  = Temp1S_data;


                    Humi1S_data = Humi1S_data;
                    Humi1M_data = Humi1S_data;
                    Humi1T_data = Humi1S_data;

                    Humi1M_smax = Humi1S_data;
                    Humi1M_smin = Humi1S_data;
                    Humi1T_smax = Humi1S_data;
                    Humi1T_smin = Humi1S_data;

                    Humi1D_max  = Humi1S_data;
                    Humi1D_min  = Humi1S_data;
                    Humi1M_max  = Humi1S_data;
                    Humi1M_min  = Humi1S_data;
                    Humi1T_max  = Humi1S_data;
                    Humi1T_min  = Humi1S_data;

                    precipi_1sec_conter = 0;
                    precipi_1min_conter = 0;
                    precipi_10min_conter= 0;
                    precipi_1day_conter = 0;
                    precipis_1day_conter = 0;
                    precipis_1min_conter = 0;
                    precipis_10min_conter= 0;
                    pre_10min_conter=0;
                    pre_1ea_check=0;
                    pre_con,pre_con1=0;

                    SunofMD_count = 0;
                    SunofTD_count = 0;
                    SunofDD_count = 0;
                    SunofM_count = 0;
                    SunofT_count = 0;

                    SunM_data = 0;
                    SunT_data = 0;
                    SunM_sdata = 0;
                    SunT_sdata = 0;
                    SunMs_data = 0;
                    SunTs_data = 0;
                    SunD_max=0;
                    SunD_min=0;
                    SunM_max=0;
                    SunM_min=0;
                    SunT_max=0;
                    SunT_min=0;
                    SunM_smax=0;
                    SunM_smin=0;
                    SunT_smax=0;
                    SunT_smin=0;

                }
            }
        }
    }
    if (TI == 1 && RI == 0)
    {
       RI=0;
       TI=0;
       P1.0=1;                         /* P1.1 ===>red */
       con=0;

       if(++send_conter1 <17)
       {
          SBUF=SEND_DATA[send_conter1];
       }
        else  send_conter1=0;

    }
}



/***************************************************************************/
void main(void)
{

    P1.0=0;
    P1.1=0;

    initial();
    while(1)
    {
        if (++con >= 200)
        {
            con=0;
            P1.0=0;
            P1.1=0;
        }
        if(flag==1)
        {
            if(clock_conter == 1)
            {
                pre_10min_conter++;
                if(Init_count < 15) { Init_count++; ad_con=0;}
                if(++Second == 60)  { Second=0; Minute= ++Minute%10; }
                if(Init_count1 < 20)  Init_count1++;

                P1.3=0; P1.4=0; P1.5=0;         /* Sensor input 1       */
                sensor0_read();                 /* Temperature1 Measure */
                P1.3=1; P1.4=1; P1.5=1;         /* Sensor input 8       */
            }
            else if(clock_conter == 2)
            {
                P1.3=1; P1.4=1; P1.5=1;         /* Sensor input 8       */

                sensor1_read();                 /* Humi  Measure */
                P1.3=1; P1.4=0; P1.5=0;
            }
            else if(clock_conter == 3)
            {
                Precipi_check();                /* Precipicatio  */
                sensor2_read();                 /* pred Measure  */
                sensor6_read();                 /* SUNOF (pre-d)*/

            }
            else if(clock_conter >= 4)
            {
                P1.3=1; P1.4=0; P1.5=0;
                sensor4_read();                 /* SUN */
                clock_conter=0;
                P1.3=0; P1.4=0; P1.5=0;         /* Sensor input 1       */
            }

            flag=0;
            if(flag==2)
            {
                sensor3_read();                 /* press               */
                sensor5_read();                 /* SUNOF -detile       */
            }
        }
    }
}

/***************************************************************************/
/***************************************************************************/
void data_read(void)            /* precipi   */
{
    P1.2=1;
    adconter=0;
    write_XDATA(0x8000,0xff);
    while(P1.7==1)
    {
         if(adconter++ > 300)  break;
    }
    delay();
    EA=0;
    adl = read_XDATA(0x8001);
    adh = read_XDATA(0x8000);
    EA=1;
}

/***************************************************************************/
/***************************************************************************/
void sensor0_read(void)            /* Temperature 1  */
{
    unsigned int i=0,ii=0;
    long  flag=0,j=0,k=0;
    int test=0;
    delay();
    delay();

    do{
        data_read();
        if(adh == adl)
        {
            data_read();
        }
        Temp1l1=adl;
        Temp1h1=adh;

        Temp1_data  = (Temp1l1 & 0x0f);
        Temp1_data |= (Temp1h1 & 0xf0);
        Temp1_data |= (Temp1h1 & 0x0f) << 8;
        Temp1_data  = Temp1_data & 0x0fff;
        Stemp[i]    = Temp1_data;

    } while(i++ < sum);


    j=0;
    for(i=0; i != sum; i++)
        j+=Stemp[i];
    j /=sum;

    Temp1S_data= ((j * 1000) /4095);
    Temp1S_data+=500;
    if( Temp1S_data > 1499 )
    {
         sensor_temp_status = 2;
         Temp1S_data=1500;
    }

    else if(Temp1S_data < 501 )
    {
         sensor_temp_status = 2;
         Temp1S_data=500;
    }
    else
    {

        if(Temp1S_data > 1+500 && Temp1S_data< 1000+500)
        {
            sensor_temp_status = 0;
            if(Init_count1 < 5)
            {

                sensor_temp_status = 0;
                Temp1M_data = Temp1S_data;
                Temp1T_data = Temp1S_data;
                Temp1M_smax = Temp1S_data;
                Temp1M_smin = Temp1S_data;
                Temp1T_smax = Temp1S_data;
                Temp1T_smin = Temp1S_data;
                for_temp1   = Temp1M_data;

                Temp1D_max = Temp1S_data;
                Temp1D_min = Temp1S_data;
                Temp1M_max = Temp1S_data;
                Temp1M_min = Temp1S_data;
                Temp1T_max = Temp1S_data;
                Temp1T_min = Temp1S_data;
            }

            else if(Temp1S_data > Temp1M_data + 50 || Temp1S_data + 50 < Temp1M_data)
                sensor_temp_status = 2;
            else
            {

                Temp1M_count++;
                Temp1Ms_data += Temp1S_data;

                if(Temp1D_max < Temp1S_data)      Temp1D_max=Temp1S_data; /* Day Limit */
                else if(Temp1D_min > Temp1S_data) Temp1D_min=Temp1S_data;

                if(Temp1M_max < Temp1S_data)      Temp1M_max=Temp1S_data; /* Minute Limit */
                else if(Temp1M_min > Temp1S_data) Temp1M_min=Temp1S_data;
                if(Temp1T_max < Temp1S_data)      Temp1T_max=Temp1S_data; /* Ten Minute Limit */
                else if(Temp1T_min > Temp1S_data) Temp1T_min=Temp1S_data;
             }
        }
    }
    if(Second == 0)
    {
        if(Temp1M_count != 0)
        {
            Temp1T_count++;
            Temp1M_data = Temp1Ms_data/Temp1M_count;
            Temp1Ts_data += Temp1M_data;
            Temp1Ms_data=0;
            Temp1M_count=0;
        }
        else Temp1M_data=Temp1S_data;

        Temp1M_smax = Temp1M_max;               /* Limit Clear */
        Temp1M_smin = Temp1M_min;
        Temp1M_max  = Temp1S_data;               /* Limit Clear */
        Temp1M_min  = Temp1S_data;

        if(Minute == 0)
        {
            if(Temp1T_count != 0)
            {
                Temp1T_data=Temp1Ts_data/Temp1T_count;
                Temp1Ts_data=0;
                Temp1T_count=0;
            }
            else
                Temp1T_data=Temp1S_data;

            Temp1T_smax = Temp1T_max;               /* Limit Clear */
            Temp1T_smin = Temp1T_min;
            Temp1T_max  = Temp1S_data;               /* Limit Clear */
            Temp1T_min  = Temp1S_data;
        }

        if(Temp1M_data > Temp1M_smax) { Temp1M_data=Temp1S_data; Temp1M_smax=Temp1S_data+1;}
        if(Temp1M_data < Temp1M_smin) { Temp1M_data=Temp1S_data; Temp1M_smax=Temp1S_data-1;}
        if(Temp1T_data > Temp1T_smax) { Temp1T_data=Temp1S_data; Temp1T_smax=Temp1S_data+1;}
        if(Temp1T_data < Temp1T_smin) { Temp1T_data=Temp1S_data; Temp1T_smax=Temp1S_data-1;}
    }

}

/***************************************************************************/
/***************************************************************************/
void sensor2_read(void)              /* pred  */
{
    if(P1.6==1)
    {
        pred=10;
        Pred_10min_con=1;
        sensor_pred_status = 0;
    }
    else pred=0;
}

/***************************************************************************/
/***************************************************************************/
void sensor1_read(void)            /* Humierature 1  */
{
    unsigned int i=0,k=0;
    long  flag=0,j=0;
    int test=0;
    delay();
    delay();

    do{
        data_read();
        if(adh == adl)
        {
            data_read();
        }

        Humi1l=adl;
        Humi1h=adh;
        Humi1_data  = Humi1l & 0x0f;
        Humi1_data |= Humi1h & 0xf0;
        Humi1_data |= (Humi1h & 0x0f) << 8;

        SHumi[i] = Humi1_data;
    } while(i++ < sum);

    j=0;
    for(i=0; i != sum; i++)
        j+=SHumi[i];

    j /=sum;

    Humi1S_data= ((j * 1000) /4095);
        if(Humi1S_data > 999)  Humi1S_data =999;

    if(Init_count1 < 5)
    {
        Humi1D_max = Humi1S_data;
        Humi1D_min = Humi1S_data;
        Humi1M_smax = Humi1S_data;
        Humi1M_smin = Humi1S_data;
        Humi1T_smax = Humi1S_data;
        Humi1T_smin = Humi1S_data;

        Humi1M_max=Humi1S_data;
        Humi1M_min=Humi1S_data;
        Humi1T_max=Humi1S_data;
        Humi1T_min=Humi1S_data;

        for_humi = Humi1M_data;
        Humi1M_data = Humi1S_data;
        Humi1T_data = Humi1S_data;
        Humi1M_sdata = Humi1S_data;
        Humi1T_sdata = Humi1S_data;
    }

//    else if(Humi1S_data > Humi1M_sdata + 300 || Humi1S_data + 300 < Humi1M_sdata)
//        sensor_humi_status = 2;
    else if(Humi1S_data < 20)
    {
        Humi1S_data =0;
        sensor_humi_status = 2;
    }
    else
    {
        sensor_humi_status = 0;

        Humi1M_count++;
        Humi1Ms_data += Humi1S_data;

        if(Humi1D_max < Humi1S_data)       Humi1D_max=Humi1S_data;    /* Day Limit */
        else if(Humi1D_min > Humi1S_data)  Humi1D_min=Humi1S_data;

        if(Humi1M_max < Humi1S_data)       Humi1M_max=Humi1S_data;    /* Minute Limit */
        else if(Humi1M_min > Humi1S_data)  Humi1M_min=Humi1S_data;
        if(Humi1T_max < Humi1S_data)       Humi1T_max=Humi1S_data;    /* Ten Minute Limit */
        else if(Humi1T_min > Humi1S_data)  Humi1T_min=Humi1S_data;
    }

    if(Second == 0)
    {
        if(Humi1M_count != 0)
        {
            Humi1T_count++;
            Humi1M_data=Humi1Ms_data/Humi1M_count;
            Humi1M_sdata=Humi1M_data;
            Humi1Ts_data += Humi1M_data;
            Humi1Ms_data=0;
            Humi1M_count=0;
        }
        else
        {
            Humi1M_data=0;
            Humi1M_sdata=Humi1S_data;
        }
        Humi1M_smax = Humi1M_max;               /* Limit Clear */
        Humi1M_smin = Humi1M_min;
        Humi1M_max  = Humi1S_data;               /* Limit Clear */
        Humi1M_min  = Humi1S_data;

        if(Minute == 0)
        {
            if(Humi1T_count != 0)
            {
                Humi1T_data=Humi1Ts_data/Humi1T_count;
                Humi1T_sdata = Humi1T_data;
                Humi1Ts_data=0;
                Humi1T_count=0;
            }
            else
            {
                Humi1T_data=0;
                Humi1T_sdata = Humi1S_data;
            }

            Humi1T_smax = Humi1T_max;               /* Limit Clear */
            Humi1T_smin = Humi1T_min;
            Humi1T_max  = Humi1S_data;               /* Limit Clear */
            Humi1T_min  = Humi1S_data;
        }
    }
}
/***************************************************************************/
void sensor3_read(void)            /* Press  */
{
    unsigned int i=0,k=0;
    long  flag=0,j=0;
    int test=0;
    delay();
    delay();

    do{
        data_read();
        if(adh == adl)
        {
            data_read();
        }

        Pressl=adl;
        Pressh=adh;
        Press_data  = Pressl & 0x0f;
        Press_data |= Pressh & 0xf0;
        Press_data |= (Pressh & 0x0f) << 8;

        SPress[i] = Press_data;
    } while(i++ < sum);

    j=0;
    for(i=0; i != sum; i++)
        j+=SPress[i];

    j /=sum;

    press_id= 255-read_XDATA(0x9000);

    if(sensor_id < 127) PressS_data= ((j * 7326/10000)+8000) + press_id;
    else PressS_data= ((j * 7326/10000)+8000) - (press_id-127);

    if(PressS_data > 12000) PressS_data = 12000;
    if(PressS_data < 8001)  PressS_data = 8000;

    if(Init_count1 < 5)
    {
        PressD_max  = PressS_data;
        PressD_min  = PressS_data;
        PressM_smax = PressS_data;
        PressM_smin = PressS_data;
        PressT_smax = PressS_data;
        PressT_smin = PressS_data;

        PressM_max  = PressS_data;
        PressM_min  = PressS_data;
        PressT_max  = PressS_data;
        PressT_min  = PressS_data;

        PressM_data  = PressS_data;
        PressT_data  = PressS_data;
        PressM_sdata = PressS_data;
        PressT_sdata = PressS_data;
    }

    else if(PressS_data < 8001 && PressS_data > 11999)
    {
        sensor_press_status = 2;
    }
    else
    {
        sensor_press_status = 0;

        PressM_count++;
        PressMs_data += PressS_data;

        if(PressD_max < PressS_data)       PressD_max=PressS_data;    /* Day Limit */
        else if(PressD_min > PressS_data)  PressD_min=PressS_data;

        if(PressM_max < PressS_data)       PressM_max=PressS_data;    /* Minute Limit */
        else if(PressM_min > PressS_data)  PressM_min=PressS_data;
        if(PressT_max < PressS_data)       PressT_max=PressS_data;    /* Ten Minute Limit */
        else if(PressT_min > PressS_data)  PressT_min=PressS_data;
    }

    if(Second == 0)
    {
        if(PressM_count != 0)
        {
            PressT_count++;
            PressM_data=PressMs_data/PressM_count;
            PressM_sdata=PressM_data;
            PressTs_data += PressM_data;
            PressMs_data=0;
            PressM_count=0;
        }
        else
        {
            PressM_data=0;
            PressM_sdata=PressS_data;
        }
        PressM_smax = PressM_max;               /* Limit Clear */
        PressM_smin = PressM_min;
        PressM_max  = PressS_data;               /* Limit Clear */
        PressM_min  = PressS_data;

        if(Minute == 0)
        {
            if(PressT_count != 0)
            {
                PressT_data=PressTs_data/PressT_count;
                PressT_sdata = PressT_data;
                PressTs_data=0;
                PressT_count=0;
            }
            else
            {
                PressT_data=0;
                PressT_sdata = PressS_data;
            }

            PressT_smax = PressT_max;               /* Limit Clear */
            PressT_smin = PressT_min;
            PressT_max  = PressS_data;               /* Limit Clear */
            PressT_min  = PressS_data;
        }
    }
}


/***************************************************************************/
void sensor4_read(void)            /* sun  */
{
    unsigned int i=0,k=0;
    long  flag=0,j=0;
    int test=0;
    delay();
    delay();

    do{
        data_read();
        if(adh == adl)
        {
            data_read();
        }

        Sunl=adl;
        Sunh=adh;
        Sun_data  = Sunl & 0x0f;
        Sun_data |= Sunh & 0xf0;
        Sun_data |= (Sunh & 0x0f) << 8;

        SSun[i] = Sun_data;
    } while(i++ < sum);

    j=0;
    for(i=0; i != sum; i++)
        j+=SSun[i];

    j /=sum;

    SunS_data= ((j * 105374) /4095);   /* 9.49  uV/Wm~2 ==> 1W    */

                                       /* 11.86  uV/Wm~2 ==> 1W    */
                                       /* 11.86  mV/Wm~2 ==> 1000W */
                                       /* 11.86  mV/Wm~2 ==> 1000W */
                                       /*  5.93  mV/Wm~2 ==>  500W */
                                       /*  2.965 mV/Wm~2 ==>  250W */
                                       /*  0.5   mV/Wm~2 ==>   45W */
  /* 9.49  uV/Wm~2 ==> 1W    105374 */
  /* 9.49  uV/Wm~2 ==> 1W    105374 namyangju*/
  /* 10.12 uV/Wm~2 ==> 1W    98814  BEAKUNG */
  /* 11.86 uV/Wm~2 ==> 1W    84317  */
  /* 10.61 uV/Wm~2 ==> 1W    94250  HEANAM  */

    if(SunS_data > 50000)
    {
        sensor_sun_status = 2;
        SunS_data = 50000;
    }
    else
    {
        sensor_sun_status = 0;

        SunM_count++;
        SunMs_data += SunS_data;
    }
   EA=0;
   SunS_data1 =SunS_data/10;
   EA=1;
    if(Second == 0)
    {
        if(SunM_count != 0)
        {
            SunT_count++;
            SunM_data=(SunMs_data * 6 / SunM_count)/10000;
            SunM_sdata=SunM_data;
            SunTs_data += SunM_data;
            SunMs_data=0;
            SunM_count=0;
        }
        else
        {
            SunM_data=0;
            SunM_sdata=SunS_data*6/10000;
        }

        if(Minute == 0)
        {
            if(SunT_count != 0)
            {
                SunT_sdata = SunTs_data;
                SunTs_data=0;
                SunT_count=0;
            }
            else
            {
                SunTs_data=0;
                SunT_sdata =(SunS_data * 6)/1000;
            }
            EA=0;
            SunD_max += SunT_sdata;
            EA=1;
        }
    }
}

/***************************************************************************/
void sensor5_read(void)
{

    unsigned int i=0,k=0;
    long  flag=0,j=0;
    int test=0;
    delay();
    delay();

    data_read();
    if(adh == adl)
    {
        data_read();
    }

    Sunofl=adl;
    Sunofh=adh;
    Sunof_data  = Sunofl & 0x0f;
    Sunof_data |= Sunofh & 0xf0;
    Sunof_data |= (Sunofh & 0x0f) << 8;

    j =Sunof_data;

    SunofS_data= ((j * 1000) /4095);

    if(SunofS_data > 300)
    {
        SunofSD_data=10;
        SunofM_count += 10;
        SunofT_count += 10;
    }
    else
    {
        SunofSD_data=0;
    }
    if(Second == 0)
    {
        SunofMD_count = SunofM_count;

        if(SunofMD_count > 300)
        {
            SunofDD_count+= 10;
        }
        SunofM_count=0;

        if(Minute == 0)
        {
            SunofTD_count=SunofT_count;
            SunofT_count=0;
        }
    }
}

/***************************************************************************/
void sensor6_read(void)
{

    unsigned long j;

    if(P1.6 ==1)
    {
        SunofSD_data=0x0A;
        SunofM_count += 10;
        SunofT_count += 10;
    }
    else
    {
        SunofSD_data=0;
    }
    if(Second == 0)
    {
        SunofMD_count = SunofM_count;

        if(SunofMD_count > 300)
        {
            SunofDD_count+= 10;
        }
        SunofM_count=0;

        if(Minute == 0)
        {
            SunofTD_count=SunofT_count;
            SunofT_count=0;
        }
    }
}

/***************************************************************************/
void ad_read(void)
{
   unsigned long j;

    adconter=0;
    write_XDATA(0x8001,0xff);
    while(P1.7==1)
    {
         if(adconter > 500)  break;
    }
    if(adconter > 999) adc_check=4;
    else adc_check = 0;
    adconter=0;

}
/***************************************************************************/
void delay(void)
{
    int i;
    for (i=0; i <100; i++)
    {
        i++;
        i--;
    }
}
/***************************************************************************/
void delay1(void)
{
    int i;
    for (i=0; i< 50; i++)
    {
        i++;
        i--;
    }
}
/***************************************************************************/
void send_check(void)
{
    unsigned char    ii=2;
    send_check_sdata=0;
    for(ii=2; ii < 14;ii++)  send_check_sdata ^= SEND_DATA[ii];
}
/***************************************************************************/
void call_check(void)
{
    unsigned char    ii=2;
    call_check_sdata=0;
    for(ii=2; ii < 8;ii++)   call_check_sdata ^= CALL[ii];
}
