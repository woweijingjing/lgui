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

#include "../include/common.h"
#include "../include/hdc.h"
#include "../include/rect.h"
#include "../include/message.h"
#include "../include/blockheap.h"
#include "../include/invalidregion.h"
#include "../include/clipregion.h"
#include "../include/caret.h"
#include "../include/winnc.h"
#include "../include/winbase.h"
#include "../include/window.h"
#include "../include/bmp.h"
#include "../include/framebuffer.h"
#include "../include/gdibmp.h"
    
extern int _lGUI_iBytesDataType;

BOOL GUIAPI 
ShowBitmapWithTrColor(
	HDC hDC, 
	int xPos, 
	int yPos, 
	char* pFile, 
	COLORREF crTrColor
)
{
	ClientToWindow(hDC->hWnd,&xPos,&yPos);
	return winShowBitmapWithTrColor(hDC,xPos,yPos,pFile,crTrColor);
}


BOOL 
winShowBitmapWithTrColor(
	HDC hDC, 
	int xPos, 
	int yPos, 
	char* pFile, 
	COLORREF crTrColor
)
{
	int iType;
	iType=hDC->iType;
 	if(iType==DC_TYPE_WIN)
		if(ShowBitmapWithTrColorWin(hDC, xPos, yPos, pFile, crTrColor))
			return true;
		else
			return false;
	else
		if(ShowBitmapWithTrColorMem(hDC, xPos, yPos, pFile, crTrColor))
			return true;
		else
			return false;
}


BOOL 
ShowBitmapWithTrColorWin(
	HDC hDC, 
	int xPos, 
	int yPos, 
	char* pFile, 
	COLORREF crTrColor
)
{
	PClipRect pClipRect;
	FILE* fp;
	BMPINFOHEADER infoHeader;

	void* pData;
	RECT rcSource,rcDest;
	int iWinWidth, iWinHeight;
	int iBmpWidth, iBmpHeight;
	int iWidth,iHeight;

	GetWindowRect(hDC->hWnd,&rcSource);
	WindowToScreen(hDC->hWnd,&xPos,&yPos);

	if(xPos>rcSource.right || xPos<rcSource.left)
		return false;
	if(yPos>rcSource.bottom || yPos<rcSource.top)
		return false;

	iWinWidth	=rcSource.right-xPos+1;
	iWinHeight	=rcSource.bottom-yPos+1;

	fp=fopen(pFile,"rb");
	if(!fp)
		return false;
	fseek(fp,0x12,0);
	fread(&(infoHeader.biWidth),sizeof(infoHeader.biWidth),1,fp);
	fread(&(infoHeader.biHeight),sizeof(infoHeader.biHeight),1,fp);

	fseek(fp,0x1c,0);
	fread(&(infoHeader.biBitCount),sizeof(infoHeader.biBitCount),1,fp);

	iBmpWidth	=infoHeader.biWidth;
	iBmpHeight	=infoHeader.biHeight;

	iWidth=min(iBmpWidth,iWinWidth);
	iHeight=min(iBmpHeight,iWinHeight);

	pData=malloc(iWidth*iHeight*_lGUI_iBytesDataType);
	if(!pData){
		fclose(fp);
		return false;
	}
	SetRect(&rcSource,xPos,yPos,xPos+iWidth-1,yPos+iHeight-1);

	if(!ReadBmpToBuf(fp, iWidth, iHeight, pData)){
		free(pData);
		fclose(fp);
		return false;
	}

	pClipRect=((PWindowsTree)(hDC->hWnd))->pClipRgn->pHead;
	while(pClipRect){
		if(IntersectRect(&rcDest,&rcSource,&(pClipRect->rect)))
			PaintRect2ScreenWithTrColor(&rcDest,&rcSource,pData,crTrColor);
		pClipRect=pClipRect->pNext;
	}

	free(pData);
	fclose(fp);
	return true;
}


BOOL 
ShowBitmapWithTrColorMem(
	HDC hDC, 
	int xPos, 
	int yPos, 
	char* pFile, 
	COLORREF crTrColor
)
{
	FILE* fp;
	BMPINFOHEADER infoHeader;
	int iBmpWidth, iBmpHeight;
	int iCliWidth,iCliHeight;
	int iWinWidth,iWinHeight;
	int iWidth,iHeight;

	int x,y;
	void* pCurPointer;
	void* pOutPointer;

	void *pData;
	RECT rc;


	if(!(hDC->hWnd))
		return false;
	GetWindowRect(hDC->hWnd,&rc);
	iWinWidth = rc.right - rc.left + 1;
	iWinHeight = rc.bottom - rc.top + 1;


	if(!(hDC->hWnd))
		return false;
	GetWindowRect(hDC->hWnd,&rc);
	iWinWidth = rc.right - rc.left + 1;
	iWinHeight = rc.bottom - rc.top + 1;

	GetClientRect(hDC->hWnd,&rc);
	iCliWidth = rc.right - rc.left + 1;
	iCliHeight = rc.bottom - rc.top + 1;


	if(xPos>=iWinWidth || xPos<0){
		printerror("show bitmap out of range.");
		return false;
	}
	if(yPos>=iWinHeight || yPos<0){
		printerror("show bitmap out of range.");
		return false;
	}
	fp=fopen(pFile,"rb");
	if(!fp){
		printerror("open file error!");
		return false;
	}

	fseek(fp,0x12,0);
	fread(&(infoHeader.biWidth),sizeof(infoHeader.biWidth),1,fp);
	fread(&(infoHeader.biHeight),sizeof(infoHeader.biHeight),1,fp);

	fseek(fp,0x1c,0);
	fread(&(infoHeader.biBitCount),sizeof(infoHeader.biBitCount),1,fp);

	iBmpWidth=infoHeader.biWidth;
	iBmpHeight=infoHeader.biHeight;

	iWidth=min(iBmpWidth,iCliWidth-xPos);
	iHeight=min(iBmpHeight,iCliHeight-yPos);

	pData=malloc(iWidth*iHeight*_lGUI_iBytesDataType);
	if(!pData){
		printerror("alloc memory error!");
		return false;
	}

	if(!ReadBmpToBuf(fp, iWidth, iHeight, pData)){
		free(pData);
		fclose(fp);
		return false;
	}
	for(y=0;y<iHeight;y++){
		for(x=0;x<iWidth;x++){
			pCurPointer=pData + (y*iWidth+x)*_lGUI_iBytesDataType;
			if(*((PCOLORREF)(pCurPointer))!=crTrColor){
				pOutPointer=hDC->pData + (y + yPos)*iWinWidth*_lGUI_iBytesDataType + 
					(x + xPos)*_lGUI_iBytesDataType;
				memcpy(pOutPointer,pCurPointer,_lGUI_iBytesDataType);
			}
		}
	}

	free(pData);
	fclose(fp);
	return true;
}



BOOL GUIAPI 
ShowBitmap(
	HDC hDC, 
	int xPos, 
	int yPos, 
	char* pFile
)
{
	ClientToWindow(hDC->hWnd,&xPos,&yPos);
	return winShowBitmap(hDC,xPos,yPos,pFile);
}


BOOL 
winShowBitmap(
	HDC hDC, 
	int xPos, 
	int yPos, 
	char* pFile
)
{
	int iType;
	iType=hDC->iType;
	if(iType==DC_TYPE_WIN){
		if(ShowBitmapWin(hDC, xPos, yPos, pFile))
			return true;
		else
			return false;
	}
	else{
		if(ShowBitmapMem(hDC, xPos, yPos, pFile))
			return true;
		else
			return false;
	}

}


BOOL 
ShowBitmapWin(
	HDC hDC, 
	int xPos, 
	int yPos, 
	char* pFile
)
{
	PClipRect pClipRect;
	FILE* fp;
	BMPINFOHEADER infoHeader;

	void* pData;
	RECT rcSource,rcDest;
	int iWinWidth, iWinHeight;
	int iBmpWidth, iBmpHeight;
	int iWidth,iHeight;

	GetWindowRect(hDC->hWnd,&rcSource);
	scrGetClientRect(hDC->hWnd,&rcSource);
	WindowToScreen(hDC->hWnd,&xPos,&yPos);

	if(xPos>rcSource.right || xPos<rcSource.left)
		return false;
	if(yPos>rcSource.bottom || yPos<rcSource.top)
		return false;

	iWinWidth	=rcSource.right-xPos+1;
	iWinHeight	=rcSource.bottom-yPos+1;

	fp=fopen(pFile,"rb");
	if(!fp)
		return false;
	fseek(fp,0x12,0);
	fread(&(infoHeader.biWidth),sizeof(infoHeader.biWidth),1,fp);
	fread(&(infoHeader.biHeight),sizeof(infoHeader.biHeight),1,fp);

	fseek(fp,0x1c,0);
	fread(&(infoHeader.biBitCount),sizeof(infoHeader.biBitCount),1,fp);

	iBmpWidth	=infoHeader.biWidth;
	iBmpHeight	=infoHeader.biHeight;

	iWidth=min(iBmpWidth,iWinWidth);
	iHeight=min(iBmpHeight,iWinHeight);

	pData=malloc(iWidth*iHeight*_lGUI_iBytesDataType);
	if(!pData){
		fclose(fp);
		return false;
	}
	SetRect(&rcSource,xPos,yPos,xPos+iWidth-1,yPos+iHeight-1);

	if(!ReadBmpToBuf(fp, iWidth, iHeight, pData)){
		free(pData);
		fclose(fp);
		return false;
	}
	pClipRect=((PWindowsTree)(hDC->hWnd))->pClipRgn->pHead;
	while(pClipRect){
		if(IntersectRect(&rcDest,&rcSource,&(pClipRect->rect)))
			PaintRect2Screen(&rcDest,&rcSource,pData);
		pClipRect=pClipRect->pNext;
	}

	free(pData);
	fclose(fp);
	return true;	
}


BOOL 
ShowBitmapMem(
	HDC hDC, 
	int xPos, 
	int yPos, 
	char* pFile
)
{
	FILE* fp;
	BMPINFOHEADER infoHeader;
	int iBmpWidth, iBmpHeight;
	int iWinWidth,iWinHeight;
	int iCliWidth,iCliHeight;
	int iWidth,iHeight;
	int iLeftMarge,iTopMarge,iBottom;

	int y;
	void* pCurPointer;
	void* pOutPointer;

	void *pData;
	RECT rc;


	if(!(hDC->hWnd))
		return false;
	GetWindowRect(hDC->hWnd,&rc);
	iWinWidth = rc.right - rc.left + 1;
	iWinHeight = rc.bottom - rc.top + 1;

	GetClientRect(hDC->hWnd,&rc);
	iCliWidth = rc.right - rc.left + 1;
	iCliHeight = rc.bottom - rc.top + 1;

	if(xPos>=iWinWidth || xPos<0){
		printerror("show bitmap out of range.");
		return false;
	}
	if(yPos>=iWinHeight || yPos<0){
		printerror("show bitmap out of range.");
		return false;
	}
	fp=fopen(pFile,"rb");
	if(!fp){
		printerror("open file error!");
		return false;
	}

	fseek(fp,0x12,0);
	fread(&(infoHeader.biWidth),sizeof(infoHeader.biWidth),1,fp);
	fread(&(infoHeader.biHeight),sizeof(infoHeader.biHeight),1,fp);

	fseek(fp,0x1c,0);
	fread(&(infoHeader.biBitCount),sizeof(infoHeader.biBitCount),1,fp);

	iBmpWidth=infoHeader.biWidth;
	iBmpHeight=infoHeader.biHeight;

	iWidth=min(iBmpWidth,iCliWidth-xPos);
	iHeight=min(iBmpHeight,iCliHeight-yPos);

	pData=malloc(iWidth*iHeight*_lGUI_iBytesDataType);
	if(!pData){
		printerror("alloc memory error!");
		return false;
	}
	if(!ReadBmpToBuf(fp, iWidth, iHeight, pData)){
		free(pData);
		fclose(fp);
		return false;
	}


	iBottom=yPos+iHeight-1;

	iLeftMarge=xPos*_lGUI_iBytesDataType;
	iTopMarge=iWinWidth*yPos*_lGUI_iBytesDataType;
	pCurPointer=pData;
	pOutPointer=hDC->pData+iTopMarge+iLeftMarge;
	for(y=yPos;y<=iBottom;y++){

		memcpy(pOutPointer,pCurPointer,iWidth*_lGUI_iBytesDataType);
		pOutPointer+=iWinWidth*_lGUI_iBytesDataType;
		pCurPointer+=iWidth*_lGUI_iBytesDataType;
	}

	free(pData);
	fclose(fp);
	return true;

}



BOOL 
OutputBmpObj(
	HDC hDC,
	int x,
	int y,
	int iDestWidth, 
	int iDestHeight, 
	HBITMAP hBitmap
)
{
	if(hDC->iType == DC_TYPE_MEM)
		OutputBmpObjMem(hDC,x,y,iDestWidth,iDestHeight,hBitmap);
	else
		OutputBmpObjWin(hDC,x,y,iDestWidth,iDestHeight,hBitmap);
}

BOOL 
OutputBmpObjWin(
	HDC hDC, 
	int x, 
	int y, 
	int iDestWidth, 
	int iDestHeight,
	HBITMAP hBitmap
)
{
	PBITMAP pBitmap;
	int iWidth, iHeight;
	int i,j;
	PCOLORREF pData;
	if(!pBitmap)
		return false;
	iWidth = pBitmap->bmWidth;
	iHeight = pBitmap->bmHeight;
	pData = (PCOLORREF)(pBitmap->bmBits);
	for(i=0;i<iHeight;i++){
		for(j=0;j<iWidth;j++){
			SetPixel(hDC,x+j,y+i,*pData);
			pData++;		
		}
	}
	return true;
}


BOOL 
OutputBmpObjMem(
	HDC hDC, 
	int x,
	int y,
	int iDestWidth, 
	int iDestHeight,
	HBITMAP hBitmap
)
{
	PBITMAP pBitmap;
	int iWidth, iHeight;
	int i;
	PCOLORREF pDestData;
	PCOLORREF pSrcData;
	pBitmap=(PBITMAP)hBitmap;
	if(!pBitmap)
		return false;
	iWidth = pBitmap->bmWidth;
	iHeight = pBitmap->bmHeight;
	pDestData = (PCOLORREF)(hDC->pData);
	if(!pDestData)
		return false;
	pDestData += y*iDestWidth+x;
	pSrcData = (PCOLORREF)(pBitmap->bmBits);
	for(i=0;i<iHeight;i++){
		memcpy((void*)pDestData,(void*)pSrcData,iWidth*_lGUI_iBytesDataType);
		pSrcData += iWidth;
		pDestData += iDestWidth;
	}
	return true;
}



BOOL 
OutputBmpObjWithTrColor(
	HDC hDC,
	int x,
	int y,
	int iDestWidth, 
	int iDestHeight, 
	HBITMAP hBitmap,
	COLORREF crColor
)
{
	if(hDC->iType == DC_TYPE_MEM)
		OutputBmpObjMemWithTrColor(hDC,x,y,iDestWidth,iDestHeight,hBitmap,crColor);
	else
		OutputBmpObjWinWithTrColor(hDC,x,y,iDestWidth,iDestHeight,hBitmap, crColor);
}


BOOL 
OutputBmpObjWinWithTrColor(
	HDC hDC, 
	int x, 
	int y, 
	int iDestWidth, 
	int iDestHeight,
	HBITMAP hBitmap,
	COLORREF crColor
)
{
	PBITMAP pBitmap;
	int iWidth, iHeight;
	int i,j;
	PCOLORREF pData;
	pBitmap=(PBITMAP)hBitmap;
	if(!pBitmap)
		return false;
	iWidth = pBitmap->bmWidth;
	iHeight = pBitmap->bmHeight;
	pData = (PCOLORREF)(pBitmap->bmBits);
	for(i=0;i<iHeight;i++){
		for(j=0;j<iWidth;j++){
			if(*pData != crColor)
				SetPixel(hDC,x+j,y+i,*pData);
			pData++;		
		}
	}
	return true;
}


BOOL 
OutputBmpObjMemWithTrColor(
	HDC hDC, 
	int x,
	int y,
	int iDestWidth, 
	int iDestHeight,
	HBITMAP hBitmap,
	COLORREF crColor
)
{
	PBITMAP pBitmap;
	int iWidth, iHeight;
	int i,j;
	PCOLORREF pDestData;
	PCOLORREF pSrcData;
	pBitmap=(PBITMAP)hBitmap;
	if(!pBitmap)
		return false;
	iWidth = pBitmap->bmWidth;
	iHeight = pBitmap->bmHeight;
	pDestData = (PCOLORREF)(hDC->pData);
	if(!pDestData)
		return false;
	pDestData += y*iDestWidth+x;
	pSrcData = (PCOLORREF)(pBitmap->bmBits);
	for(i=0;i<iHeight;i++){
		for(j=0;j<iWidth;j++){
			if(*pSrcData != crColor)
			*(pDestData+j) = *pSrcData;
			pSrcData++;
		}
		pDestData += iDestWidth;		
	}
	return true;
}

