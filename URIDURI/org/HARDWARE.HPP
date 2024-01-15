#ifndef	__HARDWARE_HPP__
#define	__HARDWARE_HPP__
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//		IRQ_NO		  SYS_NO			//	IRQ Number Table
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define	IRQ_00			0x08
#define	IRQ_01			0x09
#define	IRQ_02			0x0A
#define	IRQ_03			0x0B
#define	IRQ_04			0x0C
#define	IRQ_05			0x0D
#define	IRQ_06			0x0E
#define	IRQ_07			0x0F
#define	IRQ_08			0x70
#define	IRQ_09			0x71
#define	IRQ_10			0x72
#define	IRQ_11			0x73
#define	IRQ_12			0x74
#define	IRQ_13			0x75
#define	IRQ_14			0x76
#define	IRQ_15			0x77
#define	IRQ_NC			0xFF
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//	Interrupt Controller 8259
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#define IC_8259A		0x20 				// Interrupt Controller Address
#define IC_8259B		0xA0
#define IC_8259(irqno)	((irqno & 0xF0) ? IC_8259B : IC_8259A)
#define IMR1	  		(IC_8259A + 1)      // Interrupt Mask Address
#define IMR2        	(IC_8259B + 1)
#define IMR(irqno)		((irqno & 0xF0) ? IMR2 : IMR1)
#define MASKOFF(irqno)	(unsigned char)(  0x01 << (irqno & 0x07))	// Mask OFF Value
#define MASKON(irqno)	(unsigned char)(~(0x01 << (irqno & 0x07))) 	// Mask ON  Value
#define EOI 			0x20										// End of Interrupt Value
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//  Interrupt Controller End of Interrupt Value
//	control is	:	if(	IRQ8 ~ IRQ15)	outportb(IC_8259_2, EOI)
//					outportb(IC_8259_1, EOI)
//	Note		:
//
//	[ IC 8259A 0x20 ]
//  +-------+-------+-------+-------+-------+-------+-------+-------+
//  | IRQ00 | IRQ01 | IRQ02 | IRQ03 | IRQ04 | IRQ05 | IRQ06 | IRQ07 |
//  +-------+-------+-------+-------+-------+-------+-------+-------+
//              /\
//              ||   [ IC 8259B 0xA0 ]
//  	    +---++--+-------+-------+-------+-------+-------+-------+-------+
//      	| IRQ08 | IRQ09 | IRQ10 | IRQ11 | IRQ12 | IRQ13 | IRQ14 | IRQ15 |
//      	+-------+-------+-------+-------+-------+-------+-------+-------+
//
//  End of Interrupt is
//		When  Write is Interrupt Process Routine is Ended.
//		Where Write is Interrupt Controller Base Address.
//	So,	IRQ00 ~ IRQ07 is
//		Write IC_8259A, EOI
//	So, IRQ08 ~ IRQ15 is
//		Write IC_8259B, EOI
//		Write IC_8259A, EOI
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//	High Level Langage Hardware Interrupt Interface
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#include <DOS.H>
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
inline void intrrupt_close(unsigned char irqno, void interrupt (*oldisr)(...))
{
	outportb(IMR(irqno), inportb(IMR(irqno)) | MASKOFF(irqno));
	setvect(irqno, oldisr);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
inline void interrupt (*intrrupt_open(unsigned char irqno, void interrupt (*isr)(...)))(...)
{
	void interrupt (*oldisr)(...) = getvect(irqno);
	setvect(irqno,isr);
	outportb(IMR(irqno), inportb(IMR(irqno)) & MASKON(irqno));
	return	oldisr;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
inline void End_of_Interrupt(unsigned char irqno)
{
	if(irqno & 0xF0)	outportb(IC_8259B, EOI);
	outportb(IC_8259A, EOI);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#endif 	__HARDWARE_HPP__
