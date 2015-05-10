/*
 * sensereversal.c
 *
 *  Created on: Feb 17, 2015
 *      Author: harshit
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sensereversal.h"

void SenseReversalOpenMP_BarrierInit(long num_threads, SenseReversalOpenMP_Barrier* barrier){
	int i;
	barrier->count = num_threads;
	barrier->mysense = (int*)malloc(sizeof(int)*num_threads);
	barrier->max_count = num_threads;
	barrier->global_sense = FALSE;
	for(i=0;i<num_threads;i++){
		barrier->mysense[i] = TRUE;
	}
}

void SenseReversalOpenMP_Barrier_Wait(SenseReversalOpenMP_Barrier* barrier,long threadid){

	assert(threadid < barrier->max_count);

#pragma omp critical
	{
		barrier->count--;
	}

	if(barrier->count==0){
		barrier->count = barrier->max_count;
		barrier->global_sense = barrier->mysense[threadid];
	}
	else{
		//printf("entering while 1 by %d\n", threadid);
		while(barrier->global_sense !=  barrier->mysense[threadid]);
		//printf("leaving while 1 by %d\n", threadid);

	}

	if(barrier->mysense[threadid]==1){
		barrier->mysense[threadid] = 0;
	}
	else{
		barrier->mysense[threadid] =1;
	}
}


