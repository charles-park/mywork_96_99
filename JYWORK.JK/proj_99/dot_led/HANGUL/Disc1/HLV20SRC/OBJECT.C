/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Object Drawing              */
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

#include "hghlib.h"
#include "hginit.h"
#include "hwindow.h"

/* for default set */

int dft_bstyle[MAXSTYLE][4]={ {WHITE,ZERO,ZERO,1},
                              {LIGHTGRAY,ZERO,ZERO,1},
                              {LIGHTGRAY,WHITE,DARKGRAY,1},
                              {LIGHTGRAY,DARKGRAY,WHITE,1},
                              {BLUE,WHITE,DARKGRAY,1},
                              {LIGHTGRAY,WHITE,DARKGRAY,2},
                              {LIGHTGRAY,ZERO,WHITE,2},
                              {LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,1}};

int dspos=7;

/* external prototype */

void putmark(int x,int y);
void putmend(int x,int y);

/* style adding */

int hgAddStyle(int back,int color1,int color2,int deep)
{
        dspos++;
        if (dspos>=MAXSTYLE) return -1; /* for error */
        dft_bstyle[dspos][0]=back;
        dft_bstyle[dspos][1]=color1;
        dft_bstyle[dspos][2]=color2;
        dft_bstyle[dspos][3]=deep;
        return dspos;
}
void hgAdjustStyle()
{
        int data[MAXSTYLE][4]={{63,0,0,1},
                               {42,0,0,1},
                               {42,63,21,1},
                               {42,21,63,1},
                               {42,63,21,1},
                               {42,63,21,2},
                               {42,0,63,2},
                               {42,42,42,1}};
        int i,j;

        for (i=0;i<8;i++)
                for (j=0;j<4;j++) {
                        if (data[i][j]==63) data[i][j]=whitec;
                        else if (data[i][j]==0) data[i][j]=zeroc;
                        else if (data[i][j]==42) data[i][j]=lightgrayc;
                        else if (data[i][j]==21) data[i][j]=darkgrayc;
                        dft_bstyle[i][j]=data[i][j];
                }
}

/* fundamental definition */

void hgBSTYLE_Load(BSTYLE *p,int back,int color1,int color2,int deep)
{
        p->back=back;
        p->color1=color1;
        p->color2=color2;
        p->deep=deep;
}
void hgWCOLOR_Load(WCOLOR *p,int border,int tfore,int tback,int back)
{
        p->border=border;
        p->tfore=tfore;
        p->tback=tback;
        p->back=back;
}
void hgBCOLOR_Load(BCOLOR *p,int border,int back)
{
        p->border=border;
        p->back=back;
}
void hgWIDTH_Load(WIDTH *p,int xwidth,int ywidth)
{
        p->xwidth=xwidth;
        p->ywidth=ywidth;
}

/* object definition */

HWINDOW *hgHWINDOW_Load(char *t,WCOLOR c,WIDTH s)
{
        HWINDOW *p;

        p=(HWINDOW *)malloc(sizeof(HWINDOW));
        p->titl=(char *)malloc((size_t)(strlen(t)+2));
        strcpy(p->titl,t);

        p->color=c;
        p->size=s;

        return p;
}

void hgHWINDOW_Free(HWINDOW **p)
{
        free((*p)->titl);
        free(*p);
}
void hgHWINDOW_Clrscr(HWINDOW *p,int x,int y)
{
        int xx,yy;

        xx=p->size.xwidth;
        yy=p->size.ywidth;

        hgBoxFill(x+13,y+33,x+xx-13,y+yy-13,WHITE);
}
void hgHWINDOW_ScrUp(HWINDOW *p,int x,int y,int n)
{
        int xx,yy;

        xx=p->size.xwidth;
        yy=p->size.ywidth;

        hgScrUp(x+13,y+33,x+xx-13,y+yy-13,n,WHITE);
}
void hgHWINDOW_ScrDown(HWINDOW *p,int x,int y,int n)
{
        int xx,yy;

        xx=p->size.xwidth;
        yy=p->size.ywidth;

        hgScrDown(x+13,y+33,x+xx-13,y+yy-13,n,WHITE);
}

HBOX *hgHBOX_Load(char *i[MAXITEM],WCOLOR c,WIDTH w)
{
        HBOX *p;
	int pointer=0;

        p=(HBOX *) malloc(sizeof(HBOX));

        while (strcmp(i[pointer],"")) {
                p->items[pointer]=(char *)malloc((size_t)(strlen(i[pointer])+2));
                strcpy(p->items[pointer],i[pointer]);
                pointer++;
	}

        p->items[pointer]=(char *)malloc((size_t)3);
        strcpy(p->items[pointer],"");

        p->color=c;
        p->size=w;

        return p;
}

void hgHBOX_Free(HBOX **p)
{
	int pos=0;
        while (strcmp((*p)->items[pos],""))
                free((*p)->items[pos++]);
        if ((*p)->items[pos]) free((*p)->items[pos]);
        free (*p);
}

HBAR *hgHBAR_Load(BCOLOR c,WIDTH w)
{
        HBAR *p;

        p=(HBAR *) malloc(sizeof(HBAR));

        p->color=c;
        p->size=w;

        return p;
}
void hgHBAR_Free(HBAR **p)
{
        free(*p);
}

HBUTTON *hgHBUTTON_Load(char *i,WCOLOR c,WIDTH s)
{
        HBUTTON *p;

        p=(HBUTTON *) malloc(sizeof(HBUTTON));

        p->item=(char *)malloc((size_t)(strlen(i)+2));
        strcpy(p->item,i);

        p->color=c;
        p->size=s;

        return p;
}

void hgHBUTTON_Free(HBUTTON **p)
{
        free((*p)->item);
        free(*p);
}

void hgHWINDOW_Draw(HWINDOW *p,int x,int y)
{
        int xx,yy;
        int bd,back,fore;
        int tback;
        int temp,temp1,temp2,temp3,temp4;
        int save;
        int wback;

        unsigned xoffset;

        xx=p->size.xwidth-1;
        yy=p->size.ywidth-1;
        bd=p->color.border;
        fore=p->color.tfore;
        tback=p->color.tback;
        wback=p->color.back;
        
        temp=dft_bstyle[4][0];
        dft_bstyle[4][0]=tback;

        hgHideMouse();

        hgBoxFill(x,y,x+xx,y+32,dft_bstyle[bd][0]);
        hgBoxFill(x,y+yy-12,x+xx,y+yy,dft_bstyle[bd][0]);
        hgBoxFill(x,y+32,x+12,y+yy-12,dft_bstyle[bd][0]);
        hgBoxFill(x+xx-12,y+32,x+xx,y+yy-12,dft_bstyle[bd][0]);

        hgDrawBorder(x,y,x+xx,y+yy,dft_bstyle[bd],NOFILL);

        temp1=dft_bstyle[3][1];temp2=dft_bstyle[3][2];
        dft_bstyle[3][1]=dft_bstyle[bd][2];
        dft_bstyle[3][2]=dft_bstyle[bd][1];
        hgDrawBorder(x+8,y+8,x+xx-8,y+yy-8,dft_bstyle[3],NOFILL);

        hgDrawBorder(x+8,y+8,x+8+16,y+8+20,dft_bstyle[bd],FILL);
        hgDrawBorder(x+xx-8-16,y+8,x+xx-8,y+8+20,dft_bstyle[bd],FILL);
        hgDrawBorder(x+8+4,y+8+20+4,x+xx-8-4,y+yy-8-4,dft_bstyle[wback],FILL);

        temp3=dft_bstyle[4][1];temp4=dft_bstyle[4][2];
        dft_bstyle[4][1]=dft_bstyle[bd][1];
        dft_bstyle[4][2]=dft_bstyle[bd][2];
        hgDrawBorder(x+8+16,y+8,x+xx-8-16,y+8+20,dft_bstyle[4],FILL);

        dft_bstyle[3][1]=temp1;
        dft_bstyle[3][2]=temp2;
        dft_bstyle[4][1]=temp3;
        dft_bstyle[4][2]=temp4;

        putmark(x+8,y+8);
        putmark_R(x+xx-8-16,y+8);

        dft_bstyle[4][0]=temp;

        save=hgGetTFcolor();            /* save foreground */
        hgSetTFcolor(fore);
        xoffset=((x+8+16)+(x+xx-8-16))/2;
        xoffset=xoffset-strlen(p->titl)*4;
        hgOutTextXy(xoffset,y+10,p->titl);
        hgSetTFcolor(save);             /* restore foreground */

        hgShowMouse();
}

void hgHBOX_Draw(HBOX *p,int x,int y)
{
        int xx,yy;
        int bd;
        int i,count=0;

        xx=p->size.xwidth;
        yy=p->size.ywidth;
        bd=p->color.border;

        hgHideMouse();
        hgDrawBorder(x,y,x+xx,y+yy,dft_bstyle[bd],FILL);

        for (i=y+4;;i+=(20*hgGetYFactor()))  {
                hgOutTextXy(x+6,i,p->items[count++]);
                if (!strcmp(p->items[count],"")) break;
	}
        hgShowMouse();
}
void hgHBOX_Save(HBOX *p,int x,int y)
{
        int xx,yy;

        xx=p->size.xwidth;
        yy=p->size.ywidth;

        hgGetImageVIRTUAL(x,y,x+xx,y+yy,&p->buff);
}
void hgHBOX_Restore(HBOX *p,int x,int y)
{
        int xx,yy;
        int j;

        xx=p->size.xwidth;
        yy=p->size.ywidth;

        hgPutImageVIRTUAL(x,y,x+xx,y+yy,p->buff);
        hgFreeVIMAGE(&p->buff);
}
void hgHBAR_Draw(HBAR *p,int x,int y,int option)
{
	int xx,yy;
        int bd;
        int j;

        xx=p->size.xwidth;
        yy=p->size.ywidth;
        bd=p->color.border;

        hgHideMouse();
        if (option==DRAW) {
                hgGetImage4(x,y,x+xx,y+yy,&(p->buff));
                hgDrawBorder(x,y,x+xx,y+yy,dft_bstyle[bd],NOFILL);
        }
        else {
                hgPutImage4(x,y,x+xx,y+yy,p->buff);
                hgFreeCIMAGE(&(p->buff));
        }
        hgShowMouse();
}
void hgHBUTTON_Draw(HBUTTON *p,int x,int y,int option)
{
        int xx,yy;
        int bd,bk;
        int xoffset;

        xx=p->size.xwidth;
        yy=p->size.ywidth;
        bd=p->color.border;
        bk=p->color.back;

        hgHideMouse();

	if (option==NOPUSH)
                hgDrawBorder(x,y,x+xx,y+yy,dft_bstyle[bd],FILL);
	else
                hgDrawBorder(x,y,x+xx,y+yy,dft_bstyle[bk],FILL);

        xoffset=x+xx/2-strlen(p->item)*pw(2,hgGetXFactor()-1)*4;
        hgOutTextXy(xoffset,y+3,p->item);
        hgShowMouse();
}
