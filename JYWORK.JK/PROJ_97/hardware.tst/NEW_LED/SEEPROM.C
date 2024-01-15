//  Program Name : Serial EEPROM Reader & Writer
//  Program : Jong-Kil,PARK
//  Date : 1996, 12, 18(Start)
//         1996, 12,   (End)

#include <io51.h>

#define     Clocks      P3.0
#define     ChipCs      P3.1
#define     DataIn      P3.2
#define     DataOut     P3.3

static      bit     Clock;          // Clocks
static      bit     Chip_Cs;        // Chip Select
static      bit     Data_In;        // Data Inport  Bit
static      bit     Data_Out;       // Data Outport Bit

static      unsigned    char    Mode;

void mode(unsigned char Run_Mode)
{
}
void sclock(void)
{
}
void main(void)
{
}

