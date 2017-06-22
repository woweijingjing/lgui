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
#include "../include/mouse_ial.h"

MOUSEDEVICE* mousedev;

extern MOUSEDEVICE  mousedev_PS2;
extern MOUSEDEVICE  mousedev_TS;

void 
InstallMouseDevice()
{
#ifdef _PC_VERSION
	mousedev = &mousedev_PS2;
#else
	mousedev = &mousedev_TS;
#endif
}


int 
OpenMouse()
{
	return mousedev->Open();
}


void 
CloseMouse()
{
	mousedev->Close();
}


int 
ReadMouse(
	int* x, 
	int* y, 
	int* event
)
{
	return mousedev->Read(x, y, event);
}

