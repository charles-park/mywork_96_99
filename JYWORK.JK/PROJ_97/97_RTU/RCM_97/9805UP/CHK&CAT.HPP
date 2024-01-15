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
byte rx1_cat1(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0;
	word	count  = 0, catch_size = 0;

	watch();

	if(var->buf[(var->p_sp +4) % var->size] != RCM_ID)				return 0;
	if(var->buf[(var->p_sp +5) % var->size] != 0xFA  )				return 0;

	for(count = 4; count<var->size-6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}

	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;

	if(var->buf[(var->p_sp +6) % var->size] != 'D')					return 0;

	if(Event_run_flag != ON)	{
		if(Receive_count++ > EVENT_SAVE_WAIT_TIME)		Event_run_flag = ON;
	}

	catch_size   = var->buf[(var->p_sp +7) % var->size] <<8;
	catch_size  |= var->buf[(var->p_sp +8) % var->size]    ;

	for(count = 0; count < com1.pcnt; count++)	{
		if(com1.p[count].var.size == catch_size)	set_protocol_flag(&com1, count, ON);
	}	Mpm_check_count = 0;										return 0;
}
//[*]----------------------------------------------------------------------[*]
byte rx1_cat2(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0;
	word	count  = 0;
	time_t	send_time;

	watch();
	if(var->buf[(var->p_sp +13) % var->size] != RCM_ID)				return 1;
	if(var->buf[(var->p_sp +14) % var->size] != 0xFF)				return 1;
	if(var->buf[(var->p_sp +15) % var->size] != 'L' )				return 1;
	for(count = 4; count<var->size-6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 1;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 1;

	Jijum  = var->buf[(var->p_sp +11) % var->size] <<8;
	Jijum |= var->buf[(var->p_sp +12) % var->size]    ;

	status.send.s_time.yy = send_time.yy = var->buf[(var->p_sp +4) % var->size];
	status.send.s_time.mm = send_time.mm = var->buf[(var->p_sp +5) % var->size];
	status.send.s_time.dd = send_time.dd = var->buf[(var->p_sp +6) % var->size];
	status.send.s_time.hh = send_time.hh = var->buf[(var->p_sp +7) % var->size];
	status.send.s_time.bb = send_time.bb = var->buf[(var->p_sp +8) % var->size];
	status.send.s_time.ss = send_time.ss = var->buf[(var->p_sp +9) % var->size];

	for(count = 0; count<8; count++)	{
		status.send.bd_sta [count] = mpm.send.bd_sta [count] = var->buf[(var->p_sp +28 +count) % var->size];
		status.send.sen_sta[count] = mpm.send.sen_sta[count] = var->buf[(var->p_sp +36 +count) % var->size];
	}
	status.send.bd_sta[0] &= 0xFE;
	mpm.send.bd_sta[0]    &= 0xFE;

	int_send_ack(&com1, 'D', sizeof(mpm_send_u));
	send_mpm    (&com1, 'L', &send_time);
	Event_check_flag = ON;											return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx2_cat1(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0;
	word	count  = 0, catch_size = 0;
	time_t	send_time;

	watch();
	if( var->buf[(var->p_sp +10) % var->size] != Rtuid )			return 0;
	if((var->buf[(var->p_sp +13) % var->size] != RCM_ID) &&
	   (var->buf[(var->p_sp +13) % var->size] != MPM_ID))			return 0;
	if( var->buf[(var->p_sp +14) % var->size] != 0xFF  )			return 0;

	for(count = 4; count<var->size -6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;

	else	if((var->buf[(var->p_sp +13) % var->size] == MPM_ID) &&
			   (var->buf[(var->p_sp +15) % var->size] == 'X'))
	{	Ack_check_count = 0;										return 0;}

	if(var->buf[(var->p_sp +15) % var->size] == 'X')				return 0;

	catch_size   = var->buf[(var->p_sp +16) % var->size] <<8;
	catch_size  |= var->buf[(var->p_sp +17) % var->size]    ;

	status.send.s_time.yy = send_time.yy = var->buf[(var->p_sp +4) % var->size];
	status.send.s_time.mm = send_time.mm = var->buf[(var->p_sp +5) % var->size];
	status.send.s_time.dd = send_time.dd = var->buf[(var->p_sp +6) % var->size];
	status.send.s_time.hh = send_time.hh = var->buf[(var->p_sp +7) % var->size];
	status.send.s_time.bb = send_time.bb = var->buf[(var->p_sp +8) % var->size];
	status.send.s_time.ss = send_time.ss = var->buf[(var->p_sp +9) % var->size];

	if(var->buf[(var->p_sp +15) % var->size] == 'D')	{
		if(var->buf[(var->p_sp +13) % var->size] == MPM_ID)		{
			if(Ack_check_count++ >= ACK_MAX_WAIT)	{
				Ack_check_count = 0;		mpm_reset();
				status.send.cnt_bd.word_data  = 0;
				status.send.cnt_bd.bits.reset = FAIL;
				Event_save_flag = ON;
			}
		}
		for(count = 0; count<com2.pcnt; count++)	{
			if(com2.p[count].var.size == catch_size)	set_protocol_flag(&com2, count, ON);
		}
	}																return 0;
}
//[*]----------------------------------------------------------------------[*]
byte rx2_cat2(variable_t *var)
{
	byte	ck_xor = 0, ck_add     = 0;
	word	count  = 0, event_size = 0;
	time_t	send_time;

	watch();
	if( var->buf[(var->p_sp +10) % var->size] != Rtuid )			return 1;
	if((var->buf[(var->p_sp +13) % var->size] != RCM_ID) &&
	   (var->buf[(var->p_sp +13) % var->size] != MPM_ID))			return 1;
	if( var->buf[(var->p_sp +14) % var->size] != 0xFF  )			return 1;

	if((var->buf[(var->p_sp +15) % var->size] == 'r') ||
	   (var->buf[(var->p_sp +15) % var->size] == 'm') ||
	   (var->buf[(var->p_sp +15) % var->size] == 'n') ||
	   (var->buf[(var->p_sp +15) % var->size] == 'o') ||
	   (var->buf[(var->p_sp +15) % var->size] == 'v'))				return 1;

	for(count = 4; count<var->size -6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 1;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 1;

	status.send.s_time.yy = send_time.yy = var->buf[(var->p_sp +4) % var->size];
	status.send.s_time.mm = send_time.mm = var->buf[(var->p_sp +5) % var->size];
	status.send.s_time.dd = send_time.dd = var->buf[(var->p_sp +6) % var->size];
	status.send.s_time.hh = send_time.hh = var->buf[(var->p_sp +7) % var->size];
	status.send.s_time.bb = send_time.bb = var->buf[(var->p_sp +8) % var->size];
	status.send.s_time.ss = send_time.ss = var->buf[(var->p_sp +9) % var->size];

	switch((var->buf[(var->p_sp +15) % var->size]))	{
		case	RTU_STATUS_CALL  :
			ext_send_ack(&com2, 'D', sizeof(status_u)    , &send_time);
			send_status(&com2, RTU_STATUS_CALL);					return 1;
		case	RTU_VOLT_CALL    :
			ext_send_ack(&com2, 'D', sizeof(volts_send_u), &send_time);
			send_volts (&com2, RTU_VOLT_CALL  , &send_time);		return 1;
		case	RTU_WATCH_CALL   :
			ev_point.trace = ev_point.hear;
			event_size = (ev_point.forward + EVENT_GETSU - ev_point.trace) % EVENT_GETSU;
			ext_send_ack(&com2, 'D', sizeof(status_u) * event_size, &send_time);
			Event_send_flag = ON;                                   return 1;
		case	RTU_RESET_CALL   :
			if(var->buf[(var->p_sp +16) % var->size] == 0xFF)		{
				status.send.cnt_bd.word_data  = 0;
				status.send.cnt_bd.bits.reset = FAIL;
				rtu_reset();	mpm_reset();						break;
			}	else	{
				status.send.cnt_bd.word_data  = 0;
				status.send.cnt_bd.word_data  = var->buf[(var->p_sp +16) % var->size];
				if(status.send.cnt_bd.bits.set_num >  5)			return 1;
				if(status.send.cnt_bd.bits.set_cmd == 0)
					ext_signal_set(OFF, status.send.cnt_bd.bits.set_num);
				else    {
					status.send.cnt_bd.bits.set_cmd = 1;
					ext_signal_set(ON , status.send.cnt_bd.bits.set_num);
				}
			}       												break;
		case	RTU_ID_SET       :
			Ack_check_count = 0;
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.id_set        = FAIL;			break;
		case	RTU_PASS_SET     :
			Ack_check_count = 0;
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.pass_set      = FAIL;			break;
		case	RTU_TIME_DATE_SET:
			Ack_check_count = 0;
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.time_date_set = FAIL;			break;
		case	RTU_BAUD_SET     :
			Ack_check_count = 0;
			Baud_set_flag   = ON;
			Baud_set_port   = 2 ;
			Baud_set_value  = var->buf[(var->p_sp +16) % var->size] <<8;
			Baud_set_value |= var->buf[(var->p_sp +17) % var->size]    ;
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.baud_set      = FAIL;			break;
		case	RTU_BUFFER_CLR   :
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.buffer_clr    = FAIL;			break;
		case	CONFIG_SET       :
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.config_set    = FAIL;			break;
		default                  :                                  return 1;
	}	Event_save_flag = ON ;										return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx3_cat1(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0;
	word	count  = 0, catch_size = 0;
	time_t	send_time;

	watch();
	if( var->buf[(var->p_sp +10) % var->size] != Rtuid )			return 0;
	if((var->buf[(var->p_sp +13) % var->size] != RCM_ID) &&
	   (var->buf[(var->p_sp +13) % var->size] != MPM_ID))			return 0;
	if( var->buf[(var->p_sp +14) % var->size] != 0xFF  )			return 0;

	for(count = 4; count<var->size -6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;

	else	if((var->buf[(var->p_sp +13) % var->size] == MPM_ID) &&
			   (var->buf[(var->p_sp +15) % var->size] == 'X'))
	{	Ack_check_count = 0;										return 0;}

	if(var->buf[(var->p_sp +15) % var->size] == 'X')				return 0;

	catch_size   = var->buf[(var->p_sp +16) % var->size] <<8;
	catch_size  |= var->buf[(var->p_sp +17) % var->size]    ;

	status.send.s_time.yy = send_time.yy = var->buf[(var->p_sp +4) % var->size];
	status.send.s_time.mm = send_time.mm = var->buf[(var->p_sp +5) % var->size];
	status.send.s_time.dd = send_time.dd = var->buf[(var->p_sp +6) % var->size];
	status.send.s_time.hh = send_time.hh = var->buf[(var->p_sp +7) % var->size];
	status.send.s_time.bb = send_time.bb = var->buf[(var->p_sp +8) % var->size];
	status.send.s_time.ss = send_time.ss = var->buf[(var->p_sp +9) % var->size];

	if(var->buf[(var->p_sp +15) % var->size] == 'D')	{
		if(var->buf[(var->p_sp +13) % var->size] == MPM_ID)		{
			if(Ack_check_count++ >= ACK_MAX_WAIT)	{
				Ack_check_count = 0;		mpm_reset();
				status.send.cnt_bd.word_data  = 0;
				status.send.cnt_bd.bits.reset = FAIL;
				Event_save_flag = ON;
			}
		}
		for(count = 0; count<com3.pcnt; count++)	{
			if(com3.p[count].var.size == catch_size)	set_protocol_flag(&com3, count, ON);
		}
	}																return 0;
}
//[*]----------------------------------------------------------------------[*]
byte rx3_cat2(variable_t *var)
{
	byte	ck_xor = 0, ck_add     = 0;
	word	count  = 0, event_size = 0;
	time_t	send_time;

	watch();
	if( var->buf[(var->p_sp +10) % var->size] != Rtuid )			return 1;
	if((var->buf[(var->p_sp +13) % var->size] != RCM_ID) &&
	   (var->buf[(var->p_sp +13) % var->size] != MPM_ID))			return 1;
	if( var->buf[(var->p_sp +14) % var->size] != 0xFF  )			return 1;

	if((var->buf[(var->p_sp +15) % var->size] == 'r') ||
	   (var->buf[(var->p_sp +15) % var->size] == 'm') ||
	   (var->buf[(var->p_sp +15) % var->size] == 'n') ||
	   (var->buf[(var->p_sp +15) % var->size] == 'o') ||
	   (var->buf[(var->p_sp +15) % var->size] == 'v'))				return 1;

	for(count = 4; count<var->size -6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 1;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 1;

	status.send.s_time.yy = send_time.yy = var->buf[(var->p_sp +4) % var->size];
	status.send.s_time.mm = send_time.mm = var->buf[(var->p_sp +5) % var->size];
	status.send.s_time.dd = send_time.dd = var->buf[(var->p_sp +6) % var->size];
	status.send.s_time.hh = send_time.hh = var->buf[(var->p_sp +7) % var->size];
	status.send.s_time.bb = send_time.bb = var->buf[(var->p_sp +8) % var->size];
	status.send.s_time.ss = send_time.ss = var->buf[(var->p_sp +9) % var->size];

	switch((var->buf[(var->p_sp +15) % var->size]))	{
		case	RTU_STATUS_CALL  :
			ext_send_ack(&com3, 'D', sizeof(status_u)    , &send_time);
			send_status(&com3, RTU_STATUS_CALL);					return 1;
		case	RTU_VOLT_CALL    :
			ext_send_ack(&com3, 'D', sizeof(volts_send_u), &send_time);
			send_volts (&com3, RTU_VOLT_CALL  , &send_time);		return 1;
		case	RTU_WATCH_CALL   :
			ev_point.trace = ev_point.hear;
			event_size = (ev_point.forward + EVENT_GETSU - ev_point.trace) % EVENT_GETSU;
			ext_send_ack(&com3, 'D', sizeof(status_u) * event_size, &send_time);
			Event_send_flag = ON;                                   return 1;
		case	RTU_RESET_CALL   :
			if(var->buf[(var->p_sp +16) % var->size] == 0xFF)		{
				status.send.cnt_bd.word_data  = 0;
				status.send.cnt_bd.bits.reset = FAIL;
				rtu_reset();										break;
			}	else	{
				status.send.cnt_bd.word_data  = 0;
				status.send.cnt_bd.word_data  = var->buf[(var->p_sp +16) % var->size];
				if(status.send.cnt_bd.bits.set_num >  5)			return 1;
				if(status.send.cnt_bd.bits.set_cmd == 0)
					ext_signal_set(OFF, status.send.cnt_bd.bits.set_num);
				else    {
					status.send.cnt_bd.bits.set_cmd = 1;
					ext_signal_set(ON , status.send.cnt_bd.bits.set_num);
				}
			}       												break;
		case	RTU_ID_SET       :
			Ack_check_count = 0;
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.id_set        = FAIL;			break;
		case	RTU_PASS_SET     :
			Ack_check_count = 0;
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.pass_set      = FAIL;			break;
		case	RTU_TIME_DATE_SET:
			Ack_check_count = 0;
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.time_date_set = FAIL;			break;
		case	RTU_BAUD_SET     :
			Ack_check_count = 0;
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.baud_set      = FAIL;			break;
		case	RTU_BUFFER_CLR   :
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.buffer_clr    = FAIL;			break;
		case	CONFIG_SET       :
			status.send.cnt_bd.word_data = 0;
			status.send.cnt_bd.bits.config_set    = FAIL;			break;
		default                  :                                  return 1;
	}	Event_save_flag = ON ;										return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx3_debug_chk(variable_t *var)
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
byte rx3_debug_cat(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0, chk_bit = 0;
	word	count  = 0;

	watch();
	for(count = 4; count<var->size -6; count++)	{
		ck_xor ^= var->buf[(var->p_sp + count) % var->size];
		ck_add += var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -6) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -5) % var->size])	return 0;
	debug_data_send(&com2, var);       								return 0;
}
//[*]----------------------------------------------------------------------[*]
