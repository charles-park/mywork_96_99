#include "PLM.H"
extern xdata unsigned char MEMORY_C00;
extern plm short PRINT_C00(data short);
extern plm short PRINT_BUFF_C01(data short);


void HEJ_C02(data char **p)
{
	xdata short nr;
	xdata char arr[] = "C writes\n";
	
	nr = 0;
	MEMORY_C00 = mem_XDATA;
	nr += PRINT_C00(C_to_PLM_word_p(arr));
	nr += PRINT_BUFF_C01(C_to_PLM_word_p(p));
}
