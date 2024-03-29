/*			- CTYPE.H -

   The ANSI character testing function declarations.
	   
   Note: 7-bit ASCII or 8-bit ASCII may be supported by these functions.
	 
   Version: 3.20 [IHAW]
		    
*/

#ifndef _CTYPE_INCLUDED
#define _CTYPE_INCLUDED

#define	_U	01
#define	_L	02
#define	_N	04
#define	_S	010
#define _P	020
#define _C	040
#define _X	0100
#define _B	0200

#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

#ifdef __8_BIT_ASCII_WANTED
extern const unsigned char _Large_Ctype[257];
#define _Ctype	_Large_Ctype
#else
extern const unsigned char _Small_Ctype[129];
#define _Ctype  _Small_Ctype
#endif

int isalpha(int c);
int isupper(int c);
int islower(int c);
int isdigit(int c);
int isxdigit(int c);
int isspace(int c);
int ispunct(int c);
int isalnum(int c);
int isprint(int c);
int isgraph(int c);
int iscntrl(int c);
int toupper(int c);
int tolower(int c);

#if __TID__ & 0x8000
#pragma function=default
#endif


#define	isalpha(c)	((_Ctype+1)[c]&(_U|_L))
#define	isupper(c)	((_Ctype+1)[c]&_U)
#define	islower(c)	((_Ctype+1)[c]&_L)
#define	isdigit(c)	((_Ctype+1)[c]&_N)
#define	isxdigit(c)	((_Ctype+1)[c]&(_N|_X))
#define	isspace(c)	((_Ctype+1)[c]&_S)
#define ispunct(c)	((_Ctype+1)[c]&_P)
#define isalnum(c)	((_Ctype+1)[c]&(_U|_L|_N))
#define isprint(c)	((_Ctype+1)[c]&(_P|_U|_L|_N|_B))
#define isgraph(c)	((_Ctype+1)[c]&(_P|_U|_L|_N))
#define iscntrl(c)	((_Ctype+1)[c]&_C)
#define toupper(c)	(islower((c))? (c)&0x5F: (c))
#define tolower(c)	(isupper((c))? (c)|0x20: (c))

#endif

