//[*]----------------------------------------------------------------------[*]
void maxmin_save(byte addr, word max, word min)
{
	watch();
	save(addr   , max >> 8);			// max high save
	save(addr +1, max     );			// max low  save
	save(addr +2, min >> 8);			// min high save
	save(addr +3, min     );			// min low  save
}
//[*]----------------------------------------------------------------------[*]
word maxmin_load(byte addr, byte load_data)
{
	word	ret_data = 0;

	watch();
	if         (load_data == MAX_LOAD)	{
		ret_data  = load(addr   ) << 8;	// max high load
		ret_data |= load(addr +1)     ;	// max low  load
		return		ret_data;
	}	else if(load_data == MIN_LOAD)	{
		ret_data  = load(addr +2) << 8;	// min high load
		ret_data |= load(addr +3)     ;	// min low  load
		return		ret_data;
	}	else		return 0;
}
//[*]----------------------------------------------------------------------[*]
word adc_read(void)
{
	word	i = 0, ret_value = 0;

	watch();

	delay(150);		output(ADC_MSB, 0xff);		delay(10) ;
	while(ADC_STATUS)	{	if(i++ > 100)		break;	}

	disable(EA);
	ret_value  = input(ADC_MSB) <<4;	ret_value  |= input(ADC_LSB) >>4;
	enable (EA);

	return		(word)(ret_value);
}
//[*]----------------------------------------------------------------------[*]
void wind_dir_func(byte cal_mode)
{
	word	new_value = 0, old_value = 0, i = 0;
	byte	pros_flag = 0;
	float	wd_real = 0., flag = 0.;
	double	aver = 0.;

	watch();
	switch(cal_mode)	{
		case	REAL:
			WD_SEL();	proc_grp.real[WD].value = adc_read();	TEMP_SEL();

			if(proc_grp.real[WD].value >= 4005)		proc_grp.real[WD].value = 4005;

			if      (proc_grp.real[WD].value == 0) 	proc_grp.real[WD].avr = 0;
			else    {
				for(i=0; i<16; i++)	{
					flag += 22.5;

					// -------------------------------
					new_value = A_wd_direction[i] - 90;	// AD REFERANCE ERROR


					if(old_value < proc_grp.real[WD].value && proc_grp.real[WD].value <= new_value)	{
						pros_flag = 1;
						wd_real = (float)((flag - (flag - 22.5)) / (float)(new_value - old_value));
						proc_grp.real[WD].avr =
							(word)(float)(((wd_real * (float)proc_grp.real[WD].value) - (wd_real * (float)old_value) + (flag - 22.5)) * 10.);
						i = END_CAL;
					}	else	old_value = new_value;
				}
				if(pros_flag == 0)		proc_grp.real[WD].avr = 3599;
			}

			aver = (double)((float)proc_grp.real[WD].avr/10.) * (3.141592/180.);

			proc_grp.real [WD].sin  = sin(aver);	proc_grp.real [WD].cos = cos(aver);
			proc_grp.min10[WD].sin += proc_grp.real [WD].sin;
			proc_grp.min10[WD].cos += proc_grp.real [WD].cos;
			proc_grp.hour [WD].sin += proc_grp.real [WD].sin;
			proc_grp.hour [WD].cos += proc_grp.real [WD].cos;

			aver = (double)(atan(proc_grp.real [WD].sin/proc_grp.real [WD].cos)) * (180./3.141592);

			if     (proc_grp.real [WD].sin < 0 && proc_grp.real [WD].cos > 0)		aver += 360.;
			else if(proc_grp.real [WD].sin > 0 && proc_grp.real [WD].cos < 0)		aver += 180.;
			else if(proc_grp.real [WD].sin < 0 && proc_grp.real [WD].cos < 0)		aver += 180.;
			proc_grp.real[WD].avr = (word)(float)(aver * 10.);

			// POWER ON initiliaze..
			if(init.flag.wd)	{
				init.flag.wd = OFF;

				save_grp.min10[WD].avr = proc_grp.real[WD].avr;
				save_grp.hour [WD].avr = proc_grp.real[WD].avr;

				proc_grp.real [WD].max = proc_grp.real[WD].avr;
				proc_grp.real [WD].min = proc_grp.real[WD].avr;
			}
			save_grp.real [WD].avr = proc_grp.real[WD].avr;
			save_grp.real [WD].max = proc_grp.real[WD].max;
			save_grp.real [WD].min = proc_grp.real[WD].min;
			break;
		case 	MIN10:
			watch();
			aver = (double)(atan(proc_grp.min10[WD].sin / proc_grp.min10[WD].cos)) * (180./3.141592);
			if     (proc_grp.min10[WD].sin < 0 && proc_grp.min10[WD].cos > 0)	aver += 360.;
			else if(proc_grp.min10[WD].sin > 0 && proc_grp.min10[WD].cos < 0)	aver += 180.;
			else if(proc_grp.min10[WD].sin < 0 && proc_grp.min10[WD].cos < 0)	aver += 180.;
			proc_grp.min10[WD].sin = 0.;	proc_grp.min10[WD].cos = 0.;
			proc_grp.min10[WD].avr = (word)(float)(aver * 10.);
			save_grp.min10[WD].avr = (word)(float)(aver * 10.);
			break;
		case	HOUR:
			watch();
			aver = (double)(atan(proc_grp.hour [WD].sin / proc_grp.hour [WD].cos)) * (180./3.141592);
			if     (proc_grp.hour [WD].sin < 0 && proc_grp.hour [WD].cos > 0)	aver += 360.;
			else if(proc_grp.hour [WD].sin > 0 && proc_grp.hour [WD].cos < 0)	aver += 180.;
			else if(proc_grp.hour [WD].sin < 0 && proc_grp.hour [WD].cos < 0)	aver += 180.;
			proc_grp.hour [WD].sin = 0.;	proc_grp.hour [WD].cos = 0.;
			proc_grp.hour [WD].avr = (word)(float)(aver * 10.);
			save_grp.hour [WD].avr = (word)(float)(aver * 10.);
			break;
		default:                break;
	}
}
//[*]----------------------------------------------------------------------[*]
void wind_spd_func(byte cal_mode)
{
	word	new_range = 0, new_speed = 0, old_range = 0, old_speed = 0, i = 0;
	byte	cal_flag = 1;
	float	range = 0;

	watch();
	switch(cal_mode)	{
		case	REAL:
			TR0 = STOP;
			proc_grp.real[WS].value  = TH0 <<8;
			proc_grp.real[WS].value |= TL0    ;
			TH0 = 0;			TL0 = 0;
			TR0 = RUN;

			if     (proc_grp.real[WS].value == 0)	proc_grp.real[WS].avr = 0;
			else 	{
				for(i=0; i<16; i++)	{
					if(Speed) 		new_speed = A_ws_speed_38[i];
					else            new_speed = A_ws_speed_45[i];

					new_range = A_ws_range[i];
					range = (float)(new_range - old_range)/(float)(new_speed - old_speed);

					if(old_speed <= proc_grp.real[WS].value &&
					   new_speed >  proc_grp.real[WS].value )	{
						proc_grp.real[WS].avr =
							(word)(float)(((float)old_range + (float)(proc_grp.real[WS].value - old_speed) * range) * 10.);
						cal_flag = 1;	i = END_CAL;
					}	else	{
						cal_flag = 0;
						old_speed = new_speed;	old_range = new_range;
					}
				}
			}
			// POWER ON initiliaze..
			if(init.flag.ws)	{
				init.flag.ws = OFF;
				save_grp.min10[WS].avr = proc_grp.real[WS].avr;
				save_grp.hour [WS].avr = proc_grp.real[WS].avr;
				proc_grp.real [WS].max = proc_grp.real[WS].avr;
				proc_grp.real [WS].min = proc_grp.real[WS].avr;
			}

			if(cal_flag)   {
				if     (proc_grp.real[WS].max < proc_grp.real[WS].avr)	{
					proc_grp.real[WS].max = proc_grp.real[WS].avr;
					proc_grp.real[WD].max = proc_grp.real[WD].avr;
				}
				else if(proc_grp.real[WS].min > proc_grp.real[WS].avr)	{
					proc_grp.real[WS].min = proc_grp.real[WS].avr;
					proc_grp.real[WD].min = proc_grp.real[WD].avr;
				}
				maxmin_save(WD_MAXMIN, proc_grp.real[WD].max, proc_grp.real[WD].min);
				maxmin_save(WS_MAXMIN, proc_grp.real[WS].max, proc_grp.real[WS].min);

				proc_grp.min10[WS].proc_count++;
				proc_grp.hour [WS].proc_count++;
				proc_grp.min10[WS].sum += (long)(proc_grp.real[WS].avr);
				proc_grp.hour [WS].sum += (long)(proc_grp.real[WS].avr);
				Chekam_ws = (((float)proc_grp.real[WS].avr / 10.));
			}
			save_grp.real[WS].avr = proc_grp.real[WS].avr;
			save_grp.real[WS].max = proc_grp.real[WS].max;
			save_grp.real[WS].min = proc_grp.real[WS].min;
			break;
		case	MIN10:
			watch();
			if(proc_grp.min10[WS].proc_count != 0)	{
				proc_grp.min10[WS].avr = (word)(float)((float)proc_grp.min10[WS].sum / (float)proc_grp.min10[WS].proc_count);
				save_grp.min10[WS].avr = (word)(float)((float)proc_grp.min10[WS].sum / (float)proc_grp.min10[WS].proc_count);
				proc_grp.min10[WS].proc_count = 0;
				proc_grp.min10[WS].sum = 0.;
			}
			break;
		case	HOUR:
			watch();
			if(proc_grp.hour [WS].proc_count != 0)	{
				proc_grp.hour [WS].avr = (word)(float)((float)proc_grp.hour [WS].sum / (float)proc_grp.hour [WS].proc_count);
				save_grp.hour [WS].avr = (word)(float)((float)proc_grp.hour [WS].sum / (float)proc_grp.hour [WS].proc_count);
				proc_grp.hour [WS].proc_count = 0;
				proc_grp.hour [WS].sum = 0.;
			}
			break;
		default:							return;
	}
}
//[*]----------------------------------------------------------------------[*]
void temper_func(byte cal_mode)
{
	byte    cal_flag = 1;
	word	ad_data  = 0;

	watch();
	switch(cal_mode)	{
		case	REAL:
			TEMP_SEL();		ad_data = adc_read();	HUMI_SEL();
			proc_grp.real[TEMP].value = (word)(float)(((float)ad_data / 4.095) + 500.);

			if(proc_grp.real[TEMP].value == 1000)	{
				Bul_temp = 0.;	Chekam_temp = 0.;
			}	else	{
				Chekam_temp = (float)(((float)proc_grp.real[TEMP].value - 1000.) / 10.);
				Bul_temp 	= (word)Chekam_temp;
			}

			if(proc_grp.real[TEMP].value == 0)	proc_grp.real[TEMP].avr = 0;
			else	{
				proc_grp.real[TEMP].avr = proc_grp.real[TEMP].value;

				if(proc_grp.real[TEMP].avr > 1499)	{
					proc_grp.real[TEMP].avr = 1500;	cal_flag = 0;
				}	else	if(proc_grp.real[TEMP].avr < 501)	{
					proc_grp.real[TEMP].avr = 500;	cal_flag = 0;
				}
			}
			// POWER ON initiliaze..
			if(init.flag.temp)	{
				init.flag.temp = OFF;
				save_grp.min10[TEMP].avr = proc_grp.real[TEMP].avr;
				save_grp.hour [TEMP].avr = proc_grp.real[TEMP].avr;
				proc_grp.real [TEMP].max = proc_grp.real[TEMP].avr;
				proc_grp.real [TEMP].min = proc_grp.real[TEMP].avr;
			}

			// Max Min Error Pros
			if(proc_grp.real[TEMP].max >= 1500)
				proc_grp.real[TEMP].max = proc_grp.real[TEMP].avr;

			if(proc_grp.real[TEMP].min <= 500 )
				proc_grp.real[TEMP].min = proc_grp.real[TEMP].avr;

			if(cal_flag)	{
				proc_grp.min10[TEMP].proc_count++;
				proc_grp.hour [TEMP].proc_count++;
				proc_grp.min10[TEMP].sum += (long)(proc_grp.real[TEMP].avr);
				proc_grp.hour [TEMP].sum += (long)(proc_grp.real[TEMP].avr);
				// Max Min proc
				if(proc_grp.real[TEMP].max < proc_grp.real[TEMP].avr)
					proc_grp.real[TEMP].max = proc_grp.real[TEMP].avr;
				if(proc_grp.real[TEMP].min > proc_grp.real[TEMP].avr)
					proc_grp.real[TEMP].min = proc_grp.real[TEMP].avr;
				maxmin_save(TEMP_MAXMIN, proc_grp.real[TEMP].max, proc_grp.real[TEMP].min);
			}

			save_grp.real[TEMP].avr = proc_grp.real[TEMP].avr;
			save_grp.real[TEMP].max = proc_grp.real[TEMP].max;
			save_grp.real[TEMP].min = proc_grp.real[TEMP].min;
			break;
		case	MIN10:
			watch();
			if(proc_grp.min10[TEMP].proc_count != 0)	{
				proc_grp.min10[TEMP].avr = (word)(float)((float)proc_grp.min10[TEMP].sum / (float)proc_grp.min10[TEMP].proc_count);
				save_grp.min10[TEMP].avr = (word)(float)((float)proc_grp.min10[TEMP].sum / (float)proc_grp.min10[TEMP].proc_count);
				proc_grp.min10[TEMP].proc_count = 0;
				proc_grp.min10[TEMP].sum = 0.;
			}
			break;
		case	HOUR:
			watch();
			if(proc_grp.hour[TEMP].proc_count != 0)	{
				proc_grp.hour[TEMP].avr = (word)(float)((float)proc_grp.hour[TEMP].sum / (float)proc_grp.hour[TEMP].proc_count);
				save_grp.hour[TEMP].avr = (word)(float)((float)proc_grp.hour[TEMP].sum / (float)proc_grp.hour[TEMP].proc_count);
				proc_grp.hour[TEMP].proc_count = 0;
				proc_grp.hour[TEMP].sum = 0.;
			}
			break;
		default:							return;
	}
}
//[*]----------------------------------------------------------------------[*]
void humidt_func(byte cal_mode)
{
	byte	cal_flag = 1;
	word	ad_data  = 0;
	watch();
	switch(cal_mode)	{
		case	REAL:
			HUMI_SEL();		ad_data = adc_read();	WD_SEL();
			proc_grp.real[HUMI].value = (word)(float)((float)ad_data/4.095) ;

			if(proc_grp.real[HUMI].value == 0)	Bul_humi = 0;
			else
				Bul_humi = (word)(float)((float)proc_grp.real[HUMI].value / 10.);

			if(proc_grp.real[HUMI].value == 0)	proc_grp.real[HUMI].avr = 0;
			else	{
				proc_grp.real[HUMI].avr = proc_grp.real[HUMI].value;

				if(proc_grp.real[HUMI].avr >= 1000)	{
					proc_grp.real[HUMI].avr = 999;	cal_flag = 0;
				}	else if(proc_grp.real[HUMI].avr < 20)	{
					proc_grp.real[HUMI].avr = 0;	cal_flag = 0;
				}
			}
			// POWER ON initiliaze..
			if(init.flag.humi)	{
				init.flag.humi = OFF;
				save_grp.min10[HUMI].avr = proc_grp.real [HUMI].avr;
				save_grp.hour [HUMI].avr = proc_grp.real [HUMI].avr;
				proc_grp.real [HUMI].max = proc_grp.real [HUMI].avr;
				proc_grp.real [HUMI].min = proc_grp.real [HUMI].avr;
			}

			// Max Min Error proc
			if(proc_grp.real[HUMI].max >= 1000)	proc_grp.real[HUMI].max = proc_grp.real[HUMI].avr;
			if(proc_grp.real[HUMI].min <= 0	  )	proc_grp.real[HUMI].min = proc_grp.real[HUMI].avr;

			if(cal_flag)   {
				if(proc_grp.real[HUMI].max < proc_grp.real[HUMI].avr)
					proc_grp.real[HUMI].max = proc_grp.real[HUMI].avr;
				if(proc_grp.real[HUMI].min > proc_grp.real[HUMI].avr)
					proc_grp.real[HUMI].min = proc_grp.real[HUMI].avr;

				proc_grp.min10[HUMI].proc_count++;
				proc_grp.hour [HUMI].proc_count++;
				proc_grp.min10[HUMI].sum += (long)(proc_grp.real[HUMI].avr);
				proc_grp.hour [HUMI].sum += (long)(proc_grp.real[HUMI].avr);
				// Max Min Save
				maxmin_save(HUMI_MAXMIN, proc_grp.real[HUMI].max, proc_grp.real[HUMI].min);
			}

			save_grp.real[HUMI].avr = proc_grp.real[HUMI].avr;
			save_grp.real[HUMI].max = proc_grp.real[HUMI].max;
			save_grp.real[HUMI].min = proc_grp.real[HUMI].min;
			break;
		case	MIN10:
			watch();
			if(proc_grp.min10[HUMI].proc_count != 0)	{
				proc_grp.min10[HUMI].avr = (word)(float)((float)proc_grp.min10[HUMI].sum / (float)proc_grp.min10[HUMI].proc_count);
				save_grp.min10[HUMI].avr = (word)(float)((float)proc_grp.min10[HUMI].sum / (float)proc_grp.min10[HUMI].proc_count);
				proc_grp.min10[HUMI].proc_count = 0;
				proc_grp.min10[HUMI].sum = 0.;
			}
			break;
		case	HOUR:
			watch();
			if(proc_grp.hour[HUMI].proc_count != 0)	{
				proc_grp.hour[HUMI].avr = (word)(float)((float)proc_grp.hour[HUMI].sum / (float)proc_grp.hour[HUMI].proc_count);
				save_grp.hour[HUMI].avr = (word)(float)((float)proc_grp.hour[HUMI].sum / (float)proc_grp.hour[HUMI].proc_count);
				proc_grp.hour[HUMI].proc_count = 0;
				proc_grp.hour[HUMI].sum = 0.;
			}
			break;
		default:							return;
	}
}
//[*]----------------------------------------------------------------------[*]
void precipi_func(byte cal_mode)
{
	watch();

	switch(cal_mode)	{
		case	REAL:
			if(input(PRE_ADDR) != 0)	{
				proc_grp.real [PRE].avr += 5 ;
				proc_grp.min10[PRE].sum += 5.;
				proc_grp.hour [PRE].sum += 5.;
			}
			proc_grp.real[PRE].max = proc_grp.real[PRE].min = proc_grp.real[PRE].avr;

			maxmin_save(PRE_MAXMIN, proc_grp.real[PRE].max, proc_grp.real[PRE].min);
			PRE_CLEAR = ON;		delay(100);		PRE_CLEAR = OFF;

			// POWER ON initiliaze..
			if(init.flag.pre)		{
				init.flag.pre = OFF;
				save_grp.min10[PRE].avr = 0;
				save_grp.hour [PRE].avr = 0;
			}

			save_grp.real[PRE].avr = proc_grp.real[PRE].avr;
			save_grp.real[PRE].max = proc_grp.real[PRE].max;
			save_grp.real[PRE].min = proc_grp.real[PRE].min;
			break;
		case	MIN10:
			proc_grp.min10[PRE].avr = (word)(proc_grp.min10[PRE].sum);
			save_grp.min10[PRE].avr = (word)(proc_grp.min10[PRE].sum);
			proc_grp.min10[PRE].sum = 0.;
			break;
		case	HOUR:
			proc_grp.hour [PRE].avr = (word)(proc_grp.hour [PRE].sum);
			save_grp.hour [PRE].avr = (word)(proc_grp.hour [PRE].sum);
			proc_grp.hour [PRE].sum = 0.;
			break;
		default:							return;
	}
}
//[*]----------------------------------------------------------------------[*]
void yst_pre_func(byte cal_mode)
{
	watch();
	switch(cal_mode)	{
		case	REAL:
		case	MIN10:
		case	HOUR:
			// POWER ON initiliaze..
			if(init.flag.ypre)		init.flag.ypre = OFF;

			proc_grp.real [YPRE].avr = maxmin_load(YPRE_MAXMIN, MAX_LOAD);
			proc_grp.real [YPRE].max = proc_grp.real[YPRE].avr;
			proc_grp.real [YPRE].min = proc_grp.real[YPRE].avr;
			proc_grp.min10[YPRE].avr = proc_grp.real[YPRE].avr;
			proc_grp.hour [YPRE].avr = proc_grp.real[YPRE].avr;

			save_grp.real [YPRE].avr = proc_grp.real[YPRE].avr;
			save_grp.real [YPRE].max = proc_grp.real[YPRE].avr;
			save_grp.real [YPRE].min = proc_grp.real[YPRE].avr;
			save_grp.min10[YPRE].avr = proc_grp.real[YPRE].avr;
			save_grp.hour [YPRE].avr = proc_grp.real[YPRE].avr;

			// Max Min Save
			maxmin_save(YPRE_MAXMIN, proc_grp.real[YPRE].max, proc_grp.real[YPRE].min);
			break;
		default		:				return;
	}
}
//[*]----------------------------------------------------------------------[*]
double dewpoint(double Temperature, double Humidity)
{
	double E  = Humidity * 0.0611 * pow(10, ((7.5*Temperature) / (237.3+Temperature)));
	double DP = (237.3 * log10(E) - 186.527) / (8.286 - log10(E));
	return DP;
}
//[*]----------------------------------------------------------------------[*]
float bul_calculater(double temp, double humi)
{
	double t, td, tf, tt, ttd, a, b, c, rh, di;

	t   	= temp;
	td  	= dewpoint(temp, humi);
	tf  	= 9./5. * t  + 32;
	tt  	= t  + 273.15;
	ttd 	= td + 273.15;
	a   	= tt - ttd;
	b   	= tt * ttd;
	c   	= 4.25 * pow(10.,-4.);
	rh      = pow(10.,(-a / (b * c)));
	di      = tf - 0.55 * (1. - rh) * (tf - 58.);

	return (float)di;
}
//[*]----------------------------------------------------------------------[*]
void bul_jisu_func(byte cal_mode)
{
	byte	cal_flag = 1;

	watch();
	switch(cal_mode)	{
		case    REAL:
			proc_grp.real[BUL].avr =
				(word)((bul_calculater((double)Bul_temp,
									   (double)Bul_humi)) * 10.);

			if(proc_grp.real[BUL].avr >= 1000)	{
				proc_grp.real[BUL].avr = 999;	cal_flag = 0;
			}	else if(proc_grp.real[BUL].avr < 20   )	{
				proc_grp.real[BUL].avr = 0;		cal_flag = 0;
			}

			// POWER ON initiliaze..
			if(init.flag.bul)	{
				init.flag.bul = OFF;
				save_grp.min10[BUL].avr = proc_grp.real[BUL].avr;
				save_grp.hour [BUL].avr = proc_grp.real[BUL].avr;
				proc_grp.real [BUL].max = proc_grp.real[BUL].avr;
				proc_grp.real [BUL].min = proc_grp.real[BUL].avr;
			}

			// Max Min Error Pros..
			if(proc_grp.real[BUL].max >= 1000)	proc_grp.real[BUL].max = proc_grp.real[BUL].avr;
			if(proc_grp.real[BUL].max <=    0)	proc_grp.real[BUL].min = proc_grp.real[BUL].avr;

			if(cal_flag)	{
				if(proc_grp.real[BUL].max < proc_grp.real[BUL].avr)
					proc_grp.real[BUL].max = proc_grp.real[BUL].avr;
				if(proc_grp.real[BUL].min > proc_grp.real[BUL].avr)
					proc_grp.real[BUL].min = proc_grp.real[BUL].avr;

				proc_grp.min10[BUL].proc_count++;
				proc_grp.hour [BUL].proc_count++;
				proc_grp.min10[BUL].sum += (long)(proc_grp.real[BUL].avr);
				proc_grp.hour [BUL].sum += (long)(proc_grp.real[BUL].avr);
				// Max Min Save
				maxmin_save(BUL_MAXMIN, proc_grp.real[BUL].max, proc_grp.real[BUL].min);
			}

			save_grp.real[BUL].avr = proc_grp.real[BUL].avr;
			save_grp.real[BUL].max = proc_grp.real[BUL].max;
			save_grp.real[BUL].min = proc_grp.real[BUL].min;
			break;
		case	MIN10:
			watch();
			if(proc_grp.min10[BUL].proc_count != 0)	{
				proc_grp.min10[BUL].avr = (word)(float)((float)proc_grp.min10[BUL].sum / (float)proc_grp.min10[BUL].proc_count);
				save_grp.min10[BUL].avr = (word)(float)((float)proc_grp.min10[BUL].sum / (float)proc_grp.min10[BUL].proc_count);
				proc_grp.min10[BUL].proc_count = 0;
				proc_grp.min10[BUL].sum = 0.;
			}
			break;
		case	HOUR:
			watch();
			if(proc_grp.hour [BUL].proc_count != 0)	{
				proc_grp.hour [BUL].avr = (word)(float)((float)proc_grp.hour[BUL].sum / (float)proc_grp.hour[BUL].proc_count);
				save_grp.hour [BUL].avr = (word)(float)((float)proc_grp.hour[BUL].sum / (float)proc_grp.hour[BUL].proc_count);
				proc_grp.hour [BUL].proc_count = 0;
				proc_grp.hour [BUL].sum = 0.;
			}
			break;
		default:							return;
	}
}
//[*]----------------------------------------------------------------------[*]
//   1998, 3, 19 Upgrade
//[*]----------------------------------------------------------------------[*]
//float chekamtemp_cal(float windspd,float temp)
//{
//	float tt;
//	tt = 33. - ((10.45 + 10 * (float)( sqrt( (double)windspd ) - (double)windspd)) * (33. -temp) / 20.04);
//	return tt;
//}
/*(=)--------------------------------------------------------------------(=)*/
float chekamtemp_cal(float windspd,float temp)
{
	float tt;
	tt = 33. - (0.047 * (33. - temp) * (10.45 + 8.69 * (float)(sqrt((double)windspd)) - 0.755 * windspd));
	return tt;
}
/*(=)--------------------------------------------------------------------(=)*/
void chekam_func(byte cal_mode)
{
	byte	cal_flag = 1;

	watch();
	switch(cal_mode)	{
		case    REAL:
			proc_grp.real[CHE].avr =
				(word)(float)((chekamtemp_cal((float)Chekam_ws,(float)Chekam_temp) + 100.) * 10.);

			if(proc_grp.real[CHE].avr >= 1500)	{
				proc_grp.real[CHE].avr = 1500;		cal_flag = 0;
			}	else if(proc_grp.real[CHE].avr <= 500)	{
				proc_grp.real[CHE].avr = 500;		cal_flag = 0;
			}

			// POWER ON initiliaze..
			if(init.flag.chekam)	{
				init.flag.chekam = OFF;
				save_grp.min10[CHE].avr = proc_grp.real[CHE].avr;
				save_grp.hour [CHE].avr = proc_grp.real[CHE].avr;
				proc_grp.real [CHE].max = proc_grp.real[CHE].avr;
				proc_grp.real [CHE].min = proc_grp.real[CHE].avr;
			}

			if(cal_flag)   {
				if(proc_grp.real[CHE].max < proc_grp.real[CHE].avr)
					proc_grp.real[CHE].max = proc_grp.real[CHE].avr;
				if(proc_grp.real[CHE].min > proc_grp.real[CHE].avr)
					proc_grp.real[CHE].min = proc_grp.real[CHE].avr;

				proc_grp.min10[CHE].proc_count++;
				proc_grp.hour [CHE].proc_count++;
				proc_grp.min10[CHE].sum += (long)(proc_grp.real[CHE].avr);
				proc_grp.hour [CHE].sum += (long)(proc_grp.real[CHE].avr);

				// Max Min Save
				maxmin_save(CHEKAM_MAXMIN, proc_grp.real[CHE].max, proc_grp.real[CHE].min);
			}

			save_grp.real[CHE].avr = proc_grp.real[CHE].avr;
			save_grp.real[CHE].max = proc_grp.real[CHE].max;
			save_grp.real[CHE].min = proc_grp.real[CHE].min;
			break;
		case	MIN10:
			watch();
			if(proc_grp.min10[CHE].proc_count != 0)	{
				proc_grp.min10[CHE].avr = (word)(float)((float)proc_grp.min10[CHE].sum / (float)proc_grp.min10[CHE].proc_count);
				save_grp.min10[CHE].avr = (word)(float)((float)proc_grp.min10[CHE].sum / (float)proc_grp.min10[CHE].proc_count);
				proc_grp.min10[CHE].proc_count = 0;
				proc_grp.min10[CHE].sum = 0.;
			}
			break;
		case	HOUR:
			watch();
			if(proc_grp.hour[CHE].proc_count != 0)	{
				proc_grp.hour[CHE].avr = (word)(float)((float)proc_grp.hour[CHE].sum / (float)proc_grp.hour[CHE].proc_count);
				save_grp.hour[CHE].avr = (word)(float)((float)proc_grp.hour[CHE].sum / (float)proc_grp.hour[CHE].proc_count);
				proc_grp.hour[CHE].proc_count = 0;
				proc_grp.hour[CHE].sum = 0.;
			}
			break;
		default:							return;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]




