/*
	
Vers	Date		Changes										Author
----	--------	----------------------------------------	----------
1.1		21 Jun 93	initial version								P Loomis
1.11	17 Feb 94	corrections as indicated by 'v1-11'			P Loomis

TSIP_RPT.C consists of a main subroutine RPT_PACKET() called by MAIN().  

RPT_PACKET() 
This routine takes a character buffer that has been received as a
report from a TSIP device and interprets it.  The character buffer
has been assembled using a sequence of routines contained in TSIP_IFC.C:
RESET_RPTBUF(), ACCUMULATE_RPTBUF(), END_OF_RPTBUF(), and UNSTUFF_RPTBUFF().

A large CASE statement directs processing to one of many mid-level
subroutines.  The mid-level subroutine specific to the current report
code passes the report buffer to the appropriate report decoder
rpt_0x?? () in TSIP_IFC.C, which outputs the variable values in the
report buffer rpt.buf.  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include "tsipincl.h"

/* utility to indicate report packet length error */
void bad_len (int id, int len)
{
	xprintf ("Bad packet length:  ID %#02X, length %d", id, len);
}

/**/
/* 0x3D */
static void show_chan_A_config (TSIPPKT *rpt)
{
	static char
		baudnum[10] = { 0, 1, 4, 5, 6, 8, 9, 11, 12, 28 },

		*parity_text [5] = {"even", "odd", "", "", "no"},

		*baud_text[10] = { "50 bps", "110 bps", "300 bps", "600 bps",
		"1200 bps", "2400 bps", "4800 bps", "9600 bps",
		"38.4 Kbps", "19.2 Kbps"},

		*input_cha[2] = { "Packets", "Raw RTCM data"},

		*output_cha_TANS[6] = { "Packets",	"No output",
		"Flow-thru from SuperTANS", "", "", "NMEA 0183"};

	byte 
		tx_baud_index, rx_baud_index,
		char_format_index, databits, parity,
		stop_bits,
		tx_mode_index, rx_mode_index;

	int 
		i;

	/* unload rptbuf */
	if (!rpt_0x3D (rpt,
		&tx_baud_index, &rx_baud_index, &char_format_index, &stop_bits, 
		&tx_mode_index, &rx_mode_index)) {
		bad_len (0x3D, rpt->cnt);
		return;
	}

	xprintf ("Channel A Configuration");
	show_crlf();

	/* 10 baud rate choices */
	for(i=0; i<10; ++i)	if (tx_baud_index == baudnum[i]) break;

	xprintf ("Transmit %s at %s Baud",
		/* changed, v1-11 */
		(tx_mode_index>5)?"undefined":output_cha_TANS[tx_mode_index],
/*		(tx_mode_index>1)?"undefined":output_cha_TANS[tx_mode_index],*/
		(i==10)?"undefined":baud_text[i]);
	show_crlf ();

	/* 10 baud rate choices */
	for(i=0; i<10; ++i)	if (rx_baud_index == baudnum[i]) break;

	xprintf("Receive %s at %s Baud",
		/* changed, v1-11 */
		(rx_mode_index>1)?"undefined":input_cha[rx_mode_index],
/*		(rx_mode_index>5)?"undefined":input_cha[rx_mode_index],*/
		(i==10)?"undefined":baud_text[i]);
	show_crlf ();

	databits = (char_format_index & 0x03) + 5;
	parity = char_format_index >> 2;
	if (parity > 4) parity = 2;

	xprintf("Character format (bits/char, parity, stop bits) is:  %d-%s-%d",
		databits, parity_text[parity], stop_bits);

}

/**/
/* 0x40 */
static void rpt_almanac_data_page (TSIPPKT *rpt)
{
	byte 
		sv_prn;
	int 
		week_num;
	float
		t_zc,
		eccentricity,
		t_oa,
		i_0,
		OMEGA_dot,
		sqrt_A,
		OMEGA_0,
		omega,
		M_0;

	/* unload rptbuf */
	if (!rpt_0x40 (rpt,
		&sv_prn, &week_num, &t_zc, &eccentricity, &t_oa,
		&i_0, &OMEGA_dot, &sqrt_A, &OMEGA_0, &omega, &M_0)) {
		bad_len (0x40, rpt->cnt);
		return;
	}
	
	xprintf ("Almanac for SV #%2d", sv_prn);
	show_crlf ();
	
	xprintf ("       Captured:");
	xprintf ("%15.0f", t_zc);
	show_time (t_zc);
	xprintf ("   week:%5d", week_num);
	show_crlf ();
	
	xprintf ("   Eccentricity:%15g", eccentricity);
	show_crlf ();
	
	xprintf ("           T_oa:");
	xprintf ("%15.0f", t_oa);
	show_time (t_oa);
	show_crlf ();

	xprintf ("            i 0:%15g", i_0);
	show_crlf ();
	xprintf ("      OMEGA dot:%15g", OMEGA_dot);
	show_crlf ();
	xprintf ("  square root A:%15g", sqrt_A);
	show_crlf ();
	xprintf ("        OMEGA 0:%15g", OMEGA_0);
	show_crlf ();
	xprintf ("          omega:%15g", omega);
	show_crlf ();
	xprintf ("            M 0:%15g", M_0);
}

/* 0x41 */
static void rpt_GPS_time (TSIPPKT *rpt)
{
	float
		time_of_week, UTC_offset;
	int
		week_num;

	/* unload rptbuf */
	/* unload rptbuf */
	if (!rpt_0x41 (rpt, &time_of_week, &UTC_offset, &week_num)) {
		bad_len (0x41, rpt->cnt);
		return;
	}

	xprintf ("GPS time:");
	show_time (time_of_week);
	xprintf ("   Week # %d   UTC offset %4.1f secs", week_num, UTC_offset);
	show_crlf();

#ifdef PORT_INPUT
	/* This subroutine will reset PC clock if a request has been previously */
	/* generated in a 't' command.  Record of the request is kept by */
	/* time_set_request in TSIP_UTL.C, set by request_PC_time_set() */
	/* which is called from the 't' command handler in TSIP_CMD.C */
	GPS_time_to_PC 
		(time_of_week + week_num*604800. - UTC_offset);
#endif
}

/*0x42 */
static void rpt_float_ECEF_position (TSIPPKT *rpt)
{
	float 
		ECEF_pos[3], time_of_fix;

	/* unload rptbuf */
	if (!rpt_0x42 (rpt, ECEF_pos, &time_of_fix)) {
		bad_len (0x42, rpt->cnt);
		return;
	}

	xprintf ("Position:%15.2f%15.2f%15.2f    ",
		ECEF_pos[0], ECEF_pos[1], ECEF_pos[2]);
	show_time (time_of_fix);
}

/* 0x43 */
static void rpt_float_ECEF_velocity (TSIPPKT *rpt)
{

	float 
		ECEF_vel[3], freq_offset, time_of_fix;

	/* unload rptbuf */
	if (!rpt_0x43 (rpt, ECEF_vel, &freq_offset, &time_of_fix)) {
		bad_len (0x43, rpt->cnt);
		return;
	}

	xprintf ("Velocity:");
	xprintf ("%11.3f%11.3f%11.3f%12.3f    ",
		ECEF_vel[0], ECEF_vel[1], ECEF_vel[2], freq_offset);
	show_time (time_of_fix);
}

/* 0x44 */
static void rpt_4SV_selection (TSIPPKT *rpt) {

	byte 
		manual_mode, nsvs, sv_prn[4];
	float 
		pdop, hdop, vdop, tdop;

	/* unload rptbuf */
	if (!rpt_0x44 (rpt,
		&manual_mode, &nsvs, sv_prn, &pdop, &hdop, &vdop, &tdop)) {
		bad_len (0x44, rpt->cnt);
		return;
	}

	xprintf ("Mode:  ");
	xprintf ("%s %d Sat (%d-D) :  %2d %2d %2d %2d",
		manual_mode?"Manual":"  Auto",
		nsvs, nsvs-1, sv_prn[0], sv_prn[1], sv_prn[2], sv_prn[3]);
	xprintf ("  P,H,V,TDOP = ");
	xprintf ("%.2f %.2f %.2f %.2f", pdop, hdop, vdop, tdop);
}

/*  0x45  */
static void rpt_SW_version (TSIPPKT *rpt) {
	byte
		major_nav_version, minor_nav_version,
		nav_day, nav_month, nav_year,
		major_dsp_version, minor_dsp_version,
		dsp_day, dsp_month, dsp_year;

	/* unload rptbuf */
	if (!rpt_0x45 (rpt,
		&major_nav_version, &minor_nav_version,
		&nav_day, &nav_month, &nav_year,
		&major_dsp_version, &minor_dsp_version,
		&dsp_day, &dsp_month, &dsp_year)) {
		bad_len (0x45, rpt->cnt);
		return;
	}

	xprintf ("Software Versions:");
	xprintf ("   Nav Proc %2d.%02d  %2d/%2d/%2d",
		major_nav_version, minor_nav_version,
		nav_day, nav_month, nav_year);
	xprintf ("   Sig Proc %2d.%02d  %2d/%2d/%2d",
		major_dsp_version, minor_dsp_version,
		dsp_day, dsp_month, dsp_year);
}

/* 0x46 */
#define MAX_SC_MESSAGE 13
#define MAX_EC_MESSAGE 6
static void rpt_rcvr_health (TSIPPKT *rpt)
{
	byte 
		status_code, error_code;
	int 
		i;

	static char 
		*sc_text[MAX_SC_MESSAGE] = {
		"Doing position fixes",
		"Don't have GPS time yet",
		"Waiting for almanac collection",
		"PDOP too high",
		"No satellites available",
		"Only 1 satellite available",
		"Only 2 satellites available",
		"Only 3 satellites available",
		"No satellites usable",
		"Only 1 satellite usable",
		"Only 2 satellites usable",
		"Only 3 satellites usable",
		"Chosen satellite unusable"},

		*ec_text[MAX_EC_MESSAGE] = {
		"Battery back-up failed",
		"Signal processor error",
		"Alignment of channel 1",
		"Alignment of channel 2",
		"Antenna feed line fault",
		"Excessive Freq Bias"};


	/* unload rptbuf */
	if (!rpt_0x46 (rpt, &status_code, &error_code)) {
		bad_len (0x46, rpt->cnt);
		return;
	}

	if (status_code > MAX_SC_MESSAGE) {
		xprintf ("Unknown status code %d", status_code);
	}
	else {
		xprintf ("Health of GPS:  %s", sc_text[rpt->buf[0]]);
		for (i=0; i<MAX_EC_MESSAGE; i++) {
			if (status_code && (error_code & (1 << i))) {
				show_crlf (); 
				xprintf ("        %s", ec_text[i]);
			} 
		} 
	} 
}

/* 0x47 */
static void rpt_SNR_all_SVs (TSIPPKT *rpt)
{
	byte 
		nsvs, sv_prn[12];
	int 
		isv;
	float 
		snr[12];

	/* unload rptbuf */
	if (!rpt_0x47 (rpt, &nsvs, sv_prn, snr)) {
		bad_len (0x47, rpt->cnt);
		return;
	}

	xprintf ("SNR for satellites:  %d", nsvs);
	for (isv=0; isv<nsvs; isv++) {
		show_crlf ();
		xprintf ("   SV %2d   %6.2f", sv_prn[isv], snr[isv]);
	}
}

/* 0x48 */
static void rpt_GPS_system_message (TSIPPKT *rpt)
{
	byte 
		message[23];

	/* unload rptbuf */
	if (!rpt_0x48 (rpt, message)) {
		bad_len (0x48, rpt->cnt);
		return;
	}

	xprintf ("GPS message:  %s", message);
}

/* 0x49 */
static void rpt_almanac_health_page (TSIPPKT *rpt)
{
	short 
		iprn;
	byte 
		sv_health [32];

	/* unload rptbuf */
	if (!rpt_0x49 (rpt, sv_health)) {
		bad_len (0x49, rpt->cnt);
		return;
	}

	xprintf ("Almanac health page:");
	for (iprn=0; iprn<32; iprn++) {
		if (!(iprn%5)) show_crlf ();
		xprintf ("    SV%2.2d  %2X", (iprn+1) , sv_health[iprn]);
	}
}

/* 0x4A */
static void rpt_float_lla_position (TSIPPKT *rpt) {
	int
		lat_deg, lon_deg;
	float
		lat, lon, lat_min, lon_min,
		alt, clock_bias, time_of_fix;
	byte
		north_south, east_west;

	if (!rpt_0x4A (rpt,
		&lat, &lon, &alt, &clock_bias, &time_of_fix)) {
		bad_len (0x4A, rpt->cnt);
		return;
	}

	/* convert from radians to degrees */
	lat *= R2D;
	lon *= R2D;
	north_south = 'N';
	if (lat < 0.0) {
		north_south = 'S';
		lat = -lat;
	}
	lat_deg = lat;
	lat_min = (lat - lat_deg) * 60.0;
	east_west = 'E';
	if (lon < 0.0) {
		east_west = 'W';
		lon = -lon;
	}
	lon_deg = lon;
	lon_min = (lon - lon_deg) * 60.0;
	xprintf ("Position:");
	xprintf ("%4d %6.3f %c", lat_deg, lat_min, north_south);
	xprintf ("%5d %6.3f %c", lon_deg, lon_min, east_west);
	xprintf ("%10.2f%12.2f", alt, clock_bias);
	show_time (time_of_fix);
}

/* 0x4B */
#define MAX_AS1_MESSAGE 4
#define MAX_MID_MESSAGE 41
static void rpt_rcvr_id_and_status (TSIPPKT *rpt)
{
	static char 
		*as1_text[MAX_AS1_MESSAGE] = {
		"Synthesizer fault",
		"Battery powered time clock fault",
		"A-to-D converter fault",
		"Almanac not complete and current"  },

		*mid_text[MAX_MID_MESSAGE] = {
		"10X and 400", "10X", "400", "Datum Timing Module",
		"2 Ch TANS", "6-Channel", "TRIMPACK", "TANS Timing Module",
		"ND TANS", "3 Ch TANS", "3 Ch TRIMPACK", "Navigator GPS",
		"Aircraft GPS", "TRANSPAK", "Pathfinder Basic", "TRANSPAK 2",
		"unused", "Jupiter 7", "Trident", "DAR",
		"6 Ch TANS", "TOGA", "Sushi", "Placer",
		"Newton II", "6 Channel Timing TANS", "Car 54", "Jupiter 6",
		"Acutis 6", "Pathfinder Basic-E6", "TNL-1000 Cobra GPS",
		"Jupiter 6 Sushi",
		"P-code Galileo", "S/A Trimpack", "Jupiter 6 NMEA OEM", 
		"Delco GPS Sensor",
		"Placer GPS 300", "MARS", "Galaxy GPS", "ELVIS 3 Channel",
		"Vector TANS"};

	byte 
		machine_id, status_1, status_2;
	int 
		imsg;

	/* unload rptbuf */
	if (!rpt_0x4B (rpt, &machine_id, &status_1, &status_2)) {
		bad_len (0x4B, rpt->cnt);
		return;
	}

	xprintf ("Machine/Code ID: %d  '%s'   Status: %d %d", machine_id,
		(machine_id>=MAX_MID_MESSAGE)?"unknown":mid_text[machine_id],
		status_1, status_2);
	for (imsg = 0; imsg < MAX_AS1_MESSAGE; imsg++) {
		if (status_1 & (1 << imsg)) {
			show_crlf ();
			xprintf ("        %s", as1_text[imsg]);
		}
	}
}

/* 0x4C */
#define MAX_DYN_MESSAGE 5
static void rpt_operating_parameters (TSIPPKT *rpt)
{
	byte
		dyn_code;
	float
		el_mask, snr_mask, pdop_mask, pdop_switch;
	static char 
		*dyn_text[MAX_DYN_MESSAGE] = {"Unchanged",
		"Land", "Sea", "Air (default)", "Static"};

	/* unload rptbuf */
	if (!rpt_0x4C (rpt, &dyn_code, &el_mask,
		&snr_mask, &pdop_mask, &pdop_switch)) {
		bad_len (0x4C, rpt->cnt);
		return;
	}

	xprintf ("Operating Parameters:");
	show_crlf ();
	xprintf ("    Dynamics code = %d %s",
		dyn_code,
		(dyn_code<6)?dyn_text[dyn_code]:"unknown dynamics code");
	show_crlf ();

	xprintf ("   Elevation mask = %.2f degrees", el_mask*R2D);
	show_crlf ();

	xprintf ("         SNR mask = %.2f", snr_mask);
	show_crlf ();

	xprintf ("        PDOP mask = %.2f", pdop_mask);
	show_crlf ();

	xprintf ("      PDOP switch = %.2f", pdop_switch);
}

/* 0x4D */
static void rpt_oscillator_offset (TSIPPKT *rpt)
{
	float 
		osc_offset;

	/* unload rptbuf */
	if (!rpt_0x4D (rpt, &osc_offset)) {
		bad_len (0x4D, rpt->cnt);
		return;
	}

	xprintf ("Oscillator offset:   %.2f Hz.", osc_offset);
}

/* 0x4E */
static void rpt_GPS_time_set_response (TSIPPKT *rpt)
{

	byte 
		response;

	/* unload rptbuf */
	if (!rpt_0x4E (rpt, &response)) {
		bad_len (0x4E, rpt->cnt);
		return;
	}

	switch (response) {
	case 'Y':
		xprintf ("Time set accepted");
		break;
	case 'N':
		xprintf ("Time set rejected or not required");
		break;
	default:
		xprintf ("Time set response error");
	}
}

/* 0x4F */
static void rpt_UTC_offset (TSIPPKT *rpt)
{
	double
		a0;
	float
		a1, time_of_data;
	int
		dt_ls, wn_t, wn_lsf, dn, dt_lsf;

	/* unload rptbuf */
	if (!rpt_0x4F (rpt, &a0, &a1, &time_of_data,
		&dt_ls, &wn_t, &wn_lsf, &dn, &dt_lsf)) {
		bad_len (0x4F, rpt->cnt);
		return;
	}

	xprintf ("UTC correction data");
	show_crlf ();

	xprintf ("           A 0 = %15g", a0);
	show_crlf ();

	xprintf ("           A 1 = %15g", a1);
	show_crlf ();

	xprintf ("    delta t LS = %6d", dt_ls);
	show_crlf ();

	xprintf ("          t ot = %6.0f  ", time_of_data);
	show_time (time_of_data);
	show_crlf ();

	xprintf ("          WN t = %6d", wn_t);
	show_crlf ();

	xprintf ("        WN LSF = %6d", wn_lsf);
	show_crlf ();

	xprintf ("            DN = %6d", dn);
	show_crlf ();

	xprintf ("   delta t LSF = %6d", dt_lsf);
}

/**/

/* 0x54 */
static void rpt_1SV_bias (TSIPPKT *rpt)
{
	float 
		clock_bias, freq_offset, time_of_fix;

	/* unload rptbuf */
	if (!rpt_0x54 (rpt, &clock_bias, &freq_offset, &time_of_fix)) {
		bad_len (0x54, rpt->cnt);
		return;
	}

	xprintf ("One satellite based");
	xprintf ("   Bias: %6.2f", clock_bias);
	xprintf ("   Bias Rate: %6.2f", freq_offset);
	show_time (time_of_fix);
}

/* 0x55 */
static void rpt_io_opt (TSIPPKT *rpt)
{
	byte 
		pos_code, vel_code, time_code, aux_code;

	/* unload rptbuf */
	if (!rpt_0x55 (rpt,
		&pos_code, &vel_code, &time_code, &aux_code)) {
		bad_len (0x55, rpt->cnt);
		return;
	}
	/* rptbuf unloaded */

	xprintf ("I/O Options:  %02X %02X %02X %02X",
		pos_code, vel_code, time_code, aux_code);

	if (pos_code & 0x01) {
		show_crlf ();
		xprintf ("    ECEF XYZ position output");
	}

	if (pos_code & 0x02) {
		show_crlf ();
		xprintf ("    LLA position output");
	}

	show_crlf ();
	if (pos_code & 0x04)
		xprintf ("    MSL altitude output (Geoid height) ");
	else
		xprintf ("    WGS-84 altitude output");

	show_crlf ();
	if (pos_code & 0x08)
		xprintf ("    MSL altitude input");
	else
		xprintf ("    WGS-84 altitude input");

	show_crlf ();
	if (pos_code & 0x10)
		xprintf ("    Double precision");
	else
		xprintf ("    Single precision");

	if (pos_code & 0x20) {
		show_crlf ();
		xprintf ("    Superpacket ");
		xprintf ("%s", (pos_code&0x40)?"ASCII":"binary");
	}

	if (vel_code & 0x01) {
		show_crlf ();
		xprintf ("    ECEF XYZ velocity output");
	}

	if (vel_code & 0x02) {
		show_crlf ();
		xprintf ("    ENU velocity output");
	}

	show_crlf ();
	if (time_code & 0x01)
		xprintf ("    Time tags in UTC");
	else
		xprintf ("    Time tags in GPS time");

	if (time_code & 0x02) {
		show_crlf ();
		xprintf ("    Fixes delayed to integer seconds");
	}

	if (time_code & 0x04) {
		show_crlf ();
		xprintf ("    Fixes sent only on request");
	}

	if (time_code & 0x08) {
		show_crlf ();
		xprintf ("    Synchronized measurements");
	}

	if (time_code & 0x10) {
		show_crlf ();
		xprintf ("    Minimize measurement propagation");
	}

	if (aux_code & 0x01) {
		show_crlf ();
		xprintf ("    Raw measurement output");
	}

	if (aux_code & 0x02) {
		show_crlf ();
		xprintf ("    Code-phase smoothed before output");
	}

	if (aux_code & 0x04) {
		show_crlf ();
		xprintf ("    Additional fix status");
	}

}

/* 0x56 */
static void rpt_ENU_velocity (TSIPPKT *rpt)
{
	float
		vel_ENU[3], freq_offset, time_of_fix;

	/* unload rptbuf */
	if (!rpt_0x56 (rpt, vel_ENU, &freq_offset, &time_of_fix)) {
		bad_len (0x56, rpt->cnt);
		return;
	}

	xprintf ("Vel  ENU:");
	xprintf ("%11.2f%11.2f%11.2f%12.2f    ",
		vel_ENU[0], vel_ENU[1], vel_ENU[2], freq_offset);
	show_time (time_of_fix);
}

/* 0x57 */
static void rpt_last_fix_info (TSIPPKT *rpt)
{
	byte
		source_code, diag_code;
	int
		week_num;
	float
		time_of_fix;

	/* unload rptbuf */
	if (!rpt_0x57 (rpt, &source_code, &diag_code, &week_num, &time_of_fix)) {
		bad_len (0x57, rpt->cnt);
		return;
	}

	xprintf ("Last fix information:");
	show_crlf ();

	xprintf ("    Source codes:  %02X %02X",
		source_code, diag_code);
	show_crlf ();

	xprintf ("    Pos_fix time:");
	show_time (time_of_fix);
	xprintf ("   Week no.: %d", week_num);
}

/* 0x58 */
#define MAX_DAT_MESSAGE 7
#define MAX_OPS_MESSAGE 4
static void rpt_GPS_system_data (TSIPPKT *rpt)
{
	byte
		op_code, data_type, sv_prn,
		data_length, data_packet[250];
	ALM_INFO
		*almanac;
	ALH_PARMS
		*almh;
	UTC_INFO
		*utc;
	ION_INFO
		*ionosphere;
	SF1_INFO
		*sdata;
	EPH_INFO
		*edata;
	NAV_INFO
		*nav_data;
	byte
		curr_t_oa;
	short int
		curr_wn_oa;

	static char

		*datname[MAX_DAT_MESSAGE] = {"", "", "Almanac",
		"Health Page, T_oa, WN_oa", "Ionosphere", "UTC ",
		"Ephemeris"},

		*opsname[MAX_OPS_MESSAGE] = {"Acknowledged, can't use",
		"Acknowledged", "Data Out", "No Data Available"};

	/* unload rptbuf */
	if (!rpt_0x58 (rpt, &op_code, &data_type, &sv_prn,
		&data_length, data_packet)) {
		bad_len (0x58, rpt->cnt);
		return;
	}

	if (op_code == 2) {
		xprintf ("System data output[%d]:  ", data_type);
		show_crlf();
		switch (data_type) {
		case 2:
			/* Almanac */
			xprintf ("%s  SV%2.2d  length = %d bytes",
				datname[data_type], sv_prn, data_length);
			show_crlf();
			if (sv_prn == 0 || sv_prn > 32) {
				xprintf ("Binary PRN invalid");
				return;
			}
			almanac = (ALM_INFO*)data_packet;
			xprintf ("t_oa_raw = % -12g  ", almanac->t_oa_raw );
			xprintf ("SV_hlth  = % -12g  ", almanac->SV_health );
			show_crlf();
			xprintf ("e        = % -12g  ", almanac->e        );
			xprintf ("t_oa     = % -12g  ", almanac->t_oa     );
			show_crlf();
			xprintf ("i_0      = % -12g  ", almanac->i_0      );
			xprintf ("OMEGADOT = % -12g  ", almanac->OMEGADOT );
			show_crlf();
			xprintf ("sqrt_A   = % -12g  ", almanac->sqrt_A   );
			xprintf ("OMEGA_0  = % -12g  ", almanac->OMEGA_0  );
			show_crlf();
			xprintf ("omega    = % -12g  ", almanac->omega    );
			xprintf ("M_0      = % -12g  ", almanac->M_0      );
			show_crlf();
			xprintf ("a_f0     = % -12g  ", almanac->a_f0     );
			xprintf ("a_f1     = % -12g  ", almanac->a_f1     );
			show_crlf();
			xprintf ("Axis     = % -12g  ", almanac->Axis     );
			xprintf ("n        = % -12g  ", almanac->n        );
			show_crlf();
			xprintf ("OMEGA_n  = % -12g  ", almanac->OMEGA_n  );
			xprintf ("ODOT_n   = % -12g  ", almanac->ODOT_n   );
			show_crlf();
			xprintf ("t_zc     = % -12g  ", almanac->t_zc     );
			xprintf ("weeknum  = % -12g  ", almanac->weeknum  );
			show_crlf();
			xprintf ("wn_oa    = %f  ", almanac->wn_oa    );
			break;
		case 3:
			/* Almanac health page */
			xprintf ("%s  length = %d bytes  ",
				datname[data_type], data_length);
			show_crlf();
			almh = (ALH_PARMS*)data_packet;
			xprintf ("t_oa = %d, wn_oa = %d  ", almh->t_oa, almh->WN_a);
			show_crlf();
			xprintf ("%d %d %d %d %d %d %d %d  ",
				almh->SV_health[0], almh->SV_health[1], almh->SV_health[2],
				almh->SV_health[3], almh->SV_health[4], almh->SV_health[5],
				almh->SV_health[6], almh->SV_health[7]);
			show_crlf();
			xprintf ("%d %d %d %d %d %d %d %d  ",
				almh->SV_health[8], almh->SV_health[9], almh->SV_health[10],
				almh->SV_health[11], almh->SV_health[12], almh->SV_health[13],
				almh->SV_health[14], almh->SV_health[15]);
			show_crlf();
			xprintf ("%d %d %d %d %d %d %d %d  ",
				almh->SV_health[16], almh->SV_health[17], almh->SV_health[18],
				almh->SV_health[19], almh->SV_health[20], almh->SV_health[21],
				almh->SV_health[22], almh->SV_health[23]);
			show_crlf();
			xprintf ("%d %d %d %d %d %d %d %d  ",
				almh->SV_health[24], almh->SV_health[25], almh->SV_health[26],
				almh->SV_health[27], almh->SV_health[28], almh->SV_health[29],
				almh->SV_health[30], almh->SV_health[31]);
			show_crlf();
			curr_t_oa = data_packet[data_length-3];
			curr_wn_oa = *(int*)data_packet[data_length-2];
			xprintf ("t_oa = %d, wn_oa = %d  ", curr_t_oa, curr_wn_oa);
			break;
		case 4:
			/* Ionosphere */
			xprintf ("%s  length = %d bytes  ",
				datname[data_type], data_length);
			show_crlf();
			ionosphere = (ION_INFO*)data_packet;
			xprintf ("alpha_0 = % -12g   ", ionosphere->alpha_0 );
			xprintf ("alpha_1 = % -12g   ", ionosphere->alpha_1 );
			show_crlf();
			xprintf ("alpha_2 = % -12g   ", ionosphere->alpha_2 );
			xprintf ("alpha_3 = % -12g   ", ionosphere->alpha_3 );
			show_crlf();
			xprintf ("beta_0  = % -12g    ", ionosphere->beta_0 );
			xprintf ("beta_1  = % -12g    ", ionosphere->beta_1 );
			show_crlf();
			xprintf ("beta_2  = % -12g   ", ionosphere->beta_2 );
			xprintf ("beta_3  = % -12g   ", ionosphere->beta_3 );
			show_crlf();
			break;
		case 5:
			/* UTC */
			xprintf ("%s  length = %d bytes",
				datname[data_type], data_length);
			show_crlf();
			/* bgetfloat reverses four-byte strings */
			utc = (UTC_INFO*)data_packet;

			xprintf ("A_0 = %f  ", utc->A_0 );
			xprintf ("A_1 = %f  ", utc->A_1 );
			show_crlf();
			xprintf ("delta_t_LS = %f  ", utc->delta_t_LS);
			xprintf ("t_ot = %f  ", utc->t_ot );
			show_crlf();
			xprintf ("WN_t = %f  ", utc->WN_t );
			xprintf ("WN_LSF = %f  ", utc->WN_LSF );
			show_crlf();
			xprintf ("DN = %f  ", utc->DN );
			xprintf ("delta_t_LSF = %f  ", utc->delta_t_LSF );
			break;
		case 6: /* Ephemeris */
			xprintf ("%s SV%2.2d length = %d bytes  ",
				datname[data_type], sv_prn,	data_length);
			show_crlf();
			if (sv_prn == 0) {
				xprintf ("No ephemeris available for SV%02.2d  ", sv_prn);
				show_crlf();
				return;
			}
			if (sv_prn > 32) {
				xprintf ("Binary PRN invalid");
				show_crlf();
				return;
			}

			nav_data = (NAV_INFO*)data_packet;

			xprintf ("   SV_PRN = % -12d . ", nav_data->sv_number );
			xprintf ("  t_ephem = % -12g . ", nav_data->t_ephem );
			show_crlf();

			sdata = &(nav_data->subframe1);

			xprintf ("  weeknum = % -12d . ", sdata->weeknum );
			xprintf ("   codeL2 = % -12d . ", sdata->codeL2 );
			xprintf ("  L2Pdata = % -12d", sdata->L2Pdata );
			show_crlf();

			xprintf ("SVacc_raw = % -12d . ", sdata->SVacc_raw );
			xprintf ("SV_health = % -12d . ", sdata->SV_health );
			xprintf ("     IODC = % -12d", sdata->IODC );
			show_crlf();

			xprintf ("     T_GD = % -12g . ", sdata->T_GD );
			xprintf ("     t_oc = % -12g . ", sdata->t_oc );
			xprintf ("     a_f2 = % -12g", sdata->a_f2 );
			show_crlf();

			xprintf ("     a_f1 = % -12g . ", sdata->a_f1 );
			xprintf ("     a_f0 = % -12g . ", sdata->a_f0 );
			xprintf ("    SVacc = % -12g", sdata->SVacc );
			show_crlf();

			edata = &(nav_data->ephemeris);

			xprintf ("     IODE = % -12d . ", edata->IODE );
			xprintf ("fit_intvl = % -12d . ", edata->fit_interval );
			xprintf ("     C_rs = % -12g", edata->C_rs );
			show_crlf();

			xprintf ("  delta_n = % -12g . ", edata->delta_n );
			xprintf ("      M_0 = % -12g . ", edata->M_0 );
			xprintf ("     C_uc = % -12g", edata->C_uc );
			show_crlf();

			xprintf ("      ecc = % -12g . ", edata->e );
			xprintf ("     C_us = % -12g . ", edata->C_us );
			xprintf ("   sqrt_A = % -12g", edata->sqrt_A );
			show_crlf();

			xprintf ("     t_oe = % -12g . ", edata->t_oe );
			xprintf ("     C_ic = % -12g . ", edata->C_ic );
			xprintf ("  OMEGA_0 = % -12g", edata->OMEGA_0 );
			show_crlf();

			xprintf ("     C_is = % -12g . ", edata->C_is );
			xprintf ("      i_0 = % -12g . ", edata->i_0 );
			xprintf ("     C_rc = % -12g", edata->C_rc );
			show_crlf();

			xprintf ("    omega = % -12g . ", edata->omega );
			xprintf (" OMEGADOT = % -12g . ", edata->OMEGADOT );
			xprintf ("     IDOT = % -12g", edata->IDOT );
			show_crlf();

			xprintf ("     Axis = % -12g . ", edata->Axis );
			xprintf ("        n = % -12g . ", edata->n );
			xprintf ("    r1me2 = % -12g", edata->r1me2 );
			show_crlf();

			xprintf ("  OMEGA_n = % -12g . ", edata->OMEGA_n );
			xprintf ("   ODOT_n = % -12g", edata->ODOT_n );
			break;
		}
	}
	else {
		xprintf ("System data response:  ");
		xprintf ("%s %s  SV%2.2d",
			opsname[op_code], datname[data_type], sv_prn);
	}
}


/* 0x59: */
static void rpt_SVs_enabled (TSIPPKT *rpt)
{
	byte
		code_type, status_code[32];
	int
		iprn;

	/* unload rptbuf */
	if (!rpt_0x59 (rpt, &code_type, status_code)) {
		bad_len (0x59, rpt->cnt);
		return;
	}

	if (code_type == 3) {
		xprintf ("SVs Enable/Disable");
		show_crlf ();

		for (iprn=0; iprn<32; iprn++) {
			xprintf ("%c%c", (iprn%10)?' ':'.',
				status_code[iprn]?'D':'E');
		}
	}
	if (code_type == 6) {
		xprintf ("SVs Heed/Ignore Health");
		show_crlf ();

		for (iprn=0; iprn<32; iprn++) {
			xprintf ("%c%c", (iprn%10)?' ':'.',
				status_code[iprn]?'I':'H');
		}
	}
}


/* 0x5A */
static void rpt_raw_msmt (TSIPPKT *rpt)
{
	byte
		sv_prn;
	float
		sample_length, signal_level, code_phase, Doppler;
	double
		time_of_fix;

	/* unload rptbuf */
	if (!rpt_0x5A (rpt, &sv_prn, &sample_length, &signal_level,
		&code_phase, &Doppler, &time_of_fix)) {
		bad_len (0x5A, rpt->cnt);
		return;
	}

	xprintf ("SV%02d %5.0f %7.1f %10.2f %10.2f %12.3f",
		sv_prn, sample_length, signal_level, code_phase, Doppler, time_of_fix);
	show_time (time_of_fix);
}

/* 0x5B */
static void rpt_SV_ephemeris_status (TSIPPKT *rpt)
{
	byte
		sv_prn, sv_health, sv_iode, fit_interval_flag;
	float
		time_of_collection, time_of_eph, sv_accy;

	/* unload rptbuf */
	if (!rpt_0x5B (rpt, &sv_prn, &sv_health, &sv_iode, &fit_interval_flag,
		&time_of_collection, &time_of_eph, &sv_accy)) {
		bad_len (0x5B, rpt->cnt);
		return;
	}

	/* rptbuf unloaded */

	xprintf ("    %2d  ", sv_prn);
	show_time (time_of_collection);
	xprintf ("   %02X     %02X ", sv_health, sv_iode);
	show_time (time_of_eph);
	xprintf ("   %1d   %4.1f", fit_interval_flag, sv_accy);
}

/* 0x5C */
static void rpt_SV_tracking_status (TSIPPKT *rpt)
{
	byte
		sv_prn, chan, slot, acq_flag, eph_flag,
		old_msmt_flag, integer_msec_flag, bad_data_flag,
		data_collect_flag;
	float
		signal_level, time_of_last_msmt,
		elev, azim;

	/* unload rptbuf */
	if (!rpt_0x5C (rpt,
		&sv_prn, &slot, &chan, &acq_flag, &eph_flag,
		&signal_level, &time_of_last_msmt, &elev, &azim,
		&old_msmt_flag, &integer_msec_flag, &bad_data_flag,
		&data_collect_flag)) {
		bad_len (0x5C, rpt->cnt);
		return;
	}

	xprintf ("   %2d  %1d %1d  %1d   %1d   %4.1f ",
		sv_prn, chan, slot, acq_flag, eph_flag, signal_level);
	show_time (time_of_last_msmt);
	xprintf ("  %5.1f  %5.1f   %d    %d    %d    %d",
		elev*R2D, azim*R2D, old_msmt_flag,
		integer_msec_flag, bad_data_flag, data_collect_flag);
}

/* 0x5E */
static void rpt_addl_fix_status (TSIPPKT *rpt)
{
	byte
		num_reused_msmts,
		diff_Doppler_unavailable,
		fix_still_converging,
		num_old_msmts;


	/* unload rptbuf */
	if (!rpt_0x5E (rpt,
		&num_reused_msmts, &diff_Doppler_unavailable,
		&fix_still_converging, &num_old_msmts)) {
		bad_len (0x5E, rpt->cnt);
		return;
	}

	xprintf ("%3d measmts re-used", num_reused_msmts);
	show_crlf();
	xprintf ("%3d measmts old", num_old_msmts);
	show_crlf();
	xprintf (diff_Doppler_unavailable?
		"differential Doppler unavailable":
		"differential Doppler available");
	show_crlf();
	xprintf (fix_still_converging?
		"Fix converging ":"Fix converged ");
}

/**/
/* 0x6D */
static void rpt_allSV_selection (TSIPPKT *rpt)
{
	byte
		manual_mode, nsvs, sv_prn[8], ndim;
	int
		islot;
	float
		pdop, hdop, vdop, tdop;

	/* unload rptbuf */
	if (!rpt_0x6D (rpt,
		&manual_mode, &nsvs, &ndim, sv_prn,
		&pdop, &hdop, &vdop, &tdop)) {
		bad_len (0x6D, rpt->cnt);
		return;
	}

	xprintf (manual_mode?"Mode:  Manual ":"Mode:    Auto ");
	xprintf (" %d Sat (%d-D) :", nsvs, ndim-1);
	for (islot=0; islot<nsvs; islot++) if (sv_prn[islot]) {
		xprintf (" %2d", sv_prn[islot]);
	}
	show_crlf();
	xprintf ("  P,H,V,TDOP = ");
	xprintf ("%.2f %.2f %.2f %.2f", pdop, hdop, vdop, tdop);
}

/**/
#ifndef POSFILT
/* 0x72 */
static void show_pos_filter (TSIPPKT *rpt)
{
	int
		pf;
	int
		dumint1, dumint2, dumint3, dumint4, dumint5;
	float 
		dumflt1, dumflt2, dumflt3, dumflt4, dumflt5;
	
	if (!rpt_0x72 (rpt,
		&pf, 
		&dumflt1, &dumint1, &dumint2, &dumint3, &dumint4,
		&dumflt2, &dumflt3, &dumflt4, &dumflt5,	&dumint5)) {
		bad_len (0x72, rpt->cnt);
		return;
	}

	xprintf ("Position filter %s", pf?"enabled":"disabled");
	show_crlf();
	xprintf(" %.5f %d %d %d %d %.3f %.0f %.3f %.0f %d", 
		dumflt1, dumint1, dumint2, dumint3, dumint4, 
		dumflt2, dumflt3, dumflt4, dumflt5, dumint5);
}
#endif

/* 0x74 */
static void show_altitude_filter (TSIPPKT *rpt)
{
	float 
		alt_filter_parm;

	/* unload rptbuf */
	if (!rpt_0x74 (rpt, &alt_filter_parm)) {
		bad_len (0x74, rpt->cnt);
		return;
	}

	/* changed, v1-11 */
	if (alt_filter_parm == -1.0) {
/*	if (alt_filter_parm == 0.0) {*/
		xprintf ("Altitude filter disabled");
	}
	else {
		xprintf("Alt filter max N = %4.0f fixes", alt_filter_parm);
	}
}

/* 0x76 */
static void show_high_6_mode (TSIPPKT *rpt)
{
	static char 
		*trackmode_text[2] = {"Track high 8, best-4 fix",
		"Track high 6, over-determined fix"};
	byte 
		trackmode;

	/* unload rptbuf */
	if (!rpt_0x76 (rpt, &trackmode)) {
		bad_len (0x76, rpt->cnt);
		return;
	}

	xprintf("Current tracking mode: %s", trackmode_text[trackmode]);
}

/* 0x78 */
static void rpt_DC_maxage (TSIPPKT *rpt)
{
	int 
		max_age;

	/* unload rptbuf */
	if (!rpt_0x78 (rpt, &max_age)) {
		bad_len (0x78, rpt->cnt);
		return;
	}

	xprintf ("Differential Correction Max Age:   %3d sec.", max_age);
}

/**/
/* 0x82 */
static void rpt_DGPS_position_mode (TSIPPKT *rpt)
{
	byte
		diff_mode;
	
	/* unload rptbuf */
	if (!rpt_0x82 (rpt, &diff_mode)) {
		bad_len (0x82, rpt->cnt);
		return;
	}

	xprintf ("DGPS mode is %d, ", diff_mode);
	switch (diff_mode) {
	case 0:
		xprintf ("diff off");
		break;
	case 1:
		xprintf ("report diff fixes only");
		break;
	case 2:
		xprintf ("auto diff, diff off");
		break;
	case 3:
		xprintf ("auto diff, diff on");
		break;
	}
}

/* 0x83 */
static void rpt_double_ECEF_position (TSIPPKT *rpt)
{

	double
		ECEF_pos[3], clock_bias;
	float
		time_of_fix;

	/* unload rptbuf */
	if (!rpt_0x83 (rpt, ECEF_pos, &clock_bias, &time_of_fix)) {
		bad_len (0x83, rpt->cnt);
		return;
	}

	xprintf ("Dbl Pos:");
	xprintf("%12.2f%13.2f%13.2f%12.2f",
		ECEF_pos[0], ECEF_pos[1], ECEF_pos[2], clock_bias);
	show_time (time_of_fix);
}

/* 0x84 */
static void rpt_double_lla_position (TSIPPKT *rpt)
{
	int
		lat_deg, lon_deg;
	double
		lat, lon, lat_min, lon_min,
		alt, clock_bias;
	float
		time_of_fix;
	byte
		north_south, east_west;

	/* unload rptbuf */
	if (!rpt_0x84 (rpt,
		&lat, &lon, &alt, &clock_bias, &time_of_fix)) {
		bad_len (0x84, rpt->cnt);
		return;
	}

	lat *= R2D;
	lon *= R2D;
	if (lat < 0.0) {
		north_south = 'S';
		lat = -lat;
	} else {
		north_south = 'N';
	}
	lat_deg = (int)lat;
	lat_min = (lat - lat_deg) * 60.0;

	if (lon < 0.0) {
		east_west = 'W';
		lon = -lon;
	} else {
		east_west = 'E';
	}
	lon_deg = (int)lon;
	lon_min = (lon - lon_deg) * 60.0;

	xprintf ("Position:");
	xprintf ("%4d %6.3f %c", lat_deg, lat_min, north_south);
	xprintf ("%5d %6.3f %c", lon_deg, lon_min, east_west);
	xprintf ("%10.2f%12.2f", alt, clock_bias);
	show_time (time_of_fix);
}

/* 0x85 */
static void rpt_dgps_info (TSIPPKT *rpt)
{
	static char
		*dm_text[6] = {"Good", "Delt", "StaH", " Age", "UDRE", "IODE"  };
	byte
		sv_prn, summary_status, station_health, SV_UDRE_parm,
		iode1, iode2;
	float
		z_cnt_tow, range_corr, range_rate_corr, delta_range_corr;

	/* unload rptbuf */
	if (!rpt_0x85 (rpt,
		&sv_prn, &summary_status, &station_health,
		&SV_UDRE_parm, &iode1, &iode2,
		&z_cnt_tow, &range_corr,
		&range_rate_corr, &delta_range_corr)) {
		bad_len (0x85, rpt->cnt);
		return;
	}

	xprintf ("    %2d  %d=%s    %d    %d    %02X    %02X  ", 
		sv_prn, summary_status, 
		(summary_status < 6)?dm_text[summary_status]:"Unkn",
		station_health, SV_UDRE_parm, iode1, iode2);

	show_time (z_cnt_tow);
	xprintf (" %6.1f", range_corr);
	xprintf (" %6.1f", range_rate_corr);
	xprintf (" %6.1f", delta_range_corr);
}

static void rpt_chb_parameters ( TSIPPKT *rpt)
/* Channel B configuration for dual port operation */	
{
	static char
		baudnum[10] = { 0, 1, 4, 5, 6, 8, 9, 11, 12, 28 },

		*parity_text [5] = {"even", "odd", "", "", "no"},

		*baud_text[11] = { "50 bps", "110 bps", "300 bps", "600 bps",
		"1200 bps", "2400 bps", "4800 bps", "9600 bps",
		"38.4 Kbps", "19.2 Kbps", "undefined"};

	byte tx_baud_index, rx_baud_index, char_format_index, stop_bits;
	byte databits, parity;
	int i;

	if (rpt->cnt != 5) {
		bad_len (0x8F, rpt->cnt);
		return;
		}
	tx_baud_index = rpt->buf[1];
	rx_baud_index = rpt->buf[2];
	char_format_index = rpt->buf[3];
	stop_bits = (rpt->buf[4] == 0x07) ? 1 : 2;

	xprintf ("Channel B Configuration");
	show_crlf();

	/* 10 baud rate choices */
	for(i=0; i<10; ++i)	if (tx_baud_index == baudnum[i]) break;
	xprintf ("Transmit at %s Baud", baud_text[i]);
	show_crlf ();

	/* 10 baud rate choices */
	for(i=0; i<10; ++i)	if (rx_baud_index == baudnum[i]) break;
	xprintf("Receive at %s Baud",	baud_text[i]);
	show_crlf ();

	databits = (char_format_index & 0x03) + 5;
	parity = char_format_index >> 2;
	if (parity > 4) parity = 2;

	xprintf("Character format (bits/char, parity, stop bits) is:  %d-%s-%d",
		databits, parity_text[parity], stop_bits);
}

/* 0x8F15 */
static void rpt_datum (TSIPPKT *rpt)
{

	double
		datum_coeffs[5];
	int
		datum_idx;

	/* unload rptbuf */
	if (!rpt_0x8F15 (rpt, &datum_idx, datum_coeffs)) {
		bad_len (0x8F, rpt->cnt);
		return;
	}

	xprintf ("Datum Index %2d ", datum_idx);
	show_crlf ();
	xprintf ("   dx    dy     dz       a-axis          e-squared");
	show_crlf ();
	xprintf("%6.1f %6.1f %6.1f %10.3f %16.14f", datum_coeffs[0], datum_coeffs[1], 
			datum_coeffs[2], datum_coeffs[3],datum_coeffs[4]);
}


/**/
void rpt_packet (TSIPPKT *rpt)
{
	/* last_rptcode is to determine whether to print a header */
	/* for the first occurence of a series of reports */
	static byte
		last_rptcode=0;

	show_crlf();
	
	switch (rpt->code) {

	case 0x3D:
		show_chan_A_config (rpt);
		break;

	case 0x40:
		rpt_almanac_data_page (rpt);
		break;

	case 0x41:
		rpt_GPS_time (rpt);
		break;

	case 0x42:
		rpt_float_ECEF_position (rpt);
		break;

	case 0x43:
		rpt_float_ECEF_velocity (rpt);
		break;

	case 0x44:
		rpt_4SV_selection (rpt);
		break;

	case 0x45:
		rpt_SW_version (rpt);
		break;

	case 0x46:
		rpt_rcvr_health (rpt);
		break;

	case 0x47:
		rpt_SNR_all_SVs (rpt);
		break;

	case 0x48:
		rpt_GPS_system_message (rpt);
		break;

	case 0x49:
		rpt_almanac_health_page (rpt);
		break;

	case 0x4A:
		rpt_float_lla_position (rpt);
		break;

	case 0x4B:
		rpt_rcvr_id_and_status (rpt);
		break;

	case 0x4C:
		rpt_operating_parameters (rpt);
		break;

	case 0x4D:
		rpt_oscillator_offset (rpt);
		break;

	case 0x4E:
		rpt_GPS_time_set_response (rpt);
		break;

	case 0x4F:
		rpt_UTC_offset (rpt);
		break;

	case 0x54:
		rpt_1SV_bias (rpt);
		break;

	case 0x55:
		rpt_io_opt (rpt);
		break;

	case 0x56:
		rpt_ENU_velocity (rpt);
		break;

	case 0x57:
		rpt_last_fix_info (rpt);
		break;

	case 0x58:
		rpt_GPS_system_data (rpt);
		break;

	case 0x59:
		rpt_SVs_enabled (rpt);
		break;

	case 0x5A:
		rpt_raw_msmt (rpt);
		break;

	case 0x5B:
		if (last_rptcode != 0x5B) {
			/* supply a header in console output */
			xprintf ("Ephemeris Status");
			show_crlf ();
			xprintf (
"    SV     Time collected    Health  IODE        t oe          Fit   URA");
			show_crlf ();
		}
		rpt_SV_ephemeris_status (rpt);
		break;

	case 0x5C:
		if (last_rptcode != 0x5C) {
			/* supply a header in console output */
			xprintf ("Tracking Info");
			show_crlf ();
			xprintf ("   SV  C S Acq Eph   SNR");
			xprintf (
				"     Time of meas     Elev    Azim   Old Msec BadD DCol");
			show_crlf ();
		}
		rpt_SV_tracking_status (rpt);
		break;

	case 0x5E:
		rpt_addl_fix_status (rpt);
		break;

	case 0x6D:
		rpt_allSV_selection (rpt);
		break;

	case 0x72:
		show_pos_filter (rpt);
		break;

	case 0x74:
		show_altitude_filter (rpt);
		break;

	case 0x76:
		show_high_6_mode (rpt);
		break;

	case 0x78:
		rpt_DC_maxage (rpt);
		break;

	case 0x82:
		rpt_DGPS_position_mode (rpt);
		break;

	case 0x83:
		rpt_double_ECEF_position (rpt);
		break;

	case 0x84:
		rpt_double_lla_position (rpt);
		break;

	case 0x85:
		if (last_rptcode != 0x85) {
			/* supply a header in console output */
			xprintf ("Differential Correction Status");
			show_crlf ();
			xprintf (
"    SV  Status  StaH  SV H  IODE1  IODE2  Z-count time        PRC    RRC    DRC");
			show_crlf ();
		}
		rpt_dgps_info (rpt);
		break;

	case 0x8F:
		switch (rpt->buf[0])	{
			case 0x03:   /* Subcode 3 is Channel B parameters */
				rpt_chb_parameters(rpt);
				break;
			case 0x15:   /* Subcode 15 is Datum parameters */
				rpt_datum(rpt);
				break;
			default:
				xprintf ("Unknown sub-packet ID %02X, length %d",
					rpt->buf[0], rpt->cnt);

		}
		break;

	default:
#ifdef TNL
		/* returns TRUE if recognized */
		if (tnl_rpt (rpt)) break;
#endif
		if (last_rptcode != 0)
			xprintf ("Unknown packet ID %02X, length %d",
				rpt->code, rpt->cnt);
		break;
	}

	last_rptcode = rpt->code;
}
