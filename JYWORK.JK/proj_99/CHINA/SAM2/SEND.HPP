//[*]----------------------------------------------------------------------[*]
//   SEND.HPP File (DATA Struct Send Function)
//[*]----------------------------------------------------------------------[*]
void data_send(sensor_u *p, byte call_id, byte cmd)
{
	word	i;

	p->sam.header = 0xFFFF ;	p->sam.tail   = 0xFFFE ;
	p->sam.sid    = call_id;	p->sam.cmd    = cmd    ;
	p->sam.xor    = 0x00   ;

	if     (call_id == Sam_id  )	p->sam.status |= status_temp0.byte_data;
	else if(call_id == Sam_id+1)	p->sam.status |= status_temp1.byte_data;
	else if(call_id == Sam_id+2)	p->sam.status |= status_temp2.byte_data;
	else if(call_id == Sam_id+3)	p->sam.status |= status_prec.byte_data;
	else							return;

	for(i=0; i<sizeof(sensor_u); i++)	{
		if(i>1 && i<sizeof(sensor_u) -3)	p->sam.xor ^= p->bytes[i];
		com.tx_buf[com.tx_ep++] = p->bytes[i];
		if(com.tx_ep >= BUFFER_SIZE)		com.tx_ep = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
void init_send(byte call_id)
{
	word	i;

	data_call.sam.header = 0xFFFF  ;	data_call.sam.tail   = 0xFFFE  ;
	data_call.sam.cmd    = '$'     ;	data_call.sam.status = 0       ;
	data_call.sam.sid    = call_id ;	data_call.sam.avr    = 0	   ;
	data_call.sam.spc    = 0	   ;	data_call.sam.min    = 0	   ;
	data_call.sam.max    = 0	   ;	data_call.sam.xor    = 0	   ;

	for(i=0; i<sizeof(sensor_u); i++)	{
		if(i>1 && i<sizeof(sensor_u) -3)	data_call.sam.xor ^= data_call.bytes[i];
		com.tx_buf[com.tx_ep++] = data_call.bytes[i];
		if(com.tx_ep >= BUFFER_SIZE)		com.tx_ep = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]


