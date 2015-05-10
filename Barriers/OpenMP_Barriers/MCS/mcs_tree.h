/*
 * mcs_tree.h
 *
 *  Created on: Feb 17, 2015
 *      Author: harshit
 */

#ifndef MCS_TREE_H_
#define MCS_TREE_H_

typedef struct _McsTreeOpenMP_Node{
	//4-Ary Arrival

	// Parent Node
	int arrival_parent;

	//Children Nodes
	int arrival_chidren[4];

	//Has Children
	int num_arrival_children;

	//Children Arrived
	int arrived_child_notready[4];

	//Binary Wakeup tree
	//Parent Node
	int wakeup_parent;

	//Children Nodes
	int wakeup_left_child;
	int wakeup_right_child;

	//Wakeup Flag
	int wakeup[2];
}McsTreeOpenMP_Node;

//typedef McsTreeOpenMP_Node* McsTreeOpenMP_Node_Ptr;

void McsTreeOpenMP_BarrierInit(McsTreeOpenMP_Node**,long numthreads);

void McsTreeOpenMP_BarrierAwait(McsTreeOpenMP_Node*, long threadid);
#endif /* MCS_TREE_H_ */
