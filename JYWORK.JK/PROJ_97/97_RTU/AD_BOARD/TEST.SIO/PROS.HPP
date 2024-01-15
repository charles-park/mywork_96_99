//[*]----------------------------------------------------------------------[*]
void snow_pros(void)
{
	byte	i = 0;
	float	snow_now = 0.,abs_snow = 0.,snow = 0.,distance = 0., snow_total = 0.;

	watch();
	if(Sensor_cat != -9999.0)	{
		if(Old_snow == -9999.0)		Old_snow = Sensor_cat;

		abs_snow = Sensor_cat - Old_snow;
		if(abs_snow <   0)	abs_snow = abs_snow * (-1.);
		if(abs_snow > 100)	{
			Sensor_err_count++;
			if(Sensor_err_count >= 5)	{
				Sensor_err_count = 0;
				Old_snow = Sensor_cat;
			}
		}	else	{
			Snow_aver[Sensor_cat_count++] = Old_snow = Sensor_cat;

			if(Sensor_cat_count >= 10)	Snow_flag = Sensor_cat_count = 0;

			if(Snow_flag == 0)	{
				distance = (float)(sqrt(Temp / 273.15));
				for(i=0; i<10; i++)		snow_total += Snow_aver[i];

				distance = (float)(snow_total / 100.) * distance;
				Off_back = distance;

				if(Offset_auto_flag)	{
					Offset = distance;	Offset_auto_flag = 0;
					send_ack(&com1, 'D', sizeof(sensor_field_t), 0);
					send_offset(&com1, Offset, &offset);
				}
				if(Offset != 0.0)	{
					snow_now = Offset - distance;
					if(snow_now <= 0.5)	snow_now = 0.0;
					real.snow.avr = snow_now;
					if(real.snow.max < real.snow.avr)	real.snow.max = real.snow.avr;
					Jugi_avr      = snow_now;
					Jugi_max      = real.snow.max;

					if(Offset < distance)	{
						Sensor_cat_count = 0;
						Offset = distance;
						Offset_auto_flag = 1;
					}
					Sensor_ack_count = 0;
					Snow_flag = 1;
				}
			}
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void jugi_pros(void)
{
	watch();
	jugi.snow.avr = Jugi_avr;	jugi.snow.max = Jugi_max;	Jugi_avr = 0.;
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
