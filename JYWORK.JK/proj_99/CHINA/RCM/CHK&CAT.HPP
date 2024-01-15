//[*]----------------------------------------------------------------------[*]
//   CHK&CAT.HPP File (Protocol Check & Protocol Catch Function)
//[*]----------------------------------------------------------------------[*]
//   Receive Q Protocol Check ( Header & Tail )
//[*]----------------------------------------------------------------------[*]
byte rx_check(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)    return 0;

	RX_LED1(OFF);	RX_LED2(OFF);

	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
//   Receive Q Protocol Catch ( Command & Data )
//[*]----------------------------------------------------------------------[*]
byte rx_catch_int(variable_t *var)
{
	byte	ck_xor = 0;
	word	count  = 0;

	for(count = 0; count<var->size; count++)	{
		if(count > 1 && count < (var->size -3))
			ck_xor ^= var->buf[(var->p_sp + var->size + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -3) % var->size])	return 0;

	if(var->buf[(var->p_sp  +2) % var->size] != 'L' )				return 0;
	if(var->buf[(var->p_sp  +3) % var->size] != 0xFA)				return 0;

	LOG_ID  = var->buf[(var->p_sp +4) % var->size] << 8;
	LOG_ID |= var->buf[(var->p_sp +5) % var->size]     ;

	for(count=0; count<4; count++)	{
		mpm.send.sta.board [count] =
					var->buf[(var->p_sp + 14 + count) % var->size];
		mpm.send.sta.sensor[count] =
					var->buf[(var->p_sp + 19 + count) % var->size];
	}
	mpm.send.sta.cnt_board.byte_data =
					var->buf[(var->p_sp + 18        ) % var->size];

	if(mpm_status < ERROR_TIME)		{
		mpm_status = 0;	mpm.send.sta.cnt_board.bits.MPM_STA = NORMAL;
	} else				mpm.send.sta.cnt_board.bits.MPM_STA = FAIL;
	send_to_mpm(&com_int);											return 0;
}
//[*]----------------------------------------------------------------------[*]
//   External Serial Catch
//[*]----------------------------------------------------------------------[*]
byte rx_catch_ext(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0;
	word	count  = 0;

	// RX_LED2(OFF);
	for(count = 0; count<var->size; count++)	{
		if(count > 1 && count < (var->size -4))	{
			ck_xor ^= var->buf[(var->p_sp  + count) % var->size];
			ck_add += var->buf[(var->p_sp  + count) % var->size];
		}
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -4) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -3) % var->size])	return 0;

	t.year  = var->buf[(var->p_sp  +2) % var->size];
	t.month = var->buf[(var->p_sp  +3) % var->size];
	t.day   = var->buf[(var->p_sp  +4) % var->size];
	t.hour  = var->buf[(var->p_sp  +5) % var->size];
	t.min   = var->buf[(var->p_sp  +6) % var->size];
	t.sec   = var->buf[(var->p_sp  +7) % var->size];

	switch(var->buf[(var->p_sp + var->size +12) % var->size])	{
		case	'S':	send_to_host_status(&com_ext);				return 0;
		case	'P':	send_to_host_detail(&com_ext);				return 0;
		case	'R':	mpm_reset();								return 0;
		case	'I':	case	'B':		case	'A':
			if(++call_count > 100)		mpm_reset();				return 0;
		case	'X':	call_count = 0;								return 0;
		default	   :	                                            return 0;
	}
}
//[*]----------------------------------------------------------------------[*]
