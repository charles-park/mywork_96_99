/*			- MATH.H -

   The ANSI-defined (+ a few additional) mathematical functions.
	   
   Version: 3.20 [IHAW]
	   
*/

#ifndef _MATH_INCLUDED
#define _MATH_INCLUDED

#ifndef HUGE_VAL
#if sizeof(float)==sizeof(double)
#define HUGE_VAL 3.402823466e+38
#else
#define HUGE_VAL 1.7976931348623158e+308
#endif

#endif

#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE double atan (double arg);

MEMORY_ATTRIBUTE double atan2 (double arg1, double arg2);

MEMORY_ATTRIBUTE double cos (double arg);

MEMORY_ATTRIBUTE double cosh (double arg);

MEMORY_ATTRIBUTE double fabs(double arg);

MEMORY_ATTRIBUTE double fmod(double arg1, double arg2);

MEMORY_ATTRIBUTE double exp (double arg);

MEMORY_ATTRIBUTE double ldexp (double arg1, int arg2);

MEMORY_ATTRIBUTE double log (double arg);

MEMORY_ATTRIBUTE double log10 (double arg);

MEMORY_ATTRIBUTE double modf (double value, double *iptr);

MEMORY_ATTRIBUTE double pow (double arg1, double arg2);

MEMORY_ATTRIBUTE double sin (double arg);

MEMORY_ATTRIBUTE double sinh (double arg);

MEMORY_ATTRIBUTE double sqrt (double arg);

MEMORY_ATTRIBUTE double tan (double arg);

MEMORY_ATTRIBUTE double tanh (double arg);

MEMORY_ATTRIBUTE double floor (double arg);

MEMORY_ATTRIBUTE double ceil (double arg);

MEMORY_ATTRIBUTE double frexp (double arg1, int *arg2);

MEMORY_ATTRIBUTE double acos (double arg);

MEMORY_ATTRIBUTE double asin (double arg);


#if __TID__ & 0x8000
#pragma function=default
#endif

#endif





