/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*                 Plot                   */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

void (*plot)(int x,int y,char color);
char (*glot)(int x,int y);

void hgEnablePlot(void (*p)(int x,int y,char color))
{
        plot=p;
}
void hgPlotXy(int x,int y,char color)
{
        (*plot)(x,y,color);
}
void hgEnableGetPlot(char (*p)(int x,int y))
{
        glot=p;
}
char hgGetPlotXy(int x,int y)
{
        char ret;

        ret=(*glot)(x,y);
        return ret;
}
