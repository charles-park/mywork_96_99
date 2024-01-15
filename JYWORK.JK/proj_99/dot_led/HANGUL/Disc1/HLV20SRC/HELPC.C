#include <stdio.h>

void writedata(FILE *idx,char *buff,long offset)
{
	buff[strlen(buff)-1]=0;
	fprintf(idx,"%-s,%ld\n",buff,offset);
}

void main(int argc,char *argv[])
{
	char temp[50];
	char buff[80];
	FILE *fpt,*idx;
	long offset;

	printf("\nHurumi Hangul Library v2.0 Help Index Maker\n");
	if (argc!=2) {
		printf("\nUsage: helpc <filename> ( don't include extension ) \n");
		exit(1);
	}

	strcpy(temp,argv[1]);
	strcat(temp,".hhf");
	fpt=fopen(temp,"r");
	if (fpt==NULL) {
		printf("\nSource File not Found\n");
		exit(1);
	}
	strcpy(temp,argv[1]);
	strcat(temp,".hix");
	idx=fopen(temp,"w");
	if (idx==NULL) {
		printf("\nWrite error\n");
		exit(1);
	}
	while (fgets(buff,80,fpt)!=NULL) {
		if (buff[0]=='$' && buff[1]=='$') break;
		if (buff[0]=='$') {
			offset=ftell(fpt);
			writedata(idx,buff,offset);
		}
	}
	fclose(idx);
	fclose(fpt);
	printf("%-s is created..\n",temp);
}
