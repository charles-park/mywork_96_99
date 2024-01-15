/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            PCX File Header             */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#ifndef __HGPCX_H
#define __HGPCX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct pcxhdr {
	char Manufacturer;
	char Version;
	char Encoding;
	char Bitsperpixel;
        int  x1,y1,x2,y2;
	int  HRes;
	int  VRes;
	unsigned char Colormap[16][3];
	char Reserved;
	char NPlanes;
        unsigned BytesperLine;
	int  PaletteInfo;
	char Filler[58];
} PCXHDR;

/* pcx picture type */

#define PCX_HERCULES    0
#define PCX_VGA16       1
#define PCX_VGA256      2
#define NO_PCX         -1

/* function define */

#define PCX_VGA16_FUNCTION           &hgPCXFileDisplayV
#define PCX_VGA256_FUNCTION          &hgPCXFileDisplay256
#define PCX_HERCULES_FUNCTION        &hgPCXFileDisplayH
#define PCX_MONO_FUNCTION            &hgPCXFileDisplayM

/* function prototype */

void hgGetPCXHdr(PCXHDR *p,char *fname);
int  hgGetPCXMode(char *fname);
void hgEnablePCXFileDisplay(void (*p)(int x,int y,char *fname));
void hgPCXFileDisplay(int x,int y,char *fname);

void hgPCXFileDisplayH(int x,int y,char *fname);
void hgPCXFileDisplayV(int x,int y,char *fname);
void hgPCXFileDisplayM(int x,int y,char *fname);
void hgSetPCXMonoColor(char color);
void hgPCXFileDisplay256(int x,int y,char *fname);

/* quick function */

void hgPrtPCXWindowXy(int x,int y,char *titl,char *fname);
void hgPrtPCXBorderXy(int x,int y,char *fname,int style);
void hgPrtPCXWindowXyM(char *titl,char *fname);
void hgPrtPCXBorderXyM(char *fname,int style);
void hgPCXpaletteOn(void);
void hgPCXpaletteOff(void);
void hgPCXpaletteSet(char *fname);

#ifdef __cplusplus
}
#endif

#endif
