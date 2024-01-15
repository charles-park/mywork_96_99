/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Image Size cal.             */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

unsigned (*imgsize4)(int x1,int y1,int x2,int y2);

void hgEnableImgSize4(unsigned (*i)(int x1,int y1,int x2,int y2))
{
        imgsize4=i;
}
unsigned hgImgSize4(int x1,int y1,int x2,int y2)
{
        unsigned size;

        size=(*imgsize4)(x1,y1,x2,y2);
        return size;
}
