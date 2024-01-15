#include <stdio.h>
#include <io51.h>

#define  ON    1
#define  OFF   0

void delay(unsigned int count)
{
   unsigned int cmp;
   for(cmp=0; cmp<count; cmp++)  {  cmp++;   cmp