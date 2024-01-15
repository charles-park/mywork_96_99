//[*]----------------------------------------------------------------------[*]
void max_min_pros(var_back_t *p)
{
	watch();

	switch(send_ch[Real_ch].real.sen_code)	{
		case	0x13:	case	0x14:
			if(p->err_flag)	{
				p->err_flag = OFF;
				send_ch[Real_ch].real.avr = 0.;
				p->nujuk_sum = 0.;
				p->nujuk_count = 0;
			}	break;
		default		:
			if(p->err_flag)	{
				p->err_flag  = OFF;
				send_ch[Real_ch].real.max   = send_ch[Real_ch].real.avr;
				send_ch[Real_ch].real.min   = send_ch[Real_ch].real.avr;
				send_ch[Real_ch].jugi.max   = send_ch[Real_ch].real.avr;
				send_ch[Real_ch].jugi.min   = send_ch[Real_ch].real.avr;
				send_ch[Real_ch].jugi.avr	= send_ch[Real_ch].real.avr;

				p->nujuk_max = p->nujuk_min = send_ch[Real_ch].real.avr;
				p->nujuk_sum   = 0.;
				p->nujuk_count = 0 ;
			}	else	{
				if     (send_ch[Real_ch].real.avr > send_ch[Real_ch].real.max)
				{	send_ch[Real_ch].real.max = send_ch[Real_ch].real.avr;	}
				else if(send_ch[Real_ch].real.avr < send_ch[Real_ch].real.min)
				{	send_ch[Real_ch].real.min = send_ch[Real_ch].real.avr;	}
				if     (send_ch[Real_ch].real.avr > p->nujuk_max)
				{	p->nujuk_max = send_ch[Real_ch].real.avr;		}
				else if(send_ch[Real_ch].real.avr < p->nujuk_min)
				{	p->nujuk_min = send_ch[Real_ch].real.avr;		}
			}	break;
	}
}
//[*]----------------------------------------------------------------------[*]
void temp_pros(var_back_t *p)
{
	byte	flag = 0;
	word	temp_new = 0, temp_old = 0, offset = 0, ad_value = 0, i = 0;
	float	temp = -60. , pros_value = 0.;

	watch();
	ad_value = ad_convert_read();

	if(ad_value < 7)		send_ch[Real_ch].real.avr = -50.;
	else	{
		for(i=0; i<14; i++)		{
			temp_new = temp_table [i];	offset = temp_offset[i];

			if((temp_old <= ad_value) && (ad_value < temp_new))	{
				if( ((p->noise_check -offset) <= ad_value) &&
					((p->noise_check +offset) >  ad_value))	flag = 1;
				else	{
					p->noise_check = ad_value;			flag = 1;
					pros_value = (float)(10./(float)(temp_new - temp_old)) * (float)(ad_value - temp_old);
					send_ch[Real_ch].real.avr = (float)(temp + pros_value);
				}
			}
			if(flag == 1) 			i = END_CAL;
			temp_old = temp_new;	temp += 10.;
		}
		if(flag != 1)		send_ch[Real_ch].real.avr = 80.;
	}
	if(flag == 1)			{	// sensor error flag set
		max_min_pros(p);    status.bits.sensor = NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	}	else	{
		p->err_flag = ON;	status.bits.sensor = FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
void dewp_pros(var_back_t *p)
{
	byte	flag = 0;
	word	dewp_new = 0, dewp_old = 0, offset = 0, ad_value = 0, i = 0;
	float	dewp = -45., pros_value = 0.;

	watch();
	ad_value = ad_convert_read();

	if(ad_value < 7)		send_ch[Real_ch].real.avr = -40.;
	else	{
		for(i=0; i<17; i++)		{
			dewp_new = dewp_table [i];	offset = dewp_offset[i];

			if((dewp_old <= ad_value) && (ad_value < dewp_new))	{
				if( ((p->noise_check -offset) <= ad_value) &&
					((p->noise_check +offset) >  ad_value))	flag = 1;
				else	{
					p->noise_check = ad_value;			flag = 1;
					pros_value   = (float)(5./(float)(dewp_new - dewp_old)) * (float)(ad_value - dewp_old);
					send_ch[Real_ch].real.avr = (float)(dewp + pros_value);
				}
			}
			if(flag == 1) 			i = END_CAL;
			dewp_old = dewp_new;	dewp += 5.;
		}
		if(flag != 1)		send_ch[Real_ch].real.avr = 40.;

		// Dew pont error pros
		if( send_ch[Real_ch].real.avr > send_ch[channel[Real_ch].ref_ch1].real.avr)
		{
			send_ch[Real_ch].real.avr = send_ch[channel[Real_ch].ref_ch1].real.avr;
			p->noise_check = 0;
		}
	}
	// sensor error flag set
	if(flag == 1)			{
		max_min_pros(p);    status.bits.sensor = NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	}	else	{
		p->err_flag = ON;	status.bits.sensor = FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
float humi_cal(float ttemp, float ddewp)
{
   float  dp, humi;

   watch();

   dp    = pow(10.,(ddewp *8.286 + 186.527)/(237.3+ddewp));
   humi  = dp/(0.0611*pow(10.,(7.5*ttemp/(237.3+ddewp))));
   if(humi > 99.9)		humi = 99.9;
   return humi;
}
//[*]----------------------------------------------------------------------[*]
void humi_pros(var_back_t *p)
{
	watch();

	send_ch[Real_ch].real.avr =
		humi_cal(send_ch[channel[Real_ch].ref_ch1].real.avr,
				 send_ch[channel[Real_ch].ref_ch2].real.avr);

	if(send_ch[Real_ch].real.avr > 0.)	{		// sensor error flag set
		max_min_pros(p);    status.bits.sensor = NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	}	else	{
		p->err_flag = ON;	status.bits.sensor = FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
void boksa_pros(var_back_t *p)
{
	byte	flag = 0;
	word	boksa_new = 0, boksa_old = 0, offset = 0, ad_value = 0, i = 0;
	float	boksa = 0., pros_value = 0.;

	watch();
	ad_value = ad_convert_read();

	if(ad_value < 13)		{	send_ch[Real_ch].real.avr = 0.;	flag = 1;	}
	else	{
		offset = 20;
		for(i=0; i<28; i++)		{
			boksa_new = boksa_table [i];

			if((boksa_old <= ad_value) && (ad_value < boksa_new))	{
				if( ((p->noise_check -2) <= ad_value) &&
					((p->noise_check +2) >  ad_value))		flag = 1;
				else	{
					p->noise_check = ad_value;			flag = 1;
					pros_value   = (float)((float)(offset)/(float)(boksa_new - boksa_old)) * (float)(ad_value - boksa_old);
					send_ch[Real_ch].real.avr = (float)(boksa + pros_value);
				}
			}
			if     (flag == 1 ) 		i = END_CAL;
			if     (i    == 5 )		{	offset = 100 ;	boksa = 100;	}
			else if(i    == 23)		{	offset = 1000;	boksa = 1000;	}
			boksa_old = boksa_new;		boksa += offset;
		}
		if(flag != 1)		 send_ch[Real_ch].real.avr = 6000.;
	}
	// sensor error flag set
	if(flag == 1)			{
		max_min_pros(p);    status.bits.sensor = NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	}	else	{
		p->err_flag = ON;	status.bits.sensor = FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
void jugi_pros(byte s_ch)
{
	watch();

	switch(send_ch[s_ch].jugi.sen_code)	{
		case	0x13:	case	0x14:
			send_ch[s_ch].jugi.avr =
				(float)(channel[s_ch].var.nujuk_sum / channel[s_ch].var.nujuk_count);
			send_ch[s_ch].jugi.avr  = (float)(send_ch[s_ch].jugi.avr / 3600.);

			send_ch[s_ch].jugi.max += send_ch[s_ch].jugi.avr;
			send_ch[s_ch].real.max  = send_ch[s_ch].jugi.max;
			channel[s_ch].var.nujuk_sum   = 0.;
			channel[s_ch].var.nujuk_count = 0;
			break;
		default		:
			send_ch[s_ch].jugi.avr =
				(float)(channel[s_ch].var.nujuk_sum / channel[s_ch].var.nujuk_count);

			send_ch[s_ch].jugi.max = channel[s_ch].var.nujuk_max;
			send_ch[s_ch].jugi.min = channel[s_ch].var.nujuk_min;
			channel[s_ch].var.nujuk_max = send_ch[s_ch].real.avr;
			channel[s_ch].var.nujuk_min = send_ch[s_ch].real.avr;
			channel[s_ch].var.nujuk_sum   = 0.;
			channel[s_ch].var.nujuk_count = 0;
			break;
	}
}
//[*]----------------------------------------------------------------------[*]





