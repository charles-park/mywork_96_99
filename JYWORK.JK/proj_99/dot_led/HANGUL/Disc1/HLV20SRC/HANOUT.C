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

#pragma inline

#include <alloc.h>
#include <mem.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <stdarg.h>
#include <stdio.h>

#define SPECIAL_CODE            212

#include "hginit.h"
#include "hghlib.h"

char *Dml;               /* convert table ( middle-last  ) */
char *Dfm;               /* convert table ( first-middle ) */
char *Dmf;               /* convert table ( middle-first ) */

char *code_table[3];

char *fir;		/* first 	*/
char *mid;		/* mid   	*/
char *las;		/* las   	*/
char *spe;              /* special font */
char *eng;		/* english 	*/

HTYPE htypes;
ETYPE etypes;

union {
        hcode c;
        hchar b;
} han;

/* general x,y position */

int _xpos=0;
int _ypos=0;

/* flags */

char engflag=0;
char specialenable=0;    /* special character not enabled */
char graphokflag=0;      /* graphic character not enabled */
char hanjaokflag=0;      /* hanjaokflag not enabled       */
char textattrflag=0;     /* textattribute not enabled     */

/* prototype */

void specialout_process(char *str,int pos);
void englishout_process(char *str,int pos);
void hangulout_process(char *str,int pos);
void hanjaout_process(char *str,int pos);
void graphout_process(char *str,int pos);
void textattr_process(char *str,int *pos);

/* function pointer for hanja_process */

void hanja_null_function(char *str,int pos);
void pos_null_function(char *str,int *pos);

void (*hjps)(char *str,int pos)=&hanja_null_function;
void (*grps)(char *str,int pos)=&hanja_null_function;
void (*atps)(char *str,int *pos)=&pos_null_function;

void complete_hangul(char mode, char *dest, char *src,char num);

void hanja_null_function(char *str,int pos)
{
        hangulout_process(str,pos);
}
void pos_null_function(char *str,int *pos)
{
        englishout_process(str,*pos);
        (*pos)++;
}

int  ishanja(char *str,int pos)
{
        unsigned char hc1,hc2;

        hc1=str[pos];
        hc2=str[pos+1];

        if ((hc1>=0xe0 && hc1<=0xfa) &&
           ( (hc2>=0x31 && hc2<=0x7e) ||
             (hc2>=0x91 && hc2<=0xfe) ) )

        return hgSUCCESS;
        else   return hgFAIL;
}
int  isgraphic(char *str,int pos)
{
        unsigned fontID[]={ 0xd480,0xd931,0xda31,0xdb31,0xdc31,0xdd31,0xde31 };
        unsigned char hc1,hc2;
        int      i;

        for (i=0;i<7;i++) {
                hc1=(fontID[i]>>8)&0xff;
                hc2=(fontID[i]&0xff);
                if (((unsigned char)str[pos])==hc1 && ((unsigned char)str[pos+1])>=hc2)
                         return hgSUCCESS;
        }
        return hgFAIL;
}
int isspecial(char *str,int pos)
{
        if ((unsigned char)str[pos]==SPECIAL_CODE && (unsigned char)str[pos]>=0x80)
                return hgSUCCESS;
        else
                return hgFAIL;
}
void hgOutTextXy(int x,int y,char *str)
{
	int pos=0;	/* string position */
        int flag;

	_xpos=x;
	_ypos=y;

	while (str[pos]!=0) {
                if (str[pos]=='/') textattr_process(str,&pos);
                else if (str[pos]=='\t') {
                        _xpos=_xpos+(8-(_xpos/8)%8)*8;
			pos++;
                } else if (!(str[pos]&0x80)) {
                        englishout_process(str,pos);
                        pos++;
                } else if (specialenable && isspecial(str,pos)) {
                        specialout_process(str,pos);
                        pos+=2;
                } else if (graphokflag && isgraphic(str,pos)) {
                        graphout_process(str,pos);
                        pos+=2;
                } else if (hanjaokflag && ishanja(str,pos)) {
                        hanjaout_process(str,pos);
                        pos+=2;
                } else {
                        hangulout_process(str,pos);
			pos+=2;
		}
	};
}

void attribute_process(char *dest,int bytes)
{
        char attr;

        if (bytes==16) attr=hgGetEAttr();
        else if (bytes==32) attr=hgGetHAttr();
        else if (bytes<0) {
                attr=hgGetExtAttr();
                bytes=-bytes;
        }

        if (attr==NORMAL) return;    /* for NORMAL attribute */

        if (attr & REVERSE) hgDoAttr(dest,bytes,0);
        if (attr & DIM    ) hgDoAttr(dest,bytes,1);
        if (attr & SHADOW ) hgDoAttr(dest,bytes,2);
	if (attr & THREED ) hgDoAttr(dest,bytes,3);
        if (attr & OUTLINE) hgDoAttr(dest,bytes,4);
        if (attr & BOLD   ) hgDoAttr(dest,bytes,5);
        if (attr & UNDER  ) hgDoAttr(dest,bytes,6);
}

void englishout_process(char *str,int pos)
{
	char ebuffer[16];

        engflag=1;
        memcpy(ebuffer,eng+((unsigned char)str[pos])*16,16);
	attribute_process(ebuffer,16);
	puteng(_xpos,_ypos,ebuffer,hgGetTFcolor());
	_xpos+=8;
}

void specialout_process(char *str,int pos)
{
        char sbuffer[32];

        memcpy(sbuffer,spe+((unsigned char)str[pos+1]-128)*32,32);
        attribute_process(sbuffer,32);
        puthan(_xpos,_ypos,sbuffer,hgGetTFcolor());
        _xpos+=16;
}
void hanjaout_process(char *str,int pos)
{
        (*hjps)(str,pos);               /* function pointer */
}
void graphout_process(char *str,int pos)
{
        (*grps)(str,pos);               /* function pointer */
}
void textattr_process(char *str,int *pos)
{
        (*atps)(str,pos);
}
void hangulout_process(char *str,int pos)
{
	int fcode,mcode,lcode;
	int f1=0,f2=0,f3=0;
	int hflag;
	char hbuffer[32];

	han.b.fchar=str[pos];
	han.b.schar=str[pos+1];

	fcode  = code_table [ 0 ] [ han.c.firc ];
	mcode  = code_table [ 1 ] [ han.c.midc ];
	lcode  = code_table [ 2 ] [ han.c.lasc ];

        /*
        if (mcode) f3=Dml[mcode];
                   f2=Dfm[fcode*2+(lcode!=0)];
        if (mcode) f1=Dmf[mcode*2+(lcode!=0)];
        if ((mcode == 0) && (lcode == 0)) f1=0;
        */

        f3=Dml[mcode];
        f2=Dfm[fcode*2+(lcode!=0)];
        f1=Dmf[mcode*2+(lcode!=0)];

	hflag = 1;
        engflag=0;

	if (fcode){
                complete_hangul(1, hbuffer,fir+(f1*16+f1*4+fcode)*32,16);
		hflag = 0;
	}
	if (mcode){
                complete_hangul(hflag, hbuffer,mid+(f2*22+mcode)*32,16);
		hflag = 0;
	}
	if (lcode){
                complete_hangul(hflag, hbuffer,las+(f3*32-f3*4+lcode)*32,16);
		hflag = 0;
	}
	attribute_process(hbuffer,32);
	puthan(_xpos,_ypos,hbuffer,hgGetTFcolor());
	_xpos+=16;
}

/* copy src to dest.. if mode 1 , first clear dest */

void complete_hangul(char mode, char *dest, char *src,char nums)
{
	    char mtemp=mode;
	    char mnumb=nums;

	    asm push ds
            asm mov cl,BYTE PTR mnumb
            asm xor ch,ch
            asm les di,dest
            asm lds si,src
	    asm cmp BYTE PTR mtemp,1
	    asm jnz imgnext2
            asm cld
            asm REP movsw
	    asm pop ds

	    return;

imgnext2:   asm lodsw
            asm or es:[di],ax
            asm inc di
            asm inc di
	    asm loop imgnext2
	    asm pop ds
}

void hgOutText(char *str)
{
        hgOutTextXy(_xpos,_ypos,str);
}

void hgPrintfXy(int x,int y,char *fmt,...)
{
        char s[255];

        va_list argptr;
        va_start(argptr,fmt);
        vsprintf(s,fmt,argptr);

        hgOutTextXy(x,y,s);
}

void hgPrintf(char *fmt,...)
{
	hgPrintfXy(_xpos,_ypos,fmt);
}

void hgGotoXy(int x,int y)
{
	_xpos=x;
	_ypos=y;
}
int  hgGetOutX()
{
        return _xpos;
}
int  hgGetOutY()
{
        return _ypos;
}
