/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*             File Box Icon              */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     upgraded by Kim kwan-hyuk.         */
/*     Thanks! (Hitel ID : kkhlove)       */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hwindow.h"
#include "hghlib.h"
#include "hgquick.h"

#include <dir.h>
#include <alloc.h>
#include <dos.h>
#include <io.h>
#include <string.h>

/* sub function prototype */

void directory(int x,int y,char *mask,char *ret);
void add(char *ret);

char *direc[_MAXDIR];
char file[50];
char rett[50];
int   dirpos;
HDIRBOX *p;

extern int saveflag;

void    hgSelectDirBoxXy(int x,int y,char *items[],char *title,char *ret)
{

    WIDTH    w={ 494,194 };
    int      i;

    p=hgHDIRBOX_Load(items,w,4,7);
    hgHDIRBOX_Choose(p,x,y,title,ret);
    hgHDIRBOX_Free(&p);
}
int     hgFileDirBoxXy(int x,int y,char *fname,char *ret)
{
    char  file2[50];
    char  mask[50];
    char  oldpath[50];
    char  far *oldDTA;
    char  drive[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];
    int   i,flag;
    int   temp,search;

    temp=saveflag;

    strcpy(file,fname);
    strcpy(file2,fname);
    getcurdir(0,oldpath);
    oldDTA=getdta();                /*  dta store  */

    flag=fnsplit(file,drive,dir,name,ext);

    if ((flag & DRIVE)==0 && (flag & WILDCARDS)) {
        flag=fnsplit(rett,drive,dir,name,ext);
        if (flag & DIRECTORY) {
                flag=fnsplit(rett,drive,dir,name,ext);
                strcpy(file,drive);
                strcat(file,dir);
                flag=fnsplit(file2,drive,dir,name,ext);
                strcat(file,name);
                strcat(file,ext);
                sprintf(mask,"%s%s",name,ext);
        } else {
                flag=fnsplit(file,drive,dir,name,ext);
                strcpy(mask,name);
                strcat(mask,ext);
        }
    }
    else if ((flag & DRIVE) && (flag & WILDCARDS))  /* "c:*.*" */
             sprintf(mask,"%s%s",name,ext);
    else if ((flag & DRIVE) ||
             ((flag & WILDCARDS)==0 && (flag & EXTENSION)==0))  /* "c:" */
        {
        if ((char *)dir[strlen(dir)-1]=="\\") strcat(file,"*.*");
        else strcat(file,"\\*.*");
        fnsplit(file,drive,dir,name,ext);
        sprintf(mask,"%s%s",name,ext);
        }
    else sprintf(mask,"%s%s",name,ext);   /* "c:\spescial.spe" */

    directory(x,y,mask,ret);
    setdta(oldDTA);                 /* dta restore */
    add(oldpath);
    chdir(oldpath);

    if (temp) hgSetSaveOn();
    else hgSetSaveOff();

    if (dirpos==-1) return -1;
    else return 1;
}

void directory(int x,int y,char mask[20],char *rtn)
{
    int flag;
    struct ffblk ffblk; /* for findfirst */

    char drive[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];
    char temp[50];
    char temp2[50],temp3[50];
    char *str1,*str2;
    char ret[50];
    char oldpath[50];
    unsigned len;
    int  i,disk_nu,save_disk;
    char disk_table[27],disk_temp[10];


    save_disk=getdisk();

    for (disk_nu=0;disk_nu<26;disk_nu++)
    {   setdisk(disk_nu);
        if ( disk_nu==getdisk() )
           disk_table[disk_nu] = 'A' + disk_nu;
        else disk_table[disk_nu] = '0';
    }

    while (disk_table[disk_nu--]=='0');
    disk_nu++;

    flag=fnsplit(file,drive,dir,name,ext);
    if (flag & DRIVE)
        {
        i=toupper(drive[0])-'A';
        setdisk(i);
        strcpy(temp,drive);
        strcat(temp,dir);
        strcat(temp,name);
        strcat(temp,ext);
        }
    else setdisk(save_disk);

    if (flag & DIRECTORY)
        {
        dir[strlen(dir)-1]=0;
        if (dir[0]==0) chdir("\\");
        else chdir(dir);
        }

   while (1)
   {
        getcurdir(0,oldpath); /* get current directory */
        flag=fnsplit(file,drive,dir,name,ext);
        strcpy(temp,drive);
        strcat(temp,dir);
        strcat(temp,name);
        strcat(temp,ext);

        if ((flag & WILDCARDS) == 0) {  /* if not WILDCARD  */
                flag=fnsplit(file,drive,dir,name,ext);
                strcpy(temp,drive);
                strcat(temp,dir);
                strcat(temp,name);
                strcat(temp,ext);
                strcpy(rtn,temp);
                dirpos=-1;              /* for not free */
                return;
        }

    flag=hgFindFirstDir("*.*",FA_DIREC,temp);
    dirpos=0;

    while (flag)
    {
        if ((strstr(temp,".")==NULL) || (strstr(temp,"..")!=NULL))
        {
           len=strlen(temp)+4;
           {
           direc[dirpos]=(char *)malloc((size_t)len);
           strcpy(direc[dirpos],"<");
           strcat(direc[dirpos],temp);
           strcat(direc[dirpos++],">");
           }
        }

        flag=hgFindNextDir(FA_DIREC,temp);
    }

    for (i=0;i<disk_nu;i++)
    {   if (disk_table[i]!='0')
        {  direc[dirpos] = (char *)malloc((size_t)7);
           sprintf( disk_temp,"[ %c: ]",disk_table[i] );
           strcpy( direc[dirpos++],disk_temp );
        }
    }

    flag=findfirst(mask,&ffblk,FA_ARCH);

    while (!flag)
  {   strcpy(temp,ffblk.ff_name);
        len=strlen(temp)+4;
        direc[dirpos]=(char *)malloc((size_t)len);
        strcpy(direc[dirpos++],temp);
        flag=findnext(&ffblk);
        if (dirpos>=(_MAXDIR-5)) break;
    }
    direc[dirpos]=(char *)malloc((size_t)3);
    strcpy(direc[dirpos],"");

    /* This is PATH & file name to display in DIR BOX. */
    sprintf(temp,"%c:",getdisk()+'A');
    strcat(temp,"\\");
    strcat(temp,oldpath);
    if (strlen(oldpath)!=0) strcat(temp,"\\");
    strcat(temp,mask);
    strcpy(rett,temp);

    hgSelectDirBoxXy(x,y,direc,temp,ret);
    hgSetSaveOff();         /* for nesting */

    for (i=0;i<=dirpos;i++)
        free(direc[i]);

    if (!strcmp(ret,""))
    { strcpy(rtn,"\\");
      strcpy(temp2,"\\");
      if (strcmp(oldpath,""))
           strcat(temp2,oldpath);
      setdisk( save_disk );
      chdir( temp2 );
      return;
    }
    /* to here. */


    if ( ret[0]=='<' )
    {  strcpy(temp,&ret[1]);
       temp[strlen(temp)-1]=0;
       chdir(temp);
    }
    else if ( ret[0]=='[' )
    {
       i = ret[2] - 'A';
       setdisk(i);
    }
    else


    /* This part returns path & file name. */
    {   sprintf(temp,"%c:",getdisk()+'A');
    if (strlen(oldpath)!=0)
       {
        strcpy(temp3,"\\");
        strcat(temp3,oldpath);
        strcpy(oldpath,temp3);
        strcat(temp,oldpath);
       }
        strcat(temp,"\\");
        strcat(temp,ret);
        strcpy(temp2,"\\");
        if (strcmp(oldpath,""))
           strcat(temp2,oldpath);
        setdisk( save_disk );
        chdir(temp2);
        strcpy(rtn,temp);
        strcpy(rett,rtn);
        return;
        /* to here */
        }
   }
}

void add(char *s)
{
    char temp[50];

    strcpy(temp,"\\");
    strcat(temp,s);
    strcpy(s,temp);
}

void    hgSelectDirBoxXyM(char *items[],char *title,char *ret)
{
         int x=(hgGetx2r()+hgGetx1r())/2-478/2;
         int y=(hgGety2r()+hgGety1r())/2-194/2;

         hgSelectDirBoxXy(x,y,items,title,ret);
}

int     hgFileDirBoxXyM(char *fname,char *ret)
{
        int x=(hgGetx2r()+hgGetx1r())/2-478/2;
        int y=(hgGety2r()+hgGety1r())/2-194/2;
        int flag;

        flag=hgFileDirBoxXy(x,y,fname,ret);
        return flag;
}
