//[*]----------------------------------------------------------------------[*]
//   Protocol Description.(11 bytes Protocol)
//[*]----------------------------------------------------------------------[*]
//  /---------- Header ---------/ /-CMD-/ /DATA/ /CheckSUM/ /----------- TAIL ----------/
//	0		1		2		3		4		5		6		7		8		9       10
//	0xAA	0xAA	0xAA	0xAA	CMD     KEY		CHK		0x55	0x55	0x55	0x55
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
byte rx_check(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0x55)	return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0x55)	return 0;
	if(var->buf[(var->p_sp + var->size -3) % var->size] != 0x55)	return 0;
	if(var->buf[(var->p_sp + var->size -4) % var->size] != 0x55)	return 0;
	if(var->buf[(var->p_sp             +3) % var->size] != 0xAA)    return 0;
	if(var->buf[(var->p_sp             +2) % var->size] != 0xAA)    return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xAA)	return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xAA)	return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte rx_catch(variable_t *var)
{
	byte	ck_xor = 0, catch_key = 0;
	word	i = 0, d_size = 0;

	watch();
	if(var->buf[(var->p_sp +4) % var->size] != 'M')			return 0;

	ck_xor ^= var->buf[(var->p_sp +4) % var->size];
	ck_xor ^= var->buf[(var->p_sp +5) % var->size];
	if(var->buf[(var->p_sp +6) % var->size] != ck_xor)		return 0;

	if(var->buf[(var->p_sp +5) % var->size] > 6)            return 0;
	catch_key = var->buf[(var->p_sp +5) % var->size];

	if(catch_key == 0)		{	catch_key = 1;				return 0;	}

	ok_send(catch_key);		Led_send = ON;
	return 0;
}
//[*]----------------------------------------------------------------------[*]
void ok_send(byte key)
{
	word	j;

	watch();
	key_send.send.xor = 0;		key_send.send.key = key;
	for(j = 0; j < sizeof(key_send_u); j++)	{
		if(j>3 && j<sizeof(key_send_u) -5)    key_send.send.xor^= key_send.bytes[j];
		data_move(key_send.bytes[j]);
	}	transmit_check();
}
//[*]----------------------------------------------------------------------[*]
