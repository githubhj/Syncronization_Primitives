/*
 * array_lock.h
 *
 *  Created on: Mar 11, 2015
 *      Author: harshit
 */

#ifndef ARRAY_LOCK_H_
#define ARRAY_LOCK_H_


#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#define HAS_LOCK 1
#define MUST_WAIT 0

typedef struct _array_lock_t{
	unsigned char* lock_flags;
	unsigned long queuelast;
	unsigned long max_threads;
	unsigned long curent_lock_pos;
}array_lock_t;

void array_lock_init(array_lock_t*, unsigned long);
void array_lock_acquire(array_lock_t*);
void array_lock_release(array_lock_t*);

#endif /* ARRAY_LOCK_H_ */
