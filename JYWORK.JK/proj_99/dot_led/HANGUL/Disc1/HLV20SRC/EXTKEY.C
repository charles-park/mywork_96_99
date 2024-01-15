/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*               Exit Key                 */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <alloc.h>

/* exit key */

int *ext_key;
int num_ext_key;
int ext_enable=1;

/* disable key */

int *disable_key;
int num_disable_key;
int disable_enable=1;

void hgSetExtKey(int *a)
{
	int p=0;

        while (a[p]) p++;
	num_ext_key=p;

        ext_key=(int *)malloc((size_t)p*2);

	p=0;
        while (a[p]) {
                ext_key[p]=a[p];
                p++;
        }
}

int *hgGetExtKey()
{
        return ext_key;
}

int hgGetNumExtKey()
{
        if (!ext_enable) return 0;     /* for disable */
        return num_ext_key;
}

void hgFreeExtKey()
{
	free(ext_key);
        num_ext_key=0;
}
void hgSetExtKeyOn()
{
        ext_enable=1;
}
void hgSetExtKeyOff()
{
        ext_enable=0;
}

void hgSetDisableKey(int *a)
{
	int p=0;

        while (a[p]) p++;
        num_disable_key=p;

        disable_key=(int *)malloc((size_t)p*2);

	p=0;
        while (a[p]) {
                disable_key[p]=a[p];
                p++;
        }
}

int *hgGetDisableKey()
{
        return disable_key;
}

int hgGetNumDisableKey()
{
        if (!disable_enable) return 0;     /* for disable */
        return num_disable_key;
}

void hgFreeDisableKey()
{
        free(disable_key);
        num_disable_key=0;
}
void hgSetDisableKeyOn()
{
        disable_enable=1;
}
void hgSetDisableKeyOff()
{
        disable_enable=0;
}
