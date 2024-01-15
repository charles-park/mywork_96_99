/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Mouse Positioning            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

int get_mpos(int x,int y,int xw,int yw,int num)
{
        int i;
        int xx;
        int yy;

        hgGetMousePos(&xx,&yy);

        for (i=0;i<num;i++) {
                if ((xx>x && xx<(x+xw)) && (yy>y && yy<(y+yw)))
                        return i;
                y+=yw;
        }
        return -1;      /* for error */
}
int get_xwpos(int x[],int y,int xw[],int yw,int num)
{
        int i;
        int xx;
        int yy;

        hgGetMousePos(&xx,&yy);

        for (i=0;i<num;i++)
                if ((xx>x[i] && xx<(x[i]+xw[i])) && (yy>y && yy<(y+yw)))
                        return i;
        return -1;      /* for error */
}
