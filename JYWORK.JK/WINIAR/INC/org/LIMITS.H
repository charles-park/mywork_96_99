/*			- LIMITS.H -

   Integral ANSI element sizes.

   Version: 3.20 [IANR]
   Revised: 14/Jan/94	ICAP	[USHRT_MAX, UINT_MAX, ULONG_MAX "unsigned"]

*/

#ifndef _LIMITS_INCLUDED

#define _LIMITS_INCLUDED


#define CHAR_BIT         8              /* Number of bits in any "char" */

#if _argt$(char)==1			/* "char" = "unsigned" "char" */
#define CHAR_MAX         255
#define CHAR_MIN         0
#else
#define CHAR_MAX         127            /* Maximum "char" value */
#define CHAR_MIN        -128            /* Mimimum "char" value */
#endif

#define SCHAR_MAX        127            /* Maximum "signed" "char" value */
#define SCHAR_MIN       -128            /* Minimum "signed" "char" value */

#define UCHAR_MAX        255            /* Maximum "unsigned" "char" value */

#define SHRT_MAX         32767          /* Maximum ("signed") "short" value */
#define SHRT_MIN        -32768          /* Minimum ("signed") "short" value */
#define USHRT_MAX        0xFFFFU        /* Maximum "unsigned" "short" value */

#if sizeof(int)==2			/* For 32-bit CPU's usually not */
#define INT_MAX          32767          /* Maximum ("signed") "int" value */
#define INT_MIN         -32768          /* Minimum ("signed") "int" value */
#define UINT_MAX         0xFFFFU        /* Maximum "unsigned" "int" value */
#else
#define INT_MAX          2147483647
#define INT_MIN         -2147483648
#define UINT_MAX         0xFFFFFFFFU
#endif

#define LONG_MAX         2147483647     /* Maximum ("signed") "long" value */
#define LONG_MIN        -2147483648     /* Minimum ("signed") "long" value */
#define ULONG_MAX        0xFFFFFFFFU    /* Maximum "unsigned" "long" value */


#endif
