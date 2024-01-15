//[*]----------------------------------------------------------------------[*]
void send_ack(protocol_grp_t *comx, byte cmd, word d_size, byte sen_id)
{
	word	i;

	watch();
	ack.send.header = 0xFFFFFFFF;	ack.send.tail = 0xFEFEFEFE;
	ack.send.bdid   = 0x00    ;		ack.send.sen_id = sen_id;
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
void real_send(protocol_grp_t *comx, byte ch_num)
{
	word	i;
	byte	*s_data = (byte *)(&send_ch[ch_num].real);

	send_ch[ch_num].real.status |= Board_status;
	send_ch[ch_num].real.xor = 0;	send_ch[ch_num].real.add = 0;

	for(i=0; i<sizeof(sensor_field_t); i++)	{
		if(i>3 && i<sizeof(sensor_field_t) -6)	{
			send_ch[ch_num].real.xor ^= s_data[i];
			send_ch[ch_num].real.add += s_data[i];
		}   move_to_serial_buffer(comx, s_data[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void jugi_send(protocol_grp_t *comx, byte ch_num)
{
	word	i;
	byte	*s_data = (byte *)(&send_ch[ch_num].jugi);

	send_ch[ch_num].jugi.status |= Board_status;
	send_ch[ch_num].jugi.xor = 0;	send_ch[ch_num].jugi.add = 0;

	for(i=0; i<sizeof(sensor_field_t); i++)	{
		if(i>3 && i<sizeof(sensor_field_t) -6)	{
			send_ch[ch_num].jugi.xor ^= s_data[i];
			send_ch[ch_num].jugi.add += s_data[i];
		}   move_to_serial_buffer(comx, s_data[i]);
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
