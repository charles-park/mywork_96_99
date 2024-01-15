//[*]----------------------------------------------------------------------[*]
void int_send_ack(protocol_grp_t *comx, byte cmd, word d_size)
{
	word	i;

	watch();
	for(i=0; i<4; i++)	{
		int_ack.send.header[i] = 0xFF;	int_ack.send.tail  [i] = 0xFE;
	}
	int_ack.send.bdid      = RCM_ID    ;	int_ack.send.sen_id    = 0xFA;
	int_ack.send.size      = d_size    ;	int_ack.send.cmd       = cmd ;
	int_ack.send.xor       = 0         ;	int_ack.send.add       = 0   ;

	for(i = 0; i<sizeof(int_ack_send_u); i++)	{
		if(i>3 && i<sizeof(int_ack_send_u)-6)	{
			int_ack.send.xor ^= int_ack.bytes[i];
			int_ack.send.add += int_ack.bytes[i];
		}	move_to_serial_buffer(comx, int_ack.bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void ext_send_ack(protocol_grp_t *comx, byte cmd, word d_size, time_t *s_time)
{
	word	i;

	watch();
	for(i=0; i<4; i++)	{
		ext_ack.send.header[i] = 0xFF;	ext_ack.send.tail  [i] = 0xFE;
	}
	ext_ack.send.rtuid     = Rtuid     ;	ext_ack.send.jijum     = Jijum     ;
	ext_ack.send.bdid      = RCM_ID    ;	ext_ack.send.size      = d_size    ;
	ext_ack.send.cmd[0]    = 0xFF      ;	ext_ack.send.cmd[1]    = cmd       ;
	ext_ack.send.s_time.yy = s_time->yy;	ext_ack.send.s_time.mm = s_time->mm;
	ext_ack.send.s_time.dd = s_time->dd;	ext_ack.send.s_time.hh = s_time->hh;
	ext_ack.send.s_time.bb = s_time->bb;	ext_ack.send.s_time.ss = s_time->ss;
	ext_ack.send.xor       = 0         ;	ext_ack.send.add       = 0         ;

	for(i = 0; i<sizeof(ext_ack_send_u); i++)	{
		if(i>3 && i<sizeof(ext_ack_send_u)-6)	{
			ext_ack.send.xor ^= ext_ack.bytes[i];
			ext_ack.send.add += ext_ack.bytes[i];
		}	move_to_serial_buffer(comx, ext_ack.bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_volts(protocol_grp_t *comx, byte cmd, time_t *s_time)
{
	word	i;

	watch();
	for(i=0; i<4; i++)	{
		volts.send.header[i] = 0xFF;	volts.send.tail  [i] = 0xFE;
	}
	volts.send.rtuid     = Rtuid     ;	volts.send.jijum     = Jijum     ;
	volts.send.bdid      = RCM_ID    ;
	volts.send.cmd[0]    = 0xFF      ;	volts.send.cmd[1]    = cmd       ;
	volts.send.s_time.yy = s_time->yy;	volts.send.s_time.mm = s_time->mm;
	volts.send.s_time.dd = s_time->dd;	volts.send.s_time.hh = s_time->hh;
	volts.send.s_time.bb = s_time->bb;	volts.send.s_time.ss = s_time->ss;
	volts.send.xor       = 0         ;	volts.send.add       = 0         ;

	// Remark 1998, 5, 15 (Fixd Voltage Display)
//	volts.send.p12 = volts_data.p12  ;	volts.send.p5  = volts_data.p5   ;
//	volts.send.m12 = volts_data.m12  ;
	volts.send.p12 = +15.23 ;			volts.send.p5  = +5.14  ;
	volts.send.m12 = -15.12 ;

	for(i = 0; i<sizeof(volts_send_u); i++)	{
		if(i>3 && i<sizeof(volts_send_u)-6)	{
			volts.send.xor ^= volts.bytes[i];
			volts.send.add += volts.bytes[i];
		}	move_to_serial_buffer(comx, volts.bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_mpm(protocol_grp_t *comx, byte cmd, time_t *s_time)
{
	word	i;

	watch();
	for(i=0; i<4; i++)	{
		mpm.send.header[i] = 0xFF;	mpm.send.tail  [i] = 0xFE;
	}
	mpm.send.rtuid     = Rtuid     ;	mpm.send.jijum     = Jijum     ;
	mpm.send.bdid      = RCM_ID    ;
	mpm.send.cmd[0]    = 0xFF      ;	mpm.send.cmd[1]    = cmd       ;
	mpm.send.s_time.yy = s_time->yy;	mpm.send.s_time.mm = s_time->mm;
	mpm.send.s_time.dd = s_time->dd;	mpm.send.s_time.hh = s_time->hh;
	mpm.send.s_time.bb = s_time->bb;	mpm.send.s_time.ss = s_time->ss;
	mpm.send.xor       = 0         ;	mpm.send.add       = 0         ;

	// Remark 1998, 5, 15 (Fixd Voltage Display)
//	mpm.send.p12 = volts_data.p12  ;	mpm.send.p5  = volts_data.p5   ;
//	mpm.send.m12 = volts_data.m12  ;
	mpm.send.p12 = +15.23;				mpm.send.p5  =  +5.14;
	mpm.send.m12 = -15.12;

	for(i = 0; i<sizeof(mpm_send_u); i++)	{
		if(i>3 && i<sizeof(mpm_send_u)-6)	{
			mpm.send.xor ^= mpm.bytes[i];
			mpm.send.add += mpm.bytes[i];
		}	move_to_serial_buffer(comx, mpm.bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_status(protocol_grp_t *comx, byte cmd)
{
	word	i;

	watch();
	for(i=0; i<4; i++)	{
		status.send.header[i] = 0xFF;	status.send.tail  [i] = 0xFE;
	}
	status.send.rtuid     = Rtuid     ;	status.send.jijum     = Jijum     ;
	status.send.bdid      = RCM_ID    ;
	status.send.cmd[0]    = 0xFF      ; status.send.cmd[1]    = cmd       ;
	status.send.xor       = 0         ;	status.send.add       = 0         ;

	status.send.p12 = volts_data.p12_sta;
	status.send.p5  = volts_data.p5_sta ;
	status.send.m12 = volts_data.m12_sta;

	for(i = 0; i<sizeof(status_u); i++)	{
		if(i>3 && i<sizeof(status_u)-6)	{
			status.send.xor ^= status.bytes[i];
			status.send.add += status.bytes[i];
		}	move_to_serial_buffer(comx, status.bytes[i]);
	}	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void send_event(protocol_grp_t *comx, byte event_num)
{
	word	i;
	byte	*s_data = (byte *)(&event.num[event_num]);
	for(i=0; i<sizeof(status_u); i++)	move_to_serial_buffer(comx, s_data[i]);
	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
//   A/D Board Debug Data Send Program..
//[*]----------------------------------------------------------------------[*]
void debug_data_send(protocol_grp_t *comx, variable_t *var)
{
	word	i;

	for(i=0; i<4; i++)	{
		debug.send.header[i] = 0xFF;	debug.send.tail[i] = 0xFF;
	}	debug.send.xor = 0;				debug.send.add = 0;
	for(i=0; i<40; i++)	{
		debug.send.f_data[i] = var->buf[(var->p_sp + i + 4) % var->size];
		debug.send.xor      ^= var->buf[(var->p_sp + i + 4) % var->size];
		debug.send.add      += var->buf[(var->p_sp + i + 4) % var->size];
	}
	for(i=0; i<sizeof(debug_u); i++)	move_to_serial_buffer(comx, debug.bytes[i]);
	comx->cat_flag = OFF;
}
//[*]----------------------------------------------------------------------[*]
void move_to_serial_buffer(protocol_grp_t *comx, byte s_data)
{
	watch();
	comx->tx_buf[comx->tx_ep++] = s_data;
	if(comx->tx_ep >= BUFFER_SIZE)	comx->tx_ep = 0;
}
//[*]----------------------------------------------------------------------[*]
