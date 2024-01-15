/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*          "ICON Header File"            */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#ifndef __HGICON_H
#define __HGICON_H

#ifdef __cplusplus
extern "C" {
#endif

#define WINCLOSE_ICON  &winclose_icon
#define WINEND_ICON    &winend_icon
#define WINCLOSER_ICON &winclr_icon
#define WINENDR_ICON   &winenr_icon

/* VGA Icon Function External define */

extern void winclose_icon(HICON *p);
extern void winend_icon(HICON *p);
extern void winclr_icon(HICON *p);
extern void winenr_icon(HICON *p);

#ifdef __cplusplus
}
#endif

#endif
