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

#ifndef _IPCSOCKET_H_
#define _IPCSOCKET_H_

#ifdef __cplusplus                     
extern "C" {
#endif

//format of IPC message:
/*
 DWORD(message value )  +  DWORD(size of added ) +  added data
|------4------|+|-------4-------|+|.........

size of added include the DWORD itself

*/


void 
TerminateIpcSocket();

//thread cleanup function
//register to do cleanup work while thread exit
void 
cleanup_after_malloc(
	void* allocated_memory
);

void 
cleanup_after_opensock(
	void* fd
);

//===============================================================================
//****************************Server*********************************************
//===============================================================================


//Initial listen socket and create thread for server
BOOL
InitIpcSocketServer();

//main loop for listen thread
void* 
IpcSocketMainLoopServer(
	void* para
);

//Main loop for socket read
void* 
ReadMainLoopServer(
	void* clifd
);

//Process ipc message for sersver
BOOL 
ProcessIpcMessageServer(
	int fd, 
	int iMsg,  
	void* pAttachData, 
	int iAttachSize
);


BOOL 
CreateApplicationServer(
	int fd, 
	void* pAttachData, 
	int iAttachSize
);


//Destroy an application
//socket id
// pAttachData iAttachSize no used 
BOOL 
DestroyApplicationServer(
	int fd, 
	void* pAttachData, 
	int iAttachSize
);



//Show Main Window
//para:
// socket id
// pAttachData iAttachSize no used 
BOOL 
ShowMainWinServer(
	int fd, 
	void* pAttachData, 
	int iAttachSize
);


//send new generaged clip region to confirm the client waitting process
//the content sent by this message is same like send clip change message
void 
SendShowMainWinAnsToClient(
	PlGUIAppStat pStat
);

//Hide main window
//Param
//  socket id
//  pAttachData iAttachSize no used 
BOOL 
HideMainWinServer(
	int fd, 
	void* pAttachData, 
	int iAttachSize
);


//send message to client
//parameter:
//1. application description pointer
//2. message type
//3. message added data
//4. size of added message data
BOOL 
SendMsgByServer(
	PlGUIAppStat pStat,
	int iMsg,
	void* pAttachData,
	int iAttachSize
);


//send clip region change message 
void 
SendClipRgnChangeMsg(
	PlGUIAppStat pStat
);

//recalculate clip regioin of all application main window under current window 
void 
RecalClipRgnUnderThis(
	const PlGUIAppStat pStat
);

//===============================================================================
//****************************client*********************************************
//===============================================================================

//Initial ipc socket for client
BOOL 
InitIpcSocketClient();

//thread main loop for client socket
void* 
ReadMainLoopClient(
	void* srvfd
);


//process ipc message for client
BOOL 
ProcessIpcMessageClient(
	int iMsg,  
	void* pAttachData, 
	int iAttachSize
);


//send message by client
BOOL 
SendMsgByClient(
	int iMsg,
	void* pAttachData,
	int iAttachSize
);


BOOL 
ProcessMouseMsgClient(
	int iMsg,
	int x, 
	int y
);


BOOL 
ProcessMouseDownMsgClient(
	int iMsg, 
	int x, 
	int y
);


BOOL 
ProcessMouseMoveMsgClient(
	int iMsg, 
	int x, 
	int y
);


BOOL 
ProcessMouseUpMsgClient(
	int iMsg, 
	int x, int y
);


void 
SendString2Client(
	char* pString
);


void 
SendKeyDown2Client(
	BYTE byteKeyValue
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _IPCSOCKET_H_ */
