/* 
	
Vers	Date		Changes										Author
----	--------	----------------------------------------	----------
1.1		21 Jun 93	initial version								P Loomis
1.2      8  Aug 94   changed int/byte for MS Compiler       G Turetzky
							 in I/O options

For header information, see TSIPINCL.H 

TSIP_CMD.C consists of three subroutines called by DO_COMMAND in MAIN:
INTERPRET_KEYSTROKE(), PROC_KBD(), and REPLY_REQUESTED().

INTERPRET_KEYSTOKE ():
An incoming keystroke is tested to see if it corresponds to a valid
command.  If the keystroke = :
	'?', a command help screen is shown; 
	unrecognizable, an error message is printed;
	recognizable, the keystroke is translated into a TSIP command code.  

PROC_KBD ():
A large CASE statement directs processing to the mid-level subroutine
specific to that command code.  The mid-level routine queries the
user for command parameters and passes them to the appropriate
command string generator cmd_0x?? in TSIP_IFC.C.  The suffix
convention for the subroutines is:

	cmd_0x??q for query, 
	cmd_0x??s for set, 
	cmd_0x??c for clear or set to default, 
	cmd_0x??e for enable,
	cmd_0x??d for disable.

Because one command code may correspond to one or more keystrokes,
the keystroke character is sometimes passed to the mid-level
subroutine as an option identifier.  The command string is passed
out of PROC_KBD() to MAIN(), where it will be sent to the receiver.

Two exceptions to the single-command model are two subroutines called
by PROC_KBD(), ALMGETB() and ALMPUTB().  Each of these subroutines begins a
series of operations that get/load a full set of binary data
from/into the receiver.  When either of these subroutines is invoked,
many commands and reports are generated before exiting back to the
main routine.

REPLY_REQUESTED():
A vector, reply [], is generated which enumerates report codes expected
to be returned as a result of the command.  The vector reply[] is
indexed by report code.
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "tsipincl.h"

/* Generate Control-letter character */
#define cntrl(x) ((x)-'A'+0x01)

int 
	cmd_esc;	/* abort command flag; set in TSIP_UTL.C also. */  

/* utility to check on sv_prn values */
int prn_valid (byte sv_prn)
{
	if (sv_prn > 0 && sv_prn < 33) return TRUE;

	xprintf ("SV number must be between 1 and 32 inclusive");
	show_crlf ();
	return FALSE;
}

/**/
/* 0x1D */
static void set_or_clear_osc_offset (TSIPPKT *cmd, byte kbch)
{
	float 
		offset;

	switch (kbch) {
	case 'F':
		xprintf ("Input oscillator offset value?");
		if (!ask_verify()) {
			cmd_esc = TRUE;
			return;
		}
		offset = ask_dbl ("Set oscillator offset (Hz) to:");
		if (cmd_esc) return;
		xprintf ("Reset offset to %g Hz?", offset);
		if (!ask_verify ()) {
			cmd_esc = TRUE;
			return;
		}

		cmd_0x1Ds (cmd, offset);
		return;
	case 'f':
		xprintf ("Clear oscillator offset?");
		if (!ask_verify()) {
			cmd_esc = TRUE;
			return;
		}

		cmd_0x1Dc (cmd);
		return;
	}
}

/* 0x1E */
static void clear_RAM (TSIPPKT *cmd)
{
	xprintf ("Do you really wish to clear battery backed-up RAM?");
	if (!ask_verify ()) {
		cmd_esc = TRUE;
		return;
	}

	cmd_0x1E (cmd);
	return;
}

/* 0x1F */
static void request_SW_version (TSIPPKT *cmd)
{

	cmd_0x1F (cmd);
	return;
}

/**/
/* 0x20 */
static void request_almanac_data_page (TSIPPKT *cmd)
{
	byte 
		sv_prn;

	sv_prn = ask_byte ("Enter SV number: ");
	if (!prn_valid(sv_prn)) cmd_esc = TRUE;
	if (cmd_esc) return;

	cmd_0x20 (cmd, sv_prn);
	return;
}

/* 0x21 */
static void request_time (TSIPPKT *cmd, byte kbch)
{
	if (kbch == 'z') {
		/* sets a flag in TSIP_UTL that watches for time report */
		request_PC_time_set ();
	}
	cmd_0x21 (cmd);
	return;
}

/* 0x22 */
static void set_nav_mode (TSIPPKT *cmd, byte kbch)
{
	byte 
		nav_mode;

	switch (kbch) {
	case cntrl('N'):
		/* auto 2-D/3-D */
		nav_mode = 0;
		break;
	case cntrl('T'):
		/* 1 SV timing */
		nav_mode = 1;
		break;
	case'n':
		/* manual 2-D */
		nav_mode = 3;
		break;
	case 'N':
		/* manual 3-D */
		nav_mode = 4;
		break;
	}

	cmd_0x22 (cmd, nav_mode);
	return;
}

/* 0x23 */
static void set_initial_ECEF_position (TSIPPKT *cmd)
{
	float 
		pos_ECEF[3];

	pos_ECEF[0] = ask_dbl ("Enter ECEF x: ");
	if (cmd_esc) return;
	pos_ECEF[1] = ask_dbl ("Enter ECEF y: ");
	if (cmd_esc) return;
	pos_ECEF[2] = ask_dbl ("Enter ECEF z: ");
	if (cmd_esc) return;

	xprintf ("Set new position to %g, %g, %g m ECEF?",
		pos_ECEF[0], pos_ECEF[1], pos_ECEF[2]);
	if (!ask_verify ()) {
		cmd_esc = TRUE;
		return;
	}

	cmd_0x23 (cmd, pos_ECEF);
	return;
}

/* 0x24 */
static void request_nav_mode (TSIPPKT *cmd)
{

	cmd_0x24 (cmd);
	return;
}

/* 0x25 */
static void reset_RX (TSIPPKT *cmd)
{
	xprintf ("Resetting the GPS receiver");

	cmd_0x25 (cmd);
	return;
}

/* 0x26 */
static void request_rx_health (TSIPPKT *cmd)
{

	cmd_0x26 (cmd);
	return;
}

/* 0x27 */
static void request_SNRs (TSIPPKT *cmd)
{

	cmd_0x27 (cmd);
	return;
}

/* 0x28 */
static void request_GPS_system_message (TSIPPKT *cmd)
{

	cmd_0x28 (cmd);
	return;
}

/* 0x29 */
static void request_almanac_health_page (TSIPPKT *cmd)
{

	cmd_0x29 (cmd);
	return;
}

/* 0x2A */
static void set_altitude (TSIPPKT *cmd)
{
	float 
		alt;

	alt = ask_dbl ("Enter reference altitude:  ");
	if (cmd_esc) return;
	xprintf ("Set new 2-D altitude to %g m?", alt);
	if (!ask_verify ()) {
		cmd_esc = TRUE;
		return;
	}

	cmd_0x2A (cmd, alt);
	return;
}

/* 0x2B */
static void set_initial_LLA_position (TSIPPKT *cmd)
{
	float 
		lat, lon, alt;

	lat = ask_dbl ("Enter lat (ø): ");
	if (cmd_esc) return;
	lon = ask_dbl ("Enter lon (ø): ");
	if (cmd_esc) return;
	alt = ask_dbl ("Enter alt (m): ");
	if (cmd_esc) return;

	xprintf("Set new position to %gølat, %gølon, %gm alt?",
		lat, lon, alt);
	if (!ask_verify ()) {
		cmd_esc = TRUE;
		return;
	}
	lat *= D2R;
	lon *= D2R;

	cmd_0x2B (cmd, lat, lon, alt);
	return;
}

/* 0x2C */
static void set_or_request_op_parms (TSIPPKT *cmd, byte kbch)
{
	byte 
		dyn_code;
	float 
		elev_mask, snr, pdop_mask, pdop_switch;
	static char
		*dyn_text [5] = {"leave unchanged", "land",
		"sea", "air (default)", "static"};

	switch (kbch) {
	case 'p':
		cmd_0x2Cq (cmd);
		return;
	case 'P':
		dyn_code = pick_one ("Enter dynamics code:   ", dyn_text, 5);
		if (cmd_esc) return;
		elev_mask = ask_dbl ("Enter elev angle mask :");
		if (cmd_esc) return;
		snr = ask_dbl ("Enter SNR mask:        ");
		if (cmd_esc) return;
		pdop_mask = ask_dbl ("Enter PDOP mask:       ");
		if (cmd_esc) return;
		pdop_switch = ask_dbl ("Enter PDOP switch:   ");
		if (cmd_esc) return;
		
		xprintf ("Set new masks to %d, %g, %g, %g, %g?",
			dyn_code, elev_mask, snr, pdop_mask, pdop_switch);
		if (!ask_verify ()) {
			cmd_esc = TRUE;
			return;
		}
		/* Convert elevation mask to radians. */
		elev_mask *= D2R;
		cmd_0x2Cs (cmd, dyn_code, elev_mask, snr, pdop_mask, pdop_switch);
		return;
	}

}

/* 0x2D */
static void request_osc_offset (TSIPPKT *cmd)
{

	cmd_0x2D (cmd);
	return;
}

/* 0x2E */
static void set_time (TSIPPKT *cmd)
{
	int
		week_num;
	double
		gpstime;
	float
		time_of_week;

	/*  set time from PC  */
	GPS_time_from_PC (&gpstime);
	week_num = (int) (gpstime/604800.);
	time_of_week = (gpstime - 604800.*week_num);
	xprintf ("Week number = %3d", week_num);
	show_time (time_of_week);

	show_crlf();
	xprintf ("Set receiver time?");
	if (!ask_verify()) {
		cmd_esc = TRUE;
		return;
	}

	cmd_0x2E (cmd, time_of_week, week_num);
	return;
}


/* 0x2F */
static void request_UTC_data (TSIPPKT *cmd)
{

	cmd_0x2F (cmd);
	return;
}

/**/
/* 0x31 */
static void set_accurate_ECEF_position (TSIPPKT *cmd)
{
	float 
		pos_ECEF[3];

	xputs ("CAUTION:  Position should be accurate to 50 km!");
	show_crlf ();
	pos_ECEF[0] = ask_dbl ("Enter ECEF x: ");
	if (cmd_esc) return;
	pos_ECEF[1] = ask_dbl ("Enter ECEF y: ");
	if (cmd_esc) return;
	pos_ECEF[2] = ask_dbl ("Enter ECEF z: ");
	if (cmd_esc) return;

	xprintf ("Set new position to %g, %g, %g m?",
		pos_ECEF[0], pos_ECEF[1], pos_ECEF[2]);
	if (!ask_verify ()) {
		cmd_esc = TRUE;
		return;
	}

	cmd_0x31 (cmd, pos_ECEF);
	return;
}

/* 0x32 */
static void set_accurate_LLA_position (TSIPPKT *cmd)
{
	float 
		lat, lon, alt;

	xputs ("CAUTION:  Position should be accurate to 50 km!");
	show_crlf ();

	lat = ask_dbl ("Enter lat (ø): ");
	if (cmd_esc) return;
	lon = ask_dbl ("Enter lon (ø): ");
	if (cmd_esc) return;
	alt = ask_dbl ("Enter alt (ø): ");
	if (cmd_esc) return;
	
	xprintf("Set new position to %gølat, %gølon, %gm alt?",
		lat, lon, alt);
	if (!ask_verify ()) {
		cmd_esc = TRUE;
		return;
	}

	/* Convert lat and lon to radians */
	lat *= D2R;
	lon *= D2R;

	cmd_0x32 (cmd, lat, lon, alt);
	return;
}

/* 0x34 */
static void set_SV_for_1SV_mode (TSIPPKT *cmd)
{
	byte 
		sv_prn;
	
	sv_prn = ask_byte ("Enter SV PRN number for 1-SV mode (0 for highest):  ");
	if (sv_prn != 0) if (!prn_valid (sv_prn)) cmd_esc = TRUE;
	if (cmd_esc) return;

	cmd_0x34 (cmd, sv_prn);
	return;
}

/* 0x35 */
static void set_or_request_IO_options (TSIPPKT *cmd, byte kbch)
{
	int 
		pos_code, vel_code, time_code, aux_code;
	static char
		*toggle_text[2] = {"off", "on"},
		*alt_text[2] = {"WGS-84", "MSL   "},
		*prec_text[2] = {"single", "double"},
		*time_text[2] = {"GPS", "UTC"},
		*latent_text[2] = {"ASAP               ", "next integer second"},
		*query_text[2] = {"when computed  ", "only on request"},
		*raw_text[2] = {"raw (unfiltered)", "Doppler-smoothed"};

	byte 
		bit0, bit1, bit2, bit3, bit4;

	switch (kbch) {

	case 'O':
		cmd_0x35q (cmd);
		return;

	case cntrl('O'):
		
		xputs ("Enter full menu instead of hex codes?");
		if (!ask_verify ()) {
			if (cmd_esc) return;
			ask_4_hex ("Default is 1 1 0 0 :  ",
				&pos_code, &vel_code, &time_code, &aux_code);
			show_crlf();
			if (cmd_esc) return;
		}
		else {

			/* position code */
			bit0 = pick_one ("ECEF position output: ", toggle_text, 2);
			if (cmd_esc) return;

			bit1 = pick_one ("LLA position output: ", toggle_text, 2);
			if (cmd_esc) return;
			
			if (bit1 == 1) {
				bit2 = pick_one ("altitude output: ", alt_text, 2);
				if (cmd_esc) return;
			}
			else bit2 = 0;
			
			bit3 = pick_one ("altitude input: ", alt_text, 2);
			if (cmd_esc) return;
			
			bit4 = pick_one ("output precision: ", prec_text, 2);
			if (cmd_esc) return;
			
			/* Superpackets not supported */
			pos_code = bit0 + 2*bit1 + 4*bit2 + 8*bit3 + 16*bit4;

			/* velocity code */
			bit0  = pick_one ("ECEF velocity output: ", toggle_text, 2);
			if (cmd_esc) return;

			bit1  = pick_one ("ENU velocity output: ", toggle_text, 2);
			if (cmd_esc) return;
			
			vel_code = bit0 + 2*bit1;
			
			/* time code */
			bit0 = pick_one ("Time tags in: ", time_text, 2);
			if (cmd_esc) return;
			
			bit1 = pick_one ("Fix computation time: ", latent_text, 2);
			if (cmd_esc) return;
			
			bit2 = pick_one ("Output time: ", query_text, 2);
			if (cmd_esc) return;
			
			bit3 = pick_one ("Synchronized measmts: ", toggle_text, 2);
			if (cmd_esc) return;
			
			bit4 = pick_one ("Min measmt propagate: ", toggle_text, 2);
			if (cmd_esc) return;
			
			time_code = bit0 + 2*bit1 + 4*bit2 + 8*bit3 + 16*bit4;
			
			/* auxiliary code */
			bit0 = pick_one ("Pseudorange output: ", toggle_text, 2);
			if (cmd_esc) return;
			
			if (bit0 == 1) {
				bit1  = pick_one ("Pseudorange quality: ", raw_text, 2);
				if (cmd_esc) return;
			}
			else bit1 = 0;
			
			bit2 = pick_one ("Additional fix status: ", toggle_text, 2);
			if (cmd_esc) return;
			
			aux_code = bit0 + 2*bit1 + 4*bit2;

		}

		xprintf ("Set new I/O options to %02Xh %02Xh %02Xh %02Xh?",
			pos_code, vel_code, time_code, aux_code);

		if (!ask_verify ()) {
			cmd_esc = TRUE;
			return;
		}

		cmd_0x35s (cmd, pos_code, vel_code, time_code, aux_code);
		return;
	}

}

/* 0x36 */
static void initial_velocity (TSIPPKT *cmd)
{
	byte
		frame, enable;
	static char
		*frame_text[2] = {"XYZ", "ENU"};
	float
		vel [3];

	frame = pick_one ("Enter coordinate frame:  ", frame_text, 2);
	if (cmd_esc) return;

	vel[0] = ask_dbl ("Enter first vel coord:  ");
	if (cmd_esc) return;
	vel[1] = ask_dbl ("Enter second vel coord: ");
	if (cmd_esc) return;
	vel[2] = ask_dbl ("Enter third vel coord:  ");
	if (cmd_esc) return;
	
	xprintf ("Send %s velocity as %g, %g, %g m/s?",
		frame?"ENU":"XYZ", vel[0], vel[1], vel[2]);
	if (!ask_verify ()) {
		cmd_esc = TRUE;
		return;
	}
	enable = 1;

	cmd_0x36 (cmd, frame, enable, vel);
	return;
}

/* 0x37 */
static void request_last_pos_info (TSIPPKT *cmd)
{

	cmd_0x37 (cmd);
	return;
}

/* 0x38 */
static void set_or_request_SV_system_data (TSIPPKT *cmd, byte kbch)
{
	byte 
		get_or_load,
		type_of_data,
		sv_prn;
	static char
		*get_text[3] = {"", "request data", "load data"},
		*type_text[7] = {"", "", "almanac", "health page, t_oa, wn_oa",
		"ionosphere", "UTC", "ephemeris"};

	switch (kbch) {

	case '$':

		get_or_load = pick_one ("Get or load SV System data?  ", get_text, 3);
		if (cmd_esc) return;
		
		type_of_data = pick_one ("Type of data:", type_text, 7);
		if (cmd_esc) return;

		if ((type_of_data==2) || (type_of_data==6)) {
			sv_prn = ask_byte ("Enter SV number: ");
			if (!prn_valid(sv_prn)) cmd_esc = TRUE;
			if (cmd_esc) return;
		}
		else sv_prn = 0;

		switch (get_or_load) {
		case 1:
			cmd_0x38q (cmd, type_of_data, sv_prn);
			return;
		case 2:
			/* At this point, a data buffer must be supplied from somewhere */
			/* with the information to be loaded.  Because misloading of */
			/* data can cause the receiver to act unpleasantly, the load */
			/* feature is not implemented here. */
			/* cmd_0x38s (cmd, type_of_data, sv_prn, nbytes, databuf); */
			xprintf ("Not supported by this program -- aborted ");
			return;
		}
	case '!':
		/* The following routine has its own cmdbuf.  It gives a series of */
		/* commands before returning control back to the main routine. */
		/* It creates a file GPSALM.DAT that contains all information */
		/* required to do a warm start. */

		xprintf ("Enter GPSALM.DAT fetch sequence?");
		if (ask_verify ()) {
			if (!almgetb ()) cmd_esc = TRUE;
			cmd->code = 0x00;
		}
		return;

	case '@':
		/* The following routine has its own cmdbuf.  It gives a series of */
		/* commands before returning control back to the main routine. */
		/* It loads a file GPSALM.DAT that contains all information */
		/* required to do a warm start. */

		xprintf ("Enter GPSALM.DAT load sequence?");
		if (ask_verify ()) {
			if (!almputb ()) cmd_esc = TRUE;
			cmd->code = 0x00;
		}
		return;
	}
}

/* 0x39 */
static void request_disable_or_health_override_list (TSIPPKT *cmd)
{
	byte 
		op_code, sv_prn;
	static char
		*op_text[7] = {"", "enable SV", "disable SV", 
		"list enabled/disabled", "heed SV health", 
		"ignore SV health", "list heed/ignore health"};

	op_code = pick_one ("Enter operation code:  ", op_text, 7);
	if (cmd_esc) return;

	sv_prn = 0;
	if ((op_code != 3) && (op_code != 6)) {
		sv_prn = ask_byte ("Enter SV PRN number or 0 for all:  ");
		if (cmd_esc) return;
		if (sv_prn == 0) {
			xprintf ("Do %s for all SVs?", op_text[op_code]);
			if (!ask_verify ()) cmd_esc = TRUE;
		}
		else if (!prn_valid(sv_prn)) {
			cmd_esc = TRUE;
		}
		else {
			xprintf ("Do %s for for SV%2.2d?", op_text[op_code], sv_prn);
			if (!ask_verify ()) cmd_esc = TRUE;
		}
		if (cmd_esc) return;
		xprintf ("No reply will be sent.");
	}

	cmd_0x39 (cmd, op_code, sv_prn);
	return;
}

/* 0x3A */
static void request_raw_msmt_data (TSIPPKT *cmd)
{
	byte 
		sv_prn;

	sv_prn = ask_byte ("Enter SV PRN number or 0 for all:  ");
	if (sv_prn != 0) if (!prn_valid (sv_prn)) cmd_esc = TRUE;
	if (cmd_esc) return;

	cmd_0x3A (cmd, sv_prn);
	return;
}

/* 0x3B */
static void request_ephemeris_status (TSIPPKT *cmd)
{
	byte 
		sv_prn;

	sv_prn = ask_byte ("Enter SV PRN number or 0 for all:  ");
	if (sv_prn != 0) if (!prn_valid (sv_prn)) cmd_esc = TRUE;
	if (cmd_esc) return;

	cmd_0x3B (cmd, sv_prn);
	return;
}

/* 0x3C */
static void request_track_info (TSIPPKT *cmd)
{
	byte 
		sv_prn;
	
	sv_prn = ask_byte ("Enter SV PRN number or 0 for all:  ");
	if (sv_prn != 0) if (!prn_valid (sv_prn)) cmd_esc = TRUE;
	if (cmd_esc) return;

	cmd_0x3C (cmd, sv_prn);
	return;
}

/* 0x3D  and 8E 03*/
#define MAX_BAUD 10
static void set_or_request_chan_config (TSIPPKT *cmd, byte kbch)
{
	int 
		i, j;

	byte 
		baud_out,  /* XMT baud rate */
		baud_inp,       /* RCV baud rate */
		char_code,      /* parity and #bits per byte */
		stopbitcode,            /* number of stop bits code */
		output_mode,    /* Ch. A transmission mode */
		input_mode;     /* Ch. A reception mode */

	char
		prompt[40];

	static byte
		baudnum [MAX_BAUD] = { 0, 1, 4, 5, 6, 8, 9, 11, 12, 28 };

	static char

		*bits_per_bytetext [4] =  {"", "", "7", "8"},

		*paritytext [5] = {"even", "odd", "", "", "no"},

		*baud_text [MAX_BAUD] = {
		"50 bps", "110 bps", "300 bps", "600 bps",
		"1200 bps", "2400 bps", "4800 bps", "9600 bps",
		"38.4 Kbps", "19.2 Kbps"},

		*stop_bit_text[3] = {"", "1", "2"},

		*tx_mode_text [6] = {"Packets", "No output",
		"Flow-thru from SuperTANS", "", "", "NMEA 0183"},

		*rx_mode_text [2] = {"Packets", "Raw RTCM data"};

	switch (kbch) {
	case cntrl('U'):
	case 'C':

		i = pick_one ("Enter XMT Baud Rate:     ", baud_text, MAX_BAUD);
		if (cmd_esc) return;
		baud_out = baudnum[i];

		i = pick_one ("Enter RCV Baud Rate:     ", baud_text, MAX_BAUD);
		if (cmd_esc) return;
		baud_inp = baudnum[i];

		i = pick_one ("Enter bits/char:  ", bits_per_bytetext, 4);
		if (cmd_esc) return;
		sprintf (prompt, "Enter parity:     %s-", bits_per_bytetext[i]);

		j = pick_one (prompt, paritytext, 5);
		if (cmd_esc) return;
		char_code = i + (j<<2);

		sprintf (prompt, "Enter stop bits:  %s-%s-",
			bits_per_bytetext[i], paritytext[j]);

		i = pick_one (prompt, stop_bit_text, 3);
		if (cmd_esc) return;
		if (i == 1)      /* 0x07 */
			stopbitcode = 0x07;
		else if (i == 2)        /* 2 stop bits code */
			stopbitcode = 0x0F;


		if (kbch == cntrl('U'))	{
			i = pick_one ("Enter XMIT language: ", tx_mode_text, 6);
			if (cmd_esc) return;
			output_mode = i;

			i = pick_one ("Enter RECV language: ", rx_mode_text, 2);
			if (cmd_esc) return;
			input_mode = i;

			xputs ("CAUTION! Improper use of this command may hang a single-port receiver!");
			show_crlf();
			xputs ("Change settings on port A? ");
			}
		else	/* Port B does not have language control */
			xputs ("Change settings on port B? ");

		if (!ask_verify ()) {
			cmd_esc = TRUE;
			return;
		}
		if (kbch == cntrl('U')) 
			cmd_0x3Ds (cmd, baud_out, baud_inp, char_code,
				stopbitcode, output_mode, input_mode);
		else cmd_0x8E03s (cmd, baud_out, baud_inp, char_code,
				stopbitcode);
		return;

	case 'c':
		cmd_0x8E03q (cmd);
		return;

	case 'U':
		cmd_0x3Dq (cmd);
		return;
	}			 
}

/* 0x3E */
static void request_aux_fix_parms (TSIPPKT *cmd)
{
	
	cmd_0x3E (cmd);
	return;
}

/**/
/* 0x62 */
static void set_or_request_DGPS_mode (TSIPPKT *cmd)
{
	byte 
		dgps_mode;
	static char
		*dgps_mode_text[5] = {"Manual GPS (DGPS off)",
		"Manual GPD (DGPS Req'd)", "Auto DGPS/GPS",
		"Auto DGPS/GPS", "Query receiver for current mode"};

	dgps_mode = pick_one ("Enter DGPS mode:  ", dgps_mode_text, 5);
	/* dgps_mode other than 0, 1, 2, or 3 results in query */
	/* this subroutine sets dgps_mode to 4 for query */
	if (cmd_esc) return;

	if (dgps_mode < 4) {
		xprintf ("Set DGPS mode to %s?", dgps_mode_text[dgps_mode]);
		if (!ask_verify ()) {
			cmd_esc = TRUE;
			return;
		}
		cmd_0x62s (cmd, dgps_mode);
	}
	else {
		cmd_0x62q (cmd);
	}
	return;
}

/* 0x65 */
static void request_DGPS_corrections (TSIPPKT *cmd)
{
	byte 
		sv_prn;

	sv_prn = ask_byte ("Enter SV PRN number or 0 for all:  ");
	if (sv_prn != 0) if (!prn_valid (sv_prn)) cmd_esc = TRUE;
	if (cmd_esc) return;

	cmd_0x65 (cmd, sv_prn);
	return;
}

/**/
#ifndef POSFILT
/* 0x71 */
static void set_or_request_pos_filter_parms (TSIPPKT *cmd, byte kbch)
{
	byte 
		pf, chr1, chr2, chr3, chr4, chr5;
	float 
		flt1, flt2, flt3, flt4, flt5;
	char 
		*able_text[2] = {"DISABLED", "ENABLED"};

	switch (kbch) {
	case 'L':               /*  Set low pass filter options  */
		pf = pick_one ("Position filter: ", able_text, 2);
		if (cmd_esc) return;
		xprintf ("Use default values?");
		if (ask_verify ()) {
			if (cmd_esc) return;
			flt1 = .0625;
			chr1 = 1;
			chr2 = 1;
			chr3 = 0;
			chr4 = 6;
			flt2 = .1;
			flt3 = 100.;
			flt4 = 1.21;
			flt5 = 1000000.;
			chr5 = 2;
		}
		else {
			if (cmd_esc) return;
			flt1 = ask_dbl ("flt1: ");
			if (cmd_esc) return;
			chr1 = ask_byte ("chr1: ");
			if (cmd_esc) return;
			chr2 = ask_byte ("chr2: ");
			if (cmd_esc) return;
			chr3 = ask_byte ("chr3: ");
			if (cmd_esc) return;
			chr4 = ask_byte ("chr4: ");
			if (cmd_esc) return;
			flt2 = ask_dbl ("flt2: ");
			if (cmd_esc) return;
			flt3 = ask_dbl ("flt3: ");
			if (cmd_esc) return;
			flt4 = ask_dbl ("flt4: ");
			if (cmd_esc) return;
			flt5 = ask_dbl ("flt5: ");
			if (cmd_esc) return;
			chr5 = ask_byte ("chr5: ");
			if (cmd_esc) return;
		} 

		xputs ("Reset filter settings to: ");
		xprintf("%s", pf?"ENABLED":"DISABLED");
		show_crlf ();
		xprintf("flt1:%.5f chr1:%.1d chr2:%1d chr3:%1d chr4:%2d",
			flt1, chr1, chr2, chr3, chr4);
		show_crlf ();
		xprintf("flt2:%.3f flt3:%.0f flt4:%.3f flt5:%.0f chr5:%d?",
			flt2, flt3, flt4, flt5, chr5);
		if (!ask_verify()) {
			cmd_esc = TRUE;
			return;
		}

		cmd_0x71s (cmd,
			pf, flt1, chr1, chr2, chr3, chr4,
			flt2, flt3, flt4, flt5, chr5);
		return;

	case 'l':               /*  Show low pass filter options  */
		cmd_0x71q (cmd);
		return;
	}
}
#endif

/* 0x73 */
static void set_altitude_filter_parms (TSIPPKT *cmd)
{
	float 
		time_constant;

	xprintf("Use altitude filter?");
	if (ask_verify ())  {
		xprintf("Change altitude filter?");
		if (ask_verify()) {
			time_constant = ask_dbl(
				"Enter altitude time constant (# fixes) (0 for default) :");
			if (cmd_esc) return;
			if (time_constant <= 0.0) {
				xputs ("Reset to default?");
				if (!ask_verify()) {
					cmd_esc = TRUE;
					return;
				}
				cmd_0x73c (cmd);
				return;
			}
			else {
				xprintf("num fixes: %.2f ?", time_constant);
				if (!ask_verify()) {
					cmd_esc = TRUE;
					return;
				}
				cmd_0x73s (cmd, time_constant);
				return;
			}
		}
		else {
			if (cmd_esc) return;
			cmd_0x73q (cmd);
			return;
		}

	} else  {
		if (cmd_esc) return;
		xprintf ("Turn off altitude filter?");
		if (!ask_verify ()) {
			cmd_esc = TRUE;
			return;
		}
		cmd_0x73d (cmd);
		return;
	}
}

/* 0x75 */
static void set_or_request_high_6_mode (TSIPPKT *cmd, byte kbch)
{
	static char 
		*trackmode_text[2] = {"Track high 8, best-4 fix",
		"Track high 6, over-determined fix"};
	byte 
		trackmode;

	switch (kbch) {

	case 'j':
		/*  Request HIGH 6 mode status */
		xprintf("Request track mode status");
		cmd_0x75q (cmd);
		return;
	
	case 'J':
		/*  set high 6 mode */
		trackmode = pick_one ("Tracking mode:  ", trackmode_text, 2);
		xprintf ("Set tracking mode to:  %s", trackmode_text[trackmode]);
		if (!ask_verify()) {
			cmd_esc = TRUE;
			return;
		}
		cmd_0x75s (cmd, trackmode);
		return;
	}
}

/* 0x77 */
static void set_or_request_DC_maxage (TSIPPKT *cmd, byte kbch)
{
	int  maxage;

	if (kbch == 'g') {
		xprintf("Request DC max age");
		cmd_0x77q (cmd);
		}
	else {	 /* kcbh = 'G' */
		maxage = ask_int ("Enter Maximum age for Differential corrections:  ");
		if (cmd_esc) return;
		xprintf ("Set new maxage to %2d secs?", maxage);
		if (!ask_verify ()) {
			cmd_esc = TRUE;
			return;
			}
		cmd_0x77s (cmd, maxage);
		}
}

/* 0x8E 14/15 */
static void set_or_request_datum (TSIPPKT *cmd, byte kbch)
{
	int  datum_idx;
	double datum_coeffs[5];

	if (kbch == 'k') {
		xprintf("Request datum");
		cmd_0x8E15q (cmd);
		}
	else {	 /* kcbh = 'K' */
		datum_idx = ask_int ("Enter Desired Datum index (-1 for custom) :  ");
		if (cmd_esc) return;
		if (datum_idx == -1) {
			/* Get 5 double values */
			datum_coeffs[0] = ask_dbl(" Enter value for DX :");
			datum_coeffs[1] = ask_dbl(" Enter value for DY :");
			datum_coeffs[2] = ask_dbl(" Enter value for DZ :");
			datum_coeffs[3] = ask_dbl(" Enter value for A-axis :");
			datum_coeffs[4] = ask_dbl(" Enter value for E-squared :");
			xprintf("Confirm %g %g %g %10.3f %16.14f ?", datum_coeffs[0],datum_coeffs[1],
				datum_coeffs[2], datum_coeffs[3], datum_coeffs[4]);
			if (!ask_verify ()) {
				cmd_esc = TRUE;
				return;
				}
			}
		else {
			xprintf ("Set new datum to %2d?", datum_idx);
			if (!ask_verify ()) {
				cmd_esc = TRUE;
				return;
				}
			}
		/* Since 8E 14 does not generate an auto reply we need to send */
		/* an explicit query with it to satisfy the report window      */
		cmd_0x8E14s (cmd, datum_idx,datum_coeffs);
		send_cmd(cmd);  /* Force sending */
		cmd_0x8E15q(cmd); /* So we can immediately follow with query */
		}
}


/**/
#ifndef TNL
#define NUM_KB_CMDS 53
int num_kb_cmds = NUM_KB_CMDS;
/* list of keyboard strokes and command each invokes. */
KB_CMD kb_cmd[NUM_KB_CMDS] = {
	{" f",  'f', 0x1D, "clear osc offset "},
	{" F",  'F', 0x1D, "change osc offset"},
	{"^K", 0x0B, 0x1E, "clear battery RAM"},
	{" v",  'v', 0x1F, "show S/W version "},
	{" A",  'A', 0x20, "almanac data     "},
	{" t",  't', 0x21, "show time        "},
	{" z",  'z', 0x21, "sync PC to UTC   "},
	{" n",  'n', 0x22, "set manual 2-D   "},
	{" N",  'N', 0x22, "set manual 3-D   "},
	{"^N", 0x0E, 0x22, "set auto 2-D/3-D "},
	{"^T", 0x14, 0x22, "set 1SV time mode"},
	{" i",  'i', 0x23, "input XYZ pos    "},
	{" M",  'M', 0x24, "show 2-D/3-D mode"},
	{"^R", 0x12, 0x25, "reset receiver   "},
	{" h",  'h', 0x26, "receiver health  "},
	{" s",  's', 0x27, "show sig levels  "},
	{" m",  'm', 0x28, "GPS sys message  "},
	{" a",  'a', 0x29, "almanac health   "},
	{"^A", 0x01, 0x2A, "altitude for 2-D "},
	{" I",  'I', 0x2B, "input LLA pos    "},
	{" p",  'p', 0x2C, "show mask parms  "},
	{" P",  'P', 0x2C, "set mask parms   "},
	{" o",  'o', 0x2D, "show osc offset  "},
	{" T",  'T', 0x2E, "set time from PC "},
	{" u",  'u', 0x2F, "show UTC info    "},
	{" b",  'b', 0x31, "set acc ECEF pos "},
	{" B",  'B', 0x32, "set acc LLA pos  "},
	{" S",  'S', 0x34, "set SV for 1-SV  "},
	{" O",  'O', 0x35, "show I/O options "},
	{"^O", 0x0F, 0x35, "set I/O options  "},
	{"^V", 0x16, 0x36, "velocity input   "},
	{" w",  'w', 0x37, "show last fix    "},
	{" $",  '$', 0x38, "set/recv SV data "},
	{" !",  '!', 0x38, "navdata from rcvr"},
	{" @",  '@', 0x38, "navdata into rcvr"},
	{" d",  'd', 0x39, "disable/ign hlth "},
	{" <",  '<', 0x3A, "show raw meas    "},
	{" e",  'e', 0x3B, "ephemeris info   "},
	{" >",  '>', 0x3C, "show track status"},
	{" U",  'U', 0x3D, "show CHAN-A formt"},
	{"^U", 0x15, 0x3D, "set CHAN-A format"},
	{" Z",  'Z', 0x3E, "show aux fix prms"},
	{" D",  'D', 0x62, "set DGPS mode    "},
	{" /",  '/', 0x65, "show DGPS info   "},
	{" l",  'l', 0x71, "show pos flt pms "},
	{" L",  'L', 0x71, "set pos flt prms "},
	{" H",  'H', 0x73, "height filter    "},
	{" j",  'j', 0x75, "show HIGH 6 mode "},
	{" J",  'J', 0x75, "set HIGH 6 mode  "},
	{" g",  'g', 0x77, "show DC Max Age  "},
	{" G",  'G', 0x77, "Set  DC Max Age  "},
/* Not yet fully implemented in 4.06 firmware */
/*	{" c",  'c', 0x8E, "Show CHAN-B formt"}, */
/*	{" C",  'C', 0x8E, "Set CHAN-B format"}, */
	{" k",  'k', 0x8E, "Show Datum values"},
	{" K",  'K', 0x8E, "Set Datum values "}
};

/* When '?' is entered, a command list is displayed and returns FALSE. */
/* When unrecognized keystroke is entered, error message appears and */
/* returns FALSE. Otherwise, loads command code and returns TRUE.*/

int interpret_keystroke (byte kbch, byte *cmdcode)
{
	/* list of keyboard strokes and command each invokes is above. */
	int
		icmd, icol, irow, col_len;

	/* keystroke is '?'; display commands in command window and return */
	if (kbch == '?') {
		/*  Display command list, exit */
		xputs ("Keystroke command list:  (key  pkt-ID  description) ");
		col_len = (num_kb_cmds+2)/3;
		for (irow=0; irow < col_len; irow++) {
			show_crlf();
			for (icol=0; icol<3; icol++) {
				icmd = irow + col_len*icol;
				if (icmd >= num_kb_cmds) break;
				xprintf (" %s 0x%2.2X %s", kb_cmd[icmd].kb_stroke,
					kb_cmd[icmd].cmdcode, kb_cmd[icmd].cmdtext);
			}
		}
		show_crlf ();
		xputs (
"  <CR>=clear window;   <ESC>=EXIT;   ^F=log TSIP;   ^L=log TAIP;   ^I=RS232"
	);
		return FALSE;
	}

	/* search command list */
	for (icmd=0; icmd<num_kb_cmds; icmd++) {
		if (kb_cmd[icmd].kbch==kbch) {
			*cmdcode = kb_cmd[icmd].cmdcode;
			xprintf ("command 0x%02X - %s", 
				kb_cmd[icmd].cmdcode, kb_cmd[icmd].cmdtext);
			show_crlf ();
			return TRUE;
		}
	}

	/* no valid command found */
	if (isprint(kbch)) {
		xprintf ("Keystroke '%c' unknown", kbch);
	}
	else if (kbch>=cntrl('A') && kbch <= cntrl('Z')) {
		xprintf ("Keystroke '^%c' unknown", kbch - cntrl('A') + 'A');
	}
	else {
		xprintf ("Keystroke %2.2Xh unknown", kbch);
	}
	return FALSE;
}
#endif

/**/
/* Returns > 0 if cmdcode requests some reply, 0 otherwise. The vector
reply[], indexed by rpt.code, indicates the number of replies expected for
each type of report requested by a command. */

int reply_requested (byte cmdcode, int *reply)
{

	/* Reply is a vector indexed by rptcode. */
	/* reply[0x??] = no. of reports expected for rptcode 0x??. */
	/* reply[0x??] <= 0 means no report expected. */

	/* TSIP commands */
	switch (cmdcode) {

	case 0x1F:
		return (reply [0x45] = 1);
	case 0x20:
		return (reply [0x40] = 1);
	case 0x21:
		return (reply [0x41] = 1);
	case 0x22:
		return (reply [0x44] = reply [0x6D] = 1);
	case 0x24:
		return (reply [0x44] = reply [0x6D] = 1);
	case 0x25:
		return (reply [0x45] = reply [0x46] = reply [0x4B] = reply [0x42] =
			reply [0x83] =  reply [0x4A] = reply [0x84] = 1);
	case 0x26:
		return (reply [0x46] = reply [0x4B] = 1);
	case 0x27:
		return (reply [0x47] = 1);
	case 0x28:
		return (reply [0x48] = 1);
	case 0x29:
		return (reply [0x49] = 1);
	case 0x2C:
		return (reply [0x4C] = 1);
	case 0x2D:
		return (reply [0x4D] = 1);
	case 0x2E:
		return (reply [0x4E] = 1);
	case 0x2F:
		return (reply [0x4F] = 1);
	case 0x35:
		return (reply [0x55] = 1);
	case 0x37:
		return (reply [0x57] = reply [0x42] = reply [0x83] = reply [0x4A] = 
			reply [0x84] = reply [0x43] = reply [0x56] = 1);
	case 0x38:
		return (reply [0x58] = 1);
	case 0x39:
		return (reply [0x59] = 1);
	case 0x3A:
		return (reply [0x5A] = 32);
	case 0x3B:
		return (reply [0x5B] = 32);
	case 0x3C:
		return (reply [0x5C] = reply [0x6C] = 32);
	case 0x3D:
		return (reply [0x3D] = 1);
	case 0x3E:
		return (reply[0x5E] = 1);
	case 0x62:
		return (reply [0x82] = 1);
	case 0x65:
		return (reply [0x85] = 32);
	case 0x71:
		return (reply[0x72] = 1);
	case 0x73:
		return (reply [0x74] = 1);
	case 0x75:
		return (reply[0x76] = 1);
	case 0x77:
		return (reply [0x78] = 1);
	case 0x8E:
		return (reply [0x8F] = 1);
	default:
#ifdef TNL
		return tnl_reply (cmdcode, reply);
#else
		return 0;
#endif

	}
}

/**/
/* returns TRUE if a command is to be sent to the receiver, FALSE otherwise */
void proc_kbd (byte kbch, TSIPPKT *cmd)
{
	/* Note that kbch is used again as an option for some command choices. */

	cmd_esc = FALSE;

	/* TSIP commands */
	switch (cmd->code) {

	case 0x1D:
		set_or_clear_osc_offset (cmd, kbch);
		break;

	case 0x1E:
		clear_RAM (cmd);
		break;

	case 0x1F:
		request_SW_version (cmd);
		break;

	case 0x20:
		request_almanac_data_page (cmd);
		break;

	case 0x21:
		request_time (cmd, kbch);
		break;

	case 0x22:
		set_nav_mode (cmd, kbch);
		break;

	case 0x23:
		set_initial_ECEF_position (cmd);
		break;

	case 0x24:
		request_nav_mode (cmd);
		break;

	case 0x25:
		reset_RX (cmd);
		break;

	case 0x26:
		request_rx_health (cmd);
		break;

	case 0x27:
		request_SNRs (cmd);
		break;

	case 0x28:
		request_GPS_system_message (cmd);
		break;

	case 0x29:
		request_almanac_health_page (cmd);
		break;

	case 0x2A:
		set_altitude (cmd);
		break;

	case 0x2B:
		set_initial_LLA_position (cmd);
		break;

	case 0x2C:
		set_or_request_op_parms (cmd, kbch);
		break;

	case 0x2D:
		request_osc_offset (cmd);
		break;

	case 0x2E:
		set_time (cmd);
		break;

	case 0x2F:
		request_UTC_data (cmd);
		break;

	case 0x31:
		set_accurate_ECEF_position (cmd);
		break;

	case 0x32:
		set_accurate_LLA_position (cmd);
		break;

	case 0x34:
		set_SV_for_1SV_mode (cmd);
		break;

	case 0x35:
		set_or_request_IO_options (cmd, kbch);
		break;

	case 0x36:
		initial_velocity (cmd);
		break;

	case 0x37:
		request_last_pos_info (cmd);
		break;

	case 0x38:
		set_or_request_SV_system_data (cmd, kbch);
		break;

	case 0x39:
		request_disable_or_health_override_list (cmd);
		break;

	case 0x3A:
		request_raw_msmt_data (cmd);
		break;

	case 0x3B:
		request_ephemeris_status (cmd);
		break;

	case 0x3C:
		request_track_info (cmd);
		break;

	case 0x3D:
		set_or_request_chan_config (cmd, kbch);
		break;

	case 0x3E:
		request_aux_fix_parms (cmd);
		break;

	case 0x62:
		set_or_request_DGPS_mode (cmd);
		break;

	case 0x65:
		request_DGPS_corrections (cmd);
		break;

	case 0x71:
		set_or_request_pos_filter_parms (cmd, kbch);
		break;

	case 0x73:
		set_altitude_filter_parms (cmd);
		break;

	case 0x75:
		set_or_request_high_6_mode (cmd, kbch);
		break;
	
	case 0x77:
		set_or_request_DC_maxage (cmd, kbch);
		break;

	case 0x8E:
		switch (kbch) {
			case 'C':
			case 'c':
				set_or_request_chan_config (cmd, kbch);
				break;
			case 'k':
			case 'K':
				set_or_request_datum (cmd, kbch);
				break;
			}
		break;

	default:
#ifdef TNL
		tnl_cmd (cmd, kbch);
#endif
		break;
			
	}
	if (cmd_esc) {
		cmd->code = 0x00;
		show_crlf();
		xprintf ("-- aborted --");
	}

}
