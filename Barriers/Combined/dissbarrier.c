#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "dissbarrier.h"

void dissbarrier()
{
	int my_msg = 1;
	//MPI_Status status;

	int i, rank, rounds, procs;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	rounds = ceil(log(procs)/log(2));

	if(procs == 1) return;

	for(i=0; i< rounds; i++)
	{
		MPI_Send(&my_msg, 1, MPI_INT, ((int)(rank + pow(2,i)) % procs), 0, MPI_COMM_WORLD);
		MPI_Recv(&my_msg, 1, MPI_INT, (((int)(rank - pow(2,i)) + procs)) % procs, 0, MPI_COMM_WORLD, NULL );
	}
	//usleep(1000); // to ensure proper buffering of print statements

}