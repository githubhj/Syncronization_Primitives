#ifndef DISSBARRIER_H_
#define DISSBARRIER_H_

#include <stdio.h>
#include <stdlib.h>

struct participant{
	unsigned int sendTo;
	unsigned int recvFrom;
	unsigned int pid;
};

void disshonbarrier();

#endif
