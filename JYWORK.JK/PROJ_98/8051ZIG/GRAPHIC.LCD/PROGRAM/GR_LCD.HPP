//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
static	void my_put_c_in_string(byte c, void *ptr)
{
	*(*(char **) ptr)++ = c;
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_printf(byte x, byte y, byte pg, const byte *format, ...)
{
	va_list		ap;
	word		cnt, y = 0x0001;
	byte		buf[129], fnt, i, j, k, addr = 0, t = 0x80;
	byte		*pointer = &buf[0];

	watch();
	if(x  >= GR_LCD_X   )			return;
	if(y  >= GR_LCD_Y   )			return;
	if(pg >= GR_LCD_PAGE)			return;

	for(i=0 ; i<129; i++)			buf[i] = ' ';

	va_start(ap, format);
	cnt = _formatted_write(format, my_put_c_in_string, (void *) &pointer, ap);
	va_end(ap);

	for(i=0; i < cnt; i++)	{
		if((y + (i * 8)) < GR_LCD_Y)	{
			for(j=0; j<8; j++)	{
				conv.word_data = 0;
				for(k=0; k<16; k++)	{
					fnt = EngFont[buf[i]][k];
					if(fnt & t)		conv.word_data |= y;
					y = y <<1;
				}
				gr_lcd_buffer.disp[x    ][y + i + addr)][pg] = conv.byte_data.b2;
				gr_lcd_buffer.disp[x + 1][y + i + addr)][pg] = conv.byte_data.b1;
				y = 0x0001;				t = t >>1;
				addr ++;				watch();
			}
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_buffer_clear(byte pg)
{
	for(j=0; j<GR_LCD_X; j++)	{
		for(i=0; i<GR_LCD_Y; i++)	{
			gr_lcd_buffer.disp[j][i][pg] = ' ';		watch();
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_onoff(byte cs, byte status)
{
	if(cs > 1)							return;
	if(cs)		{
		if(status)		output(GR_LCD_CNT1, 0x3F);
		else			output(GR_LCD_CNT1, 0x3E);
	}	else	{
		if(status)		output(GR_LCD_CNT0, 0x3F);
		else			output(GR_LCD_CNT0, 0x3E);
	}
	delay(10);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
byte gr_lcd_status_read(byte cs)
{
	if(cs > 1)						return 0xFF;
	if(cs)	return	(byte)(input(GR_LCD_STA1));
	else	return	(byte)(input(GR_LCD_STA0));
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_reset(void)
{
	GR_LCD_RST(OFF);	delay(100);
	GR_LCD_RST(ON); 	delay(100);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_set_disp_line(byte cs, byte line)
{
	if(line >= GR_LCD_LINE)		return;
	if(cs > 1)					return;
	if(cs)	{
		output(GR_LCD_CNT1, 0xC0 + line );	delay(10);	// display line
	}	else	{
		output(GR_LCD_CNT0, 0xC0 + line );	delay(10);	// display line
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_set_page(byte cs, byte pg)
{
	if(pg >= GR_LCD_X)			return;
	if(cs > 1)					return;
	if(cs)						output(GR_LCD_CNT1, 0xB8 + pg);
	else                        output(GR_LCD_CNT0, 0xB8 + pg);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_set_yline(byte yline)
{
	if(yline >= GR_LCD_Y)		return;
	if(yline >= GR_LCD_LINE)	output(GR_LCD_CNT1, 0x40 + (yline % GR_LCD_LINE));
	else						output(GR_LCD_CNT0, 0x40 +  yline               );
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_put_data(byte addr, byte p_data)
{
	graphic_lcd_set_yline(addr);
	if(addr >= GR_LCD_Y)		return;
	if(addr >= GR_LCD_LINE)		output(GR_LCD_DATA1, p_data);
	else						output(GR_LCD_DATA0, p_data);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_clear(void)
{
	byte	addr, pg;

	graphic_lcd_onoff(0, OFF);
	graphic_lcd_onoff(1, OFF);
	for(pg = 0; pg <= GR_LCD_X; pg++)	{
		graphic_lcd_set_page(0, pg);
		graphic_lcd_set_page(1, pg);
		for(addr = 0; addr < GR_LCD_Y; addr++)	{
			graphic_lcd_put_data(addr, 0x0);
		}
	}
	graphic_lcd_set_page(0, 0);
	graphic_lcd_set_page(1, 0);
	graphic_lcd_onoff(0, ON);
	graphic_lcd_onoff(1, ON);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_init(void)
{
	word	i;

	// Software Clear..
	gr_lcd_buffer.x        = 0;
	gr_lcd_buffer.y        = 0;			// line control
	gr_lcd_buffer.d_page   = 0;			// display page
	gr_lcd_buffer.new_page = OFF;		// new page flag
	for(i=0; i<GR_LCD_PAGE; i++)	gr_lcd_buffer_clear(i)

	// Hardware Clear..
	gr_lcd_reset();
	gr_lcd_set_disp_line(0, 0);
	gr_lcd_set_disp_line(1, 0);
	gr_lcd_set_page(0, 1);
	gr_lcd_set_page(1, 1);
	gr_lcd_clear();

//	TMOD = 0x01;	TH0 = 0xFF;		TL0 = 0;
//	enable(ET0);	enable(TR0);	enable(EA);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
void gr_lcd_test(void)
{
	word 	ap = 0;
	graphic_lcd_init();

	while(1)	{
		for(ap = 0; ap < 255; ap++)	{
			watch();
			gr_lcd_clear();		gr_lcd_buffer_clear(0);
			gr_lcd_printf(0, 0, 0, "DATA %c", (byte)ap);
			gr_lcd_printf(2, 0, 0, "DATA %c", (byte)ap);
			gr_lcd_printf(4, 0, 0, "DATA %c", (byte)ap);
			gr_lcd_printf(6, 0, 0, "DATA %c", (byte)ap);
			delay(5000);
		}
	}
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
interrupt [0x0B] void T0_int (void)
{
	disable(TR0);	disable(TF0);
	switch(Sunsor++)	{
		case	1:	if(gr_lcd_buffer.y >= (GR_LCD_Y / 2))
						gr_lcd_set_page(1, gr_lcd_buffer.x);
					else
						gr_lcd_set_page(0, gr_lcd_buffer.x);
					break;
		case	2:	gr_lcd_set_yline(gr_lcd_buffer.y);
					break;
		case	3:	gr_lcd_put_data (gr_lcd_buffer.y, gr_lcd_buffer.disp[gr_lcd_buffer.x][gr_lcd_buffer.y][gr_lcd_buffer.d_page]);
					break;
		default	 : 	Sunsor = 0;
					if(++gr_lcd_buffer.y >= GR_LCD_Y)	{
						gr_lcd_buffer.y = 0;
						if(++gr_lcd_buffer.x >= GR_LCD_X)	{
							gr_lcd_buffer.x = 0;
							if(gr_lcd_buffer.new_page)	{
								gr_lcd_buffer.d_page++;	gr_lcd_buffer.new_page = OFF;
								if(gr_lcd_buffer.d_page >= GR_LCD_PAGE)	gr_lcd_buffer.d_page = 0;
							}
						}
					}
					break;
	}
	TH0 = 0xFF;		TL0 = 0x00;
	enable(TR0);
}
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//typedef struct gr_lcd_buffer__t	{
//	byte		x;
//	byte		y;
//	byte		d_page;
//	byte		new_page;
//	byte
//}	gr_lcd_buffer_t;

//static	gr_lcd_buffer_t		gr_lcd_buffer;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//void gr_lcd_set_page (byte cs, byte pg);
//void gr_lcd_set_yline(byte yline);
//void gr_lcd_put_data (byte addr, byte p_data);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//   Function define..
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//static	void my_put_c_in_string(byte c, void *ptr);
//void gr_lcd_printf(byte x, byte y, byte pg, const byte *format, ...);
//void gr_lcd_buffer_clear(byte pg);
//void gr_lcd_onoff(byte cs, byte status);
//byte gr_lcd_status_read(byte cs);
//void gr_lcd_reset(void);
//void gr_lcd_set_disp_line(byte cs, byte line);
//void gr_lcd_set_page(byte cs, byte pg);
//void gr_lcd_set_yline(byte yline);
//void gr_lcd_put_data(byte addr, byte p_data);
//void gr_lcd_clear(void);
//void gr_lcd_init(void);
//void gr_lcd_test(void);
//interrupt [0x0B] void T0_int (void);
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
