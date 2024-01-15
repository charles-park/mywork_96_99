#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc,char *argv[])
{
	int spotX,spotY;
	FILE *fpt;
	FILE *wrt;

	int  i,count;
	char temp[20];
	char temp2[20];
	int  ch;

	printf("\nHurumi Hangul Library v2.0 MouseCursor Converter\n");

	if (argc!=4) {
		printf("\nUsage: hmcmaker <filename> spotX spotY\n");
		exit(1);
	}

	spotX=atoi(argv[2]);
	spotY=atoi(argv[3]);

	strcpy(temp,argv[1]);
	strcpy(temp2,argv[1]);
	strcat(temp2,".hmf");
	strcat(temp,".hmc");
	fpt=fopen(temp,"rb");
	wrt=fopen(temp2,"wb");

	if (fpt==NULL) {
		printf("\nFile Not Found.\n");
		exit(1);
	}
	fprintf(wrt,"#include <alloc.h>\n");
	fprintf(wrt,"#include <mem.h>\n");
	fprintf(wrt,"#include \"mouse.h\"\n");
	fprintf(wrt,"\nvoid %-s()\n{\n",argv[1]);
	fprintf(wrt,"\tchar s1[]={\n");

	count=0;
	fprintf(wrt,"\t");
	for (i=0;i<32;i++) {
		ch=fgetc(fpt);
		if (i!=31) fprintf(wrt,"%3d,",ch);
		else fprintf(wrt,"%3d",ch);
		count++;
		if (count>15) {
			fprintf(wrt,"\n\t");
			count=0;
		}
	}
	fprintf(wrt,"\n\t};\n");
	fprintf(wrt,"\tchar s2[]={\n");

	count=0;
	fprintf(wrt,"\t");
	for (i=0;i<32;i++) {
		ch=fgetc(fpt);
		if (i!=31) fprintf(wrt,"%3d,",ch);
		else fprintf(wrt,"%3d",ch);
		count++;
		if (count>15) {
			fprintf(wrt,"\n\t");
			count=0;
		}
	}
	fprintf(wrt,"\n\t};\n");
	fprintf(wrt,"\tint x,y;\n");
	fprintf(wrt,"\tGraphCursor *p;\n\n");
	fprintf(wrt,"\tp=(GraphCursor *)malloc(sizeof(GraphCursor));\n");
	fprintf(wrt,"\tp->spotX=%d;p->spotY=%d;\n",spotX,spotY);
	fprintf(wrt,"\tmemcpy(p->screenMask,s1,32);\n");
	fprintf(wrt,"\tmemcpy(p->cursorMask,s2,32);\n");
	fprintf(wrt,"\thgGetMousePos(&x,&y);\n");
	fprintf(wrt,"\thgHideMouse();\n");
	fprintf(wrt,"\tsetMouseCursor(p);\n");
	fprintf(wrt,"\thgMoveMouse(x,y);\n");
	fprintf(wrt,"\thgShowMouse();\n");
	fprintf(wrt,"\tfree(p);\n");
	fprintf(wrt,"}");

	fclose(wrt);
	fclose(fpt);
	printf("%-s is created.\n",temp2);
}