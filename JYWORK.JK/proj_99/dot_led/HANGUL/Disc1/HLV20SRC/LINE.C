/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*                 Line                   */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

void (*lineg)(int x1,int y1,int x2,int y2,char color);

void hgEnableLine(void (*l)(int x1,int y1,int x2,int y2,char color))
{
        lineg=l;
}
void hgLine(int x1,int y1,int x2,int y2,char color)
{
        (*lineg)(x1,y1,x2,y2,color);
}
