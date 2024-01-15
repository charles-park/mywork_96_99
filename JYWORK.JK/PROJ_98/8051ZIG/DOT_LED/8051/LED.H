//[*]----------------------------------------------------------------------[*]
//   LED Dot Driver Header File
//[*]----------------------------------------------------------------------[*]
#ifndef	__LED__
#define	__LED__
#endif

#ifndef	H_FONT
	#define	H_FONT	0
#endif

#if   (H_FONT == 1)
	#include "MJ_FONT.HPP"
	const byte *Font_Addr1 = HANFONT1;
	const byte *Font_Addr2 = HANFONT2;
	const byte *Font_Addr3 = HANFONT3;
#elif (H_FONT == 2)
	#include "HANPIL.HPP"
	const byte *Font_Addr1 = HAN_PIL_1;
	const byte *Font_Addr2 = HAN_PIL_2;
	const byte *Font_Addr3 = HAN_PIL_3;
#elif (H_FONT == 3)
	#include "HANBOOT.HPP"
	const byte *Font_Addr1 = HAN_BOOT_1;
	const byte *Font_Addr2 = HAN_BOOT_2;
	const byte *Font_Addr3 = HAN_BOOT_3;
#elif (H_FONT == 4)
	#include "HANCHG2.HPP"
	const byte *Font_Addr1 = HANCHG_1;
	const byte *Font_Addr2 = HANCHG_2;
	const byte *Font_Addr3 = HANCHG_3;
#elif (H_FONT == 5)
	#include "HANCHG4.HPP"
	const byte *Font_Addr1 = HANCHG4_1;
	const byte *Font_Addr2 = HANCHG4_2;
	const byte *Font_Addr3 = HANCHG4_3;
#elif (H_FONT == 6)
	#include "HANSON.HPP"
	const byte *Font_Addr1 = HANSON_1;
	const byte *Font_Addr2 = HANSON_2;
	const byte *Font_Addr3 = HANSON_3;
#else
	#include "HANSOFT.HPP"
	const byte *Font_Addr1 = HANSOFT_1;
	const byte *Font_Addr2 = HANSOFT_2;
	const byte *Font_Addr3 = HANSOFT_3;
#endif

#ifndef	A_FONT
	#define	A_FONT 	1
#endif

#if  (A_FONT == 1)
	#include "ASCII.HPP"
	const byte (*Eng_Font)[16] = ENGFONT;
#endif
//[*]----------------------------------------------------------------------[*]
#define		watch()		P1.7 = 1 - P1.7
//[*]----------------------------------------------------------------------[*]
#define		ON			1
#define		OFF			0
//[*]----------------------------------------------------------------------[*]
#define		disable(y)	y = OFF
#define		enable(z)	z = ON
//[*]----------------------------------------------------------------------[*]
#define		COMMON		0xC000
#define		HI_DATA		0xA000
#define		LO_DATA		0x8000
//[*]----------------------------------------------------------------------[*]
typedef	struct hcode__t	{
	unsigned int	lasc	:	5;
	unsigned int	midc	:	5;
	unsigned int	firc	:	5;
	unsigned int	dummy	:	1;
}	hcode_t;
//[*]----------------------------------------------------------------------[*]
typedef	struct hchar__t	{
	unsigned char	fchar;
	unsigned char	schar;
}	hchar_t;
//[*]----------------------------------------------------------------------[*]
typedef union han__u	{
	hcode_t c;
	hchar_t b;
}	han_u;

static	han_u	han;

static	byte	hbuffer[100];
//[*]----------------------------------------------------------------------[*]
const byte sangyongtable[3][33] = {
	{	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
		0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01	},
	{	0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
		0x00, 0x00, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
		0x00, 0x00, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
		0x00, 0x00, 0x12, 0x13, 0x14, 0x15, 0x00, 0x00, 0x02	},
	{	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
		0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x00, 0x11, 0x12, 0x13, 0x14, 0x15,
		0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x00, 0x00, 0x01	}
};
//[*]----------------------------------------------------------------------[*]
const byte Dml[22] = { 0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1 															     };
const byte Dfm[40] = { 1, 3, 0, 2, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 0, 2, 1, 3, 1, 3, 1, 3 			 };
const byte Dmf[44] = { 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 1, 6, 3, 7, 3, 7, 3, 7, 1, 6, 2, 6, 4, 7, 4, 7, 4, 7, 2, 6, 1, 6, 3, 7, 0, 5 };
//[*]----------------------------------------------------------------------[*]
static 	byte 	*code_table[3];		// hangul code	pointer
//[*]----------------------------------------------------------------------[*]
static	word	Sunsor = 0;
static	byte	Common = 0;			
static	byte	Hi_lo  = 0;			
//[*]----------------------------------------------------------------------[*]
//   Function Prototype
//[*]----------------------------------------------------------------------[*]
interrupt [0x0B] 	void T0_int			(void);
					void variable_init	(void);
					void interrupt_init	(void);
					void putch_engfont	(char attrib, word index, byte hi_lo);
					void clear_buffer	(void);
//					void putch_engfont	(word index, byte hi_lo);
//					void putch_engfont	(int index);
//					void complete_hangul(char mode, unsigned char *dest, const unsigned char *src);
//					void putch_hanfont	(char *str);
					void complete_hangul(char attrib, char mode, unsigned char *dest, const unsigned char *src);
					void putch_hanfont	(char attrib, char *str);
					void main			(void);
//[*]----------------------------------------------------------------------[*]
