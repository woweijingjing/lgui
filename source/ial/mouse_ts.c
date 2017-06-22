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
#include "../include/mouse_ts.h"
#include "../include/mouse_ial.h"


static int 
TS_Open(void);


static void 
TS_Close(void);


static int 
TS_Read(
	int *px, 
	int *py, 
	int *pb
);


static void 
GetDefaultAccel(
	int *pscale,
	int *pthresh
);


static int 
PD_Read(
	int *px, 
	int *py, 
	int *pz, 
	int *pb
);


static int 
filter_transform(
	int state, 
	int *xpos, 
	int *ypos
);


static int 
filter_absrotate(
	int state, 
	int *xpos, 
	int *ypos
);


static void 
GdMoveMouse(
	int newx, 
	int newy
);


MOUSEDEVICE mousedev_TS = {
        TS_Open,
        TS_Close,
        TS_Read
};

TSMOUSEINFO tsMouseInfo={0,0,STATUS_PENUP};


static MWTRANSFORM g_trans;	/* current transform*/

static struct {
	int x;
	int y;
	int count;
} jitter = { 0, 0, 0};

static int	flags;		/* input filter flag*/
static int	mouse_fd;
static int	changed;	/* mouse state has changed */
static int	scale;		/* acceleration scale factor */
static int	thresh;		/* acceleration threshhold */
static int 	xpos;		/* current x position of mouse */
static int	ypos;		/* current y position of mouse */
static int	buttons;	/* current state of buttons */
static int	maxx;		/* maximum allowed x position */
static int	minx;		/* minimum allowed x position */
static int	miny;		/* minimum allowed y position */
static int	maxy;		/* maximum allowed y position */





static int 
TS_Open(void)
{
	int fd;

	/* init mouse position info*/
	buttons = 0;
	xpos	= 0;
	ypos	= 0;
	changed = TRUE;
	minx	= MIN_MWCOORD;
	miny	= MIN_MWCOORD;
	maxx	= MAX_MWCOORD;
	maxy	= MAX_MWCOORD;

	if((mouse_fd = open(TS_DEVICE_FILE, O_NONBLOCK)) < 0) {
		EPRINTF("Error %d opening %s touchscreen device [%s]\n", 
			errno, TS_DEVICE, TS_DEVICE_FILE);
		return -1;
	}

	GetDefaultAccel(&scale, &thresh);

	flags &= ~MOUSE_RAW;
	
	return mouse_fd;
}


static void 
TS_Close(void)
{ 
	if(mouse_fd < 0) return;
 
	close(mouse_fd);
	mouse_fd = -1;
}




static int 
TS_Read(
	int *px, 
	int *py, 
	int *pb
)
{
	int	x, y, z;
	int	newbuttons;	
	int	status;		/* status of reading mouse */
	int dx, dy;
	
	int event;

	*px = xpos;
	*py = ypos;
	*pb = buttons;

	if (changed) {
		changed = FALSE;
		return 1;
	}

	status = PD_Read(&x, &y, &z, &newbuttons);
	if (status < 0)
		return -1;


	if (status == 0)
		return 0;


	dx = x;
	dy = y;


	if (flags & MOUSE_TRANSFORM)  {
		if (!filter_transform(status, &dx, &dy))
			return 0;
	}

	if (!(flags & MOUSE_RAW))
		filter_absrotate(status, &dx, &dy);


	if (buttons != newbuttons) {
		changed = TRUE;
		buttons = newbuttons;
	}


	if (status != 3)
		GdMoveMouse(dx, dy);


	if (!changed)
		return 0;


	changed = FALSE;
	*px = xpos;
	*py = ypos;
	*pb = buttons;


	*px = (220 - *px)*240/210;
	*py = (310 - *py)*320/310;
	
	if((tsMouseInfo.nLastStatus == STATUS_PENDOWN) && *pb)
		event = MOUSE_EVENT_MOVE;
	else if((tsMouseInfo.nLastStatus == STATUS_PENDOWN) && !*pb)
		event = MOUSE_EVENT_LBUTTON_UP;
	else if((tsMouseInfo.nLastStatus == STATUS_PENUP) && *pb) 
		event = MOUSE_EVENT_LBUTTON_DOWN;

	tsMouseInfo.nLastStatus = *pb;		

	*pb = event;

	if(*px<0 || *py<0)
		return 0;
	return 1;
}


static int 
PD_Read(
	int *px, 
	int *py, 
	int *pz, 
	int *pb
)
{
	TS_EVENT event;
	int bytes_read;
  
	bytes_read = read(mouse_fd, &event, sizeof(event));
  
	if(bytes_read != sizeof(event)) {
		if(bytes_read == -1) {
			if(errno == EINTR || errno == EAGAIN) return 0;
			EPRINTF("[%s] Error %d reading from touch panel\n", TS_DEVICE, errno);
			return -1;
		}

		EPRINTF("[%s] Wrong number of bytes %d read from touch panel "
			"(expected %d)\n", TS_DEVICE, bytes_read, sizeof(event));
		return 0;
	}

	*px = event.x;
	*py = event.y;


	*pb = (event.pressure > 50) ? MWBUTTON_L : 0;

	*pz = event.pressure;
  
	if(!*pb)
		return 3;
	return 2;
}


static void 
GetDefaultAccel(
	int *pscale,
	int *pthresh
)
{
	*pscale = 3;
	*pthresh = 5;
}



static int 
filter_transform(
	int state, 
	int *xpos, 
	int *ypos
)
{
	/* No transform data is available, just return the raw values */
	if (state == 3) {
		jitter.count = jitter.x = jitter.y = 0;
		return 1;
	} else if (state == 1)
		return 1;

	if (jitter.count == JITTER_DEPTH) {
		jitter.x = jitter.x >> JITTER_SHIFT_BITS;
		jitter.y = jitter.y >> JITTER_SHIFT_BITS;

		/* No translation if transform not setup yet*/
		if (g_trans.s && !(flags & MOUSE_RAW)) {
			*xpos = ((g_trans.a * jitter.x +
				  g_trans.b * jitter.y +
				  g_trans.c) / g_trans.s);
			*ypos = ((g_trans.d * jitter.x +
				  g_trans.e * jitter.y +
				  g_trans.f) / g_trans.s);

			jitter.count = jitter.x = jitter.y = 0;
			return 1;
		} else {
			*xpos = jitter.x;
			*ypos = jitter.y;

			jitter.count = jitter.x = jitter.y = 0;
			return 2;
		}
	}

	jitter.x += *xpos;
	jitter.y += *ypos;
	++jitter.count;
	return 0;		/* In other words, don't use the returned value */
}

static int 
filter_absrotate(
	int state, 
	int *xpos, 
	int *ypos
)
{
	int x = *xpos;
	int y = *ypos;

	if (state == 3)
		return 0;


	*xpos = x;
	*ypos = y;


	return 0;
}


void 
GdMoveMouse(
	int newx, 
	int newy
)
{
	if (!(flags & MOUSE_RAW)) {
		if (newx < minx)
			newx = minx;
		if (newx > maxx)
			newx = maxx;
		if (newy < miny)
			newy = miny;
		if (newy > maxy)
			newy = maxy;
	}

	if (newx == xpos && newy == ypos)
		return;

	changed = TRUE;

	xpos = newx;
	ypos = newy;
}



