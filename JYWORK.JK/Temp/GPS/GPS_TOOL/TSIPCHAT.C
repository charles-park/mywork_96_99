/* For general header information see TSIPINCL.H.

Vers	Date		Changes										Author
----	--------	----------------------------------------	----------
1.1		21 Jun 93	initial version								P Loomis
1.11	16 Feb 94	corrections to TSIP_ULT, TSIP_RPT   		P Loomis
1.12  22 Aug 94   Added new TSIP messages for 4.06          G Turetzky
1.13	24 Oct 94	Changed parameter declaration for ask_4_hex() from 
						byte to int, for Borland compiler support. E Jones

TSIPCHAT.C contains the main routine for TSIPCHAT. The following
files are required for compilation:

TSIPINCL.H: prototypes.
TSIPCHAT.C: main program
TSIP_UTL.C: user interface routines
TSIP_CMD.C: command generator
TSIP_RPT.C: report interpreter
TSIP_IFC.C: standard command/report interface for TSIP
TSIP_ALM.C: grabs almanacs from, and stuffs them into, receiver

The macros PORT_INPUT and SPDRIVE must be defined.  The macro BORLAND
must be defined for dual window version, otherwise the screen
scrolls.  Do not define FILE_INPUT.

Software design has a minimum of "extern" defines.  There are two:
1) the binary FILE *stream in TSIPCHAT, accessed by TSIP_ALM.C; and
2) the command abort flag, cmd_esc, in TSIP_CMD.C and TSIP_UTL.C.

The main routine is a good model for any program interfacing with a
TSIP device.  The subroutines in TSIP_IFC.C can be used in any C
program without change.

1) Binary Output Storage.  If a filename for binary output is
specified, all reports, both requested and automatically generated,
will be saved into a binary file.  A storage file can be opened at
the start of the program by supplying a file name as an argument.  A
new storage file can be opened, or an old one closed or re-opened,
during a program session with '^F'.  The TSIPPRNT program can
translate the binary file into readable format.

2) ASCII Output Storage.  A storage file can be opened, closed, or
re-opened during a program session with '^L'.  The output will be in
Trimble ASCII Interface Protocol (TAIP) Long Navigation message
format.  GPSSK.EXE can be used to map the output if the output file
has extension '.LTF' and no field separaotr is declared.  If field
separators are declared, any spreadsheet program (Lotus, Excel,
Matlab) can be used to analyze the data.

2) Serial Communications.  Trimble has supplied a serial buffer TSR
program named SPDRIVE.COM that must be invoked before using using
TSIPCHAT.  However, Trimble encourages users to write their own
serial handling routines.  The port settings (speed, number of data
bits, stop bits, parity) on the computer can be changed using '^I'.
There is an undocumented and unsupported backdoor in TSIPCHAT for
running without SPDRIVE.COM; invoke it with the command-line argument
'-notsr'.  */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "tsipincl.h"

#define TSIPFILE 1
#define TAIPFILE 0

/* files for recording report data */
/* tsipstream is required in TSIP_ALM.C */
static FILE 
	*taipstream = NULL;
FILE 
	*tsipstream = NULL;

/* file names for same */
static char 
	tsipfile[80], taipfile[80];	

/* These variables are set in do_command and used in */
/* read_rpts_wait_for_kbhit(). */
static int 
	reply [0xFF],	/* indexed by rpt.code; >0 if requestd by cmd*/
	serial_port_hold = FALSE;

/* The following routine is used in main and routines requiring user input. */
/* By using this routine, the incoming serial stream is serviced while the */
/* user is inputting data in such routines as ask_byte and ask_double in */
/* TSIP_UTL.C */

byte read_rpts_wait_for_kbhit (void)
{
	static TSIPPKT
		rpt;		/* structure for currrent incoming TSIP report  */
	int
		in_cmd_window, /* if in dual window connfiguration, current window */
		kbch_waiting;
	byte
		kbch;

	/* Save to reset screen at exit. */
	in_cmd_window = which_window();

	kbch_waiting = FALSE;

	/* When changing serial port settings, discontinue report packet */
	/* processing. */
	if (serial_port_hold) rpt.cnt = 0;

	while (!kbch_waiting || rpt.cnt != 0) {

		if (kbhit()) {
			/* record keystroke.  If important, leave immediately; */
			/* otherwise, wait for report pcaket to finish. */
			kbch = getch();
			kbch_waiting = TRUE;
			if (kbch == 0x09 || kbch == '+') {
				/* ungraceful end of recording; dump remaining binary bytes */
				/* into file even if report packet is inncomplete. */
				if (rpt.cnt != 0 && tsipstream != NULL) {
					fwrite (rpt.buf, 1, rpt.cnt, tsipstream);
					rpt.cnt = 0;
				}
				return kbch;
			}
			if (kbch == 0x1B) {
				/* graceful end of recording; do not write partial packet. */
				return kbch;
			}
			/* to prevent the command window from scrolling off the screen. */
			return kbch;
		}

		if (!serial_port_hold) {

#ifndef BORLAND
			/* to prevent command from scrolling off the screen. */
			if (in_cmd_window) continue;
#endif

			/* returns TRUE if a character is in the serial RX buffer */
			if (accumulate_rptbuf (&rpt)) {

				/* if FALSE, end-of-packet sequence not received */
				if (end_of_rptbuf (&rpt)) {

					/* end of packet received, process packet */
					/* direct binary write with stuffing if requested */
					if (tsipstream != NULL)
						fwrite (rpt.buf, 1, rpt.cnt, tsipstream);

					/* remove DLE stuffing, head and tail bytes, re-store in */
					/* rpt->buf.  Returns FALSE if packet uninterpretable. */
					if (unstuff_rptbuf (&rpt)) {
						
						/* if reply is requested or a diagnostic, print */
						/* in the command window. */
						
						if (rpt.code == 0x5F || reply[rpt.code]>0) {
							switch_to_cmd_window ();
							rpt_packet (&rpt);
							if (!in_cmd_window) switch_to_auto_window ();
							
							/* Decrement the reply request.  There may be */
							/* 32 requests, one for each SV: e.g., cmd_0x3A.*/
							reply[rpt.code]--;
						}
						else {
							switch_to_auto_window();
							rpt_packet (&rpt);
							if (in_cmd_window) switch_to_cmd_window();
							
						}

						if (taipstream != NULL) 
							taip_output (&rpt, taipstream);
						
					}
					/* reset rpt->cnt to zero, code to 0x00 */
					reset_rptbuf (&rpt);
				}
			}
		}
	}
	return kbch;
}

static void do_command (byte kbch)
{
	static TSIPPKT
		cmd;				/* structure for current outgoing TSIP command */
	int 
		irpt;

	/* zero out all previous report requests */
	for (irpt=0; irpt<0xFF; irpt++) reply[irpt] = 0;
	cmd.cnt = 0;

	close_cmd_window ();
	switch_to_cmd_window ();

	/* 0x06, 0x09, 0x0C, 0x0D do not generate TSIP commands; */
	/* service them separately. */
	switch (kbch) {

	case 0x06:
		/* '^F' - binary file storage control */
		toggle_file_storage (&tsipstream, tsipfile, TSIPFILE);
		break;

	case 0x09:
		/* '^I' - change serial port parameters */
		
		/* When changing serial port settings, discontinue report packet */
		/* processing. */
		serial_port_hold = TRUE;
		initialize_serial_port ();
		serial_port_hold = FALSE;
		break;

	case 0x0C:
		/* '^L' - ASCII long nav file storage control */
		toggle_file_storage (&taipstream, taipfile, TAIPFILE);

	case 0x0D:
		/* <CR> - just blanks out screen */
		break;
		
	default:
		/* any other keystroke - translate keystroke to command */
		/* interpret_keystroke returns FALSE if not recognized */
		if (interpret_keystroke (kbch, &cmd.code)) {
			
			/* assemble command string */
			proc_kbd (kbch, &cmd);
			
			/* send command string to receiver */
			if (cmd.code != 0x00) {
				send_cmd (&cmd);
				
				/* if a report has been requested, display message */
				if (reply_requested (cmd.code, reply)>0) {
					xprintf (" ... waiting for reply\r");
				}
			}
		}
	}
	switch_to_auto_window ();
}

/**/
/* ************************************************************** */
/* ****************  main routine for tsiptalk ****************** */
/* ************************************************************** */

int num_args;
char *arg_ptr[10];

void main (int argc, char *argv[])
{
	byte
		kbch;
	int
		iarg;

	/* provision for command line arguments starting with '-' */
	for (iarg=0, num_args=0; iarg<argc; iarg++) {
		if (argv[iarg][0] == '-') {
			arg_ptr[num_args] = argv[iarg];
			num_args++;
		}
	}

	/* clear screen and start at the bottom. */
	/* initializes two screen setup. */
	initialize_screen ();

	switch_to_cmd_window();
	show_crlf(); show_crlf();

	xprintf (
		"         TSIPCHAT -- TSIP Receiver Control Interface -- Ver. 1.13");
/*  	"         TSIPCHAT -- TSIP Receiver Control Interface -- Ver. 1.1");*/
	show_crlf();
	xprintf (
		"              (c) Copyright 1993,1994 Trimble Navigation");
	show_crlf();
	xprintf (
		"                    Type '?' for command help menu");
	show_crlf(); show_crlf();

	/* if SPDRIVE is not invoked, the program will exit from */
	/* inside the following routine. */
	initialize_serial_port ();

#ifdef TNL

	show_crlf(); show_crlf(); show_crlf();
	xprintf (
	"                     **** PROPRIETARY SOFTWARE ****              ");
	show_crlf();
	xprintf (
	"        Use requires explicit prior permission by Trimble Navigation");
	show_crlf(); show_crlf(); show_crlf();

#endif

	switch_to_auto_window();

	/* initialize binary storage file if first command line argument */
	/* is a valid file name. */
	tsipfile[0] = taipfile[0] = '\0';
	if (argc > 1 && argv[1][0] != '-') {
		strcpy (tsipfile , argv[1]);
		toggle_file_storage (&tsipstream, tsipfile, TSIPFILE);
	}

	/* MAIN LOOP */
	/* read bytes from buffered serial port input */
	/* if kbhit, do corresponding command */
	
	for (;;) {
		kbch = read_rpts_wait_for_kbhit ();
		if (kbch == 0x1B || kbch == '+') {
			/* ESC or '+' quit program */
			break;
		}
		do_command (kbch);
		kbch = 0;
	}
	
	reset_serial_port ();
	reset_screen ();
}
