/*			- SETJMP.H -
 
   The ANSI "setjmp" and "longjmp" function declarations.
	   
   Version: 3.20 [ICLM]
	   
*/

#ifndef _SETJMP_INCLUDED
#define _SETJMP_INCLUDED

#ifdef __JMP_BUF__

typedef char jmp_buf[__JMP_BUF__];

#else

typedef char jmp_buf[8];

#endif

#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif


#if ((__TID__ & 0x7F00) >> 8) == 10

MEMORY_ATTRIBUTE short setjmp (jmp_buf env);
MEMORY_ATTRIBUTE void longjmp (jmp_buf env, short val);

#else

MEMORY_ATTRIBUTE int setjmp (jmp_buf env);
MEMORY_ATTRIBUTE void longjmp (jmp_buf env, int val);

#endif



#if __TID__ & 0x8000
#pragma function=default
#endif

#endif


