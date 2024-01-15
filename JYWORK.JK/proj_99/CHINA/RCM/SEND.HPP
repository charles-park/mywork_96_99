//[*]----------------------------------------------------------------------[*]
//   SEND.HPP File (DATA Struct Send Function)
//[*]----------------------------------------------------------------------[*]
//   Set Command ACK
//[*]----------------------------------------------------------------------[*]
void send_to_host_ack(protocol_grp_t *com, byte gubun)
{
	word	i;

	rcm_to_host_ack.send.header = 0xFFFF ;
	rcm_to_host_ack.send.logid  = LOG_ID ;
	rcm_to_host_ack.send.cmd    = gubun  ;
	rcm_to_host_ack.send.year   = t.year ;
	rcm_to_host_ack.send.month  = t.month;
	rcm_to_host_ack.send.day    = t.day  ;
	rcm_to_host_ack.send.hour   = t.hour ;
	rcm_to_host_ack.send.min    = t.min  ;
	rcm_to_host_ack.send.xor    = 0      ;
	rcm_to_host_ack.send.add    = 0      ;
	rcm_to_host_ack.send.tail   = 0xFFFE ;

	strcpy(rcm_to_host_ack.send.ack, "OKAY");

	for(i=0; i<sizeof(rcm_to_host_ack_u); i++)	{
		if(i>1 && i<sizeof(rcm_to_host_ack_u) -4)	{
			rcm_to_host_ack.send.xor ^= rcm_to_host_ack.bytes[i];
			rcm_to_host_ack.send.add += rcm_to_host_ack.bytes[i];
		}
		com->tx_buf[com->tx_ep++] = rcm_to_host_ack.bytes[i];
		if(com->tx_ep >= BUFFER_SIZE)	com->tx_ep = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
//   RCM Status & Detail Voltage Send to MPM
//[*]----------------------------------------------------------------------[*]
void send_to_mpm(protocol_grp_t *com)
{
	byte i;

	mpm.send.header = 0xFFFF;
	mpm.send.cmd    =    'L';
	mpm.send.xor    =      0;
	mpm.send.tail   = 0xFFFE;
	for(i=0; i<sizeof(rcm_to_mpm_u); i++)	{
		if(i>1 && i<sizeof(rcm_to_mpm_u)-3)		mpm.send.xor ^= mpm.bytes[i];
		com->tx_buf[com->tx_ep++] = mpm.bytes[i];
		if(com->tx_ep >= BUFFER_SIZE)	com->tx_ep = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
//   RCM Status Send to HOST
//[*]----------------------------------------------------------------------[*]
void send_to_host_status(protocol_grp_t *com)
{
	word	i;

	host_status.send.header = 0xFFFF ;	host_status.send.logid  = LOG_ID ;
	host_status.send.cmd    = 'S'    ;	host_status.send.year   = t.year ;
	host_status.send.month  = t.month;	host_status.send.day    = t.day  ;
	host_status.send.hour   = t.hour ;	host_status.send.min	= t.min  ;
	host_status.send.sec	= t.sec  ;	host_status.send.xor    = 0      ;
	host_status.send.add    = 0      ;	host_status.send.tail   = 0xFFFE ;

	volts_status_check(&host_status.send.sta.volt.bits);

	for(i=0; i<4; i++)	{
		host_status.send.sta.board[i]  = mpm.send.sta.board[i];
		host_status.send.sta.sensor[i] = mpm.send.sta.sensor[i];
	}
	host_status.send.sta.cnt_board.byte_data = mpm.send.sta.cnt_board.byte_data;

	for(i=0; i<sizeof(status_send_to_host_u); i++)	{
		if(i > 1 && i < sizeof(status_send_to_host_u)-4)	{
			host_status.send.xor ^= host_status.bytes[i];
			host_status.send.add += host_status.bytes[i];
		}
		com->tx_buf[com->tx_ep++] = host_status.bytes[i];
		if(com->tx_ep >= BUFFER_SIZE)	com->tx_ep = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
//   RCM Detail Voltage Send to HOST
//[*]----------------------------------------------------------------------[*]
void send_to_host_detail(protocol_grp_t *com)
{
	word	i;
	host_detail.send.header = 0xFFFF ;	host_detail.send.logid  = LOG_ID ;
	host_detail.send.cmd    = 'P'    ;	host_detail.send.year   = t.year ;
	host_detail.send.month  = t.month;	host_detail.send.day    = t.day  ;
	host_detail.send.hour   = t.hour ;	host_detail.send.min    = t.min  ;
	host_detail.send.sec    = t.sec  ;	host_detail.send.xor    = 0      ;
	host_detail.send.add    = 0      ;	host_detail.send.tail   = 0xFFFE ;

	host_detail.send.volt.pv5_1   = mpm.send.volt.pv5_1  ;
	host_detail.send.volt.pv5_2   = mpm.send.volt.pv5_2  ;
	host_detail.send.volt.pv12_1  = mpm.send.volt.pv12_1 ;
	host_detail.send.volt.pv12_2  = mpm.send.volt.pv12_2 ;
	host_detail.send.volt.mv12_1  = mpm.send.volt.mv12_1 ;
	host_detail.send.volt.mv12_2  = mpm.send.volt.mv12_2 ;
	host_detail.send.volt.battery = mpm.send.volt.battery;
	host_detail.send.volt.ac_volt = mpm.send.volt.ac_volt;
	host_detail.send.volt.fan     = mpm.send.volt.fan    ;
	host_detail.send.volt.door    = mpm.send.volt.door   ;
	host_detail.send.volt.wd      = mpm.send.volt.wd     ;
	host_detail.send.volt.ws      = mpm.send.volt.ws     ;
	host_detail.send.volt.ped     = mpm.send.volt.ped    ;
	host_detail.send.volt.prs     = mpm.send.volt.prs    ;
	host_detail.send.volt.rh      = mpm.send.volt.rh     ;
	host_detail.send.volt.tsp     = mpm.send.volt.tsp    ;

	for(i=0; i<sizeof(detail_send_to_host_u); i++)	{
		if(i>1 && i<sizeof(detail_send_to_host_u)-4)	{
			host_detail.send.xor ^= host_detail.bytes[i];
			host_detail.send.add += host_detail.bytes[i];
		}
		com->tx_buf[com->tx_ep++] = host_detail.bytes[i];
		if(com->tx_ep >= BUFFER_SIZE)	com->tx_ep = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
