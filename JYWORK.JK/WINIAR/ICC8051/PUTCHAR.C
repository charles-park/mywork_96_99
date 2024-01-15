/*			- PUTCHAR.C -

  The ANSI "putchar" function delivered here is supposed to be tailored
  for the actual target hardware.  This version assumes that the serial port
  of the 8051 is initiated before any calls to "putchar" is performed.

  Version: 5.00 [IMAF]
	  
*/

#include <stdio.h>
#include <io51.h>

static void low_level_put(char c)
  {
    do { } while (!TI);
    TI = 0;
    SBUF = c;
  }

int putchar(int val)
  {
    if (val == '\n')		/* EOL (LF) should be converted to CR-LF */
      {
        low_level_put ('\r');
      }
    low_level_put (val);
    return (val);
  }

