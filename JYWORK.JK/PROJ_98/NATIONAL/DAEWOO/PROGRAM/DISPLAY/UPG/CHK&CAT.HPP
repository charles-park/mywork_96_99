byte rx_chk(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)	return 0;
	LED_BLUE(OFF);
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
				case	'M':	led_disp.bits.real  = ON;
								Disp_mode = REAL_MODE;
								output(LED_ADDR, (led_disp.byte_data));
								Recv_sta = NORMAL;					return 0;
				case	'T':    Disp_mode = MIN10_MODE;
								led_disp.bits.min10  = ON;
								output(LED_ADDR, (led_disp.byte_data));
								Recv_sta = NORMAL;					return 0;
				case	'o':    Disp_mode = HOUR_MODE;
								led_disp.bits.hour   = ON;
								output(LED_ADDR, (led_disp.byte_data));
								Recv_sta = NORMAL;					return 0;
				default	   :	Recv_sta = FAIL;					return 0;
			}
		case	1:	case	2:										return 0;
		case	3:	case	4:	case	5:	case	6:
		case	7:	case	8:	case	9:	case	10:
			if(Recv_sta == FAIL)									return 0;
			send_to_pc(&recv);		Error_count = 0;	Error_flag = OFF;
			output(LED_ADDR, 0x00);	output(LED_ADDR, (led_disp.byte_data));

			if(recv.send.sta != 0x00)	err_disp(recv.send.sta, recv.send.sid);
			else						convert (recv.send.avr, recv.send.sid);
																	return 0;
		default	 :													return 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void err_disp(byte sta, byte id)
{
	byte	temp[10], i;

	watch();
	for(i=0; i<7; i++)	temp[i] = BLK;

	if(id == 7)	{
		temp[0] = BLK;		temp[1] = BLK;
		if(sta == 0x08)	{
			// Stand by
			temp[2] = 0x92;		temp[3] = 0x87;		temp[4] = 0x83;
		}	else	{
			// Error
			temp[2] = 0x86;		temp[3] = 0xCF;		temp[4] = 0xCF;
		}
	}	else	{
		temp[0] = BLK;
		// Error
		temp[1] = 0x86;		temp[2] = 0xCF;		temp[3] = 0xCF;
	}
	switch(id)	{
		case	3 :  for(i=0; i<4; i++)	buf0[WD   + i] = temp[i];	break;
		case	4 :  for(i=0; i<4; i++)	buf0[WS   + i] = temp[i];	break;
		case	5 :  for(i=0; i<4; i++)	buf1[TEMP + i] = temp[i];	break;
		case	6 :  for(i=0; i<4; i++)	buf1[HUMI + i] = temp[i];	break;
		case	7 :  for(i=0; i<5; i++)	buf0[PRESS+ i] = temp[i];	break;
		default	  :													return;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void convert(word d_data, byte id)
{
	byte	temp[10], i, flag;
	double	press, result, tmp;

	watch();
	for(i=0; i<7; i++)	temp[i] = BLK;

	watch();
	if(id == 5)	{	// temperature...
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
	}	else if(id == 7)	{		// PRESS
		flag = BLK;
		if(Test_mode == 0x04)	{
			press   = (double)d_data / (double)10.;
			tmp     = press / 1013.25;
			result  = 145454.5 * (1. - pow(tmp, (double)(0.19026)) );
			result *= 0.3048;
			if     (result <= -698.3)		result = -698.3;
			else if(result >= 3012.2)		result = 3012.2;

			if(result < 0.)	{
				flag = MINUS;	d_data = (word)((result * (-1.)) * 10.);
			}	else	{
				flag = BLK  ;   d_data = (word) (result * 10.);
			}
		}
		if((d_data / 10000) == 0)		temp[0] = BLK;
		else							temp[0] = TABLE[(d_data / 10000) % 10];

		if(temp[0] != BLK)          	temp[1] = TABLE[(d_data / 1000 ) % 10];
		else	{
			if((d_data / 1000 ) == 0)	temp[1] = BLK;
			else						temp[1] = TABLE[(d_data / 1000 ) % 10];
		}

		if(temp[1] != BLK)				temp[2] = TABLE[(d_data / 100  ) % 10];
		else	{
			if((d_data / 100  ) == 0)	temp[2] = BLK;
			else						temp[2] = TABLE[(d_data / 100  ) % 10];
		}
		temp[3] = TABLE[(d_data / 10) % 10] & POINT;
		temp[4] = TABLE[ d_data % 10      ];

		if(Test_mode == 0x04)	{
			if     (temp[2] == BLK)			temp[2] = flag;
			else if(temp[1] == BLK)			temp[1] = flag;
			else 							temp[0] = flag;
		}
	}	else				{
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

	watch();
	switch(id)	{
		case	3 :  for(i=0; i<4; i++)	buf0[WD   + i] = temp[i];	break;
		case	4 :  for(i=0; i<4; i++)	buf0[WS   + i] = temp[i];	break;
		case	5 :  for(i=0; i<4; i++)	buf1[TEMP + i] = temp[i];	break;
		case	6 :  for(i=0; i<4; i++)	buf1[HUMI + i] = temp[i];	break;
		case	7 :  for(i=0; i<5; i++)	buf0[PRESS+ i] = temp[i];	break;
		default	  :													return;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
