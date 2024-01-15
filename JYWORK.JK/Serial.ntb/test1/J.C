#include "stdio.h"
#include "stdlib.h"
#include "stdlib.h"
#include "conio.h"

/////////// Header File ///////////////
typedef unsigned char	byte;

typedef struct sample__t	{
	byte	com1_rx_flag;
	byte	com1_tx_flag;
	byte	com2_rx_flag;
	byte	com2_tx_flag;
	byte	com1_rx;
	byte	com1_tx;
	byte	com2_rx;
	byte	com2_tx;
}	sample_t;

/////////// Header File End ///////////
//sample_t proce(void)
/*sample_t *proce(void)
{
	sample_t	sample;

	sample.com1_rx_flag = 1;
	sample.com1_tx_flag = 0;
	sample.com2_rx_flag = 1;
	sample.com2_tx_flag = 1;
	sample.com1_rx = 0xFF ;
	sample.com1_tx = 0xFE ;
	sample.com2_rx = 0xFC ;
	sample.com2_tx = 0xFD ;
	return(&sample);
}
*/
/*
sample_t *proce(sample_t *p)
{
	p->com1_rx_flag = 1;
	p->com1_tx_flag = 0;
	p->com2_rx_flag = 1;
	p->com2_tx_flag = 1;
	p->com1_rx = 0xFF ;
	p->com1_tx = 0xFE ;
	p->com2_rx = 0xFC ;
	p->com2_tx = 0xFD ;
	return(p);
}
*/
sample_t	comport;

//extern	proce(sample_t *);
void main(void)
{
	clrscr();
	proce(&comport);
	printf("com1_rx_flag = %d \n\r", comport.com1_rx_flag);
	printf("com1_tx_flag = %d \n\r", comport.com1_tx_flag);
	printf("com2_rx_flag = %d \n\r", comport.com2_rx_flag);
	printf("com2_tx_flag = %d \n\r", comport.com2_tx_flag);
	printf("com1_rx = %02X \n\r",	 comport.com1_rx);
	printf("com1_tx = %02X \n\r",    comport.com1_tx);
	printf("com2_rx = %02X \n\r",    comport.com2_rx);
	printf("com2_tx = %02X \n\r",    comport.com2_tx);
	getch();
}