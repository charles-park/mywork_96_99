#include <stdio.h>
#include <conio.h>
#include <alloc.h>
//(=)����������������������������������������������������������������������(=)
unsigned long memchk;
//(=)����������������������������������������������������������������������(=)
void BEGIN(void)
{
	memchk = farcoreleft();
}
//(=)����������������������������������������������������������������������(=)
void END  (void) {
	printf("\nPROGRAM STARTING FREE MEMORY SIZE : %8lu Byte"    ,memchk);
	printf("\nPROGRAM ENDED    FREE MEMORY SIZE : %8lu Byte\n\n",farcoreleft());
	if(memchk != farcoreleft())	{
		putch(0x07); delay(1);
		putch(0x07); delay(1);
		putch(0x07); delay(1);
		getch();
	}
}
//(=)����������������������������������������������������������������������(=)
#pragma startup BEGIN 64
#pragma exit    END   64
//(=)����������������������������������������������������������������������(=)