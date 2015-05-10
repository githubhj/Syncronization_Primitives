/*
 * sensereaversal.h
 *
 *  Created on: Feb 17, 2015
 *      Author: harshit
 */

#ifndef SENSEREVERSAL_H_
#define SENSEREVERSAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FALSE 0
#define TRUE 1

typedef struct _SenseReversalOpenMP_Barrier{
	long count;
	int global_sense;
	int* mysense;
	long max_count;
}SenseReversalOpenMP_Barrier;

void SenseReversalOpenMP_BarrierInit(long, SenseReversalOpenMP_Barrier*);

void SenseReversalOpenMP_Barrier_Wait(SenseReversalOpenMP_Barrier*,long);

#endif /* SENSEREVERSAL_H_ */
