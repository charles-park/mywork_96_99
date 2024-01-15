#pragma memory = code
unsigned char	NAME[7][77] = {
"[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]",
"[*]              97' AWS and AMOS RTU(Remote Terminal Unit)             [*]",
"[*]                            main program                             [*]",
"[*]          Program version 9708.12   Copyright Jinyang inc..          [*]",
"[*]                          Date 1997-08-12                            [*]",
"[*]                     Program by Jinmin - Mok                         [*]",
"[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]" };
#pragma memory = default
//[*]---------------------------------------------------------------------[*]
#include <io51.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//[*]---------------------------------------------------------------------[*]
#define WATCHDOG	P1.5 = 1 - P1.5;
#define M_PI        3.14159265358979323846
#define	MSB_DB		0x7FFE

#define	REAL		0
#define	CALL		60
#define	SAVE	    600		// Defaule : 600 = 10 Min average

#define	INIT		'I'
#define	RST			'@'

#define	ADH_ADDR	0x8000
#define	ADL_ADDR	0x8001
#define	JUMPER		0x9000
#define	CHANNEL		0xA000
#define	DAH_ADDR	0xB000
#define	DAL_ADDR	0xC000

#define	TX_LED_ON	P1.6 = 1
#define	RX_LED_ON	P1.7 = 1
#define	TX_LED_OFF	P1.6 = 0
#define	RX_LED_OFF	P1.7 = 0

#define	WD			0
#define	WS			1
#define	SI			2


//[*]---------------------------------------------------------------------[*]
#define disable(y)	y = OFF
#define enable(z)	z = ON
//[*]---------------------------------------------------------------------[*]
#define	ON			1
#define	OFF			0
#define	NORMAL		0
#define	FAIL		1

//[*]---------------------------------------------------------------------[*]
typedef struct value__t	{
	unsigned char	id;
	unsigned char	tbl_switch;
	unsigned char	start_flag;
	unsigned int	delay_count;

	unsigned char	average;

	unsigned int	oldv;
	unsigned int	newv;
	unsigned long	new_cnt;
	unsigned long	old_cnt;

			 float	wspeed;
			 float	flag;

	unsigned int	wd_read;
	unsigned char	wd_bl_pos;
			 float	wd_value;
			 float	wd_sin_t1;
			 float	wd_cos_t1;
			 float	wd_sin_t10;
			 float	wd_cos_t10;
			 float	wd_call_maxi;
			 float	wd_save_maxi;

	unsigned int	ws_cnt;
			 float	ws_call_tot;
			 float	ws_save_tot;
			 float	ws_call_maxi;
			 float	ws_save_maxi;

			 float	value;
} value_t;

static value_t		value;

//[*]---------------------------------------------------------------------[*]
typedef struct clock__t	{
	unsigned char	one_sec;
	unsigned int	call_sec;
	unsigned int	save_sec;
} clock_t;

static clock_t		clock;

//[*]---------------------------------------------------------------------[*]
typedef struct sigma__t	{
	unsigned char	flag;
			 float	sin_sec[240];
			 float	cos_sec[240];
			 float	sin_tot[15];
			 float	cos_tot[15];
	unsigned char	bl_pos;
	unsigned char	bin_pos;
	unsigned int	sigma;
			 float	sin_bin_tot;
			 float	cos_bin_tot;
} sigma_t;

static sigma_t		sigma;

//[*]---------------------------------------------------------------------[*]
static	unsigned	int		ADIRECTION[16] = {
 254, 509, 763,1018,1272,1526,1781,2035,2289,2544,2798,3053,3307,3561,3816,4070};
// 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
//[*]---------------------------------------------------------------------[*]
static	unsigned	int		SPEED_TABLE_38[16] = {
  38,  72, 152, 227, 303, 382, 460, 544, 627, 706, 785, 864, 944,1022,1101,1181};
static	unsigned	int		SPEED_TABLE_45[16] = {
  45,  80, 160, 248, 333, 420, 507, 593, 680, 766, 853, 940,1027,1113,1200,1287};
// 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
//[*]---------------------------------------------------------------------[*]
typedef struct 	sign__t	{
	unsigned long	header;
	unsigned char	board_id;
	unsigned char	sensor_id;
	unsigned char	command;
	unsigned int	data_size;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}	sign_t;

typedef union sign__u	{
	sign_t			sam;
	unsigned char	bytes[sizeof(sign_t)];
}	sign_u;

static sign_u		sign;

//[*]---------------------------------------------------------------------[*]
typedef struct	reveive__t	{
	unsigned long	header;
	unsigned char	command;
	unsigned int	schedule;
	unsigned char	status;
	unsigned char	sensor_code;
	unsigned char	sensor_id;
			 float	aver;
			 float	spec;
			 float	mini;
			 float	maxi;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}   reveive_t;

typedef union	reveive__u	{
	reveive_t		receive;
	unsigned char	bytes[sizeof(reveive_t)];
}	reveive_u;

static reveive_u	mpm;

//[*]---------------------------------------------------------------------[*]
typedef struct	weather__t	{
	unsigned long	header;
	unsigned char	command;
	unsigned int	schedule;
	unsigned char	status;
	unsigned char	sensor_code;
	unsigned char	sensor_id;
			 float	aver;
			 float	spec;
			 float	mini;
			 float	maxi;
	unsigned char	chk_xor;
	unsigned char	chk_add;
	unsigned long	tail;
}   weather_t;

typedef union	weather__u	{
	weather_t		sam;
	unsigned char	bytes[sizeof(weather_t)];
}   weather_u;

static weather_u	real[3], call[3], save[3];

//[*]---------------------------------------------------------------------[*]
typedef struct		recoder__t	{
	unsigned int	aver[8];
	unsigned int	mini[8];
	unsigned int	maxi[8];
	unsigned char	pos;
	unsigned char	sel;
}	recoder_t;

static recoder_t	recoder;

//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void variable_initial(void);
void  sample(void);								// W/D Sensor sample
float ws_calc_38(float wssum);					// Calculate W/S(38Hz = 3m/s)
float ws_calc_45(float ws_sum);					// Calculate W/S(45Hz = 3m/s)
float main_540deg(float winddir);
void  main(void);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#include "init&int.hpp"
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void variable_initial(void)
{
	unsigned int i;
	unsigned char Switch;

	value.id		 = ~(input(JUMPER)) & 0x07;
	value.tbl_switch = ~(input(JUMPER)) & 0x20;

	Switch = 64;								// default
	switch (Switch)	{
		case   4: value.average = 240; break;
		case   8: value.average = 120; break;
		case  16: value.average =  60; break;
		case  32: value.average =  40; break;
		case  64: value.average =  20; break;
		case 128: value.average =   1; break;
	}
	WATCHDOG;

	for(i=0; i<3; i++)	{
		real[i].sam.header   = call[i].sam.header  = save[i].sam.header  = 0xFFFFFFFF;
		real[i].sam.command  = call[i].sam.command = save[i].sam.command = 'W';
		real[i].sam.schedule = REAL;
		call[i].sam.schedule = CALL;
		save[i].sam.schedule = SAVE;
		real[i].sam.status   = call[i].sam.status  = save[i].sam.status  = 0 ;
		real[i].sam.aver     = call[i].sam.aver    = save[i].sam.aver    = 0.;
		real[i].sam.spec     = call[i].sam.spec    = save[i].sam.spec    = 0.;
		real[i].sam.mini     = call[i].sam.mini    = save[i].sam.mini    = 0.;
		real[i].sam.maxi     = call[i].sam.maxi    = save[i].sam.maxi    = 0.;
		real[i].sam.chk_xor  = call[i].sam.chk_xor = save[i].sam.chk_xor = 0 ;
		real[i].sam.chk_add  = call[i].sam.chk_add = save[i].sam.chk_add = 0 ;
		real[i].sam.tail     = call[i].sam.tail    = save[i].sam.tail    = 0xFEFEFEFE;
		WATCHDOG;
	}
	real[WD].sam.sensor_code =
	call[WD].sam.sensor_code = save[WD].sam.sensor_code = 0x00;
	real[WS].sam.sensor_code =
	call[WS].sam.sensor_code = save[WS].sam.sensor_code = 0x01;
	real[SI].sam.sensor_code =
	call[SI].sam.sensor_code = save[SI].sam.sensor_code = 0x12;

	real[WD].sam.sensor_id   =
	call[WD].sam.sensor_id   = save[WD].sam.sensor_id   = value.id;
	real[WS].sam.sensor_id   =
	call[WS].sam.sensor_id   = save[WS].sam.sensor_id   = value.id + 1;
	real[SI].sam.sensor_id   =
	call[SI].sam.sensor_id   = save[SI].sam.sensor_id   = value.id + 200;

	clock.one_sec     = 0;
	clock.call_sec    = clock.save_sec   = 0;

	value.wd_sin_t1   = value.wd_cos_t1  = 0.;
	value.wd_sin_t10  = value.wd_cos_t10 = 0.;

	value.ws_call_tot = value.ws_save_tot = value.ws_save_maxi = 0.;

	value.start_flag  = 0;
	sigma.flag        = 0;
	sigma.bl_pos	  = 0;

	for(value.delay_count = 0; value.delay_count < 5;)	WATCHDOG;
	TX_LED_ON;
	for(value.delay_count = 0; value.delay_count < 5;)	WATCHDOG;
	TX_LED_OFF;	RX_LED_ON;
	for(value.delay_count = 0; value.delay_count < 5;)	WATCHDOG;
	TX_LED_OFF;	RX_LED_OFF;

	recoder.pos = 0;
	recoder.sel = 1;
	for(i = 0; i < 8; i++)	{
		recoder.aver[i] = 0xFFFF;
		recoder.mini[i] =   5000;
		recoder.maxi[i] =      0;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void sample(void)								// W/D Sensor sample
{
	word	i;
	byte	sw;
	float	sin_value, cos_value;

	clock.one_sec = 0;
	clock.call_sec++;
	clock.save_sec++;

	P1.4 = 0;									// Sample hold
	write_XDATA(ADH_ADDR,0xFF);					// Converting Start
	for(i=0; i<5; i++)	{						// Delay & Write Recoder
		real[WD].sam.status &= 0xFB;			// AD Converter normal
		call[WD].sam.status &= 0xFB;
		save[WD].sam.status &= 0xFB;
	}
	i = 0;
	while(P3.5 == 1)	{						// Check end of converting
		if(i++ > 100)	{
			real[WD].sam.status |= 0x04;		// AD Converter error
			call[WD].sam.status |= 0x04;
			save[WD].sam.status |= 0x04;
			break;
		}
		WATCHDOG;
	}
	value.wd_read  = read_XDATA(ADL_ADDR) >> 4;	// W/D read
	value.wd_read |= read_XDATA(ADH_ADDR) << 4;
	P1.4 = 1;									// Sample run

	value.oldv = 0;								// Calculate W/D
	value.flag = 0;
	sw         = 0;
	for(i=0; i != 16; i++)
	{
		value.flag += 22.5;
		value.newv  = ADIRECTION[i];
		if(value.wd_read < value.newv && value.wd_read >= value.oldv)	{
			sw = 1;
			value.value = (value.flag-(value.flag-22.5)) / (value.newv-value.oldv);
			value.wd_value = (float)((value.value*value.wd_read)-(value.value*value.oldv)+(value.flag-22.5));
			break;
		}
		else value.oldv = value.newv;
	}
	if(!sw) value.wd_value = 359.9;

	real[WD].sam.aver = value.wd_value;					// Real W/D data

	value.value = value.wd_value * (M_PI / 180.0);		// Calculate sigma

	sin_value = sin(value.wd_value * M_PI / 180.0);
	cos_value = cos(value.wd_value * M_PI / 180.0);

	sigma.sin_sec[sigma.bl_pos] = sin_value;
	sigma.cos_sec[sigma.bl_pos] = cos_value;

	value.wd_sin_t1  += sin_value;
	value.wd_sin_t10 += sin_value;
	value.wd_cos_t1  += cos_value;
	value.wd_cos_t10 += cos_value;

	sigma.bl_pos++;
	if(sigma.bl_pos >= value.average)	{
		sigma.bl_pos = 0;
		sigma.sin_tot[sigma.bin_pos] = 0;
		sigma.cos_tot[sigma.bin_pos] = 0;
		for(i=0; i<value.average; i++)	{
			sigma.sin_tot[sigma.bin_pos] += sigma.sin_sec[i];
			sigma.cos_tot[sigma.bin_pos] += sigma.cos_sec[i];
		}
		sigma.sin_tot[sigma.bin_pos] = sigma.sin_tot[sigma.bin_pos] / (float)(value.average);
		sigma.cos_tot[sigma.bin_pos] = sigma.cos_tot[sigma.bin_pos] / (float)(value.average);
		sigma.bin_pos ++;
		if(sigma.bin_pos >= 15)	{
			sigma.bin_pos = 0;	sigma.flag = 1;
		}
		if(sigma.flag == 1) {
			real[SI].sam.status &= 0xFD;			// Sigma sensor normal
			call[SI].sam.status &= 0xFD;
			save[SI].sam.status &= 0xFD;

			sigma.sin_bin_tot = 0;
			sigma.cos_bin_tot = 0;
			for(i=0; i<15; i++)	{
				sigma.sin_bin_tot += sigma.sin_tot[i];
				sigma.cos_bin_tot += sigma.cos_tot[i];
			}
			sigma.sin_bin_tot = sigma.sin_bin_tot / 15.;
			sigma.cos_bin_tot = sigma.cos_bin_tot / 15.;
			value.value = sqrt((sigma.sin_bin_tot*sigma.sin_bin_tot)+(sigma.cos_bin_tot*sigma.cos_bin_tot));
			value.value = 2.- 2. * value.value;
			if(value.value >= 0)	{
				sigma.sigma = (unsigned int)((360. / (2*M_PI) * sqrt(value.value)) * 10.);
			}	else if(real[WS].sam.aver >= 5)	{
					sigma.sigma = 2000;
					real[SI].sam.status |= 0x02;	// Sigma sensor error
					call[SI].sam.status |= 0x02;
					save[SI].sam.status |= 0x02;
			}	else	sigma.sigma = 0;
			real[SI].sam.aver = sigma.sigma;		// Real sigma data
			call[SI].sam.aver = sigma.sigma;		// 1 mini sigma data
			save[SI].sam.aver = sigma.sigma;		// 10 mini sigma data

			if        (                      sigma.sigma >= 240) real[SI].sam.spec = call[SI].sam.spec = save[SI].sam.spec = 1;
			else	if(sigma.sigma <= 230 && sigma.sigma >= 190) real[SI].sam.spec = call[SI].sam.spec = save[SI].sam.spec = 2;
			else	if(sigma.sigma <= 180 && sigma.sigma >= 140) real[SI].sam.spec = call[SI].sam.spec = save[SI].sam.spec = 3;
			else	if(sigma.sigma <= 130 && sigma.sigma >=  90) real[SI].sam.spec = call[SI].sam.spec = save[SI].sam.spec = 4;
			else	if(sigma.sigma <=  80 && sigma.sigma >=  40) real[SI].sam.spec = call[SI].sam.spec = save[SI].sam.spec = 5;
			else	if(sigma.sigma <=  30                      ) real[SI].sam.spec = call[SI].sam.spec = save[SI].sam.spec = 6;

			if (real[SI].sam.mini > real[SI].sam.aver)	{
				real[SI].sam.mini = real[SI].sam.aver;
				call[SI].sam.mini = real[SI].sam.aver;
				save[SI].sam.mini = real[SI].sam.aver;
			}
			if (real[SI].sam.maxi < real[SI].sam.aver)	{
				real[SI].sam.maxi = real[SI].sam.aver;
				call[SI].sam.maxi = real[SI].sam.aver;
				save[SI].sam.maxi = real[SI].sam.aver;
			}
		}
	}
	value.tbl_switch	= ~(input(JUMPER)) & 0x20;
	// Real W/S data
	if(value.tbl_switch == 0x20)	real[WS].sam.aver = ws_calc_45((float)(value.ws_cnt));
	else							real[WS].sam.aver = ws_calc_38((float)(value.ws_cnt));
	value.ws_call_tot += real[WS].sam.aver;
	value.ws_save_tot += real[WS].sam.aver;

	if(real[WS].sam.maxi < real[WS].sam.aver)	{
	   real[WS].sam.maxi = real[WS].sam.aver;
	   real[WD].sam.maxi = real[WD].sam.aver;
	}
	if(value.ws_call_maxi < real[WS].sam.aver)	{
		value.wd_call_maxi = real[WD].sam.aver;
		value.ws_call_maxi = real[WS].sam.aver;
	}
	if(value.ws_save_maxi < real[WS].sam.aver)	{
		value.wd_save_maxi = real[WD].sam.aver;
		value.ws_save_maxi = real[WS].sam.aver;
	}
	if(value.start_flag == 0)	{
		call[WD].sam.aver  = save[WD].sam.aver  = real[WD].sam.aver;
		call[WD].sam.maxi  = save[WD].sam.maxi  = real[WD].sam.aver;
		call[WS].sam.aver  = save[WS].sam.aver  = real[WS].sam.aver;
		call[WS].sam.maxi  = save[WS].sam.maxi  = real[WS].sam.aver;

		value.wd_call_maxi = value.wd_save_maxi = real[WD].sam.aver;
		value.ws_call_maxi = value.ws_save_maxi = real[WS].sam.aver;
		value.start_flag   = 1;
	}
	if(clock.call_sec >= CALL)	{											// Call mini data
		clock.call_sec = 0;
		value.value = (atan(value.wd_sin_t1 / value.wd_cos_t1)) * (180. / M_PI);
		if(value.wd_sin_t1 < 0 && value.wd_cos_t1 > 0)			value.value += 360.;
		else if((value.wd_sin_t1 > 0 && value.wd_cos_t1 < 0) ||
				(value.wd_sin_t1 < 0 && value.wd_cos_t1 < 0))	value.value += 180.;
		call[WD].sam.aver  = value.value;									// Call min W/D data
		call[WS].sam.aver  = value.ws_call_tot / (float)(clock.call_sec);	// Call min W/S data
		call[WD].sam.maxi  = value.wd_call_maxi;
		call[WS].sam.maxi  = value.ws_call_maxi;
		value.wd_call_maxi = value.ws_call_maxi = 0.;
		value.wd_sin_t1    = value.wd_cos_t1    = value.ws_call_tot = 0.;
	}
	if(clock.save_sec >= SAVE)	{											// Save mini data
		value.value = (atan(value.wd_sin_t10 / value.wd_cos_t10)) * (180. / M_PI);
		if(value.wd_sin_t10 < 0 && value.wd_cos_t10 > 0)		value.value += 360.;
		else if((value.wd_sin_t10 > 0 && value.wd_cos_t10 < 0) ||
				(value.wd_sin_t10 < 0 && value.wd_cos_t10 < 0))	value.value += 180.;
		save[WD].sam.aver = value.value;									// Save min W/D data
		save[WS].sam.aver = value.ws_save_tot / (float)(clock.save_sec);	// Save min W/S data
		if(save[WS].sam.aver > 60.1)	{
			save[WS].sam.aver    = 60.0;
			save[WS].sam.status |= 0x02;
		}
		save[WD].sam.maxi  = value.wd_save_maxi;
		save[WS].sam.maxi  = value.ws_save_maxi;
		value.wd_save_maxi = value.ws_save_maxi = 0.;
		value.wd_sin_t10   = value.wd_cos_t10   = value.ws_save_tot = 0.;
		clock.save_sec     = 0;
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
float ws_calc_38(float ws_sum)	// Calculate W/S
{
	byte i;

	if(ws_sum == 0)	ws_sum = 0;
	else	if(ws_sum <= SPEED_TABLE_38[0])	{
		value.value = 3. / (float)(SPEED_TABLE_38[0]);
		ws_sum = value.value * ws_sum;
	} else	if(ws_sum > (float)(SPEED_TABLE_38[0]) &&
				ws_sum <= (float)(SPEED_TABLE_38[1]))	{
		value.newv   = SPEED_TABLE_38[1];
		value.oldv   = SPEED_TABLE_38[0];
		value.value  = 2. / (float)(value.newv - value.oldv);
		ws_sum  = ((value.value * ws_sum) - (value.value * value.oldv) + 3.);
	} else	{
		value.oldv = SPEED_TABLE_38[1];
		value.flag = 5.;
		for(i=2; i!=16; i++)	{
			value.flag += 5.;
			value.newv  = SPEED_TABLE_38[i];
			if( ws_sum < (float)(value.newv) && ws_sum >= (float)(value.oldv))	{
				value.value = (float)(value.flag - (value.flag - 5.)) / ((float)(value.newv - value.oldv));
				ws_sum = (value.value * ws_sum)-(value.value * (float)(value.oldv))+( value.flag - 5.);
				break;
			} else	value.oldv = value.newv;
		}
	}
	if(ws_sum > 60.0)	return 60.1;
	else				return ws_sum;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
float ws_calc_45(float ws_sum)					// Calculate W/S
{
	byte i;

	if(ws_sum == 0)	ws_sum = 0;
	else	if(ws_sum <= SPEED_TABLE_45[0])	{
		value.value = 3. / (float)(SPEED_TABLE_45[0]);
		ws_sum = value.value * ws_sum;
	} else	if(ws_sum > (float)(SPEED_TABLE_45[0]) &&
				ws_sum <= (float)(SPEED_TABLE_45[1]))	{
		value.newv   = SPEED_TABLE_45[1];
		value.oldv   = SPEED_TABLE_45[0];
		value.value  = 2. / (float)(value.newv - value.oldv);
		ws_sum  = ((value.value * ws_sum) - (value.value * value.oldv) + 3.);
	} else	{
		value.oldv = SPEED_TABLE_45[1];
		value.flag = 5.;
		for(i=2; i!=16; i++)	{
			value.flag += 5.;
			value.newv  = SPEED_TABLE_45[i];
			if( ws_sum < (float)(value.newv) && ws_sum >= (float)(value.oldv))	{
				value.value = (float)(value.flag - (value.flag - 5.)) / ((float)(value.newv - value.oldv));
				ws_sum = (value.value * ws_sum)-(value.value * (float)(value.oldv))+( value.flag - 5.);
				break;
			} else	value.oldv = value.newv;
		}
	}
	if(ws_sum > 60.0)	return 60.1;
	else				return ws_sum;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
float main_540deg(float winddir)
{
	static float oldwinddir=0.0;
	float ret_winddir;
	float tmp_winddir1, tmp_winddir2;

	if     (winddir <   0.0)	winddir =     0.0;
	else if(winddir > 360.0)	winddir =   360.0;
	else                        winddir = winddir;
	ret_winddir = winddir;

	if(oldwinddir <= 180.0)									{
		ret_winddir = winddir;
	}	else if(oldwinddir > 180.0 && oldwinddir <= 360.0)	{
		if(winddir < 180.0)     {
			tmp_winddir1 = (oldwinddir - (winddir +   0.0));
			tmp_winddir2 = (oldwinddir - (winddir + 360.0));
			if(tmp_winddir1 < 0.0)  tmp_winddir1 = tmp_winddir1 * -1.0;
			if(tmp_winddir2 < 0.0)  tmp_winddir2 = tmp_winddir2 * -1.0;
			if(tmp_winddir1 <= tmp_winddir2)     ret_winddir = winddir;
			else				ret_winddir = winddir + 360.0;
		}	else				ret_winddir = winddir;
	}   else if(oldwinddir >= 360.0)    {
		if(winddir <= 180.0)	ret_winddir = winddir + 360.0;
		else					ret_winddir = winddir;
	}
	oldwinddir = ret_winddir;
	return	     ret_winddir;
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void main(void)
{
	TX_LED_OFF;
	RX_LED_OFF;

	WATCHDOG;
	initiliaze();								// Serial port & rxQ initialize.
	variable_initial();							// Variable initialize.

	WATCHDOG;
	while(1) {
		WATCHDOG;
		if(clock.one_sec >= 8)	sample();
		receive_check();
		transmit_check();
	}
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
