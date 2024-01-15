/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Font management              */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <stdio.h>

#include "hghlib.h"

extern char *fir;
extern char *mid;
extern char *las;
extern char *spe;
extern char *eng;

extern HTYPE htypes;
extern ETYPE etypes;

void hgSetHanFontRoom(int htype)
{
	int hsizef[4]={ 8*20*32,6*20*32,10*20*32,2*20*32 };
	int hsizem[4]={ 4*22*32,2*22*32, 4*22*32,1*22*32 };
	int hsizel[4]={ 4*28*32,1*28*32, 4*28*32,2*28*32 };

	fir=(char *)malloc((size_t)hsizef[htype]);
	mid=(char *)malloc((size_t)hsizem[htype]);
	las=(char *)malloc((size_t)hsizel[htype]);
}
void hgSetEngFontRoom(int etype)
{
        int esize [1]={ 256*16 };  /* 16 is dummy */

        eng=(char *)malloc((size_t)esize [etype]);
}
void hgSetSpecialFontRoom(int stype)
{
        int ssize    =  128*32;

        stype&=0xaa;
        spe=(char *)malloc((size_t)ssize);
}
void hgFreeHanRoom()
{
        free(fir);
        free(mid);
        free(las);

        fir=mid=las=NULL;
}
void hgFreeEngRoom()
{
        free(eng);

        eng=NULL;
}
void hgFreeSpecialRoom()
{
        free(spe);

        spe=NULL;
}

void hgLoadHanFont(char *hname,int htype)
{
        int hsizef[4]={ 8*20*32,6*20*32,10*20*32,2*20*32 };
        int hsizem[4]={ 4*22*32,2*22*32, 4*22*32,1*22*32 };
        int hsizel[4]={ 4*28*32,1*28*32, 4*28*32,2*28*32 };
	int handle;

        htypes.firs=hsizef[htype];
        htypes.mids=hsizem[htype];
        htypes.lass=hsizel[htype];

        handle=open(hname,O_RDWR|O_BINARY);

        read(handle,fir,hsizef[htype]);
        read(handle,mid,hsizem[htype]);
        read(handle,las,hsizel[htype]);

        close(handle);

        hgSetCtable(htype);
}

void hgLoadEngFont(char *ename,int etype)
{
	int esize [1]={ 256*16 };
	int handle;

	etypes.engs=esize[etype];

	handle=open(ename,O_RDWR|O_BINARY);
	read(handle,eng,esize[etype]);
	close(handle);
}

void hgLoadSpecialFont(char *sname,int stype)
{
        int ssize=128*32;
	int handle;

        stype&=0xaa;
        handle=open(sname,O_RDWR|O_BINARY);
        read(handle,spe,ssize);
	close(handle);
}
void hgSetHanFont(void (*hf)(),int htype)
{
        (*hf)();
        hgSetCtable(htype);
}
void hgSetEngFont(void (*ef)(),int etype)
{
        (*ef)();
        etype&=0xaa;    /* for remove warning */
}
void hgSetSpecialFont(void (*sf)(),int stype)
{
        (*sf)();
        stype&=0xaa;    /* for remove warning */
}
