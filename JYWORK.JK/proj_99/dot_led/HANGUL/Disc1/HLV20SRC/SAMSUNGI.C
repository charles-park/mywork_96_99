/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          SAMSUNG Code Format           */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

extern char far samsungtable[3][33];
extern char *code_table[3];

void samsung_code()
{
        code_table[0]=&samsungtable[0][0];
        code_table[1]=&samsungtable[1][0];
        code_table[2]=&samsungtable[2][0];
}
