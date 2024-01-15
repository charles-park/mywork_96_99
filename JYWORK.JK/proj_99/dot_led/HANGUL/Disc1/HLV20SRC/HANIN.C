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

#include <mem.h>
#include <bios.h>
#include <string.h>
#include <dos.h>

#include "key.h"               /* key definition        */
#include "hghlib.h"            /* hangul-output header  */
#include "hginit.h"	       /* hurumi initial header */

#include <ctype.h>
#include <alloc.h>

/* some constants */

#define MID_OFF 100             /* middle code OFFSET */
#define LAST_OFF 200

#define NO_CODE  0
#define OFF2     2              /* please,,Don't change this */
#define OFF0     0              /* and this.                 */

#define NOKEY   -1              /* for inkey function        */
#define WAIT     0
#define NOWAIT   1

#define HANMODE  1              /* if hangul mode, this is hgSUCCESS */
#define ENGMODE  0

#define CursREVS 0              /* cursor reverse check. */
#define CursNORM 1

#define _SW1     1
#define _SW0     0

#define EMPTY    1		/* don't change,too */
#define EMPTY2   2

#define MAXKEYFUNC 20           /* key function maximum */

#define CHOSUNG         0       /* for code convert */
#define JUNGSUNG        1
#define JONGSUNG        2

/* key index pointer */

unsigned char *key;
unsigned char *key_shifted;
unsigned char *first_last;
unsigned char *middle2;
unsigned char *last2;
unsigned char *key3_t;
unsigned char *key3_t_s;
unsigned char *key3;
unsigned char *key3_s;
unsigned char *first2;

int  h_code;             /*      hangul code      */
int  t_code;             /*      temp   code      */
char *inbuff;            /*      buffer           */
int  pos;                /*      buffer position  */
int  HANMETHOD=2;        /*      2-bul input rout.*/
int  mode=ENGMODE;
int  ins_flag=_SW1;
int  Xpos,Ypos;          /* global position in hgetsxy function */
int  in_max;             /* MAXIMUM return string length      */
int  savech;             /* for save ch       */

long cursor_rate=7;      /* for cursor blink  */
long timedelay;          /* for check time    */

char specinmode=0;       /* for special input */
char enginmode=0;        /* for english input */
char digitinmode=0;      /* for digit input   */

char hgCAttr=REVERSE;    /* default = REVERSE */
char hgCursColor=ZERO;

/* some function */

void complete();
void ins_proc();
void set_cursor(int attribute);
int  buf_len(char *str);
void ins_proc(int n);
void stosattr();
void recoverattr();
void hanin2_process(int ch);
void english_process(int ch);
void hanin3_process(int ch);
void in_null();
void internal_routine();
void hangulcode_convert(char *s,int i,unsigned char (*p)(unsigned char data,int tnum));
unsigned char findcodedata1(unsigned char data,int tnum);
unsigned char findcodedata1(unsigned char data,int tnum);
int  isfillcode(unsigned char data,int tnum);
int  isnocode(unsigned char data,int tnum);
void hgSetEngMode();

/* sangyong chohap hangul code table */

extern char far sangyongtable[3][33];
extern char *code_table[3];

/* function pointer */

void (*internal)()=&in_null;

char attr_temp1,attr_temp2;               /* for attribute saving */

union {
	hcode c;
	hchar b;
} hanin;

/* input-routine-key function define */

void insflag_proc(),bs_proc(),home_proc(),end_proc(),del_proc(),
     left_proc(),right_proc(),return_proc(),space_proc();

int keys[MAXKEYFUNC]={ INS,BS,HOME,END,DEL,LEFT,RIGHT,RETURN,ESC,SPACE };
void (*keyfunc[MAXKEYFUNC])()={ insflag_proc,bs_proc,home_proc,end_proc,
                                del_proc,left_proc,right_proc,return_proc,
                                return_proc,space_proc };
int keynum=10;

void hgRegisterKeyFunction(int keyvalue,void (*kf)())
{
        if (keynum>=MAXKEYFUNC) return;
        keys[keynum]=keyvalue;
        keyfunc[keynum]=kf;
        keynum++;
}
void hgSetCursAttr(char attr)
{
        hgCAttr=attr;
}
char hgGetCursAttr()
{
        return hgCAttr;
}
void hgSetCursColor(char color)
{
	hgCursColor=color;
}
char hgGetCursColor()
{
        return hgCursColor;
}

unsigned char last_by_first(int code)
{
	int i;

	for (i=0;i<16;i++)
                if (*(first_last+i*2)==code) return *(first_last+i*2+1);

	return NO_CODE;
}
unsigned char first_by_last(int code)
{
	int i;

	for (i=0;i<16;i++)
		if (*(first_last+i*2+1)==code) return *(first_last+i*2);

	return NO_CODE;
}
unsigned char last2_find(void)
{
	int i;

	for (i=0;i<11;i++)
		if (*(last2+i*3)==hanin.c.lasc && *(last2+i*3+1)==h_code)
			return *(last2+i*3+2);

	return NO_CODE;
}
unsigned char middle2_find(void)
{
	int i;

	for (i=0;i<7;i++)
		if (*(middle2+i*3)==hanin.c.midc && *(middle2+i*3+1)==h_code)
			return *(middle2+i*3+2);

	return NO_CODE;
}
unsigned char last2_find_mo(void)
{
	int i;

	for (i=0;i<11;i++)
		if (*(last2+i*3+2)==hanin.c.lasc) {
			hanin.c.lasc=*(last2+i*3);
			return *(last2+i*3+1);
		}

	return NO_CODE;
}
unsigned char middle2_find_mo(void)
{
	int i;

	for (i=0;i<7;i++)
		if (*(middle2+i*3+2)==hanin.c.midc) {
			hanin.c.midc=*(middle2+i*3);
			return *(middle2+i*3+1);
		}

	return NO_CODE;
}
unsigned char first2_find(void)
{
	int i;

	for (i=0;i<5;i++)
		if (*(first2+i*3)==hanin.c.firc && *(first2+i*3+1)==h_code)
			return *(first2+i*3+2);
	return NO_CODE;
}
unsigned char find3(int ch,int shift)
{
	int i;

	for (i=0;i<41;i++)
		switch(shift){
			case 0 :
				if (*(key3_t+i)==ch) return *(key3+i);
				break;
			case 1 :
				if (*(key3_t_s+i)==ch) return *(key3_s+i);
				break;
		}
	return NO_CODE;
}
void initCode(void)
{
	hanin.c.firc=1;         /* fill code */
	hanin.c.midc=2;
	hanin.c.lasc=1;
	hanin.c.dummy=0;
}
void ja_process(void)
{
	 int temp;

	 if (hanin.c.firc==EMPTY && hanin.c.midc==EMPTY2)
		hanin.c.firc=h_code;
	 else if (hanin.c.firc!=EMPTY && hanin.c.midc==EMPTY2){
		complete();
		hanin.c.firc=h_code;
	 }
	 else if (hanin.c.midc!=EMPTY2)
	 if (hanin.c.lasc==EMPTY)
		  if ((temp=last_by_first(h_code))!=NO_CODE)
			      hanin.c.lasc=temp;
		  else {
			      complete();
                              hanin.c.firc=h_code;
		  }
         else if ((temp=last2_find())==NO_CODE){
                  complete();
                  hanin.c.firc=h_code;
         }
	 else hanin.c.lasc=temp;
}

void mo_process(void)
{
	int temp;

	h_code-=MID_OFF;

	if (hanin.c.lasc!=EMPTY)
		if ((temp=last2_find_mo())==NO_CODE) {
			temp=first_by_last(hanin.c.lasc);
			hanin.c.lasc=EMPTY;
			complete();
			hanin.c.firc=temp;
			hanin.c.midc=h_code;
		}
		else {
			complete();
			hanin.c.firc=temp;
			hanin.c.midc=h_code;
		}
        else if (hanin.c.midc==EMPTY2) hanin.c.midc=h_code;
        else {
                        if ((temp=middle2_find())==NO_CODE) {
				complete();
				hanin.c.midc=h_code;
			}
			else hanin.c.midc=temp;
        }
}
void ja3_process(void)
{
    int temp;

    if (hanin.c.firc==EMPTY && hanin.c.midc==EMPTY2)
		if (hanin.c.lasc==EMPTY)
			hanin.c.firc=h_code;
		else {
			complete();
			hanin.c.firc=h_code;
		}
    else if (hanin.c.midc!=EMPTY2 || hanin.c.lasc!=EMPTY){
		complete();
		hanin.c.firc=h_code;
    }
    else if (hanin.c.firc!=EMPTY)
		if ((temp=first2_find())!=NO_CODE)
			hanin.c.firc=temp;
		else {
			complete();
			hanin.c.firc=h_code;
		}
}
void mo3_process(void)
{
	int temp;

	h_code-=MID_OFF;

	if (hanin.c.lasc!=EMPTY){
			complete();
			hanin.c.midc=h_code;
        }
        else if (hanin.c.midc==EMPTY2) hanin.c.midc=h_code;
        else {
			if ((temp=middle2_find())==NO_CODE) {
				complete();
				hanin.c.midc=h_code;
			}
			else hanin.c.midc=temp;
        }
}
void jong3_process(void)
{
	int temp;
	int medi;

	h_code-=LAST_OFF;

	if (hanin.c.midc==EMPTY2 && hanin.c.firc!=EMPTY){
		complete();
		hanin.c.lasc=h_code;
	}
	else if (hanin.c.lasc==EMPTY)
		hanin.c.lasc=h_code;
	else {
		medi=h_code;
		h_code=first_by_last(h_code);
		if ((temp=last2_find())==NO_CODE){
			h_code=medi;
			complete();
			hanin.c.lasc=h_code;
		}
		else hanin.c.lasc=temp;
		h_code=medi;
	}
}
void complete(void)
{
	set_cursor(CursNORM);
	hanin.c.dummy=1;
	inbuff[pos++]=hanin.b.fchar;
	inbuff[pos++]=hanin.b.schar;
	initCode();
        if (ins_flag==_SW1)
		if (buf_len(inbuff)<in_max) ins_proc(2);
		else pos-=2;
	else
		if (pos>=in_max-1) pos-=2;
}
void complete_2(void)
{
	set_cursor(CursNORM);
	hanin.c.dummy=1;
	inbuff[pos++]=hanin.b.fchar;
	inbuff[pos++]=hanin.b.schar;
	initCode();
}
int is_complete(void)
{
    if ((hanin.c.firc+hanin.c.midc+hanin.c.lasc)!=4) return OFF0;
    return OFF2;
}

void hgInitStr(char *s,int n)
{
        memset(s,0,n-1);
}

int is_hangul(void)
{
	int ret_mode,i;

	for (i=0;i<pos;)
		if (isascii(inbuff[i])) ret_mode=OFF0,i++;
                else ret_mode=OFF2,i+=2;

	return ret_mode;
}

int buf_len(char *str)
{
	int i;

	for (i=in_max;i>=0;i--)
		if (str[i]!=0) return i+1;

	return 0;
}

void closeStr(void)
{
      int l,i;

      l=buf_len(inbuff);
      inbuff[l]=0;
      for (i=0;i<=l-1;i++)
      if (inbuff[i]==0) inbuff[i]=32;
}

void set_cursor(int attribute)
{
	char temp[3];
	char eattr=0,hattr=0;
        char save;

        save=hgGetTFcolor();

        if (isascii(inbuff[pos])) {
				temp[0]=inbuff[pos];
				temp[1]=0;
				if (inbuff[pos]==0) temp[0]=' ';
	}
	else {
				temp[0]=inbuff[pos];
				temp[1]=inbuff[pos+1];
				temp[2]=0;
	}
	if (attribute==CursREVS){
		stosattr();
                eattr=hattr=hgCAttr;
                if (temp[0]==32 && !(hgCAttr & UNDER))
                       eattr=REVERSE,hattr=REVERSE;
		hgSetTFcolor(hgCursColor);
                hgSetEAttr(eattr);
                hgSetHAttr(hattr);
	}
        hgOverTextXy(Xpos+pos*8,Ypos,temp);
	if (attribute==CursREVS) {
                hgSetTFcolor(save);
		recoverattr();
	}
}
void stosattr()
{
	attr_temp1=hgGetEAttr();
	attr_temp2=hgGetHAttr();
}
void recoverattr()
{
	hgSetEAttr(attr_temp1);
	hgSetHAttr(attr_temp2);
}

void left_proc(void)
{
	if (pos<=0) return;
        set_cursor(CursNORM);
	if (!is_complete()) {
		complete_2();
		pos-=2;
	}
	if (is_hangul()) pos-=2;
	else pos--;
}
void right_proc(void)
{
	if (pos>=in_max-2+isascii(inbuff[pos])) return;
	set_cursor(CursNORM);
	if (!is_complete()) {
		complete_2();
		pos-=2;
	}
	if (isascii(inbuff[pos])) pos++;
	else pos+=2;
        if (inbuff[pos]==0) inbuff[pos]=32;
}
void ctrlleft_proc(void)
{
        while (1) {
                if (pos<=0) return;
                left_proc();
                if (inbuff[pos]==' ') break;
        };
        right_proc();
}
void ctrlright_proc(void)
{
        while (1) {
                if (pos>=in_max-2+isascii(inbuff[pos])) return;
                right_proc();
                if (inbuff[pos]==' ') break;
                if (inbuff[pos]==0) break;
        };
        left_proc();
}
void del_proc(void)
{
	int l,i;

	l=buf_len(inbuff);

	if (pos>=l) return;
	if (!is_complete()) {
		complete_2();
		pos-=2;
	}
	if (isascii(inbuff[pos])) {
				for (i=pos+1;i<=l-1;i++)
                                        inbuff[i-1]=inbuff[i];
                                for (i=l-1;i<in_max+4;i++)
                                        inbuff[i]=0;
                                hgOverTextXy(Xpos+(l-1)*8,Ypos," ");
                                hgOverTextXy(Xpos+pos*8,Ypos," ");
	}
	else {
		for (i=pos+2;i<=l-1;i++)
			inbuff[i-2]=inbuff[i];
		for (i=l-2;i<in_max+4;i++)
                        inbuff[i]=0;
                hgOverTextXy(Xpos+(l-2)*8,Ypos,"  ");
                hgOverTextXy(Xpos+pos*8,Ypos,"  ");
	}
        hgOverTextXy(Xpos+pos*8,Ypos,&inbuff[pos]);
}
void end_proc(void)
{
        int i,save;

        set_cursor(CursNORM);
	if (!is_complete()) complete_2();
	pos=buf_len(inbuff);
        if (pos>=in_max-1) {
                save=0;
                for (i=0;i<buf_len(inbuff);) {
			save=i;
                        if (inbuff[i]&0x80) i+=2;
			else i++;
                }
                pos=save;
        }
}
void home_proc(void)
{
        set_cursor(CursNORM);
	if (!is_complete()) complete_2();
	pos=0;
}
void bs_proc(void)
{
	int temp;

	set_cursor(CursNORM);
	if (is_complete()){
		if (pos<=0) return;
		if (!is_hangul()) {
			pos--;
			del_proc();
		}
		else {
			pos-=2;
			del_proc();
		}
	}
	else {
		if (hanin.c.lasc!=EMPTY){
			if (last2_find_mo()==NO_CODE)
				hanin.c.lasc=EMPTY;
		}
        else if (hanin.c.midc!=EMPTY2){
			if (middle2_find_mo()==NO_CODE)
                 hanin.c.midc=EMPTY2;
		}
		else if (hanin.c.firc!=EMPTY) hanin.c.firc=EMPTY;
		if (is_complete()) {
						del_proc();
						goto bs_next;
		}
		inbuff[pos]=hanin.b.fchar;
		inbuff[pos+1]=hanin.b.schar;
bs_next:        hgOverTextXy(Xpos+pos*8,Ypos,&inbuff[pos]);
		set_cursor(CursREVS);
	}
}

void ins_proc(int n)
{
	int i,l;

	l=buf_len(inbuff);

	if (n==1) {
		for (i=l-1;i>=pos;i--)
			inbuff[i+1]=inbuff[i];
		inbuff[pos]=SPACE;
	}
	else {
		for (i=l-1;i>=pos;i--)
			inbuff[i+2]=inbuff[i];
		inbuff[pos]=SPACE;
		inbuff[pos+1]=SPACE;
	}
}
void insflag_proc()
{
	set_cursor(CursNORM);
	ins_flag=1-ins_flag;
	if (!is_complete()) {
                  complete_2();
                  if (pos>=in_max) pos-=2;
        }
}
void return_proc()
{
        set_cursor(CursNORM);
        if (!is_complete()) complete_2();
        closeStr();
}

void space_proc()
{
        char bios;

        set_cursor(CursNORM);
	bios=bioskey(2);bios&=0x7f;
	if (!is_complete()) {
                 complete_2();
		 if (pos>=in_max) pos-=2;
        }
        if (bios & LEFT_SHIFT || bios & RIGHT_SHIFT) mode=HANMODE-mode;
	else english_process(' ');      /* for space input */
}

int key_process(int ch)
{
        int ret_flag=_NORMAL_KEYCODE;        /* for no match */
        int i;
        int *dis;               /* disable key  */

        dis=hgGetDisableKey();

        for (i=0;i<hgGetNumDisableKey();i++)
                if (dis[i]==ch) return _DISABLE_KEYCODE;

        for (i=keynum-1;i>=0;i--)
                if (keys[i]==ch) {
                        (*keyfunc[i])();
                        ret_flag=_SPECIAL_KEYCODE;
                        break;
                }
        if (ch==RETURN || ch==ESC) ret_flag=_EXIT_KEYCODE;
        return ret_flag;
}

void code_convert(char *s,unsigned char (*p)(unsigned char data,int tnum))
{
        int i;

        for (i=0;i<strlen(s);) {
                if (s[i]&0x80) {
                        hangulcode_convert(s,i,p);
                        i+=2;
                } else  i++;
        }
}
void hangulcode_convert(char *s,int i,unsigned char (*p)(unsigned char data,int tnum))
{
        unsigned char data;

        hanin.b.fchar=s[i];
        hanin.b.schar=s[i+1];

        data=hanin.c.firc;
        hanin.c.firc=(*p)(data,CHOSUNG);
        data=hanin.c.midc;
        hanin.c.midc=(*p)(data,JUNGSUNG);
        data=hanin.c.lasc;
        hanin.c.lasc=(*p)(data,JONGSUNG);

        s[i]=hanin.b.fchar;
        s[i+1]=hanin.b.schar;
}
int isfillcode(unsigned char data,int tnum)
{
        if (code_table[tnum][32]==data) return hgSUCCESS;
        return hgFAIL;
}
int isnocode(unsigned char data,int tnum)
{
        if (isfillcode(data,tnum)) return hgFAIL;
        if (code_table[tnum][data]==0) return hgSUCCESS;
	return hgFAIL;
}

/* ----------------------------- */
/* various code to sangyong code */
/* ----------------------------- */

unsigned char findcodedata1(unsigned char data,int tnum)
{
        int i;
        unsigned char temp=data;        /* save */

        if (isnocode(data,tnum)) return data;
	if (isfillcode(data,tnum)) return sangyongtable[tnum][32];

        data=code_table[tnum][data];

        if (data==0) return temp;    /* no code */
        else {
                for (i=0;i<32;i++)
                   if (sangyongtable[tnum][i]==data) return i;
        }
        return 0;
}

/* ----------------------------- */
/* sangyong code to various code */
/* ----------------------------- */

unsigned char findcodedata2(unsigned char data,int tnum)
{
        int i;
        unsigned char temp=data;        /* save      */

        sangyong_code();
        if (isnocode(data,tnum)) {
                hgResetCodeTable();
                return data;
        }
        if (isfillcode(data,tnum)) {
                hgResetCodeTable();
                return code_table[tnum][32];
        }
        data=sangyongtable[tnum][data];
        hgResetCodeTable();
        if (data==0) return temp;       /* no code */
        else {
                for (i=0;i<32;i++)
                   if (code_table[tnum][i]==data) return i;
        }
        return 0;
}
void settimerzero()
{
        union REGS in,out;

        in.h.ah=0;
        int86(0x1a,&in,&out);
        timedelay=(unsigned long)out.x.cx*65536+(long)out.x.dx;
}
long gettimerticks()
{
        union REGS in,out;
        long diff;

        in.h.ah=0;
        int86(0x1a,&in,&out);
        diff=(unsigned long)out.x.cx*65536+(long)out.x.dx-timedelay;

        return diff;
}
int hgInTextXy(int x,int y,char *retn_str,int max)
{
	int ch;
	int tpos;
	char bios;
	int t;
	int i;
	int i2;
	int *ext;
	int extflag;
	int xfac,yfac;
	long saveticks;
	int reversemode=1;
	int ishan=0;
        int savee,saveh;
        int modesave;

        /* save attribute & factor */

        savee=hgGetEAttr();
        saveh=hgGetHAttr();

        xfac=hgGetXFactor();
        yfac=hgGetYFactor();

        hgSetXFactor(1);hgSetYFactor(1);

	Xpos=x;Ypos=y;
	in_max=max;

	pos=0;

        code_convert(retn_str,&findcodedata1);
        sangyong_code();        /* sangyong code table */

        hgHideMouse();          /* for safe */
        hgMouseInFlagOff();

	inbuff=(char *)malloc((size_t)in_max+4);
	hgInitStr(inbuff,in_max+4);

        if (strlen(retn_str)) {
                memcpy(inbuff,retn_str,strlen(retn_str));
                for (i=strlen(retn_str);i<(in_max+4);i++)
                        inbuff[i]=0;
        }

        if (specinmode) {
                hgSetEAttr(REVERSE);
                hgSetHAttr(REVERSE);
                pos=buf_len(inbuff);
                if (pos>=in_max-1) pos=in_max-1;
        }

	hgOverTextXy(Xpos,Ypos,inbuff);

        if (specinmode) {
                hgSetEAttr(savee);
                hgSetHAttr(saveh);
                ishan=1;                /* for once */
        }

	initCode();
	ext=hgGetExtKey();

	while (1){
		set_cursor(CursREVS);
                settimerzero();
                reversemode=1;
		do {
                        saveticks=gettimerticks();
                        if (saveticks>cursor_rate && cursor_rate!=0) {
                                reversemode=1-reversemode;
                                if (reversemode) set_cursor(CursREVS);
                                else set_cursor(CursNORM);
                                settimerzero();
                        }
                        internal_routine();
                        ch=inkey(NOWAIT);
			savech=ch;
			internal_routine();
		} while (ch==NOKEY);
                set_cursor(CursNORM);

		extflag=-1;

		for (i=0;i<hgGetNumExtKey();i++)
                        if (ext[i]==ch) return_proc(),extflag=_EXIT_KEYCODE;

                if (extflag!=_EXIT_KEYCODE) {
                        modesave=mode;  /* now mode */
                        extflag=key_process(ch);
                        if (extflag==_SPECIAL_KEYCODE && modesave==mode
                            && ch==SPACE && ishan) {
                                hgInitStr(inbuff,max+4);
                                hgBoxFill(Xpos,Ypos,Xpos+(max-1)*8+7,Ypos+15,hgGetTBcolor());
                                ishan=0;
                                pos=0;
                                english_process(' ');
                        }
                        if ((extflag==_SPECIAL_KEYCODE && ishan && ch!=SPACE)) {
                                hgOverTextXy(Xpos,Ypos,inbuff);
                                ishan=0;
                        }
                }

                hgHideMouse();  /* for safe */

                if (extflag==_EXIT_KEYCODE) {
                          if (ishan) {
                                hgOverTextXy(Xpos,Ypos,inbuff);
                                ishan=0;
                          }
			  internal_routine();
			  i2=buf_len(inbuff);
			  for (i=0;i<=i2;i++)
				retn_str[i]=inbuff[i];
			  retn_str[i]=0;
                          hgResetCodeTable();
                          code_convert(retn_str,&findcodedata2);
			  free(inbuff);
                          hgSetXFactor(xfac);
                          hgSetYFactor(yfac);
                          hgShowMouse();
                          hgMouseInFlagOn();
			  return ch;
		}
                if (extflag==_NORMAL_KEYCODE) {
                          if (ishan) {
                                hgInitStr(inbuff,max+4);
                                hgBoxFill(Xpos,Ypos,Xpos+(max-1)*8+7,Ypos+15,hgGetTBcolor());
                                ishan=0;
                                pos=0;
                          }
                          if (ch>=32 && ch<=255) {
                                  if (!enginmode && !digitinmode ) {
                                          if (HANMETHOD==2) hanin2_process(ch);
                                          else hanin3_process(ch);
                                  } else if (enginmode) english_process(ch);
                                         else if (ch>='0' && ch<='9')
                                                english_process(ch);
                          }
                }
	};
}
void outputnew()
{
        int i,i2,tpos;
        char temp[4];

        if (ins_flag==_SW0)
                if (isascii(inbuff[pos]) && inbuff[pos+1]&0x80)
                        inbuff[pos+2]=SPACE;

        hanin.c.dummy=1;
        inbuff[pos]=hanin.b.fchar;
        inbuff[pos+1]=hanin.b.schar;

        i2=is_hangul();
        tpos=(pos>2) ? pos-i2:0;
        if (i2!=0 && pos>=1) {
                for (i=0;i<i2;i++)
                        temp[i]=inbuff[tpos+i];
                temp[i]=0;
                hgOverTextXy(Xpos+tpos*8,Ypos,temp);
        }
        if (ins_flag) {
                if (pos+2<in_max)
                        hgOverTextXy(Xpos+(pos+2)*8,Ypos,&inbuff[pos+2]);
        }
        else {
                temp[0]=inbuff[pos];
                temp[1]=inbuff[pos+1];
                if (inbuff[pos+2]==' ') temp[2]=inbuff[pos+2];
                else temp[2]=0;
                temp[3]=0;
                hgOverTextXy(Xpos+pos*8,Ypos,temp);
        }
}
void hanin2_process(int ch)
{
	char bios;

	if (mode==HANMODE && isalpha(ch)){
		if (pos==in_max-1) return;
		if (is_complete() && ins_flag==_SW1)
		      if (buf_len(inbuff)<in_max)
			      ins_proc(2);
		      else return;
		bios=bioskey(2);bios&=0x7f;
		if (bios & LEFT_SHIFT || bios & RIGHT_SHIFT)
		      h_code=key_shifted[toupper(ch)-'A'];
		else
		      h_code=key[toupper(ch)-'A'];
		if (h_code>MID_OFF) mo_process();
		else ja_process();
                outputnew();
	}
	else english_process(ch);
}
void english_process(int ch)
{
        char temp[2];

        set_cursor(CursNORM);
        if (mode==HANMODE) {
		 if (!is_complete()) {
			complete_2();
			if (pos>=in_max) pos-=2;
		 }
	}
	if (ins_flag==_SW1)
		 if (buf_len(inbuff)<in_max)
			ins_proc(1);
		 else return;
	if (!isascii(inbuff[pos])) inbuff[pos+1]=SPACE;
        inbuff[pos]=ch;

        temp[0]=ch;
        temp[1]=0;

        if (ins_flag)  hgOverTextXy(Xpos+pos*8,Ypos,&inbuff[pos]);
        else hgOverTextXy(Xpos+pos*8,Ypos,temp);

        if (pos<(in_max-1)) pos++;
}
void hanin3_process(int ch)
{
	char bios;

        bios=bioskey(2);bios&=0x7f;
	h_code=find3(toupper(ch),(bios & LEFT_SHIFT || bios & RIGHT_SHIFT));
	if (h_code==NO_CODE) english_process(ch);
	else {
		 if (mode==HANMODE && h_code!=0){
                         if (h_code<MID_OFF && h_code>30) {
                                ch=h_code;
                                h_code=0;
                                english_process(ch);
                                return;
                         }
			 if (pos==in_max-1) return;
			 if (is_complete() && ins_flag==_SW1)
			 if (buf_len(inbuff)<in_max)
				ins_proc(2);
			 else return;

                         if (h_code>MID_OFF && h_code<LAST_OFF) mo3_process();
			 else if (h_code>LAST_OFF) jong3_process();
			 else if (h_code<MID_OFF) ja3_process();
                         outputnew();
		}
		else english_process(ch);
	}
}
void hgSetEngMode(void)
{
	mode=ENGMODE;
}
void hgSetHanMode(void)
{
	mode=HANMODE;
}
void hgSetInMethod(int ch)
{
	HANMETHOD=ch;
}
int hgGetInMethod()
{
        return HANMETHOD;
}
int  hgGetInStatus()
{
        if (mode==ENGMODE) return hgENGLISH;
        else if (HANMETHOD==2) return hgHAN2;
        else return hgHAN3;
}
void internal_routine()
{
        (*internal)();
}
void in_null()          /* null function */
{
}
void hgSetInternal(void (*func)())
{
        internal=func;
}
int hgGetPos()
{
	return pos;
}
int hgGetCh()
{
        return savech;
}
void hgSetInsertOn()
{
        ins_flag=_SW1;
}
void hgSetInsertOff()
{
        ins_flag=_SW0;
}
int hgIsInsertOn()
{
        return ins_flag;
}
void hgSetCursorRate(long rate)
{
        cursor_rate=rate;
}
void hgSpecInModeOn()
{
        specinmode=1;
}
void hgSpecInModeOff()
{
        specinmode=0;
}
void hgEngInModeOn()
{
        enginmode=1;
}
void hgEngInModeOff()
{
        enginmode=0;
}
void hgDigitInModeOn()
{
        digitinmode=1;
}
void hgDigitInModeOff()
{
        digitinmode=0;
}
