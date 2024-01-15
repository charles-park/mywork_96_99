/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           "Initial Header"             */
/*                Module                  */
/*                                        */
/*     made by Han Woo-jin.KIT e900519    */
/*                                        */
/*     KETEL ID : hwjketel                */
/*     TEL      : (02) 488-1558,house     */
/*                (042) 820-4231,KIT.     */
/*                                        */
/*========================================*/

#ifndef __HGINIT_H
#define __HGINIT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __LARGE__
#error Hurumi 2.0 only supports Large Model
#endif

/* video card */

#define hgHERCULES      0
#define hgEGA           1
#define hgVGA           2
#define hgSVGA          3
#define hgUNKNOWN       4

/* video mode */

#define hgVGA640x480x16         0
#define hgVGA800x600x16         1
#define hgVGA1024x768x256       2
#define hgEGA640x350x16         3
#define hgEGA640x200x16         4
#define hgHERC720x348           5
#define hgHERC640x400           6

#define hgVGA640x480x256        7
#define hgVGA800x600x256        8

/* ChipSet */

#define ET4000                  &et4000init
#define PARADISE                &paradiseinit
#define ET3000                  &et3000init
#define GENOA                   &genoainit
#define WONDER                  &wonderinit
#define AHEAD                   &aheadinit
#define POACH                   &poachinit

/* hgGetChipSet(void).. */

#define CHIP_ET4000             0
#define CHIP_PARADISE           1
#define CHIP_ET3000             2
#define CHIP_GENOA              3
#define CHIP_WONDER             4
#define CHIP_AHEAD              5
#define CHIP_POACH              6
#define CHIP_UNKNOWN           -1

/* hgSetMouseCursor */

#define DEFAULT                 &default_arrow
#define CROSS                   &cross
#define QUESTION                &question
#define NOT                     &not
#define TIMER                   &timer
#define BUBBLE                  &bubble
#define ARROW2                  &arrow2

/* video mode */

#define hgGRAPHICS              1
#define hgTEXT                  0

/* text mode */

#define hgColorText             3
#define hgMonoText              7

/* success or not */

#define hgSUCCESS               1
#define hgFAIL                  0

/* input define */

#define hgHAN3                  3
#define hgHAN2                  2
#define hgENGLISH               0

/* for BORLAND graphics.h */

#ifndef __COLORS
#define __COLORS

enum COLORS {
    BLACK,		    /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,		    /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

#endif

/* Color Define for 256 color */

extern char c256[16];

#define zeroc           c256[0]
#define bluec           c256[1]
#define greenc          c256[2]
#define cyanc           c256[3]
#define redc            c256[4]
#define magentac        c256[5]
#define brownc          c256[6]
#define lightgrayc      c256[7]
#define darkgrayc       c256[8]
#define lightbluec      c256[9]
#define lightgreenc     c256[10]
#define lightcyanc      c256[11]
#define lightredc       c256[12]
#define lightmagentac   c256[13]
#define yelloc          c256[14]
#define whitec          c256[15]

/* for inkey function */

#define NOKEY   -1
#define WAIT     0
#define NOWAIT   1
#define ZERO     BLACK                  /* special color value */

/* for chipset function */

extern void et4000init(void);
extern void paradiseinit(void);
extern void et3000init(void);
extern void genoainit(void);
extern void wonderinit(void);
extern void aheadinit(void);
extern void poachinit(void);

#ifdef __cplusplus
}
#endif

#endif
