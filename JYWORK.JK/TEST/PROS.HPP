//[*]----------------------------------------------------------------------[*]
void wdir_pros(var_back_t *p)
{
	word	ad_value;

	watch();	ad_value = ad_convert_read();

	send_ch[Real_ch].real.avr = (float)ad_value / 4095. * 540.;

	if(send_ch[Real_ch].real.avr >= 360.)
			send_ch[Real_ch].real.avr -= 360.;
	if(send_ch[Real_ch].real.avr <    0.)
			send_ch[Real_ch].real.avr *=  -1.;
	if(send_ch[Real_ch].real.avr >= 360.)
			send_ch[Real_ch].real.avr  = 359.99;

	send_ch[Real_ch].real.avr = fabs(send_ch[Real_ch].real.avr);

	watch();
	if(send_ch[channel[Real_ch].ref_ch1].real.avr < 0.2)	{
		wd_sin_tot += sin(0.2 * send_ch[Real_ch].real.avr * M_PI / 180.);
		wd_cos_tot += cos(0.2 * send_ch[Real_ch].real.avr * M_PI / 180.);
	}	else	{
		wd_sin_tot += sin(send_ch[channel[Real_ch].ref_ch1].real.avr *
						  send_ch[Real_ch].real.avr * M_PI / 180.);
		wd_cos_tot += cos(send_ch[channel[Real_ch].ref_ch1].real.avr *
						  send_ch[Real_ch].real.avr * M_PI / 180.);
	}

	send_ch[Real_ch].real.max =	send_ch[Real_ch].real.min =
	send_ch[Real_ch].real.spc =	send_ch[Real_ch].real.avr;

	send_ch[Real_ch].real.status &= SEN_NORMAL;
}
//[*]----------------------------------------------------------------------[*]
void wspd_pros(var_back_t *p)
{
	word	ad_value;

	watch();	ad_value = ad_convert_read();

	send_ch[Real_ch].real.avr = (float)ad_value / 4095. * 60.;

	if(send_ch[Real_ch].real.avr >= 60.)
			send_ch[Real_ch].real.avr = 59.9;

	send_ch[Real_ch].real.max =	send_ch[Real_ch].real.min =
	send_ch[Real_ch].real.spc =	send_ch[Real_ch].real.avr;

	p->nujuk_sum += send_ch[Real_ch].real.avr;
	p->nujuk_count++;	send_ch[Real_ch].real.status &= SEN_NORMAL;
}
//[*]----------------------------------------------------------------------[*]
void sigma_pros(var_back_t *p)
{
	float	tmp = 0.;

	watch();
	tmp = send_ch[channel[Real_ch].ref_ch1].real.avr - Wd15avr;
	if(tmp >  180.)		tmp -= 360;
	if(tmp <- 180.)		tmp += 360;
	sigma_tot += tmp * tmp;
	send_ch[Real_ch].real.status &= SEN_NORMAL;		p->nujuk_count++;
}
//[*]----------------------------------------------------------------------[*]
void temp_pros(var_back_t *p)
{
	word	ad_value;

	watch();	ad_value = ad_convert_read();

	if     (ad_value <= 0   )	send_ch[Real_ch].real.avr = -50.0;
	else if(ad_value >= 4095)   send_ch[Real_ch].real.avr =  50.0;
	else						send_ch[Real_ch].real.avr =
									(float)((float)ad_value/40.95 - 50.0);

	send_ch[Real_ch].real.max =	send_ch[Real_ch].real.min =
	send_ch[Real_ch].real.spc =	send_ch[Real_ch].real.avr;

	send_ch[Real_ch].real.spc = (float)ad_value;

	p->nujuk_sum += send_ch[Real_ch].real.avr;
	p->nujuk_count++;	send_ch[Real_ch].real.status &= SEN_NORMAL;
}
//[*]----------------------------------------------------------------------[*]
void humi_pros(var_back_t *p)
{
	word  ad_value;

	watch();
	ad_value = ad_convert_read();
	if(ad_value == 0)	send_ch[Real_ch].real.avr = 0.;
	else	{
		send_ch[Real_ch].real.avr =
				(float)((float)(ad_value) / 4095.) * 100.;
		if(send_ch[Real_ch].real.avr >= 99.9)
				send_ch[Real_ch].real.avr = 99.9;
	}

	watch();
	send_ch[Real_ch].real.max =	send_ch[Real_ch].real.min =
	send_ch[Real_ch].real.spc =	send_ch[Real_ch].real.avr;

	send_ch[Real_ch].real.status &= SEN_NORMAL;
	p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
}
//[*]----------------------------------------------------------------------[*]
void fan_pros(var_back_t *p)
{
	word  ad_value;

	watch();
	ad_value = ad_convert_read();
	if(ad_value == 0)	send_ch[Real_ch].real.avr = 0.;
	else				send_ch[Real_ch].real.avr =
							(float)((float)(ad_value) * 2. / 4095.) * 10.0;

	send_ch[Real_ch].real.max =	send_ch[Real_ch].real.min =
	send_ch[Real_ch].real.spc =	send_ch[Real_ch].real.avr;

	send_ch[Real_ch].real.status &= SEN_NORMAL;
	p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
}
//[*]----------------------------------------------------------------------[*]
//   jugi pros all 15 min average
//[*]----------------------------------------------------------------------[*]
void jugi_pros(byte s_ch)
{
	float 	sigma = 0.;
	float 	level = 0.;
	double	aver  = 0.;
	watch();
	switch(send_ch[s_ch].jugi.sen_code) {
		case    0x00:
			if(abs(wd_cos_tot) < .0001)		aver  = (atan(wd_sin_tot / .0001)) *
													(180./M_PI);
			if(wd_sin_tot      <    0.)		aver -= 180.;
			else							aver  = (atan(wd_sin_tot/wd_cos_tot)) *
													(180./M_PI);
			if(wd_cos_tot <   0.)			aver += 180.;
 //			if(aver       < 180.)			aver += 360.;
 // 99.07.20 Upgrade
			if(aver       <   0.)			aver += 360.;
			if(aver       > 360.)			aver -= 360.;
			if(aver      >= 360.)			aver  = 359.99;

			send_ch[s_ch].jugi.max = send_ch[s_ch].jugi.avr =
			send_ch[s_ch].jugi.min = Wd15avr = fabs(aver);
			wd_sin_tot = wd_cos_tot = 0.;						break;
		case	0x12:
			sigma =
				(float)sqrt((double)sigma_tot / (double)channel[s_ch].var.nujuk_count);
			sigma_tot = 0.; 	channel[s_ch].var.nujuk_count = 0.;
			send_ch[s_ch].real.max = send_ch[s_ch].real.avr =
			send_ch[s_ch].real.min = send_ch[s_ch].jugi.max =
			send_ch[s_ch].jugi.avr = send_ch[s_ch].jugi.min = sigma;
			if     (sigma > 22.5 )					level = 1.;
			else if(sigma > 17.5 && sigma <= 22.5)	level = 2.;
			else if(sigma > 12.5 && sigma <= 17.5) 	level = 3.;
			else if(sigma >  7.5 && sigma <= 12.5) 	level = 4.;
			else if(sigma >  3.8 && sigma <=  7.5) 	level = 5.;
			else if(sigma >  2.1 && sigma <=  3.8) 	level = 6.;
			else									level = 9.;
			send_ch[s_ch].real.spc = send_ch[s_ch].jugi.spc = level;
																break;
		default     :	// TEMP, HUMI
			send_ch[s_ch].jugi.avr =
				(float)(channel[s_ch].var.nujuk_sum / channel[s_ch].var.nujuk_count);

			send_ch[s_ch].jugi.max = send_ch[s_ch].jugi.avr;
			send_ch[s_ch].jugi.min = send_ch[s_ch].jugi.avr;
			channel[s_ch].var.nujuk_sum   = 0.;
			channel[s_ch].var.nujuk_count = 0.;					break;
	}		send_ch[s_ch].jugi.status = NORMAL;
}
//[*]----------------------------------------------------------------------[*]
