/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           GraphicChar Input            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hwindow.h"
#include "hghlib.h"

#include <alloc.h>
#include <ctype.h>

/* extern varible from hanin.c */

extern char    *inbuff;
extern int      pos;
extern int      in_max;
extern int      Xpos,Ypos;
extern int      is_complete();
extern void     complete_2();
extern char     graphokflag;
extern int      isgraphic(char *str,int pos);

/* prototype */

void graphin_process();
void graphchoose_process();
int  choose_graphic();
int  sgpos=0;

int  sspos[7]={0,};
int  sobjt[7]={0,};
int  sapos[7]={0,};

/* global name */

char    *selectg[]={ " ¬q¥¡‹aŸ±¢…¸a ",
                     " •¡Ñw¢…¸a ",
                     " ¸åˆbµw¢…,Ðe‹i{¸a ",
                     " ¡ a®•¸a,‰á¬å¢…¸a ",
                     " ”e¶á‹¡Ñ¡,œaË¥(1)  ",
                     " œaË¥(2) ,·©¥¥(1)  ",
                     " ·©¥¥(2) ,œá¯¡´a   ","" };

void hgGraphicCharInInit(int keyvalue1,int keyvalue2)
{
        if (!graphokflag) return;
        hgRegisterKeyFunction(keyvalue1,&graphchoose_process);
        hgRegisterKeyFunction(keyvalue2,&graphin_process);
}

void graphchoose_process()
{

        int temp;

        hgMouseInFlagOn();
        hgHercBarOn();
        hgSetSaveOn();
        temp=hgSelectXyM(selectg,sgpos);
        hgSetSaveOff();
        hgHercBarOff();
        hgHideMouse();
        hgMouseInFlagOff();
        if (temp!=-1) sgpos=temp;
}

void graphin_process()
{
        unsigned fontID[]={ 0xd480,0xd931,0xda31,0xdb31,0xdc31,0xdd31,0xde31 };
        unsigned addoff[]={ 125,162,188,162,188,177,175 };
        unsigned sblank[]={   1,  1,  0,  0,  0,  0,  0 };
        unsigned char start=0x31;
        unsigned char id;

        int      i;
        int      retflag;
        char    *items[_MAXDIR];
        char     ret[10];
        int      x,y;
        HDIRBOX *p;
        WIDTH    w={ 280,178 };

        if ((pos+2)>=in_max) return;
        if (hgIsInsertOn())
                if (inbuff[in_max-1]!=0 || inbuff[in_max-2]!=0) return;

        id=(fontID[sgpos]>>8)&0xff;
        if (id==0xd4) start=0x80;

        for (retflag=0;retflag<(addoff[sgpos]+sblank[sgpos]);retflag++) {
                items[retflag]=(char *)malloc(3);
                items[retflag][0]=id;
                items[retflag][1]=start;
                items[retflag][2]=0;
                start++;
                if (id!=0xd4 && start==0x78) start=0x8a;
        }
        items[retflag]=(char *)malloc(3);
        items[retflag][0]=0;
        items[retflag][1]=0;
        items[retflag][1]=0;

        x=(hgGetx2r()+hgGetx1r())/2-w.xwidth/2;
        y=(hgGety2r()+hgGety1r())/2-w.ywidth/2;

        hgMouseInFlagOn();
        hgSetRecPosOn();

        hgChangeDirBox(sspos[sgpos],sobjt[sgpos],sapos[sgpos]);

        hgSetSaveOn();
        hgHercBarOn();
        p=hgHDIRBOX_Load(items,w,8,6);
        hgHDIRBOX_Choose(p,x,y,selectg[sgpos],ret);
        hgHDIRBOX_Free(&p);
        hgHercBarOff();

        hgGetDirBox(&sspos[sgpos],&sobjt[sgpos],&sapos[sgpos]);

        hgSetSaveOff();
        hgRestore();
        hgHideMouse();

        if (!strcmp(ret,"")) return;

        if (!is_complete())
                complete_2();

        if (hgIsInsertOn()) {
                if (inbuff[in_max-1]==0 && inbuff[in_max-2]==0) {
                        for (i=in_max;i>=pos+2;i--)
                                inbuff[i]=inbuff[i-2];
                        inbuff[pos]=ret[0];
                        inbuff[pos+1]=ret[1];
                }
        } else  {
                if (isascii(inbuff[pos]) && inbuff[pos+1]&0x80) inbuff[pos+2]=32;
                inbuff[pos]=ret[0];
                inbuff[pos+1]=ret[1];
        }

        for (retflag=0;retflag<=(addoff[sgpos]+sblank[sgpos]);retflag++)
                free(items[retflag]);

        hgOverTextXy(Xpos+pos*8,Ypos,&inbuff[pos]);
        pos+=2;
        if (pos>=in_max) pos=in_max-2;

        hgHideMouse();
        hgMouseInFlagOff();
        hgSetRecPosOff();
}
