/*
	Copyright (C) 2004-2005 Li Yudong
*/
/*
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _MOUSE_TS_H_
#define _MOUSE_TS_H_

#ifdef __cplusplus                     
extern "C" {
#endif



#define STATUS_PENDOWN	1
#define STATUS_PENUP	0


#define MOUSE_NORMAL		0x0000	/* mouse in normal mode*/
#define MOUSE_RAW			0x0001	/* mouse in raw mode*/
#define MOUSE_TRANSFORM		0x0002	/* perform transform*/

#define MWBUTTON_L			0x01

#define TS_DEVICE			"UCB1X00"
#define TS_DEVICE_FILE			"/dev/h3600_ts"

#define EPRINTF				printf
#define	MIN_MWCOORD			-32768	/* minimum coordinate value */
#define	MAX_MWCOORD			32767	/* maximum coordinate value */

/* Input filter routines - global mouse filtering is cool */
#define JITTER_SHIFT_BITS	2
#define JITTER_DEPTH		(1 << (JITTER_SHIFT_BITS))


	
typedef struct tagTSMOUSEINFO{
	int x;
	int y;
	int nLastStatus;
} TSMOUSEINFO;


typedef struct tagTS_EVENT {
	unsigned short pressure;
	unsigned short x;
	unsigned short y;
	unsigned short pad;
	//struct timeval stamp;
} TS_EVENT;



/** touchscreen device transform coefficients for GdSetTransform*/
typedef struct {
	int	a, b, c;	/* xpos = (a*jitx + b*jity + c)/denom */
	int	d, e, f;	/* ypos = (d*jitx + e*jity + f)/denom */
	int	s;		/* denom*/
} MWTRANSFORM;





#ifdef __cplusplus
}
#endif 

#endif 
