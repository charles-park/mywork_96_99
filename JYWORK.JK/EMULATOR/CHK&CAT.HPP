//[*]----------------------------------------------------------------------[*]
byte disp_chk(variable_t *var)
{
	watch();
	if(var->buf[(var->p_sp + var->size -1) % var->size] != 0xFE)    return 0;
	if(var->buf[(var->p_sp + var->size -2) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp             +1) % var->size] != 0xFF)    return 0;
	if(var->buf[(var->p_sp               ) % var->size] != 0xFF)    return 0;
																	return 1;
}
//[*]----------------------------------------------------------------------[*]
byte disp_cat(variable_t *var)
{
	byte    ck_xor = 0, cmd = 0, catch_id = 0, scode  = 0;
	word	value  = 0, i   = 0;

	watch();
	for(i = 2; i < var->size -3; i++)	{
		ck_xor ^= var->buf[(var->p_sp + i) % var->size];	watch();
	}
	if(ck_xor != var->buf[(var->p_sp + var->size -3) % var->size])  return 0;

	cmd        = var->buf[(var->p_sp +2) % var->size];
	if(cmd == 'P')	{
		if(chk_cfg.rom.lid != var->buf[(var->p_sp +5) % var->size])	return 0;
		for(i=0; i<var->size; i++)	{
			watch();	chk_cfg.bytes[i] = var->buf[(var->p_sp +i) % var->size];
		}	chk_cfg.rom.xor = 0x00;		led_setup(&chk_cfg);
	}	else	{
		if(cmd      != chk_cfg.rom.cmd)								return 0;
		scode        = var->buf[(var->p_sp +4) % var->size]    ;
		if(scode    != 0x00           )								return 0;
		catch_id     = var->buf[(var->p_sp +5) % var->size]    ;
		if(catch_id != chk_cfg.rom.id )								return 0;
		value  = var->buf[(var->p_sp +6 +chk_cfg.rom.did *2) % var->size]<<8;
		value |= var->buf[(var->p_sp +7 +chk_cfg.rom.did *2) % var->size]   ;
	}
	wind_pros (value);												return 0;
}
//[*]----------------------------------------------------------------------[*]
void wind_pros(word value)
{
	float	pros = 0.;

	pros = (float)value;	pros = pros / 10.;

	if     ((pros >=  11.25) && (pros <  33.75))    dot_puts("¦‚¦‚•·");  // NNE
	else if((pros >=  33.75) && (pros <  56.25))    dot_puts("¦‚  •·");  //  NE
	else if((pros >=  56.25) && (pros <  78.75))    dot_puts("•·¦‚•·");  // ENE
	else if((pros >=  78.75) && (pros < 101.25))    dot_puts("    •·");  //   E
	else if((pros >= 101.25) && (pros < 123.75))    dot_puts("•·q•·");  // ESE
	else if((pros >= 123.75) && (pros < 146.25))    dot_puts("q  •·");  //  SE
	else if((pros >= 146.25) && (pros < 168.75))    dot_puts("qq•·");  // SSE
	else if((pros >= 168.75) && (pros < 191.25))    dot_puts("    q");  //   S
	else if((pros >= 191.25) && (pros < 213.75))    dot_puts("qq¬á");  // SSW
	else if((pros >= 213.75) && (pros < 236.25))    dot_puts("q  ¬á");  //  SW
	else if((pros >= 236.25) && (pros < 258.75))    dot_puts("¬áq¬á");  // WSW
	else if((pros >= 258.75) && (pros < 281.25))    dot_puts("    ¬á");  //   W
	else if((pros >= 281.25) && (pros < 303.75))    dot_puts("¬á¦‚¬á");  // WNW
	else if((pros >= 303.75) && (pros < 326.25))    dot_puts("¦‚  ¬á");  //  NW
	else if((pros >= 326.25) && (pros < 348.75))    dot_puts("¦‚¦‚¬á");  // NNW
	else                                            dot_puts("    ¦‚");  //   N
}
//[*]----------------------------------------------------------------------[*]
