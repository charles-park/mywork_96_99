/*			- ICCLBUTL.H -

   Low-level declarations for non-ANSI functions
   used by the C library.

   Version: 3.20 [ICLM]
                 [IPEH] 94-08-03  Prototype parameter names added

*/

#ifndef _ICCLBUTL_INCLUDED
#define _ICCLBUTL_INCLUDED

#include "stdarg.h"

		/*-----------------------------------------*/
		/* Formatters used by "scanf" and "sscanf" */
		/*-----------------------------------------*/

#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE int _formatted_read (const char **line,  /* Full ANSI */
                            const char **format,
		            va_list ap);

MEMORY_ATTRIBUTE int _medium_read (const char **line, /* Without floating point */
                         const char **format,
			 va_list ap);


		/*-------------------------------------------*/
		/* Formatters used by "printf" and "sprintf" */
		/*-------------------------------------------*/

MEMORY_ATTRIBUTE int _formatted_write (const char *format,    /* Full ANSI */
			     void outputf(char, void *),
			     void *sp,
			     va_list ap);

MEMORY_ATTRIBUTE int _medium_write (const char *format,	
						  /* Without floating point */
			  void outputf(char, void *),
			  void *sp,
			  va_list ap);

MEMORY_ATTRIBUTE int _small_write (const char *format,
						    /* Very reduced version */
			 void outputf(char, void *),
			 void *sp,
			 va_list ap);

#if __TID__ & 0x8000
#pragma function=default
#endif

#endif
