/*

   Small recursive program that calculates Fibonacci numbers.
  
   This program was originally featured in the "Embedded Systems Magazine"
   April 1989

*/
   

#include <stdio.h>
unsigned fib (int n)
  {
    if (n <= 2) return 1;
    return fib(n-1) + fib(n-2);
  }

unsigned results[11];

main()
  {
    int loop;
    for (loop=1; loop < 10; loop++)
      {
        results[loop] = fib(loop);
        printf("fib(%d) = %d\n",loop,results[loop]);
      }
  }
