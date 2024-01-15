//[*]----------------------------------------------------------------------[*]
void send_ack(protocol_grp_t *comx, byte cmd, word d_size, byte sen_id)
{
	word	i;

	watch();
	for(i=0; i<4; i++)	{
		ack.send.header[i] = 0xFF;	ack.send.tail  [i] = 0xFE;
	}
	ack.send.bdid   = Board_id;		ack.send.sen_id = sen_id;
	ack.send.size   = d_size  ;		ack.send.cmd    = cmd   ;
	ack.send.xor    = 0       ;		ack.send.add    = 0     ;

	for(i = 0; i<sizeof(ack_send_u); i++)	{
		if(i>3 && i<sizeof(ack_send_u)-6)	{
			ack.send.xor ^= ack.bytes[i];
			ack.send.add += ack.bytes[i];
		}	move_to_serial_buffer(comx, ack.bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_mpm(protocol_grp_t *comx, word jugi, ch_info_t *send, byte number)
{
	word	i;
	byte	*s_data = (byte *)(&send->sensor);

	watch();
	for(i=0; i<4; i++)	{
		send->sensor.header[i] = 0xFF;	send->sensor.tail  [i] = 0xFE;
	}
	send->sensor.call_cmd = 'W';
	send->sensor.gubun[0] = (byte)(jugi >> 8);
	send->sensor.gubun[1] = (byte)(jugi     );
	send->sensor.xor = 0;			send->sensor.add = 0;

	switch(number)	{
		case	0:  send->sensor.avr = (float)(send->var.real_sum );
					send->sensor.spc = (float)(send->var.real_spc );
					send->sensor.min = (float)(send->var.real_min );
					send->sensor.max = (float)(send->var.real_max );	break ;
		case	1:	send->sensor.avr = (float)(send->var.jugi1_sum);
					send->sensor.spc = (float)(send->var.jugi1_spc);
					send->sensor.min = (float)(send->var.jugi1_min);
					send->sensor.max = (float)(send->var.jugi1_max);	break ;
		case	2:  send->sensor.avr = (float)(send->var.jugi2_sum);
					send->sensor.spc = (float)(send->var.jugi2_spc);
					send->sensor.min = (float)(send->var.jugi2_min);
					send->sensor.max = (float)(send->var.jugi2_max);	break ;
		default  :                                               		return;
	}
	for(i=0; i<sizeof(sensor_field_t); i++)	{
		if(i>3 && i<sizeof(sensor_field_t) -6)	{
			send->sensor.xor ^= s_data[i];		send->sensor.add += s_data[i];
		}
		move_to_serial_buffer(comx, s_data[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_maxmin_call(protocol_grp_t *comx, ch_info_t *send)
{
	word	i;
	byte	*s_data = (byte *)(&send->sensor);

	watch();
	for(i=0; i<4; i++)	{
		send->sensor.header[i] = 0xFF;	send->sensor.tail  [i] = 0xFE;
	}
	send->sensor.call_cmd = '$';
	send->sensor.gubun[0] = 0x00;
	send->sensor.gubun[1] = 0x00;
	send->sensor.xor = 0;			send->sensor.add = 0;

	for(i=0; i<sizeof(sensor_field_t); i++)	{
		if(i>3 && i<sizeof(sensor_field_t) -6)	{
			send->sensor.xor ^= s_data[i];		send->sensor.add += s_data[i];
		}
		move_to_serial_buffer(comx, s_data[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void move_to_serial_buffer(protocol_grp_t *comx, byte s_data)
{
	watch();
	comx->tx_buf[comx->tx_ep++] = s_data;
	if(comx->tx_ep >= BUFFER_SIZE)	comx->tx_ep = 0;
}
//[*]----------------------------------------------------------------------[*]
//   test send program
//[*]----------------------------------------------------------------------[*]
void test_send(void)
{
	word	i;
	watch();
	for(i=0; i<sizeof(test_send_t); i++)
		move_to_serial_buffer(&com1, test.bytes[i]);
	com1.cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
