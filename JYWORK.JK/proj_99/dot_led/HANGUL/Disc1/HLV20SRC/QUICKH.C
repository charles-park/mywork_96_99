/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           Quick Help Module            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include <stdio.h>
#include <alloc.h>

#include "hwindow.h"

long keysearch(FILE *idx,char *key)
{
	char temp[80];
	long offset;
	int  i;

	fseek(idx,0,0);
	while (fgets(temp,80,idx)!=NULL) {
		for (i=0;i<strlen(temp);i++)
			if (temp[i]==',') break;
		offset=atol(&temp[i+1]);
        temp[i]=0;
        if (!strcmp(temp,key)) return offset;
	}
	return -1L;
}

void hgPrtHelpBoxXy(int x,int y,char *items[],int xw,int yw,char *title)
{
        HHELP *p;

        p=hgHHELP_Load(items,xw,yw);
        hgHHELP_Draw(p,x,y,title);
        hgHHELP_Free(&p);
}

int hgFileHelpBoxXy(char *h,char *i,int x,int y,int xw,int yw,char *key)
{
        long offset;
        char *buff[MAXHELPLINE];
        char  temp[255];                /* max string length */
        FILE *idx,*hlp;
        char  keys[60];
        int   bpos=0;
        int   k;

        idx=fopen(i,"r");
        if (idx==NULL) return -1;

        strcpy(keys,"$");
        strcat(keys,key);

        offset=keysearch(idx,keys);
        fclose(idx);

        if (offset==-1L) return -1;

        hlp=fopen(h,"r");

        if (hlp==NULL) return -1;
        fseek(hlp,offset,0);                  /* set position */

        while (fgets(temp,255,hlp)!=NULL) {
		if (temp[0]=='$') break;
		temp[strlen(temp)-1]=0;
                if (temp[0]=='/' && strlen(temp)==1) strcpy(temp," ");
		buff[bpos]=(char *)malloc((size_t)strlen(temp)+2);
		strcpy(buff[bpos],temp);
		bpos++;
	}
        fclose(hlp);

        buff[bpos]=(char *)malloc(3);
        buff[bpos][0]=0;
        buff[bpos][1]=0;

        hgPrtHelpBoxXy(x,y,buff,xw,yw,key);

        for (k=0;k<=bpos;k++)
                free(buff[k]);

        return 1;
}
void hgPrtHelpBoxXyM(char *items[],int xw,int yw,char *title)
{
        int x=(hgGetx2r()+hgGetx1r())/2-(xw*8+44)/2;
        int y=(hgGety2r()+hgGety1r())/2-(yw*16+48)/2;

        hgPrtHelpBoxXy(x,y,items,xw,yw,title);
}
int hgFileHelpBoxXyM(char *h,char *i,int xw,int yw,char *title)
{
        int x=(hgGetx2r()+hgGetx1r())/2-(xw*8+44)/2;
        int y=(hgGety2r()+hgGety1r())/2-(yw*16+48)/2;
        int flag;

        flag=hgFileHelpBoxXy(h,i,x,y,xw,yw,title);
        return flag;
}
