//[*]----------------------------------------------------------------------[*]
void send_ack(protocol_grp_t *comx, byte cmd, word d_size, byte sen_id)
{
	word	i;

	watch();
	ack.send.header = 0xFFFFFFFF;	ack.send.tail   = 0xFEFEFEFE;
	ack.send.bdid   = Board_id  ;	ack.send.sen_id = sen_id    ;
	ack.send.size   = d_size    ;	ack.send.cmd    = cmd       ;
	ack.send.xor    = 0         ;	ack.send.add    = 0         ;

	for(i = 0; i<sizeof(ack_send_u); i++)	{
		if(i>3 && i<sizeof(ack_send_u)-6)	{
			ack.send.xor ^= ack.bytes[i];
			ack.send.add += ack.bytes[i];
		}	move_to_serial_buffer(comx, ack.bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_real(protocol_grp_t *comx, word jugi, sensor_field_u *p)
{
	word	i;

	watch();
	p->snow.gubun = jugi;	p->snow.xor = 0;		p->snow.add = 0;
	p->snow.status.byte_data = status.byte_data;

	for(i=0; i<sizeof(sensor_field_u); i++)	{
		if(i>3 && i<sizeof(sensor_field_u) -6)	{
			p->snow.xor ^= p->bytes[i];		p->snow.add += p->bytes[i];
		}	move_to_serial_buffer(comx, p->bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_jugi(protocol_grp_t *comx, word jugi, sensor_field_u *p)
{
	word	i;

	watch();
	p->snow.gubun = jugi;	p->snow.xor = 0;		p->snow.add = 0;
	p->snow.status.byte_data = status.byte_data;

	for(i=0; i<sizeof(sensor_field_u); i++)	{
		if(i>3 && i<sizeof(sensor_field_u) -6)	{
			p->snow.xor ^= p->bytes[i];		p->snow.add += p->bytes[i];
		}	move_to_serial_buffer(comx, p->bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_offset(protocol_grp_t *comx, float offs, sensor_field_u *p)
{
	word	i;

	watch();
	p->snow.avr = p->snow.max = p->snow.min = offs;
	p->snow.xor = 0;	p->snow.add = 0;

	for(i=0; i<sizeof(sensor_field_u); i++)	{
		if(i>3 && i<sizeof(sensor_field_u) -6)	{
			p->snow.xor ^= p->bytes[i];		p->snow.add += p->bytes[i];
		}	move_to_serial_buffer(comx, p->bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_maxmin_call(protocol_grp_t *comx, sensor_field_u *p)
{
	word	i;

	watch();
	p->snow.xor = 0;	p->snow.add = 0;
	for(i=0; i<sizeof(sensor_field_u); i++)	{
		if(i>3 && i<sizeof(sensor_field_u) -6)	{
			p->snow.xor ^= p->bytes[i];		p->snow.add += p->bytes[i];
		}	move_to_serial_buffer(comx, p->bytes[i]);
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
//[*]----------------------------------------------------------------------[*]
