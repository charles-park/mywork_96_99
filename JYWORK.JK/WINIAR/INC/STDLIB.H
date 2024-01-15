/*			- STDLIB.H -

   The ANSI-defined 'standard' functions.

   Version: 3.20 [ICLM]

*/

#ifndef _STDLIB_INCLUDED
#define _STDLIB_INCLUDED

#ifndef size_t
#if sizeof((char *)0 - (char *)0) == sizeof(int)
#define size_t		unsigned int
#else
#define size_t		unsigned long
#endif
#endif

#ifndef NULL
#define	NULL	(void*) 0 
#endif

typedef	struct
	{
	  int	quot;
	  int	rem;
	} div_t;

typedef	struct
	{
	  long int	quot;
	  long int	rem;
	} ldiv_t;


#define RAND_MAX	32768

#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1


#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE void	*malloc(size_t size);

MEMORY_ATTRIBUTE void	free(void *ptr);

MEMORY_ATTRIBUTE void	exit(int status);

MEMORY_ATTRIBUTE void	*calloc(unsigned int nelem, size_t elsize);

MEMORY_ATTRIBUTE void	*realloc(void *ptr, size_t size);

MEMORY_ATTRIBUTE int atoi (const char *nptr);

MEMORY_ATTRIBUTE long atol (const char *nptr);

MEMORY_ATTRIBUTE double atof (const char *nptr);

MEMORY_ATTRIBUTE double strtod(const char *nptr, char **endptr);

MEMORY_ATTRIBUTE long int strtol(const char *nptr, char **endptr, int base);

MEMORY_ATTRIBUTE unsigned long int strtoul(const char *nptr, char **endptr, int base);

MEMORY_ATTRIBUTE int rand(void);

MEMORY_ATTRIBUTE void srand(unsigned int seed);

MEMORY_ATTRIBUTE void abort(void);

MEMORY_ATTRIBUTE int abs(int j);

MEMORY_ATTRIBUTE div_t div(int numer, int denom);

MEMORY_ATTRIBUTE long int labs(long int j);

MEMORY_ATTRIBUTE ldiv_t ldiv(long int numer, long int denom);

#if __TID__ & 0x8000
#pragma function=default
#endif

#endif

