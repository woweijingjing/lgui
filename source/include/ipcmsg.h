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

#ifndef _IPCMSG_H_
#define _IPCMSG_H_

#ifdef __cplusplus                     
extern "C" {
#endif


//IPC MESSAGE

//#define QUEUE_ID				137
#define SIZE_IPC_MSG			1024		//
#define MSG_TYPE_SERVER			1			//
	

void 
cleanup_after_malloc(
	void* para
);

//+++++++++++++++++++++++++ Server +++++++++++++++++++++++
BOOL 
InitMsgQueueServer();


BOOL 
DestroyIpcMsgQueue();
//Send IPC message by server
//parameter:
//Message value, Message AttachData, AttachData Size


BOOL 
SendMsgServer(
	PlGUIAppStat pStat,
	int iMsg,
	char* pAttachData,
	int iAttachSize
);


BOOL 
GetMsgServer(
	int* iSourceMsgType, 
	int* iMsg,  
	char* pAttachData, 
	int* iAttachSize
);


void* 
IpcMsgMainLoopServer(
	void* para
);


BOOL 
ProcessIpcMessageServer(
	int iSourceMsgType, 
	int iMsg,  
	char* pAttachData, 
	int iAttachSize
);

//++++++++++++++++++++++++++++++++++++++++client +++++++++++++++++++

BOOL 
InitMsgQueueClient();


BOOL 
SendMsgClient(
	int iMsg,
	char* pAttachData,
	int iAttachSize
);


BOOL 
GetMsgClient(
	int* iSourceMsgType, 
	int* iMsg,  
	char* pAttachData, 
	int* iAttachSize
);


void* 
IpcMsgMainLoopClient(
	void* srvfd
);


BOOL 
ProcessIpcMessageClient(
	int iSourceMsgType, 
	int iMsg,  
	char* pAttachData, 
	int iAttachSize
);


BOOL 
ProcessMouseMessageClient(
	int iMsg,
	int x, 
	int y
	);

#ifdef __cplusplus
}
#endif 

#endif 
