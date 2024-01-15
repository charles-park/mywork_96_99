#include "PLM.H"
extern xdata unsigned char MEMORY_C00;
extern plm short PRINT_C00(data short);

static short print_ln(data short nr)
{
	xdata short nr2;
	data unsigned char str[2] = "\n";
	
	nr2 = 0;
	while (nr--)
	{
		nr2++;
		MEMORY_C00 = mem_DATA;
		PRINT_C00(C_to_PLM_word_p(str));
	}
	return (nr2);
}

plm short PRINT_BUFF_C01(data short pek)
{
	idata short nr;
	char **p;
	
	nr = 0;
	p = (char **) PLM_to_C_p(mem_XDATA,pek);
	while (*p)
	{
		MEMORY_C00 = C_to_PLM_memory(*p);
		nr += PRINT_C00(C_to_PLM_word_p(*p));
		nr += print_ln(1);
		p++;
	}
	return (nr);
}
