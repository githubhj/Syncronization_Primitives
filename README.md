# Syncronization_Primitives
Implementation of locks and barriers in OpenMP and MPI. Ticket-lock and array-lock were implemented in OpenMP, where as Tree barrier and centralized barrier were implemented in Open MP and MPI.

## Locks
==============

#####Ticket Lock

* API implemented in /Locks/ticketlock/
* Implemented three fucntions:

  1. *void ticket_lock_init(ticket_lock_t lock)* 	    : Initialize next_ticket and now_serving to 0.
  2. *void ticket_lock_acquire(ticket_lock_t lock)* 	: Acquire lock, do fetch and add on next_ticket and wait till my_ticket is equal to now serving.
  3. *void ticket_lock_release(ticket_lock_t lock)*	  : Release lock, increment now_serving.
  
* Analysis: A multithreaded bubble sort is implemented using this lock API. Mutiple instanses with constant data segments are evaluate using Regress.py on a cluster with 2 6-core Intel Westmere processors and the results were noted on /Locks/[ECE 6101 PDCA]Homework4.pdf

#####Array Lock

*
*

