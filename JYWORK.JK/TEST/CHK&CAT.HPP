//[*]----------------------------------------------------------------------[*]
byte rx_chk(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != 0xFE)	return 0;
	LED_BLUE = OFF;
	if(var->buf[(var->p_sp             +3) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp             +2) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx_cat1(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0, catch_id = 0, catch_flag = OFF, ch = 0;
	word	i = 0, d_size = 0;

	watch();
	catch_id = var->buf[(var->p_sp +5) % var->size];
	for(i = 0; i <7; i++)	{
		if((catch_id == send_ch[i].real.sen_id) && (Ch_flag[i] == ON))	{
			ch = i;		i = END_CAL;	catch_flag = ON ;
		}
	}
	if(catch_flag == OFF)										return 0;

	for(i = 4; i<var->size -6; i++)	{
		ck_xor ^= var->buf[(var->p_sp + i) % var->size];
		ck_add += var->buf[(var->p_sp + i) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;

	if(var->buf[(var->p_sp +6) % var->size] != 'D')					return 0;

	d_size   = var->buf[(var->p_sp +7) % var->size] <<8;
	d_size  |= var->buf[(var->p_sp +8) % var->size]    ;

	for(i = 0; i < com1.pcnt; i++)	{
		if(com1.p[i].var.size == d_size)	set_protocol_flag(&com1, i, ON);
	}																return 0;
}
//[*]----------------------------------------------------------------------[*]
byte rx_cat2(variable_t *var)
{
	byte	ck_xor   = 0, ck_add     = 0, cmd = 0;
	byte	catch_ch = 0, catch_id   = 0, catch_flag = OFF;
	word	i        = 0, catch_jugi = 0;

	watch();
	catch_id    = var->buf[(var->p_sp +9) % var->size];
	for(i = 0; i <7; i++)	{
		if(catch_id == send_ch[i].real.sen_id)	{
			catch_ch = i;	catch_flag = ON ;	i = END_CAL;
		}
	}
	if(catch_flag == OFF)											return 1;
	if(Ch_flag[catch_ch] == OFF)                                    return 1;

	for(i = 4; i<var->size -6; i++)	{
		ck_xor ^= var->buf[(var->p_sp + i) % var->size];
		ck_add += var->buf[(var->p_sp + i) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 1;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 1;

	cmd         = var->buf[(var->p_sp +4) % var->size];
	catch_jugi  = var->buf[(var->p_sp +5) % var->size] <<8;
	catch_jugi |= var->buf[(var->p_sp +6) % var->size]    ;

	switch(cmd)	{
		case	'W':	switch(catch_jugi)	{
							case	0:
								send_ack(&com1, 'D', sizeof(sensor_field_t), catch_id);
								real_send(&com1, catch_ch);			return 1;
							case	900:
								send_ack(&com1, 'D', sizeof(sensor_field_t), catch_id);
								jugi_send(&com1, catch_ch);			return 1;
							default	:								return 1;
						}
		case	'I':
		case	'@':	init_sensor_data(catch_ch);                 return 1;
		default    :												return 1;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
