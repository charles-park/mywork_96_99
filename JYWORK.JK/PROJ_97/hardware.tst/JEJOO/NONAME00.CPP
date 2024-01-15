#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define		ESC		0x1b

void main(void)
{
	FILE	*fp;
	char	ch;

	if( fp = fopen(readfile,"rt")) != NULL)	{

		while(1)	{
			if( (ch = fgetc(fp)) == EOF )		break;
			else 	if(ESC == kbhit())			break;
			else	printf(ch);

		}
	}
}