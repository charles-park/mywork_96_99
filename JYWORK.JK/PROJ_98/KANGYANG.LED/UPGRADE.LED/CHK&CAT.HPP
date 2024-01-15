//[*]----------------------------------------------------------------------[*]
byte rx_chk(variable_t *var)
{
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp             +4) % var->size] != 'M' )	return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx_cat1(variable_t *var)
{
	byte	ck_xor = 0, ck_add = 0;
	word	i;

	for(i=0; i<var->size; i++)	{
		if(i > 1 && i< var->size -4)	{
			ck_xor ^= var->buf[(var->p_sp + i) % var->size];
			ck_add += var->buf[(var->p_sp + i) % var->size];
		}
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -4) % var->size])	return 0;
	if(ck_add != var->buf[(var->p_sp + var->size -3) % var->size])	return 0;
	Led_send = ON;
	led_data_catch(var);
																	return 0;
}
//[*]----------------------------------------------------------------------[*]
void led_data_catch(variable_t *var)
{
	word	i = 0, value1 = 0, value2 = 0;
	byte	scode, cmd, sid;

	for(i=10; i < 143; i += 12)	{

		cmd   = var->buf[(var->p_sp +i   ) % var->size];
		sid   = var->buf[(var->p_sp +i +2) % var->size];
		scode = var->buf[(var->p_sp +i +3) % var->size];

		if(cmd == 'M')	{
			switch(scode)	{
				case	2:
					//if(sid != 2)								break;
					value2  = var->buf[(var->p_sp +i+8) % var->size] << 8;
					value2 |= var->buf[(var->p_sp +i+9) % var->size];
					value1  = var->buf[(var->p_sp +i+4) % var->size] << 8;
					value1 |= var->buf[(var->p_sp +i+5) % var->size];	break;
				case	3:
					value2  = var->buf[(var->p_sp +i+8) % var->size] << 8;
					value2 |= var->buf[(var->p_sp +i+9) % var->size];
					// Average	pros(ON/OFF)
					//value1  = var->buf[(var->p_sp +i+4) % var->size] << 8;
					//value1 |= var->buf[(var->p_sp +i+5) % var->size];	break;
					// Maximun	pros(Nujuk)
					value1  = var->buf[(var->p_sp +i+10) % var->size] << 8;
					value1 |= var->buf[(var->p_sp +i+11) % var->size];	break;
				case	6:
					//if(sid != 4)								break;
				case	0: 	case	1:
					value1  = var->buf[(var->p_sp +i+4) % var->size] << 8;
					value1 |= var->buf[(var->p_sp +i+5) % var->size];	break;
				default	 :                                           	break;
			}
			disp_buffer_save(scode, value1, value2);
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void disp_buffer_save(byte scode, word v1, word v2)
{
	byte	d1[5], d2[5], i;

	if(scode == 2)	{
		// Temperature calculate
		if(v1 > 1000)				v1 = v1 - 1000;
		else	{	Minus1 = ON;	v1 = 1000 - v1;	}
		if(v2 > 1000)				v2 = v2 - 1000;
		else	{	Minus2 = ON;	v2 = 1000 - v2;	}
	}
	d1[0] = v1 / 10000;	  v1 %= 10000;
	d1[1] = v1 / 1000 ;   v1 %= 1000 ;
	d1[2] = v1 / 100  ;   v1 %= 100  ;
	d1[3] = v1 / 10   ;   d1[4] = v1 % 10;

	d2[0] = v2 / 10000;	  v2 %= 10000;
	d2[1] = v2 / 1000 ;   v2 %= 1000 ;
	d2[2] = v2 / 100  ;   v2 %= 100  ;
	d2[3] = v2 / 10   ;   d2[4] = v2 % 10;

	switch(scode)	{
		case	0:	for(i=0; i<5; i++)	disp_buf.wd      [i] = d1[i];	break;
		case	1:  for(i=0; i<5; i++)	disp_buf.ws      [i] = d1[i];	break;
		case	2:	for(i=0; i<5; i++)	disp_buf.temp_avr[i] = d1[i];
					for(i=0; i<5; i++)	disp_buf.temp_min[i] = d2[i];	break;
		case	3:	for(i=0; i<5; i++)	disp_buf.pre_avr [i] = d1[i];
					for(i=0; i<5; i++)	disp_buf.pre_min [i] = d2[i];	break;
		case	6:  for(i=0; i<5; i++)	disp_buf.humi    [i] = d1[i];	break;
		default	 :														return;
	}
}
//[*]----------------------------------------------------------------------[*]
