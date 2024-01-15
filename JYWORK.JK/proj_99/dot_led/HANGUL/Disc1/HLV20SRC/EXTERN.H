/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*            Extern Header               */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#ifndef __EXTERN_H
#define __EXTERN_H

#ifdef __cplusplus
extern "C" {
#endif

/* extern function */

/* for output */

extern void vputche(int x,int y,char *buff,char color);
extern void vputchh(int x,int y,char *buff,char color);
extern void vputche_byte(int x,int y,char *buff,char color);
extern void vputchh_byte(int x,int y,char *buff,char color);
extern void vputchmn_out(int x,int y,char *buff,int xw,int yw,char c);
extern void vputchmn_out_byte(int x,int y,char *buff,int xw,int yw,char c);

extern void vputch256e(int x,int y,char *buff,char color);
extern void vputch256h(int x,int y,char *buff,char color);
extern void vputch256mn_out(int x,int y,char *buff,int xw,int yw,char c);

extern void hputche(int x,int y,char *buff,char color);
extern void hputchh(int x,int y,char *buff,char color);
extern void hputche_byte(int x,int y,char *buff,char color);
extern void hputchh_byte(int x,int y,char *buff,char color);
extern void hputchmn_out(int x,int y,char *buff,int xw,int yw,char c);
extern void hputchmn_out_byte(int x,int y,char *buff,int xw,int yw,char c);

/* special Write Mode 3. only VGA 16 color function */

extern void vputche_vga(int x,int y,char *buff,char color);
extern void vputchh_vga(int x,int y,char *buff,char color);
extern void vputche_byte_vga(int x,int y,char *buff,char color);
extern void vputchh_byte_vga(int x,int y,char *buff,char color);

/* for image */

extern void hgVGetImage(int x1,int y1,int x2,int y2,char *buff);
extern void hgVPutImage(int x1,int y1,int x2,int y2,char *buff);
extern void hgHGetImage(int x1,int y1,int x2,int y2,char *buff);
extern void hgHPutImage(int x1,int y1,int x2,int y2,char *buff);
extern void hgVGetImage4(int x1,int y1,int x2,int y2,CIMAGE **p);
extern void hgVPutImage4(int x1,int y1,int x2,int y2,CIMAGE *p);
extern void hgHGetImage4(int x1,int y1,int x2,int y2,CIMAGE **p);
extern void hgHPutImage4(int x1,int y1,int x2,int y2,CIMAGE *p);
extern void hgVGetImage256(int x1,int y1,int x2,int y2,CIMAGE **p);
extern void hgVPutImage256(int x1,int y1,int x2,int y2,CIMAGE *p);

/* for fast line */

extern void hgVhline(int x1,int x2,int y,char color);
extern void hgVvline(int x,int y1,int y2,char color);
extern void hgHhline(int x1,int x2,int y,char color);
extern void hgHvline(int x,int y1,int y2,char color);
extern void hgVhline256(int x1,int x2,int y,char color);
extern void hgVvline256(int x,int y1,int y2,char color);

/* for boxfill */

extern void hgVBoxFill(int x1,int y1,int x2,int y2,int color);
extern void hgVBoxFillFast(int x1,int y1,int x2,int y2,int color);
extern void hgVBoxFill256(int x1,int y1,int x2,int y2,int color);
extern void hgHBoxFill(int x1,int y1,int x2,int y2,int color);
extern void hgHBoxFillFast(int x1,int y1,int x2,int y2,int color);

/* for scroll */

extern void hgVScrUp(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgVScrDown(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgVScrUp256(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgVScrDown256(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgVScrUpFast(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgVScrDownFast(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgHScrUp(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgHScrDown(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgHScrUpFast(int x1,int y1,int x2,int y2,int lines,int bkcolor);
extern void hgHScrDownFast(int x1,int y1,int x2,int y2,int lines,int bkcolor);

/* for plot,line */

extern void hgHPlotXy(int x,int y,char color);
extern void hgVPlotXy(int x,int y,char color);
extern void hgVPlotXy256(int x,int y,char color);
extern char hgVGetPlotXy(int x,int y);
extern char hgVGetPlotXy256(int x,int y);
extern char hgHGetPlotXy(int x,int y);
extern void hgHLineG(int x1,int y1,int x2,int y2,char color);
extern void hgVLineG(int x1,int y1,int x2,int y2,char color);
extern void hgVLine256(int x1,int y1,int x2,int y2,char color);

/* extern virtual image function */

extern void hgVGetImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE **p);
extern void hgVPutImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE *p);
extern void hgVGetImageVIRTUAL256(int x1,int y1,int x2,int y2,VIMAGE **p);
extern void hgVPutImageVIRTUAL256(int x1,int y1,int x2,int y2,VIMAGE *p);
extern void hgHGetImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE **p);
extern void hgHPutImageVIRTUAL(int x1,int y1,int x2,int y2,VIMAGE *p);

/* for image size ( user may not call this.. ) */

extern unsigned hgVImgSize(int x1,int y1,int x2,int y2);
extern unsigned hgHImgSize(int x1,int y1,int x2,int y2);
extern unsigned hgVImgSize4(int x1,int y1,int x2,int y2);
extern unsigned hgHImgSize4(int x1,int y1,int x2,int y2);
extern unsigned hgVImgSize256(int x1,int y1,int x2,int y2);

/* clear screen */

extern void hgVClearScreen(void);
extern void hgClearScreen256(void);
extern void hgHClearScreen(void);

/* mouse init */

extern void hgSetHMouseInit(void);
extern void hgSetVMouseInit(void);
extern void hgSetSMouseInit(void);

/* attribute extern function prototype */

extern void attr_null(char *dest,int bytes);
extern void Reverse_Img(char *dest,int bytes);
extern void Dim_Img(char *dest,int bytes);
extern void Shadow_Img(char *dest,int bytes);
extern void ThreeD_Img(char *dest,int bytes);
extern void Outline_Img(char *dest,int bytes);
extern void Under_Img(char *dest,int bytes);
extern void Bold_Img(char *dest,int bytes);

/* font function external define */

/* english */

extern void  big_eng(void);
extern void  italic_eng(void);
extern void  small_eng(void);
extern void  pilgi_eng(void);
extern void  inbold_eng(void);
extern void  bold_eng(void);
extern void  eart_eng(void);
extern void  ad24_eng(void);
extern void  roman_eng(void);

/* hangul */

extern void  hanchg2_han(void);
extern void  hanchg4_han(void);
extern void  haniyg_han(void);
extern void  hansoft_han(void);
extern void  hwp_han(void);
extern void  han_m_han(void);
extern void  pilhan_han(void);
extern void  ppalae_han(void);
extern void  ad24_han(void);
extern void  sam_han(void);
extern void  hould_han(void);
extern void  hmapple_han(void);
extern void  hanson_han(void);
extern void  hanpil_han(void);
extern void  hanboot_han(void);

/* special */

extern void  special_fnt(void);

/* mouse cursor external define */

extern void cross(void);
extern void default_arrow(void);
extern void question(void);
extern void not(void);
extern void timer(void);
extern void bubble(void);
extern void arrow2(void);

/* hangul code type */

extern void sangyong_code(void);
extern void samsung_code(void);
extern void goldstar_code(void);

#ifdef __cplusplus
}
#endif

#endif
