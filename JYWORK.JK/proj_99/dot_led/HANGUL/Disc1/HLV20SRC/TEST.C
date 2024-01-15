#include "hginit.h"
#include "hghlib.h"
#include "hgpcx.h"

void main()
{
	hgAutoChipSet();
	hgSetDisplay(hgVGA640x480x256);
	hgSetMode(hgGRAPHICS);

	hgEnablePCXFileDisplay(PCX_VGA256_FUNCTION);

	hgSetBack(BLUE);
	hgClearScreen();

	hgPCXFileDisplay(0,0,"js4.pcx");

	inkey(WAIT);

	hgSetMode(hgTEXT);
}