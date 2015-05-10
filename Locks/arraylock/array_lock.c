/*
 * array_lock.c
 *
 *  Created on: Mar 11, 2015
 *      Author: harshit
 */

#include "array_lock.h"

void array_lock_init(array_lock_t* lock, unsigned long num_threads){

	lock->max_threads = num_threads;
	lock->curent_lock_pos = 0;

	lock->lock_flags = (unsigned char*)malloc(sizeof(unsigned char)*num_threads);
	lock->lock_flags[0] = HAS_LOCK;

	int i;
	for(i=1;i<num_threads;i++){
		lock->lock_flags[i] = MUST_WAIT;
	}

	lock->queuelast = 0;
}

void array_lock_acquire(array_lock_t* lock){
	unsigned long my_place = __sync_fetch_and_add(&lock->queuelast,1);
	unsigned long my_actual_place = my_place%lock->max_threads;

	while(lock->lock_flags[my_actual_place] == MUST_WAIT);
	lock->curent_lock_pos = my_place;
}

void array_lock_release(array_lock_t* lock){
	assert(lock->lock_flags[(lock->curent_lock_pos%lock->max_threads)] == HAS_LOCK);

	lock->lock_flags[(lock->curent_lock_pos%lock->max_threads)] = MUST_WAIT;

	lock->lock_flags[((lock->curent_lock_pos+1)%lock->max_threads)] = HAS_LOCK;
}


