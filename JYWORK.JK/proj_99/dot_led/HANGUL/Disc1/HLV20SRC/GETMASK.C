/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Calculate Mask              */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

unsigned char get_lmask(int x)
{
	unsigned char mask=0xff;
        int r=x&7;

	mask>>=r;

	return mask;
}

unsigned char get_rmask(int x)
{
	unsigned char mask=0xff;
        int r=7-(x&7);

        mask<<=r;

        return mask;
}

unsigned char get_vmask(int x)
{
	unsigned char mask=0x80;
        int r=x&7;

	mask>>=r;

	return mask;
}
