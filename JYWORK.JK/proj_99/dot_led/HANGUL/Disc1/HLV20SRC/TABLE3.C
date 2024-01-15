/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           3 bul Input Init             */
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

unsigned char _key3_t[]={ '1','2','3','4','5','6','7','8','9','0',
                         'Q','W','E','R','T','Y','U','I','O','P',
                         'A','S','D','F','G','H','J','K','L',';','\'',
                         'Z','X','C','V','B','N','M',',','.','/'};
unsigned char _key3_t_s[]={ '!','@','#','$','%','^','&','*','(',')',
                         'Q','W','E','R','T','Y','U','I','O','P',
                         'A','S','D','F','G','H','J','K','L',':','"',
                         'Z','X','C','V','B','N','M','<','>','?'};
unsigned char _key3[]=  { 229,222,219,119,126,105,112,128,120,17,
                         221,209,111,104,107,7,5,8,16,19,
                         223,205,129,103,127,4,13,2,14,9,18,
                         217,202,110,113,120,11,20,0,0,113 };
unsigned char _key3_s[]={ 224,0,0,0,0,0,0,0,0,0,
                         228,227,226,106,';','<','7','8','9','>',
                         208,207,210,203,'/',',','4','5','6',':','"',
                         225,220,211,216,'!','0','1','2','3',0};
unsigned char _first2[][3]=
                {{2,2,3},{5,5,6},{9,9,10},{11,11,12},{14,14,15}};

void hg3bulInit()
{
        key3_t=&_key3_t[0];
        key3_t_s=&_key3_t_s[0];
        key3=&_key3[0];
        key3_s=&_key3_s[0];
        first2=&_first2[0][0];
}
