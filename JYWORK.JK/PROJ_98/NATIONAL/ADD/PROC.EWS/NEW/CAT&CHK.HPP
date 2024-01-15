//[*]----------------------------------------------------------------------[*]
//   Protocol Description.(17 bytes Protocol) -> Key Check...
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
byte rx_check(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)	return 0;
	RX_LED = OFF;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx_catch(variable_t *var)
{
	byte	ck_xor 	= 0;
	word	i 		= 0;
	time_t	set_time;

	watch();
	for(i = 2; i<sizeof(sensor_t) -3; i++)
		ck_xor ^= var->buf[(var->p_sp + i) % var->size];
	if(ck_xor != var->buf[(var->p_sp + var->size -3) % var->size])	return 0;

	switch(var->buf[(var->p_sp + 2) % var->size])	{
		case	'K':	case	'k':
			// 'M', 'T', 'o'
			Send_mode = var->buf[(var->p_sp + 4) % var->size];		break;
		case	'S':	case	's':
			set_time.yy = 98;	set_time.mm = 8 ;	set_time.dd = 28;
			set_time.hh = var->buf[(var->p_sp + 7 ) % var->size];
			set_time.bb = var->buf[(var->p_sp + 9 ) % var->size];
			set_time.ss = 0;
			while(rtcc_date_time_set(&set_time, TIME_SET));
																	return 0;
		default	   :												return 0;
	}	Send_flag = ON;												return 0;
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
