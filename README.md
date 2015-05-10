# Syncronization_Primitives
Implementation of locks and barriers in OpenMP and MPI. Ticket-lock and array-lock were implemented in OpenMP, where as Tree barrier and centralized barrier were implemented in Open MP and MPI.

## Locks
==============

###Ticket Lock

* API implemented in /Locks/ticketlock/
* Implemented three fucntions: 
** *void ticket_lock_init(ticket_lock_t* lock)* 	: Initialize next_ticket and now_serving to 0.
** *void ticket_lock_acquire(ticket_lock_t* lock)* 	: Acquire lock, do fetch and add on next_ticket and wait till my_ticket is equal to now serving
** *void ticket_lock_release(ticket_lock_t* lock)*	: Release lock, increment now_serving.


