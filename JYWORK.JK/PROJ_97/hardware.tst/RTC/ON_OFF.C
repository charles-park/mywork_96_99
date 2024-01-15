
/* Display ON/OFF Control DATA Write to LCD         */
/* RS  R/W  D7  D6  D5  D4  D3  D2  D1  D0          */
/* 0   0    0   0   0   0   1   D   C   B           */
/* D = All Display ON/OFF, C = Cursor ON/OFF, B = Cursor Position BLINK ON/OFF */

#define		ON		0x0d
#define		OFF		0x0c

void Cursor_OnOff(Byte on_off, Byte x, Byte y)
{
	delay(100);
	if(on_off == ON)	lcd_disp(x,y);
	output(LCD_CON, on_off & 0xf);

}

void main(void)
{
	Cursor_OnOff(ON, 0, 0);
	Cursor_OnOff(OFF, 0, 0);
}