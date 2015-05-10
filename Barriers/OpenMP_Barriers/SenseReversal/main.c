//============================================================================
// Name        : AOS_OMP_Barrier.cpp
// Author      : Harshit Jain
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <omp.h>
#include "sensereversal.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

double get_wall_clock_time(){
	struct timeval t1;
	gettimeofday(&t1, NULL);
	return ((double)t1.tv_usec);
}

double get_time_avg(double* time_array, int size){
	double time_avg;
	int i;

	for(i=0; i<size; i++){
		time_avg += time_array[i];
	}

	return time_avg/size;
}

int main(int argc, char** argv) {

	if(argc <= 2){
		fprintf(stderr,"Enter number of threads and number iterations\n");
		exit(1);
	}

	//First argument is number of threads
	int nothreads = atoi(argv[1]);
	int noiterations = atoi(argv[2]);

	//Local Iterator
	int i;

	//Time Array
	double* time_array = (double*)malloc(sizeof(double)*nothreads);
	double* time_array_iteration = (double*)malloc(sizeof(double)*noiterations);

	srand (time(NULL));

	SenseReversalOpenMP_Barrier sensebarrier;

	//Data container
	int* a = (int*)malloc(sizeof(int)*nothreads);

	SenseReversalOpenMP_BarrierInit(nothreads, &sensebarrier);

#pragma omp parallel for private(i) shared(a, nothreads, sensebarrier) num_threads(nothreads)

	for(i = 0; i<nothreads; i++){
		a[i] =  rand() % 100 + 1;
	}

	for(i = 0; i<noiterations; i++){

		#pragma omp parallel shared(a, nothreads, sensebarrier) num_threads(nothreads)
			{
				double t2,t1;

				int thread_id = omp_get_thread_num();

				t1 = get_wall_clock_time();

				SenseReversalOpenMP_Barrier_Wait(&sensebarrier, thread_id);

				t2 = get_wall_clock_time();

				time_array[thread_id] = t2 - t1;


			}

			time_array_iteration[i] = get_time_avg(time_array,nothreads);

	}

	printf("Average time taken: %f\n", get_time_avg(time_array_iteration,noiterations));

	return 0;
}
