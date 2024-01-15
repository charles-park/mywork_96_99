//[*]----------------------------------------------------------------------[*]
//   CHK&CAT.HPP File (Protocol Check & Protocol Catch Function)
//[*]----------------------------------------------------------------------[*]
//   Receive Q Protocol Check ( Header & Tail )
//[*]----------------------------------------------------------------------[*]
byte rx_check(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)    return 0;
	RX_LED(OFF);
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
//   Receive Q Protocol Catch ( Command & Data )
//[*]----------------------------------------------------------------------[*]
byte rx_catch(variable_t *var)
{
	byte	ck_xor = 0, cat_id = 0, send_p = REAL, comd = 0x00;
	word	count  = 0, mmax   = 0, mmin   = 0;

	for(count = 0; count<var->size; count++)	{
		if(count > 1 && count < (var->size -3))
			ck_xor ^= var->buf[(var->p_sp + count) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -3) % var->size])	return 0;

	cat_id   = var->buf[(var->p_sp +3) % var->size];

	if(cat_id != Sam_id   && cat_id != Sam_id+1 &&
	   cat_id != Sam_id+2 && cat_id != Sam_id+3	  )					return 0;

	switch( var->buf[(var->p_sp +2) % var->size])	{
		case	'M':    send_p = REAL ;		comd = 'M';				break;
		case	'T':	send_p = MIN10;		comd = 'T';				break;
		case	'I':	init_val  (cat_id);							return 0;
		case	'@':	mmin  = var->buf[(var->p_sp +4) % var->size]<<8;
						mmin |= var->buf[(var->p_sp +5) % var->size]   ;
						mmax  = var->buf[(var->p_sp +6) % var->size]<<8;
						mmax |= var->buf[(var->p_sp +7) % var->size]   ;
						maxmin_val(cat_id, mmax, mmin);				return 0;
		default	   :												return 0;
	}
	if         (cat_id == Sam_id  )	{
		if(Temp0_flag)	init_send(cat_id);
		else			data_send(&temp0_data[send_p], cat_id, comd);
	}	else if(cat_id == Sam_id+1)	{
		if(Temp1_flag)	init_send(cat_id);
		else			data_send(&temp1_data[send_p], cat_id, comd);
	}	else if(cat_id == Sam_id+2)	{
		if(Temp2_flag)	init_send(cat_id);
		else			data_send(&temp2_data[send_p], cat_id, comd);
	}	else if(cat_id == Sam_id+3)	{
		if(Prec_flag)	init_send(cat_id);
		else			data_send(&prec_data[send_p] , cat_id, comd);
	}																	return 0;
}
//[*]----------------------------------------------------------------------[*]



