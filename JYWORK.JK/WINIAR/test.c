#include <io51.h>
#include <stdio.h>

#define  watch()     P1.7  = 1 - P1.7

void delay(unsigned int count)
{
   unsigned int i;
   for(i=0; i<count ;i++)   { i++;  i--;  }
}

void main(void)
{
   while(1) {
      watch();
      P1 = 0xff;
      delay(10000);
      P1 = 0x00;
   }
}