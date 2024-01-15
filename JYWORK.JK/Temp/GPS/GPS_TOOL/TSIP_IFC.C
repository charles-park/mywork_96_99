/*
	
Vers	Date		Changes										Author
----	--------	----------------------------------------	----------
1.1		21 Jun 93	initial version								P Loomis

TSIP_IFC.C is a series of interface routines that prepare commands to
be sent to the receiver and interprets reports sent form the receiver.
These routines are effectively primitives and are recommended for all 
platforms using the C language.  Some of the routines contained in this 
file are DOS-specific; they deal with serial ports, and are at the end of 
the file.

Declarations of the functions are included at the head of this file to provide
prototyping definitions.

A.  Command routines:

These handle packets going into the receiver.  The following routine is 
called from the main routine:

	SEND_CMD supplies DLE stuffing to a command string and sends the 
	command to the receiver using the primitive sendb().

The routines cmd_0x?? are command string generators patterned after the 
documentation in the TSIP manual.  They are called from a command input 
routine such as PROC_KBD() in TSIP_CMD.C.  There are more routines
than command codes.  If a TSIP command code has different argument
lists depending on optional operating modes, it has been separated
into a number of separate command routines according to the following
suffix convention:
	cmd_0x??q - query for current value;
	cmd_0x??c - clear value (0x1D, 0x73 only);
	cmd_0x??s - set new values;
	cmd_0x??e - enable:
	cmd_0x??d - disable;
	cmd_0x??g - get (0x38g only);
	cmd_0x??l - load (0x38g only).
This allows each routine to serve a unique purpose and cleans up the argument
lists somewhat.

B. Report routines:

These handle packets coming from the receiver.  The following 
four routines are called in the main routine (TSIPTALK.C or TSIPPRNT.C):

	ACCUMULATE_RPTBUF accumulates characters into rpt.buf from the
	primitive getb(); returns FALSE if no character is waiting in the
	buffer.

	END_OF_RPTBUF checks to see if the report end sequence DLE-ETX 
	has been received; returns TRUE at end of packet.

	UNSTUFF_RPTBUF removes all excess DLE "stuffing" in rpt.buf; 
	returns FALSE if error is found.

	RESET_RPTBUF sets rpt.buf to zero after report has been nterpreted.
	
The routines rpt_0x?? are report-string interpreters patterned after the 
documentation in the TSIP manual.  They are called by a report output routine 
such as RPT_PACKET() in TSIP_RPT.C.  If the report buffer is sent in
with the wrong length, the routine returns FALSE.  */

#include <string.h>
#include <stdio.h>
#include "tsipincl.h"

/**/
#ifdef PORT_INPUT
void send_cmd (TSIPPKT *cmd)
/* formats a command for sending to TSIP receiver */
{
	int i;

	sendb (DLE);
	sendb (cmd->code);
	for (i=0; i < cmd->cnt; i++) {
		if (cmd->buf[i] == DLE) sendb (DLE);
		sendb (cmd->buf[i]);
	}
	sendb (DLE);
	sendb (ETX);
}

void bputfloat (float a, byte *cmdbuf)
/* puts a float in a command string */
{
	union {byte bd[4]; float fv;} uval;

	uval.fv = a;
	*cmdbuf++ = uval.bd[3];
	*cmdbuf++ = uval.bd[2];
	*cmdbuf++ = uval.bd[1];
	*cmdbuf++ = uval.bd[0];
}

void bputdouble (double a, byte *cmdbuf)
/* puts a double in a command string */
{
	union {byte bd[8]; double dv;} uval;

	uval.dv = a;
	*cmdbuf++ = uval.bd[7];
	*cmdbuf++ = uval.bd[6];
	*cmdbuf++ = uval.bd[5];
	*cmdbuf++ = uval.bd[4];
	*cmdbuf++ = uval.bd[3];
	*cmdbuf++ = uval.bd[2];
	*cmdbuf++ = uval.bd[1];
	*cmdbuf++ = uval.bd[0];
}
/**/
void cmd_0x1Dc (TSIPPKT *cmd)
/* clear oscillator offset */
{
	cmd->buf[0] = 0x43;
	cmd->cnt = 1;
	cmd->code = 0x1D;
}

void cmd_0x1Ds (TSIPPKT *cmd,
	float offset)
/* set oscillator offset */
{	
	bputfloat (offset, &cmd->buf[0]);
	cmd->cnt = 4;
	cmd->code = 0x1D;
}

void cmd_0x1E (TSIPPKT *cmd)
/* clear battery back-up, then reset */
{
	cmd->buf[0] = 'K';
	cmd->cnt = 1;
	cmd->code = 0x1E;
}

void cmd_0x1F (TSIPPKT *cmd)
/* request software versions */
{
	cmd->cnt = 0;
	cmd->code = 0x1F;
}

/**/
void cmd_0x20 (TSIPPKT *cmd,
	byte sv_prn)
/* request almanac */	
{
	cmd->buf[0] = sv_prn;
	cmd->cnt = 1;
	cmd->code = 0x20;
}

void cmd_0x21 (TSIPPKT *cmd)
/* request current time */
{
	cmd->cnt = 0;
	cmd->code = 0x21;
}

void cmd_0x22 (TSIPPKT *cmd,
	byte nav_mode)
/* select position fix mode (2D, 3D, ...) */
{
	cmd->buf[0] = nav_mode;
	cmd->cnt = 1;
	cmd->code = 0x22;
}

void cmd_0x23 (TSIPPKT *cmd,
	float pos_ECEF[3])
/* initial position in ECEF coordinates */	
{
	bputfloat (pos_ECEF[0], &cmd->buf[0]);
	bputfloat (pos_ECEF[1], &cmd->buf[4]);
	bputfloat (pos_ECEF[2], &cmd->buf[8]);
	cmd->cnt = 12;
	cmd->code = 0x23;
}

void cmd_0x24 (TSIPPKT *cmd)
/* request position fix mode */
{
	cmd->cnt = 0;
	cmd->code = 0x24;
}

void cmd_0x25 (TSIPPKT *cmd)
/* initiate soft reset & self-test (equivalent to power cycle) */
{
	cmd->cnt = 0;
	cmd->code = 0x25;
}

void cmd_0x26 (TSIPPKT *cmd)
/* request receiver health */
{
	cmd->cnt = 0;
	cmd->code = 0x26;
}

void cmd_0x27 (TSIPPKT *cmd)
/* request signal levels */
{
	cmd->cnt = 0;
	cmd->code = 0x27;
}

void cmd_0x28 (TSIPPKT *cmd)
/* request GPS system message */
{
	cmd->cnt = 0;
	cmd->code = 0x28;
}

void cmd_0x29 (TSIPPKT *cmd)
/* request almanac health page */
{
	cmd->cnt = 0;
	cmd->code = 0x29;
}

void cmd_0x2A (TSIPPKT *cmd,
	float alt)
/* altitude for 2-D mode */	
{
	bputfloat (alt, &cmd->buf[0]);
	cmd->cnt = 4;
	cmd->code = 0x2A;
}

void cmd_0x2B (TSIPPKT *cmd,
	float lat, float lon, float alt)
/* initial position (latitude-longitude-altitude) */	
{
	bputfloat (lat, &cmd->buf[0]);
	bputfloat (lon, &cmd->buf[4]);
	bputfloat (alt, &cmd->buf[8]);
	cmd->cnt = 12;
	cmd->code = 0x2B;
}

void cmd_0x2Cq (TSIPPKT *cmd)
/* request operating parameters and masks */
{
	cmd->buf[0] = 0;
	bputfloat (-1.0, &cmd->buf[1]);
	bputfloat (-1.0, &cmd->buf[5]);
	bputfloat (-1.0, &cmd->buf[9]);
	bputfloat (-1.0, &cmd->buf[13]);
	cmd->cnt = 17;
	cmd->code = 0x2C;
}

void cmd_0x2Cs (TSIPPKT *cmd,
	byte dyn_code, 
	float elev_mask, float snr, float pdop_mask, float pdop_switch)
/* set operating parameters and masks */
{
	cmd->buf[0] = dyn_code;
	bputfloat (elev_mask, &cmd->buf[1]);
	bputfloat (snr, &cmd->buf[5]);
	bputfloat (pdop_mask, &cmd->buf[9]);
	bputfloat (pdop_switch, &cmd->buf[13]);
	cmd->cnt = 17;
	cmd->code = 0x2C;
}

void cmd_0x2D (TSIPPKT *cmd)
/* request oscillator offset */
{
	cmd->cnt = 0;
	cmd->code = 0x2D;
}

void cmd_0x2E (TSIPPKT *cmd,
	float time_of_week, int week_num)
/* set GPS time */	
{
	bputfloat (time_of_week, &cmd->buf[0]);
	cmd->buf[4] = (week_num >> 8);
	cmd->buf[5] = week_num;
	cmd->cnt = 6;
	cmd->code = 0x2E;
}

void cmd_0x2F (TSIPPKT *cmd)
/* request UTC parameters */
{
	cmd->cnt = 0;
	cmd->code = 0x2F;
}

/**/
void cmd_0x31 (TSIPPKT *cmd,
	float ECEF_pos[3])
/* initial accurate position in ECEF coordinates */	
{
	bputfloat (ECEF_pos[0], &cmd->buf[0]);
	bputfloat (ECEF_pos[1], &cmd->buf[4]);
	bputfloat (ECEF_pos[2], &cmd->buf[8]);
	cmd->cnt = 12;
	cmd->code = 0x31;
}

void cmd_0x32 (TSIPPKT *cmd,
	float lat, float lon, float alt)
/* initial accurate position in latitude-longitude-altitude */	
{
	bputfloat (lat, &cmd->buf[0]);
	bputfloat (lon, &cmd->buf[4]);
	bputfloat (alt, &cmd->buf[8]);
	cmd->cnt = 12;
	cmd->code = 0x32;
}

void cmd_0x33 (TSIPPKT *cmd)
/* Request analog-to-digital readings.  /
/* This function is not supported on standard SVeeSix.*/
{
	cmd->cnt = 0;
	cmd->code = 0x32;
}

void cmd_0x34 (TSIPPKT *cmd,
	byte sv_prn)
/* choose satellite	for 1-SV mode */
{
	cmd->buf[0] = sv_prn;
	cmd->cnt = 1;
	cmd->code = 0x34;
}

void cmd_0x35q (TSIPPKT *cmd)
/* request serial I/O options */
{
	cmd->cnt = 0;
	cmd->code = 0x35;
}

void cmd_0x35s (TSIPPKT *cmd,
	byte pos_code, byte vel_code, byte time_code, byte opts_code)
/* set serial I/O options */	
{
	cmd->buf[0] = pos_code;
	cmd->buf[1] = vel_code;
	cmd->buf[2] = time_code;
	cmd->buf[3] = opts_code;
	cmd->cnt = 4;
	cmd->code = 0x35;
}

void cmd_0x36 (TSIPPKT *cmd,
	byte coord_frame, byte enable, float vel[3])
/* set velocity for acquisition aiding */	
{
	cmd->buf[0] = coord_frame;	/* 0 = ECEF, 1 = ENU */
	cmd->buf[1] = enable?0:1;
	bputfloat (vel[0], &cmd->buf[2]);
	bputfloat (vel[1], &cmd->buf[6]);
	bputfloat (vel[2], &cmd->buf[10]);
	cmd->cnt = 14;
	cmd->code = 0x36;
}

void cmd_0x37 (TSIPPKT *cmd)
/* request last position, velocity, and status */
{
	cmd->cnt = 0;
	cmd->code = 0x37;
}

void cmd_0x38q (TSIPPKT *cmd,
	byte data_type, byte sv_prn)
/* request GPS system data (binary) */	
{
	cmd->buf[0] = 1;
	cmd->buf[1] = data_type;
	cmd->buf[2] = sv_prn;
	cmd->cnt = 3;
	cmd->code = 0x38;
}

void cmd_0x38s (TSIPPKT *cmd,
	byte data_type, byte sv_prn, byte data_length, byte *databuf)
/* load GPS system data (binary) */	
{
	cmd->buf[0] = 2;
	cmd->buf[1] = data_type;
	cmd->buf[2] = sv_prn;
	cmd->buf[3] = data_length;
	memcpy (&cmd->buf[4], databuf, (int)data_length);
	cmd->cnt = (int)data_length + 4;
	cmd->code = 0x38;
}

void cmd_0x39 (TSIPPKT *cmd,
	byte op_code, byte sv_prn)
/* set or request enable/health-heed status of satellites */	
{
	cmd->buf[0] = op_code;
	cmd->buf[1] = sv_prn;
	cmd->cnt = 2;
	cmd->code = 0x39;
}

void cmd_0x3A (TSIPPKT *cmd,
	byte sv_prn)
/* request last code-phase/Doppler measurement */	
{
	cmd->buf[0] = sv_prn;
	cmd->cnt = 1;
	cmd->code = 0x3A;
}

void cmd_0x3B (TSIPPKT *cmd,
	byte sv_prn)
/* request satellite epehmeris status */	
{
	cmd->buf[0] = sv_prn;
	cmd->cnt = 1;
	cmd->code = 0x3B;
}

void cmd_0x3C (TSIPPKT *cmd,
	byte sv_prn)
/* request tracking status */	
{
	cmd->buf[0] = sv_prn;
	cmd->cnt = 1;
	cmd->code = 0x3C;
}

void cmd_0x3Dq (TSIPPKT *cmd)
/* request Channel A configuration for dual-port operation */
{
	cmd->cnt = 0;
	cmd->code = 0x3D;
}

void cmd_0x3Ds (TSIPPKT *cmd,
	byte baud_out, byte baud_inp, byte char_code,
	byte stopbitcode, byte output_mode, byte input_mode)
/* set Channel A configuration for dual-port operation */
{
	cmd->buf[0] = baud_out;		/* XMT baud rate */
	cmd->buf[1] = baud_inp;		/* RCV baud rate */
	cmd->buf[2] = char_code;	/* parity and #bits per byte */
	cmd->buf[3] = stopbitcode;	/* number of stop bits code */
	cmd->buf[4] = output_mode;	/* Ch. A transmission mode */
	cmd->buf[5] = input_mode;	/* Ch. A reception mode */
	cmd->cnt = 6;
	cmd->code = 0x3D;
}

void cmd_0x3E (TSIPPKT *cmd)
/* request auxiliary fix parameters */
{
	cmd->cnt = 0;
	cmd->code = 0x3E;
}

/**/
void cmd_0x62q (TSIPPKT *cmd)
/* query DGPS fix mode */
{
	cmd->buf[0] = 4;
	cmd->cnt = 1;
	cmd->code = 0x62;
}

void cmd_0x62s (TSIPPKT *cmd,
	byte dgps_mode)
/* set DGPS fix mode */
{
	cmd->buf[0] = dgps_mode;
	cmd->cnt = 1;
	cmd->code = 0x62;
}

void cmd_0x65 (TSIPPKT *cmd,
	byte sv_prn)
/* request satellite differential correction info */	
{
	cmd->buf[0] = sv_prn;
	cmd->cnt = 1;
	cmd->code = 0x65;
}

/**/
void cmd_0x71q (TSIPPKT *cmd)
/* request position-velocity filter parameters */
{
	cmd->cnt = 0;
	cmd->code = 0x71;
}

void cmd_0x71s (TSIPPKT *cmd, 
	byte pf, 
	float flt1, byte chr1, byte chr2, byte chr3, byte chr4,	
	float flt2, float flt3, float flt4, float flt5,	byte chr5)
/* enable position-velocity filter with default parameters */
{
	cmd->buf[0] = pf;
	bputfloat (flt1, &cmd->buf[1]);
	cmd->buf[5] = chr1;
	cmd->buf[6] = chr2;
	cmd->buf[7] = chr3;
	cmd->buf[8] = chr4;
	bputfloat (flt2, &cmd->buf[9]);
	bputfloat (flt3, &cmd->buf[13]);
	bputfloat (flt4, &cmd->buf[17]);
	bputfloat (flt5, &cmd->buf[21]);
	cmd->buf[25] = chr5;
	cmd->cnt = 26;
	cmd->code = 0x71;
}

void cmd_0x73q (TSIPPKT *cmd)
/* request altitude filter parameters */
{
	cmd->cnt = 0;
	cmd->code = 0x73;
}

void cmd_0x73s (TSIPPKT *cmd,
	float time_constant)
/* set altitude filter parameters */
{
	bputfloat (time_constant, &cmd->buf[0]);
	cmd->cnt = 4;
	cmd->code = 0x73;
}

void cmd_0x73d (TSIPPKT *cmd)
/* disable altitude filter */
{
	bputfloat (1.0, &cmd->buf[0]);
	cmd->cnt = 4;
	cmd->code = 0x73;
}

void cmd_0x73c (TSIPPKT *cmd)
/* set altitude filter parameters to default */
{
	bputfloat (0.0, &cmd->buf[0]);
	cmd->cnt = 4;
	cmd->code = 0x73;
}

void cmd_0x75q (TSIPPKT *cmd)
/* request high-six tracking mode status */
{
	cmd->cnt = 0;
	cmd->code = 0x75;
}

void cmd_0x75s (TSIPPKT *cmd, byte trackmode)
/* set high-six tracking mode */
{
	cmd->buf[0] = trackmode;
	cmd->cnt = 1;
	cmd->code = 0x75;
}

void cmd_0x77q (TSIPPKT *cmd)
/* request DC max age */
{
	cmd->cnt = 0;
	cmd->code = 0x77;
}

void cmd_0x77s (TSIPPKT *cmd, int maxage)
/* request DC max age */
{
	cmd->buf[0]= maxage >> 8;
	cmd->buf[1]= maxage & 0x00ff;
	cmd->cnt = 2;
	cmd->code = 0x77;
}

void cmd_0x8E03q (TSIPPKT *cmd)
/* request Channel B configuration for dual-port operation */
{
	cmd->cnt = 1;
	cmd->code = 0x8E;
	cmd->buf[0] = 3;		/* Subcode 3 is Chan B config */
}

void cmd_0x8E03s (TSIPPKT *cmd,
	byte baud_out, byte baud_inp, byte char_code,
	byte stopbitcode)
/* set Channel B configuration for dual-port operation */
{
	cmd->buf[0] = 3;  /*Subcode 3 */
	cmd->buf[1] = baud_out;		/* XMT baud rate */
	cmd->buf[2] = baud_inp;		/* RCV baud rate */
	cmd->buf[3] = char_code;	/* parity and #bits per byte */
	cmd->buf[4] = stopbitcode;	/* number of stop bits code */
	cmd->cnt = 5;
	cmd->code = 0x8E;
}

void cmd_0x8E15q (TSIPPKT *cmd)
/* request current datum parameters */
{
	cmd->cnt = 1;
	cmd->code = 0x8E;
	cmd->buf[0] = 0x15;		/* Subcode 0x15 is datum request */
}

void cmd_0x8E14s (TSIPPKT *cmd,
	int  datum_index, double datum_coeffs[5])
/* set Datum parameters */
{
	cmd->buf[0] = 0x14;  /*Subcode 14 */
	if (datum_index >= 0) {
		cmd->buf[1]= datum_index >> 8;
		cmd->buf[2]= datum_index & 0x00ff;
		cmd->cnt = 3;
		}
	else {
		bputdouble (datum_coeffs[0], &cmd->buf[1]);
		bputdouble (datum_coeffs[1], &cmd->buf[9]);
		bputdouble (datum_coeffs[2], &cmd->buf[17]);
		bputdouble (datum_coeffs[3], &cmd->buf[25]);
		bputdouble (datum_coeffs[4], &cmd->buf[33]);
		cmd->cnt = 41;
		}
	cmd->code = 0x8E;
}

#endif
/**/
int accumulate_rptbuf (TSIPPKT *rpt)
/* grabs characters off the serial port buffer and adds them to received */
/* character string. */
{
	int this_byte;

	/* pull byte off input serial buffer; returned as int */
	/*  get byte, if available  */
	this_byte = getb ();
	if (this_byte == -1) return FALSE;
	rpt->buf[rpt->cnt]  = (byte)this_byte;
	rpt->cnt++;
	return TRUE;
}

int end_of_rptbuf (TSIPPKT * rpt)
/* checks to see if a complete report has been received; */
/* end of report is signified by DLE ETX. */
{
	int
		i;

	if (rpt->buf[rpt->cnt-1] != ETX) return FALSE;

	/* End of packet byte ETX has been received.  To determine that it is
	not a data byte, check for previous DLE.  Since DLE DLE ETX is a 
	legitimate data byte sequence, check backward until a non-DLE is found.
	An even number of DLEs means the ETX is just a data byte. */
	
	for (i=2; i<rpt->cnt; i++) {
		if (rpt->buf[rpt->cnt-i] == DLE) continue;
		/* first non-DLE byte; return depends on even or odd */
		return (i%2)?TRUE:FALSE;
	}
	
	/* Came to start of packet; unless a null command (DLE ETX) was sent 
	to clear the line, an error has probably occurred.  By declaring
	end of packet, the burden of error detection is passed to the unstuff 
	routine and a new packet is started immediately. */
	
	return TRUE;
}

#define MAX_RPTBUF 256
int unstuff_rptbuf (TSIPPKT *rpt)
/* removes DLE stuffing from received character string. */
{
	int
		endcnt, outcnt, incnt;
	static byte
		have_dle;

	endcnt = rpt->cnt;

	/* check that end of packet sequence (DLE ETX) is last two bytes. */
	if (rpt->buf[endcnt-1] != ETX) return FALSE;
	if (rpt->buf[endcnt-2] != DLE) return FALSE;

	/* find start of packet: (non-DLE) (DLE) (idcode) */
	for (incnt=0, have_dle = FALSE; incnt < endcnt-2; incnt++) {
		if (rpt->buf[incnt] == DLE) {
			have_dle = !have_dle;
		}
		else {
			if (have_dle) break;
		}
	}
	
	if (incnt == endcnt-2) {
		/* returns idcode = 0x00 packet, clears the rpt structure */
		/* either a null DLE ETX packet or an error */
		return TRUE;
	}

	/* idcode is the first non-DLE byte after a DLE */
	rpt->code = rpt->buf[incnt];
	incnt++;
	have_dle = FALSE;
	for (outcnt=0; incnt<endcnt-2; incnt++) {
		if (rpt->buf[incnt] == DLE) {
			if (!have_dle) {
				have_dle = TRUE;
				continue;
			}
		}
		else {
			if (have_dle) {
				/* dle fault; will result in bad packet length */
				rpt->cnt = outcnt;
				return FALSE;
			}
		}
		have_dle = FALSE;
		if (outcnt < MAX_RPTBUF) {
			rpt->buf [outcnt++] = rpt->buf [incnt];
		}
	}
	rpt->cnt = outcnt;
	return TRUE;
}

void reset_rptbuf (TSIPPKT *rpt)
/* prepares for receipt of new report */
{
	rpt->cnt = 0;
	rpt->code = 0x00;
}

short int bgetint (byte *bp)
/* gets an integer from the report string */
{
	union {
		byte    bd[2];
		int     iv;
	}  uval;

	uval.bd[1] = *(bp++);
	uval.bd[0] = *bp;
	return uval.iv;
}

float bgetfloat (byte *bp)
/* gets a float from the report string */
{
	union {
		byte    bd[4];
		float   fv;
	}  uval;

	uval.bd[3] = *(bp++);
	uval.bd[2] = *(bp++);
	uval.bd[1] = *(bp++);
	uval.bd[0] = *bp;
	return uval.fv;
}

double bgetdouble (byte *bp)
/* gets a double from the report string */
{
	union {
		byte    bd[8];
		double  dv;
	}  uval;

	uval.bd[7] = *(bp++);
	uval.bd[6] = *(bp++);
	uval.bd[5] = *(bp++);
	uval.bd[4] = *(bp++);
	uval.bd[3] = *(bp++);
	uval.bd[2] = *(bp++);
	uval.bd[1] = *(bp++);
	uval.bd[0] = *bp;
	return uval.dv;
}
/**/
int rpt_0x3D (
	TSIPPKT *rpt,
	byte *tx_baud_index, byte *rx_baud_index, byte *char_format_index,
	byte *stop_bits, byte *tx_mode_index, byte *rx_mode_index)
/* Channel A configuration for dual port operation */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 6) return FALSE;
	*tx_baud_index = buf[0];
	*rx_baud_index = buf[1];
	*char_format_index = buf[2];
	*stop_bits = (buf[3] == 0x07) ? 1 : 2;
	*tx_mode_index = buf[4];
	*rx_mode_index = buf[5];
	return TRUE;
}
/**/
int rpt_0x40 (
	TSIPPKT *rpt,
	byte *sv_prn,
	int *week_num,
	float *t_zc, float *eccentricity, float *t_oa, float *i_0, 
	float *OMEGA_dot, float *sqrt_A, float *OMEGA_0, float *omega,
	float *M_0)
/* almanac data for specified satellite */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 39) return FALSE;
	*sv_prn = buf[0];
	*t_zc = bgetfloat (&buf[1]);
	*week_num = bgetint (&buf[5]);
	*eccentricity = bgetfloat (&buf[7]);
	*t_oa = bgetfloat (&buf[11]);
	*i_0 = bgetfloat (&buf[15]);
	*OMEGA_dot = bgetfloat (&buf[19]);
	*sqrt_A = bgetfloat (&buf[23]);
	*OMEGA_0 = bgetfloat (&buf[27]);
	*omega = bgetfloat (&buf[31]);
	*M_0 = bgetfloat (&buf[35]);
	return TRUE;
}

int rpt_0x41 (TSIPPKT *rpt,
	float *time_of_week, float *UTC_offset, 
	int *week_num)
/* GPS time */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 10) return FALSE;
	*time_of_week = bgetfloat (buf);
	*week_num = bgetint (&buf[4]);
	*UTC_offset = bgetfloat (&buf[6]);
	return TRUE;
}

int rpt_0x42 (TSIPPKT *rpt,
	float pos_ECEF[3], float *time_of_fix)
/* position in ECEF, single precision */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 16) return FALSE;
	pos_ECEF[0] = bgetfloat (buf);
	pos_ECEF[1]= bgetfloat (&buf[4]);
	pos_ECEF[2]= bgetfloat (&buf[8]);
	*time_of_fix = bgetfloat (&buf[12]);
	return TRUE;
}

int rpt_0x43 (TSIPPKT *rpt,
	float ECEF_vel[3], float *freq_offset, float *time_of_fix)
/* velocity in ECEF, single precision */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 20) return FALSE;
	ECEF_vel[0] = bgetfloat (buf);
	ECEF_vel[1] = bgetfloat (&buf[4]);
	ECEF_vel[2] = bgetfloat (&buf[8]);
	*freq_offset = bgetfloat (&buf[12]);
	*time_of_fix = bgetfloat (&buf[16]);
	return TRUE;
}

int rpt_0x44 (TSIPPKT *rpt,
	byte *manual_mode, byte *nsvs, byte sv_prn[4],
	float *pdop, float *hdop, float *vdop, float *tdop)
/* best-4 satellite selection for position fixes, PDOP, fix mode */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 21) return FALSE;
	*manual_mode = (buf[0] > 4);
	*nsvs = buf[0] & 0x07;
	sv_prn[0] = buf[1];
	sv_prn[1] = buf[2];
	sv_prn[2] = buf[3];
	sv_prn[3] = buf[4];
	*pdop = bgetfloat (&buf[5]);
	*hdop = bgetfloat (&buf[9]);
	*vdop = bgetfloat (&buf[13]);
	*tdop = bgetfloat (&buf[17]);
	return TRUE;
}

int rpt_0x45 (TSIPPKT *rpt,
	byte *major_nav_version, byte *minor_nav_version,
	byte *nav_day, byte *nav_month, byte *nav_year,
	byte *major_dsp_version, byte *minor_dsp_version,
	byte *dsp_day, byte *dsp_month, byte *dsp_year)
/* software versions */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 10) return FALSE;
	*major_nav_version = buf[0];
	*minor_nav_version = buf[1];
	*nav_day = buf[2];
	*nav_month = buf[3];
	*nav_year = buf[4];

	*major_dsp_version = buf[5];
	*minor_dsp_version = buf[6];
	*dsp_day = buf[7];
	*dsp_month = buf[8];
	*dsp_year = buf[9];
	return TRUE;
}
 
int rpt_0x46 (TSIPPKT *rpt,
	byte *status_code, byte *error_code)
/* receiver health and status */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 2) return FALSE;
	*status_code = buf[0];
	*error_code = buf[1];
	return TRUE;
}

int rpt_0x47 (TSIPPKT *rpt,
	byte *nsvs, byte *sv_prn, 
	float *snr)
/* signal levels for all satellites tracked */
{
	int isv;
	byte *buf;
	buf = rpt->buf;

	if (rpt->cnt != 1 + 5*buf[0]) return FALSE;
	*nsvs = buf[0];
	for (isv=0; isv<(*nsvs); isv++) {
		sv_prn[isv] = buf[5*isv + 1];
		snr[isv] = bgetfloat (&buf[5*isv + 2]);
	}
	return TRUE;
}

int rpt_0x48 (TSIPPKT *rpt,
	byte *message)
/* GPS system message */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 22) return FALSE;
	memcpy (message, buf, 22);
	message[22] = 0;
	return TRUE;
}

int rpt_0x49 (TSIPPKT *rpt,
	byte *sv_health)
/* health for all satellites from almanac health page */
{
	int i;
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 32) return FALSE;
	for (i=0; i<32; i++) sv_health [i]= buf[i];
	return TRUE;
}

int rpt_0x4A (TSIPPKT *rpt,
	float *lat, float *lon, float *alt,	
	float *clock_bias, float *time_of_fix)
/* position in lat-lon-alt, single precision */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 20) return FALSE;
	*lat = bgetfloat (buf);
	*lon = bgetfloat (&buf[4]);
	*alt = bgetfloat (&buf[8]);
	*clock_bias = bgetfloat (&buf[12]);
	*time_of_fix = bgetfloat (&buf[16]);
	return TRUE;
}

int rpt_0x4B (TSIPPKT *rpt,
	byte *machine_id, byte *status_1, byte *status_2)
/* machine ID code, status */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 3) return FALSE;
	*machine_id = buf[0];
	*status_1 = buf[1];
	*status_2 = buf[2];
	return TRUE;
}

int rpt_0x4C (TSIPPKT *rpt,
	byte *dyn_code, 
	float *el_mask, float *snr_mask, float *pdop_mask, float *pdop_switch)
/* operating parameters and masks */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 17) return FALSE;
	*dyn_code = buf[0];
	*el_mask = bgetfloat (&buf[1]);
	*snr_mask = bgetfloat (&buf[5]);
	*pdop_mask = bgetfloat (&buf[9]);
	*pdop_switch = bgetfloat (&buf[13]);
	return TRUE;
}

int rpt_0x4D (TSIPPKT *rpt,
	float *osc_offset)
/* oscillator offset */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 4) return FALSE;
	*osc_offset = bgetfloat (buf);
	return TRUE;
}

int rpt_0x4E (TSIPPKT *rpt,
	byte *response)
/* yes/no response to command to set GPS time */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 1) return FALSE;
	*response = buf[0];
	return TRUE;
}

int rpt_0x4F (TSIPPKT *rpt,
	double *a0, float *a1, float *time_of_data,
	int *dt_ls,	int *wn_t, int *wn_lsf, int *dn, int *dt_lsf)
/* UTC data */
{
	byte *buf;
	buf = rpt->buf;

	if (rpt->cnt != 26) return FALSE;
	*a0 = bgetdouble (buf);
	*a1 = bgetfloat (&buf[8]);
	*dt_ls = bgetint (&buf[12]);
	*time_of_data = bgetfloat (&buf[14]);
	*wn_t = bgetint (&buf[18]);
	*wn_lsf = bgetint (&buf[20]);
	*dn = bgetint (&buf[22]);
	*dt_lsf = bgetint (&buf[24]);
	return TRUE;
}
/**/
int rpt_0x53 (TSIPPKT *rpt,
	float *a2d)
/* Report analog-to-digital readings.  */
/* This function is not supported on standard SVee 6.  */
{
	byte *buf;
	int i;

	buf = rpt->buf;
	
	if (rpt->cnt != 32) return FALSE;

	for (i=0; i<8; i++) a2d[i] = bgetfloat (buf+4*i);
	return TRUE;
}

int rpt_0x54 (TSIPPKT *rpt,
	float *clock_bias, float *freq_offset, float *time_of_fix)
/* clock offset and frequency offset in 1-SV (0-D) mode */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 12) return FALSE;
	*clock_bias = bgetfloat (buf);
	*freq_offset = bgetfloat (&buf[4]);
	*time_of_fix = bgetfloat (&buf[8]);
	return TRUE;
}

int rpt_0x55 (TSIPPKT *rpt,
	byte *pos_code, byte *vel_code, byte *time_code, byte *aux_code)
/* I/O serial options */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 4) return FALSE;
	*pos_code = buf[0];
	*vel_code = buf[1];
	*time_code = buf[2];
	*aux_code = buf[3];
	return TRUE;
}

int rpt_0x56 (TSIPPKT *rpt,
	float vel_ENU[3], float *freq_offset, float *time_of_fix)
/* velocity in east-north-up coordinates */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 20) return FALSE;
	/* east */
	vel_ENU[0] = bgetfloat (buf);
	/* north */
	vel_ENU[1] = bgetfloat (&buf[4]);
	/* up */
	vel_ENU[2] = bgetfloat (&buf[8]);
	*freq_offset = bgetfloat (&buf[12]);
	*time_of_fix = bgetfloat (&buf[16]);
	return TRUE;
}

int rpt_0x57 (TSIPPKT *rpt,
	byte *source_code, byte *diag_code,
	int *week_num,
	float *time_of_fix)
/* info about last computed fix */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 8) return FALSE;
	*source_code = buf[0];
	*diag_code = buf[1];
	*time_of_fix = bgetfloat (&buf[2]);
	*week_num = bgetint (&buf[6]);
	return TRUE;
}

int rpt_0x58 (TSIPPKT *rpt,
	byte *op_code, byte *data_type, byte *sv_prn,
	byte *data_length, byte *data_packet)
/* GPS system data or acknowledgment of GPS system data load */	
{
	byte *buf, *buf4;
	buf = rpt->buf;

	if (buf[0] == 2) {
		if (rpt->cnt < 4) return FALSE;
		if (rpt->cnt != 4+buf[3]) return FALSE;
	}
	else if (rpt->cnt != 3) {
		return FALSE;
	}
	*op_code = buf[0];
	*data_type = buf[1];
	*sv_prn = buf[2];
	if (*op_code == 2) {
		*data_length = buf[3];
		buf4 = &buf[4];
		switch (*data_type) {
		case 2:
			/* Almanac */
			if (*data_length != sizeof (ALM_INFO)) return FALSE;
			((ALM_INFO*)data_packet)->t_oa_raw  = buf4[0];
			((ALM_INFO*)data_packet)->SV_health = buf4[1];
			((ALM_INFO*)data_packet)->e        = bgetfloat(&buf4[2]);
			((ALM_INFO*)data_packet)->t_oa     = bgetfloat(&buf4[6]);
			((ALM_INFO*)data_packet)->i_0      = bgetfloat(&buf4[10]);
			((ALM_INFO*)data_packet)->OMEGADOT = bgetfloat(&buf4[14]);
			((ALM_INFO*)data_packet)->sqrt_A   = bgetfloat(&buf4[18]);
			((ALM_INFO*)data_packet)->OMEGA_0  = bgetfloat(&buf4[22]);
			((ALM_INFO*)data_packet)->omega    = bgetfloat(&buf4[26]);
			((ALM_INFO*)data_packet)->M_0      = bgetfloat(&buf4[30]);
			((ALM_INFO*)data_packet)->a_f0     = bgetfloat(&buf4[34]);
			((ALM_INFO*)data_packet)->a_f1     = bgetfloat(&buf4[38]);
			((ALM_INFO*)data_packet)->Axis     = bgetfloat(&buf4[42]);
			((ALM_INFO*)data_packet)->n        = bgetfloat(&buf4[46]);
			((ALM_INFO*)data_packet)->OMEGA_n  = bgetfloat(&buf4[50]);
			((ALM_INFO*)data_packet)->ODOT_n   = bgetfloat(&buf4[54]);
			((ALM_INFO*)data_packet)->t_zc     = bgetfloat(&buf4[58]);
			((ALM_INFO*)data_packet)->weeknum  = bgetint(&buf4[62]);
			((ALM_INFO*)data_packet)->wn_oa    = bgetint(&buf4[64]);
			break;
		case 3:
			/* Almanac health page */
			if (*data_length != sizeof (ALH_PARMS)+3) return FALSE;

			/* this record is all bytes */
			memcpy (data_packet, buf, *data_length-2);
			data_packet[*data_length-2] = bgetint (&buf[*data_length-2]);
			break;
		case 4:
			/* Ionosphere */
			if (*data_length != sizeof (ION_INFO) + 8) return FALSE;
			((ION_INFO*)data_packet)->alpha_0 = bgetfloat (&buf4[8]);
			((ION_INFO*)data_packet)->alpha_1 = bgetfloat (&buf4[12]);
			((ION_INFO*)data_packet)->alpha_2 = bgetfloat (&buf4[16]);
			((ION_INFO*)data_packet)->alpha_3 = bgetfloat (&buf4[20]);
			((ION_INFO*)data_packet)->beta_0 = bgetfloat (&buf4[24]);
			((ION_INFO*)data_packet)->beta_1 = bgetfloat (&buf4[28]);
			((ION_INFO*)data_packet)->beta_2 = bgetfloat (&buf4[32]);
			((ION_INFO*)data_packet)->beta_3 = bgetfloat (&buf4[36]);
			break;
		case 5:
			/* UTC */
			if (*data_length != sizeof (UTC_INFO) + 13) return FALSE;
			((UTC_INFO*)data_packet)->A_0 = bgetdouble (&buf4[13]);
			((UTC_INFO*)data_packet)->A_1 = bgetfloat (&buf4[21]);
			((UTC_INFO*)data_packet)->delta_t_LS = bgetint (&buf4[25]);
			((UTC_INFO*)data_packet)->t_ot = bgetfloat(&buf4[27]);
			((UTC_INFO*)data_packet)->WN_t = bgetint (&buf4[31]);
			((UTC_INFO*)data_packet)->WN_LSF = bgetint (&buf4[33]);
			((UTC_INFO*)data_packet)->DN = bgetint (&buf4[35]);
			((UTC_INFO*)data_packet)->delta_t_LSF = bgetint (&buf4[37]);
			break;
		case 6:
			/* Ephemeris */
			if (*data_length != sizeof (NAV_INFO) - 1) return FALSE;

			((NAV_INFO*)data_packet)->sv_number = buf4[0];
			((NAV_INFO*)data_packet)->t_ephem = bgetfloat (&buf4[1]);

			((NAV_INFO*)data_packet)->subframe1.weeknum = bgetint (&buf4[5]);
			((NAV_INFO*)data_packet)->subframe1.codeL2 = buf4[7];
			((NAV_INFO*)data_packet)->subframe1.L2Pdata = buf4[8];
			((NAV_INFO*)data_packet)->subframe1.SVacc_raw = buf4[9];
			((NAV_INFO*)data_packet)->subframe1.SV_health = buf4[10];
			((NAV_INFO*)data_packet)->subframe1.IODC = bgetint (&buf4[11]);
			((NAV_INFO*)data_packet)->subframe1.T_GD = bgetfloat (&buf4[13]);
			((NAV_INFO*)data_packet)->subframe1.t_oc = bgetfloat (&buf4[17]);
			((NAV_INFO*)data_packet)->subframe1.a_f2 = bgetfloat (&buf4[21]);
			((NAV_INFO*)data_packet)->subframe1.a_f1 = bgetfloat (&buf4[25]);
			((NAV_INFO*)data_packet)->subframe1.a_f0 = bgetfloat (&buf4[29]);
			((NAV_INFO*)data_packet)->subframe1.SVacc = bgetfloat (&buf4[33]);

			((NAV_INFO*)data_packet)->ephemeris.IODE = buf4[37];
			((NAV_INFO*)data_packet)->ephemeris.fit_interval = buf4[38];
			((NAV_INFO*)data_packet)->ephemeris.C_rs = bgetfloat (&buf4[39]);
			((NAV_INFO*)data_packet)->ephemeris.delta_n = bgetfloat (&buf4[43]);
			((NAV_INFO*)data_packet)->ephemeris.M_0 = bgetdouble (&buf4[47]);
			((NAV_INFO*)data_packet)->ephemeris.C_uc = bgetfloat (&buf4[55]);
			((NAV_INFO*)data_packet)->ephemeris.e = bgetdouble (&buf4[59]);
			((NAV_INFO*)data_packet)->ephemeris.C_us = bgetfloat (&buf4[67]);
			((NAV_INFO*)data_packet)->ephemeris.sqrt_A = bgetdouble (&buf4[71]);
			((NAV_INFO*)data_packet)->ephemeris.t_oe = bgetfloat (&buf4[79]);
			((NAV_INFO*)data_packet)->ephemeris.C_ic = bgetfloat (&buf4[83]);
			((NAV_INFO*)data_packet)->ephemeris.OMEGA_0 = bgetdouble (&buf4[87]);
			((NAV_INFO*)data_packet)->ephemeris.C_is = bgetfloat (&buf4[95]);
			((NAV_INFO*)data_packet)->ephemeris.i_0 = bgetdouble (&buf4[99]);
			((NAV_INFO*)data_packet)->ephemeris.C_rc = bgetfloat (&buf4[107]);
			((NAV_INFO*)data_packet)->ephemeris.omega = bgetdouble (&buf4[111]);
			((NAV_INFO*)data_packet)->ephemeris.OMEGADOT = bgetfloat (&buf4[119]);
			((NAV_INFO*)data_packet)->ephemeris.IDOT = bgetfloat (&buf4[123]);
			((NAV_INFO*)data_packet)->ephemeris.Axis = bgetdouble (&buf4[127]);
			((NAV_INFO*)data_packet)->ephemeris.n = bgetdouble (&buf4[135]);
			((NAV_INFO*)data_packet)->ephemeris.r1me2 = bgetdouble (&buf4[143]);
			((NAV_INFO*)data_packet)->ephemeris.OMEGA_n = bgetdouble (&buf4[151]);
			((NAV_INFO*)data_packet)->ephemeris.ODOT_n = bgetdouble (&buf4[159]);
			break;
		}
	}
	return TRUE;
}

int rpt_0x59 (TSIPPKT *rpt,
	byte *code_type, byte status_code[32])
/* satellite enable/disable or health heed/ignore list */	
{
	int iprn;
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 33) return FALSE;

	*code_type = buf[0];
	for (iprn=0; iprn<32; iprn++)
		status_code[iprn] = buf[iprn+1];
	return TRUE;
}

int rpt_0x5A (TSIPPKT *rpt,
	byte *sv_prn,
	float *sample_length, float *signal_level,
	float *code_phase, float *Doppler,
	double *time_of_fix)
/* raw measurement data - code phase/Doppler */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 25) return FALSE;
	*sv_prn = buf[0];
	*sample_length = bgetfloat (&buf[1]);
	*signal_level = bgetfloat (&buf[5]);
	*code_phase = bgetfloat (&buf[9]);
	*Doppler = bgetfloat (&buf[13]);
	*time_of_fix = bgetdouble (&buf[17]);
	return TRUE;
}

int rpt_0x5B (TSIPPKT *rpt,
	byte *sv_prn, byte *sv_health, byte *sv_iode, byte *fit_interval_flag,
	float *time_of_collection, float *time_of_eph, float *sv_accy)
/* satellite ephemeris status */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 16) return FALSE;
	*sv_prn = buf[0];
	*time_of_collection = bgetfloat (&buf[1]);
	*sv_health = buf[5];
	*sv_iode = buf[6];
	*time_of_eph = bgetfloat (&buf[7]);
	*fit_interval_flag = buf[11];
	*sv_accy = bgetfloat (&buf[12]);
	return TRUE;
}

int rpt_0x5C (TSIPPKT *rpt,
	byte *sv_prn, byte *slot, byte *chan, byte *acq_flag, byte *eph_flag,
	float *signal_level, float *time_of_last_msmt, float *elev, float *azim,
	byte *old_msmt_flag, byte *integer_msec_flag, byte *bad_data_flag,
	byte *data_collect_flag)
/* satellite tracking status */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 24) return FALSE;
	*sv_prn = buf[0];
	*slot = buf[1] & 0x07;
	*chan = buf[1] >> 3;
	if (*chan == 0x10) *chan = 2;
	else (*chan) ++;
	*acq_flag = buf[2];
	*eph_flag = buf[3];
	*signal_level = bgetfloat (&buf[4]);
	*time_of_last_msmt = bgetfloat (&buf[8]);
	*elev = bgetfloat (&buf[12]);
	*azim = bgetfloat (&buf[16]);
	*old_msmt_flag = buf[20];
	*integer_msec_flag = buf[21];
	*bad_data_flag = buf[22];
	*data_collect_flag = buf[23];
	return TRUE;
}

int rpt_0x5E (TSIPPKT *rpt,
	byte *num_reused_msmts, byte *diff_Doppler_unavailable,
	byte *fix_still_converging, byte *num_old_msmts)
/* auxiliary fix status */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 2) return FALSE;
	*num_reused_msmts = buf[0] & 0x07;
	*diff_Doppler_unavailable = buf[0] & 0x08;
	*fix_still_converging = buf[0] & 0x10;
	*num_old_msmts = buf[1] & 0x07;
	return TRUE;
}
/**/
int rpt_0x6D (TSIPPKT *rpt,
	byte *manual_mode, byte *nsvs, byte *ndim, byte sv_prn[],
	float *pdop, float *hdop, float *vdop, float *tdop)
/* over-determined satellite selection for position fixes, PDOP, fix mode */
{
	int islot;
	byte *buf;
	buf = rpt->buf;

	*nsvs = (buf[0]&0xF0) >> 4;
	if ((*nsvs)>8) return FALSE;
	if (rpt->cnt != 17 + (*nsvs) ) return FALSE;

	*manual_mode = buf[0] & 0x08;
	*ndim  = (buf[0] & 0x07);
	*pdop = bgetfloat (&buf[1]);
	*hdop = bgetfloat (&buf[5]);
	*vdop = bgetfloat (&buf[9]);
	*tdop = bgetfloat (&buf[13]);
	for (islot=0; islot<(*nsvs); islot++)
		sv_prn[islot] = buf[islot+17];
	return TRUE;
}
/**/
int rpt_0x72 (TSIPPKT *rpt,
	int *pf, float *flt1, int *chr1, 
	int *chr2, int *chr3, int *chr4,
	float *flt2, float *flt3,
	float *flt4, float *flt5,
	int *chr5)
/* position-velocity filter parameters */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 32) return FALSE;

	*pf = bgetint (&buf[0]);
	*flt1 = bgetfloat (&buf[2]);
	*chr1 = bgetint (&buf[6]);
	*chr2 = bgetint (&buf[8]);
	*chr3 = bgetint (&buf[10]);
	*chr4 = bgetint (&buf[12]);

	*flt2 = bgetfloat(&buf[14]);
	*flt3 = bgetfloat(&buf[18]);
	*flt4 = bgetfloat(&buf[22]);
	*flt5 = bgetfloat(&buf[26]);
	*chr5 = bgetint(&buf[30]);
	return TRUE;
}

int rpt_0x74 (TSIPPKT *rpt,
	float *alt_filter_parm)
/* altitude filter parameters */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 4) return FALSE;
	*alt_filter_parm = bgetfloat (buf);
	return TRUE;
}

int rpt_0x76 (TSIPPKT *rpt,
	byte *mode_6)
/* high-6 mode parameters */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 1) return FALSE;
	*mode_6 = buf[0];
	return TRUE;
}
int rpt_0x78 (TSIPPKT *rpt,
	int *maxage)
/* DC max age parameters */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 2) return FALSE;
	*maxage = bgetint (&buf[0]);
	return TRUE;
}
/**/
int rpt_0x82 (TSIPPKT *rpt,
	byte *diff_mode)
/* differential fix mode */
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 1) return FALSE;
	*diff_mode = buf[0];
	return TRUE;
}

int rpt_0x83 (TSIPPKT *rpt,
	double ECEF_pos[3], double *clock_bias, float *time_of_fix)
/* position, ECEF double precision */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 36) return FALSE;
	ECEF_pos[0] = bgetdouble (buf);
	ECEF_pos[1] = bgetdouble (&buf[8]);
	ECEF_pos[2] = bgetdouble (&buf[16]);
	*clock_bias  = bgetdouble (&buf[24]);
	*time_of_fix = bgetfloat (&buf[32]);
	return TRUE;
}

int rpt_0x84 (TSIPPKT *rpt,
	double *lat, double *lon, double *alt,
	double *clock_bias, float *time_of_fix)
/* position, lat-lon-alt double precision */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 36) return FALSE;
	*lat = bgetdouble (buf);
	*lon = bgetdouble (&buf[8]);
	*alt = bgetdouble (&buf[16]);
	*clock_bias = bgetdouble (&buf[24]);
	*time_of_fix = bgetfloat (&buf[32]);
	return TRUE;
}

int rpt_0x85 (TSIPPKT *rpt,
	byte *sv_prn, byte *summary_status, byte *station_health,
	byte *SV_UDRE_parm, byte *iode1, byte *iode2,
	float *z_cnt_tow, float *range_corr,
	float *range_rate_corr, float *delta_range_corr)
/* satellite differential correction info */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 22) return FALSE;
	*sv_prn = buf[0];
	*summary_status = buf[1];
	*station_health = buf[2];
	*SV_UDRE_parm = buf[3];
	*iode1 = buf[4];
	*iode2 = buf[5];
	*z_cnt_tow = bgetfloat (&buf[6]);
	*range_corr = bgetfloat (&buf[10]);
	*range_rate_corr = bgetfloat (&buf[14]);
	*delta_range_corr = bgetfloat (&buf[18]);
	return TRUE;
}

int rpt_0x8F15 (TSIPPKT *rpt,
	int *datum_idx, double datum_coeffs[5])
/* datum index and coeffecients */	
{
	byte *buf;
	buf = rpt->buf;
	
	if (rpt->cnt != 43) return FALSE;
	*datum_idx = bgetint(&buf[1]);
	datum_coeffs[0] = bgetdouble (&buf[3]);
	datum_coeffs[1] = bgetdouble (&buf[11]);
	datum_coeffs[2] = bgetdouble (&buf[19]);
	datum_coeffs[3] = bgetdouble (&buf[27]);
	datum_coeffs[4] = bgetdouble (&buf[35]);
	return TRUE;
}
