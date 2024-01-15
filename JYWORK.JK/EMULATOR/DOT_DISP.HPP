//[*]----------------------------------------------------------------------[*]
void putch_engfont(word index, word start)
{
	word 	i, k;
	word	na = 0, mok = 0;
	byte 	*src;

	src = Eng_Font[index];

	if(start != 0)	{	mok = start / 32;	na = start % 32;	}
	else			{	mok = 0;			na = 0;				}

	if(na != 0)	{
		for(i=0, k=0; i<32; i++)	{
			if( (i%2))	hbuffer[mok *32 +i]  = src[k++];		watch();
		}
	}	else	{
		for(i=0, k=0; i<32; i++)	{
			if(!(i%2))	hbuffer[mok *32 +i]  = src[k++];		watch();
		}
	}
}
//[*]----------------------------------------------------------------------[*]
void complete_hangul(byte mode, word start, byte *dest, const byte *src)
{
	int i;

	if(mode)	for(i=start; i<start+32; i++)	{	dest   [i]  = src[i - start];	watch();	}
	else		for(i=start; i<start+32; i++)	{	dest   [i] |= src[i - start];	watch();	}
}
//[*]----------------------------------------------------------------------[*]
void dot_puts(byte *str)
{
	chk_han_u	chk_han;
	int			i  = 0;
	int 		fcode , mcode , lcode ;
	int 		f1 = 0, f2 = 0, f3 = 0;
	char 		hflag    = 1;

	Start_addr = 0x0;

	while(str[i] != 0x00)	{
		watch();	chk_han.byte_data = str[i];

		if(chk_han.bits.flag)	{
			han.b.fchar = str[i++];	han.b.schar = str[i++];

			fcode  = code_table[0][han.c.firc];
			mcode  = code_table[1][han.c.midc];
			lcode  = code_table[2][han.c.lasc];

			f3 = Dml[mcode];
			f2 = Dfm[fcode*2+(lcode!=0)];
			f1 = Dmf[mcode*2+(lcode!=0)];

			if(fcode){	complete_hangul(     1, Start_addr, hbuffer, Font_Addr1+(f1*16+fcode+f1*4)*32);	hflag = 0;	}
			if(mcode){	complete_hangul( hflag, Start_addr, hbuffer, Font_Addr2+(f2*22+mcode     )*32);	hflag = 0;	}
			if(lcode){	complete_hangul( hflag, Start_addr, hbuffer, Font_Addr3+(f3*32+lcode-f3*4)*32);	hflag = 0;	}
			Start_addr += 32;
		}	else	{
			putch_engfont(str[i++], Start_addr);	Start_addr += 16;
		}
		if(Start_addr >= 96)		return;
	}
}
//[*]----------------------------------------------------------------------[*]


