//[*]----------------------------------------------------------------------[*]
void max_min_pros(var_back_t *p)
{
	watch();

	switch(ch.num[p->mux].sensor.sen_code)	{
		case	0x13:	case	0x14:
			if(p->init_flag)	{
				p->init_flag  = OFF;
				p->nujuk1_min = p->nujuk1_sum = 0.;
				p->nujuk2_min = p->nujuk2_sum = 0.;
				p->jugi1_min  = p->jugi1_sum  = 0.;
				p->jugi2_min  = p->jugi2_sum  =	0.;
			}	break;
		default		:
			if(p->init_flag)	{
				p->init_flag  = OFF;
				p->nujuk1_max = p->nujuk1_min = p->real_sum;
				p->nujuk2_max = p->nujuk2_min = p->real_sum;
				p->jugi1_sum  = p->jugi2_sum  = p->real_sum;
				p->jugi1_max  = p->jugi1_min  = p->real_sum;
				p->jugi2_max  = p->jugi2_min  = p->real_sum;
			}	else	{
				if     (p->real_sum > p->real_max  )	p->real_max   = p->real_sum;
				else if(p->real_sum < p->real_min  )	p->real_min   = p->real_sum;
				if     (p->real_sum > p->nujuk1_max)	p->nujuk1_max = p->real_sum;
				else if(p->real_sum < p->nujuk1_min)	p->nujuk1_min = p->real_sum;
				if     (p->real_sum > p->nujuk2_max)	p->nujuk2_max = p->real_sum;
				else if(p->real_sum < p->nujuk2_min)	p->nujuk2_min = p->real_sum;
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
	ad_value = ad_convert_read(p->mux);

	if(ad_value < 7)		p->real_sum = -50.;
	else	{
		for(i=0; i<14; i++)		{
			temp_new = temp_table [i];	offset = temp_offset[i];

			if((temp_old <= ad_value) && (ad_value < temp_new))	{
				if( ((p->ref_w1 -offset) <= ad_value) &&
					((p->ref_w1 +offset) >  ad_value))	flag = 1;
				else	{
					p->ref_w1    = ad_value;			flag = 1;
					pros_value   = (float)(10./(float)(temp_new - temp_old)) * (float)(ad_value - temp_old);
					p->real_sum  = (float)(temp + pros_value);
				}
			}
			if(flag == 1) 			i = END_CAL;
			temp_old = temp_new;	temp += 10.;
		}
		if(flag != 1)		p->real_sum = 80.;
	}
	if(flag == 1)			{	// sensor error flag set
		max_min_pros(p);    ch.num[p->mux].sensor.status.bits.sensor = NORMAL;
		p->nujuk1_count++;	p->nujuk1_sum += p->real_sum;
		p->nujuk2_count++;	p->nujuk2_sum += p->real_sum;
	}	else	{
		p->init_flag = ON;	ch.num[p->mux].sensor.status.bits.sensor = FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
void dewp_pros(var_back_t *p)
{
	byte	flag = 0;
	word	dewp_new = 0, dewp_old = 0, offset = 0, ad_value = 0, i = 0;
	float	dewp = -45., pros_value = 0.;

	watch();
	ad_value = ad_convert_read(p->mux);

	if(ad_value < 7)		p->real_sum = -40.;
	else	{
		for(i=0; i<17; i++)		{
			dewp_new = dewp_table [i];	offset = dewp_offset[i];

			if((dewp_old <= ad_value) && (ad_value < dewp_new))	{
				if( ((p->ref_w1 -offset) <= ad_value) &&
					((p->ref_w1 +offset) >  ad_value))	flag = 1;
				else	{
					p->ref_w1    = ad_value;			flag = 1;
					pros_value   = (float)(5./(float)(dewp_new - dewp_old)) * (float)(ad_value - dewp_old);
					p->real_sum  = (float)(dewp + pros_value);
				}
			}
			if(flag == 1) 			i = END_CAL;
			dewp_old = dewp_new;	dewp += 5.;
		}
		if(flag != 1)		p->real_sum = 40.;

		// Dew pont error pros
		if( p->real_sum > ch.num[(ch.num[p->mux].ref_id1)].var.real_sum)	{
			p->real_sum = ch.num[(ch.num[p->mux].ref_id1)].var.real_sum;
			p->ref_w1   = 0;
		}
	}
	// sensor error flag set
	if(flag == 1)			{
		max_min_pros(p);    ch.num[p->mux].sensor.status.bits.sensor = NORMAL;
		p->nujuk1_count++;	p->nujuk1_sum += p->real_sum;
		p->nujuk2_count++;	p->nujuk2_sum += p->real_sum;
	}	else	{
		p->init_flag = ON;	ch.num[p->mux].sensor.status.bits.sensor = FAIL;
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
	word	i=0;
	watch();

	p->real_sum = humi_cal(ch.num[(ch.num[p->mux].ref_id1)].var.real_sum,
						   ch.num[(ch.num[p->mux].ref_id2)].var.real_sum);

	if(p->real_sum > 0.)			{		// sensor error flag set
		max_min_pros(p);    ch.num[p->mux].sensor.status.bits.sensor = NORMAL;
		p->nujuk1_count++;	p->nujuk1_sum += p->real_sum;
		p->nujuk2_count++;	p->nujuk2_sum += p->real_sum;
	}	else	{
		p->init_flag = ON;	ch.num[p->mux].sensor.status.bits.sensor = FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
void boksa_pros(var_back_t *p)
{
	byte	flag = 0;
	word	boksa_new = 0, boksa_old = 0, offset = 0, ad_value = 0, i = 0;
	float	boksa = 0., pros_value = 0.;

	watch();
	ad_value = ad_convert_read(p->mux);

	if(ad_value < 13)		p->real_sum = 0.;
	else	{
		offset = 20;
		for(i=0; i<28; i++)		{
			boksa_new = boksa_table [i];

			if((boksa_old <= ad_value) && (ad_value < boksa_new))	{
				if( ((p->ref_w1 -Boksa_offset) <= ad_value) &&
					((p->ref_w1 +Boksa_offset) >  ad_value))		flag = 1;
				else	{
					p->ref_w1    = ad_value;			flag = 1;
					pros_value   = (float)((float)(offset)/(float)(boksa_new - boksa_old)) * (float)(ad_value - boksa_old);
					p->real_sum  = (float)(boksa + pros_value);
				}
			}
			if     (flag == 1 ) 		i = END_CAL;
			if     (i    == 5 )		{	offset = 100 ;	boksa = 100;	}
			else if(i    == 23)		{	offset = 1000;	boksa = 1000;	}
			boksa_old = boksa_new;		boksa += offset;
		}
		if(flag != 1)		p->real_sum = 6000.;
	}
	// sensor error flag set
	if(flag == 1)			{
		max_min_pros(p);    ch.num[p->mux].sensor.status.bits.sensor = NORMAL;
		p->nujuk1_count++;	p->nujuk1_sum += p->real_sum;
		p->nujuk2_count++;	p->nujuk2_sum += p->real_sum;
	}	else	{
		p->init_flag = ON;	ch.num[p->mux].sensor.status.bits.sensor = FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
void jugi1_pros(void)
{
	word	i;
	watch();

	ch.jugi1_flag = OFF;
	for(i=0; i<7; i++)	{
		if(channel_status(i))	{
			switch(ch.num[i].sensor.sen_code)	{
				case	0x13:	case	0x14:
					ch.num[i].var.jugi1_sum =
						(float)(ch.num[i].var.nujuk1_sum/ch.num[i].var.nujuk1_count);
					ch.num[i].var.jugi1_sum  = (float)(ch.num[i].var.jugi1_sum / 600.);
					ch.num[i].var.jugi1_max += ch.num[i].var.jugi1_sum; // mj
					ch.num[i].var.nujuk1_max = ch.num[i].var.jugi1_max;
					ch.num[i].var.nujuk1_sum   = 0;
					ch.num[i].var.nujuk1_count = 0;
					break;
				default		:
					ch.num[i].var.jugi1_sum =
						(float)(ch.num[i].var.nujuk1_sum/ch.num[i].var.nujuk1_count);
					ch.num[i].var.jugi1_max = ch.num[i].var.nujuk1_max;
					ch.num[i].var.jugi1_min = ch.num[i].var.nujuk1_min;
					ch.num[i].var.nujuk1_sum   = 0;
					ch.num[i].var.nujuk1_count = 0;
					ch.num[i].var.nujuk1_max   = ch.num[i].var.real_sum ;
					ch.num[i].var.nujuk1_min   = ch.num[i].var.real_sum ;
					break;
			}
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void jugi2_pros(void)
{
	word	i;
	watch();

	ch.jugi2_flag = OFF;
	for(i=0; i<7; i++)	{
		if(channel_status(i))	{
			switch(ch.num[i].sensor.sen_code)	{
				case	0x13:	case	0x14:
					ch.num[i].var.jugi2_sum =
						(float)(ch.num[i].var.nujuk2_sum/ch.num[i].var.nujuk2_count);
					ch.num[i].var.jugi2_sum  = (float)(ch.num[i].var.jugi2_sum / 3600.);
					ch.num[i].var.jugi2_max += ch.num[i].var.jugi2_sum;	// mj
					ch.num[i].var.real_max   = ch.num[i].var.jugi2_max;
					ch.num[i].var.nujuk2_max = ch.num[i].var.jugi2_max;
					ch.num[i].var.nujuk2_sum   = 0;
					ch.num[i].var.nujuk2_count = 0;
					break;
				default		:
					ch.num[i].var.jugi2_sum =
						(float)(ch.num[i].var.nujuk2_sum/ch.num[i].var.nujuk2_count);
					ch.num[i].var.jugi2_max = ch.num[i].var.nujuk2_max;
					ch.num[i].var.jugi2_min = ch.num[i].var.nujuk2_min;
					ch.num[i].var.nujuk2_sum   = 0;
					ch.num[i].var.nujuk2_count = 0;
					ch.num[i].var.nujuk2_max   = ch.num[i].var.real_sum ;
					ch.num[i].var.nujuk2_min   = ch.num[i].var.real_sum ;
					break;
			}
		}
	}
}
//[*]----------------------------------------------------------------------[*]
