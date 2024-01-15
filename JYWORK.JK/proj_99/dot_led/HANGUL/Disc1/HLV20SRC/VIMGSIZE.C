/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         VGA calculate imagesize        */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

unsigned hgVImgSize(int x1,int y1,int x2,int y2)
{
	unsigned size;
	int xs=x1/8+1;
	int xe=x2/8-1;

        size=(xe-xs+1)*(y2-y1+1)*4;
        size+=((y2-y1+1)*2*4);          /* for side        */
        size+=2;                        /* for eight masks */
	return size;
}
