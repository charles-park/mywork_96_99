#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <io51.h>
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	DLL(X)			(X+0)
#define	DLM(X)			(X+1)
#define	THR(X)			(X+0)
#define	RBR(X)			(X+0)
#define	IER(X)			(X+1)
#define	IIR(X)			(X+2)
#define	LCR(X)			(X+3)
#define	MCR(X)			(X+4)
#define	LSR(X)			(X+5)
#define	MSR(X)			(X+6)
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	INTERNAL		0
#define	EXTERNAL		1
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	DATABIT7		0x2
#define	DATABIT8		0x3
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	STOP1			0x0
#define	STOP2			0x4
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define PARITY_NO		0x0
#define PARITY_ODD		0x8
#define PARITY_EVEN		0x18
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	BUF_SIZE		100		// Serial Buffer Size Define
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	M11				0		// 11.0592 Mhz
#define	M22				1		// 22.1184 Mhz
#define	M3				2		// 3.072   Mhz
#define	M1				3		// 1.8432  Mhz
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	BPS9600			3		// Baud 9600
#define BPS4800			2       // Baud 4800
#define	BPS2400			1       // Baud 2400
#define	BPS1200			0       // Baud 1200
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	SMOD_ON			1
#define	SMOD_OFF		0
#define	NOT_USED        0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	disable(Y)		Y = ON
#define	enable(Z)		Z = OFF
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	ON				1
#define	OFF				0
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct	Var__t	{
	Word	Sp,Ep                    ;	// Receive Start & End Pointer define
	Word	Size,Dsize               ;	// Buffer Size & Receive Data Value
	Byte	Flag                     ;	// Receive Flag
	Byte	*Buf                     ;	// Protocol Malloc
}	Var_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct	Protocol__t	{
	Var_t	var;
	Word	(*Pcheck)(Var_t *p);	// Protocol Check Function Pointer
	Word	(*Pcatch)(Var_t *p);	// if Protocol Check is OK -> Catch Protocol
}	Protocol_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef	struct	Protocol_Grp__t	{
	Byte	Pcnt     ;		// Protocol Value
	Word	BASE     ;		// Location	Address (EX: Internal-> 0)
	Byte	INT_TYPE ;		// Internal or External
	Protocol_t	*p   ;		// Protocol Malloc
}	Protocol_Grp_t   ;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	Byte INTER_Baud[6] = {0xFD, 0xFA, 0xF4, 0xE8, 0xD0, 0xA0};
static	Byte EXTER_Baud[8] = {0x0C, 0x18, 0x30, 0x60, 0x14, 0x28, 0x50, 0xA0};
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static 	Protocol_Grp_t	Com1,Com2,Com3;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Init_Serial_Grp(Protocol_Grp_t *Comx, Byte Mode, Byte B_addr, Byte CRY,
					 Word SPEED, Byte S_MOD, Byte Sta, Byte count);
void Init_Serial_Pro(Protocol_Grp_t *Comx, Byte Pvalue, Word PSize,
					 Word (*chk)(Var_t *aa), Word (*cat)(Var_t *bb));
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Word chk1_1(Var_t *Comx);
Word chk1_2(Var_t *Comx);
Word chk1_3(Var_t *Comx);
Word chk2_1(Var_t *Comx);
Word chk2_2(Var_t *Comx);
Word chk3_1(Var_t *Comx);
Word cat1_1(Var_t *Comx);
Word cat1_2(Var_t *Comx);
Word cat1_3(Var_t *Comx);
Word cat2_1(Var_t *Comx);
Word cat2_2(Var_t *Comx);
Word cat3_1(Var_t *Comx);
void main(void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Init_Serial_Grp(Protocol_Grp_t *Comx, Byte Mode, Byte B_addr, Byte CRY,
					 Word SPEED, Byte S_MOD, Byte Sta, Byte count)
{
	Byte	Baud_Value=0,Baud=0;

	Comx->BASE = B_addr;
	Comx->INT_TYPE = Mode;
	Comx->Pcnt = count;
	Comx->p = (Protocol_t *)(malloc(sizeof(Protocol_t) * Comx->Pcnt));

	if(Comx->INT_TYPE == INTERNAL)	{
		disable(EA);

		Baud_Value += ((CRY == M22)	?	1:0)	;
		if(S_MOD)	{	PCON |= 0x80;	Baud_Value += 1;	}
		else		{   PCON &= 0x7f;	Baud_Value += 0;	}

		switch(SPEED)
		{
			case	BPS9600:	TH1 = INTER_Baud[  Baud_Value];		break;
			case	BPS4800:    TH1 = INTER_Baud[++Baud_Value];		break;
			case	BPS2400:    TH1 = INTER_Baud[++Baud_Value];		break;
			case	BPS1200:    TH1 = INTER_Baud[++Baud_Value];		break;
			default		   :										break;
		}
		SCON |= 0x50;			TMOD |= 0x20;		enable(TR1);
		enable(ES);				enable(EA);
	}
	else
	{
		Baud_Value += ((CRY == M3)	?	4:0)	;

		switch(SPEED)
		{
			case	BPS9600:	Baud = EXTER_Baud[  Baud_Value];	break;
			case	BPS4800:    Baud = EXTER_Baud[++Baud_Value];	break;
			case	BPS2400:    Baud = EXTER_Baud[++Baud_Value];	break;
			case	BPS1200:    Baud = EXTER_Baud[++Baud_Value];	break;
			default		   :										break;
		}
		output( LCR(Comx->BASE),(Sta & 0x7f));
		output( LCR(Comx->BASE),0x80);
		output( DLL(Comx->BASE),Baud);
		output( DLM(Comx->BASE),0x00);
		output( IER(Comx->BASE),0x01);
		output( MCR(Comx->BASE),0x0b);
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Init_Serial_Pro(Protocol_Grp_t *Comx, Byte Pvalue, Word PSize,
					 Word (*chk)(Var_t *aa), Word (*cat)(Var_t *bb))
{
	Comx->p[Pvalue].var.Sp = 0;
	Comx->p[Pvalue].var.Ep = 0;
	Comx->p[Pvalue].var.Flag = 0;
	Comx->p[Pvalue].var.Dsize = 0;
	Comx->p[Pvalue].var.Size = PSize;
	Comx->p[Pvalue].var.Buf = (Byte *)(malloc(Comx->p[Pvalue].var.Size));
	Comx->p[Pvalue].Pcheck = chk;
	Comx->p[Pvalue].Pcatch = cat;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Word chk1_1(Var_t *Comx)
{
}
Word chk1_2(Var_t *Comx)
{
}
Word chk1_3(Var_t *Comx)
{
}
Word chk2_1(Var_t *Comx)
{
}
Word chk2_2(Var_t *Comx)
{
}
Word chk3_1(Var_t *Comx)
{
}
Word cat1_1(Var_t *Comx)
{
}
Word cat1_2(Var_t *Comx)
{
}
Word cat1_3(Var_t *Comx)
{
}
Word cat2_1(Var_t *Comx)
{
}
Word cat2_2(Var_t *Comx)
{
}
Word cat3_1(Var_t *Comx)
{
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	//Init_Serial_Grp(&Com1,INTERNAL,0x0,BPS9600,M11,NO_SMOD,NOT_USED,Protocol value);
	//Init_Serial_Pro(&Struct, Mode, Protocol Value, Buffer Size
	//                 check Function Pointer, Catch Pointer)
	while(1)
	{
		Init_Serial_Grp(&Com1,INTERNAL,0x0,BPS9600,M11,SMOD_OFF,NOT_USED,3);
		Init_Serial_Pro(&Com1,0,10,chk1_1,cat1_1);     // Com1-> Protocol 1
		Init_Serial_Pro(&Com1,1,15,chk1_2,cat1_2);	   // Com1-> Protocol 2
		Init_Serial_Pro(&Com1,2,20,chk1_3,cat1_3);	   // Com1-> Protocol 3

		Init_Serial_Grp(&Com2,EXTERNAL,0xC000,BPS4800,M1,NOT_USED,(DATABIT8|STOP1|PARITY_NO),2);
		Init_Serial_Pro(&Com2,0,15,chk2_1,cat2_1);     // Com2-> Protocol 1
		Init_Serial_Pro(&Com2,1,10,chk2_2,cat2_2);	   // Com2-> Protocol 2

		Init_Serial_Grp(&Com3,EXTERNAL,0xD000,BPS2400,M1,NOT_USED,(DATABIT8|STOP1|PARITY_NO),1);
		Init_Serial_Pro(&Com3,0,40,chk3_1,cat3_1);     // Com2-> Protocol 1
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
