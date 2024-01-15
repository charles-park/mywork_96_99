//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include "_ComLib.h"
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Init_Serial_Grp Function ]
// Return Type : void
// Input Data Field
// ( Protocol_Grp_t *, Mode, Base Addr, Baudrate, Crystal, SMOD,
//   Communication Status, Protocol Rx Value, Protocol Rx Value )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Init_Serial_Pro Function ]
// Return Type : void
// Input Data Field
// ( Protocol_Grp_t *, Protocol Count, Protocol Buffer Size,
//   Protocol check Function Pointer, Protocol catch Function Pointer, Rx or Tx )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Set_Protocol_Flag Function ]
// Return Type : void
// Input Data Field
// ( Protocol_Grp_t *, Protocol Count, Set & Reset command )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Protocol_event Function ]
// Return Type : void
// Input Data Field
// ( Protocol_Grp_t *, receive data )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Protocol_Q Function ]
// Return Type : void
// Input Data Field
// ( Protocol_Grp_t *, Protocol Count, receive data )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Com1_chk1 Function ]
// Return Type : Byte
// Input Data Field
// ( Variable_t * )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Com1_cat1 Function ]
// Return Type : Byte
// Input Data Field
// ( Variable_t * )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Com1_chk2 Function ]
// Return Type : Byte
// Input Data Field
// ( Variable_t * )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
// Used        : [ Com1_cat2 Function ]
// Return Type : Byte
// Input Data Field
// ( Variable_t * )
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Init_Serial_Grp( Protocol_Grp_t *Comx, Byte Mode, Byte B_addr, Word Speed,
					  Byte CRY, Byte S_MOD, Byte Sta, Byte Pcount_Rx, Byte Pcount_Tx)
{
	Byte	Baud_Value=0, Baud=0;

	Comx->BASE = B_addr;
	Comx->INT_TYPE = Mode;

	Comx->Pcnt_Rx = Pcount_Rx;
	Comx->p_Rx = (Protocol_t *)(malloc( sizeof(Protocol_t) * Comx->Pcnt_Rx ));
	Comx->Pcnt_Tx = Pcount_Tx;
	Comx->p_Tx = (Protocol_t *)(malloc( sizeof(Protocol_t) * Comx->Pcnt_Tx ));

	if(Comx->INT_TYPE == INTERNAL)	{
		disable(EA);

		Baud_Value += ((CRY == M22) ? 1:0);
		if(S_MOD)	{	PCON |= 0x80;	Baud_Value += 1; }
		else		{	PCON &= 0x7F;	Baud_Value += 0; }

		switch(Speed)	{
			case	BPS9600:	TH1 = INTER_Baud[  Baud_Value];		break;
			case	BPS4800:	TH1 = INTER_Baud[++Baud_Value];		break;
			case	BPS2400:	TH1 = INTER_Baud[++Baud_Value];		break;
			case	BPS1200:	TH1 = INTER_Baud[++Baud_Value];		break;
		}
		SCON |= 0x50;		TMOD |= 0x20;		enable(TR1);
		enable(ES);			enable(EA);
	}	else	{
		Baud_Value += ((CRY == M3) ? 4:0);

		switch(Speed)	{
			case	BPS9600:	Baud = EXTER_Baud[  Baud_Value];	break;
			case	BPS4800:	Baud = EXTER_Baud[++Baud_Value];	break;
			case	BPS2400:	Baud = EXTER_Baud[++Baud_Value];	break;
			case	BPS1200:	Baud = EXTER_Baud[++Baud_Value];	break;
		}
		output( LCR(Comx->BASE), (Sta & 0x7F) );
		output( LCR(Comx->BASE), (Sta | 0x80) );
		output( DLL(Comx->BASE), Baud );
		output( DLM(Comx->BASE), 0x00 );
		output( IER(Comx->BASE), 0x01 );
		output( MCR(Comx->BASE), 0x0b );
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Init_Serial_Pro(Protocol_Grp_t	*Comx, Byte Pcount, Byte BufSize, Byte (*chk)(Variable_t *FuncP1),
					 Byte (*cat)(Variable_t *FuncP2), Byte Rx_Tx)
{
	if(Rx_Tx)	{
		Comx->p_Rx[Pcount].var.Sp   = 0;
		Comx->p_Rx[Pcount].var.Ep   = 0;
		Comx->p_Rx[Pcount].var.Flag = 0;
		Comx->p_Rx[Pcount].var.Dsize= 0;
		Comx->p_Rx[Pcount].var.Size = BufSize;
		Comx->p_Rx[Pcount].var.Buf  = (Byte *)(malloc(Comx->p_Rx[Pcount].var.Size));
		Comx->p_Rx[Pcount].Pcheck   = chk;
		Comx->p_Rx[Pcount].Pcatch   = cat;
	}	else	{
		Comx->p_Tx[Pcount].var.Sp   = 0;
		Comx->p_Tx[Pcount].var.Ep   = 0;
		Comx->p_Tx[Pcount].var.Flag = 0;
		Comx->p_Tx[Pcount].var.Dsize= 0;
		Comx->p_Tx[Pcount].var.Size = BufSize;
		Comx->p_Tx[Pcount].var.Buf  = (Byte *)(malloc(Comx->p_Tx[Pcount].var.Size));
		Comx->p_Tx[Pcount].Pcheck   = chk;
		Comx->p_Tx[Pcount].Pcatch   = cat;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Set_Protocol_Flag(Protocol_Grp_t *Comx, Byte Pcount, Byte Sta, Byte Rx_Tx)
{
	if(Rx_Tx)	{
		Comx->p_Rx[Pcount].var.Flag = Sta;
		if(Comx->p_Rx[Pcount].var.Flag == ON)	Comx->p_Rx[Pcount].var.Dsize = 0;

	}	else	{
		Comx->p_Tx[Pcount].var.Flag = Sta;
		if(Comx->p_Tx[Pcount].var.Flag == ON)	Comx->p_Tx[Pcount].var.Dsize = 0;
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Protocol_Q(Protocol_Grp_t *Comx, Byte Pcount, Byte RecData)
{
	Comx->p_Rx[Pcount].var.Dsize++;
	Comx->p_Tx[Pcount].var.Ep %= Comx->p_Rx[Pcount].var.Size;

	if(Comx->p_Rx[Pcount].var.Ep == Comx->p_Rx[Pcount].var.Sp)	{
		Comx->p_Rx[Pcount].var.Sp++;
		Comx->p_Rx[Pcount].var.Sp %= Comx->p_Rx[Pcount].var.Size;
	}
	Comx->p_Rx[Pcount].var.Buf[Comx->p_Rx[Pcount].var.Ep++] = RecData;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte Com1_chk1(Variable_t *var)
{
	return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte Com1_cat1(Variable_t *var)
{
	return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte Com1_chk2(Variable_t *var)
{
	return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
Byte Com1_cat2(Variable_t *var)
{
	return	0;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void Protocol_event(Protocol_Grp_t *Comx, Byte RecData)
{
	Byte	pos;
	for(pos = 0; pos < Comx->Pcnt_Rx; pos++)	{
		if(Comx->p_Rx[pos].var.Flag)	{
			Protocol_Q(Comx, pos, RecData);
			if(Comx->p_Rx[pos].Pcheck(&Comx->p_Rx[pos].var))		{
				if(Comx->p_Rx[pos].Pcatch(&Comx->p_Rx[pos].var))	{
					Set_Protocol_Flag(Comx,pos,OFF,Rx);
				}
			}
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void main(void)
{
	Init_Serial_Grp(&Com1, INTERNAL, 0x0, BPS9600, M11, SMOD_OFF,
					NOT_USED, 1, 1);
	Init_Serial_Pro(&Com1, 0, 12, Com1_chk1, Com1_cat1, Rx);
	Init_Serial_Pro(&Com1, 0, 14, Com1_chk2, Com1_cat2, Tx);
	// Allways Serial Buffer Open
	Set_Protocol_Flag(&Com1, 0, ON, Rx);
	while(1)	{
		Protocol_event(&Com1, 0xff);
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴 Function Examples 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�[*]
//	Init_Serial_Pro(&Com1, 1, 20, Com1_chk2, Com1_cat2);

//	Init_Serial_Grp(&Com2, EXTERNAL, 0xC000, BPS4800, M1, NOT_USED, (DATABIT8|STOP1|PARITY_NO), 2);
//	Init_Serial_Pro(&Com2, 0, 12, Com2_chk1, Com2_cat1);
//	Init_Serial_Pro(&Com2, 1, 20, Com2_chk2, Com2_cat2);

//	Init_Serial_Grp(&Com3, EXTERNAL, 0xD000, BPS2400, M1, NOT_USED, (DATABIT8|STOP1|PARITY_NO), 2);
//	Init_Serial_Pro(&Com3, 0, 12, Com3_chk1, Com3_cat1);
//	Init_Serial_Pro(&Com3, 1, 20, Com3_chk2, Com3_cat2);

//	Set_Protocol_Flag(&Com2, 0, ON);
//	Set_Protocol_Flag(&Com3, 0, ON);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]