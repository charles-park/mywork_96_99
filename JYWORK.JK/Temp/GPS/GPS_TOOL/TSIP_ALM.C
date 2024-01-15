/*
	
Vers	Date		Changes										Author
----	--------	----------------------------------------	----------
1.1		21 Jun 93	initial version								P Loomis

For header information, see TSIPINCL.H and TSIPCHAT.C.

TSIP_ALM.C contains two principal routines, ALMGETB and ALMPUTB.
These two routines get binary 'system data' from the receiver
(almanac, health, ionospheric parameters) and load it back into the
receiver respectively. Each subroutine issues a sequence of more than 30
commands, and control is not passed back to the main program until
the sequence is finished or aborted.  Reports are intercepted in
this routine, so they do not pass through the TSIP_RPT.C routines.
The binary data file is named GPSALM.DAT and includes health,
ionospheric, UTC, and satellite almanacs - basically reloading RAM to
do quick satellite acquistion, or "warm start", after power to the
receiver has been discontinued.

There is currently no protection (e.g., checksums) against damage to
the files.  It is possible for the receiver to "hang" if bad almanac
data is loaded into it, in which case RAM must be erased by removing
power.  Because there is a residual capacitance in some TSIP devices,
power must be shut off for a few minutes to completely erase the
memory.  In a system application using these functions, we suggest
checksums for data integrity.  

There is an extern, FILE *tsipstream, to allow binary recording of the
data while it is being stored to GPSALM.DAT */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "tsipincl.h"

static FILE	*almfile;
extern FILE *tsipstream;

static int get_version (TSIPPKT *cmd, TSIPPKT *rpt,
	byte *major_nav_version, byte *minor_nav_version)
{
	byte
		nav_day, nav_month, nav_year, 
		major_dsp_version, minor_dsp_version,
		dsp_day, dsp_month, dsp_year;
	
	rpt->code = 0x00;
	cmd_0x1F (cmd);
	send_cmd (cmd);
	xprintf("GPS software version number requested ...");
	do {
		reset_rptbuf (rpt);
		do {
			if (kbhit()) if (getch() == 0x0D) return FALSE;
			if (!accumulate_rptbuf (rpt)) continue;
		} while (!end_of_rptbuf (rpt));
		if (tsipstream != NULL) fwrite (rpt->buf, 1, rpt->cnt, tsipstream);
		if (!unstuff_rptbuf (rpt)) continue;
	}  while (rpt->code != 0x45);

	if (!rpt_0x45 (rpt,
		major_nav_version, minor_nav_version,
		&nav_day, &nav_month, &nav_year,
		&major_dsp_version, &minor_dsp_version,
		&dsp_day, &dsp_month, &dsp_year)) return FALSE;

	xprintf ("Navigation Processor version %d.%02d",
		*major_nav_version, *minor_nav_version);
	show_crlf ();
	return TRUE;
}

/**/
static int get_and_store_health (TSIPPKT *cmd, TSIPPKT *rpt)
{
	int i, len;

	rpt->code = 0x00;
	cmd_0x38q (cmd, 3, 0);
	send_cmd (cmd);
	xprintf("Almanac health page, t_oa, wn_oa requested ...");

	do {
		reset_rptbuf (rpt);
		do {
			if (kbhit()) if (getch() == 0x0D) return FALSE;
			if (!accumulate_rptbuf (rpt)) continue;
		} while (!end_of_rptbuf (rpt));
		if (tsipstream != NULL) fwrite (rpt->buf, 1, rpt->cnt, tsipstream);
		if (!unstuff_rptbuf (rpt)) continue;
	}  while (rpt->code != 0x58);

	if ((rpt->buf[0] != 2) || (rpt->buf[1] != 3) || (rpt->buf[2] != 0))  {
		xprintf("Unexpected response!");
		return FALSE;
	}
	len = rpt->buf[3];
	xprintf ("received, length = %d", len);
	show_crlf ();
	if (rpt->cnt != (len+4))  {
		xprintf("Length error!");
		return FALSE;
	}

	fputc(0x82, almfile);
	fputc(3, almfile);
	fputc(len, almfile);
	for (i=0; i<len; i++)  {
		fputc(rpt->buf[i+4], almfile);
	}
	fputc(0x83, almfile);
	return TRUE;
}

static int get_and_store_alm (TSIPPKT *cmd, TSIPPKT *rpt, byte sv)
{
	int i, len;

	rpt->code = 0x00;
	cmd_0x38q (cmd, 2, sv);
	send_cmd (cmd);
	xprintf ("SV %d almanac requested ...", sv);
	do {
		reset_rptbuf (rpt);
		do {
			if (kbhit()) if (getch() == 0x0D) return FALSE;
			if (!accumulate_rptbuf (rpt)) continue;
		} while (!end_of_rptbuf (rpt));
		if (tsipstream != NULL) fwrite (rpt->buf, 1, rpt->cnt, tsipstream);
		if (!unstuff_rptbuf (rpt)) continue;
	}  while (rpt->code != 0x58);

	if ((rpt->buf[0] != 2) || (rpt->buf[1] != 2) || (rpt->buf[2] != sv))  {
		xprintf("Unexpected response!");
		return FALSE;
	}
	len = rpt->buf[3];
	xprintf ("received, length = %d", len);
	show_crlf ();
	if (rpt->cnt != (len+4))  {
		xprintf("Length error!");
		return FALSE;
	}

	fputc(0x82, almfile);
	fputc(2, almfile);
	fputc(sv, almfile);
	fputc(len, almfile);
	for (i=0; i<len; i++)  {
		fputc(rpt->buf[i+4], almfile);
	}
	fputc(0x83, almfile);
	return TRUE;
}

static int get_and_store_iono (TSIPPKT *cmd, TSIPPKT *rpt)
{
	int i, len;

	rpt->code = 0x00;
	cmd_0x38q (cmd, 4, 0);
	send_cmd (cmd);
	xprintf("Ionospheric data requested ...");
	do {
		reset_rptbuf (rpt);
		do {
			if (kbhit()) if (getch() == 0x0D) return FALSE;
			if (!accumulate_rptbuf (rpt)) continue;
		} while (!end_of_rptbuf (rpt));
		if (tsipstream != NULL) fwrite (rpt->buf, 1, rpt->cnt, tsipstream);
		if (!unstuff_rptbuf (rpt)) continue;
	}  while (rpt->code != 0x58);

	if ((rpt->buf[0] != 2) || (rpt->buf[1] != 4) || (rpt->buf[2] != 0))  {
		xprintf("Unexpected response!");
		return FALSE;
	}
	len = rpt->buf[3];
	xprintf ("received, length = %d", len);
	show_crlf ();
	if (rpt->cnt != (len+4))  {
		xprintf("Length error!");
		return FALSE;
	}

	fputc(0x82, almfile);
	fputc(4, almfile);
	fputc(len, almfile);
	for (i=0; i<len; i++)  {
		fputc(rpt->buf[i+4], almfile);
	}
	fputc(0x83, almfile);
	return TRUE;
}

static int get_and_store_utc(TSIPPKT *cmd, TSIPPKT *rpt)
{
	int i, len;

	rpt->code = 0x00;
	cmd_0x38q (cmd, 5, 0);
	send_cmd (cmd);
	xprintf("UTC time data requested ...");
	do {
		reset_rptbuf (rpt);
		do {
			if (kbhit()) if (getch() == 0x0D) return FALSE;
			if (!accumulate_rptbuf (rpt)) continue;
		} while (!end_of_rptbuf (rpt));
		if (tsipstream != NULL) fwrite (rpt->buf, 1, rpt->cnt, tsipstream);
		if (!unstuff_rptbuf (rpt)) continue;
	}  while (rpt->code != 0x58);

	if ((rpt->buf[0] != 2) || (rpt->buf[1] != 5) || (rpt->buf[2] != 0))  {
		xprintf("Unexpected response!");
		return FALSE;
	}
	len = rpt->buf[3];
	xprintf ("received, %d bytes", len);
	show_crlf ();
	if (rpt->cnt != (len+4))  {
		xprintf("Length error!");
		return FALSE;
	}

	fputc(0x82, almfile);
	fputc(5, almfile);
	fputc(len, almfile);
	for (i=0; i<len; i++)  {
		fputc(rpt->buf[i+4], almfile);
	}
	fputc(0x83, almfile);
	return TRUE;
}

int almgetb (void)
{
	byte sv;
	byte major_ver, minor_ver;
	static TSIPPKT cmd, rpt;

	almfile = fopen("GPSALM.DAT", "wb");
	if (almfile == NULL)  {
		xprintf("Couldn't open almanac data file!");
		return FALSE;
	}

	xprintf("Almanac data file opened");
	show_crlf ();
	xprintf ("Hit <CR> to abort binary get process");
	show_crlf ();

	if (!get_version (&cmd, &rpt, &major_ver, &minor_ver)) return FALSE;

	/* tag the binary almanac file */
	fputc (major_ver, almfile);
	fputc (minor_ver, almfile);

	if (!get_and_store_health (&cmd, &rpt)) return FALSE;

	for (sv=1; sv<=32; sv++)  {
		if (!get_and_store_alm (&cmd, &rpt, sv)) return FALSE;
	}

	if (!get_and_store_iono(&cmd, &rpt)) return FALSE;

	if (!get_and_store_utc(&cmd, &rpt)) return FALSE;

	xprintf ("System data load into GPSALM.DAT ended.");

	fclose (almfile);

	return TRUE;
}

/**/
/* read from file; load into receiver; listen for affirmation */
static int read_and_load_data (TSIPPKT *cmd, TSIPPKT *rpt)
{
	int bufch, blklen, svch;
	byte inbuff[265];

	/*  read data block from file  */
	do  {
		if (kbhit()) if (getch() == 0x0D) return FALSE;
		bufch = fgetc(almfile);
		if (bufch == EOF)  return FALSE;
	}  while (bufch != 0x82);

	inbuff[0] = 2;			/*  'LOAD' operation code  */
	bufch = fgetc(almfile);
	if (bufch == EOF)  return FALSE;
	inbuff[1] = bufch;

	switch (inbuff[1]) {
	case 1:
		svch = fgetc(almfile);
		if (svch < 1 || svch > 32) return FALSE;
		inbuff[2] = svch;
		xprintf ("Almanac raw for SV %d   ", svch);
		break;

	case 2:
		svch = fgetc(almfile);
		if (svch < 1 || svch > 32) return FALSE;
		inbuff[2] = svch;
		xprintf ("Almanac data for SV %d   ", svch);
		break;

	case 3:
		xprintf("Almanac health page, t_oa, wn_oa   ");
		inbuff[2] = 0;
		break;

	case 4:
		xprintf("Ionosphere data   ");
		inbuff[2] = 0;
		break;

	case 5:
		xprintf("UTC time correction data   ");
		inbuff[2] = 0;
		break;

	default:
		xprintf("Unknown data block type!   ");
		return FALSE;
	}

	bufch = fgetc(almfile);
	if (bufch == EOF || bufch < 0 || bufch > 256)  return FALSE;
	inbuff[3] = bufch;
	for (blklen = 4; blklen<inbuff[3]+4; blklen++)  {
		bufch = fgetc(almfile);
		if (bufch == EOF)  return FALSE;
		inbuff[blklen] = bufch;
	}
	bufch = fgetc(almfile);
	if (bufch == EOF)  return FALSE;
	if (bufch != 0x83)  {
		xprintf("End of block format error!");
		return FALSE;
	}

	/*  send data block to GPS set  */
	cmd_0x38s (cmd, inbuff[1], inbuff[2], inbuff[3], &inbuff[4]);
	send_cmd (cmd);
	rpt->code = 0x00;
	do {
		reset_rptbuf (rpt);
		do {
			if (kbhit()) if (getch() == 0x0D) return FALSE;
			if (!accumulate_rptbuf (rpt)) continue;
		} while (!end_of_rptbuf (rpt));
		if (tsipstream != NULL) fwrite (rpt->buf, 1, rpt->cnt, tsipstream);
		if (!unstuff_rptbuf (rpt)) continue;
	}  while (rpt->code != 0x58);

	if ((rpt->buf[0]!=1) || (rpt->buf[1]!=inbuff[1]) ||
		(rpt->buf[2]!=inbuff[2])){
		xprintf("Unexpected response!");
		return FALSE;
	}
	xprintf("Data block acknowledged");
	show_crlf ();
	return TRUE;
}

int almputb (void)
{
	int	i, j;
	byte major_ver, minor_ver;
	static TSIPPKT cmd, rpt;

	if ((almfile = fopen("GPSALM.DAT", "rb")) == NULL)  {
		xprintf("Couldn't open almanac data file!");
		return FALSE;
	}
	xprintf("Almanac data file opened");
	show_crlf ();
	xprintf ("Hit <CR> to abort binary load process");
	show_crlf ();
	if (!get_version (&cmd, &rpt, &major_ver, &minor_ver)) return FALSE;

	i = fgetc(almfile);
	j = fgetc(almfile);
	xprintf ("GPSALM.DAT generated using firmware version %d.%02d", i, j);
	show_crlf ();
	if ((i != major_ver) || (j != minor_ver))  {
		show_crlf ();
		xprintf("WARNING!  Different firmware versions!  ALMPUTB may fail");
		show_crlf ();
		xprintf("Press any key to continue or ^C to abort TSIPCHAT");
		show_crlf ();
		while (kbhit())  getch();
		getch();
	}
	do {
		if (!read_and_load_data (&cmd, &rpt)) break;
	} while (kbhit()?(getch()==0x0D):TRUE);
	xprintf ("System data load function ended");
	fclose (almfile);
	return TRUE;
}
