/*			- ASSERT.H -

   assert macro facility.
   
   NOTE: _Assert should report problems to 'stderr' for hosted
   implementations according to ANSI.
   May be implemented if the target supports it.
     
   Version: 3.30 [IHAW]
	   
*/

#ifndef _ASSERT_INCLUDED
#define _ASSERT_INCLUDED


#ifdef NDEBUG

#pragma warnings=off

#define assert(ignore)	( (void) 0)

#pragma warnings=default

#else

void _Assert (const char *, const char *, int);	/* forward declaration */

#define assert(arg)   ((arg) ? (void)0 : (void) _Assert ( \
    	    		#arg,__FILE__, __LINE__ ) )

#endif	/* NDEBUG */

#endif	/* _ASSERT_INCLUDED */

