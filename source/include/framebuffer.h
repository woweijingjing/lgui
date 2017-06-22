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

#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#ifdef __cplusplus                     
extern "C" {
#endif

BOOL 
InitFrameBuffer();

void 
UnInitFrameBuffer();

void inline 
lGUI_SetPixel_Direct(
	int x,
	int y, 
	COLORREF color
);

void inline 
lGUI_SetPixel(
	int x,
	int y, 
	COLORREF color
);

COLORREF inline 
lGUI_GetPixel(
	int x,
	int y
);

COLORREF inline 
lGUI_GetPixel_Direct(
	int x,
	int y
);


void 
PaintRect2Screen(
	PRECT lpOutRc,
	PRECT lpOrgRc,
	void* pData
);

void 
PaintRect2ScreenWithTrColor(
	PRECT lpOutRc,
	PRECT lpOrgRc,
	void* pData, 
	COLORREF crTrColor
);

void 
PaintLine2Screen(
	int xPos, 
	int yPos, 
	int iLen, 
	void* pData
);

void inline 
PaintLine2ScreenRAW(
	int xPos,
	int yPos, 
	int iLen, 
	void* pData
);

void 
GetLineFromScreen(
	int xPos, 
	int yPos, 
	int iLen, 
	void* pData
);

void 
GetLineFromScreenRAW(
	int xPos, 
	int yPos, 
	int iLen, 
	void* pData
);

void 
EraseRect2Screen(
	PRECT lpRc,
	COLORREF color
);

inline void 
GetBoxFromScreen(
	int x, 
	int y, 
	int nWidth, 
	int nHeight, 
	PCOLORREF pData
);

inline void 
GetBoxFromScreenRaw(
	int x,
	int y, 
	int nWidth, 
	int nHeight, 
	PCOLORREF pData
);

inline void 
PutBox2Screen(
	int x, 
	int y, 
	int nWidth, 
	int nHeight, 
	PCOLORREF pData
);

inline void 
PutBox2ScreenWithTrColor(
	int x, 
	int y, 
	int nWidth, 
	int nHeight, 
	PCOLORREF pData, 
	COLORREF crColor
);
#ifdef __cplusplus
}
#endif

#endif


