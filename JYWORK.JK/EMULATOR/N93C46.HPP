//[*]----------------------------------------------------------------------[*]
byte config_read(void)
{
	word	i;

	watch();	chk_cfg.rom.xor = 0x00;

	for(i=0; i<sizeof(chk_cfg_u); i++)	{
		if(i>1 && i<sizeof(chk_cfg_u)-3)	chk_cfg.rom.xor ^= get_byte(i);
		chk_cfg.bytes[i] = get_byte(i);		watch();
		output(0x6000+i,get_byte(i));
	}

	if(chk_cfg.rom.xor != get_byte(sizeof(chk_cfg_u)-3))	return FAIL;
	if(chk_cfg.rom.lid != read_jumper())					return FAIL;

	serial_init(chk_cfg.rom.baud);							return NORMAL;
}
//[*]----------------------------------------------------------------------[*]
void factory_setup(void)
{
	watch();
	chk_cfg.rom.header = 0xFFFF;	chk_cfg.rom.cod    = 'M'   ;
	chk_cfg.rom.cmd	   = 'M'   ;	chk_cfg.rom.id 	   = 0x00  ;
	chk_cfg.rom.climt  = 0x00  ;	chk_cfg.rom.dp	   = 0x80  ;
	chk_cfg.rom.did	   = 0x00  ;	chk_cfg.rom.th	   = 0x00  ;
	chk_cfg.rom.tl	   = 0x00  ;	chk_cfg.rom.baud   = 0x03  ;
	chk_cfg.rom.ds	   = 0x00  ;	chk_cfg.rom.nused  = 0x00  ;
	chk_cfg.rom.xor	   = 0x00  ;	chk_cfg.rom.tail   = 0xFFFE;
	chk_cfg.rom.lid	   = read_jumper();
	led_setup(&chk_cfg);
}
//[*]----------------------------------------------------------------------[*]
//   OP CODE WRITE ( SERIAL )
//[*]----------------------------------------------------------------------[*]
void op_write(byte	op_code)
{
	DATA_IN = 1; 		clock_puls();		// start command
	switch(op_code)	{
		case WRITE_OP  	:	DATA_IN = 0;	clock_puls();
							DATA_IN = 1;	clock_puls();		return;
		case READ_OP    :	DATA_IN = 1;	clock_puls();
							DATA_IN = 0;	clock_puls();		return;
		case ERASE_OP   :   DATA_IN = 1;	clock_puls();
							DATA_IN = 1;	clock_puls();		return;
		default			:   DATA_IN = 0;	clock_puls();
							DATA_IN = 0;	clock_puls();		return;
	}
}
//[*]----------------------------------------------------------------------[*]
//   Serial EEPROM ADDRESS SELECT OUTPUT
//[*]----------------------------------------------------------------------[*]
void addr_write(byte addr)
{
	byte	i=0;
	byte	ck_bit = 0x40;

	for(i=0; i<7; i++)		{
		if(addr & ck_bit)				DATA_IN = 1;
		else							DATA_IN = 0;
		ck_bit = ck_bit >>1;			clock_puls();
	}
}
//[*]----------------------------------------------------------------------[*]
//   Serial CLOCK OUTPUT
//[*]----------------------------------------------------------------------[*]
void clock_puls(void)
{
	delay_count(5);		CLK_IN = 1;		delay_count(5);		CLK_IN = 0;
	delay_count(5);		watch();
}
//[*]----------------------------------------------------------------------[*]
void delay_count(byte value)
{
	word 	i=0;
	for(i=0; i<value; i++)		{	watch();	i++;	i--;	}
}
//[*]----------------------------------------------------------------------[*]
//   Read Data Function
//[*]----------------------------------------------------------------------[*]
byte read_mode(byte rd_addr, byte op_code)
{
	byte	ret_data = 0;
	byte	i = 0, ck_bit = 0x80;

	CHIP_CS = 1;        	delay_count(5);
	op_write(op_code);  	addr_write(rd_addr);

	for(i=0; i<8; i++)	{
		clock_puls();
		if(DATA_OUT)		ret_data |=   ck_bit;
		else				ret_data &= (~ck_bit);
		ck_bit = ck_bit >>1;
	}   CHIP_CS = 0;		delay_count(5);

	return	ret_data;
}
//[*]----------------------------------------------------------------------[*]
//   Write Data Function
//[*]----------------------------------------------------------------------[*]
void write_mode(byte cmd, byte wr_addr, byte wr_data, byte op_code)
{
	byte i=0, ck_bit = 0x80;

	CHIP_CS = 1;		 	delay_count(5);
	op_write(op_code);  	addr_write(wr_addr);

	if(cmd == ERASE_ALL_CMD || cmd == ENABLE_CMD ||
	   cmd == DISABLE_CMD   || cmd == ERASE_CMD   )			{
		CHIP_CS = 0;	DATA_IN = 0;	delay_count(5);	return;
	}

	for(i=0; i<8; i++)	{
		if(wr_data & ck_bit)			DATA_IN = 1;
		else							DATA_IN = 0;
		ck_bit = ck_bit >>1;			clock_puls();
	}
	CHIP_CS = 0;  		      			DATA_IN = 0;

	//  BUSY / RAEDY?? Check
	delay_count(5);					CHIP_CS  = 1;
	delay_count(5);					DATA_OUT = 1;
	delay_count(5);					while(!DATA_OUT);
	CHIP_CS = 0;                    delay_count(5);
}
//[*]----------------------------------------------------------------------[*]
byte get_byte(word addr)
{
	watch();	return	  (byte)(read_mode(addr, READ_OP));
}
//[*]----------------------------------------------------------------------[*]
void set_byte(byte addr, byte wdata)
{
	watch();	write_mode(WRITE_CMD, addr, wdata, WRITE_OP);
}
//[*]----------------------------------------------------------------------[*]
void led_setup(chk_cfg_u *p)
{
	word	i;

	watch();
//	write_mode(ENABLE_CMD   , EWEN_ADDR, 0x0, EWEN_OP);
//	write_mode(ERASE_ALL_CMD, ERAL_ADDR, 0x0, ERAL_OP);

	for(i=0; i<sizeof(chk_cfg_u); i++)	{
		if(i>1 && i<sizeof(chk_cfg_u)-3)	p->rom.xor ^= p->bytes[i];
		set_byte(i,p->bytes[i]);			watch();
	}
		while(1)	watch();
//	write_mode(DISABLE_CMD  , EWDS_ADDR, 0x0, EWDS_OP);		while(1);
}
//[*]----------------------------------------------------------------------[*]





