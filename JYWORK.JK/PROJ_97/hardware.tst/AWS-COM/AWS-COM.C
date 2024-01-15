//[*]----------------------------------------------------------------------[*]
#include 	"AWS-COM.h"
//[*]----------------------------------------------------------------------[*]
//[*]------ Start Program -------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
signed int Point(signed int Hear, signed int Step)    // Q Buffer Point Set
{
	Hear += Step;
	if(Hear < 0)  	Hear += SIZE;
	return	Hear;
}
//[*]----------------------------------------------------------------------[*]
void interrupt SERIAL(void)
{
	disable();
	Com.RxBuf[Com.RxEp++] = inportb(RBR);
	if(Com.RxEp >= SIZE)	Com.RxEp = 0;
	outportb(PICEOI,0x20);
	enable();
}
//[*]----------------------------------------------------------------------[*]
Byte CominData(void)
{
	return	Com.RxBuf[Point(Com.RxSp,+1)];
}
//[*]----------------------------------------------------------------------[*]
int CominREADY(void)
{
	return	((Com.RxSp == Com.RxEp) ? 0:1);
}
//[*]----------------------------------------------------------------------[*]
Byte CominCH(void)
{
	while(!CominREADY());
	return	CominData();
}
//[*]----------------------------------------------------------------------[*]
void ComoutCH(Byte ch)
{
	while((inportb(LSR) & 0x20) == 0x00)
	{
	}
	outportb(THR,ch);
	printf("%02X",ch);
}
//[*]----------------------------------------------------------------------[*]
void ComoutStr(void)
{
	if(LineCount++ > 24)	{	clrscr();	LineCount = 0;	}
	printf("SEND DATA(%c): ",send.log.cmd);
	printf("Call Time->%02d:%02d:%02d ", host_time.ti_hour, host_time.ti_min, host_time.ti_sec);
	printf("%02d/%02d/%02d ", host_date.da_year-1900, host_date.da_mon, host_date.da_day);
	while(Com.TxSp != Com.TxEp)		ComoutCH(Com.TxBuf[Com.TxSp++]);
	printf("\n");
}
//[*]----------------------------------------------------------------------[*]
void init_Serial(Word MODE)
{
	switch(Com.Port)
	{
		case	COM1:	Com.Base = C1BASE;	break;
		case	COM2:	Com.Base = C2BASE;	break;
		case	COM3:   Com.Base = C3BASE;	break;
		case	COM4:   Com.Base = C4BASE;	break;
	}
	if(MODE == START)	{
		outportb(LCR,inportb(LCR) | 0x80);
		outportb(DLL,B2400);		// Baudrate Setting
		outportb(DLM,0);
		outportb(LCR,inportb(LCR) & 0x7f);

		switch(Com.Port)
		{
			case COM1:	case COM3:
				old_vect = getvect(IRQ4);	setvect(IRQ4,SERIAL);
				outportb( PICMSK, inportb(PICMSK) & I4ENA );
				break;
			case COM2:	case COM4:
				old_vect = getvect(IRQ3);	setvect(IRQ3,SERIAL);
				outportb( PICMSK, inportb(PICMSK) & I3ENA );
				break;
		}
		outportb(IER,0x01);
		outportb(MCR,0x0b);
	}	else	{
		outportb(MCR,0x0B);
		outportb(IER,0x00);
		switch(Com.Port)
		{
			case COM1:	case COM3:
				outportb(PICMSK,inportb(PICMSK) | I4DIS);
				setvect(IRQ4,SERIAL);
				break;
			case COM2:	case COM4:
				outportb(PICMSK,inportb(PICMSK) | I3DIS);
				setvect(IRQ3,SERIAL);
				break;
		}
	}
}
//[*]----------------------------------------------------------------------[*]
//          0		 1		   2		 3		   4		 5		   6		 7		   8
//          01234567890123456789012345678901234567890123456789012345678901234567890123456789
//Ú Ä Â Ä ¿ ³ Ã Å ´ À Á Ù
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
void print_to_file(unsigned char mode)
{
	FILE *fp;

	unsigned char	file_name[12],i;

	getdate(&host_date);
	sprintf(file_name,"%04d%02d%02d.log", host_date.da_year, host_date.da_mon, host_date.da_day);

	if((fp = fopen(file_name,"rt")) == NULL)	{
		fclose(fp);  		fp = fopen(file_name,"a+t");
		call_count = 0;
		fprintf(fp,"ÚÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n");
		fprintf(fp,"³CALL CMD³CALL COUNT³   DATE   ³  TIME  ³     Transmit DATA (To SAM BOARD)     ³\n");
	}
	else if(mode == END && fp != NULL)	{
		fp = fopen(file_name,"a+t");
		fprintf(fp,"ÀÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n");
		fclose(fp);		return;
	}
	else	{
		fclose(fp);  		fp = fopen(file_name,"a+t");
	}

	call_count++;
	if      (mode == SEND)	{
		fprintf(fp,"ÃÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n");
		fprintf(fp,"³   %c    ³      %4d³%4d/%2d/%2d³%02d:%02d:%02d³ ",
				send.log.cmd, call_count, host_date.da_year, host_date.da_mon, host_date.da_day,
				send.log.hour, send.log.min, send.log.sec);
		for(i=0;i<sizeof(send_log_u);i++)	fprintf(fp,"%02X",send.bytes[i]);

		fprintf(fp,"   ³\n");
		fclose(fp);
	}
/*	else if (mode == RECV)	{
		fprintf(fp,"%c",
		fclose(fp);
	}*/
	else	fclose(fp);
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
void real_call(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'I';
	send.log.year	= (unsigned char)host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void min1_call(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'B';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void old_call(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'A';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void romver_call(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'V';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void volt_detail_call(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'P';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void logger_reset(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'R';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void logger_id_set(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'D';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void logger_pass_set(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'W';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void logger_date_time_set(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'T';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void buffer_clear(void)
{
	unsigned int	i;

	send.log.header = 0xFFFF;	send.log.tail   = 0xFEFF;
	send.log.log_id = 0;     	send.log.pass   = 0;
	send.log.xor    = 0;		send.log.add	= 0;
	send.log.cmd	= 'C';
	send.log.year	= host_date.da_year;
	send.log.month	= host_date.da_mon ;
	send.log.day	= host_date.da_day ;
	send.log.hour	= host_time.ti_hour;
	send.log.min	= host_time.ti_min ;
	send.log.sec	= host_time.ti_sec ;

	for(i=0; i<sizeof(send_log_t); i++)	{
		if(i>1 && i<sizeof(send_log_t) -4)	{
			send.log.xor ^= send.bytes[i];
			send.log.add += send.bytes[i];
		}
		Com.TxBuf[Com.TxEp++] = send.bytes[i];
	}
	ComoutStr();
}
//[*]----------------------------------------------------------------------[*]
void main(int argc,char **argv)
{
	int ch;
	unsigned char	flag = 0;
	if (argc != 2)
	{
		puts("Using COMM com_port[1..4]");
		exit(0);
	}
	else	Com.Port = argv[1][0] - '0';

	if (Com.Port < 1 || Com.Port > 4)
	{
		puts("Using COMM com_port[1..4]");
		exit(0);
	}
	clrscr();
	init_Serial(START);

	while(1)
	{
		if (kbhit())
		{
			ch = getch();
			if (ch == 0x1b)		break;

			gettime(&host_time);			getdate(&host_date);

			switch(ch)	{
				case	'i':	case	'I':	real_call();			break;
				case	'b':    case	'B':	min1_call();			break;
				case	'a':    case	'A':    old_call();				break;
				case	'v':    case	'V':	romver_call();			break;
				case	'p':	case	'P':	volt_detail_call();		break;
				case	'r':	case	'R':	logger_reset();			break;
				case	'd':	case	'D':	logger_id_set();		break;
				case	'w':	case	'W':	logger_pass_set();		break;
				case	't':	case	'T':	logger_date_time_set();	break;
				case	'c':	case	'C':	buffer_clear();			break;
			}
			print_to_file(SEND);
		}

		gettime(&host_time);				getdate(&host_date);

		if((host_time.ti_sec%10) == 0)		{
			real_call();	print_to_file(SEND);
			while(1)	{
				gettime(&host_time);
				if((host_time.ti_sec%10) != 0)		break;
			}
		}

		if(host_time.ti_hour == 23 && host_time.ti_min == 59 && host_time.ti_sec == 59)
		{
			print_to_file(END);
			while(1)	{
				gettime(&host_time);
				if(host_time.ti_sec != 59)		break;
			}
		}
	}
	print_to_file(END);
	init_Serial(END);
}
//[*]----------------------------------------------------------------------[*]
