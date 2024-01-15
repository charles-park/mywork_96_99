/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]                       This is serial emulator                      [*]*/
/*[*]                                and                                 [*]*/
/*[*]                      protocal search program                       [*]*/
/*[*]                Copyright Jinyang industrial co.,ltd                [*]*/
/*[*]                      Program by Jinmin - Mok                       [*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#include	"main.h"
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
//   struct variable define
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
key_t			key;
value_t			value;
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void print_err_msg(char x)
{
	switch(x)	{
		case 1	:	printf("Argument error .....!\n");
					printf("Syntex : ");
					break;
		case 2	:	printf("File open error .....!\n");
					break;
		case 3	:	printf("Communication port mismatch .....!\n");
		default	:	break;
	}
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
char argument_check(void)
{
	switch(value.comport)	{
		case '1' :	value.comport = COM1;	break;
		case '2' :	value.comport = COM2;	break;
		default  :	print_err_msg(3);		return false;
	}
	return true;
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
void variable_initial(void)
{
	cprintf("Test run........\r\n");

	rtc_read();
	cprintf("DATE : %02d/%02d/%02d  TIME : %02d/%02d/%02d\r\n",
			rtc.year, rtc.month, rtc.day, rtc.hour, rtc.min, rtc.sec);

	send_data(value.comport, 'A');
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
unsigned char keyinput(void)
{
	static	byte oldsec  = 70;
	static	byte oldmin  = 70;

			char key_in;
			byte return_key = 0xFF;
			unsigned char i;

	comport_event();

	rtc_read();

	if(oldsec != rtc.sec)	{
		oldsec = rtc.sec;
		if(!(oldsec % 5))	{
		}
	}
	comport_event();
	if(kbhit())		return_key = getch();

	return(return_key);
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
int main(int arguc, char *argus[])
{
	FILE	*fp;
	char   file_name[20];

	clrscr();
	sprintf(file_name,"SENSOR.DAT");

	if((fp = fopen(file_name,"rt")) == NULL)	fp = fopen(file_name,"wt+");
	else                                    	fp = fopen(file_name,"wt+");

	printf("IRDAM SENSOR TEST PROGRAM...\n\r");
	printf("SAVE FILE NAME is SENSOR.DAT (TEXT FILE).\r\n");
	printf("SAVE TIME is 1 Hour.\r\n");

	if(arguc == 2)	value.comport = argus[1][0];
	else	{	print_err_msg(1);	return(0);	}

	if(argument_check() == true)	{
		initiliaze();						// Serial port & rxQ initialize.
		variable_initial();					// Variable initialize.
		cprintf("Connect port : Com%c\r\n",value.comport+0x31);
		while(1)	if(keyinput() == 0x1b)	break;
		return_serial_vector(value.comport);
	}
	clrscr();
	fclose(fp);
	return(0);
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
