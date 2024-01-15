//[*]----------------------------------------------------------------------[*]
byte rx_chk(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != 0xFE)	return 0;

	if(var->buf[(var->p_sp             +3) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp             +2) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx_cat1(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0, catch_id = 0, catch_id_flag = OFF;
	word	count  = 0, catch_size = 0;

	watch();

	for(count = 4; count<var->size -6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;

	if(var->buf[(var->p_sp +6) % var->size] != 'D')					return 0;

	catch_id = var->buf[(var->p_sp +5) % var->size];

	catch_id_flag = OFF;

	for(count = 0; count <7; count++)	{
		if((catch_id == ch.num[count].sensor.sen_id) &&
		   (channel_status(count) == ON)              )
		{
			count = END_CAL;	catch_id_flag = ON ;
		}		else			catch_id_flag = OFF;
	}
	if(catch_id_flag == OFF)										return 0;

	catch_size   = var->buf[(var->p_sp +7) % var->size] <<8;
	catch_size  |= var->buf[(var->p_sp +8) % var->size]    ;

	for(count = 0; count < com1.pcnt; count++)	{
		if(com1.p[count].var.size == catch_size)	set_protocol_flag(&com1, count, ON);
	}																return 0;
}
//[*]----------------------------------------------------------------------[*]
byte rx_cat2(variable_t *var)
{
	byte	ck_xor   = 0, ck_add     = 0, num = 0, cmd = 0;
	byte	catch_ch = 0, catch_id   = 0, catch_id_flag = OFF;
	word	count    = 0, catch_jugi = 0;
	float	cat_max  = 0., cat_min   = 0.;

	byte	*pmax = (byte *)(&cat_max);
	byte	*pmin = (byte *)(&cat_min);

	watch();
	for(count = 4; count<var->size -6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 1;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 1;

	cmd         = var->buf[(var->p_sp +4) % var->size];
	catch_jugi  = var->buf[(var->p_sp +5) % var->size] <<8;
	catch_jugi |= var->buf[(var->p_sp +6) % var->size]    ;
	catch_id    = var->buf[(var->p_sp +9) % var->size];

	catch_id_flag = OFF;
	for(count = 0; count <7; count++)	{
		if(catch_id == ch.num[count].sensor.sen_id)	{
			catch_ch = count;	catch_id_flag = ON ;	count = END_CAL;
		}		else			catch_id_flag = OFF;
	}
	if(catch_id_flag == OFF)										return 1;

	switch(cmd)	{
		case	'W':	if(ch.num[catch_ch].var.reboot_flag)	{
							send_ack(&com1, 'D', sizeof(sensor_field_t), catch_id);
							send_maxmin_call(&com1, &ch.num[catch_ch]);
																	return 1;
						}
						if     (catch_jugi == REAL_JUGI)			num = 0;
						else if(catch_jugi == ch.jugi1 )        	num = 1;
						else if(catch_jugi == ch.jugi2 )        	num = 2;
						else 										return 1;
						send_ack(&com1, 'D', sizeof(sensor_field_t), catch_id);
						send_mpm(&com1, catch_jugi, &ch.num[catch_ch], num);
																	return 1;
		case	'I':	init_sensor_data(&ch.num[catch_ch]);        return 1;
		case	'@':  	for(count = 0; count < 4; count ++)		{
							pmin[count] = var->buf[(var->p_sp +18 +count) % var->size];
							pmax[count] = var->buf[(var->p_sp +22 +count) % var->size];
						}
						init_maxmin_data(&ch.num[catch_ch], cat_max, cat_min);
																	return 1;
		default    :												return 1;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]






