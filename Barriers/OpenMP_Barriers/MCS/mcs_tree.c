/*
 * mcs_tree.c
 *
 *  Created on: Feb 17, 2015
 *      Author: harshit
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "mcs_tree.h"

#define NOTAWAKE 0
#define AWAKE 1

#define ARRIVED 1
#define UNARRIVED 0

#define DOES_NOT_EXIST -1

void McsTreeOpenMP_BarrierInit(McsTreeOpenMP_Node** mybarrier, long numthreads){

	int i,j;

	McsTreeOpenMP_Node* barrier_array = (McsTreeOpenMP_Node*) malloc(numthreads*sizeof(McsTreeOpenMP_Node));
	//printf("%lu\n",sizeof(McsTreeOpenMP_Node));


	for(i=0; i<numthreads ; i++){
		barrier_array[i].arrival_parent = DOES_NOT_EXIST;
		barrier_array[i].num_arrival_children = 0;
		barrier_array[i].wakeup_left_child = DOES_NOT_EXIST;
		barrier_array[i].wakeup_right_child = DOES_NOT_EXIST;
		barrier_array[i].wakeup_parent = DOES_NOT_EXIST;

		for(j =0; j<4;j++){
			barrier_array[i].arrival_chidren[j] = DOES_NOT_EXIST;
			barrier_array[i].arrived_child_notready[j] = UNARRIVED;
		}

		barrier_array[i].wakeup[0] = NOTAWAKE;
		barrier_array[i].wakeup[1] = NOTAWAKE;
	}


	for(i=0; i<numthreads ; i++){

		if(i==0){
			//Setup root

			//Set Parents as NULL
			barrier_array[0].arrival_parent = DOES_NOT_EXIST;
			barrier_array[0].wakeup_parent = DOES_NOT_EXIST;

			//Set Wakeup spinlocks as NOTWAKE
			barrier_array[0].wakeup[0] = NOTAWAKE;
			barrier_array[0].wakeup[1] = NOTAWAKE;

			//Let Initial Number of arrival Children be 0
			barrier_array[0].num_arrival_children =0;

			//Let Number of Wakeup Children be NULL
			barrier_array[0].wakeup_left_child = DOES_NOT_EXIST;
			barrier_array[0].wakeup_right_child = DOES_NOT_EXIST;

			for(j=0;j<4;j++){
				barrier_array[0].arrived_child_notready[j] = DOES_NOT_EXIST;
				barrier_array[0].arrival_chidren[j] = DOES_NOT_EXIST;
			}
		}

		else{

			//Set arrival and wakeup parent Ids
			barrier_array[i].wakeup_parent = ((i-1)/2);
			barrier_array[i].arrival_parent = ((i-1)/4);

			//Set Arrival Parents Children nodes
			barrier_array[barrier_array[i].arrival_parent].arrival_chidren[((i-1)%4)] = i;
			barrier_array[barrier_array[i].arrival_parent].arrived_child_notready[((i-1)%4)] = UNARRIVED;
			barrier_array[barrier_array[i].arrival_parent].num_arrival_children++;

			for(j=0;j<4;j++){
				barrier_array[i].arrived_child_notready[j] = DOES_NOT_EXIST;
			}

			barrier_array[i].wakeup[0] = NOTAWAKE;
			barrier_array[i].wakeup[1] = NOTAWAKE;

			if((i-1)%2==1){
				barrier_array[barrier_array[i].wakeup_parent].wakeup_left_child= i;
			}
			else{
				barrier_array[barrier_array[i].wakeup_parent].wakeup_right_child= i;
			}

		}

	}

	*mybarrier = barrier_array;
}

void McsTreeOpenMP_BarrierAwait(McsTreeOpenMP_Node* barrier, long threadid){
	int i,j;

	int my_child_no = barrier[threadid].num_arrival_children;
	int arrived_num =0;
	int break_out = 0;

	if(my_child_no != 0 ){

		while(1){
			for(i=0;i<my_child_no;i++){

				if(barrier[threadid].arrived_child_notready[i] == ARRIVED){
					arrived_num++;
				}
			}

			if(arrived_num == my_child_no){
				break_out = 1;
			}
			else{
				arrived_num = 0;
			}

			if(break_out){

				for(j=0;j<my_child_no;j++){
					barrier[threadid].arrived_child_notready[j] = UNARRIVED;
				}

				break;
			}
		}
	}

	if(barrier[threadid].arrival_parent != DOES_NOT_EXIST){
		barrier[barrier[threadid].arrival_parent].arrived_child_notready[(threadid-1)%4] = ARRIVED;
	}

	if(threadid != 0){
		while(!barrier[barrier[threadid].wakeup_parent].wakeup[(threadid%2)]);
		barrier[barrier[threadid].wakeup_parent].wakeup[(threadid%2)] = NOTAWAKE;
	}

	if(barrier[threadid].wakeup_left_child != DOES_NOT_EXIST){
		barrier[threadid].wakeup[0] = AWAKE;
	}

	if(barrier[threadid].wakeup_right_child != DOES_NOT_EXIST){
		barrier[threadid].wakeup[1] = AWAKE;
	}

}


