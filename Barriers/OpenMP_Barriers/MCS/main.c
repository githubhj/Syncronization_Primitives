/*
 * Test2_MCS.cpp
 *
 *  Created on: Mar 7, 2015
 *      Author: harshit
 */
#include <stdio.h>
#include <omp.h>
#include "mcs_tree.h"
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
	for(i = 0; i< nothreads;i++){
		time_array[i] = 0;
	}
	double* time_array_iteration = (double*)malloc(sizeof(double)*noiterations);

	//Data container
	int* a = (int*)malloc(sizeof(int)*nothreads);

	//Set rand seed
	srand (time(NULL));

	//Main Print
	//printf("In Main: Starting Sense Reversal Barrier thread spawning\n");

	//Barrier Data structure
	McsTreeOpenMP_Node* mcsbarrier;

	//Init Barrier
	McsTreeOpenMP_BarrierInit(&mcsbarrier, nothreads);


// Init threads
#pragma omp parallel for private(i) shared(nothreads) num_threads(nothreads)
	for(i = 0; i<nothreads; i++){
		a[i] =  rand()%100 + 1;
		//printf("Thread %d: a[i] = %d is randomly set\n", i, a[i]);
	}

//Time stamps
double t2,t1;

for(i = 0; i<noiterations; i++){
	//Main thread spawning
	#pragma omp parallel private(t1,t2) shared(a, nothreads,time_array) num_threads(nothreads)
		{
			int thread_id = omp_get_thread_num();
			int k,l;

			//Random Delay
//			for(k = 0; k < 1000 ; k++){
//				for(l = 0; l < 1000 ; l++){
//				}
//			}

			t1 = get_wall_clock_time();

			McsTreeOpenMP_BarrierAwait(mcsbarrier, thread_id);

			t2 = get_wall_clock_time();

			time_array[thread_id] = t2 - t1;
		}

	time_array_iteration[i] = get_time_avg(time_array,nothreads);
	
}

	printf("Average time taken: %f\n", get_time_avg(time_array_iteration,noiterations));

	return 0;
}
