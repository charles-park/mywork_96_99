#include "hginit.h"
#include "hghlib.h"
#include "hgquick.h"
#include "hwindow.h"
#include "mouse.h"
#include "hgpcx.h"

void main(int argc,char *argv[])
{
	int mode;
	int tmode;

	if (argc!=2) {
		printf("\nUsage:hpcx <filename>\n");
		exit(1);
	}

	if (access(argv[1],0)) {
		printf("\nFile Not Found.\n");
		exit(1);
	}

	mode=hgGetPCXMode(argv[1]);
	if (mode==NO_PCX) {
		printf("\nThis is not valid PCX File.\n");
		exit(1);
	}
	tmode=hgDetectCard();
	switch(tmode) {
		case hgHERCULES:
			if (mode!=PCX_HERCULES) {
				printf("\nThis is not HERCULES PCX File\n");
				exit(1);
			}
			hgSetDisplay(hgHERC640x400);
			hgSetMode(hgGRAPHICS);
			hgPCXFileDisplayH(0,0,argv[1]);
			break;
		case hgEGA:
			switch(mode) {
				case PCX_HERCULES:
					hgSetDisplay(hgEGA640x350x16);
					hgSetMode(hgGRAPHICS);
					hgPCXFileDisplayM(0,0,argv[1]);
					break;
				case PCX_VGA16:
					hgSetDisplay(hgEGA640x350x16);
					hgSetMode(hgGRAPHICS);
					hgPCXFileDisplayV(0,0,argv[1]);
					break;
				default:
					printf("\nThis is not valid Hercules/EGA PCX File\n");
					exit(1);
					break;
			};
                        break;
		case hgVGA:
			switch(mode) {
				case PCX_HERCULES:
					hgSetDisplay(hgVGA640x480x16);
					hgSetMode(hgGRAPHICS);
					hgPCXFileDisplayM(0,0,argv[1]);
					break;
				case PCX_VGA16:
					hgSetDisplay(hgVGA640x480x16);
					hgSetMode(hgGRAPHICS);
					hgPCXFileDisplayV(0,0,argv[1]);
					break;
				default:
					printf("\nThis is not valid VGA 16/Hercules PCX File\n");
					exit(1);
					break;
			};
                        break;
		case hgSVGA:
			switch(mode) {
				case PCX_HERCULES:
					hgSetDisplay(hgVGA640x480x16);
					hgSetMode(hgGRAPHICS);
					hgPCXFileDisplayM(0,0,argv[1]);
					break;
				case PCX_VGA16:
					hgSetDisplay(hgVGA800x600x16);
					hgSetMode(hgGRAPHICS);
					hgPCXFileDisplayV(0,0,argv[1]);
					break;
				case PCX_VGA256:
					hgAutoChipSet();
					hgSetDisplay(hgVGA800x600x256);
					hgSetMode(hgGRAPHICS);
					hgPCXFileDisplay256(0,0,argv[1]);
					break;
				default:
					printf("\nThis is not valid VGA 256/VGA 16/Hercules PCX File\n");
					exit(1);
					break;
			};
	};
	inkey(WAIT);
	hgSetMode(hgTEXT);
}
