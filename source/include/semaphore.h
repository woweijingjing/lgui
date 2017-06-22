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

#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#ifdef __cplusplus                     
extern "C" {
#endif


union semun{
	int val;
	struct semid_ds* buf;
	unsigned short int *array;
};



int 
sem_create(
	key_t sem_id
);


void 
sem_lock(
	int sem_set_id
);


void 
sem_unlock(
	int sem_set_id
);


void 
sem_remove(
	int sem_set_id
);


 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 



