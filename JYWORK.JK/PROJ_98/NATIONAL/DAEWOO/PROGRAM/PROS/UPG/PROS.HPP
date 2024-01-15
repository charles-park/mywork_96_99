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

			// A/D Refferance Error
			if(proc_grp.real[WD].value >= 3920)		proc_grp.real[WD].value = 3920;

			proc_grp.real[WD].value =
				(word)(((float)proc_grp.real[WD].value / 3920.) * 4095.);
			if     (proc_grp.real[WD].value > 4094)	proc_grp.real[WD].value = 4095;
			else if(proc_grp.real[WD].value < 2   )	proc_grp.real[WD].value = 0;

			if      (proc_grp.real[WD].value == 0) 	proc_grp.real[WD].avr = 0;
			else    {
				for(i=0; i<16; i++)	{
					flag += 22.5;
					new_value = A_wd_direction[i];

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

			if(proc_grp.real[WD].avr != Wd_comp_data)	{
				Wd_comp_data = proc_grp.real[WD].avr;	Wd_sensor_err = 0;
			}	else									Wd_sensor_err ++;

			// 6 Hour
			if(Wd_sensor_err > WD_SENSOR_WAIT)	{
				save_grp.real [WD].sta = save_grp.min10[WD].sta =
				save_grp.hour [WD].sta = 0x02;
			}	else	{
				save_grp.real [WD].sta = save_grp.min10[WD].sta =
				save_grp.hour [WD].sta = 0x00;
			}

			// POWER ON initiliaze..
			if(init.flag.wd)	{
				init.flag.wd = OFF;
				save_grp.min10[WD].avr = proc_grp.real[WD].avr;
				save_grp.hour [WD].avr = proc_grp.real[WD].avr;
			}
			save_grp.real [WD].avr  = proc_grp.real[WD].avr;
			save_grp.real [WD].scod = 0x00;
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
			save_grp.min10[WD].scod = 0x00;
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
			save_grp.hour [WD].scod = 0x00;
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

			if(cal_flag)   {
				// POWER ON initiliaze..
				if(init.flag.ws)	{
					init.flag.ws = OFF;
					save_grp.min10[WS].avr = proc_grp.real[WS].avr;
					save_grp.hour [WS].avr = proc_grp.real[WS].avr;
				}
				proc_grp.min10[WS].proc_count++;
				proc_grp.hour [WS].proc_count++;
				proc_grp.min10[WS].sum += (long)(proc_grp.real[WS].avr);
				proc_grp.hour [WS].sum += (long)(proc_grp.real[WS].avr);
			}	else	{
				save_grp.real [WS].sta  = 0x02;
				save_grp.min10[WS].sta  = 0x02;
				save_grp.hour [WS].sta  = 0x02;
			}	save_grp.real [WS].avr  = proc_grp.real[WS].avr;

			if(proc_grp.real[WS].avr != Ws_comp_data)	{
				Ws_comp_data = proc_grp.real[WS].avr;	Ws_sensor_err = 0;
			}	else									Ws_sensor_err++  ;

			// 6 Hour
			if(Ws_sensor_err > WS_SENSOR_WAIT)	{
				save_grp.real [WS].sta  = 0x02;
				save_grp.min10[WS].sta  = 0x02;
				save_grp.hour [WS].sta  = 0x02;
			}	else	{
				save_grp.real [WS].sta  = 0x00;
				save_grp.min10[WS].sta  = 0x00;
				save_grp.hour [WS].sta  = 0x00;
			}	save_grp.real [WS].scod = 0x01;
			break;
		case	MIN10:
			watch();
			if(proc_grp.min10[WS].proc_count != 0)	{
				proc_grp.min10[WS].avr  = (word)(float)((float)proc_grp.min10[WS].sum / (float)proc_grp.min10[WS].proc_count);
				save_grp.min10[WS].avr  = (word)(float)((float)proc_grp.min10[WS].sum / (float)proc_grp.min10[WS].proc_count);
				save_grp.real [WS].scod = 0x01;
				proc_grp.min10[WS].proc_count = 0;
				proc_grp.min10[WS].sum = 0.;
			}
			break;
		case	HOUR:
			watch();
			if( proc_grp.hour [WS].proc_count != 0)	{
				proc_grp.hour [WS].avr = (word)(float)((float)proc_grp.hour [WS].sum / (float)proc_grp.hour [WS].proc_count);
				save_grp.hour [WS].avr = (word)(float)((float)proc_grp.hour [WS].sum / (float)proc_grp.hour [WS].proc_count);
				save_grp.hour [WS].scod = 0x01;
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
			proc_grp.real[TEMP].avr = proc_grp.real[TEMP].value;

			if(proc_grp.real[TEMP].avr > 1499)	{
				proc_grp.real[TEMP].avr = 1500;	cal_flag = 0;
			}	else	if(proc_grp.real[TEMP].avr < 501)	{
				proc_grp.real[TEMP].avr = 500;	cal_flag = 0;
			}	else	if(init.flag.temp)	{
				// POWER ON initiliaze..
				init.flag.temp = OFF;
				save_grp.min10[TEMP].avr = proc_grp.real[TEMP].avr;
				save_grp.hour [TEMP].avr = proc_grp.real[TEMP].avr;
			}

			if(cal_flag)	{
				proc_grp.min10[TEMP].proc_count++;
				proc_grp.hour [TEMP].proc_count++;
				proc_grp.min10[TEMP].sum += (long)(proc_grp.real[TEMP].avr);
				proc_grp.hour [TEMP].sum += (long)(proc_grp.real[TEMP].avr);
				save_grp.real [TEMP].sta  = 0x00;
				save_grp.min10[TEMP].sta  = 0x00;
				save_grp.hour [TEMP].sta  = 0x00;
			}	else	{
				save_grp.real [TEMP].sta  = 0x02;
				save_grp.min10[TEMP].sta  = 0x02;
				save_grp.hour [TEMP].sta  = 0x02;
			}
			save_grp.real[TEMP].avr  = proc_grp.real[TEMP].avr;
			save_grp.real[TEMP].scod = 0x02;
			break;
		case	MIN10:
			watch();
			if(proc_grp.min10[TEMP].proc_count != 0)	{
				proc_grp.min10[TEMP].avr = (word)(float)((float)proc_grp.min10[TEMP].sum / (float)proc_grp.min10[TEMP].proc_count);
				save_grp.min10[TEMP].avr = (word)(float)((float)proc_grp.min10[TEMP].sum / (float)proc_grp.min10[TEMP].proc_count);
				save_grp.min10[TEMP].scod = 0x02;
				proc_grp.min10[TEMP].proc_count = 0;
				proc_grp.min10[TEMP].sum  = 0.;
			}	break;
		case	HOUR:
			watch();
			if(proc_grp.hour[TEMP].proc_count != 0)	{
				proc_grp.hour[TEMP].avr = (word)(float)((float)proc_grp.hour[TEMP].sum / (float)proc_grp.hour[TEMP].proc_count);
				save_grp.hour[TEMP].avr = (word)(float)((float)proc_grp.hour[TEMP].sum / (float)proc_grp.hour[TEMP].proc_count);
				save_grp.hour[TEMP].scod = 0x02;
				proc_grp.hour[TEMP].proc_count = 0;
				proc_grp.hour[TEMP].sum  = 0.;
			}	break;
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
			proc_grp.real[HUMI].avr   = proc_grp.real[HUMI].value;

			if(proc_grp.real[HUMI].value >= 1000)	{
				cal_flag = 0;	proc_grp.real[HUMI].value = 999;
			}	else if(proc_grp.real[HUMI].value <= 20)	{
				cal_flag = 0;   proc_grp.real[HUMI].value = 0;
			}	else if(init.flag.humi)	{
				// POWER ON initiliaze..
				init.flag.humi = OFF;
				save_grp.min10[HUMI].avr = proc_grp.real [HUMI].avr;
				save_grp.hour [HUMI].avr = proc_grp.real [HUMI].avr;
			}

			if(cal_flag)   {
				proc_grp.min10[HUMI].proc_count++;
				proc_grp.hour [HUMI].proc_count++;
				proc_grp.min10[HUMI].sum += (long)(proc_grp.real[HUMI].avr);
				proc_grp.hour [HUMI].sum += (long)(proc_grp.real[HUMI].avr);
				save_grp.real [HUMI].sta  = 0x00;
				save_grp.min10[HUMI].sta  = 0x00;
				save_grp.hour [HUMI].sta  = 0x00;
			}	else	{
				save_grp.real [HUMI].sta  = 0x02;
				save_grp.min10[HUMI].sta  = 0x02;
				save_grp.hour [HUMI].sta  = 0x02;
			}
			save_grp.real[HUMI].avr  = proc_grp.real[HUMI].avr;
			save_grp.real[HUMI].scod = 0x06;	break;
		case	MIN10:
			watch();
			if( proc_grp.min10[HUMI].proc_count != 0)	{
				proc_grp.min10[HUMI].avr = (word)(float)((float)proc_grp.min10[HUMI].sum / (float)proc_grp.min10[HUMI].proc_count);
				save_grp.min10[HUMI].avr = (word)(float)((float)proc_grp.min10[HUMI].sum / (float)proc_grp.min10[HUMI].proc_count);
				save_grp.min10[HUMI].scod = 0x06;
				proc_grp.min10[HUMI].proc_count = 0;
				proc_grp.min10[HUMI].sum  = 0.;
			}									break;
		case	HOUR:
			watch();
			if( proc_grp.hour[HUMI].proc_count != 0)	{
				proc_grp.hour[HUMI].avr = (word)(float)((float)proc_grp.hour[HUMI].sum / (float)proc_grp.hour[HUMI].proc_count);
				save_grp.hour[HUMI].avr = (word)(float)((float)proc_grp.hour[HUMI].sum / (float)proc_grp.hour[HUMI].proc_count);
				save_grp.hour[HUMI].scod = 0x06;
				proc_grp.hour[HUMI].proc_count = 0;
				proc_grp.hour[HUMI].sum  = 0.;
			}									break;
		default:								return;
	}
}
//[*]----------------------------------------------------------------------[*]
void pressure_func(byte cal_mode)
{
	byte	cal_flag = 1;

	watch();

	switch(cal_mode)	{
		case	REAL:
			proc_grp.real[PRESS].value = (word)(Press_catch) ;

			if     (proc_grp.real[PRESS].value >= 12000)
			{	cal_flag = 0;	proc_grp.real[PRESS].value  = 12000;	}
			else if(proc_grp.real[PRESS].value <= 8000)
			{	cal_flag = 0;	proc_grp.real[PRESS].value  = 8000;		}

			proc_grp.real [PRESS].avr = proc_grp.real[PRESS].value;

			if(cal_flag)   {
				if(init.flag.press)	{
					// POWER ON initiliaze..
					init.flag.press = OFF;
					save_grp.min10[PRESS].avr = proc_grp.real[PRESS].avr;
					save_grp.hour [PRESS].avr = proc_grp.real[PRESS].avr;
				}
				proc_grp.min10[PRESS].proc_count++;
				proc_grp.hour [PRESS].proc_count++;
				proc_grp.min10[PRESS].sum += (long)(proc_grp.real[PRESS].avr);
				proc_grp.hour [PRESS].sum += (long)(proc_grp.real[PRESS].avr);
				save_grp.real [PRESS].sta  = 0x00;
				save_grp.min10[PRESS].sta  = 0x00;
				save_grp.hour [PRESS].sta  = 0x00;
			}	else	{
			///////////
				if(init.flag.press)	{
					save_grp.real [PRESS].sta  = 0x08;
					save_grp.min10[PRESS].sta  = 0x08;
					save_grp.hour [PRESS].sta  = 0x08;
				}	else	{
					save_grp.real [PRESS].sta  = 0x02;
					save_grp.min10[PRESS].sta  = 0x02;
					save_grp.hour [PRESS].sta  = 0x02;
				}
			///////////
			}
			save_grp.real[PRESS].avr  = proc_grp.real[PRESS].avr;
			save_grp.real[PRESS].scod = 0x05;
			break;
		case	MIN10:
			watch();
			if( proc_grp.min10[PRESS].proc_count != 0)	{
				proc_grp.min10[PRESS].avr =
					(word)(float)((float)proc_grp.min10[PRESS].sum / (float)proc_grp.min10[PRESS].proc_count);
				save_grp.min10[PRESS].avr =
					(word)(float)((float)proc_grp.min10[PRESS].sum / (float)proc_grp.min10[PRESS].proc_count);
				save_grp.min10[PRESS].scod = 0x05;
				proc_grp.min10[PRESS].proc_count = 0;
				proc_grp.min10[PRESS].sum  = 0.;
			}
			break;
		case	HOUR:
			watch();
			if( proc_grp.hour[PRESS].proc_count != 0)	{
				proc_grp.hour[PRESS].avr =
					(word)(float)((float)proc_grp.hour[PRESS].sum / (float)proc_grp.hour[PRESS].proc_count);
				save_grp.hour[PRESS].avr =
					(word)(float)((float)proc_grp.hour[PRESS].sum / (float)proc_grp.hour[PRESS].proc_count);
				save_grp.hour[PRESS].scod = 0x05;
				proc_grp.hour[PRESS].proc_count = 0;
				proc_grp.hour[PRESS].sum  = 0.;
			}
			break;
		default:							return;
	}	Press_catch = 0.;
}
//[*]----------------------------------------------------------------------[*]





