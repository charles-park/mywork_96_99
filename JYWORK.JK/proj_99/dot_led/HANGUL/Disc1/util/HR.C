/*========================================*/
/*                                        */
/*             MULBORA SHELL              */
/*                                        */
/*             Hangul Viewer              */
/*                Program                 */
/*                                        */
/*========================================*/

/* include file */

#include <stdio.h>
#include <alloc.h>
#include <mem.h>
#include <dir.h>
#include <string.h>
#include <io.h>
#include <stdlib.h>

#include "hghlib.h"
#include "hginit.h"
#include "hgquick.h"
#include "key.h"

#define MAXBUFF         255
#define MAXINDEX        255
#define BUFFSIZE        0x8000
#define MAXCOLUMN       611

#define hgSUCCESS         1
#define hgFAIL            0

int  MAXROW=27;
int  _H;

VMEM *buff [MAXBUFF ];
byte *index[MAXINDEX];
unsigned *freespace;

byte     buffnum=-1;    /* for adding */
byte     nowbuff= 0;
unsigned indexnum=-1;   /* for adding */
unsigned offset;                /* offset in buff  */
unsigned ioffset;               /* offset in index */
unsigned long tlines=0; /* total lines */

byte fname[30];         /* global file name for status */

/* for speed erase */

byte elen[28];          /* max 28 line */

/* prototype */

char fileload(char *fname);
void initial(void);
char makenewbuffer(void);
char makenewindex(void);
int  isfreebuff(unsigned length);
int  isfreeindex(void);
void addline(char *str);
void updateindex(char buffnum,unsigned offset);
void putword(byte *buff,unsigned offset,unsigned data);
void putbyte(byte *buff,unsigned offset,byte data);
void freeprocess(void);
void printstring(int x,int y,byte *str,unsigned offset);
void viewprocess(void);

extern void veoute(int x,int y,char *buff,char color);
extern void veouth(int x,int y,char *buff,char color);
extern void heoute(int x,int y,char *buff,char color);
extern void heouth(int x,int y,char *buff,char color);

char fileload(char *fname)
{
        FILE *fpt;
        char temp[255];

        if ((fpt=fopen(fname,"r"))==NULL) return hgFAIL;

        while (fgets(temp,255,fpt)!=NULL)
                addline(temp);

        hgVSaveBuff(buff[buffnum]);
        hgVSetBuff(buff[nowbuff]);
        fclose(fpt);
        return hgSUCCESS;
}
void initial(void)
{
	hgVMEMInit(32);
        freespace=(unsigned *)malloc((size_t)(MAXBUFF));
	makenewbuffer();        /* buffer 0 making */
	makenewindex();         /* index  0 making */
        memset(elen,0,MAXROW);
}
char makenewbuffer()
{
        buffnum++;

	buff[buffnum]=(VMEM *)hgVMalloc(BUFFSIZE);
	if (buff[buffnum]==NULL) return hgFAIL;
	freespace[buffnum]=BUFFSIZE;
	memset(vgbuff,0,BUFFSIZE);
	offset=0;
	return hgSUCCESS;
}
char makenewindex()
{
	indexnum++;

	index[indexnum]=(byte *)malloc(BUFFSIZE);
	if (index[indexnum]==NULL) return hgFAIL;
	memset(index[indexnum],0xff,BUFFSIZE);
	ioffset=0;
	return hgSUCCESS;
}
int  isfreebuff(unsigned length)
{
        if (freespace[buffnum]>length) return hgSUCCESS;
        else return hgFAIL;
}
int  isfreeindex()
{
        if (ioffset>(unsigned)(BUFFSIZE-3)) return hgFAIL;
        else return hgSUCCESS;
}
void addline(char *str)
{
        unsigned length;

        length=strlen(str)+1;
        if (!isfreebuff(length)) {
                hgVSaveBuff(buff[buffnum]);
		makenewbuffer();
        }
        memcpy(&vgbuff[offset],str,length);
        updateindex(buffnum,offset);
        freespace[buffnum]-=length;
        tlines++;
        offset+=length;
}
void updateindex(char buffnum,unsigned offset)
{
	if (!isfreeindex()) makenewindex();
        putword(index[indexnum],ioffset,offset);ioffset+=2;
        putbyte(index[indexnum],ioffset,buffnum);ioffset++;
}
void putword(byte *buff,unsigned offset,unsigned data)
{
        byte high,low;

        high=data>>8;
        low =data & 0xff;

        buff[offset]=low;
        buff[offset+1]=high;
}
void putbyte(byte *buff,unsigned offset,byte data)
{
        buff[offset]=data;
}
unsigned getword(byte *buff,unsigned offset)
{
        unsigned ret;

        ret=(unsigned)buff[offset]+((unsigned)buff[offset+1]<<8);
        return ret;
}
char getbyte(byte *buff,unsigned offset)
{
        return buff[offset];
}
void freeprocess(void)
{
        int i;

        for (i=0;i<=buffnum;i++)
                hgVFree(&buff[i]);
        for (i=0;i<indexnum;i++)
                free(index[i]);
        free(freespace);
	hgVMEMEnd();
}
void screeninit(void)
{
	hgAutoSetDisplay();
	hgSetMode(hgGRAPHICS);
	hgSetHanFont(HMAPPLE_HAN);
	hgSetEngFont(SMALL_ENG);

	hgQuickOutInit();
	hgQuickGraphInit();

	_H=hgIsHerc();

	hgPrtWindowXy(0,0,639,479-_H*80,"¢‰¥¡œa Ðe‹i ¥¡‹¡ v1.0");
	hgForeTextXy(500,10,"[F1] •¡¶‘ i",WHITE);

	if (_H) {
		hgSetEngOut(&hputche_byte);
		hgSetHanOut(&hputchh_byte);
		hgEnableScrUp(&hgHScrUpFast);
		hgEnableScrDown(&hgHScrDownFast);
	} else {
		hgSetEngOut(&vputche_byte_vga);
		hgSetHanOut(&vputchh_byte_vga);
		hgEnableScrUp(&hgVScrUpFast);
		hgEnableScrDown(&hgVScrDownFast);
	}
	if (_H) MAXROW=22;
}
void screenend(void)
{
	hgSetMode(hgTEXT);
}
void printscreen(long *sline)
{
        unsigned lline;
        unsigned inum;
	unsigned per;
	unsigned offset;
        char     bnum,prvs;
        int      i;
        int      j;
        int      ch;            /* for recursively key input */

        per=BUFFSIZE/3;
        inum=(*sline)/per;
        lline=(*sline)-inum*per;
        lline=lline*3;

        offset=getword(index[inum],lline);
        bnum  =getbyte(index[inum],lline+2);
        prvs  =bnum;

        for (i=0;i<MAXROW;i++) {
                offset=getword(index[inum],lline);
                bnum  =getbyte(index[inum],lline+2);
                if (offset==0xffff && bnum==(char)0xff) break;
		if (elen[i]!=0) hgBoxFill(16,i*16+34,16+elen[i]*8,i*16+50,WHITE);
                if (bnum!=nowbuff) {
                        hgVSetBuff(buff[bnum]);
                        nowbuff=bnum;
                }
                printstring(16,i,vgbuff,offset);
                ch=inkey(NOWAIT);

                if (ch==CTRL_PgUp) {
                        prvs=(*sline);
                        (*sline)=0L;
                        if (prvs!=(*sline)) {
                                printscreen(sline);
                                return;
                        }
                        (*sline)=prvs;
                }
                if (ch==CTRL_PgDn) {
                        prvs=(*sline);
                        (*sline)=tlines-(long)(MAXROW-1L);
                        if (prvs!=(*sline)) {
                                printscreen(sline);
                                return;
                        }
                        (*sline)=prvs;
                }
                if (ch==PgUp) {
                        prvs=(*sline);
                        if ((*sline)>0) (*sline)-=(long)(MAXROW-1L);
                        if ((*sline)<0L) (*sline)=0L;
                        if (prvs!=(*sline)) {
                                printscreen(sline);
                                return;
                        }
                        (*sline)=prvs;
                }

                if (ch==PgDn) {
                        prvs=(*sline);
                        if (((*sline)+(long)MAXROW)<tlines) (*sline)+=(long)(MAXROW-1L);
                        if ((*sline)>=tlines) (*sline)=tlines-(long)(MAXROW-1L);
                        if (prvs!=(*sline)) {
                                printscreen(sline);
                                return;
                        }
                        (*sline)=prvs;
                }

                if (lline>(unsigned)(BUFFSIZE-3)) {
                        inum++;
                        lline=0;
                } else  lline+=3;
                if (inum>indexnum) break;
        }
	if (i<MAXROW) hgBoxFill(16,i*16+34,640-14,(MAXROW-1)*16+50,WHITE);
}
void printstring(int x,int y,byte *str,unsigned offset)
{
        byte temp[3];
        unsigned xpos=0;
	unsigned xtmp;

	while (str[offset]) {
		if (str[offset]&0x80) {
			temp[0]=str[offset];
			temp[1]=str[offset+1];
			temp[2]=0;
			hgOutTextXy(x,y*16+34,temp);
			x+=16;offset+=2;
			xpos+=2;
		} else {
			temp[0]=str[offset];
			temp[1]=0;
			if (str[offset]=='\t') {
				xtmp=xpos;
                                xpos+=((8-xpos%8))-1;
                                x+=((8-xtmp%8)*8-8);
                        }
                        else if (str[offset]!='\n')
				hgOutTextXy(x,y*16+34,temp);
                        x+=8;offset++;
                        xpos++;
                }
                if (x>MAXCOLUMN) break;
        }
        elen[y]=xpos;
}
void upprocess(long sline)
{
        unsigned lline;
        unsigned inum;
        unsigned per;
        unsigned offset;
        char     bnum;
        char     temp[255];
        int      i;

        per=BUFFSIZE/3;
        inum=sline/per;
        lline=sline-inum*per;
        lline=lline*3;

        offset=getword(index[inum],lline);
        bnum  =getbyte(index[inum],lline+2);
        if (offset==0xffff && bnum==(char)0xff) return;
        hgScrDownWindowXy(0,0,639,479-_H*80,16);

        for (i=MAXROW-1;i>=1;i--)
                elen[i]=elen[i-1];

        if (nowbuff!=bnum) {
                hgVSetBuff(buff[bnum]);
                nowbuff=bnum;
        }
        printstring(16,0,vgbuff,offset);
}
void downprocess(long sline)
{
        unsigned lline;
        unsigned inum;
        unsigned per;
        unsigned offset;
        char     bnum;
        char     temp[255];
        int      i;

        sline+=(MAXROW-1);
        per=BUFFSIZE/3;
        inum=sline/per;
        lline=sline-inum*per;
        lline=lline*3;

        offset=getword(index[inum],lline);
        bnum  =getbyte(index[inum],lline+2);
        if (offset==0xffff && bnum==(char)0xff) return;
        hgScrUpWindowXy(0,0,639,479-_H*80,16);

        for (i=1;i<MAXROW;i++)
                elen[i-1]=elen[i];

        if (nowbuff!=bnum) {
                hgVSetBuff(buff[bnum]);
                nowbuff=bnum;
        }
        printstring(16,MAXROW-1,vgbuff,offset);
}
void helpprocess(void)
{
        char *text[]={
 " ¢‰¥¡œa“e KETEL · ´å´á•·Ñ¡ÒA·¥ –ž¢‰´eµA ·¶“e ",
 " C ´å´á ¯aÈá—¡·³“¡”a. ·¡ Ïa¡‹aœ‘·e ¢‰¥¡œaµA¬á  e ",
 " —i´á»¥ Ðe‹i ¥¡‹¡ Ïa¡‹aœ‘·³“¡”a.  Virtual Memory ",
 " Ÿi »¡¶¥Ðaµa ˆá· ¢¹AÐe Ça‹¡· Text Ÿi ¥© ®  ·¶ ",
 " ·s“¡”a. ¬a¶w–A“e Ç¡·¡“e ”a·q‰Á ˆ{¯s“¡”a.",
 " ",
 "    [F1] •¡¶‘ i [F2] ÑÁ·©¬wÈ¥¡‹¡ [ESC] {‹¡",
 " ÑÁ¬iÎaÇ¡·¡,PgUp,PgDn,CTRL_PgDn,CTRL_PgUp,ALT1~0","" };

	hgSetSaveOn();
	hgForceCON();
	hgPrtWTextXyM("•¡¶‘ i",text);
	hgAutoCON();
	inkey(WAIT);
	hgSetSaveOff();
	hgRestore();
}
void fileprocess(long start)
{
	char  drive[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];
	char  *text[5];
	long  size;
	int   i;
	FILE *fpt;
	char  temp[30];
	int   y;
	int   len;
	float fsize=0.0;

	for (i=0;i<5;i++) {
		text[i]=(char *)malloc((size_t)50);
		memset(text[i],0,50);
	}

	fpt=fopen(fname,"r");
	if (fpt==NULL) return;

	fseek(fpt,0,2);
	size=ftell(fpt);
	fclose(fpt);

	fsize=(float)size/1024.0;

	fnsplit(fname,drive,dir,name,ext);
	strcpy(temp,name);
	strcat(temp,ext);

	sprintf(text[0]," ÑeÑÁ¡e ¯¡¸b œa·¥¤åÑ¡ %5ld",start+1);
	sprintf(text[1]," Á· œa·¥®            %5ld",tlines);
	sprintf(text[2]," ÑÁ·© Ça‹¡ (KB)    %8.1f",fsize);
	sprintf(text[3]," Ñe¸ ¶áÃ¡ ");
	strcpy(text[4],"");

	hgSetSaveOn();
	hgForceCON();
	hgPrtWTextXyM(temp,text);
	hgAutoCON();
	y=(hgGety2r()+hgGety1r())/2+16*2;
	len=(int)(start*100/tlines);
	hgBoxFill(310,y+8,410,y+16,LIGHTGRAY);
	for (i=0;i<len;i++)
		if (!_H)
			hgVline(310+i,y+8,y+16,RED);
		else
			hgVline(310+i,y+8,y+16,ZERO);

        inkey(WAIT);
	hgSetSaveOff();
	hgRestore();

        for (i=0;i<4;i++)
                free(text[i]);
}
void viewprocess(void)
{
        long sline=0L;
        int ch;
        long prvs=sline;

        printscreen(&sline);

        do {
                do {
                        ch=inkey(WAIT);
                } while (ch!=PgUp && ch!=PgDn &&
                         ch!=ESC  && ch!=DOWN &&
			 ch!=UP   && ch!=F1   &&
                         ch!=F2   && ch!=CTRL_PgUp && ch!=CTRL_PgDn &&
                         (ch<ALT_1  || ch>ALT_0) );

		if (ch==F1) {
			helpprocess();
                        hgVSetBuff(buff[nowbuff]);
		}
		if (ch==F2) {
			fileprocess(sline);
                        hgVSetBuff(buff[nowbuff]);
		}
		if (ch>=ALT_1 && ch<=ALT_0)
			sline=(tlines*(long)(ch-ALT_1+1))/11L;
		if (ch==CTRL_PgUp) sline=0;
		if (ch==CTRL_PgDn) sline=tlines-(long)(MAXROW-1L);
		if (ch==PgUp && sline>0) sline-=(long)(MAXROW-1L);
                if (ch==PgDn && (sline+(long)MAXROW)<tlines) sline+=(long)(MAXROW-1L);
                if (ch==UP && sline>0 ) {
                        sline--;
                        upprocess(sline);
		}
                if (ch==DOWN && (sline+MAXROW)<tlines) {
                        sline++;
                        downprocess(sline);
                }
                if (ch==ESC) break;
                if (sline<0) sline=0L;
                else if (sline>=tlines) sline=tlines-(long)(MAXROW-1L);
		if ((ch==PgDn || ch==PgUp || ch==CTRL_PgUp || ch==CTRL_PgDn
                     || (ch>=ALT_1 && ch<=ALT_0)) && prvs!=sline) printscreen(&sline);
                prvs=sline;
        } while (1);
}

void main(int argc,char *argv[])
{
        char flag;

        if (argc!=2) {
                printf("\nUsage: hr <filename>\n");
                exit(1);
	}
        if (access(argv[1],0)) {
                printf("\nFile Not Found!\n");
                exit(1);
	}
	hgSetSpecialCharOn();
	hgSetSpecialFont(SPECIAL_FNT);
	initial();
	screeninit();
	fileload(argv[1]);
	strcpy(fname,argv[1]);
	viewprocess();
	freeprocess();
	screenend();
}
