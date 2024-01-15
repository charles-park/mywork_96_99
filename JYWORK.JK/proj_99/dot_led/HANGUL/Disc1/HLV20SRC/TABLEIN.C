/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              Input Init                */
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

unsigned char _first_last[16][2]=
                        { {2,2},{3,3},{4,5},{5,8},{7,9},{8,17},{9,19},
                        {11,21},{12,22},{13,23},{14,24},{16,25},{17,26},
                        {18,27},{19,28},{20,29} };
unsigned char _middle2[7][3]=
                { {13,3,14},{13,4,15},{13,29,18},{20,7,21},{20,10,22},
                {20,29,23},{27,29,28} };
unsigned char _last2[11][3]=
                { {2,11,4},{5,14,6},{5,20,7},{9,2,10},{9,8,11},{9,9,12},
                {9,11,13},{9,18,14},{9,19,15},{9,20,16},{19,11,20}};

void hgHanInInit()
{
        first_last=&_first_last[0][0];
        middle2=&_middle2[0][0];
        last2=&_last2[0][0];
}
