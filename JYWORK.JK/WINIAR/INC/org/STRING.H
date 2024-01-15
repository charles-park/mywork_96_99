/*			 - STRING.H -

   The ANSI 'string' function declarations.
	   
   Version: 3.20 [ICLM]
		 [ICLM]    changed the NULL definition 93.01.21
*/

#ifndef _STRING_INCLUDED
#define _STRING_INCLUDED

#ifndef	NULL
#define NULL	(void*)0     /* changed from char* 93.01.21 ICLM */
#endif

#ifndef size_t
#if sizeof((char *)0 - (char *)0) == sizeof(int)
#define size_t		unsigned int
#else
#define size_t		unsigned long
#endif
#endif

#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE void *memcpy(void *s1, const void *s2, size_t n);

MEMORY_ATTRIBUTE void *memmove(void *s1, const void *s2, size_t n);

MEMORY_ATTRIBUTE void *memchr(const void *s, int c, size_t n);

MEMORY_ATTRIBUTE void *memset(void *s, int c, size_t n);

MEMORY_ATTRIBUTE int memcmp(const void *s1, const void *s2, size_t n);

MEMORY_ATTRIBUTE char *strchr(const char *s, int c);

MEMORY_ATTRIBUTE int strcmp(const char *s1, const char *s2);

MEMORY_ATTRIBUTE int strncmp(const char *s1, const char *s2, size_t n);

MEMORY_ATTRIBUTE int strcoll(const char *s1, const char *s2);

MEMORY_ATTRIBUTE size_t strlen(const char *s);

MEMORY_ATTRIBUTE size_t strcspn(const char *s1, const char *s2);

MEMORY_ATTRIBUTE size_t strspn(const char *s1, const char *s2);

MEMORY_ATTRIBUTE char *strpbrk(const char *s1, const char *s2);

MEMORY_ATTRIBUTE char *strrchr(const char *s, int c);

MEMORY_ATTRIBUTE char *strstr(const char *s1, const char *s2);

MEMORY_ATTRIBUTE char *strcat(char *s1, const char *s2);

MEMORY_ATTRIBUTE char *strncat(char *s1, const char *s2, size_t n);

MEMORY_ATTRIBUTE char *strcpy(char *s1, const char *s2);

MEMORY_ATTRIBUTE char *strncpy(char *s1, const char *s2, size_t n);

#if __TID__ & 0x8000
#pragma function=default
#endif

#endif


