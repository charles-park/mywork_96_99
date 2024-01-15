/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Hangul Output               */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

/* extern function */

extern void (*magh)(int x,int y,char *buff,char color);
extern void (*mage)(int x,int y,char *buff,char color);

/* function pointer */

void (*puth)(int x,int y,char *buff,char color);
void (*pute)(int x,int y,char *buff,char color);
void (*putm)(int x,int y,char *buff,int m,int n,char color);

void hgSetHanOut(void (*hout)(int x,int y,char *buff,char color))
{
	puth=hout;
        magh=hout;
}

void hgSetEngOut(void (*eout)(int x,int y,char *buff,char color))
{
	pute=eout;
        mage=eout;
}

void hgSetMagOut(void (*mout)(int x,int y,char *buff,int m,int n,char color))
{
        putm=mout;
}

void puthan(int x,int y,char *buff,char color)
{
        (*puth)(x,y,buff,color);
}

void puteng(int x,int y,char *buff,char color)
{
        (*pute)(x,y,buff,color);
}
void putmag(int x,int y,char *buff,int m,int n,char color)
{
        (*putm)(x,y,buff,m,n,color);
}
