/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            2 bul Input Init            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

/* key index pointer */

extern unsigned char *key;
extern unsigned char *key_shifted;
extern unsigned char *first_last;
extern unsigned char *middle2;
extern unsigned char *last2;
extern unsigned char *key3_t;
extern unsigned char *key3_t_s;
extern unsigned char *key3;
extern unsigned char *key3_s;
extern unsigned char *first2;

#include <mem.h>
#include <alloc.h>

/* keydata */

unsigned char _key[26]=
                { 8,126,16,13,5,7,20,113,105,107,103,129,127,120,104,
                  110,9,2,4,11,111,19,14,18,119,17 };
unsigned char _key_shifted[26]=
                { 8,126,16,13,6,7,20,113,105,107,103,129,127,120,106,
                  112,10,3,4,12,111,19,15,18,119,17 };

void hg2bulInit()
{
        key=&_key[0];
        key_shifted=&_key_shifted[0];
}
