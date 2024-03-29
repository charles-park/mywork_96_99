/* ================================================== */
/* THIS IS KEY-SCAN CODE TABLE                        */
/* obtained by inkey() function.                      */
/* shift flag is used by specific purpose.            */
/* by bioskey(2); check shift key.                    */
/*                                                    */
/* Made by Han Woo-jin , KIT e900519                  */
/* ================================================== */

#ifndef __KEY_H
#define __KEY_H

/* fundamental */

#define 	RETURN		13
#define		SPACE		32
#define		HOME		327
#define		END		335
#define		PgUp		329
#define		PgDn		337
#define		DEL		339
#define		BS		8
#define 	INS		338
#define		TAB		9
#define		LEFT		331
#define		RIGHT		333
#define		UP		328
#define		DOWN		336
#define 	ESC		27

/* function key */

#define		F1		315
#define		F2		316
#define		F3		317
#define		F4		318
#define		F5		319
#define		F6		320
#define 	F7		321
#define 	F8		322
#define		F9		323
#define		F10		324

/* alt key combination */

#define		ALT_1		376
#define		ALT_2		377
#define		ALT_3		378
#define		ALT_4		379
#define		ALT_5		380
#define		ALT_6		381
#define		ALT_7		382
#define		ALT_8		383
#define		ALT_9		384
#define		ALT_0		385
#define 	ALT_Q		272
#define		ALT_W		273
#define		ALT_E		274
#define		ALT_R		275
#define		ALT_T		276
#define 	ALT_Y		277
#define		ALT_U		278
#define		ALT_I		279
#define 	ALT_O		280
#define		ALT_P		281
#define		ALT_A		286
#define		ALT_S		287
#define		ALT_D		288
#define		ALT_F		289
#define		ALT_G		290
#define		ALT_H		291
#define		ALT_J		292
#define		ALT_K		293
#define		ALT_L		294
#define		ALT_Z		300
#define		ALT_X		301
#define		ALT_C		302
#define		ALT_V		303
#define		ALT_B		304
#define		ALT_N		305
#define		ALT_M		306

/* ctrl key combination */

#define 	CTRL_A		1
#define		CTRL_B		2
#define		CTRL_C		3
#define		CTRL_D		4
#define		CTRL_E		5
#define		CTRL_F		6
#define		CTRL_G		7
#define		CTRL_H		8
#define		CTRL_I		9
#define		CTRL_J		10
#define		CTRL_K		11
#define 	CTRL_L		12
#define		CTRL_M		13
#define		CTRL_N		14
#define		CTRL_O		15
#define		CTRL_P		16
#define		CTRL_Q		17
#define		CTRL_R		18
#define		CTRL_S		19
#define		CTRL_T		20
#define		CTRL_U		21
#define		CTRL_V		22
#define		CTRL_W		23
#define		CTRL_X		24
#define		CTRL_Y		25
#define		CTRL_Z		26
#define		CTRL_LEFT	371
#define		CTRL_RIGHT	372
#define		CTRL_HOME	375
#define		CTRL_END	373
#define		CTRL_PgUp	388
#define		CTRL_PgDn	374

/* shift flag */

#define 	RIGHT_SHIFT	1
#define		LEFT_SHIFT	2
#define		CTRL		4
#define		ALT		8
#define		SCROLL_LOCK	16
#define		NUM_LOCK	32
#define 	CAPS_LOCK	64
#define         INSERT		128

/* mouse status */

#define         MOUSE_LEFT      400
#define         MOUSE_RIGHT     401
#define         MOUSE_MIDDLE    402

/* mark status */

#define         LEFTMARK        403
#define         RIGHTMARK       404

#endif
