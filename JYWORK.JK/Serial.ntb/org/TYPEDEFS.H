/*
 *      File TYPEDEFS.H
 *      Type Definitions
 *      '92.2.5
 *      '95.5.2
 *      '96.1.8
 *      Written by Inkeon Lim
 */

#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

/** Macro constants ********************************************************/

#ifndef NOERROR
	#define NOERROR 0     /* No error occured      */
	#define ERROR   (-1)  /* Generic error occured */
#endif

#ifndef NULL
	#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
		#define NULL 0
	#else
		#define NULL 0L
	#endif
#endif

#ifdef __cplusplus
	#define __CPPARGS ...
#else
	#define __CPPARGS
#endif


/** Type definitions *******************************************************/

typedef signed char shortbool;
typedef enum {false, true} bool;

#ifndef _SIZE_T
	#define _SIZE_T
	typedef unsigned size_t;
#endif

#endif
