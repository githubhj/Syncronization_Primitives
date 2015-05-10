/*
 * ticket_lock.c
 *
 *  Created on: Mar 11, 2015
 *      Author: harshit
 */
#include "ticket_lock.h"

/*ticket_lock_init: Takes a pointer to ticket_lock as an argument
 * Initializes next_ticket and now serving to 0*/
void ticket_lock_init(ticket_lock_t* lock){
	lock->next_ticket = 0;
	lock->now_serving = 0;
}

/*ticket_acquire_lock: Takes a pointer to ticket_lock as an argument
 * tries to acquire lock using ticket lock algorithm*/
void ticket_lock_acquire(ticket_lock_t* lock){

	assert(lock->next_ticket < 	ULONG_MAX);

	long my_ticket = __sync_fetch_and_add(&lock->next_ticket,1);

	while(1){
		//usleep(10*(my_ticket-lock->now_serving));
		if(lock->now_serving == my_ticket){
			return;
		}
	}
}

/*ticket_lock_release: Takes a pointer to ticket_lock as an argument
 * Releases lock by incrementing now_serving*/
void ticket_lock_release(ticket_lock_t* lock){
	lock->now_serving++;
}
