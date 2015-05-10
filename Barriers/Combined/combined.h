#ifndef COMBINED_H_
#define COMBINED_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sensereversal.h"

void combined_barrier_init(SenseReversalOpenMP_Barrier*, int);
void combined_barrier(SenseReversalOpenMP_Barrier*);

//void combined_barrier_init(McsTreeOpenMP_Node**,int);
//void combined_barrier(McsTreeOpenMP_Node*);

#endif
