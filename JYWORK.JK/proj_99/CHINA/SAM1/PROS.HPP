//[*]----------------------------------------------------------------------[*]
//   'I' command
//[*]----------------------------------------------------------------------[*]
void init_val  (byte id)
{
	watch();
	if(id == Sam_id  )	{
		Wd_reboot = OFF;
		wd_data[REAL ].sam.max = wd_data[REAL ].sam.min =
		wd_data[MIN1 ].sam.max = wd_data[MIN1 ].sam.min =
		wd_data[MIN5 ].sam.max = wd_data[MIN5 ].sam.min =
		wd_data[MIN10].sam.max = wd_data[MIN10].sam.min =
		wd_data[MIN1 ].sam.avr = wd_data[MIN5 ].sam.avr =
		wd_data[MIN10].sam.avr = wd_data[REAL ].sam.avr  ;

		wd_pros[REAL ].avr = wd_pros[REAL ].min = wd_pros[REAL ].max =
		wd_pros[MIN1 ].avr = wd_pros[MIN1 ].min = wd_pros[MIN1 ].max =
		wd_pros[MIN5 ].avr = wd_pros[MIN5 ].min = wd_pros[MIN5 ].max =
		wd_pros[MIN10].avr = wd_pros[MIN10].min = wd_pros[MIN10].max =
												  wd_data[REAL ].sam.avr;

		wd_pros[REAL ].sin = wd_pros[REAL ].cos =
		wd_pros[MIN1 ].sin = wd_pros[MIN1 ].cos =
		wd_pros[MIN5 ].sin = wd_pros[MIN5 ].cos =
		wd_pros[MIN10].sin = wd_pros[MIN10].cos = 0.;
	}
	if(id == Sam_id+1)	{
		Ws_reboot = OFF;
		ws_data[REAL ].sam.max = ws_data[REAL ].sam.min =
		ws_data[MIN1 ].sam.max = ws_data[MIN1 ].sam.min =
		ws_data[MIN5 ].sam.max = ws_data[MIN5 ].sam.min =
		ws_data[MIN10].sam.max = ws_data[MIN10].sam.min =
		ws_data[MIN1 ].sam.avr = ws_data[MIN5 ].sam.avr =
		ws_data[MIN10].sam.avr = ws_data[REAL ].sam.avr  ;

		ws_pros[REAL ].avr = ws_pros[REAL ].min = ws_pros[REAL ].max =
		ws_pros[MIN1 ].avr = ws_pros[MIN1 ].min = ws_pros[MIN1 ].max =
		ws_pros[MIN5 ].avr = ws_pros[MIN5 ].min = ws_pros[MIN5 ].max =
		ws_pros[MIN10].avr = ws_pros[MIN10].min = ws_pros[MIN10].max =
												  ws_data[REAL ].sam.avr;

		ws_pros[REAL ].count = ws_pros[MIN1 ].count =
		ws_pros[MIN5 ].count = ws_pros[MIN10].count = 0 ;
		ws_pros[REAL ].sum   = ws_pros[MIN1 ].sum   =
		ws_pros[MIN5 ].sum   = ws_pros[MIN10].sum   = 0.;
	}
}
//[*]----------------------------------------------------------------------[*]
//   '@' command
//[*]----------------------------------------------------------------------[*]
void maxmin_val(byte id, word mmax, word mmin)
{
	watch();
	if(id == Sam_id  )	{
		Wd_reboot = OFF;
		wd_data[REAL ].sam.max = mmax;		wd_data[REAL ].sam.min = mmin;
		wd_data[MIN1 ].sam.max = wd_data[MIN1 ].sam.min =
		wd_data[MIN5 ].sam.max = wd_data[MIN5 ].sam.min =
		wd_data[MIN10].sam.max = wd_data[MIN10].sam.min =
		wd_data[MIN1 ].sam.avr = wd_data[MIN5 ].sam.avr =
		wd_data[MIN10].sam.avr = wd_data[REAL ].sam.avr  ;

		wd_pros[REAL ].max = mmax;			wd_pros[REAL ].min = mmin;
		wd_pros[MIN1 ].avr = wd_pros[MIN1 ].min = wd_pros[MIN1 ].max =
		wd_pros[MIN5 ].avr = wd_pros[MIN5 ].min = wd_pros[MIN5 ].max =
		wd_pros[MIN10].avr = wd_pros[MIN10].min = wd_pros[MIN10].max =
												  wd_data[REAL ].sam.avr;

		wd_pros[REAL ].sin = wd_pros[REAL ].cos =
		wd_pros[MIN1 ].sin = wd_pros[MIN1 ].cos =
		wd_pros[MIN5 ].sin = wd_pros[MIN5 ].cos =
		wd_pros[MIN10].sin = wd_pros[MIN10].cos = 0.;
	}
	if(id == Sam_id+1)	{
		Ws_reboot = OFF;
		ws_data[REAL ].sam.max = mmax;		ws_data[REAL ].sam.min = mmin;
		ws_data[MIN1 ].sam.max = ws_data[MIN1 ].sam.min =
		ws_data[MIN5 ].sam.max = ws_data[MIN5 ].sam.min =
		ws_data[MIN10].sam.max = ws_data[MIN10].sam.min =
		ws_data[MIN1 ].sam.avr = ws_data[MIN5 ].sam.avr =
		ws_data[MIN10].sam.avr = ws_data[REAL ].sam.avr  ;

		ws_pros[REAL ].max = mmax;			ws_pros[REAL ].min = mmin;
		ws_pros[MIN1 ].avr = ws_pros[MIN1 ].min = ws_pros[MIN1 ].max =
		ws_pros[MIN5 ].avr = ws_pros[MIN5 ].min = ws_pros[MIN5 ].max =
		ws_pros[MIN10].avr = ws_pros[MIN10].min = ws_pros[MIN10].max =
												  ws_data[REAL ].sam.avr;

		ws_pros[REAL ].count = ws_pros[MIN1 ].count =
		ws_pros[MIN5 ].count = ws_pros[MIN10].count = 0 ;
		ws_pros[REAL ].sum   = ws_pros[MIN1 ].sum   =
		ws_pros[MIN5 ].sum   = ws_pros[MIN10].sum   = 0.;
	}
}
//[*]----------------------------------------------------------------------[*]
//   Wind direction cal
//[*]----------------------------------------------------------------------[*]
word wind_cal(word ad_value)
{
	float 	wd_data  = 0.,  flag    = 0.;
	word	new_tbl  = 0,   old_tbl = 0 ;
	word	wd_value = 0;
	byte	cal_flag = OFF, i       = 0 ;

	watch();
	if(ad_value == 0)	{
		cal_flag = ON;		wd_value = 0;
	}
	else				{
		for(i = 0; i != 16; i++)	{
			flag    += 22.5;
			new_tbl  = WINDDIR_TBL[i];
			if(ad_value >= old_tbl && ad_value < new_tbl)	{
				cal_flag = ON;
				wd_data  = (float)((float)(flag - (flag - 22.5)) /
								   (float)(new_tbl - old_tbl)     );
				wd_value = (word)((float)((wd_data * (float)ad_value) -
										  (wd_data * (float)old_tbl ) +
										  (flag    - 22.5           )) * 10.);
				break;
			}
			else	{
				cal_flag = OFF;			old_tbl = new_tbl;
			}
		}
	}

	if(cal_flag)	return	(word)(wd_value);
	else			return 	(word)(3599    );
}
//[*]----------------------------------------------------------------------[*]
//   Wind Speed m/s Calculate
//[*]----------------------------------------------------------------------[*]
word ws_ms_38hz(word ws_puls)
{
	float 	ws_data  = 0.,  flag    = 0.;
	word	new_tbl  = 0,   old_tbl = 0 ;
	word	ws_value = 0;
	byte	cal_flag = OFF, i       = 0 ;

	watch();
	if     (ws_puls == 0           )	{
		cal_flag = ON;	ws_value = 0;
	}
	else if(ws_puls <= SPD_TBL38[0])	{
		cal_flag = ON;
		ws_data  = 3./ (float)SPD_TBL38[0];
		ws_value = (word)((ws_data * (float)ws_puls) * 10.);
	}
	else if(ws_puls > SPD_TBL38[0] && ws_puls <= SPD_TBL38[1])	{
		cal_flag = ON;
		new_tbl  = SPD_TBL38[1];		old_tbl = SPD_TBL38[0];
		ws_data  = 2./ (float)(new_tbl - old_tbl);
		ws_value = (word)(((float)(ws_data * (float)ws_puls) -
						   (float)(ws_data * (float)old_tbl) + 3.) * 10.);
	}
	else	{
		old_tbl = SPD_TBL38[1];
		flag = 5.;
		for(i = 2; i != 16; i++)	{
			flag += 5.;
			new_tbl = SPD_TBL38[i];
			if(ws_puls > old_tbl && ws_puls <= new_tbl)	{
				cal_flag = ON;
				ws_data  = (flag - (flag - 5.)) / (float)(new_tbl - old_tbl);
				ws_value = (word)(((float)(ws_data * (float)ws_puls) -
								   (float)(ws_data * (float)old_tbl) +
								   (float)(flag                - 5.)) * 10.);
				break;
			}
			else	{
				cal_flag = OFF;		old_tbl = new_tbl;
			}
		}
	}
	if(cal_flag)	return 	(word)ws_value;
	else			return	610;
}
//[*]----------------------------------------------------------------------[*]
word ws_ms_45hz(word ws_puls)
{
	float 	ws_data  = 0.,  flag    = 0.;
	word	new_tbl  = 0,   old_tbl = 0 ;
	word	ws_value = 0;
	byte	cal_flag = OFF, i       = 0 ;

	watch();
	if     (ws_puls == 0           )	{
		cal_flag = ON;	ws_value = 0;
	}
	else if(ws_puls <= SPD_TBL45[0])	{
		cal_flag = ON;
		ws_data  = 3./ (float)SPD_TBL45[0];
		ws_value = (word)((ws_data * (float)ws_puls) * 10.);
	}
	else if(ws_puls > SPD_TBL45[0] && ws_puls <= SPD_TBL45[1])	{
		cal_flag = ON;
		new_tbl  = SPD_TBL45[1];		old_tbl = SPD_TBL45[0];
		ws_data  = 2./ (float)(new_tbl - old_tbl);
		ws_value = (word)(((float)(ws_data * (float)ws_puls) -
						   (float)(ws_data * (float)old_tbl) + 3.) * 10.);
	}
	else	{
		old_tbl = SPD_TBL45[1];
		flag = 5.;
		for(i = 2; i != 16; i++)	{
			flag += 5.;
			new_tbl = SPD_TBL45[i];
			if(ws_puls > old_tbl && ws_puls <= new_tbl)	{
				cal_flag = ON;
				ws_data  = (flag - (flag - 5.)) / (float)(new_tbl - old_tbl);
				ws_value = (word)(((float)(ws_data * (float)ws_puls) -
								   (float)(ws_data * (float)old_tbl) +
								   (float)(flag                - 5.)) * 10.);
				break;
			}
			else	{
				cal_flag = OFF;		old_tbl = new_tbl;
			}
		}
	}
	if(cal_flag)	return 	(word)ws_value;
	else			return	610;
}
//[*]----------------------------------------------------------------------[*]
//   Wind Speed knot Calculate
//[*]----------------------------------------------------------------------[*]
word ws_knot_38hz(word ws_puls)
{
	float 	ws_data  = 0.,  flag    = 0.;
	word	new_tbl  = 0,   old_tbl = 0 ;
	word	ws_value = 0;
	byte	cal_flag = OFF, i       = 0 ;

	watch();
	if     (ws_puls == 0           )	{
		cal_flag = ON;	ws_value = 0;
	}
	else if(ws_puls <= SPD_TBL38[0])	{
		cal_flag = ON;
		ws_data  = 3./ (float)SPD_TBL38[0];
		ws_value = (word)((ws_data * (float)ws_puls) * 10. * MS_KNOT);
	}
	else if(ws_puls > SPD_TBL38[0] && ws_puls <= SPD_TBL38[1])	{
		cal_flag = ON;
		new_tbl  = SPD_TBL38[1];		old_tbl = SPD_TBL38[0];
		ws_data  = 2./ (float)(new_tbl - old_tbl);
		ws_value = (word)(((float)(ws_data * (float)ws_puls) -
						   (float)(ws_data * (float)old_tbl) + 3.) * 10. * MS_KNOT);
	}
	else	{
		old_tbl = SPD_TBL38[1];
		flag = 5.;
		for(i = 2; i != 16; i++)	{
			flag += 5.;
			new_tbl = SPD_TBL38[i];
			if(ws_puls > old_tbl && ws_puls <= new_tbl)	{
				cal_flag = ON;
				ws_data  = (flag - (flag - 5.)) / (float)(new_tbl - old_tbl);
				ws_value = (word)(((float)(ws_data * (float)ws_puls) -
								   (float)(ws_data * (float)old_tbl) +
								   (float)(flag    - 5.)) * 10. * MS_KNOT);
				break;
			}
			else	{
				cal_flag = OFF;		old_tbl = new_tbl;
			}
		}
	}
	if(cal_flag)	return 	(word)(ws_value      );
	else			return	(word)(MS_KNOT * 610.);
}
//[*]----------------------------------------------------------------------[*]
word ws_knot_45hz(word ws_puls)
{
	float 	ws_data  = 0.,  flag    = 0.;
	word	new_tbl  = 0,   old_tbl = 0 ;
	word	ws_value = 0;
	byte	cal_flag = OFF, i       = 0 ;

	watch();
	if     (ws_puls == 0           )	{
		cal_flag = ON;	ws_value = 0;
	}
	else if(ws_puls <= SPD_TBL45[0])	{
		cal_flag = ON;
		ws_data  = 3./ (float)SPD_TBL45[0];
		ws_value = (word)((ws_data * (float)ws_puls) * 10. * MS_KNOT);
	}
	else if(ws_puls > SPD_TBL45[0] && ws_puls <= SPD_TBL45[1])	{
		cal_flag = ON;
		new_tbl  = SPD_TBL45[1];		old_tbl = SPD_TBL45[0];
		ws_data  = 2./ (float)(new_tbl - old_tbl);
		ws_value = (word)(((float)(ws_data * (float)ws_puls) -
						   (float)(ws_data * (float)old_tbl) + 3.) * 10. * MS_KNOT);
	}
	else	{
		old_tbl = SPD_TBL45[1];
		flag = 5.;
		for(i = 2; i != 16; i++)	{
			flag += 5.;
			new_tbl = SPD_TBL45[i];
			if(ws_puls > old_tbl && ws_puls <= new_tbl)	{
				cal_flag = ON;
				ws_data  = (flag - (flag - 5.)) / (float)(new_tbl - old_tbl);
				ws_value = (word)(((float)(ws_data * (float)ws_puls) -
								   (float)(ws_data * (float)old_tbl) +
								   (float)(flag    - 5.)) * 10. * MS_KNOT);
				break;
			}
			else	{
				cal_flag = OFF;		old_tbl = new_tbl;
			}
		}
	}
	if(cal_flag)	return 	(word)(ws_value      );
	else			return	(word)(MS_KNOT * 610.);
}
//[*]----------------------------------------------------------------------[*]




