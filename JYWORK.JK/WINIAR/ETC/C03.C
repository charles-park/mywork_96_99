#include "PLM.H"
extern void HEJ_C02(data char **);
extern plm void HEJ_P02(data short);

static xdata char a[] = "First line";
static data char b[] = "Second line";
static idata char c[] = "Third line";
static code char d[] = "Fourth line";
static xdata char e[] = "Fifth line";
static data char f[] = "Sixth line";
static idata char g[] = "Seventh line";
static code char h[] = "Eight line";
static xdata char i[] = "Nineth line";

static xdata char *array[10];

xdata char *PRINT_THIS_ALSO_C03;

xdata char why_me[] = "How about this\n";

void main(void)
{
	PRINT_THIS_ALSO_C03 = why_me;
	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = d;
	array[4] = e;
	array[5] = f;
	array[6] = g;
	array[7] = h;
	array[8] = i;
	array[9] = 0;
	HEJ_C02(array);
	HEJ_P02(C_to_PLM_word_p(array));
}
