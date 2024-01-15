/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         SANGYONG Code Format           */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

extern char far sangyongtable[3][33];
extern char *code_table[3];

void sangyong_code()
{
        code_table[0]=&sangyongtable[0][0];
        code_table[1]=&sangyongtable[1][0];
        code_table[2]=&sangyongtable[2][0];
}
