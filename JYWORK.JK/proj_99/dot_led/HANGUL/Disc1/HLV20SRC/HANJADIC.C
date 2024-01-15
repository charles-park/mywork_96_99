/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           HanjaDictionary              */
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
#include <string.h>

#define MAXDICSTR 30            /* max string in dictionary */

char hdfname[30];
FILE *hdf;
int  hdfokflag=0;

void hanjadic_process();

/* extern varible from hanin.c */

extern char *inbuff;
extern int   pos;
extern int   in_max;
extern int   Xpos,Ypos;
extern int   is_complete();
extern void  complete_2();

void hgHanjaDicInit(char *fname,int keyvalue)
{
        strcpy(hdfname,fname);
        hdf=fopen(hdfname,"r");
        if (hdf==NULL) return;
        hdfokflag=1;
        hgRegisterKeyFunction(keyvalue,&hanjadic_process);
}
void hanjadic_process()
{
        char comp[MAXDICSTR],repl[MAXDICSTR];
        int  length;
        int  i;

        fseek(hdf,0,0); /* set first */

        while (fscanf(hdf,"%s",comp)!=EOF) {
                fscanf(hdf,"%s",repl);
                for (i=0;i<strlen(comp);i++)
                        if (comp[i]=='\n') comp[i]=0;
                for (i=0;i<strlen(repl);i++)
                        if (repl[i]=='\n') repl[i]=0;

                length=strlen(comp);
                if ((pos-length+2<0) || (pos-length+2)>in_max) return;
                if (!strncmp(&inbuff[pos-length+2],comp,length)) {
                        if (!is_complete()) {
                                complete_2();
                                pos-=2;
                        }
                        for (i=0;i<strlen(comp);i++)
                                inbuff[pos-length+2+i]=repl[i];
                        hgOverTextXy(Xpos+(pos-length+2)*8,Ypos,
                                     &inbuff[pos-length+2]);
                        pos+=2;
                        if (pos>=in_max) pos=in_max-2;
                }
        }
}
void hgHanjaDicEnd()
{
        if (hdfokflag) fclose(hdf);
}
