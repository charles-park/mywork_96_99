/*			- STDARG.H -
   The ANSI macros for reading variable argument lists (...).

   Version: 3.20 [ICLM]
		 [ICLM] 93.02.08   correction for Z80
		 [ICLM] 93.11.01   correction for 8096
		 [IERA] 94.01.16   correction for NEC 78000 and 78300
		 [IHAT] 94.04.18   new version of Z80 V4
		 [IPEH] 94.08.04   cleanup
*/
#ifndef _STDARG_INCLUDED

/* *********************************** */
/* *** ICCZ80 version 1.00 - 3.xx      */
/* *********************************** */
#if ((__TID__ >> 8) & 0x7f) == 4                

typedef struct
  {
    char	*parm;
    char	*addr;
    char	argc;
  } va_list[1];

#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE char *_get_varg (va_list ap, char type, char size);

#if __TID__ & 0x8000
#pragma function=default
#endif

# define va_start(ap,parmN) ((ap[0].addr = ((char *) &parmN) + sizeof(parmN)),\
					(ap[0].parm = _args$))
# define va_arg(ap,mode) *(mode *) _get_varg (ap, _argt$(mode), sizeof(mode))

# define va_end(ap) 	/* -g option warns here - OK */
#define _STDARG_INCLUDED
#endif
/* *********************************** */


/* *********************************** */
/* *** ICC8051 version 1.00 - 3.10     */
/* *** Single chip memory model        */
/* ***                                 */
/* *** ICC7700 version 1.00 -          */
/* *********************************** */
#if ((__TID__ >> 8) == 1 && (__TID__ & 15) == 0) ||             \
    ((__TID__ >> 8) & 0x7f) == 19  

typedef void *va_list[1];

/* Stack grows in positiv direction */
# define va_start(ap,parmN) (ap[0] = &parmN)
# define va_arg(ap,mode) (*--(*(mode **)ap))

# define va_end(ap) 	/* -g option warns here - OK */
#define _STDARG_INCLUDED
#endif
/* *********************************** */


/* *********************************** */
/* *** ICC78000 version 1.xx           */
/* *** ICC78300 version 1.xx           */
/* *** ICC78300 version 2.00 -         */
/* *** ICC78000 version 2.00 -         */
/* *** ICCZ80   version 4.00 -         */
/* *********************************** */
#if ((__TID__ >> 8) & 0x7f) == 15 || 				\
    ((__TID__ >> 8) & 0x7f) == 16 || 				\
    ((__TID__ >> 8) & 0x7f) == 20 || 				\
    ((__TID__ >> 8) & 0x7f) == 21 || /* IERA 94.01.16 */  	\
    ((__TID__ >> 8) & 0x7f) == 23    /* IHAT 94.04.18 */  	
/* These targets has char pushed as word on stack */
/* Stack grows in negativ direction */

typedef void *va_list[1];

#define va_start(ap,parmN) \
  ap[0] = ((char *) &parmN) + (sizeof(parmN) > 1 ? sizeof(parmN) : 2)
#define va_arg(ap,mode) \
  (sizeof(mode) == 1 ? (*(mode *)(*(int **)ap)++) : \
                       *(*(mode **) ap)++) 

# define va_end(ap) 	/* -g option warns here - OK */
#define _STDARG_INCLUDED
#endif
/* *********************************** */


/* *********************************** */
/* *** ICC8096 version 1.00 -          */
/* *********************************** */
#if ((__TID__ >> 8) & 0x7f) == 6   /* ICLM 93.11.01 */
/* 8096 has char packed as low byte in word on the stack. */

typedef void *va_list[1];

# define va_start(ap,parmN) ap[0] = ((char *) &parmN) + (sizeof(parmN) > 1 ? sizeof(parmN) : 2)
# define va_arg(ap,mode) (*(*(mode **) ap)++)

# define va_end(ap) 	/* -g option warns here - OK */
#define _STDARG_INCLUDED
#endif
/* *********************************** */


/* *********************************** */
/* *** All ICC compilers except those  */
/* *** listed above.                   */
/* *********************************** */
#ifndef _STDARG_INCLUDED

typedef void *va_list[1];

# define va_start(ap,parmN) ap[0] = ((char *) &parmN) + sizeof(parmN)
# define va_arg(ap,mode) (*(*(mode **) ap)++)

# define va_end(ap) 	/* -g option warns here - OK */
#define _STDARG_INCLUDED
#endif
/* *********************************** */


#endif
