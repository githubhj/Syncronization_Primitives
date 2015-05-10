/*
 * ticket_lock.h
 *
 *  Created on: Mar 11, 2015
 *      Author: harshit
 */

#ifndef TICKET_LOCK_H_
#define TICKET_LOCK_H_

#include <limits.h>
#include <assert.h>
#include <unistd.h>

/*Ticket Lock Data Structure:
 * Contains:
 * 1. next_ticket: value of the next ticket that can be obtained while acquirng a lock
 * gets incremented on each lock request
 * 2. now_serving: ticket value that is being serviced with the lock at present*/
typedef struct _ticket_lock_t{
	unsigned long next_ticket;
	unsigned long now_serving;
}ticket_lock_t;


/*Declaration for Ticket Lock Init function*/
void ticket_lock_init(ticket_lock_t* lock);

/*Declaration for Ticket Lock Acquire function*/
void ticket_lock_acquire(ticket_lock_t* lock);

/*Declaration for Ticket Lock Release function*/
void ticket_lock_release(ticket_lock_t* lock);

#endif /* TICKET_LOCK_H_ */
