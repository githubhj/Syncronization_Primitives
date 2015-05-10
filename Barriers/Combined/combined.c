#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sensereversal.h"
#include "dissbarrier.h"
#include <omp.h>

void combined_barrier_init(SenseReversalOpenMP_Barrier* barrier, int num_threads)
{
	long numthreads = num_threads;
	SenseReversalOpenMP_BarrierInit(numthreads, barrier);

}

void combined_barrier(SenseReversalOpenMP_Barrier* barrier)
{
	long threadid = omp_get_thread_num();


	SenseReversalOpenMP_Barrier_Wait(barrier, threadid);


	if(threadid == 0)
	{
		dissbarrier();

	}

	#pragma omp barrier

	return;

}
