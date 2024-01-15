/*
	
Vers	Date		Changes										Author
----	--------	----------------------------------------	----------
1.1		21 Jun 93	initial version								P Loomis
1.11	17 Feb 94	corrections as indicated by 'v1-11'			P Loomis

for header information, see TSIPINCL.H 
TSIP_UTL.C contains two types of routines:

1) user interface routines, specifically some query routines asking
for integer, double, or y/n inputs.  There is also a routine for
toggling file storage on and off.

2) Borland-compatible window control routines.  The Borland version of this
program supports two windows, a commandd/report window that is used
for display of the command list, entry of commands and command
parameters, and display of requested packets.  The auto window
scrolls through unrequested reports, which are generated according to
the I/O options specified.  The following BORLAND C++ functions are
used to create text windows.  If suitable routines can be found in
another compiler, the windowing features may be recreatable.

void window (int left, int top, int right, int bottom) 
	- create a window
void textattr (char attr) 
	- set background and foreground colors
void gotoxy (int x, int y) 
	- go to row x, col y w.r.t. window origin
int wherex (void) 
	- report row relative to window origin
int wherey (void) 
	- report col relative to window origin
void clrscr (void), clreol (void) 
	- clear window/ to end of line
void gettextinfo (struct *textinfo)
	- get text attributes

Compiling without BORLAND defined results in all commands and
reports, both auto and requested, scrolling up the screen.

3) Serial port routines.  Send, receive, and set parameters.  The
routines here are DOS-specific and depend to some degree on having
SPDRIVE.COM installed.

4) DOS-specific PC time and date routines.
*/

#define END_STRING '\0'
/* backspace, enter, printable characters except control characters */
#define GETOUT(kbch) (((kbch)!=0x08)&&((kbch)!=0x0D)&&(!isprint(kbch)||iscntrl(kbch)))
extern int cmd_esc;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#ifdef PORT_INPUT
#include <dos.h>
#endif
#ifdef BORLAND
#include <conio.h>
#endif

#include "tsipincl.h"

#ifdef BORLAND

void show_crlf (void) {xprintf ("\r\n");clreol ();}

#else

#define clreol()
void show_crlf (void) {xprintf("\n");}

#endif


/* convert time of week into day-hour-minute-second and print */
void show_time (float time_of_week)
{
	int	days, hours, minutes;
	float seconds;
	double tow;

	static char
		*dname[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	clreol();
	if (time_of_week == -1.0) {
		xprintf ("    <No time yet>   ");
		return;
	}

	if (time_of_week < 0.) time_of_week += 604800.;
	if (time_of_week >= 604800.0 || time_of_week < 0.0) {
		xprintf ("     <Bad time>     ");
		return;
	}

	tow = time_of_week;
	days = (int) (tow/86400.0);
	hours = (int) fmod(tow/3600., 24.);
	minutes =  (int) fmod(tow/60., 60.);
	seconds = fmod(tow, 60.);

	xprintf ("  %s %02d:%02d:%05.2f   ",
		dname[days], hours, minutes, seconds);
}

#ifdef FILE_INPUT

extern FILE *tsipstream;

int getb (void)
{
	int inbyte;
	inbyte = fgetc (tsipstream);
	return (inbyte & 0xFF00) ? -1 : inbyte;
}

#endif

/**/

#ifdef PORT_INPUT
/* current window = command or requested reply, or auto report */
/* TRUE if current window is command window */
int
	in_cmd_win = FALSE;

int which_window (void)
{
	return (in_cmd_win);
}

#ifdef BORLAND


/*
The following routines set up two windows: one which prints only
commands and requested reports, the other only unrequested reports.
The routines are set up for Borland compilers.  Similar functions can
be found with Microsoft compilers.  A set of generic routines is
included for non-Borland compilers further below.  */

int
	/* text attributes for different screen functions */
	text1, text2, text3,
	cmd_cursor_y = 1,
	cmd_cursor_x = 1,
	/* TRUE if a command window currently exists */
	cmd_win_exists = FALSE;
struct text_info
	t_info;

void switch_to_auto_window (void)
{
	/* auto window is the bottom of the screen.  Unrequested reports */
	/* are printed here in scrolled mode. */
	if (!in_cmd_win) return;
	in_cmd_win = FALSE;

	if (cmd_win_exists) {
		/* save position of cursor in command window for return */
		cmd_cursor_y = wherey ();
		cmd_cursor_x = wherex ();
	}

	textattr (text1);
	window (1, cmd_cursor_y + 1, 80, 24);
	/* start at bottom of auto window and scroll */
	gotoxy (80, 24 - cmd_cursor_y);
}

void close_cmd_window (void)
{
	int yy;

	/* cmd_window is the top line when it is 'closed' */
	if (!cmd_win_exists) return;

	/* blank out existing cmd window - to lower of current cursor or */
	/* cmd_cursor location recorded in last switch to auto window. */

	yy = wherey ();
	if (yy > cmd_cursor_y) cmd_cursor_y = yy;
	window (1, 1, 80, cmd_cursor_y);
	textattr (text1);
	clrscr ();

	cmd_win_exists = FALSE;
	cmd_cursor_y = 1;
	cmd_cursor_x = 1;

	in_cmd_win = FALSE;
	textattr (text1);
	window (1, 2, 80, 24);
	gotoxy (1, 23);
}

void switch_to_cmd_window (void)
{
	/*  cmd window is the top of the screen.  Commands and requested */
	/* reports are printed here in scrolled mode in blue or reverse text. */
	if (in_cmd_win) return;
	in_cmd_win = TRUE;

	if (!cmd_win_exists) cmd_win_exists = TRUE;

	textattr (text2);
	window (1, 1, 80, 20);
	gotoxy (cmd_cursor_x, cmd_cursor_y);
	clreol ();
}

void initialize_screen (void)
{

	/* get initial text attributes and save for restore before exit. */
	gettextinfo (&t_info);

	clrscr();
	/* text1 = auto, text2 = command, text 3 = command highlight  */
	text1 = LIGHTGRAY + (BLACK<<4);
	text2 = LIGHTGRAY + (BLUE<<4);
	text3 = WHITE + (BLUE<<4);

	/* start in auto window */
	textattr (text1);
	clrscr ();
	gotoxy (1, 25);
}

void reset_screen (void)
{
	window (1,1,80,25);
	/* return text to original */
	textattr (t_info.normattr);
	clrscr();
}

/**/
#else

/* non-Borland null routines produce a scrolling screen */
#define textattr(x)
void switch_to_auto_window() {
	if (in_cmd_win) xprintf ("\n");
	in_cmd_win = FALSE;
}
void close_cmd_window() {}
void switch_to_cmd_window() {
	if (!in_cmd_win) xprintf("\n");
	in_cmd_win = TRUE;
}
void initialize_screen() {}
void reset_screen () {}

#endif /* BORLAND */

/*  prompt for a byte from user  */
byte ask_byte (char *prompt)
{
	int	i, n = 0;
	char kbbuf[20], kbch;

	cmd_esc = FALSE;
	clreol ();
	xprintf (prompt);
	for (i=0; i<20; i++) {
		kbch = read_rpts_wait_for_kbhit ();
		if (kbch == 0x0D) break;
		if (kbch == 0x08) {
			/* backspace */
			xprintf ("%c", kbch);
			if (i>0) i-=2; else i = -1;
			continue;
		}
		if (GETOUT(kbch)) {
			cmd_esc = TRUE;
			return 0;
		}
		xprintf ("%c", kbch);
		kbbuf[i] = kbch;
	}
	show_crlf();
	if (i == 0) return 0;
	kbbuf[i] = '\0';
	cmd_esc = (sscanf (kbbuf, "%d", &n) < 1);
	if (n<0) {
		n = 0;
		cmd_esc = TRUE;
	}
	if (n>0xFF) n = 0xFF;
	return n;
}

/*  prompt for an integer from user  */
int ask_int (char *prompt)
{
	int	i, n = 0;
	char kbbuf[20], kbch;

	cmd_esc = FALSE;
	clreol ();
	xprintf (prompt);
	for (i=0; i<20; i++) {
		kbch = read_rpts_wait_for_kbhit ();
		if (kbch == 0x0D) break;
		if (kbch == 0x08) {
			/* backspace */
			xprintf ("%c", kbch);
			if (i>0) i-=2; else i = -1;
			continue;
		}
		if (GETOUT(kbch)) {
			cmd_esc = TRUE;
			return 0;
		}
		xprintf ("%c", kbch);
		kbbuf[i] = kbch;
	}
	show_crlf();
	if (i == 0) return 0;
	kbbuf[i] = '\0';
	cmd_esc = (sscanf (kbbuf, "%d", &n) < 1);
	return n;
}

/* prompt for a choice from among nine or fewer */
int pick_one (char *prompt, char *code_txt[], int nopts)
{
	int i, j;

	int kbch;

	cmd_esc = FALSE;
	/* cycle through a series of choices & allow user to choose one. */
	/* Routine returns numerical value corresponding to choice. */
	xprintf ("%s  (<Space> cycle, <CR> select, ^Z abort)", prompt);


	kbch = read_rpts_wait_for_kbhit ();
	clreol ();
	if (GETOUT(kbch)) {
		cmd_esc = TRUE;
		return 0;
	}

	/* direct choice by digit 0-9*/
	j = (kbch - '0');
	i = 0;
	if ((j<nopts) && (j>=0)) if (code_txt[j][0]!=END_STRING) i = j;

	xprintf ("\r%s                                        ", prompt);
	for (;;i++) {

		xprintf ("\r%s", prompt);

		if (i>=nopts) i -= nopts;

		/* skip choices with null character string */
		if (code_txt[i][0]==END_STRING) continue;

		textattr(text3);
		xprintf ("%s (%d)", code_txt[i], i);
		textattr(text2);
		clreol ();

		/* wait for <CR> or <space> or digit choice */
		for (;;) {
			kbch = read_rpts_wait_for_kbhit ();
			if (kbch == 0x0D) {
				/* <CR>; choice is made */
				show_crlf ();
				return i;
			}
			else if (kbch == 0x20) {
				/* kbch == <Space>  drops out to new choice */
				break;
			}
			else if (GETOUT(kbch)) {
				show_crlf();
				cmd_esc = TRUE;
				return 0;
			}
			else {
				j = (kbch - '0');
				if ((j<nopts) && (j>=0)) {
					if (code_txt[j][0]!=END_STRING) {
						/* direct choice by digit 0-9 */
						/* offset for ++ at top of loop */
						i = j-1;
						break;
					}
				}
			}
		}
	}
}

/*  prompt user to enter an double */
double ask_dbl (char *prompt)
{
	double d = 0.0;
	int i;
	char kbbuf[30], kbch;

	cmd_esc = FALSE;
	clreol ();
	xprintf (prompt);
	for (i=0; i<30; i++) {
		kbch = read_rpts_wait_for_kbhit ();
		if (kbch == 0x0D) break;
		if (kbch == 0x08) {
			/* backspace */
			xprintf ("%c", kbch);
			if (i>0) i-=2; else i = -1;
			continue;
		}
		if (GETOUT(kbch)) {
			cmd_esc = TRUE;
			return 0.0;
		}
		xprintf ("%c", kbch);
		kbbuf[i] = kbch;
	}
	show_crlf();
	if (i == 0) return 0;
	kbbuf[i] = '\0';
	cmd_esc = (sscanf (kbbuf, "%lf", &d) < 1);
	return d;
}

/* ask user for 4 hex codes */
void ask_4_hex (char *prompt, int *hex1, int *hex2, int *hex3, int *hex4)
{
	int i;
	char kbbuf [30], kbch;

	cmd_esc = FALSE;
	clreol ();
	xprintf (prompt);
	for (i=0; i<30; i++) {
		kbch = read_rpts_wait_for_kbhit ();
		if (kbch == 0x0D) break;
		if (kbch == 0x08) {
			/* backspace */
			xprintf ("%c", kbch);
			if (i>0) i-=2; else i = -1;
			continue;
		}
		if (GETOUT(kbch)) {
			cmd_esc = TRUE;
			return;
		}
		xprintf ("%c", kbch);
		kbbuf[i] = kbch;
	}
	show_crlf();
	kbbuf[i] = '\0';
	cmd_esc = (sscanf (kbbuf, "%2x %2x %2x %2x", hex1, hex2, hex3, hex4)!=4);
}


/* prompt user to verify choice with a 'Y' or 'y' */
int ask_verify (void)
{
	byte kbch;
	int affirm;

	cmd_esc = FALSE;
	xprintf ("  (Y for yes) ");
	kbch = read_rpts_wait_for_kbhit ();
	if (GETOUT(kbch)) cmd_esc = TRUE;
	affirm = ((kbch=='Y') || (kbch=='y'));
	xprintf (affirm?"YES":"NO");
	show_crlf();
	return affirm;
}

/* routines dealing with PC time */

/* logical indicating desire to set time */
static int set_time_request = FALSE;

double get_timezone (void)
/* return timezone offset (UTC - local) in seconds */
{
	static double
		local2utc = 24.;		/* nonsense value to start */
	/* added, v1-11 */
	static int
		waiting_for_input = FALSE;

	while ((local2utc < -14. || local2utc > 14.) && !waiting_for_input) {
		waiting_for_input = TRUE;
		show_crlf();
		xprintf (
"Before setting the clock, the time zone offset must be entered.  If the PC");
		show_crlf();
		xprintf (
"uses local time, it must be adjusted to GPS (GMT) with the proper timezone");
		show_crlf();
		xprintf (
"offset = number of time zones west of Greenwich.  Time zones east of");
		show_crlf ();
		xprintf (
"Greenwich are negative.  Subtract additional '1' for Daylight time.");
		show_crlf ();
		xprintf (
"Value must be between -14 and 14.");
		show_crlf ();
		show_crlf ();
		/* Some time zones are not integer hour offsets: Newfoundland, */
		/* Saudi Arabia, India, Singapore, parts of Australia, etc. */
		/* Use ask_dbl to accommodate them. */
		/* added, v1-11: */
		/* routine must be prevented from calling itself through ask_dbl () */
		/* time reports are suppressed while time zone is being entered. */
		waiting_for_input = TRUE;	
		local2utc = ask_dbl (
			"Enter UTC - local offset (PST=8, PDT=7, EST=5, GMT=0) :  ");
		/* added, v1-11 */
		waiting_for_input = FALSE;
		if (cmd_esc) {
			local2utc = 24.;
			return 0;
		}
		show_crlf ();
		if (set_time_request) {
		/* changed, v1-11 */
			xprintf (
"Time zone acknowledged.  To complete timeset of PC clock, type 'z' again.");
/*			xprintf ("Please re-attempt time set with 'z' command.");*/
			set_time_request = FALSE;
		}
		show_crlf ();
	}
	return local2utc*3600.;
}

void GPS_time_from_PC (double *gpstime)
{
	union REGS
		regs;
	long
		gpsday;
	int
		year, month, day,
		months,
		hour, minute, second;
	static int days_of_month [12] = {
		  0,  31,  59,  90, 120, 151,
		181, 212, 243, 273, 304, 334};

	regs.h.ah = 0x2A;	/*  get date from DOS  */
	intdos (&regs, &regs);
	year = regs.x.cx;			/*  year  */
	month = regs.h.dh;			/*  month  */
	day = regs.h.dl;			/*  day  */
	gpsday = day + days_of_month[month-1] + (year-1980)*365 - 6;
	months = 12*(year-1980) + month;
	/* leap years - add one for every fourth previous February */
	gpsday += (months-3)/48 + 1;

	regs.h.ah = 0x2C;	/*  get time from DOS  */
	intdos (&regs, &regs);
	hour = regs.h.ch;	/*  hours  */
	minute = regs.h.cl;	/*  minutes  */
	second = regs.h.dh;	/*  seconds  */
	*gpstime = second + 60.*(minute + 60.*
			(hour + 24.*gpsday)) + get_timezone();
}

void request_PC_time_set (void)
{
	set_time_request = TRUE;
}

void GPS_time_to_PC (double gpstime)
{
	union REGS
		regs;
	long
		gpsday;
	int
		day_of_year,
		fouryear,
		year, month, day;
	static int
		days_of_month [12] = {0,  31,  59,  90, 120, 151,
		181, 212, 243, 273, 304, 334};
	static char
		*name_month[12] = {"Jan", "Feb", "Mar", "Apr", "May",
		"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if (!set_time_request) return;

	gpstime -= get_timezone();

	set_time_request = FALSE;

	gpsday = (long) (gpstime/86400.);

	/* start at Jan 1 1981, gpsday 361 */
	year = 1981;
	fouryear = (int)((gpsday-361)/1461);
	year = (int) ((gpsday - 361 - fouryear*1461)/365);
	if (year == 4) year = 3;
	day_of_year = (int)(gpsday - 361 - fouryear*1461 - year*365);
	year += 1981 + fouryear*4;
	for (month=0; month<12; month++) {
		if (day_of_year < days_of_month[month]) break;
	}
	day = 1 + day_of_year - days_of_month[month-1];

	regs.h.ah = 0x2B;			/*  set date in DOS  */
	regs.x.cx = year;			/*  year  */
	regs.h.dh = month;			/*  month  */
	regs.h.dl = day;			/*  day  */

	xprintf ("Date: %d %s %d ", day, name_month[month-1], year);
	show_crlf();

	intdos (&regs, &regs);
	if (regs.h.al != 0x00) {
		xprintf ("Date set failure");
		return;
	}

	regs.h.ah = 0x2D;		/*  set time in DOS  */
	regs.h.ch = (int) (fmod (gpstime, 86400.) / 3600.);		/*  hours  */
	regs.h.cl = (int) (fmod (gpstime, 3600.) / 60.);		/*  minutes  */
	regs.h.dh = (int) (fmod (gpstime, 60.));				/*  seconds  */
	regs.h.dl = 0;											/*  csecs */
	xprintf ("Time: %02.2d:%02.2d:%02.2d.%02.2d",
		regs.h.ch, regs.h.cl, regs.h.dh, regs.h.dl);

	intdos (&regs, &regs);
	if (regs.h.al != 0x00) {
		show_crlf();
		xprintf ("Time set failure");
		return;
	}

	intdos (&regs, &regs);
}

/* This routine translates records into TAIP LN (long nav) output. */
/* Each line contains complete fix information. Static are required to */
/* save parameters as reports roll in. If it cannot be initialized it */
/* returns FALSE. */
static char sepindex;
static int inittaip;
/* changed, v1-11 */
#define M2FT (1.0/.3048)
#define MPS2MPH (3600.*M2FT/5280.)
/*#define M2FT (.3048)*/
/*#define MPS2MPH (3600./(M2FT*5280.))*/
void taip_output (TSIPPKT *rpt, FILE *taipstream)
{
	TSIPPKT
		cmd;
	char
		taippkt[250];
	byte
		sv_prn, sv_health, sv_iode, fit_interval_flag,
		manual_mode, svalid,
		pcode, vcode, tcode, acode,
		isv, iprn, nchar;
	static byte
		running = FALSE,
		pvalid = FALSE,
		mvalid = FALSE, 
		dvalid = FALSE,
		diff_mode, ndim, svs[12], nsvs;
	static short
		inittaip = TRUE,
		was_in_cmd_win,
		rawtaip,
		iode[32] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	float
		vel_ENU[3], freq_offset,
		vtime, tday,
		time_of_coll,
		time_of_eph,
		time_since_coll, sv_accy,
		pdop, hdop, vdop, tdop;
	static float
		ptime, toc[32];
	double
		lat, lon, clock_bias,
		hspeed, vspeed, heading;
	static double
		latdeg, londeg, alt;
	static char
		sepchar,
		sepcharlist[4] = {'.', ' ', ',', '\t'};;


	cmd_esc = FALSE;

	rawtaip = sepindex == 0;
	sepchar = sepcharlist[sepindex];

	if (inittaip) {
		if (!rawtaip) {
			fprintf (taipstream,
				"seconds  %clat (deg)  %clon (deg)   %calt (ft) ",
				sepchar, sepchar, sepchar);
			fprintf (taipstream,	"%choriz%cheadng%cvert %cSV",
				sepchar, sepchar, sepchar, sepchar);
			fprintf (taipstream,
				"%cSV%cIO%cSV%cIO%cSV%cIO%cSV%cIO%cSV%cIO%cSV%cIO%cS\n",
				sepchar, sepchar, sepchar, sepchar,	sepchar, sepchar, sepchar,
				sepchar, sepchar, sepchar, sepchar, sepchar, sepchar);
		}
		inittaip = FALSE;
		/* set I/O options */
		cmd_0x35s (&cmd, 0x1E, 0x02, 0x00, 0x00);
		send_cmd (&cmd);
		/* get current nav state */
		cmd_0x24 (&cmd);
		send_cmd (&cmd);
		/* get ephemerides */
		cmd_0x3B (&cmd, 0);
		send_cmd (&cmd);
	}
	switch (rpt->code) {
	case 0x44:
		/* mode and SVs */
		mvalid = rpt_0x44 (rpt, &manual_mode, &nsvs, svs,
			&pdop, &hdop, &vdop, &tdop);
		/* changed, v1-11 */
		ndim = nsvs;
		if (ndim < 3) mvalid = FALSE;
/*		ndim = nsvs-1;*/
/*		if (ndim < 2) mvalid = FALSE;*/
		break;
	case 0x55:
		if (!rpt_0x55 (rpt, &pcode, &vcode, &tcode, &acode)) break;
		if ((pcode != 0x1E) || (vcode != 0x02)) {
			running = FALSE;
			cmd_0x35s (&cmd, 0x1E, 0x02, 0x00, 0x00);
			send_cmd (&cmd);
		}
		running = TRUE;
		break;
	case 0x56:
		/* ENU vel */
		if (!rpt_0x56 (rpt, vel_ENU, &freq_offset, &vtime)) break;
		if (!mvalid) {
			cmd_0x24 (&cmd);
			send_cmd (&cmd);
		}
		svalid = TRUE;
		for (isv=0; isv < nsvs && svalid; isv++) {
			iprn = svs[isv] - 1;
			if (iode[iprn] < 0 || iode[iprn] > 0xFF) {
				svalid = FALSE;
			}
			time_since_coll = vtime - toc[iprn];
			if (time_since_coll < -10800.) time_since_coll += 604800.;
			if (time_since_coll > 10800.) {
				svalid = FALSE;
			}
		}
		if (!svalid) {
			cmd_0x3B (&cmd, iprn+1);
			send_cmd (&cmd);
			break;
		}
		if (!pvalid || nsvs < 3) break;
		pvalid = FALSE;

		if (fabs(ptime-vtime) > .25) break;

		/* print out contents of TAIP stream */

		tday = fmod (vtime, 86400.);
		hspeed = sqrt(vel_ENU[0]*vel_ENU[0] + vel_ENU[1]*vel_ENU[1])*MPS2MPH;
		heading = (hspeed==0.0)?0.0:(atan2(vel_ENU[0], vel_ENU[1])*R2D);
		if (heading < 0.) heading += 360.;
		vspeed = vel_ENU[2]*MPS2MPH;
		
		if (rawtaip) {
			nchar = sprintf (taippkt,
				">RLN%08ld%+010ld%+011ld%+09ld%04d%+05d%04d%02d",
				(long)(tday*1000.),
				(long)(latdeg*1.e7), (long)(londeg*1.e7), (long)(alt*100.),
				(int)(hspeed*10.), (int)(vspeed*10.), (int)(heading*10.),
				nsvs);
			for (isv=0; isv<nsvs; isv++) {
				iprn = svs[isv]-1;
				nchar += sprintf (taippkt+nchar,"%02d%02.2X",
					iprn+1, iode[iprn]);
			}
			nchar += sprintf (taippkt+nchar, "0000000000%1d2<\n",
				dvalid?diff_mode:(ndim-3));
		}
		else {
			nchar = sprintf (taippkt,
				"%09.3f%c%+011.7f%c%+012.7f%c%+010.2f",
				tday, sepchar, latdeg, sepchar, londeg, sepchar, alt);
			nchar += sprintf (taippkt+nchar,
				"%c%05.1f%c%+06.1f%c%05.1f%c%02d",
				sepchar, hspeed, sepchar, vspeed, sepchar, heading,
				sepchar, nsvs);
			for (isv=0; isv<nsvs; isv++) {
				iprn = svs[isv]-1;
				nchar += sprintf (taippkt+nchar,"%c%02d%c%02.2X",
					sepchar, iprn+1, sepchar, iode[iprn]);
			}
			for (; isv<6; isv++) {
				nchar += sprintf (taippkt+nchar,"%c  %c  ",
					sepchar, sepchar);
			}
			nchar += sprintf (taippkt+nchar, "%c%1d\n",
				/* changed, v1-11 */
				sepchar, ndim - 3 + 2*(diff_mode&1));
/*				sepchar, dvalid?diff_mode:(ndim-3));*/
		}
		was_in_cmd_win = which_window();
		switch_to_auto_window ();
		show_crlf();
		xprintf (rawtaip ? "Output TAIP LN Packet" :
			"Output Modified TAIP LN Packet");
		if (was_in_cmd_win) switch_to_cmd_window();
		fprintf (taipstream, taippkt);
		break;
	case 0x5B:
		if (rpt_0x5B (rpt, &sv_prn, &sv_health, &sv_iode,
			&fit_interval_flag, &time_of_coll,
			&time_of_eph, &sv_accy)) {
			iprn = sv_prn-1;
			iode[iprn] = sv_iode;
			toc[iprn] = time_of_coll;
		}
		else {
			/* request all ephemeris to check */
			/* set all iode's to invalid */
			for (iprn=0; iprn<32; iprn++) iode[iprn] = -1;
			cmd_0x3B (&cmd, 0);
			send_cmd (&cmd);
		}
		break;
	case 0x6D:
		mvalid = rpt_0x6D (rpt, &manual_mode, &nsvs, &ndim, svs,
			&pdop, &hdop, &vdop, &tdop);
		/* changed, v1-11 */
		if (ndim < 3) mvalid = FALSE;
/*		if (ndim < 2) mvalid = FALSE;*/
		break;
	case 0x82:
		dvalid = rpt_0x82 (rpt, &diff_mode);
		break;
	case 0x84:
		/* double prec LLA */
		pvalid = rpt_0x84 (rpt, &lat, &lon, &alt, &clock_bias, &ptime);
		if (!pvalid) break;
		latdeg = lat*R2D;
		londeg = lon*R2D;
		alt *= M2FT;
		break;
	}
}


/* Following three routines called by toggle_file_storage () below.
Return TRUE or FALSE. */

static int check_file_exists (char *newname)
{
	FILE *stream_ptr;

	stream_ptr = fopen (newname, "r");
	if (stream_ptr != NULL) {
		fclose (stream_ptr);
		return TRUE;
	}
	else return FALSE;
}

static int ask_append (char *newname)
{
	xprintf ("Append to existing file '%s'?", newname);
	return ask_verify();
}

static int ask_overwrite (char *newname)
{
	xprintf ("Overwrite existing file '%s'?", newname);
	return ask_verify();
}

void toggle_file_storage (FILE **stream_ptr, char *filename, short tsip)
{
	int
		i,
		append_mode,
		need_filename;
	char
		newname[80], kbch;
	static char *septext[4] = {"none ", "space", "comma", "tab  "};

	cmd_esc = FALSE;
	if (*stream_ptr == NULL) {
		append_mode = FALSE;
		strcpy (newname, filename);
		need_filename = (newname[0] == END_STRING);

		if (need_filename) {
			xprintf ("No previous storage file.");
			show_crlf();
		}
		else if (check_file_exists (newname)) {
			append_mode = ask_append (newname);
			if (cmd_esc) return;

			/* neither overwrite nor append, try again */
			if (!append_mode) {
				need_filename = !ask_overwrite (newname);
				if (cmd_esc) return;
			}
		}

		/* open file */
		do {
			while (need_filename) {

				newname[0] = END_STRING;
				xprintf ("Enter new output file name ('^Z' to abort):  ");
				clreol ();
				for (i= 0; i<70; i++) {
					kbch = read_rpts_wait_for_kbhit ();
					if (GETOUT(kbch)) return;
					xprintf ("%c", kbch);
					if (kbch == 0x0D) break;
					if (kbch == 0x08) {
						/* backspace */
						i-=2;
						continue;
					}
					newname[i] = kbch;
				}
				show_crlf();
				newname[i] = '\0';

				/* open to read; check to see if file exists already */
				need_filename = FALSE;
				if (check_file_exists (newname)) {
					append_mode = ask_append (newname);
					if (cmd_esc) return;
					/* neither overwrite nor append, try again */
					if (!append_mode) {
						need_filename = !ask_overwrite (newname);
						if (cmd_esc) return;
					}
				}
			}

			/* choose separator for new TAIP file */
			/* sepindex, inittaip are static global */
			if (!append_mode && !tsip) {
				sepindex = pick_one ("Enter field separator:  ", septext, 4);
				if (cmd_esc) return;
				inittaip = TRUE;
			}
			/* open file for binary write or append */
			*stream_ptr = fopen (newname,
				append_mode?(tsip?"ab":"a"):(tsip?"wb":"w"));
			if (*stream_ptr == NULL) {
				/* file open failed; loop and ask for new file name. */
				xprintf ("Could not open '%s'.", newname);
				show_crlf();
				need_filename = TRUE;
			}
		} while (*stream_ptr == NULL);

		strcpy (filename, newname);
		xprintf ("Saving to '%s'.", filename);
	}
	else {
		xprintf ("Currently storing data to '%s'.  Close file?", filename);
		if (ask_verify()) {
			if (fclose (*stream_ptr) == EOF) {
				xprintf ("File close error, '%s'", filename);
			}
			else {
				*stream_ptr = NULL;
				xprintf ("Closing '%s'.", filename);
			}
		}
	}
	show_crlf();
}

int serial_port_select (int *io_code)
{
	static char
		*databit_text [4] =  {"", "", "7", "8"},
		*parity_text [4] = {"no", "odd", "", "even"},
		*baud_text [8] = {"", "150 bps", "300 bps", "600 bps",
		"1200 bps",	"2400 bps", "4800 bps", "9600 bps"},
		*stopbit_text[2] = {"1", "2"};
	byte baud, databits, stopbits, parity;

	cmd_esc = FALSE;
	show_crlf ();
	xprintf ("Current serial I\O setting = %s baud, %s - %s - %s",
		baud_text [(*io_code>>5)&7],	databit_text [*io_code&3],
		parity_text [(*io_code>>3)&3], stopbit_text [(*io_code>>2)&1]);
	show_crlf();

	baud = pick_one ("Baud rate:  ", baud_text, 8);
	if (cmd_esc) return FALSE;

	databits = pick_one ("Data bits:  ", databit_text, 4);
	if (cmd_esc) return FALSE;

	parity = pick_one ("Parity:  ", parity_text, 4);
	if (cmd_esc) return FALSE;

	stopbits = pick_one ("Stop bits:  ", stopbit_text, 2);
	if (cmd_esc) return FALSE;

	*io_code = databits + (stopbits<<2) + (parity<<3) + (baud<<5);

	xprintf ("Set port to %s baud, %s - %s - %s?",
		baud_text [(*io_code>>5)&7],	databit_text [*io_code&3],
		parity_text [(*io_code>>3)&3], stopbit_text [(*io_code>>2)&1]);

	return ask_verify();
}
#ifdef SPDRIVE

/* serial port routines related to SPDRIVE */
int getb (void)
{
	union REGS
		regs;
	/*  get byte, if available  */
	regs.h.ah = 6;
	int86 (0x14, &regs, &regs);
	if (regs.h.ah == 1) return regs.h.al;

	/* default : no byte waiting */
	return -1;
}

void sendb (byte db) 		/*  send a byte, if able  */
{
	union REGS
		regs;


	regs.h.ah = 7;		/*  send byte  */
	regs.h.al = db;
	int86 (0x14, &regs, &regs);
}

int io_code;

void initialize_serial_port ()
{
	union REGS
		regs;
	static int
		initsp = TRUE;

	cmd_esc = FALSE;
	regs.h.ah = 5;
	regs.h.al = 0;
	regs.x.dx = 0;
	int86 (0x14, &regs, &regs);
	/* if SPDRIVE is not invoked, exit program */
	if (regs.h.ah != 1) {
		show_crlf ();
		reset_screen ();
		/* deleted v1.11 */
/*		xprintf (*/
/*		"           TSIPCHAT -- GPS Sensor Packet Data Monitor -- Ver. %s",*/
/*			TSIPCHAT_VERSION);*/
		show_crlf();
		xprintf ("         Aborted -  SPDRIVE not installed!\07");
		exit (0);
	}

	io_code = regs.h.al;
	/* return original serial port setting so that it may be reset */
	/* before exiting program */

	/*  initialize SPDRIVE  */
	if (initsp) {
		/* 9600 baud 8-odd-1 is 0xEB */
		io_code = 0xEB;
		initsp = FALSE;
	}
	else {
		if (!serial_port_select (&io_code)) {
			show_crlf();
			xprintf (" -- aborted -- ");
			return;
		}
	}

	regs.h.al = io_code;
	regs.h.ah = 4;
	regs.h.dl = 1;
	int86 (0x14, &regs, &regs);

	/*  enable receive flow control  */
	regs.h.ah = 9;
	regs.h.dh = 0;
	/* 0 = flow control */
	regs.h.dl = 1;  	
	int86 (0x14, &regs, &regs);

	/*  send end-of-packet to clear out serial  */
	sendb (DLE);
	sendb (ETX);
	/*  send Low Level reset sequence  */
	sendb ('~');
	sendb ('R');
}

void reset_serial_port ()
{
	union REGS
		regs;

	/* reset to original io_code */
	if (io_code != 0xEB) {
		show_crlf();
		xprintf ("Reset serial port to 9600 baud, 8 - odd - 1?");
		if (ask_verify()) io_code = 0xEB;
	}

	regs.h.al = io_code;
	regs.h.ah = 4;
	regs.h.dl = 1;
	int86 (0x14, &regs, &regs);

	/*  disable flow control  */
	regs.h.ah = 9;
	regs.h.dh = 0;
	regs.h.dl = 0;
	int86 (0x14, &regs, &regs);
}
#endif 	/* def SPDRIVE */
#endif	/* def PORT_INPUT */


