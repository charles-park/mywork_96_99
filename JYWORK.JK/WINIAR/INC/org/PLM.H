#define		mem_DATA	0
#define		mem_XDATA	1
#define		mem_CODE	2
#define		mem_PDATA	3

#define PLM_to_C_p(mem,adr)	\
	((unsigned char *) (((unsigned long) mem << 16) + adr))
		
#define C_to_PLM_byte_p(p)	((unsigned char) p)		

#define C_to_PLM_word_p(p)	((unsigned short) p)		

#define C_to_PLM_memory(p)	((unsigned char) ((unsigned long) p >> 16))

