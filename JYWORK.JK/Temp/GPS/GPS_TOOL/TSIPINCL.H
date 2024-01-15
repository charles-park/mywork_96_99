/*
	
Vers	Date		Changes										Author
----	--------	----------------------------------------	----------
1.1		21 Jun 93	initial version								P Loomis

The following files require TSIPINCL.H:
TSIPPRNT.C:  main program for binary to ASCII translation of TSIP files
TSIPCHAT.C:  main program for communicating with TSIP devices
TSIP_UTL.C:  user interface routines
TSIP_CMD.C:  command generator
TSIP_RPT.C:  report interpreter
TSIP_IFC.C:  standard command/report interface for TSIP
TSIP_ALM.C:  grabs almanacs from, and stuffs them into, receiver
TSIP_TNL.C, TSIP_SIO.C: proprietary rotuines.

This source code is supplied without warranty and is intended as
sample source code for exercising the TSIP interface.  We do
appreciate comments and try to support the code as much as possible.

Developers of new applications are encouraged to use the subroutines
in TSIP_IFC.C, and pattern their drivers on main() in TSIPCHAT.C

*/
/* if input is derived from a file, undefine real-time macros */
#ifdef FILE_INPUT

#undef SPDRIVE
#undef BORLAND
#undef PORT_INPUT

#endif


typedef unsigned char byte;

/* TSIP packets have the following structure, whether report or command. */
typedef struct {
	byte code;			/* TSIP code */
	byte buf[512];		/* report or command string; < 256 bytes unstuffed */
	int cnt;			/* size of buf */
} TSIPPKT;

/* data base for keyboard commands */
typedef struct {
	char kb_stroke[3];	/* description if keystroke (e.g. '^F') */
	char kbch;			/* ASCII code for keystroke */
	byte cmdcode;		/* TSIP command code */
	char cmdtext[18];	/* description of TSIP command */
} KB_CMD;

/* TSIP binary data structures */
typedef struct {
	byte    
		t_oa_raw, SV_health;
	float   
		e, t_oa, i_0, OMEGADOT, sqrt_A, 
		OMEGA_0, omega, M_0, a_f0, a_f1,
		Axis, n, OMEGA_n, ODOT_n, t_zc;
	short   
		weeknum, wn_oa;
} ALM_INFO;

typedef struct {     /*  Almanac health page (25) parameters  */
	byte    
		WN_a, SV_health[32], t_oa;
} ALH_PARMS;

typedef struct {     /*  Universal Coordinated Time (UTC) parms */
	double  
		A_0;
	float   
		A_1;
	short   
		delta_t_LS;
	float   
		t_ot;
	short   
		WN_t, WN_LSF, DN, delta_t_LSF;
} UTC_INFO;

typedef struct {      /*  Ionospheric info (float)  */
    float   
		alpha_0, alpha_1, alpha_2, alpha_3,
		beta_0, beta_1, beta_2, beta_3; 
} ION_INFO;

typedef struct {      /*  Subframe 1 info (float)  */
	short int    
		weeknum;
	byte    
		codeL2, L2Pdata, SVacc_raw, SV_health;
	short int    
		IODC;
	float
		T_GD, t_oc, a_f2, a_f1, a_f0, SVacc;
} SF1_INFO;

typedef	struct {     /*  Ephemeris info (float)  */
	byte    
		IODE, fit_interval;
	float   
		C_rs, delta_n;
	double  
		M_0;
	float   
		C_uc;
	double  
		e;
	float   
		C_us;
	double  
		sqrt_A;
	float   
		t_oe, C_ic;
	double  
		OMEGA_0;
	float   
		C_is;
	double  
		i_0;
	float   
		C_rc;
	double  
		omega;
	float   
		OMEGADOT, IDOT;
	double  
		Axis, n, r1me2, OMEGA_n, ODOT_n;
} EPH_INFO;

typedef struct {       /*  Navigation data strcuture  */
	short   
		sv_number;      /*  SV number (0 = no entry)         */
	float   
		t_ephem;        /*  time of ephemeris collection     */
	SF1_INFO 
		subframe1;      /* subframe 1 data       */
	EPH_INFO 
		ephemeris;      /* ephemeris data        */
} NAV_INFO;

#define FALSE 0
#define TRUE (!FALSE)

#define	D2R	(3.141592654/180.0)
#define	R2D	(180.0/3.141592654)

/* TSIP packet control characters */
#define DLE 0x10
#define ETX 0x03

/* If Borland is used, use console print; otherwise use stdout print */
#ifdef BORLAND
#define xprintf cprintf
#define xputs cputs
#else 
#define xprintf printf
#define xputs puts
#endif

/**/

/* This routine is called from main() in TSIPCHAT.C and from TSIP_UTL.C */
byte read_rpts_wait_for_kbhit 
	(void);

/* These routines are called from do_command() in TSIPCHAT.C */

/* in TSIP_CMD.C */
int interpret_keystroke
	(byte kbch, byte *cmdcode);
void proc_kbd
	(byte kch, TSIPPKT *cmd);
int reply_requested
	(byte cmdcode, int *reply);

/* These routines are called from do_command() and */
/* read_rpts_wait_for_kbhit() in TSIPCHAT.C, and from main() in TSIPPRNT.C */

/* in TSIP_IFC.C */
void send_cmd
	(TSIPPKT *cmd);
int accumulate_rptbuf
	(TSIPPKT *rpt);
int end_of_rptbuf
	(TSIPPKT *rpt);
int unstuff_rptbuf
	(TSIPPKT *rpt);
void reset_rptbuf 
	(TSIPPKT *rpt);

/* in TSIP_RPT.C */
void rpt_packet
	(TSIPPKT *rpt);

/********************** in TSIP_ALM.C *******************************/
/* These routines are called from TSIP_CMD.C */
int almgetb (void);
int almputb (void);

/************************** in TSIP_UTL.C **********************************/
/* Utility routines for console display */
/* put carriage return & linefeed */
void show_crlf
	(void);
/* print time of week as day, hour, minute, second */
void show_time
	(float time);
/* ask user for verification of choice */
int ask_verify
	(void);
/* prompt user to input a double */
double ask_dbl
	(char *prompt);
/* prompt user to input an integer */
byte ask_byte
	(char *prompt);
/*  prompt for an integer from user  */
int ask_int 
	(char *prompt);
/* prompt user to pick among a finite number of choices */
int pick_one
	(char *prompt, char *code_txt[], int nopts);
void ask_4_hex
	(char *prompt, int *hex1, int *hex2, int *hex3, int *hex4);

/* prints out TAIP long nav packets into a text file; called from  */
/* read_rpts_wait_for_kbhit() in TSIPCHAT.C */
void taip_output
	(TSIPPKT *rpt, FILE *taipstream);

/* dos routines to set time */
void request_PC_time_set 
	(void);
void GPS_time_from_PC 
	(double *time);
void GPS_time_to_PC 
	(double time);

/* BORLAND window routines */
void switch_to_cmd_window
	(void);
void switch_to_auto_window
	(void);
void close_cmd_window
	(void);
int which_window
	(void);
void initialize_screen
	(void);
void reset_screen
	(void);

/* file storage control */
/* returns TRUE if open, FALSE if closed */
void toggle_file_storage
	(FILE **stream, char *filename, short binasc);

/**/
/*************************** in TSIP_IFC.C ****************************/
/* These routines are low-level serial port access calls. */
/* They require SPDRIVE.COM to have been invoked previously. */
int getb
	(void);
void sendb
	(byte db);

/* These routines get items from a TSIP report packet character buffer. */
/* Byte order is reversed for DOS machines. */
short int bgetint
	(byte *bp);
float bgetfloat
	(byte *bp);
double bgetdouble
	(byte *bp);

/* This routine puts floats into a TSIP command packet character buffer */
/* Byte order is reversed for DOS machines. */
void bputfloat
	(float a, byte *cmdbuf);

/* These routines are called at start/end of main and depend on SPDRIVE.COM */
int   serial_port_select 
	(int *io_code);
void  initialize_serial_port 
	(void);
void  reset_serial_port	
	(void);

/**/
/* prototypes for command-encode primitives */
/* suffix convention:
c = clear,
s = set,
q = query,
e = enable,
d = disable */
void cmd_0x1Dc (TSIPPKT *cmd);
void cmd_0x1Ds (TSIPPKT *cmd, float offset);
void cmd_0x1E  (TSIPPKT *cmd);
void cmd_0x1F  (TSIPPKT *cmd);
void cmd_0x20  (TSIPPKT *cmd, byte sv_prn);
void cmd_0x21  (TSIPPKT *cmd);
void cmd_0x22  (TSIPPKT *cmd, byte nav_mode);
void cmd_0x23  (TSIPPKT *cmd, float pos_ECEF[3]);
void cmd_0x24  (TSIPPKT *cmd);
void cmd_0x25  (TSIPPKT *cmd);
void cmd_0x26  (TSIPPKT *cmd);
void cmd_0x27  (TSIPPKT *cmd);
void cmd_0x28  (TSIPPKT *cmd);
void cmd_0x29  (TSIPPKT *cmd);
void cmd_0x2A  (TSIPPKT *cmd, float alt);
void cmd_0x2B  (TSIPPKT *cmd, float lat, float lon, float alt);
void cmd_0x2Cq (TSIPPKT *cmd);
void cmd_0x2Cs (TSIPPKT *cmd, byte dyn_code,
	float elev_mask, float snr, float pdop_mask, float pdop_switch);
void cmd_0x2D  (TSIPPKT *cmd);
void cmd_0x2E  (TSIPPKT *cmd, float time_of_week, int week_num);
void cmd_0x2F  (TSIPPKT *cmd);
void cmd_0x31  (TSIPPKT *cmd, float ECEF_pos[3]);
void cmd_0x32  (TSIPPKT *cmd, float lat, float lon, float alt);
void cmd_0x33  (TSIPPKT *cmd);
void cmd_0x34  (TSIPPKT *cmd, byte sv_prn);
void cmd_0x35q (TSIPPKT *cmd);
void cmd_0x35s (TSIPPKT *cmd,
	byte pos_code, byte vel_code, byte time_code, byte opts_code);
void cmd_0x36  (TSIPPKT *cmd, byte coord_frame, byte enable, float vel[3]);
void cmd_0x37  (TSIPPKT *cmd);
void cmd_0x38q (TSIPPKT *cmd, byte data_type, byte sv_prn);
void cmd_0x38s (TSIPPKT *cmd, 
	byte data_type, byte sv_prn, byte data_length, byte *databuf);
void cmd_0x39  (TSIPPKT *cmd, byte op_code, byte sv_prn);
void cmd_0x3A  (TSIPPKT *cmd, byte sv_prn);
void cmd_0x3B  (TSIPPKT *cmd, byte sv_prn);
void cmd_0x3C  (TSIPPKT *cmd, byte sv_prn);
void cmd_0x3Dq (TSIPPKT *cmd);
void cmd_0x3Ds (TSIPPKT *cmd,
	byte baud_out, byte baud_inp, byte char_code,
	byte stopbitcode, byte output_mode, byte input_mode);
void cmd_0x3E  (TSIPPKT *cmd);
void cmd_0x62q (TSIPPKT *cmd);
void cmd_0x62s (TSIPPKT *cmd, byte dgps_mode);
void cmd_0x65  (TSIPPKT *cmd, byte sv_prn);
void cmd_0x71q (TSIPPKT *cmd);
void cmd_0x71s (TSIPPKT *cmd,
	byte pf, 
	float flt1, byte chr1, byte chr2, byte chr3, byte chr4,	
	float flt2, float flt3, float flt4, float flt5,	byte chr5);
void cmd_0x73q (TSIPPKT *cmd);
void cmd_0x73c (TSIPPKT *cmd);
void cmd_0x73d (TSIPPKT *cmd);
void cmd_0x73s (TSIPPKT *cmd, float time_constant);
void cmd_0x75q (TSIPPKT *cmd);
void cmd_0x75s (TSIPPKT *cmd, byte trackmode);
void cmd_0x77q (TSIPPKT *cmd);
void cmd_0x77s (TSIPPKT *cmd, int maxage);
void cmd_0x8E03q (TSIPPKT *cmd);
void cmd_0x8E03s (TSIPPKT *cmd,
	byte baud_out, byte baud_inp, byte char_code,
	byte stopbitcode);
void cmd_0x8E14s (TSIPPKT *cmd,
	int  datum_index, double datum_coeffs[5]);
void cmd_0x8E15q (TSIPPKT *cmd);
/**/
/*  prototypes for report-decode primitives */
/* in TSIP_IFC.C */
int rpt_0x3D (TSIPPKT *rpt,
	byte *tx_baud_index, byte *rx_baud_index, byte *char_format_index,
	byte *stop_bits, byte *tx_mode_index, byte *rx_mode_index);
int rpt_0x40 (TSIPPKT *rpt,
	byte *sv_prn, 
	int *week_num,
	float *t_zc, float *eccentricity, float *t_oa, float *i_0, 
	float *OMEGA_dot, float *sqrt_A, float *OMEGA_0, float *omega,
	float *M_0);
int rpt_0x41 (TSIPPKT *rpt,
	float *time_of_week, float *UTC_offset, 
	int *week_num);
int rpt_0x42 (TSIPPKT *rpt,
	float ECEF_pos[3], float *time_of_fix);
int rpt_0x43 (TSIPPKT *rpt,
	float ECEF_vel[3], float *freq_offset, float *time_of_fix);
int rpt_0x44 (TSIPPKT *rpt, 
	byte *manual_mode, byte *nsvs, byte sv_prn[4],
	float *pdop, float *hdop, float *vdop, float*tdop);
int rpt_0x45 (TSIPPKT *rpt,
	byte *major_nav_version, byte *minor_nav_version,
	byte *nav_day, byte *nav_month, byte *nav_year,
	byte *major_dsp_version, byte *minor_dsp_version,
	byte *dsp_day, byte *dsp_month, byte *dsp_year);
int rpt_0x46 (TSIPPKT *rpt, 
	byte *status_code, byte *error_code);
int rpt_0x47 (TSIPPKT *rpt, 
	byte *nsvs, byte *sv_prn, float *snr);
int rpt_0x48 (TSIPPKT *rpt, 
	byte *message);
int rpt_0x49 (TSIPPKT *rpt,
	byte *sv_health);
int rpt_0x4A (TSIPPKT *rpt,
	float *lat, float *lon, float *alt,
	float *clock_bias, float *time_of_fix);
int rpt_0x4B (TSIPPKT *rpt,
	byte *machine_id, byte *status_1, byte *status_2);
int rpt_0x4C (TSIPPKT *rpt,
	byte *dyn_code,
	float *el_mask, float *snr_mask, float *pdop_mask, float *pdop_switch);
int rpt_0x4D (TSIPPKT *rpt,
	float *osc_offset);
int rpt_0x4E (TSIPPKT *rpt,
	byte *response);
int rpt_0x4F (TSIPPKT *rpt,
	double *a0,
	float *a1, float *time_of_data,
	int *dt_ls,	int *wn_t, int *wn_lsf, int *dn, int *dt_lsf);
int rpt_0x53 (TSIPPKT *rpt,
	float *a2d);
int rpt_0x54 (TSIPPKT *rpt,
	float *clock_bias, float *freq_offset, float *time_of_fix);
int rpt_0x55 (TSIPPKT *rpt,
	byte *pos_code, byte *vel_code, byte *time_code, byte *aux_code);
int rpt_0x56 (TSIPPKT *rpt,
	float vel_ENU[3], float *freq_offset, float *time_of_fix);
int rpt_0x57 (TSIPPKT *rpt,
	byte *source_code, byte *diag_code,
	int *week_num,
	float *time_of_fix);
int rpt_0x58 (TSIPPKT *rpt,
	byte *op_code, byte *data_type, byte *sv_prn,
	byte *data_length, byte *data_packet);
int rpt_0x59 (TSIPPKT *rpt,
	byte *code_type, byte status_code[32]);
int rpt_0x5A (TSIPPKT *rpt,
	byte *sv_prn,
	float *sample_length, float *signal_level, 
	float *code_phase, float *Doppler,
	double *time_of_fix);
int rpt_0x5B (TSIPPKT *rpt,
	byte *sv_prn, byte *sv_health, byte *sv_iode, byte *fit_interval_flag,
	float *time_of_collection, float *time_of_eph, float *sv_accy);
int rpt_0x5C (TSIPPKT *rpt,
	byte *sv_prn, byte *slot, byte *chan, byte *acq_flag, byte *eph_flag,
	float *signal_level, float *time_of_last_msmt, float *elev, float *azim,
	byte *old_msmt_flag, byte *integer_msec_flag, byte *bad_data_flag,
	byte *data_collect_flag);
int rpt_0x5E (TSIPPKT *rpt,
	byte *num_reused_msmts, byte *diff_Doppler_unavailable,
	byte *fix_still_converging, byte *num_old_msmts);
int rpt_0x6D (TSIPPKT *rpt,
	byte *manual_mode, byte *nsvs, byte *ndim, byte sv_prn[],
	float *pdop, float *hdop, float *vdop, float *tdop);
int rpt_0x72 (TSIPPKT *rpt,
	int *pf, float *alpha, int *perr_threshold, 
	int *clamp, int *bad_to_good, int *max_prop_time,
	float *max_dgps_sq_vel, float *dgps_time_const, 
	float *max_sq_vel, float *time_const, 
	int *start_count);
int rpt_0x74 (TSIPPKT *rpt, float *alt_filter_parm);
int rpt_0x76 (TSIPPKT *rpt,
	byte *mode_6);
int rpt_0x78 (TSIPPKT *rpt, int *maxage);
int rpt_0x80 (TSIPPKT *rpt, byte *message);
int rpt_0x82 (TSIPPKT *rpt,
	byte *diff_mode);
int rpt_0x83 (TSIPPKT *rpt,
	double ECEF_pos[3], double *clock_bias, float *time_of_fix);
int rpt_0x84 (TSIPPKT *rpt,
	double *lat, double *lon, double *alt,
	double *clock_bias, float *time_of_fix);
int rpt_0x85 (TSIPPKT *rpt,
	byte *sv_prn, byte *summary_status, byte *station_health,
	byte *SV_UDRE_parm, byte *iode1, byte *iode2,
	float *z_cnt_tow, float *range_corr,
	float *range_rate_corr, float *delta_range_corr);
int rpt_0x8F15 (TSIPPKT *rpt,
	int *datum_idx, double datum_coeffs[5]);

#ifdef TNL
#ifndef POSFILT
#define POSFILT
#endif
/*************************** in TSIP_TNL.C ************************/

/* prototypes */
void init_kb_cmds
	(KB_CMD *kb_cmd, int *num_kb_cmds);
void tnl_cmd
	(TSIPPKT *cmd, byte kbch);
int tnl_reply
	(byte cmd_code, int *reply);
int tnl_rpt
	(TSIPPKT *rpt);
#endif
#ifdef POSFILT
void show_pos_filter
	(TSIPPKT *rpt);
void set_or_request_pos_filter_parms
	(TSIPPKT *cmd, byte kbch);
#endif


