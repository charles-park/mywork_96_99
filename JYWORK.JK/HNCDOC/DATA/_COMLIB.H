#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <io51.h>
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		DLL(x)			(x+0)
#define		DLM(x)			(x+1)
#define		THR(x)			(x+0)
#define		RBR(x)			(x+0)
#define		IER(x)			(x+1)
#define		IIR(x)			(x+2)
#define		LCR(x)			(x+3)
#define		MCR(x)			(x+4)
#define		LSR(x)			(x+5)
#define		MSR(x)			(x+6)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		INTERNAL		0
#define		EXTERNAL		1
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		Tx				0
#define		Rx				1
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		DATABIT7		0x2
#define		DATABIT8		0x3
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		STOP1			0x0
#define		STOP2			0x4
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		PARITY_NO		0x0
#define		PARITY_ODD		0x8
#define		PARITY_EVEN		0x18
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		BUF_SIZE		100
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		M11				0
#define		M22				1
#define		M3				2
#define		M1				3
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		BPS9600			0
#define		BPS4800			1
#define		BPS2400			2
#define		BPS1200			3
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		SMOD_ON			1
#define		SMOD_OFF		0
#define		NOT_USED		0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		disable(y)		y = OFF
#define		enable(z)		z = ON
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define		ON				1
#define		OFF				0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct	Variable__t	{
	Word	Sp;
	Word	Ep;
	Word	Size;
	Word	Dsize;
	Byte	Flag;
	Byte	*Buf;
}	Variable_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct	Protocol__t	{
	Variable_t	var;
	Byte	(*Pcheck)(Variable_t *p);
	Byte	(*Pcatch)(Variable_t *p);
}	Protocol_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct	Protocol_Grp__t	{
	Byte	Pcnt_Rx;
	Byte	Pcnt_Tx;
	Byte	INT_TYPE;
	Word	BASE;
	Protocol_t	*p_Rx;
	Protocol_t	*p_Tx;
}	Protocol_Grp_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	Byte	INTER_Baud[] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	Byte	EXTER_Baud[] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	Protocol_Grp_t	Com1,Com2,Com3;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Init_Serial_Grp( Protocol_Grp_t *Comx, Byte Mode, Byte B_addr, Word Speed,
					  Byte CRY, Byte S_MOD, Byte Sta, Byte Pcount_Rx, Byte Pcount_Tx);
void Init_Serial_Pro( Protocol_Grp_t	*Comx, Byte Pcount, Byte BufSize, Byte (*chk)(Variable_t *FuncP1),
					  Byte (*cat)(Variable_t *FuncP2), Byte Rx_Tx);
void Set_Protocol_Flag(Protocol_Grp_t *Comx, Byte Pcount, Byte Sta, Byte Rx_Tx);
void Protocol_Q(Protocol_Grp_t *Comx, Byte Pcount, Byte RecData);
Byte Com1_chk1(Variable_t *var);
Byte Com1_cat1(Variable_t *var);
void Protocol_event(Protocol_Grp_t *Comx, Byte RecData);
void main(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]



















































































































































