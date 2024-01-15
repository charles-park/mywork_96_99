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

			aver = (double)(atan(proc_grp.real [WD].sin/proc_grp.real [WD].cos)) * (180./3.141592);

			if     (proc_grp.real [WD].sin < 0 && proc_grp.real [WD].cos > 0)		aver += 360.;
			else if(proc_grp.real [WD].sin > 0 && proc_grp.real [WD].cos < 0)		aver += 180.;
			else if(proc_grp.real [WD].sin < 0 && proc_grp.real [WD].cos < 0)		aver += 180.;
			proc_grp.real[WD].avr = (word)(float)(aver * 10.);

			// POWER ON initiliaze..
			if(init.flag.wd)	{
				init.flag.wd = OFF;
				proc_grp.real [WD].max = proc_grp.real[WD].avr;
				proc_grp.real [WD].min = proc_grp.real[WD].avr;
			}
			save_grp.real [WD].avr = proc_grp.real[WD].avr;
			save_grp.real [WD].max = proc_grp.real[WD].max;
			save_grp.real [WD].min = proc_grp.real[WD].min;
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
			}
			save_grp.real[WS].avr = proc_grp.real[WS].avr;
			save_grp.real[WS].max = proc_grp.real[WS].max;
			save_grp.real[WS].min = proc_grp.real[WS].min;
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
			TEMP_SEL();		ad_data = adc_read();	WD_SEL();
			proc_grp.real[TEMP].value = (word)(float)(((float)ad_data / 4.095) + 500.);

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
				proc_grp.real [TEMP].max = proc_grp.real[TEMP].avr;
				proc_grp.real [TEMP].min = proc_grp.real[TEMP].avr;
			}

			// Max Min Error Pros
			if(proc_grp.real[TEMP].max >= 1500)
				proc_grp.real[TEMP].max = proc_grp.real[TEMP].avr;

			if(proc_grp.real[TEMP].min <= 500 )
				proc_grp.real[TEMP].min = proc_grp.real[TEMP].avr;

			if(cal_flag)	{
				// Max Min proc
				if(proc_grp.real[TEMP].max < proc_grp.real[TEMP].avr)
					proc_grp.real[TEMP].max = proc_grp.real[TEMP].avr;
				if(proc_grp.real[TEMP].min > proc_grp.real[TEMP].avr)
					proc_grp.real[TEMP].min = proc_grp.real[TEMP].avr;
			}

			save_grp.real[TEMP].avr = proc_grp.real[TEMP].avr;
			save_grp.real[TEMP].max = proc_grp.real[TEMP].max;
			save_grp.real[TEMP].min = proc_grp.real[TEMP].min;
			break;
		default:							return;
	}
}
//[*]----------------------------------------------------------------------[*]



