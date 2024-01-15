//[*]----------------------------------------------------------------------[*]
//   'I' command
//[*]----------------------------------------------------------------------[*]
void init_val  (byte id)
{
	if(id == Sam_id  )	{
		Temp0_flag = OFF;
		temp0_data[REAL ].sam.max = temp0_data[REAL ].sam.min =
		temp0_data[MIN10].sam.max = temp0_data[MIN10].sam.min =
		temp0_data[MIN10].sam.avr = temp0_data[REAL ].sam.avr  ;
		temp0_data[MIN10].sam.spc = temp0_data[REAL ].sam.spc = 0;
		temp0_pros.avr = temp0_pros.min = temp0_pros.max =
										  temp0_data[REAL].sam.avr;
		temp0_pros.count = 0 ;			  temp0_pros.sum   = 0.;
	}
	else if(id == Sam_id+1)	{
		Temp1_flag = OFF;
		temp1_data[REAL ].sam.max = temp1_data[REAL ].sam.min =
		temp1_data[MIN10].sam.max = temp1_data[MIN10].sam.min =
		temp1_data[MIN10].sam.avr = temp1_data[REAL ].sam.avr  ;
		temp1_data[MIN10].sam.spc = temp1_data[REAL ].sam.spc = 0;
		temp1_pros.avr = temp1_pros.min = temp1_pros.max =
										  temp1_data[REAL].sam.avr;
		temp1_pros.count = 0 ;			  temp1_pros.sum   = 0.;
	}
	else if(id == Sam_id+2)	{
		Temp2_flag = OFF;
		temp2_data[REAL ].sam.max = temp2_data[REAL ].sam.min =
		temp2_data[MIN10].sam.max = temp2_data[MIN10].sam.min =
		temp2_data[MIN10].sam.avr = temp2_data[REAL ].sam.avr  ;
		temp2_data[MIN10].sam.spc = temp2_data[REAL ].sam.spc = 0;
		temp2_pros.avr = temp2_pros.min = temp2_pros.max =
										  temp2_data[REAL].sam.avr;
		temp2_pros.count = 0 ;			  temp2_pros.sum   = 0.;
	}
	else if(id == Sam_id+3)	{
		Prec_flag = OFF;                    Prec_count = 0;
		prec_data[REAL ].sam.max = prec_data[REAL ].sam.min =
		prec_data[MIN10].sam.max = prec_data[MIN10].sam.min =
		prec_data[MIN10].sam.avr = prec_data[REAL ].sam.avr = 0;
		prec_data[MIN10].sam.spc = prec_data[REAL ].sam.spc = 0;
		prec_pros.avr = prec_pros.min = prec_pros.max = 0;
		prec_pros.count = 0 ;		  prec_pros.sum   = 0.;
	}
}
//[*]----------------------------------------------------------------------[*]
//   '@' command
//[*]----------------------------------------------------------------------[*]
void maxmin_val(byte id, word mmax, word mmin)
{
	if(id == Sam_id  )	{
		Temp0_flag = OFF;
		temp0_data[REAL ].sam.max = mmax;
		temp0_data[REAL ].sam.min = mmin;
		temp0_data[REAL ].sam.spc = 0   ;
		temp0_data[MIN10].sam.max = temp0_data[MIN10].sam.min =
		temp0_data[MIN10].sam.avr = temp0_data[REAL ].sam.avr  ;
		temp0_data[MIN10].sam.spc = 0   ;
		temp0_pros.avr = temp0_pros.min = temp0_pros.max =
										  temp0_data[REAL].sam.avr;
		temp0_pros.count = 0 ;			  temp0_pros.sum   = 0.;
	}
	else if(id == Sam_id+1)	{
		Temp1_flag = OFF;
		temp1_data[REAL ].sam.max = mmax;
		temp1_data[REAL ].sam.min = mmin;
		temp1_data[REAL ].sam.spc = 0   ;
		temp1_data[MIN10].sam.max = temp1_data[MIN10].sam.min =
		temp1_data[MIN10].sam.avr = temp1_data[REAL ].sam.avr  ;
		temp1_data[MIN10].sam.avr = 0   ;
		temp1_pros.avr = temp1_pros.min = temp1_pros.max =
										  temp1_data[REAL].sam.avr;
		temp1_pros.count = 0 ;			  temp1_pros.sum   = 0.;
	}
	else if(id == Sam_id+2)	{
		Temp2_flag = OFF;
		temp2_data[REAL ].sam.max = mmax;
		temp2_data[REAL ].sam.min = mmin;
		temp2_data[REAL ].sam.spc = 0   ;
		temp2_data[MIN10].sam.max = temp2_data[MIN10].sam.min =
		temp2_data[MIN10].sam.avr = temp2_data[REAL ].sam.avr  ;
		temp2_data[MIN10].sam.spc = 0   ;
		temp2_pros.avr = temp2_pros.min = temp2_pros.max =
										  temp2_data[REAL].sam.avr;
		temp2_pros.count = 0 ;			  temp2_pros.sum   = 0.;
	}
	else if(id == Sam_id+3)	{
		Prec_flag = OFF;					Prec_count = 0;
		prec_data[REAL ].sam.max = prec_data[REAL ].sam.avr = mmax;
		prec_data[REAL ].sam.min = mmin;
		prec_data[REAL ].sam.spc = 0   ;
		prec_data[MIN10].sam.max = mmax;
		prec_data[MIN10].sam.min = mmin;
		prec_data[MIN10].sam.avr = 0   ;
		prec_data[MIN10].sam.spc = 0   ;
		prec_pros.avr = prec_pros.min = prec_pros.max = 0;
		prec_pros.count = 0 ;		  prec_pros.sum   = 0.;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
//   A/D Converter READ
//[*]----------------------------------------------------------------------[*]
word adc_read(void)
{
	word	i, ret_value = 0;
	byte	ad_h = 0, ad_l = 0;

	ADC_STATUS = HOLD;
	for(i=0; i<10; i++)		{	i++;	i--;	}
	output(ADC_MSB, 0xff);
	for(i=0; i<10; i++)		{	i++;	i--;	}
	ADC_STATUS = SAMPLE;

	i = 0;
	while(ADC_STS)	{	if(i++ > 100)		break;	}
	if(i > 100)		{
		status_temp0.bits.adc = status_temp1.bits.adc =
		status_temp2.bits.adc = status_prec .bits.adc = FAIL  ;
	}	else	{
		status_temp0.bits.adc = status_temp1.bits.adc =
		status_temp2.bits.adc = status_prec .bits.adc = NORMAL;
	}

//	ret_value  = input(ADC_LSB) >>4;	ret_value |= input(ADC_MSB) <<4;
	ad_h       = input(ADC_MSB);		ad_l       = input(ADC_LSB);
	ret_value  = (ad_l & 0x0F);			ret_value |= (ad_h & 0xF0);
	ret_value |= (ad_h & 0x0F) <<8;

	return	(word)(ret_value);
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
//   Temperature cal
//[*]----------------------------------------------------------------------[*]
void temp_cal(sensor_u *sen, sensor_pros_t *p)
{
	word 	ad_value = 0;

	ad_value = adc_read();
	if(ad_value == 0)	sen->sam.avr = 500;
	else				sen->sam.avr =
							(word)(500.+ (float)((float)ad_value / 4.095));

	// Max Min Pors & Sensor Error Detect
	if(sen->sam.avr < 1500 && sen->sam.avr > 500)	{
		sen->sam.status &= SEN_NORMAL;
		if     (sen->sam.max < sen->sam.avr)	sen->sam.max = sen->sam.avr;
		else if(sen->sam.min > sen->sam.avr)	sen->sam.min = sen->sam.avr;
		if     (p->max       < sen->sam.avr)	p->max       = sen->sam.avr;
		else if(p->min       > sen->sam.avr)	p->min       = sen->sam.avr;
		p->sum += sen->sam.avr;					p->count ++;
	}	else									sen->sam.status |= SEN_FAIL;
}
//[*]----------------------------------------------------------------------[*]
//   Precipitation calculate
//[*]----------------------------------------------------------------------[*]
void prec_cal(sensor_u *sen, sensor_pros_t *p)
{
	if(Prec_count > 0)	{
		sen->sam.max += 5;		sen->sam.avr += 5;
		p->sum       += 5;		Prec_count    = 0;
		// PRE gangdo
		if(gangdo_flag)			gangdo_pros = ON;
		else					gangdo_pros = OFF;
		gangdo_flag = ON;
	}
	// PREC GANG DO
	if(gangdo_flag)	{
		if(gangdo_pros)	{
			sen->sam.spc = (word)((float)(1800./(float)gangdo_count) * 10.);
			gangdo_pros  = OFF;		gangdo_count = 0;
		}
		if(gangdo_count++ >= 600)	{
			gangdo_flag  = OFF;		gangdo_count = 0;
			sen->sam.spc = 0  ;
		}
	}
	sen->sam.status &= SEN_NORMAL;
}
//[*]----------------------------------------------------------------------[*]
//   Jugi Pros
//[*]----------------------------------------------------------------------[*]
void jugi_pros(byte num)
{
	switch(num)	{
		case	1:
			temp0_data[MIN10].sam.avr =
				(word)(temp0_pros.sum / (long)temp0_pros.count);
			temp0_data[MIN10].sam.max = temp0_pros.max;
			temp0_data[MIN10].sam.min = temp0_pros.min;
			temp0_pros.max = temp0_pros.min = temp0_data[REAL].sam.avr;
			temp0_pros.sum = 0.;	temp0_pros.count = 0;	break;
		case	2:
			temp1_data[MIN10].sam.avr =
				(word)(temp1_pros.sum / (long)temp1_pros.count);
			temp1_data[MIN10].sam.max = temp1_pros.max;
			temp1_data[MIN10].sam.min = temp1_pros.min;
			temp1_pros.max = temp1_pros.min = temp1_data[REAL].sam.avr;
			temp1_pros.sum = 0.;	temp1_pros.count = 0;	break;
		case	3:
			temp2_data[MIN10].sam.avr =
				(word)(temp2_pros.sum / (long)temp2_pros.count);
			temp2_data[MIN10].sam.max = temp2_pros.max;
			temp2_data[MIN10].sam.min = temp2_pros.min;
			temp2_pros.max = temp2_pros.min = temp2_data[REAL].sam.avr;
			temp2_pros.sum = 0.;	temp2_pros.count = 0;	break;
		case	4:
			prec_data[MIN10].sam.avr = (word)prec_pros.sum    ;
			prec_data[MIN10].sam.max = prec_data[REAL].sam.max;
			prec_data[MIN10].sam.min = prec_data[REAL].sam.min;
			prec_pros.sum = 0.;                             break;
		default  :											break;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]




