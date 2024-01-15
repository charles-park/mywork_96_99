#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define		ESC		0x1b
#define		MAX		20

void main(void)
{
	unsigned char 	FNAME[MAX];
	int	ch,key;

	FILE	*fp;

	clrscr();

	printf("Read in File Name?? ");		gets(FNAME);

	if((fp = fopen(FNAME,"rt")) != NULL)	{
		while(1)	{
			if( (ch=fgetc(fp)) == EOF)	{
				printf("\n End of File!!~~~~~~");
				getch();
				break;
			}
			else putchar(ch);

			if(kbhit())	{
				key	= getch();
				if(key == ESC)	{
					printf("\n ESC Key Press!!!~~~~");
					getch();
					break;
				}
			}
		}
	}   else	{
		printf("\n FILE Not Found!!!~~~");
		getch();
	}
	fclose(fp);
}
