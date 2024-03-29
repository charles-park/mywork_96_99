/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            "Demo Program"              */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#include "hginit.h"
#include "hghlib.h"
#include "hgquick.h"
#include "hwindow.h"
#include "key.h"
#include "hgpcx.h"

void make();
void intro();
void output();
void graphic();
void end();

void (*func[5])()={ make,intro,output,graphic,end };

void setscreen()
{
	int i;

	hgAutoChipSet();

	hgSetDisplay(hgVGA800x600x256);
	hgSetMode(hgGRAPHICS);
	hgSetHanFont(HMAPPLE_HAN);
	hgSetEngFont(SMALL_ENG);
	hgQuickSOutInit();
	hgQuickSGraphInit();

	hgMarkInit();

	hgEnablePCXFileDisplay(PCX_VGA256_FUNCTION);

/*	hgPCXpaletteSet("hrm.pcc");
	hgAdjustPalette256();
	hgAdjustStyle(); */

	hgSetTitleFore(whitec);
	hgSetTitleBack(darkgrayc);
	hgSetTFcolor(zeroc);
	hgPCXpaletteOff();
}
void endscreen()
{
	hgMarkEnd();
	hgSetMode(hgTEXT);
}
void initchoose()
{
	int ch=0;
	char *text[]={ "  1. �A�b�a  ","  2. �b��","  3. �b�A�� ",
		       "  4. �a��Ϣ�A�� ","  5. �{ ",""};

	hgSetBack(LIGHTGRAY);
	hgClearScreen();

	hgPrtWindowXy(0,0,hgGetx2r(),hgGety2r(),"��a�� �a���a�១ 2.0 �A��");
	do {
		while (hgLeftMouse());
		ch=hgSelectXyM(text,ch);
		if (ch!=-1) {
			(*func[ch])();
			if (ch==4) break;
		}
		else ch=4;
	} while (1);
}

void main()
{
	setscreen();
	initchoose();
	endscreen();
}

void make()
{
	char *text[]= {
  " ",
  "                    �e�w�a�A�a. ��a�� �a���a  ",
  "                    �១�� �A�b�a �e �������a  ",
  "                    �� �s���a. ���� 2.0  ���  ",
  "                    �e 1.5 ��巁 �w�� ѡ�Ŭ�  ",
  "                    �i �a���e ��巳���a.  ��  ",
  "                    �a���a���e 2.0 ��  �����a  ",
  "                    �e 256 ���  �a���a�១��  ",
  "                    �b���i �e���a�A  ���a����  ",
  "                    �a. ���e�ⷥ �b���i�e  16  ",
  "                    ��� �A���a���a���i  �q��  ",
  " �a�A�a.���A�񷡐a  ����Ё ������,���e ��a�i  ",
  " �i�e�a���e �a���� �e�bЁ ���A�a.",
  " ",
  "                     KETEL ID : hwjketel",
  "                     ������ ��ѡ�A �A���e"," ","" };
  int x1,y1,x2,y2;
  int ch;

  hgPrtWTextXyM("�A�b�a",text);
  hgRetMSize(text,&x1,&y1,&x2,&y2);
  hgPrtPCXBorderXy(x1+37,y1+55,"hrm.pcc",wcBOXNORMAL);

  do {
	  ch=windelay(x1,y1,x2,y2);
	  if (ch==MOUSE_LEFT || ch==MOUSE_RIGHT) ch=NOKEY;
  } while (ch==NOKEY);

  hgClearWindowXy(hgGetx1r(),hgGety1r(),hgGetx2r(),hgGety2r());
}

void intro()
{
	char *text[]={
  " ��a�� �e�i �a���a�១ 2.0 �e 1.5 �A ��Ё�� �g�e ",
  " ������ ��i�i �a���� ���s���a.�a�w �a�A �a��  �� ",
  " ���a�e 256 ��᷁ ������ SuperVGA Card 7 �� ���� ",
  " , Virtual Memory �s���w�� ���V�s���a.",
  " �y�a�� ���a�e �b���i �i�ᥥ�a�e ",
  " ",
  " 1. ET4000,ET3000,ATI VGA WONDER �i ͡�q�e SVGA   ",
  "    7 ���i �����e�a.",
  " 2. Virtual Memory System �i �s��,���e  �������i  ",
  "    �ş��i�� EMS,HardDisk,Conventional Memory �i  ",
  "    ���� �i�� ���A �A���a.",
  " 3. �e�a�i �����a�a,�a��Ϣ���a,�b�����a�i �����e  ",
  "    �a.���i�e ���� ���b�b�a�� �����e�a.",
  "    �e�e�e �e�a�a��,ͥ�a �ş����i ͡�q�a�� ���a.  ",
  " 4. Help,Directory Box �i �� ���A�� �a�a�v�a.   ",
  " ",
  " ���c�A�� �g�e �b���� ���s���a. ���A��� �A��  �a ",
  " ���a���A�� ���� �·��a�A�a.","" };

  int x1,y1,x2,y2;
  int ch;

  hgRetMSize(text,&x1,&y1,&x2,&y2);
  hgPrtWTextXyM("�b��",text);

  do {
          ch=windelay(x1,y1,x2,y2);
          if (ch==MOUSE_LEFT || ch==MOUSE_RIGHT) ch=NOKEY;
  } while (ch==NOKEY);

  hgClearWindowXy(hgGetx1r(),hgGety1r(),hgGetx2r(),hgGety2r());
}
void output()
{
        int x,y;
        int   ch;
        int   i;
        int   color;

        hgPrtWindowXy(130,82,510,418,"�b�A�a�a");

        hgHideMouse();

        hgSetTFcolor(whitec);
	for (x=143;x<510-28;x+=16)
		for (y=82+33;y<418-30;y+=16) {
			hgSetTBcolor((x/8+y/16)%256);
			hgOverTextXy(x,y,"�e");
	}
        hgSetTFcolor(zeroc);
        hgShowMouse();
        do {
                  ch=windelay(130,82,510,418);
                  if (ch==MOUSE_LEFT || ch==MOUSE_RIGHT) ch=NOKEY;
        } while (ch==NOKEY);
        hgClearWindowXy(130,82,510,418);
        hgHideMouse();

        hgSetTFcolor(zeroc);
        for (x=143;x<510-28;x+=16)
                for (y=82+33;y<418-30;y+=16) {
                        hgSetTFcolor((x/8+y/16)%256);
                        hgOutTextXy(x,y,"�e");
        }
        hgSetTFcolor(zeroc);
        hgShowMouse();
        do {
                  ch=windelay(130,82,510,418);
                  if (ch==MOUSE_LEFT || ch==MOUSE_RIGHT) ch=NOKEY;
        } while (ch==NOKEY);
        hgClearWindowXy(hgGetx1r(),hgGety1r(),hgGetx2r(),hgGety2r());
}
void graphic()
{
	int i,j;
	int ch;

        hgClearWindowXy(hgGetx1r(),hgGety1r(),hgGetx2r(),hgGety2r());

        hgHideMouse();
        hgPrtWindowXy(100,100,600,500,"�a��Ϣ �A�a�a");

        for (i=113;i<587;i+=4) {
             hgLine(350,300,i,133,(i/4)%256);
             hgLine(350,300,i,487,(i/4)%256);
        }
        for (i=133;i<487;i+=4) {
             hgLine(350,300,113,i,(i/4)%256);
             hgLine(350,300,587,i,(i/4)%256);
        }
        hgShowMouse();
        do {
                  ch=windelay(100,100,600,500);
                  if (ch==MOUSE_LEFT || ch==MOUSE_RIGHT) ch=NOKEY;
        } while (ch==NOKEY);
	hgHideMouse();
	hgColorWindowXy(100,100,600,500,whitec);
	for (i=30;i<180;i+=2)
		hgEllipse(350,300,i,(180-i),((i-29)/4)%256);
	hgShowMouse();
	do {
		  ch=windelay(100,100,600,500);
		  if (ch==MOUSE_LEFT || ch==MOUSE_RIGHT) ch=NOKEY;
	} while (ch==NOKEY);
	hgClearWindowXy(hgGetx1r(),hgGety1r(),hgGetx2r(),hgGety2r());
}
void end()
{
        char *text[]={
 " �i��A ���v�a�a.. ��a�� 2.0 �� ������ ",
 " �A�v�a�e ���V�s���a. �|�a���� ��  ���b ",
 " �a�a ��a�� 3.0 �i �i�a�a�V�s���a.  �� ",
 " �a�� 3.0 �A�e �a�w�a�a�e  �B��ͥ�a�e�i ",
 " �� ���w�i ���ᥡ�� ���A�a.  �a���e 3.0 ",
 " �i �A�b�i ���B�e ����a. �a��. ","" };

        int ch;
        int x1,y1,x2,y2;

        hgClearWindowXy(hgGetx1r(),hgGety1r(),hgGetx2r(),hgGety2r());
        hgPrtWTextXyM("�e�wӡ..",text);
        hgRetMSize(text,&x1,&y1,&x2,&y2);

        do {
                  ch=windelay(x1,y1,x2,y2);
                  if (ch==MOUSE_LEFT || ch==MOUSE_RIGHT) ch=NOKEY;
        } while (ch==NOKEY);
}
