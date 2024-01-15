#include "PLM.H"

xdata unsigned char MEMORY_C00;

plm short PRINT_C00(data short adr)
{
	idata unsigned char *p;
	idata short nr;
	
	p = PLM_to_C_p(MEMORY_C00,adr);
	nr = 0;
	while (*p)
	{
		putchar(*p++);
		nr++;
	}
	return (nr);
}
