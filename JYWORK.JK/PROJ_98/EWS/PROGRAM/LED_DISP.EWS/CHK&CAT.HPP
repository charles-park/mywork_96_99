byte rx_chk(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp 			   +1) % var->size] != 0xFF)	return 0;
	if(var->buf[(var->p_sp 				 ) % var->size] != 0xFF)	return 0;
																	return 1;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
byte rx_cat(variable_t *var)
{
	word	i;
	byte	ck_xor = 0 ;

	watch();
	for(i=0; i<sizeof(sensor_t); i++)	{
		if(i > 1 && i<sizeof(sensor_t)-3)
			ck_xor ^= var->buf[(var->p_sp +i) % var->size];
		recv.bytes[i] = var->buf[(var->p_sp +i) % var->size];
	}
	if(ck_xor != var->buf[(var->p_sp + 14) % var->size])			return 0;

	watch();
	switch(recv.send.sid)	{
		case	0:
			led_disp.byte_data = 0x00;
			switch(recv.send.cmd)	{
				case	'M':
					switch(recv.send.scode)	{
						case	0:	led_disp.bits.real  = ON;
									Disp_mode = REAL_MODE;			break;
						case	1:  led_disp.bits.min   = ON;
									Disp_mode = MIN_MODE; 			break;
						case	2:  led_disp.bits.max   = ON;
									Disp_mode = MAX_MODE; 			break;
						default	 :  Recv_sta = FAIL;				return 0;

					}	output(LED_OUT, ~(led_disp.byte_data));
					Recv_sta = NORMAL;								return 0;
				case	'T':    Disp_mode = MIN10_MODE;
								led_disp.bits.min10  = ON;
								output(LED_OUT, ~(led_disp.byte_data));
								Recv_sta = NORMAL;					return 0;
				case	'o':    Disp_mode = HOUR_MODE;
								led_disp.bits.hour   = ON;
								output(LED_OUT, ~(led_disp.byte_data));
								Recv_sta = NORMAL;					return 0;
				default	   :	Recv_sta = FAIL;					return 0;
			}
		case	1:													return 0;
		case	2:
		// Time Mode
			if(Recv_sta == FAIL)									return 0;
			Error_count = 0;	Error_flag = OFF;
			time.hh = (byte)recv.send.avr;	time.bb = (byte)recv.send.spc;
			time.ss = 0; // recv.send.min
			convert(recv.send.avr * 100 + recv.send.spc, recv.send.sid);
																	break;
		case	3:	case	4:	case	5:	case	6:
		case	7:	case	8:	case	9:	case	10:
			if(Recv_sta == FAIL)									return 0;
			Error_count = 0;	Error_flag = OFF;
			if     (Disp_mode == MAX_MODE)	convert(recv.send.max, recv.send.sid);
			else if(Disp_mode == MIN_MODE)	convert(recv.send.min, recv.send.sid);
			else 							convert(recv.send.avr, recv.send.sid);
																	break;
		default	 :													return 0;
	}																return 0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void convert(word d_data, byte id)
{
	byte	temp[4], i, flag;

	watch();
	for(i=0; i<4; i++)	temp[i] = BLK;

	watch();
	if(id == 5 || id == 10)	{	// temperature...
		// Error Data Detect..
		if(d_data > 1500 || d_data < 500)	d_data = 1000;

		if(d_data < 1000)	{
			flag = MINUS;	d_data = 1000 - d_data;
		}	else	{
			flag = BLK;		d_data = d_data - 1000;
		}

		if((d_data / 100 ) == 0)	temp[1] = BLK;
		else						temp[1] = TABLE[(d_data / 100 ) % 10];

		if(temp[1] == BLK)		{
			temp[0] = BLK;	temp[1] = flag;
		}	else	temp[0] = flag;

		temp[2] = TABLE[(d_data / 10) % 10] & POINT;
		temp[3] = TABLE[ d_data % 10      ];
	}	else	{
		if((d_data / 1000) == 0)	temp[0] = BLK;
		else						temp[0] = TABLE[(d_data / 1000) % 10];

		if(temp[0] != BLK)          temp[1] = TABLE[(d_data / 100 ) % 10];
		else	{
			if(id == 2)                 	temp[1] = TABLE[(d_data / 100 ) % 10];
			else	{
				if((d_data / 100 ) == 0)	temp[1] = BLK;
				else						temp[1] = TABLE[(d_data / 100 ) % 10];
			}
		}
		if(id == 2)     temp[2] = TIME_TABLE[(d_data / 10) % 10] & POINT;
		else			temp[2] = TABLE     [(d_data / 10) % 10] & POINT;
		temp[3] = TABLE[ d_data % 10      ];
	}
	if(id == 2)		temp[1] = temp[1] & POINT;

	watch();
	switch(id)	{
		case	2 :	 for(i=0; i<4; i++)	buf2[TIME + i] = temp[i];	break;
		case	3 :  for(i=0; i<4; i++)	buf0[WD   + i] = temp[i];	break;
		case	4 :  for(i=0; i<4; i++)	buf1[WS   + i] = temp[i];	break;
		case	5 :  for(i=0; i<4; i++)	buf2[TEMP + i] = temp[i];	break;
		case	6 :  for(i=0; i<4; i++)	buf0[HUMI + i] = temp[i];	break;
		case	7 :  for(i=0; i<4; i++)	buf1[PRE  + i] = temp[i];	break;
		case	8 :  for(i=0; i<4; i++)	buf2[YPRE + i] = temp[i];	break;
		case	9 :  for(i=0; i<4; i++)	buf1[BUL  + i] = temp[i];	break;
		case	10:  for(i=0; i<4; i++)	buf0[CHE  + i] = temp[i];	break;
		default	  :													return;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
