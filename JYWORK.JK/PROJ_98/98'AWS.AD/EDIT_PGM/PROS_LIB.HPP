//[*]----------------------------------------------------------------------[*]
//   PROCESSING LIBRARY FUNCTION DEFINE.
//[*]----------------------------------------------------------------------[*]
	void 	TEMPERATURE_0	(var_back_t *p);
	void 	TEMPERATURE_1	(var_back_t *p);
	void 	TEMPERATURE_2	(var_back_t *p);
	void 	HUMIDITY_0		(var_back_t *p);
	void 	HUMIDITY_1		(var_back_t *p);
	void 	HUMIDITY_2		(var_back_t *p);
	void 	PRECIPITATION_0	(var_back_t *p);
	void 	PRE_DETECT_0	(var_back_t *p);
	void 	DEW_POINT_0		(var_back_t *p);
	void 	SKY_GND_EMISS_0	(var_back_t *p);
	void 	SUN_RADA_0		(var_back_t *p);
	void 	SUN_DETECT_0	(var_back_t *p);
	void 	BUL_JISU_0		(var_back_t *p);
	void 	max_min_pros	(var_back_t *p);
	float 	humi_cal		(float ttemp, float ddewp);
	float	bul_cal			(double temp, double humi);
	void 	jugi_pros		(byte s_ch);
	double 	dewpoint		(double Temperature, double Humidity);
//[*]----------------------------------------------------------------------[*]
//   max_min lib	???
//[*]----------------------------------------------------------------------[*]
void max_min_pros(var_back_t *p)
{
	watch();
	switch(send_ch[Real_ch].real.sen_code)	{
		case  3:	case  4:	case  8:
		case  9:	case 19:	case 20:
			if(p->err_flag)	{
				p->err_flag = OFF;
				send_ch[Real_ch].real.avr = 0.;
				p->nujuk_sum   = 0.;
				p->nujuk_count = 0.;
			}	break;
		default	:
			if(p->err_flag)	{
				p->err_flag  = OFF;
				send_ch[Real_ch].real.max   = send_ch[Real_ch].real.avr;
				send_ch[Real_ch].real.min   = send_ch[Real_ch].real.avr;
				send_ch[Real_ch].jugi.max   = send_ch[Real_ch].real.avr;
				send_ch[Real_ch].jugi.min   = send_ch[Real_ch].real.avr;
				send_ch[Real_ch].jugi.avr	= send_ch[Real_ch].real.avr;

				p->nujuk_max = p->nujuk_min = send_ch[Real_ch].real.avr;
				p->nujuk_sum   = 0.;
				p->nujuk_count = 0.;
			} else	{
				if(send_ch[Real_ch].real.avr > send_ch[Real_ch].real.max)
					send_ch[Real_ch].real.max = send_ch[Real_ch].real.avr;
				if(send_ch[Real_ch].real.avr < send_ch[Real_ch].real.min)
					send_ch[Real_ch].real.min = send_ch[Real_ch].real.avr;
				if(send_ch[Real_ch].real.avr > p->nujuk_max)
					p->nujuk_max = send_ch[Real_ch].real.avr;
				if(send_ch[Real_ch].real.avr < p->nujuk_min)
					p->nujuk_min = send_ch[Real_ch].real.avr;
			}	break;
	}
}
//[*]----------------------------------------------------------------------[*]
//   	temp lib
//		void temp_pros(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void TEMPERATURE_0(var_back_t *p)
{
	byte	cnt, flag = 0;
	word	i, temp_new, offset, ad_value, temp_old = 7;
	float	temp = -60., pros_value;

	watch();
	ad_value = ad_convert_read();
	if(ad_value < 7)	send_ch[Real_ch].real.avr = -50.0;
	else	{
		for(i=0; i<14; i++)		{
			temp_new = temp_table [i];
			offset   = temp_offset[i];
			if((temp_old <= ad_value) && (ad_value < temp_new))	{
				if(((p->noise_check - offset) <= ad_value) &&
				   ((p->noise_check + offset) >  ad_value))	{
					if(noize_count > 10)	{
						noize_count = 0;
						p->noise_check = ad_value;
					}	flag = 1;
				} else	{
					noize_count = 0;
					p->noise_check = ad_value;	flag = 1;
					pros_value = (float)(10./(float)(temp_new - temp_old)) * (float)(ad_value - temp_old);
					p->move_aver[p->move_pos++] = (float)(temp + pros_value);
					if(p->move_cnt <  10)	p->move_cnt++;
					if(p->move_pos >= 10)	p->move_pos = 0;
				}
			}
			if(flag == 1) 			i = END_CAL;
			temp_old = temp_new;	temp += 10.0;
		}
		pros_value = 0.0;
		for(cnt=0; cnt<p->move_cnt; cnt++)	{
			pros_value += p->move_aver[cnt];
			send_ch[Real_ch].real.avr = pros_value / (float)(p->move_cnt);
		}
		if(!flag)	send_ch[Real_ch].real.avr = 50.0;
	}
	if(flag)	{								// sensor error flag set
		max_min_pros(p);
		send_ch[Real_ch].real.status &= SEN_NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	} else	{
		p->err_flag = ON;
		send_ch[Real_ch].real.status |= SEN_FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
//		void temp_I_pros(var_back_t *p)     // range -20 ~ +80 do
//[*]----------------------------------------------------------------------[*]
void TEMPERATURE_1(var_back_t *p)
{
	word	ad_value = 0;
	float	pros_value = 0.;

	watch();
	ad_value = ad_convert_read();

	if(ad_value < 820)		send_ch[Real_ch].real.avr = -20.;
	else	{
		pros_value = ((float)(ad_value - 820) / 3275.) * 100.;
		send_ch[Real_ch].real.avr = (float)(pros_value -20.);
	}
	max_min_pros(p);	send_ch[Real_ch].real.status &= SEN_NORMAL;
	p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
}
//[*]----------------------------------------------------------------------[*]
//		void gnd_temp_I_pros(var_back_t *p)	// range -40 ~ +60 do
//[*]----------------------------------------------------------------------[*]
void TEMPERATURE_2(var_back_t *p)
{
	word	ad_value = 0;
	float	pros_value = 0.;

	watch();
	ad_value = ad_convert_read();

	if(ad_value < 820)		send_ch[Real_ch].real.avr = -40.;
	else	{
		pros_value = ((float)(ad_value - 820) / 3275.) * 100.;
		send_ch[Real_ch].real.avr = (float)(pros_value -40.);
	}
	max_min_pros(p);	send_ch[Real_ch].real.status &= SEN_NORMAL;
	p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
}
//[*]----------------------------------------------------------------------[*]
//   	humi lib
//		void humi_pros1(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void HUMIDITY_0(var_back_t *p)
{
	byte  cnt;
	word  ad_value;
	float pros_value;

	watch();
	ad_value = ad_convert_read();
	if(ad_value < 820)	send_ch[Real_ch].real.avr = 0.;
	else	{
		pros_value = ((float)(ad_value - 820) / 3276.) * 100.;
		if(pros_value >= 99.9)	send_ch[Real_ch].real.avr = 99.9;
		else	{
			p->move_aver[p->move_pos++] = pros_value;
			if(p->move_cnt <  10)	p->move_cnt++;
			if(p->move_pos >= 10)	p->move_pos = 0;
			pros_value = 0.0;
			for(cnt=0; cnt<p->move_cnt; cnt++)	{
				pros_value += p->move_aver[cnt];
				send_ch[Real_ch].real.avr = pros_value / (float)(p->move_cnt);
			}
		}
	}
	max_min_pros(p);	send_ch[Real_ch].real.status &= SEN_NORMAL;
	p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
}
//[*]----------------------------------------------------------------------[*]
//		void humi_I_pros(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void HUMIDITY_1(var_back_t *p)
{
	word	ad_value = 0;
	float	pros_value = 0.;

	watch();
	ad_value = ad_convert_read();

	if(ad_value < 820)		send_ch[Real_ch].real.avr = 0.;
	else	{
		pros_value = ((float)(ad_value - 820) / 3275.) * 100.;
		if(pros_value >= 99.9)	send_ch[Real_ch].real.avr = 99.9;
		else                    send_ch[Real_ch].real.avr = pros_value;
	}
	max_min_pros(p);	send_ch[Real_ch].real.status &= SEN_NORMAL;
	p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
}
//[*]----------------------------------------------------------------------[*]
//		void humi_pros(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void HUMIDITY_2(var_back_t *p)
{
	watch();

	send_ch[Real_ch].real.avr =
		humi_cal(send_ch[channel[Real_ch].ref_ch1].real.avr,
				 send_ch[channel[Real_ch].ref_ch2].real.avr);

	if(send_ch[Real_ch].real.avr > 0.)	{		// sensor error flag set
		max_min_pros(p);    send_ch[Real_ch].real.status &= SEN_NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	}	else	{
		p->err_flag = ON;	send_ch[Real_ch].real.status |= SEN_FAIL;
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
//  	pre lib
//		void pre_pros(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void PRECIPITATION_0(var_back_t *p)
{
	byte	pre = 0;

	watch();
	switch(Real_ch)	{
		case 0:	pre = input(PRE1);	send_ch[Real_ch].real.status &= SEN_NORMAL;	break;
		case 1:	pre = input(PRE2);	send_ch[Real_ch].real.status &= SEN_NORMAL;	break;
		default :																return;
	}
	if(pre != 0)	{
		pre_ch_init(Real_ch);
		p->nujuk_sum += 0.5;
		send_ch[Real_ch].real.avr =	send_ch[Real_ch].real.max = p->nujuk_sum;
	}
}
//[*]----------------------------------------------------------------------[*]
//   	pred lib
//		void pred_pros(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void PRE_DETECT_0(var_back_t *p)
{
	word	ad_value;

	watch();
	ad_value = ad_convert_read();
	if(ad_value > 1024)		send_ch[Real_ch].real.avr = 10.0;
	else					send_ch[Real_ch].real.avr = 0.0;

	send_ch[Real_ch].real.status &= SEN_NORMAL;
}
//[*]----------------------------------------------------------------------[*]
//   	dew lib
//		void dewp_pros(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void DEW_POINT_0(var_back_t *p)
{
	byte	flag = 0, cnt;
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
					((p->noise_check +offset) >  ad_value))		flag = 1;
				else	{
					p->noise_check = ad_value;			flag = 1;
					pros_value   = (float)(5./(float)(dewp_new - dewp_old)) * (float)(ad_value - dewp_old);
					p->move_aver[p->move_pos++] = (float)(dewp + pros_value);
					if(p->move_cnt <  10)	p->move_cnt++;
					if(p->move_pos >= 10)	p->move_pos = 0;
				}
			}
			if(flag == 1) 			i = END_CAL;
			dewp_old = dewp_new;	dewp += 5.;
		}
		pros_value = 0.0;
		for(cnt = 0; cnt < p->move_cnt; cnt++)	pros_value += p->move_aver[cnt];
		send_ch[Real_ch].real.avr = pros_value / (float)(p->move_cnt);

		if(flag != 1)		send_ch[Real_ch].real.avr = 40.;

		// Dew pont error pros
		if( send_ch[Real_ch].real.avr > send_ch[channel[Real_ch].ref_ch1].real.avr)
		{
			send_ch[Real_ch].real.avr = send_ch[channel[Real_ch].ref_ch1].real.avr;
			//p->noise_check = 0;
		}
	}

	// sensor error flag set
	if(flag == 1)			{
		max_min_pros(p);
		send_ch[Real_ch].real.status &= SEN_NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	}	else	{
		send_ch[Real_ch].real.status |= SEN_FAIL;
		p->err_flag = ON;
	}
}
//[*]----------------------------------------------------------------------[*]
//   	boksa lib
//		void boksa_pros(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void SKY_GND_EMISS_0(var_back_t *p)
{
	byte	flag = 0, cnt;
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
					p->move_aver[p->move_pos++] = (float)(boksa + pros_value);
					if(p->move_cnt <  10)	p->move_cnt++;
					if(p->move_pos >= 10)	p->move_pos = 0;
				}
			}
			if     (flag == 1 ) 		i = END_CAL;
			if     (i    == 5 )		{	offset = 100 ;	boksa = 100;	}
			else if(i    == 23)		{	offset = 1000;	boksa = 1000;	}
			boksa_old = boksa_new;		boksa += offset;
		}
		pros_value = 0.0;
		for(cnt = 0; cnt < p->move_cnt; cnt++)	pros_value += p->move_aver[cnt];
		send_ch[Real_ch].real.avr = pros_value / (float)(p->move_cnt);

		if(flag != 1)		 send_ch[Real_ch].real.avr = 6000.;
	}
	// sensor error flag set
	if(flag == 1)			{
		max_min_pros(p);
		send_ch[Real_ch].real.status &= SEN_NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	}	else	{
		p->err_flag = ON;
		send_ch[Real_ch].real.status |= SEN_FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
//   	sun radation lib
//		void sun_radation(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void SUN_RADA_0(var_back_t *p)
{
	word	ad_value = 0;

	watch();
	ad_value = ad_convert_read();

	send_ch[Real_ch].real.avr  = (float)(2445. / 1592.) * (float)(ad_value);
	p->nujuk_sum  += send_ch[Real_ch].real.avr;
	p->nujuk_count++;
	send_ch[Real_ch].real.status &= SEN_NORMAL;
}
//[*]----------------------------------------------------------------------[*]
//   	sun detect lib
//		void sund_pros(var_back_t *p)
//[*]----------------------------------------------------------------------[*]
void SUN_DETECT_0(var_back_t *p)
{
	word	ad_value;

	watch();
	ad_value = ad_convert_read();
	if(ad_value > 1024)	{
		send_ch[Real_ch].real.avr = 10.0;
		p->nujuk_sum++;		p->nujuk_spc++;
	} else	send_ch[Real_ch].real.avr = 0.0;
	send_ch[Real_ch].real.max = (float)(p->nujuk_sum / 60.);
	send_ch[Real_ch].real.status &= SEN_NORMAL;
}
//[*]----------------------------------------------------------------------[*]
//   bulkoii jisu
//[*]----------------------------------------------------------------------[*]
double dewpoint(double Temperature, double Humidity)
{
	double E  = Humidity * 0.0611 * pow(10, ((7.5*Temperature) / (237.3+Temperature)));
	double DP = (237.3 * log10(E) - 186.527) / (8.286 - log10(E));
	return DP;
}
//[*]----------------------------------------------------------------------[*]
float bul_cal(double temp, double humi)
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
void BUL_JISU_0(var_back_t *p)
{
	watch();

	send_ch[Real_ch].real.avr =
		bul_cal(send_ch[channel[Real_ch].ref_ch1].real.avr,
				send_ch[channel[Real_ch].ref_ch2].real.avr);

	if(send_ch[Real_ch].real.avr > 0.)	{		// sensor error flag set
		max_min_pros(p);    send_ch[Real_ch].real.status &= SEN_NORMAL;
		p->nujuk_count++;	p->nujuk_sum += send_ch[Real_ch].real.avr;
	}	else	{
		p->err_flag = ON;	send_ch[Real_ch].real.status |= SEN_FAIL;
	}
}
//[*]----------------------------------------------------------------------[*]
//   pros lib
//[*]----------------------------------------------------------------------[*]
void jugi_pros(byte s_ch)
{
	watch();
	switch(send_ch[s_ch].jugi.sen_code) {
		case  3:
			send_ch[s_ch].jugi.max =
			send_ch[s_ch].jugi.avr = channel[s_ch].var.nujuk_sum;
			send_ch[s_ch].jugi.min = send_ch[s_ch].real.min;
			break;
		case  4:
			send_ch[s_ch].jugi.max =
			send_ch[s_ch].jugi.avr = send_ch[s_ch].real.avr;
			break;
		case  9:
			send_ch[s_ch].jugi.avr =
				(channel[s_ch].var.nujuk_spc >= (float)Jugi_value / 2.) ? 0x10 : 0;
			channel[s_ch].var.nujuk_spc = 0.;
		case 13:
			send_ch[s_ch].jugi.max = (float)(channel[s_ch].var.nujuk_sum / 60.);
			send_ch[s_ch].jugi.min = send_ch[s_ch].real.min;
			break;
		case  8:	case 19:	case 20:
			send_ch[s_ch].jugi.avr =
				(float)(channel[s_ch].var.nujuk_sum / channel[s_ch].var.nujuk_count);
			send_ch[s_ch].jugi.avr  = (float)(send_ch[s_ch].jugi.avr / 3600.);

			send_ch[s_ch].jugi.max += send_ch[s_ch].jugi.avr;
			send_ch[s_ch].real.max  = send_ch[s_ch].jugi.max;
			channel[s_ch].var.nujuk_sum   = 0.;
			channel[s_ch].var.nujuk_count = 0.;
			break;
		default     :
			send_ch[s_ch].jugi.avr =
				(float)(channel[s_ch].var.nujuk_sum / channel[s_ch].var.nujuk_count);
			send_ch[s_ch].jugi.max = channel[s_ch].var.nujuk_max;
			send_ch[s_ch].jugi.min = channel[s_ch].var.nujuk_min;
			channel[s_ch].var.nujuk_max = send_ch[s_ch].real.avr;
			channel[s_ch].var.nujuk_min = send_ch[s_ch].real.avr;
			channel[s_ch].var.nujuk_sum   = 0.;
			channel[s_ch].var.nujuk_count = 0.;
			break;
	}	send_ch[s_ch].jugi.status = NORMAL;
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]








