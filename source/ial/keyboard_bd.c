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
#include "../include/keyboard_ial.h"
#include "../include/keyboard_bd.h"


static int kb_fd;
static KBC_EVENT buf;

KBDEVICE kbdev_BD = {
        BDKB_Open,
        BDKB_Close,
        BDKB_Read
};

int 
BDKB_Open(void)
{
	
	kb_fd = open(KB_DEV_FILE,O_RDONLY);
	if ( kb_fd<0 ){
		printf("Open device kdc error!!!\n\n\n");
		return -1;
	}
}


void 
BDKB_Close(void)
{
	close(kb_fd);
}


int 
BDKB_Read(
	BYTE* btScanCode,
	BYTE* btPressed
)
{
	int ret;
	int size;
	char buffer[10];
	size = sizeof(KBC_EVENT);
	ret = read(kb_fd,&buf,size);
	if(ret == size){
		*btScanCode = buf.btScanCode;
		*btPressed = buf.btPressed;
		return 1;
	}
	else{
		printf("read keyboard error\n");
		return 0;
	}
	return 1;
}




