/*				 - HEAP.C -

   Storage for "malloc".

   To change default heap size:

     1. Change MALLOC_BUFSIZE (or compile with -DMALLOC_BUFSIZE=xxxx)
     2. Compile this file with the options: -b -P -H_heap_of_memory
          also add suitable memory model or CPU options.
     3. Update library with xlib:

	  xlib
	  *def-cpu PROCESSOR_NAME
	  *rep-mod heap LIBRARY_FILE
	  *exit

    PROCESSOR_NAME and LIBRARY_FILE are the same as used during linking.

    If there is a need to have the heap-space in some dedicated area, the
    easiest way to accomplish that is to rename the data-segment (UDATA)
    to for example MYHEAP, and then insert that name in the xlink command
    file.  This renaming can be done like this:

	  xlib
	  *def-cpu PROCESSOR_NAME
	  *ren-seg heap UDATA MYHEAP
	  *exit

   8051 note: The UDATA segment is actually named X_UDATA
              assuming that HEAP.C is compiled with -DMEMORY_ATTRIBUTE=xdata -e

   Version: 4.01 [IANR]

*/

#ifndef MALLOC_BUFSIZE
#define MALLOC_BUFSIZE 2000
#endif

#ifndef MEM_ATTRIBUTE
#define MEM_ATTRIBUTE
#endif

static MEM_ATTRIBUTE char bulk_storage [MALLOC_BUFSIZE];

char *const _heap_of_memory = bulk_storage;

char *_last_heap_object = bulk_storage;

char *const _top_of_heap = bulk_storage + MALLOC_BUFSIZE - 1;

