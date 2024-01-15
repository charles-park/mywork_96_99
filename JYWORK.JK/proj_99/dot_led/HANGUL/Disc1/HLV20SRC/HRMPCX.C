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
	char *text[]={ "  1. ¹A¸b¸a  ","  2. Ëb»·","  3. Â‰b•A¡¡ ",
		       "  4. ‹aœÏ¢•A¡¡ ","  5. { ",""};

	hgSetBack(LIGHTGRAY);
	hgClearScreen();

	hgPrtWindowXy(0,0,hgGetx2r(),hgGety2r(),"ÐáŸa£¡ œa·¡§aœáŸ¡ 2.0 •A¡¡");
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
  "                    ´e‘wÐa­A¶a. ÐáŸa£¡ œa·¡§a  ",
  "                    œáŸ¡· ¹A¸b¸a Ðe ¶»¥·¡œa  ",
  "                    ‰¡ Ðs“¡”a. ·¡¤å 2.0  ¤á¸å  ",
  "                    ·e 1.5 ¤á¸å· ¬w¶á Ñ¡ÑÅ¬÷  ",
  "                    ·i ˆa»¡“e ¤á¸å·³“¡”a.  ·¡  ",
  "                    Ïa¡‹aœ‘·e 2.0 ·¡  »¡¶¥Ða  ",
  "                    “e 256 Äéœá  œa·¡§aœáŸ¡·  ",
  "                    Ëb»··i ˆeœ¢Ða‰A  ¥¡µaº““¡  ",
  "                    ”a. ·©¤e¸â·¥ Ëb»·—i·e  16  ",
  "                    Äéœá •A¡¡Ïa¡‹aœ‘·i  Àq‰¡  ",
  " Ða­A¶a.¢…¹A¸ñ·¡a  ¹¡´åÐ º¯©¸ñ,™¡“e ¤á‹aŸi  ",
  " ¤i‰eÐa¯¡¡e ´aœ¡ µeœbÐ º­A¶a.",
  " ",
  "                     KETEL ID : hwjketel",
  "                     –ž¢‰ •·Ñ¡ÒA ‰A¯¡Ìe"," ","" };
  int x1,y1,x2,y2;
  int ch;

  hgPrtWTextXyM("¹A¸b¸a",text);
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
  " ÐáŸa£¡ Ðe‹i œa·¡§aœáŸ¡ 2.0 ·e 1.5 µA §¡Ð¬á  g·e ",
  " ¬¡¶… ¸ñ—i·i ˆa»¡‰¡ ·¶·s“¡”a.ˆa¸w Ça‰A ¤aŽå  ¸ñ ",
  " ·¡œa¡e 256 Äéœá· »¡¶¥‰Á SuperVGA Card 7 ˆ »¡¶¥ ",
  " , Virtual Memory Ès¸—w·¡ ·¶‰V·s“¡”a.",
  " ¡yˆa»¡ º—¶aÐe Ëb»··i —i´á¥¥”a¡e ",
  " ",
  " 1. ET4000,ET3000,ATI VGA WONDER Ÿi Í¡ÐqÐe SVGA   ",
  "    7 ˆŸi »¡¶¥Ðe”a.",
  " 2. Virtual Memory System ·i Ès¸,ÑÁ¡e  ·¡£¡»¡Ÿi  ",
  "    ‰ÅŸ¡Ði˜ EMS,HardDisk,Conventional Memory Ÿi  ",
  "    ¡¡– ³i® ·¶‰A –A´ö”a.",
  " 3. Ðe¸aŸi »¡¶¥Ða¡a,‹aœÏ¢¢…¸a,Ëb®¢…¸aŸi »¡¶¥Ðe  ",
  "    ”a.·¡—i·e ¡¡– ·³b¤b¯a•¡ »¡¶¥Ðe”a.",
  "    ˆe”eÐe Ðe¸a¬a¸å,Í¥Ëa ‰ÅŸ¡‹¡Ÿi Í¡ÐqÐa‰¡ ·¶”a.  ",
  " 4. Help,Directory Box Ÿi ¬ ˆ‚ÁA¡ ÂˆaÐaµv”a.   ",
  " ",
  " ·¡¤cµA•¡  g·e Ëb»··¡ ·¶·s“¡”a.  “A´é‰Á •A¡¡  Ïa ",
  " ¡‹aœ‘µA¬á »¢¸ó ÑÂ·¥Ða­A¶a.","" };

  int x1,y1,x2,y2;
  int ch;

  hgRetMSize(text,&x1,&y1,&x2,&y2);
  hgPrtWTextXyM("Ëb»·",text);

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

        hgPrtWindowXy(130,82,510,418,"Â‰bÉA¯aËa");

        hgHideMouse();

        hgSetTFcolor(whitec);
	for (x=143;x<510-28;x+=16)
		for (y=82+33;y<418-30;y+=16) {
			hgSetTBcolor((x/8+y/16)%256);
			hgOverTextXy(x,y,"Ðe");
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
                        hgOutTextXy(x,y,"Ðe");
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
        hgPrtWindowXy(100,100,600,500,"‹aœÏ¢ ÉA¯aËa");

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
 " »iˆó‰A ¥¡­va¶a.. ÐáŸa£¡ 2.0 ·¡ •¡¶‘·¡ ",
 " –A­v·a¡e ¹½‰V·s“¡”a. ´|·a¡•¡ ”á  ‘¡b ",
 " Ðaµa ÐáŸa£¡ 3.0 ·i ¤iÎaÐa‰V·s“¡”a.  Ðá ",
 " Ÿa£¡ 3.0 µA“e ˆa“wÐa”a¡e  ¥BÈáÍ¥ËaÐe‹i ",
 " · ‹¡“w·i ý´á¥¡‰¡ ¯¼‘A¶a.  ´a»¢·e 3.0 ",
 " ·i ¹A¸bÐi ‰ÒB·e ´ô´á¶a. ‹aœñ. ","" };

        int ch;
        int x1,y1,x2,y2;

        hgClearWindowXy(hgGetx1r(),hgGety1r(),hgGetx2r(),hgGety2r());
        hgPrtWTextXyM("´e‘wÓ¡..",text);
        hgRetMSize(text,&x1,&y1,&x2,&y2);

        do {
                  ch=windelay(x1,y1,x2,y2);
                  if (ch==MOUSE_LEFT || ch==MOUSE_RIGHT) ch=NOKEY;
        } while (ch==NOKEY);
}
