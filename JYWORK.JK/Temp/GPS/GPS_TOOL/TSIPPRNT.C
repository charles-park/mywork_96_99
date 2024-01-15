/*
	
Vers	Date		Changes										Author
----	--------	----------------------------------------	----------
1.1		21 Jun 93	initial version								P Loomis
1.11	16 Feb 94	corrections to TSIP_RPT 					P Loomis
1.12  22 Aug 94   Added support for 4.06 messages        G Turetzky
See TSIPINCL.H for general header information.

The following files are used in compilation:
TSIPINCL.H:  protoypes.
TSIPPRNT.C:  main program
TSIP_RPT.C:  report interpreter
TSIP_UTL.C:  utilities
TSIP_IFC.C:  standard command/report interface for TSIP
TSIP_TNL.C:  required only if 'TNL' is defined.

The macro FILE_INPUT must be defined at compilation.  Do not define the macros
BORLAND, PORT_INPUT, or SPDRIVE.

For general header information see TSIPINCL.H.  
TSIPPRNT translates a TSIP binary data stream that has been captured.  
TSIPCHAT can be used to capture the data file using the ^F option.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tsipincl.h"

FILE *tsipstream; 		 /* file for recording report data */

/**/
/* ************************************************************** */
/* ****************  main routine for tsiptalk ****************** */
/* ************************************************************** */

int main (int argc, char *argv[])
{
	static TSIPPKT
		rpt;				/* structure for TSIP report  */

	printf ("TSIPPRNT -- TSIP Packet Translator -- Ver. 1.12r\n");
/*  printf ("TSIPPRNT -- TSIP Packet Translator -- Ver. 1.1\r\n");*/
	printf ("  (c) Copyright 1993, 1994 Trimble Navigation\r\n");

	/* initialize binary storage file if desired */
	if (argc != 2) {
		printf ("\n usage: tsipprnt infilename");
		exit (0);
	}

	tsipstream = fopen (argv[1], "rb");
	if (tsipstream == NULL) {
		printf ("\n usage: tsipprnt infilename");
		exit (0);
	}

	/* clear report buffer - initialize to rpt.cnt of zero */
	reset_rptbuf (&rpt);

	do {

		/* if FALSE, no character was read, continue looping */
		if (accumulate_rptbuf (&rpt)) {


			/* if FALSE, end-of-packet sequence not received, continue */
			if (end_of_rptbuf (&rpt)) {

				/* remove DLE stuffing, head and tail bytes, */
				/* restore in rpt->buf */
				/* if FALSE, packet interpretation failure */
				/*show_crlf();*/
				if (unstuff_rptbuf (&rpt)) {
					rpt_packet (&rpt);
				}

				/* reset rpt->cnt to zero, code to 0x00 */
				reset_rptbuf (&rpt);
			}
		}

	} while (!feof(tsipstream) && !ferror (tsipstream));
	return TRUE;
}
