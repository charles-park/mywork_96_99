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
void addr_write(byte addr)
{
	byte	i=0;
	byte	ck_bit = 0x20;

	for(i=0; i<6; i++)		{
		if(addr & ck_bit)				DATA_IN = 1;
		else							DATA_IN = 0;
		ck_bit = ck_bit >>1;			clock_puls();
	}
}
//[*]----------------------------------------------------------------------[*]
void clock_puls(void)
{
	delay_count(10);		CLK_IN = 1;
	delay_count(10);		CLK_IN = 0;
	delay_count(10);
}
//[*]----------------------------------------------------------------------[*]
void delay_count(word value)
{
	word 	i=0;
	for(i=0; i<value; i++)		{	i++;	i--;	}
}
//[*]----------------------------------------------------------------------[*]
word read_mode(byte rd_addr, byte op_code)
{
	word	ret_data = 0;
	word	i = 0, ck_bit = 0x8000;

	CHIP_CS = 1;        	delay_count(10);
	op_write(op_code);  	addr_write(rd_addr);

	for(i=0; i<16; i++)	{
		clock_puls();
		if(DATA_OUT)		ret_data |=   ck_bit;
		else				ret_data &= (~ck_bit);
		ck_bit = ck_bit >>1;
	}   CHIP_CS = 0;		delay_count(10);

	return	ret_data;
}
//[*]----------------------------------------------------------------------[*]
void write_mode(byte cmd, byte wr_addr, word wr_data, byte op_code)
{
	word i=0, ck_bit = 0x8000;

	CHIP_CS = 1;		 	delay_count(10);
	op_write(op_code);  	addr_write(wr_addr);

	if(cmd == ERASE_ALL_CMD || cmd == ENABLE_CMD ||
	   cmd == DISABLE_CMD   || cmd == ERASE_CMD   )			{
		CHIP_CS = 0;	DATA_IN = 0;	delay_count(100);	return;
	}

	for(i=0; i<16; i++)	{
		if(wr_data & ck_bit)			DATA_IN = 1;
		else							DATA_IN = 0;
		ck_bit = ck_bit >>1;			clock_puls();
	}
	CHIP_CS = 0;  		      			DATA_IN = 0;

	//  BUSY / RAEDY?? Check
	delay_count(10);					CHIP_CS = 1;
	delay_count(10);					DATA_OUT = 1;
	delay_count(10);					while(!DATA_OUT);
	CHIP_CS = 0;                        delay_count(10);
}
//[*]----------------------------------------------------------------------[*]
