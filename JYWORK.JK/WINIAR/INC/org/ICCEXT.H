/*                     - ICCEXT.H -

	Some extra declarations for non-ANSI functions used by the C library.

	Version: 3.20 [ICLM]

*/


#ifndef _ICCEXT_INCLUDED
#define _ICCEXT_INCLUDED

#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE double exp10(double arg);

#if __TID__ & 0x8000
#pragma function=default
#endif



#endif
