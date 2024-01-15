/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Hangul Output               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

unsigned (*imgsize)(int x1,int y1,int x2,int y2);

void hgEnableImgSize(unsigned (*i)(int x1,int y1,int x2,int y2))
{
        imgsize=i;
}
unsigned hgImgSize(int x1,int y1,int x2,int y2)
{
        unsigned size;

        size=(*imgsize)(x1,y1,x2,y2);
        return size;
}
