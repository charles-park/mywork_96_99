#include "hginit.h"
#include "hghlib.h"

void main()
{
	char *s[]={ "Tseng ET4000","PARADISE","Tseng ET3000","GENOA",
		    "ATI VGA Wonder","AHEAD","POACH","UNKNOWN" };
	int chip;

	chip=hgGetChipSet();

	if (chip==-1) chip=7;

	if (hgGetTextMode()==7) printf("\n\nThis Video Card is Hercules\n\n");
	else if (hgIsEGAVGA() && !hgIsVGA()) printf("\n\nThis Video Card is EGA\n\n");
	else if (hgIsVGA()) {
		printf("\n\nThis Video Card is VGA\n");
		printf("SuperVGA ChipSet is %-s\n\n",s[chip]);
	} else
		printf("\n\nThis Video Card can't be supported by Hurumi Library 2.0\n\n");
}