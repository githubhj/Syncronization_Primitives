#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

#include "sort.h"

void init_array(int * list, long num_elems, unsigned int seed) {
  srand(seed);
  for (long i=0; i < num_elems; i++) {
    list[i] = rand();
  }
}

int validate_sort(int * list, long num_elems) {
  int last = list[0];
  for(long i=1; i < num_elems; i++) {
    if (last > list[i]) {
      return -1;
    }
    last = list[i];
  }
  return 1;
}

int main(int argc, char * argv[]) {
  long num_elems = atol(argv[1]);
  unsigned int seed = atoi(argv[2]);
  int threads = atoi(argv[3]);

  int unsorted[num_elems];
  int * sorted;

  printf("Num Elems: %ld\n",num_elems);
  printf("Seed: %u\n",seed);
  printf("Threads: %d\n",threads);

  printf("\nInitializing Array...");

  init_array(unsorted, num_elems, seed);

  printf("Done\n");
  
  printf("\nSorting...");


  /* NOTE: ONLY TIME THE CALL TO 
   * bubble_sort_serial() or bubble_sort_parallel()
   */
  struct timeval start, end;

  gettimeofday(&start, NULL);
  if (threads == 1) {
    sorted = bubble_sort_serial(unsorted,num_elems);
  } else {
    sorted = bubble_sort_parallel(unsorted,num_elems,threads);
  }
  gettimeofday(&end, NULL);

  printf("Done\n");

  printf("\nValidating Result...");  
  int valid = validate_sort(sorted,num_elems);
  if (valid == 1) {
    printf("\nSorting valid!\n");
  } else {
    printf("\nINVALID Sorting!\n");
  }
  printf("Done\n");
  long diff;

  if (start.tv_sec == end.tv_sec) {
    diff = end.tv_usec - start.tv_usec;
  } else {
    diff = 1e6 - start.tv_usec;
    diff += end.tv_usec;
    diff += 1e6*(end.tv_sec - (start.tv_sec + 1));
  }

  printf("Time: %lf\n",(double)diff/1e6);
}
