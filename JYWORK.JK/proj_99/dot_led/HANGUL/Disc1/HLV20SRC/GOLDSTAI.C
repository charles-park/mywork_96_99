/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*         GOLDSTAR Code Format           */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

extern char far goldstartable[3][33];
extern char *code_table[3];

void goldstar_code()
{
        code_table[0]=&goldstartable[0][0];
        code_table[1]=&goldstartable[1][0];
        code_table[2]=&goldstartable[2][0];
}
