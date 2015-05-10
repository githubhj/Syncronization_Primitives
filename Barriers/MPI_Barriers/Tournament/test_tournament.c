#include <stdio.h>
#include "mpi.h"
#include <math.h>
#include <sys/time.h>
#include "tournamentbarrier.h"

#define NUM_BARRIERS 100

double elapsedTime(struct timeval t1, struct timeval t2){
  double delta;
  delta = (t2.tv_sec - t1.tv_sec) * 1000000.0;      // sec to ms
  delta += (t2.tv_usec - t1.tv_usec) ;   // us to ms
  return delta; 
}

int main(int argc, char** argv)
{
	int rank, size;
	struct timeval t1,t2;
	double elapsed_time, temp_sum, temp_store;
	temp_sum = 0;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	tournamentbarrier_init();

	MPI_Barrier(MPI_COMM_WORLD);

	for( int f=0; f<NUM_BARRIERS; f++ ){
		struct timespec begin,end;
  		double elapsed_time;

	gettimeofday(&t1, NULL); 
    tournamentbarrier();
    gettimeofday(&t2, NULL);

    elapsed_time = elapsedTime(t1,t2);

    if(rank == 0)
    {
    	int i;
    	for(i=1; i<size; i++)
    	{
    		MPI_Recv(&temp_store, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, NULL );
    		temp_sum += temp_store;
    	}
    }

    else
    {
		MPI_Send(&elapsed_time, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

	}

	if(rank == 0)
		printf("time taken by for %d nodes is %f\n", size, temp_sum/(double)size);

	MPI_Finalize();
	return 0;

}

	
	

	



