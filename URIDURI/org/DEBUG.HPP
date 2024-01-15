#include <stdio.h>
#include <conio.h>
#include <alloc.h>
//(=)컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴(=)
unsigned long memchk;
//(=)컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴(=)
void BEGIN(void)
{
	memchk = farcoreleft();
}
//(=)컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴(=)
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
//(=)컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴(=)
#pragma startup BEGIN 64
#pragma exit    END   64
//(=)컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴(=)
