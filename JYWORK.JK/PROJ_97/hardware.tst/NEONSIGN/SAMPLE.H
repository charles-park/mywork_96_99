#define     Run_Led     P3.7
#define     Stop_Led    P3.5
#define     Set_Led     P3.4
#define     Mode3_Key   P3.3
#define     Mode4_Key   P3.2
#define     Run_Key     P3.1
#define     Stop_Key    P3.0

#define     Mode1_Key   P1.7
#define     Mode2_Key   P1.6

#define     On      0
#define     Off     1

#define     Run     0
#define     Stop    1

#define     Mode1   1
#define     Mode2   2
#define     Mode3   3
#define     Mode4   4

void Mode1_Led(void);
void Mode2_Led(void);
void Mode3_Led(void);
void Mode4_Led(void);
void delay(data unsigned char Count);
void initiliaze(void);
void Key_Scan(void);
