#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>

void main(void)
{
	FILE *rp;
	FILE *wp;

	rp = fopen("HAN.FOT", "rb");
	wp = fopen("HAN.H"  , "wt");


	fprintf(wp, "byte EngFont[256][16] = {\n");
	for(int i=0; i<256; i++)	{
		fprintf(wp, "\t{");
		for(int j=0; j<16; j++)		{
			fprintf(wp," 0x%02X%c",fgetc(rp), ((j<15) ? ',' : ' '));
		}
		fprintf(wp, "}%c\n",((i<255) ? ',' : ' '));
	}
	fprintf(wp, "};\n");

	fclose(rp);
	fclose(wp);
}

