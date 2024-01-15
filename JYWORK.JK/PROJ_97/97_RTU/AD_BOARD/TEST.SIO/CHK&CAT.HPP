//[*]----------------------------------------------------------------------[*]
byte rx_chk(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != 0xFE)	return 0;
	disable(GREEN);
	if(var->buf[(var->p_sp             +3) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp             +2) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx_cat1(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0;
	word	i = 0, catch_size = 0;

	watch();
	if(var->buf[(var->p_sp +5) % var->size] != Board_id)			return 0;
	if(var->buf[(var->p_sp +6) % var->size] != 'D')					return 0;

	for(i = 4; i<var->size -6; i++)	{
		ck_xor ^= var->buf[(var->p_sp + i) % var->size];
		ck_add += var->buf[(var->p_sp + i) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;

	catch_size   = var->buf[(var->p_sp +7) % var->size] <<8;
	catch_size  |= var->buf[(var->p_sp +8) % var->size]    ;

	for(i = 0; i < com1.pcnt; i++)	{
		if(com1.p[i].var.size == catch_size)	set_protocol_flag(&com1, i, ON);
	}																return 0;
}
//[*]----------------------------------------------------------------------[*]
byte rx_cat2(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0, cmd = 0;
	word	i = 0, catch_jugi = 0;
	float	cat_max  = 0., cat_min   = 0.;

	byte	*pmax = (byte *)(&cat_max);
	byte	*pmin = (byte *)(&cat_min);

	watch();

	if(var->buf[(var->p_sp +9) % var->size] != Board_id)			return 1;

	for(i = 4; i < var->size -6; i++)	{
		ck_xor ^= var->buf[(var->p_sp + i) % var->size];
		ck_add += var->buf[(var->p_sp + i) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 1;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 1;

	cmd         = var->buf[(var->p_sp +4) % var->size];
	catch_jugi  = var->buf[(var->p_sp +5) % var->size] <<8;
	catch_jugi |= var->buf[(var->p_sp +6) % var->size]    ;

	switch(cmd)	{
		case	'W':	if(Reboot_flag)	{
							send_ack(&com1, 'D', sizeof(sensor_field_t), Board_id);
							send_maxmin_call(&com1, &maxmin);		return 1;
						}
						if     (catch_jugi == 0)	{
							send_ack (&com1, 'D', sizeof(sensor_field_t), Board_id);
							send_real(&com1, catch_jugi, &real);
						}
						else if(catch_jugi == 600)	{
							send_ack (&com1, 'D', sizeof(sensor_field_t), Board_id);
							send_jugi(&com1, catch_jugi, &jugi);
						}											return 1;
		case	'I':	init_sensor_data();      					return 1;
		case	'@':  	for(i = 0; i < 4; i++)		{
							pmin[i] = var->buf[(var->p_sp +18 +i) % var->size];
							pmax[i] = var->buf[(var->p_sp +22 +i) % var->size];
						}	init_maxmin_data(cat_max, cat_min);		return 1;
		case	'*': 	Offset = Off_back;
						send_ack(&com1, 'D', sizeof(sensor_field_t), Board_id);
						send_offset(&com1, Offset, &offset);		return 1;
		case	'#':    for(i = 0; i < 4; i++)		{
							pmin[i] = var->buf[(var->p_sp +18 +i) % var->size];
						}
						if(cat_min != 0.)	{	Offset = cat_min;	Offset_auto_flag = 0;}
						else				{	Offset = 0.;		Offset_auto_flag = 1;}
																	return 1;
		default    :												return 1;
	}
}
//[*]----------------------------------------------------------------------[*]
byte snow_chk(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != ASCII_LF) return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != ASCII_CR) return 0;
	if(var->buf[(var->p_sp               ) % var->size] != '+')		 return 0;
																	 return 1;
}
//[*]----------------------------------------------------------------------[*]
byte snow_cat1(variable_t *var)
{
	watch();
	if((var->buf[(var->p_sp +1) % var->size] == '.')) {
		Sensor_cat  = (float)(var->buf[(var->p_sp +2) % var->size] -0x30) * 100.;
		Sensor_cat += (float)(var->buf[(var->p_sp +3) % var->size] -0x30) * 10. ;
		Sensor_cat += (float)(var->buf[(var->p_sp +4) % var->size] -0x30)       ;
		Sensor_ack_count = 0;
	}	else if(var->buf[(var->p_sp +2) % var->size] == '.'  )	{
		Sensor_cat  = (float)(var->buf[(var->p_sp +1) % var->size] -0x30) * 1000.;
		Sensor_cat += (float)(var->buf[(var->p_sp +3) % var->size] -0x30) * 100. ;
		Sensor_cat += (float)(var->buf[(var->p_sp +4) % var->size] -0x30) * 10.  ;
		Sensor_cat += (float)(var->buf[(var->p_sp +5) % var->size] -0x30)        ;
		Sensor_ack_count = 0;
	}	else 	{
		Sensor_cat = -9999.0;		Sensor_ack_count++;
	}	output(MCR(com2.base),CALL_OFF);   							return 0;
}
//[*]----------------------------------------------------------------------[*]
byte led_chk_30(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0x0D)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp + var->size -5) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp + var->size -6) % var->size] != '9' )	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte led_chk_daeju(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0x03)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp + var->size -5) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp + var->size -6) % var->size] != '9' )	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0x02)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte led_cat_30(variable_t *var)
{
	word	i;
	byte	temp_ascii[6] ;
	float	temp_cat;

	watch();
	for(i=0; i<5; i++)	temp_ascii[i] = var->buf[(var->p_sp +10 + i) % var->size];
	temp_ascii[5] = 0;	temp_cat = atof(temp_ascii)  / 10.0;
	if(temp_cat > 50.0 || temp_cat < -50.0)							return 0;
	Temp = (float)(temp_cat + 273.16);								return 0;
}
//[*]----------------------------------------------------------------------[*]
byte led_cat_daeju(variable_t *var)
{
	word	i;
	byte	temp_ascii[6];
	float	temp_cat;

	watch();
	for(i=0; i<5; i++)	temp_ascii[i] = var->buf[(var->p_sp +11 + i) % var->size];
	temp_ascii[5] = 0;	temp_cat = atof(temp_ascii) / 10.0;
	if(temp_cat > 50.0 || temp_cat < -50.0)							return 0;
	Temp = (float)(temp_cat + 273.16);								return 0;
}
//[*]----------------------------------------------------------------------[*]
