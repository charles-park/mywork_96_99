/*			- STDIO.H -

   Subset of ANSI standard I/O function declarations.

   Version: 3.20 [ICLM]
                 [IPEH] 94-08-03  Prototype parameter names added

*/

#ifndef _STDIO_INCLUDED
#define _STDIO_INCLUDED

#ifndef NULL
#define	NULL	(void *) 0
#endif

#ifndef EOF
#define	EOF	(-1)
#endif

#ifndef size_t
#if	sizeof((char*)0 - (char*)0) == sizeof(int)
#define	size_t	unsigned int
#else
#define	size_t 	unsigned long 
#endif
#endif

/* ===

#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

=== */

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE int 	puts(const char *s);
MEMORY_ATTRIBUTE int	putchar(int value);
MEMORY_ATTRIBUTE int	getchar(void);
MEMORY_ATTRIBUTE int	sprintf(char *s,const char *format,...);
MEMORY_ATTRIBUTE int	printf(const char *format,...);
MEMORY_ATTRIBUTE int	scanf(const char *format,...);
MEMORY_ATTRIBUTE int	sscanf(const char *s, const char *format,...);
MEMORY_ATTRIBUTE char	*gets(char *s);

#if __TID__ & 0x8000
#pragma function=default
#endif

#endif



