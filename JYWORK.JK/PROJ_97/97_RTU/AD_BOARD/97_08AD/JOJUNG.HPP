//[*]----------------------------------------------------------------------[*]
void temp_test(var_back_t *p)
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
					pros_value   = (float)(10./(temp_new - temp_old)) * (float)(ad_value - temp_old);
					p->real_sum  = (float)(temp + pros_value);
				}
			}
			if(flag == 1) 			i = END_CAL;
			temp_old = temp_new;	temp += 10.;
		}
		if(flag != 1)		p->real_sum = 80.;
	}
}
//[*]----------------------------------------------------------------------[*]
void dewp_test(var_back_t *p)
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
					pros_value   = (float)(5./(dewp_new - dewp_old)) * (float)(ad_value - dewp_old);
					p->real_sum  = (float)(dewp + pros_value);
				}
			}
			if(flag == 1) 			i = END_CAL;
			dewp_old = dewp_new;	dewp += 5.;
		}
		if(flag != 1)		p->real_sum = 40.;
	}
}
//[*]----------------------------------------------------------------------[*]
void boksa_test(var_back_t *p)
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
}
//[*]----------------------------------------------------------------------[*]
