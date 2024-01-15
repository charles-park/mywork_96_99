/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*              "Attribute"               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <dos.h>
#include <mem.h>

char hgEAttr=1;          /* 1 = NORMAL */
char hgHAttr=1;          /* 1 = NORMAL */
char hgExtAttr=1;        /* 1 = NORMAL */

void attr_null(char *dest,int bytes);

void (*afunc[7])(char *dest,int bytes)=
	{ attr_null,attr_null,attr_null,attr_null,
	  attr_null,attr_null,attr_null };

void attr_null(char *dest,int bytes)    /* null function */
{
        bytes&=dest[0];         /* I hate warning!! */
}

void hgSetEAttr(char attr)
{
        hgEAttr=attr;
}

char hgGetEAttr()
{
        return hgEAttr;
}

void hgSetHAttr(char attr)
{
        hgHAttr=attr;
}

char hgGetHAttr()
{
        return hgHAttr;
}
void hgSetExtAttr(char attr)
{
        hgExtAttr=attr;
}

char hgGetExtAttr()
{
        return hgExtAttr;
}

void hgDoAttr(char *dest,int bytes,int n)
{
	(*afunc[n])(dest,bytes);
}

void hgEnableAttr(void (*func)(char *dest,int bytes),int n)
{
        afunc[n]=func;
}
