/*  sieve.c */

#include "stdio.h"

/* Eratosthenes Sieve Prime Number Program in C from Byte Jan 1983. */

#define TRUE 1
#define FALSE 0
#define SIZE 8190

char flags[SIZE+1];

void main()
  {
    register int i,k;
    int prime,count,iter;

    printf("10 iterations\n");
    for (iter = 1; iter <= 10; iter++)		/* do program 10 times */
      {	
	count = 0;			/* initialize prime counter */
	for (i = 0; i <= SIZE; i++)	/* set all flags true */
	  flags[i] = TRUE;
	for (i = 0; i <= SIZE; i++)
	  {
	    if (flags[i])		/* found a prime */
	      {
		prime = i + i + 3;	/* twice index + 3 */
		for (k = i + prime; k <= SIZE; k += prime)
		  flags[k] = FALSE;	/* kill all multiples */
		count++;		/* primes found */
	      }
	  }
      }
    printf("%d primes.\n",count);		/*primes found in 10th pass */
  }
