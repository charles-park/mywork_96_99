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

	delay(150);			MUX_STATUS = HOLD;
	delay(10) ;			output(ADC_MSB, 0xff);		delay(10) ;
	while(ADC_STATUS)	{	if(i++ > 100)		break;	}

	ret_value  = input(ADC_MSB) <<4;	ret_value  |= input(ADC_LSB) >>4;
	MUX_STATUS = SAMPLE;

	return		(word)(ret_value);
}
//[*]----------------------------------------------------------------------[*]
void led16_pros(word value)
{
	watch();		led16_clr();	// define function
	if     ((value >= 3487.5) || (value < 112.5))  led16.bits.L15 = ON;
	else if((value >=  112.5) && (value < 337.5))  led16.bits.L14 = ON;
	else if((value >=  337.5) && (value < 562.5))  led16.bits.L13 = ON;
	else if((value >=  562.5) && (value < 787.5))  led16.bits.L12 = ON;
	else if((value >=  787.5) && (value <1012.5))  led16.bits.L11 = ON;
	else if((value >= 1012.5) && (value <1237.5))  led16.bits.L10 = ON;
	else if((value >= 1237.5) && (value <1462.5))  led16.bits.L9  = ON;
	else if((value >= 1462.5) && (value <1687.5))  led16.bits.L8  = ON;
	else if((value >= 1687.5) && (value <1912.5))  led16.bits.L1  = ON;
	else if((value >= 1912.5) && (value <2137.5))  led16.bits.L2  = ON;
	else if((value >= 2137.5) && (value <2362.5))  led16.bits.L3  = ON;
	else if((value >= 2362.5) && (value <2587.5))  led16.bits.L4  = ON;
	else if((value >= 2587.5) && (value <2812.5))  led16.bits.L5  = ON;
	else if((value >= 2812.5) && (value <3037.5))  led16.bits.L6  = ON;
	else if((value >= 3037.5) && (value <3262.5))  led16.bits.L7  = ON;
	else if((value >= 3262.5) && (value <3487.5))  led16.bits.L0  = ON;
	led16_disp();	// define function
}
//[*]----------------------------------------------------------------------[*]
void wind_direction_cal(byte cal_mode)
{
	word	new_value = 0, old_value = 0, i = 0;
	byte	pros_flag = 0;
	float	wd_real = 0., flag = 0.;
	double	aver = 0.;

	watch();
	switch(cal_mode)	{
		case	REAL:
			WD_SEL();		wd_pros.real_sum = adc_read();

			if      (wd_pros.real_sum == 0)         wd_pros.real_avr = 0.;
			else    {
				for(i=0; i<16; i++)	{
					flag += 22.5;
					new_value = A_wd_direction[i];
					if(old_value < wd_pros.real_sum && wd_pros.real_sum <= new_value)	{
						pros_flag = 1;
						wd_real = (float)((flag - (flag - 22.5))/(float)(new_value - old_value));
						wd_pros.real_avr =
							(word)(float)(((wd_real * (float)wd_pros.real_sum) - (wd_real * (float)old_value) + (flag + 22.5)) * 10.);
						i = END_CAL;
					}	else	old_value = new_value;
				}
				if(pros_flag == 0)		wd_pros.real_avr = 3599;
			}
			aver = (double)((float)wd_pros.real_avr/10.) * (3.141592/180.);

			wd_pros.real_sin   = sin(aver);         wd_pros.real_cos   = cos(aver);

			aver = (double)(atan(wd_pros.real_sin/wd_pros.real_cos)) * (180./3.141592);
			if     (wd_pros.real_sin < 0 && wd_pros.real_cos > 0)		aver += 360.;
			else if(wd_pros.real_sin > 0 && wd_pros.real_cos < 0)		aver += 180.;
			else if(wd_pros.real_sin < 0 && wd_pros.real_cos < 0)		aver += 180.;
			wd_pros.real_avr = (word)(float)(aver * 10.);

			// LED DISPLAY 16
			led16_pros(wd_pros.real_avr);
			break;
		default:                break;
	}
}
//[*]----------------------------------------------------------------------[*]
void wind_speed_ms_cal(byte cal_mode)
{
	word	new_range = 0, new_speed = 0, old_range = 0, old_speed = 0, i = 0;
	byte	cal_flag = 1;
	float	ms_range = 0;

	watch();
	switch(cal_mode)	{
		case	REAL:
			TR0 = STOP;
			ws_pros.real_sum = TH0 <<8;		ws_pros.real_sum |= TL0;
			TH0 = 0;						TL0 = 0;
			TR0 = RUN;

			if     (ws_pros.real_sum == 0)	ws_pros.real_avr = 0;
			else 	{
				for(i=0; i<16; i++)	{
					if(Read_id) 	new_speed = A_ws_speed_38[i];
					else            new_speed = A_ws_speed_45[i];

					new_range = A_ws_range[i];
					ms_range = (float)(new_range - old_range) / (float)(new_speed - old_speed);

					if(old_speed <= (word)(ws_pros.real_sum) &&
					   new_speed >  (word)(ws_pros.real_sum))	{
						ws_pros.real_avr =
							(word)(float)(((float)old_range + (float)(ws_pros.real_sum - old_speed) * ms_range) * 10.);
						cal_flag = 1;	i = END_CAL;
					}	else	{
						cal_flag = 0;
						old_speed = new_speed;	old_range = new_range;
					}
				}
			}

			if(cal_flag)   {
				if(ws_pros.real_max < ws_pros.real_avr)    {
					ws_pros.real_max = ws_pros.real_avr;
					wd_pros.real_max = wd_pros.real_avr;
					// maxmin save
					maxmin_save(WD_MAXMIN, wd_pros.real_max, wd_pros.real_max);
					maxmin_save(WS_MAXMIN, ws_pros.real_max, ws_pros.real_min);
				}
			}
			break;
		default:							return;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
