//void proce(sample_t *p)
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

void proce(sample_t *p)
{
	p->com1_rx_flag = 1;
	p->com1_tx_flag = 0;
	p->com2_rx_flag = 1;
	p->com2_tx_flag = 1;
	p->com1_rx = 0xFF ;
	p->com1_tx = 0xFE ;
	p->com2_rx = 0xFC ;
	p->com2_tx = 0xFD ;
}
