/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
/*[*]                       This is serial emulator                      [*]*/
/*[*]                                and                                 [*]*/
/*[*]                      protocal search program                       [*]*/
/*[*]                Copyright Jinyang industrial co.,ltd                [*]*/
/*[*]                      Program by Jinmin - Mok                       [*]*/
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
#include	"main.h"
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
key_t			key;
value_t			value;
host_u			host;
led_u			led;
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
typedef struct	led93__t	{
	unsigned char	wd[5]  ;
	unsigned char	ws[5]  ;
	unsigned char	temp[5];
	unsigned char	pre[5] ;
	unsigned char	ypre[5];
	unsigned char	pred[5];
	unsigned char	tail[5];
	unsigned char	ext    ;
}	led93_t;

typedef union	led93__u	{
	led93_t			send;
	unsigned char	bytes[sizeof(led93_t)];
}	led93_u;
static	led93_u	led93;
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
unsigned char keyinput(void)
{
	static	byte oldsec  = 70;
	static	byte oldmin  = 70;
	static	word pre     = 0;
	static	word ypre    = 0;
	static	word pre_flag= 0;

			char key_in;
			byte return_key = 0xFF;
			unsigned char i;
			int  d_save = 0;
			unsigned char temp[6];

	comport_event();

	rtc_read();
	if(pre_flag == 0)	{	ypre = random(500);	pre_flag = 1;	}

	if(oldsec != rtc.sec)	{
		oldsec = rtc.sec;
		if(!(oldsec % 10))	{

			d_save = random(3600);	// wd
			sprintf(temp,"%5d", d_save);
			for(i=0; i<5; i++)	led93.send.wd[i] = temp[i];
			printf("WD : %5.1f  ",(float)((float)d_save/10.));

			d_save = random(600);	// ws
			sprintf(temp,"%5d", d_save);
			for(i=0; i<5;i++)	led93.send.ws[i] = temp[i];
			printf("WS : %4.1f  ",(float)((float)d_save/10.));

			d_save  = random(500);	// temp
			d_save -= 250;
			sprintf(temp,"%5d", d_save);
			for(i=0; i<5;i++)	led93.send.temp[i] = temp[i];
			printf("TEMP : %4.1f  ",(float)((float)d_save/10.));

			d_save = random(2);		// pred
			sprintf(temp,"%5d" , d_save);
			for(i=0; i<5;i++)	led93.send.pred[i] = temp[i];
			if(d_save != 0)		printf("PRED : OFF  ");
			else				printf("PRED : ON   ");

			sprintf(temp,"%5d" , ypre);	// ypre
			for(i=0; i<5;i++)	led93.send.ypre[i] = temp[i];
			printf("YPRE : %4.1f  ",(float)((float)ypre/10.));

			if(oldmin != rtc.min)	{	oldmin = rtc.min;	pre+= random(5);}
			sprintf(temp,"%5d" , pre);
			for(i=0; i<5;i++)	led93.send.pre[i] = temp[i];
			printf("PRE : %4.1f  ",(float)((float)pre/10.));
			printf("\n\r");

			for(i=0; i<5;i++)	led93.send.tail[i] = '9';
			led93.send.ext = 0x0D;

			printf("PROTOCOL:");
			for(i=0; i<sizeof(led93_u); i++)	{
				send_data(value.comport, led93.bytes[i]);
				printf("%c",  led93.bytes[i]);
			}	printf("\n\r");	printf("\n\r");
		}
	}
	comport_event();
	if(kbhit())		return_key = getch();
/*
	if(kbhit())	{
		key_in = getch();
		if(key_in == 0)	key_in = getch();

		else	if((key_in >= 48) && (key_in <= 57))	key_in -= 48;

		key.key_data[key.key_pos_st++] = key_in;
		if(key.key_pos_st >= 20)	key.key_pos_st = 0;
	}
	comport_event();
	if(key.key_pos_st != key.key_pos_ed)	{
		value.delay_count = 0;
		return_key = key.key_data[key.key_pos_ed++];
		if(key.key_pos_ed >= 20)	key.key_pos_ed = 0;

*/

//printf("%c", return_key);
//send_data(value.comport, return_key);

//nd_led_data(byte port, byte cmd,byte sta, byte cd, byte id)
//send_led_data(value.comport, 'M', 0, 0, 0);
//send_led_data(value.comport, 'M', 0, 0, 1);
//send_led_data(value.comport, 'M', 0, 0, 2);
//send_led_data(value.comport, 'M', 0, 0, 3);
//send_led_data(value.comport, 'M', 0, 0, 4);
//send_led_data(value.comport, 'M', 0, 0, 5);
//send_led_data(value.comport, 'M', 0, 0, 6);
//send_led_data(value.comport, 'M', 0, 0, 7);

	return(return_key);
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
struct ffblk ffblk;
FILE   *fp;
char   file_name[20];

/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
int g_test(void)
{
   int i;

   clrscr();
   for (i=0; i<16; i++)
   {
	   textattr(i + ((i+1) << 4));
	   cprintf("This is a test\r\n");
   }

   return 0;
}

#define	black				0
#define	blue				1
#define	green				2
#define	cyan				3
#define	red					4
#define	magenta				5
#define	brown				6
#define	light_gray			7
#define	dark_gray			8
#define	light_blue			9
#define	light_green			10
#define	light_cyan			11
#define	light_red			12
#define	light_magenta		13
#define	yellow				14
#define	white				15

#define	attribute(b,f)		(b << 4) | f

/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
int main(int arguc, char *argus[])
{
	randomize();
	textbackground(blue);	clrscr();
	sprintf(file_name,"N_SERIAL.CFG");
	if((fp = fopen(file_name,"rt")) == NULL)	{
		fp = fopen(file_name,"wt");
		fprintf(fp,"Test .......");
	} else	{
		fp = fopen(file_name,"wt");
		fprintf(fp,"OK  Test .......");
	}
	fclose(fp);

	textattr(attribute(dark_gray,white));
	cprintf("Clock test....\r\n");
	textattr(attribute(magenta,BLACK));
	cprintf("Clock test....\r\n");
	textattr(attribute(light_magenta,white));
	cprintf("Clock test....\r\n");

//	g_test();

	if(arguc == 2)	value.comport = argus[1][0];
	else	{
		print_err_msg(1);
		return(0);
	}
	if(argument_check() == true)	{
		initiliaze();						// Serial port & rxQ initialize.
		variable_initial();					// Variable initialize.
		cprintf("Connect port : Com%c\r\n",value.comport+0x31);
		while(1)	if(keyinput() == 0x1b)	break;
		return_serial_vector(value.comport);
	}
	textbackground(black);	clrscr();
	return(0);
}
/*[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]*/
