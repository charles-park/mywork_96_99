#include <io51.h>
#include <stdio.h>
//#include "ASCII.HPP"
//#include "HAN.HPP"
#include "FONT.HPP"

#define		watch()		P1.7 = 1 - P1.7
//[*]----------------------------------------------------------------------[*]
typedef struct	han_bits__t	{
	word	last	:	5;
	word	middle	:	5;
	word	first	:	5;
	word	status	:	1;
}	han_bits_t;

typedef	struct	han_byte__t	{
	byte	lsb;
	byte	msb;
}	han_byte_t;

typedef	union	han__u	{
	han_bits_t	bits ;
	han_byte_t	bytes;
}	han_u;

static	han_u	han;
//[*]----------------------------------------------------------------------[*]
static	byte	disp_buf[33];
//[*]----------------------------------------------------------------------[*]
//   HanGul Table ( 8 X 4 X 4 : FONT )
//[*]----------------------------------------------------------------------[*]
// cho
const	byte	_dmf[22][2] = {
	0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 1, 6, 3, 7,
	3, 7, 3, 7, 1, 6, 2, 6, 4, 7, 4, 7, 4, 7, 2, 6, 1, 6, 3, 7, 0, 5
};

// jung
const	byte	_dfm[20][2] = {
	1, 3, 0, 2, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3,
	1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 0, 2, 1, 3, 1, 3, 1, 3
};

// jong
const	byte	_dml[22]    = {
	0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1
};
//[*]----------------------------------------------------------------------[*]
void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)	{	watch();	}
}
//[*]----------------------------------------------------------------------[*]
void buffer_write(byte mode, byte *w_buf, byte *m_buf)
{
	word	i;

	for(i=0; i<32; i++)	{
		watch();
		if(mode)	w_buf[i]  = m_buf[i];
		else		w_buf[i] |= m_buf[i];
	}
}
//[*]----------------------------------------------------------------------[*]
void han_put(byte *str)
{
	byte	lst, mid, fir;

//	han.bytes.msb = str[0];		han.bytes.lsb = str[1];
	han.bytes.msb = str[1];		han.bytes.lsb = str[0];

//	lst = _dml[han.bits.last  ];
//	mid = _dfm[han.bits.first ][(han.bits.last != 0)];
//	fir = _dmf[han.bits.middle][(han.bits.last != 0)];
	lst = _dml[han.bits.middle];
	mid = _dfm[han.bits.first ][(han.bits.last != 0)];
	fir = _dmf[han.bits.middle][(han.bits.last != 0)];

//	buffer_write(1, disp_buf, &HANFONT1[32]);
//	buffer_write(0, disp_buf, &HANFONT2[32]);
// 	buffer_write(0, disp_buf, &HANFONT3[32]);
	buffer_write(1, disp_buf, &HANFONT1[fir * 32]);
	buffer_write(0, disp_buf, &HANFONT2[mid * 32]);
	buffer_write(0, disp_buf, &HANFONT3[lst * 32]);
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	word	i = 0;
	word	j = 0;

	han_put("‹±");
	while(1)	{
		watch();
		output(0xA000, ~disp_buf[j  ]);	output(0x8000, ~disp_buf[j+1]);
		output(0xC000, i   );				delay(10);
		output(0xC000, 0xFF);				delay(1);

		if(++i >= 16)			i = 0;

		j += 2;
		if(j >= 32)             j = 0;
	}
}
//[*]----------------------------------------------------------------------[*]
/*
// bul, num, bytes
const byte _han_fntHF1[8][20][32] = {
const byte _han_fntHF2[4][22][32] = {
const byte _han_fntHF3[4][28][32] = {
void main(void)
{
	word	i = 0;
	word	j = 0;
	word	k = 0;


	while(1)	{
		watch();
		comm.bytes.msb = EngFont[k%256][i];
		comm.bytes.lsb = EngFont[k%256][i];

		output(0x8000, ~comm.bytes.msb);	output(0xA000, ~comm.bytes.lsb);
		output(0xC000, i   );				delay(10);
		output(0xC000, 0xFF);				delay(1);

		if(++i >= 16)			i = 0;
		if(++j > 1000)		{	j = 0;	k++;	}
	}
}
//[*]----------------------------------------------------------------------[*]
void delay(word count)
{
	word	i;
	for(i=0; i<count; i++)	{	watch();	output(0xC000, i);	}
}
//[*]----------------------------------------------------------------------[*]
void main(void)
{
	word	i = 0;

	while(1)	{
		output(0x8000, 0x55);		output(0xA000, 0x55);	delay(50000);
		output(0x8000, 0xAA);		output(0xA000, 0xAA);   delay(50000);
	}
}
*/

//[*]----------------------------------------------------------------------[*]





