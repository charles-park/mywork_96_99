/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           "Font Viewer 1.0"            */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <alloc.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <ctype.h>

#include "hginit.h"
#include "hghlib.h"
#include "hgquick.h"
#include "hwindow.h"
#include "key.h"

#define  _8x4x4  0
#define  _6x2x1  1
#define _10x4x4  2
#define  _2x1x2  3

#define ENGLISH 4

#define MAXFONT 500

typedef struct {
	char *fname;
	int  ftype;
} FONT_;

FONT_ font[MAXFONT];
int   fpos=0;

#define HDIFF hgIsHerc()*80

void init_mainscreen()
{
	hgPrtWindowXy(0,0,639,479-HDIFF,"ÐáŸa£¡ Í¥Ëa  e—i‹¡");
	hgBoxFill(15,35,625,465-HDIFF,LIGHTGRAY);
	hgSetTFcolor(ZERO);
	hgHline(15,625,465-40-HDIFF,ZERO);
	hgHline(15,625,465-39-HDIFF,WHITE);
	hgOutTextXy(50,438-HDIFF,"RETURN : ‹i¸a¥¡‹¡    F1 : —w¢¡¡—I  e—i‹¡    ALT_X : {‹¡");
}
void search(char *mask)
{
	int flag;
	struct ffblk *file;
	char temp[50];

	file=(struct ffblk *)malloc(sizeof(struct ffblk));
	flag=findfirst(mask,file,0xFF);

	while (flag==0) {
		if (_chmod(file->ff_name,0)&FA_ARCH && strcmp(file->ff_name,"..")) {
			strcpy(temp,file->ff_name);
			font[fpos].fname=(char *)malloc((size_t)(strlen(temp)+1));
			strcpy(font[fpos++].fname,temp);
		}
		flag=findnext(file);
		if (fpos>=MAXFONT) break;
	}
	free(file);
}
void load_fontfilename()
{
	fpos=0;

	search("*.fnt");
	search("*.eng");
	search("*.kor");
}
void free_fontfilename()
{
	int i;

	for (i=0;i<fpos;i++)
		free(font[i].fname);
}
void print_filename()
{
	int x=35,y=42;
	int i;

	for (i=0;i<fpos;i++) {
		if (i>=80) break;
		hgOutTextXy(x,y,font[i].fname);
		x+=150;
		if (x==635) x=35,y+=20;
	}
}

void get_barxy(int hpos,int vpos,int *x,int *y)
{
	*x=hpos*150+31;
	*y=vpos*20+40;
}

int get_fonttype(int n)
{
	FILE *fpt;
	long size;

	fpt=fopen(font[n].fname,"r");

	fseek(fpt,0,2);
	size=ftell(fpt);

	fclose(fpt);

	if (size<4000) return  _2x1x2;
	if (size<5000) return   ENGLISH;
	if (size<8000) return  _6x2x1;
	if (size<12000) return _8x4x4;

	return _10x4x4;
}

void view(int n)
{
	char *texth[]={ " ´á“ai ¬¥b,e ÎŸeÐa“i —i·a¡a Å©œaŸi ",
			"  a­a”–»¡. Óªµe µe‹¡¬a·¡¡ §¡Ã¡“e ‹a ",
			" ¥¡¡a e Ðá wÐ–´á.‹a”µa Ðe¤å ¶•´á  º ",
			" ‘A¡¡‘A· ¯iÏe ¡¡¯s·a¡ ÑÁÐ ˆa‘A.     ","" };

	char *texte[]={ " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijk ",
			" lmnopqrstuvwxyz01234567890`-=~!@#$%^& ",
			" *()|[];',./{}:\"<>? ",
			" program may give you much flexiblity  ","" };

	char *buff;
	char temp[50];

	int type=get_fonttype(n);

	int xwidth=350;
	int ywidth=130;
	int ch;
        int pos=0;
	int save=hgGetTFcolor();

	WCOLOR   msgc;
	WIDTH    msgs;
	HWINDOW *msgw;

	switch(type) {
		case ENGLISH:
			sprintf(temp,"%s (ENGLISH 256)",font[n].fname);
			break;
		case _8x4x4 :
			sprintf(temp,"%s (8x4x4)",font[n].fname);
			break;
		case _10x4x4:
			sprintf(temp,"%s (10x4x4)",font[n].fname);
			break;
		case _6x2x1:
			sprintf(temp,"%s (6x2x1)",font[n].fname);
			break;
		case _2x1x2:
			sprintf(temp,"%s (2x1x2)",font[n].fname);
			break;
	}

	hgWCOLOR_Load(&msgc,2,WHITE,DARKGRAY,0);
	hgWIDTH_Load(&msgs,xwidth,ywidth);
	msgw=hgHWINDOW_Load(temp,msgc,msgs);

	buff=(char *)malloc((size_t)hgImgSize(150,150,500,280));
	hgGetImage(150,150,500,280,buff);

	hgHWINDOW_Draw(msgw,150,150);

	hgSetTFcolor(ZERO);

	if (type!=ENGLISH) {
		hgSetHanFontRoom(HAN_10x4x4);
		hgLoadHanFont(font[n].fname,type);
		while (strcmp(texth[pos],"")) {
			hgOutTextXy(150+16,150+40+pos*16,texth[pos]);
			pos++;
		}
		hgFreeHanRoom();
	}
	else {
		hgSetEngFontRoom(ENG_256);
		hgLoadEngFont(font[n].fname,0);
		while (strcmp(texte[pos],"")) {
			hgOutTextXy(150+16,150+40+pos*16,texte[pos]);
			pos++;
		}
		hgFreeEngRoom();
	}

	hgSetTFcolor(save);
	inkey(WAIT);

	hgPutImage(150,150,500,280,buff);
	free(buff);
	hgSetHanFont(HMAPPLE_HAN);
	hgSetEngFont(SMALL_ENG);
}

void fputs_header(FILE *wrt)
{
	int i;

	fputs("/* ======================================= */\n",wrt);
	fputs("/*       THIS IS Hurumi Total Library      */\n",wrt);
	fputs("/*     Font File. this can be compiled     */\n",wrt);
	fputs("/*      by BC 2.0 and can be linked by     */\n",wrt);
	fputs("/*            Tlink 2.0 or higher.         */\n",wrt);
	fputs("/*                                         */\n",wrt);
	fputs("/*              Good luck to you.          */\n",wrt);
	fputs("/*                                         */\n",wrt);
	fputs("/*           written by KIT e900519        */\n",wrt);
	fputs("/*                 Han Woo-jin             */\n",wrt);
	fputs("/* ======================================= */\n\n",wrt);

	fprintf(wrt,"extern char *fir;\n");
	fprintf(wrt,"extern char *mid;\n");
	fprintf(wrt,"extern char *las;\n");
	fprintf(wrt,"extern char *eng;\n\n");
}
void fends_header(FILE *wrt,char *str,char *titl,int i,int j,int k)
{
	char temp[50];
	int  l;

	for (l=0;l<strlen(titl);l++)
		titl[l]=tolower(titl[l]);

	if (i!=-1) {
		strcat(titl,"_han");
		fprintf(wrt,"extern char far %-s_f[%d][20][32];\n",str,i);
		fprintf(wrt,"extern char far %-s_m[%d][22][32];\n",str,j);
		fprintf(wrt,"extern char far %-s_l[%d][28][32];\n",str,k);
	}
	else {
		strcat(titl,"_eng");
		fprintf(wrt,"extern char far %-s_e[%d][16];\n",str,256);
	}
	fprintf(wrt,"\nvoid %-s()\n{",titl);
	if (i!=-1) {
		strcpy(temp,str);
		strcat(temp,"_f");
		fprintf(wrt,"\n\tfir=&%-s[0][0][0];\n",temp);
		strcpy(temp,str);
		strcat(temp,"_m");
		fprintf(wrt,"\tmid=&%-s[0][0][0];\n",temp);
		strcpy(temp,str);
		strcat(temp,"_l");
		fprintf(wrt,"\tlas=&%-s[0][0][0];\n",temp);
	} else  {
		strcpy(temp,str);
		strcat(temp,"_e");
		fprintf(wrt,"\n\teng=&%-s[0][0];\n",temp);
	}
	fprintf(wrt,"};");
}
void pr_header(char *str,FILE *fpt,FILE *wrt,int c)
{
	int l;
	int ch;
	int count=0;

	fprintf(wrt,"\n			public _%-s\n",str);
	fprintf(wrt,"%-sTEXT	segment byte public 'CODE'\n",str);
	fprintf(wrt,"_%-s		label byte\n",str);
	fprintf(wrt,"\n;font data\n\n");
	fprintf(wrt,"\tdb ");
	for (l=0;l<c;l++) {
		ch=fgetc(fpt);
		fprintf(wrt,"%3d",ch);
		if (l!=c-1 && count!=15) fprintf(wrt,",");
		count++;
		if (count==16 && l!=c-1) {
			count=0;
			fprintf(wrt,"\n\tdb ");
		}
	}
	fprintf(wrt,"\n%-sTEXT		ends\n",str);
}

void register_font(int n)
{
	int type;
	int i,j,k;
	int len;

	char temp[50];
	char titl[50];
	char temp2[50];
	char titl2[50];

	FILE *fpt;
	FILE *wrt;
	FILE *wrti;

	for (i=0;i<strlen(font[n].fname);i++)
		if (font[n].fname[i]=='.') break;

	strcpy(titl,font[n].fname);
	titl[i]=0;
	strcpy(temp,titl);
	strcpy(titl2,titl);
	strcat(titl2,".asm");

	len=strlen(titl);
	if (len!=8) {
		titl[len]='i';
		titl[len+1]=0;
	} else {
		titl[7]='i';
		titl[8]=0;
	}
	strcat(titl,".hft");
	fpt=fopen(font[n].fname,"rb");
	wrti=fopen(titl,"w");
	wrt=fopen(titl2,"w");

	type=get_fonttype(n);

	switch(type) {
		case _8x4x4:
			i=8;
			j=4;
			k=4;
			break;
		case _10x4x4:
			i=10;
			j=4;
			k=4;
			break;
		case _6x2x1:
			i=6;
			j=2;
			k=1;
			break;
		case _2x1x2:
			i=2;
			j=1;
			k=2;
			break;
		case ENGLISH:
			i=j=k=-1;
			break;
	};

	strcpy(titl,temp);

	if (i!=-1) strcat(temp,"_han");
	else strcat(temp,"_eng");

	fputs_header(wrti);

	if (i!=-1) {
		strcpy(temp2,temp);
		strcat(temp2,"_f");
		pr_header(temp2,fpt,wrt,i*32*20);
		strcpy(temp2,temp);
		strcat(temp2,"_m");
		pr_header(temp2,fpt,wrt,j*32*22);
		strcpy(temp2,temp);
		strcat(temp2,"_l");
		pr_header(temp2,fpt,wrt,k*32*28);
	}
	else {
		strcpy(temp2,temp);
		strcat(temp2,"_e");
		pr_header(temp2,fpt,wrt,256*16);
	}
	fprintf(wrt,"				end\n");
	fends_header(wrti,temp,titl,i,j,k);
	fclose(fpt);
	fclose(wrt);
	fclose(wrti);
}

void select_process()
{
	int vpos=0;
	int hpos=0;
	int x,y;
	int ch;

	WIDTH  sbars;
	BCOLOR sbarc;
	HBAR  *sbarb;

	hgWIDTH_Load(&sbars,120,20);
	hgBCOLOR_Load(&sbarc,2,0);
	sbarb=hgHBAR_Load(sbarc,sbars);

	print_filename();

	do {
		get_barxy(hpos,vpos,&x,&y);

		hgHBAR_Draw(sbarb,x,y,DRAW);
		ch=inkey(WAIT);
		hgHBAR_Draw(sbarb,x,y,ERASE);

		if (ch==RIGHT) hpos++;
		if (ch==LEFT) hpos--;
		if (ch==UP) vpos--;
		if (ch==DOWN) vpos++;

		if (hpos<0) hpos=0;
		if (hpos>3) hpos=3;
		if (vpos>20) vpos=20;
		if (vpos<0) vpos=0;

		if (vpos*4+hpos >= fpos) {
			vpos=(fpos-1)/4;
			hpos=(fpos-1)%4;
		}
		if (ch==ALT_X) break;
		if (ch==RETURN) view(hpos+vpos*4);
		if (ch==F1) register_font(hpos+vpos*4);
	} while (1);
}

void main()
{
	char *text[]={
" Ñe —¡BÉ¡Ÿ¡µA“e .kor,.eng,.fnt —w ÐáŸa£¡ˆa ",
" ”až‰® ·¶“e ÑÂ¸w¸a· ÑÁ·©·¡ ´ô·s“¡”a. ÑÂ·¥ ",
" Ð¥¡­A¶a.. ","" };

	hgAutoSetDisplay();
	hgSetMode(hgGRAPHICS);
	hgSetHanFont(HMAPPLE_HAN);
	hgSetEngFont(SMALL_ENG);

	hgQuickInit();
	hgQuickOutInit();
	hgQuickGraphInit();
	hgVMEMInit();

	/* for 2.0, standard image process function changed. */

        if (hgIsHerc()) {
		hgEnableImage(&hgHGetImage,&hgHPutImage);
		hgEnableImgSize(&hgHImgSize);
	}
	else    {
		hgEnableImage(&hgVGetImage,&hgVPutImage);
		hgEnableImgSize(&hgVImgSize);
	}


	init_mainscreen();
	load_fontfilename();

	if (fpos!=0) {
		select_process();
		free_fontfilename();
	} else {
		hgPrtWTextXyM("º·!",text);
		inkey(WAIT);
	}

	hgVMEMEnd();
	hgQuickEnd();
}
