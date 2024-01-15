#include <alloc.h>
#include <stdio.h>
#include <ctype.h>
#include <dir.h>

#include "hginit.h"
#include "hghlib.h"
#include "hgquick.h"
#include "hwindow.h"
#include "key.h"

#define  ON     1
#define  OFF    0

int _H;

HICON *gicon;           /* global icon         */
char  fname[50];        /* global filename     */
char  *buff;            /* global buff         */
char  *cbuff;           /* global cursor buff  */
char  *csbuff;          /* color select buff   */
int   x=0,y=0;          /* global cursor pos.  */
int   color;            /* global color        */

char  *pbuff;           /* process buffer      */

int  isfill=OFF;
int  ismodify=OFF;      /* ismodify             */

/* cursor data */

static char cursor1[32]={
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  7,128,  7,224,
  7,224,  7,192,  4,224,  0,112,  0, 56,  0, 28,  0,  0,  0,  0 };
static char cursor2[32]={
  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,  9,128,  8, 96,  8, 24,
  8, 30,  8, 32, 11, 16, 12,136,  8, 68,  0, 34,  0, 28,  0,  0 };


/* prototype */

void pulldown_process();
void draw_color(int color);
void hide_color(int color);

void pbuff_lattice();

void draw_lattice(int xbyte,int ydot)
{
        int yincre=((_H)? 8 : 10);
        int x,y;

        hgBoxFill(25,50,425,yincre*40+50,WHITE);
        for (x=25;x<=25+xbyte*80;x+=10)
                hgVline(x,50,50+ydot*yincre,ZERO);
        for (y=50;y<=50+ydot*yincre;y+=yincre)
                hgHline(25,25+xbyte*80,y,ZERO);
}

void PrtColor(int color,int mode)
{
	if (!_H) {
		hgBoxFill(460,color*20+130,502,color*20+148,color);
		if (mode==NOPUSH)
			hgDrawBorder(460,color*20+130,502,color*20+148,BARNORMAL,NOFILL);
		else
			hgDrawBorder(460,color*20+130,502,color*20+148,BARREVERSE,NOFILL);
	} else {
		hgDrawBorder(451,200,500,249,BOXREVERSE,FILL);
		hgBoxFill(456,205,495,244,color);
		hgPrtButtonXy(455,175,"¬‚Œi",NOPUSH);
	}
}
int  choose_color(int color)
{
	int temp=color;
	int ch;

	if (!_H) {
		do {
			draw_color(color);
			ch=inkey(WAIT);
			hide_color(color);
			if (ch==UP) color--;
			if (ch==DOWN) color++;
			if (ch==RETURN || ch==ESC || ch==TAB) break;
			if (color<0)  color=15;
			if (color>15) color=0;
		} while (1);

		if (ch==ESC || ch==TAB) return temp;
		PrtColor(temp,NOPUSH);
		PrtColor(color,PUSH);
		return color;
        } else {
                if (color!=ZERO) color=ZERO;
                else color=WHITE;
		PrtColor(color,PUSH);
                return color;
        }
}

void draw_status(char *fname)
{
        int  i;
        char *s="´a·¡Å¥";
        char drive[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];
	char temp[20];

        fnsplit(fname,drive,dir,name,ext);

        strcpy(temp,name);
        strcat(temp,ext);

        hgDrawBorder(438,35,626,466-_H*80,BARREVERSE,FILL);
        hgOutTextXy(460,40,"Íe»³ÑÁ·©");
        hgPrtBarXy(441,622,37);
        hgFBTextXy(460,70,temp,ZERO,LIGHTGRAY);
        hgOutTextXy(460,100,"Äéœá¬åÈ‚");
        hgPrtBarXy(441,622,97);

        hgDrawBorder(531,200,580,249,BOXREVERSE,FILL);
        hgBoxFill(536,205,575,244,WHITE);
        hgPrtButtonXy(525,175,s,NOPUSH);

        hgDrawBorder(531,200,580,249,BOXREVERSE,FILL);
        hgBoxFill(536,205,575,244,WHITE);
	hgPrtButtonXy(525,175,s,NOPUSH);

	for (i=0;i<16;i++)
		PrtColor(i,NOPUSH);
}
void setscreen()
{
	hgAutoSetDisplay();
	hgSetMode(hgGRAPHICS);
	hgSetHanFont(HMAPPLE_HAN);
	hgSetEngFont(SMALL_ENG);

	hgQuickGraphInit();
	hgQuickOutInit();
	hgQuickInInit();
	hgVMEMInit(32);

	hgSetBack(LIGHTGRAY);
	hgClearScreen();

	/* for 2.0, standard image process function changed. */

	if (hgIsHerc()) {
		hgEnableImage(&hgHGetImage,&hgHPutImage);
		hgEnableImgSize(&hgHImgSize);
	}
	else    {
		hgEnableImage(&hgVGetImage,&hgVPutImage);
		hgEnableImgSize(&hgVImgSize);
	}

	_H=hgIsHerc();

	if (_H) hgEnablePlot(&hgHPlotXy);
	else    hgEnablePlot(&hgVPlotXy);

	if (_H) color=WHITE;
	else color=1;

	hgPrtWindowXy(0,0,639,479-_H*80,"ICON DRAW v1.0");

	gicon=(HICON *)malloc(sizeof(HICON));
}
void endscreen()
{
	hgVMEMEnd();
	hgSetMode(hgTEXT);

	free(gicon);
}

void main()
{
        setscreen();
        pulldown_process();
        endscreen();
}

void hurumi()
{
        char *text[]={
" ICONDRAW v1.0 ·e ÐáŸa£¡ 1.5 · ¦­¢ ·AË©Ÿ¡Ë¡ ",
" ·³“¡”a. ÐáŸa£¡ 1.5 µA ³a·¡“e ´a·¡Å¥·i —¡¸a·¥ ",
" Ða‹¡ ¶áÐ ¬é‰–A´ö·s“¡”a. ·¡ ·AË©Ÿ¡Ë¡¡ ´a·¡ ",
" Å¥·i —¡¸a·¥Ða¡e ÐáŸa£¡ 1.5 µA¬á“e ÑÁ·©¤w¯¢ , ",
" ™¡“e œa·¡§aœáŸ¡ ¤w¯¢·a¡ ´a·¡Å¥·i ¬a¶wÐi  ® ",
" ·¶‰A –S“¡”a. ‹aœñ.. ÐáŸa£¡ ","" };

        hgSetRealWindow(0,0,639,hgGety2r());
        hgSetSaveOn();
        hgPrtWTextXyM("ÐáŸa£¡",text);
        inkey(WAIT);
        hgRestore();
        hgSetSaveOff();
        hgSetRealWindow(0,0,460,hgGety2r());
}
void maker()
{
        char *text[]={
" ÐáŸa£¡ 1.5 “e KETEL ·  ‰·ˆ¸aža¯©µA¬á ",
"  “A´é‰Á µ¹A ÑÁ·©,œa·¡§aœáŸ¡,·AË©Ÿ¡Ë¡ ",
" Ÿi Í¡ÐqÐaµa ŠÐa¯© ® ·¶·s“¡”a. ”e  º— ",
" ˆe¤á¸å·¡£a¡ ­¡¯a“e ‰·ˆÐa»¡  ´g‰V·s“¡ ",
" ”a. ·¢…¸ñ·¡ ·¶·a·¡¡e µeœbº­A¶a..     ",
" ",
"             –ž¢‰ •·Ñ¡ÒA. ID:hwjketel ",
"             ‰Á‹¡” ¸å¬e‰Á 2 ‘e Ðe ¶»¥ ","" };

        hgSetRealWindow(0,0,639,hgGety2r());
        hgSetSaveOn();
        hgPrtWTextXyM("¹A¸b¸a",text);
        inkey(WAIT);
        hgRestore();
        hgSetSaveOff();
        hgSetRealWindow(0,0,460,hgGety2r());
}
int isexist(char *fname)
{
        return (!access(fname,0));
}
void fputs_header(char *str,FILE *wrt)
{
        int i,x,y;
        char temp[40];

        fputs("/* ======================================= */\n",wrt);
        fputs("/*       THIS IS Hurumi Total Library      */\n",wrt);
        fputs("/*     ICON File. this can be compiled     */\n",wrt);
        fputs("/*      by BC 2.0 and can be linked by     */\n",wrt);
        fputs("/*            Tlink 2.0 or higher.         */\n",wrt);
        fputs("/*                                         */\n",wrt);
        fputs("/*              Good luck to you.          */\n",wrt);
        fputs("/*                                         */\n",wrt);
        fputs("/*           written by KIT e900519        */\n",wrt);
        fputs("/*                 Han Woo-jin             */\n",wrt);
        fputs("/* ======================================= */\n\n",wrt);

        for (i=0;i<strlen(str);i++)
                str[i]=tolower(str[i]);

        fprintf(wrt,"#include <alloc.h>\n");
        fprintf(wrt,"#include <mem.h>\n");
        fprintf(wrt,"#include \"hwindow.h\"\n\n");

        fprintf(wrt,"void %-s(HICON *p)\n",str);
        fprintf(wrt,"{\n");
}
void fends_header(FILE *wrt,int size)
{
        char temp[50];
        int  xx,yy;

        xx=gicon->size.xwidth;
        yy=gicon->size.ywidth;

        sprintf(temp,"\tp->size.xwidth=%d;\n",xx);
        fprintf(wrt,temp);
        sprintf(temp,"\tp->size.ywidth=%d;\n\n",yy);
        fprintf(wrt,temp);
        fprintf(wrt,"\tp->buff=(char *)malloc((size_t)%-d);\n",size);
        fprintf(wrt,"\n\tmemcpy(p->buff,s1,%d);",size);
        fprintf(wrt,"\n};");
}
void pr_header(char *str,FILE *fpt,FILE *wrt,int c)
{
        int l;
        int ch;
        int count=0;

        fprintf(wrt,"\tchar %-s[]={\n\t",str);
        for (l=0;l<c;l++) {
                ch=fgetc(fpt);
                fprintf(wrt,"%3d",ch);
                if (l!=c-1) fprintf(wrt,",");
                count++;
                if (count==16) {
                        count=0;
                        fprintf(wrt,"\n\t");
                }
        }
        fprintf(wrt,"};\n\n");
}
int makefile()
{
        int   i;
        FILE *fpt;
        FILE *wrt;
	char  titl[50];
	char  temp[50];
	unsigned size;
	char  drive[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];

	if (!isfill) return ESC;

	strcpy(titl,fname);
	for (i=0;i<strlen(titl);i++)
		if (titl[i]=='.') break;
	titl[i]=0;
	strcpy(temp,titl);
	strcat(titl,".hif");
	fpt=fopen(fname,"rb");
	wrt=fopen(titl,"w");
	if (fpt==NULL) {
		store();
		fpt=fopen(fname,"rb");
	}
	fseek(fpt,33,0);                /* skip header */

	fnsplit(temp,drive,dir,name,ext);
	strcpy(temp,name);

	strcat(temp,"_icon");
	fputs_header(temp,wrt);
	size=gicon->size.xwidth*gicon->size.ywidth*4;
	pr_header("s1",fpt,wrt,size);
	fends_header(wrt,size);
	fclose(fpt);
        fclose(wrt);
        return 1;
}

int store()
{
        char sizeb[2];
        char *sav;
        int  i,j;
        unsigned size;
        char *vhdr="Hurumi 2.0 Icon File            ";
        char data1,data2;

	FILE *fpt;

        if (!isfill) return ESC;

        sizeb[0]=(char)gicon->size.xwidth;
        sizeb[1]=(char)gicon->size.ywidth;

        fpt=fopen(fname,"wb");
        fwrite(vhdr,33,1,fpt);                  /* write header */

        for (i=0;i<gicon->size.ywidth;i++)
                for (j=0;j<gicon->size.xwidth*8;j+=2) {
                        data1=pbuff[get_pos(j,i)];
			data2=pbuff[get_pos(j+1,i)];
			data1=((data1<<4)+data2);
			fputc(data1,fpt);
		}

        fseek(fpt,30,0);
        putc(0x1a,fpt);                         /* send EOF     */
	fseek(fpt,31,0);                        /* skip one ch. */
        putc(sizeb[0],fpt);                     /* send Xsize   */
        putc(sizeb[1],fpt);                     /* send Ysize   */
        fclose(fpt);
        ismodify=0;
        return 1;
}

int writename()
{
        char str[30];
        char *text[]={ " ·¡£¡ ¹¥¸Ða“e ÑÁ·©·³“¡”a ",
                       " ”ü´á³iŒa¶a ? ","" };
        char *item[]={ " Âá­¡ "," ”ü´á³q ","" };

        int  flag;

        if (!isfill) return ESC;

        hgInitStr(str,28);

        hgSetSaveOn();
        flag=hgInWTextXyM("¤aŽ‰ ÑÁ·©·¡Ÿq",str,25);
        hgRestore();
        if (flag==ESC) {
                hgSetSaveOff();
                return ESC;
        }
        if (isexist(str)) {
                flag=hgPrtButtonBarXyM(text,item);
                hgRestore();
                if (flag==0) {
                        hgSetSaveOff();
                        return ESC;
                }
        }
        hgSetSaveOff();
        strcpy(fname,str);
        store();
	draw_status(fname);
	pbuff_lattice();
        return 1;
}
void draw_cursor(int x,int y)
{
        int yincre=((_H)? 8 : 10);
        int xx,yy;
        int color1=YELLOW,color2=BLUE;

        if (_H) {
                color1=ZERO;
                color2=WHITE;
        }

        xx=25+x*10;
        yy=50+y*yincre;

        hgGetImage(xx,yy,xx+16,yy+16,cbuff);
        puthan(xx,yy,cursor1,color1);
        puthan(xx,yy,cursor2,color2);
}
void hide_cursor(int x,int y)
{
        int yincre=((_H)? 8 : 10);
        int xx,yy;

        xx=25+x*10;
        yy=50+y*yincre;

        hgPutImage(xx,yy,xx+16,yy+16,cbuff);
}
void draw_color(int color)
{
        int xx,yy;

        xx=475;
        yy=134+color*20;

        hgGetImage(xx,yy,xx+16,yy+16,csbuff);
        puthan(xx,yy,cursor1,YELLOW);
	puthan(xx,yy,cursor2,BLUE);
}
void hide_color(int color)
{
        int xx,yy;

        xx=475;
        yy=134+color*20;

        hgPutImage(xx,yy,xx+16,yy+16,csbuff);
}
int get_pos(int x,int y)
{
        int pos;

        pos=gicon->size.xwidth*y*8+x;
        return pos;
}
void fill_box(int x,int y,int color)
{
	int yincre=((_H)? 8 : 10);
        int xx,yy;

        xx=25+x*10;
        yy=50+y*yincre;

        hgBoxFill(xx+1,yy+1,xx+9,yy+yincre-1,color);
        hgPlotXy(x+536,y+205,color);
        pbuff[get_pos(x,y)]=color;
        ismodify=1;
}
void pbuff_lattice()
{
        int i,j;
        int pos;

        for (i=0;i<gicon->size.xwidth*8;i++)
                for (j=0;j<gicon->size.ywidth;j++) {
                        pos=get_pos(i,j);
                        fill_box(i,j,pbuff[pos]);
		}
}
int isicon(FILE *f)
{
    long offset,size;
    int  xs,ys;

    fseek(f,31,0);
    xs=getc(f);
    ys=getc(f);
    fseek(f,0,2);
    size=xs*ys*4+33;
    offset=ftell(f);
    fseek(f,0,0);
    if (size!=offset) return 0;
    else return 1;
}

/*
void dither()
{
        char paltable[16]={ 0,4,24,29,12,17,24,42,21,25,45,50,33,38,58,63 };
        int dtable[8][8] = {
            { 0, 32,  8, 40,  2, 34, 10, 42},
            {48, 16, 56, 24, 50, 18, 58, 26},
            {12, 44,  4, 36, 14, 46,  6, 38},
            {60, 28, 52, 20, 62, 30, 54, 22},
            { 3, 35, 11, 43,  1, 33,  9, 41},
            {51, 19, 59, 27, 49, 17, 57, 25},
            {15, 47,  7, 39, 13, 45,  5, 37},
            {63, 31, 55, 23, 61, 29, 53, 21} };
        
        int  i,j,k,l;
        char clr;
        int  pos;
        char comp;

        for (i=0;i<gicon->size.ywidth;i+=8)
                for (j=0;j<gicon->size.xwidth*8;j+=8) {
                        for (k=0;k<8;k++)
                                for (l=0;l<8;l++) {
                                        pos=get_pos(l+j,k+i);
                                        clr=pbuff[pos];
                                        comp=dtable[k][l];
                                        if (paltable[clr]>comp) pbuff[pos]=WHITE;
                                        else pbuff[pos]=ZERO;
                                }
                }
}
*/

void dither()
{
        char paltable[16]={ 0,4,24,29,12,17,24,42,21,25,45,50,33,0,58,63 };
        int  i;
        char clr;
        unsigned size=gicon->size.xwidth*gicon->size.ywidth*8;

        for (i=0;i<size;i++) {
                clr=pbuff[i];
                if (paltable[clr]>31) pbuff[i]=WHITE;
                else pbuff[i]=ZERO;
        }
}

int import()
{
	FILE *fpt;
	int   flag;
        char str[50];
        char ret[50];
	int   i,j,clr,pos;

	char ctable[16]={ BLACK,RED,GREEN,YELLOW,BLUE,MAGENTA,CYAN,
			  DARKGRAY,LIGHTGRAY,LIGHTGRAY,GREEN,YELLOW,
			  LIGHTBLUE,MAGENTA,LIGHTCYAN,WHITE };

	char *text[]={ " Ñe¸ ÑÁ·©·¡ Íe»³º—·³“¡”a. ",
		       " ¸á¸wÐiŒa¶a ? ","" };
	char *item[]={ " ¸á¸w "," ¸á¸w´eÐq ","" };

	hgInitStr(str,28);

	hgSpecInModeOn();
	strcpy(str,"*.ico");

	hgSetSaveOn();
	flag=hgInWTextXyM("ÑÁ·©·¡Ÿq",str,25);
	hgRestore();

	hgSpecInModeOff();

	if (flag==ESC) {
		hgSetSaveOff();
		return ESC;
	}

	hgSetRealWindow(0,0,639,479-_H*80);
	flag=hgFileDirBoxXyM(str,ret);
	hgSetRealWindow(0,0,460,479-_H*80);
	if (flag!=-1) hgRestore();

        if (!strcmp(ret,"\\")) {
             hgSetSaveOff();
             return ESC;
        }

        if (ismodify) {
                flag=hgPrtButtonBarXyM(text,item);
                hgRestore();
                if (flag==0) store();
        }
        hgSetSaveOff();

        strcpy(fname,ret);
        draw_status(fname);

        fpt=fopen(fname,"rb");

        if (fpt!=NULL) {
                        fseek(fpt,126,0);
                        gicon->size.xwidth=4;
                        gicon->size.ywidth=32;
                        if (isfill) free(pbuff);
                        pbuff=(char *)malloc((size_t)1024);
                        for (i=0;i<32;i++)
                                for (j=0;j<32;j+=2) {
                                        pos=get_pos(j,31-i);
                                        clr=fgetc(fpt);
                                        pbuff[pos]=ctable[(clr & 0xf0) >> 4];
                                        pos=get_pos(j+1,31-i);
                                        pbuff[pos]=ctable[(clr & 0x0f)];
                                }
                        if (_H) dither();
                        draw_lattice(4,32);
                        pbuff_lattice();
                        hgGetImage(13,33,437,63,buff);
                        isfill=1;
	} else {
                if (isfill) free(pbuff);
                isfill=0;
        }
        fclose(fpt);

        ismodify=0;     /* modify flag init     */
        x=0;            /* cursor position init */
        y=0;
        color=1;        /* color init           */
        if (_H) color=WHITE;

        return hgSUCCESS;
}

int load()
{
        char hdr[40];
        unsigned size;
        FILE *fpt;
	char clr;
        int  i,j;
        int  xs,ys;
        char data1,data2;

        char str[30];
        char ret[50];
        int  flag;
	char *text[]={ " Ñe¸ ÑÁ·©·¡ Íe»³º—·³“¡”a. ",
                       " ¸á¸wÐiŒa¶a ? ","" };
        char *item[]={ " ¸á¸w "," ¸á¸w´eÐq ","" };
        char *notm[]={ " ¸â¸éÐe ´a·¡Å¥ Èa·³·¡ ´a“³“¡”a.",
                       " ´a¢ Ç¡·¡a ’Ÿa­A¶a.","" };

        hgInitStr(str,28);

	hgSpecInModeOn();
	strcpy(str,"*.hic");

	hgSetSaveOn();
	flag=hgInWTextXyM("ÑÁ·©·¡Ÿq",str,25);
	hgRestore();

	hgSpecInModeOff();

	if (flag==ESC) {
		hgSetSaveOff();
		return ESC;
	}

        hgSetRealWindow(0,0,639,479);
        flag=hgFileDirBoxXyM(str,ret);
        hgSetRealWindow(0,0,460,479);
        if (flag!=-1) hgRestore();

        if (!strcmp(ret,"\\")) {
             hgSetSaveOff();
             return ESC;
        }

        if (ismodify) {
		flag=hgPrtButtonBarXyM(text,item);
		hgRestore();
                if (flag==0) store();
        }
        hgSetSaveOff();

        if (!hgIsIcon(ret)) {
             hgSetSaveOn();
             hgPrtWTextXyM("µAœá",notm);
             inkey(WAIT);
             hgSetSaveOff();
             hgRestore();
             return ESC;
        }

        strcpy(fname,ret);
        draw_status(fname);

        fpt=fopen(fname,"rb");

        if (fpt!=NULL) {
                isfill=1;
                fseek(fpt,31,0);
                xs=getc(fpt);                           /* get Xsize   */
                ys=getc(fpt);                           /* get Ysize   */
                fseek(fpt,0,0);
                gicon->size.xwidth=xs;
		gicon->size.ywidth=ys;

                fread(hdr,33,1,fpt);                    /* read header */
		size=(unsigned) ( gicon->size.xwidth * gicon->size.ywidth * 8 );
		if (isfill) free(pbuff);
		pbuff=(char *)malloc((size_t)size);
		for (i=0;i<ys;i++)
			for (j=0;j<xs*8;j+=2) {
				data1=fgetc(fpt);
                                data2=data1 & 0x0f;
				data1=(data1 & 0xf0)>>4;
				pbuff[get_pos(j,i)]=data1;
				pbuff[get_pos(j+1,i)]=data2;
			}
                if (_H) dither();
		draw_lattice(xs,ys);
		pbuff_lattice();
		hgGetImage(13,33,437,63,buff);
	} else {
		if (isfill) free(pbuff);
		isfill=0;
	}
	fclose(fpt);

	ismodify=0;     /* modify flag init     */
	x=0;            /* cursor position init */
	y=0;
        color=1;        /* color init           */
        if (_H) color=WHITE;

        return hgSUCCESS;
}
int new()
{
        char str[30];
        char ret[50];
        int  flag;
        char *text[]={ " Ñe¸ ÑÁ·©·¡ Íe»³º—·³“¡”a. ",
                       " ¸á¸wÐiŒa¶a ? ","" };
        char *item[]={ " ¸á¸w "," ¸á¸w´eÐq ","" };
        char *text2[]={" ·¡£¡ ¹¥¸Ða“e ÑÁ·©·³“¡”a. ",
                       " ”ü´á³iŒa¶a ? ","" };
        char *item2[]={" Âá­¡ "," ”ü´á³q ","" };

        hgSetSaveOn();
        if (ismodify) {
                flag=hgPrtButtonBarXyM(text,item);
                hgRestore();
                if (flag==0) store();
        }

        hgInitStr(str,28);
	flag=hgInWTextXyM("ÑÁ·©·¡Ÿq",str,25);
	hgRestore();
	if (flag==ESC) {
		hgSetSaveOff();
		return ESC;
	}

	if (isexist(str)) {
                flag=hgPrtButtonBarXyM(text2,item2);
                hgRestore();
                if (flag==0) {
                    hgSetSaveOff();
                    return ESC;
                }
        }
        strcpy(fname,str);
        hgSetSaveOff();
        draw_status(fname);

	if (isfill) free(pbuff);

	isfill=0;       /* fill optione         */
	ismodify=0;     /* modify flag init     */
	x=0;            /* cursor position init */
	y=0;
	color=1;        /* color init           */
	if (_H) color=WHITE;

        return hgSUCCESS;
}
void rotate(int xoff,int yoff)
{
        unsigned size;
        char *temp;
        int  i,j;
        int  i2,j2;

        size=gicon->size.xwidth*gicon->size.ywidth*8;
        temp=(char *)malloc((size_t)size);

        for (i=0;i<gicon->size.xwidth*8;i++)
                for (j=0;j<gicon->size.ywidth;j++) {
			i2=i+xoff;
                        j2=j+yoff;
                        if (i2<0) i2=gicon->size.xwidth*8-1;
                        if (i2>=(gicon->size.xwidth*8)) i2=0;
                        if (j2<0) j2=gicon->size.ywidth-1;
                        if (j2>=gicon->size.ywidth) j2=0;
                        temp[get_pos(i2,j2)]=pbuff[get_pos(i,j)];
                }
        memcpy(pbuff,temp,size);
        free(temp);
        pbuff_lattice();
}
void fillpbuff()
{
	int i;
	unsigned size;

        size=gicon->size.xwidth*gicon->size.ywidth*8;

	for (i=0;i<size;i++)
		pbuff[i]=color;
        pbuff_lattice();
}
void edit()
{
        char str[5];
        int  xsize=5;           /* default size    */
        int  ysize=40;
        int  ch;
        int  xpos=0;
        int  temp;
        int  i;
        int  size;
        int  yincre=((_H) ? 8 : 10);

        WIDTH  barw={70,20};
	BCOLOR barc={2,0};
        HBAR  *p;

        p=hgHBAR_Load(barc,barw);

        if (!isfill) {
                hgBoxFill(25,50,425,yincre*40+50,WHITE);
                hgGetImage(13,33,437,63,buff);
                hgPrtWindowXy(120,150,320,281,"Ça‹¡¸÷Ða‹¡");
                hgBoxFill(133,183,307,268-_H*40,LIGHTGRAY);
                if (_H) hgDHline(134,306,228);
                hgOutTextXy(146,200,"ˆa¡Ça‹¡  ­A¡Ça‹¡");
                hgPrintfXy(146,230+_H*12, "   %2d        %2d",xsize*8,ysize);

                do {
                        hgHBAR_Draw(p,144+xpos*80,198,DRAW);
                        ch=inkey(WAIT);
                        hgHBAR_Draw(p,144+xpos*80,198,ERASE);
                        if (ch==F10)   ch=ESC;
                        if (ch==LEFT ) xpos--;
                        if (ch==RIGHT) xpos++;
                        if (ch==DOWN && xpos==0) xsize--;
                        if (ch==DOWN && xpos==1) ysize--;
                        if (ch==UP   && xpos==0) xsize++;
                        if (ch==UP   && xpos==1) ysize++;
                        if (ch==RETURN || ch==ESC) break;
			if (xsize>5) xsize=1;
                        if (xsize<1) xsize=5;
                        if (ysize>40) ysize=1;
                        if (ysize<1) ysize=40;
                        if (xpos<0) xpos=1;
                        if (xpos>1) xpos=0;
                        if (!_H) hgBoxFill(146,230,307,250,LIGHTGRAY);
                        else hgBoxFill(146,230+_H*12,307,250+_H*12,WHITE);
                        hgPrintfXy(146,230+_H*12, "   %2d        %2d",xsize*8,ysize);
                } while (1);

                if (ch==ESC) {
                        hgBoxFill(120,150,321,282,WHITE);
			return;
                }

                temp=ismodify;
                pbuff=(char *)malloc((size_t)(xsize*ysize*8));
                memset(pbuff,15,xsize*ysize*8);
                gicon->size.xwidth=xsize;
                gicon->size.ywidth=ysize;
                draw_lattice(xsize,ysize);
                pbuff_lattice();
                hgGetImage(13,33,437,63,buff);
                draw_status(fname);
                ismodify=temp;
	}
        else {
                xsize=gicon->size.xwidth;
                ysize=gicon->size.ywidth;
        }
        isfill=ON;
        gicon->size.xwidth=xsize;
        gicon->size.ywidth=ysize;
        size=xsize*ysize;

        PrtColor(color,PUSH);
        do {
                draw_cursor(x,y);
		ch=inkey(WAIT);
                hide_cursor(x,y);

                if (ch==F10)  ch=ESC;
                if (ch==LEFT) x--;
                if (ch==RIGHT) x++;
                if (ch==UP) y--;
                if (ch==DOWN) y++;

                if (ch<255) ch=toupper(ch);

                if (ch=='D') fill_box(x,y,color);
                if (ch=='C') {
			memset(pbuff,15,xsize*ysize*8);
                        pbuff_lattice();
                }
                if (ch=='R' && _H) {
                        for (i=0;i<size*8;i++)
                                if (pbuff[i]==ZERO) pbuff[i]=WHITE;
                                else pbuff[i]=ZERO;
                        pbuff_lattice();
                }
                if (ch=='I') rotate(0,-1);
                if (ch=='M') rotate(0, 1);
                if (ch=='J') rotate(-1,0);
		if (ch=='K') rotate(1, 0);
		if (ch=='F') fillpbuff();
                if (ch==TAB)    color=choose_color(color);
                if (ch==ESC) break;
                if (x>=xsize*8) x=0;
                if (x<0) x=xsize*8-1;
                if (y>=ysize) y=0;
                if (y<0) y=ysize-1;
        } while (1);
        hgGetImage(13,33,437,63,buff);
}

void pulldown_process()
{
        char *item[4]={ " ÐáŸa£¡ "," ÑÁ·© "," Ëb¥i¡A“A ","" };
        char *text[4][6]={ { " ÐáŸa£¡ "," ¹A¸b¸a "," { ","" },
                           { " ¦‰œáµ¡‹¡ "," ¸á¸wÐa‹¡ "," ¬¡ ",
                             " ¬·¡Ÿq·a¡ "," ÑÁ·©¸á¸w ","" },
                           { " .ICO ˆa¹aµ¡‹¡","" } };

        char *tend[]={ "      Ñe¸ ÑÁ·©·¡ Íe»³º—·³“¡”a.         ",
                       "      ¸á¸wÐiŒa¶a ? ","" };
        char *titm[]={ " ¸á¸w "," ¸á¸w´eÐq "," {‘Âá­¡ ","" };

        int  x[3]={ 30,120,210 };
        WIDTH   pw[3]={ { 104,64 },{ 124,104 },{ 144,24 } };

        WIDTH dw;
        WCOLOR pc;
        HPOPUP *pu[3];
        HPULLDOWN *pd;

        int xx,yy;
        int i;
        int flag;
        long freesize;

        hgWCOLOR_Load(&pc,wcBOXNORMAL,ZERO,DARKGRAY,LIGHTGRAY);

        for (i=0;i<3;i++)
                pu[i]=hgHPOPUP_Load(text[i],pc,pw[i]);

        draw_status("");

        hgSetRealWindow(0,0,460,hgGety2r());

        hgWIDTH_Load(&dw,423,30);
        pd=hgHPULLDOWN_Load(item,x,dw,pu);

        csbuff=(char *)malloc((size_t)hgImgSize(0,0,25,25));
        cbuff=(char *)malloc((size_t)hgImgSize(0,0,25,25));
        buff=(char *)malloc((size_t)hgImgSize(13,33,437,63));

        hgGetImage(13,33,437,63,buff);

        do {
                hgHPULLDOWN_Choose(pd,13,33,-1,&xx,&yy);
                if (xx==0 && yy==2) {
                        hgPutImage(13,33,437,63,buff);
                        flag=1; /* end */
                        if (ismodify) {
                                hgSetSaveOn();
                                flag=hgPrtButtonBarXyM(tend,titm);
                                hgRestore();
                        }
                        if (flag==0) store();
                        if (flag!=2) break;
                }
                if (xx==-1) {
                        flag=ESC;
                        hgPutImage(13,33,437,63,buff);
                }
                switch(xx) {
                        case 0:
                                hgPutImage(13,33,437,63,buff);
                                switch(yy) {
                                        case 0:
                                                hurumi();
                                                flag=ESC;
                                                break;
                                        case 1:
                                                maker();
                                                flag=ESC;
                                                break;
                                }
                                break;
                        case 1:
                                hgPutImage(13,33,437,63,buff);
                                switch(yy) {
                                        case 0:
                                                flag=load();
                                                break;
                                        case 1:
                                                flag=store();
                                                break;
                                        case 2:
                                                flag=new();
                                                break;
                                        case 3:
                                                flag=writename();
                                                break;
                                        case 4:
                                                flag=makefile();
                                                break;
                                }
                                break;
                        case 2:
                                hgPutImage(13,33,437,63,buff);
                                switch(yy) {
                                        case 0:
                                                flag=import();
                                                break;
                                }
                }
                if (flag!=ESC || (flag==ESC && isfill)) edit();
        } while (1);

        for (i=0;i<3;i++)
                hgHPOPUP_Free(&(pu[i]));

        hgHPULLDOWN_Free(&pd);

        free(buff);
        free(cbuff);
        free(csbuff);
        free(pbuff);
}
