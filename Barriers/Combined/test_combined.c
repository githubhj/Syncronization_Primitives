#include <stdio.h>
#include "mpi.h"
#include <math.h>
#include <sys/time.h>
#include "combined.h"
#include "sensereversal.h"
#include <omp.h>

#define NUM_BARRIERS 100

double elapsedTime(struct timeval t1, struct timeval t2){
  double delta;
  delta = (t2.tv_sec - t1.tv_sec) * 1000000.0;
  delta += (t2.tv_usec - t1.tv_usec) ;
  return delta;
}

int main(int argc, char** argv)
{
		  int rank, size, f, NUMTHREADS;
		  double temp_sum = 0.0;
		  double temp_store = 0.0;


		  double elapsed_time_array[100];
		  double elapsed_time = 0.0;

		  MPI_Init(&argc, &argv);

		  NUMTHREADS = atoi(argv[1]);

		  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		  MPI_Comm_size(MPI_COMM_WORLD, &size);

		  SenseReversalOpenMP_Barrier barrier_obj;
		  combined_barrier_init(&barrier_obj, NUMTHREADS);

		  //MPI_Barrier();

		  int i;
		  for(i=0; i<NUM_BARRIERS; i++)
		  {
				#pragma omp parallel shared(temp_sum, elapsed_time_array, barrier_obj, elapsed_time) num_threads(NUMTHREADS)
			  	{
			  		struct timeval t1,t2;

			  		int thread_id = omp_get_thread_num();

			  		gettimeofday(&t1, NULL);
			  		combined_barrier(&barrier_obj);
			  		gettimeofday(&t2, NULL);

			  		elapsed_time_array[thread_id] = elapsedTime(t1,t2);

					#pragma omp barrier

			  		if(thread_id == 0)
			  		{
			  			int j;
			  			for(j=0; j<NUMTHREADS; j++)
			  			{
			  				elapsed_time += elapsed_time_array[j];
			  			}

			  		}





			  	}

		  }

		  if(rank == 0)
		  {
			  int j;
			  for(j=1; j<size; j++)
			  {
				MPI_Recv(&temp_store, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, NULL );
			  	temp_sum += temp_store;
			  }
			  temp_sum += elapsed_time;
		  }

		  else{
			  MPI_Send(&elapsed_time, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		  }

		  if(rank == 0)
		      printf("time taken by for %d nodes and %d threads is %f s and %f us \n", size, NUMTHREADS, temp_sum/(double)(1000000.0*size*NUMTHREADS), temp_sum/(double)(NUM_BARRIERS*NUMTHREADS));

		  MPI_Finalize();
		  return;


}
