#include "sort.h"

int * bubble_sort_serial(int * unsorted, long num_elems) {
  int * sorted = (int*)calloc(sizeof(int),num_elems);
  for (int i=0; i < num_elems; i++) {
    sorted[i] = unsorted[i];
  }

  int tmp;

  for (long i=0; i < num_elems; i++) {
    for (long j=0; j < num_elems - i; j++) {
      if (sorted[j] > sorted[j+1]) {
        tmp = sorted[j];
        sorted[j] = sorted[j+1];
        sorted[j+1] = tmp;
      }
    }
  }

  return sorted;
}

int * bubble_sort_parallel(int * unsorted, long num_elems, int threads) {
  int * sorted = (int*)calloc(sizeof(int),num_elems);
  for (int i=0; i < num_elems; i++) {
    sorted[i] = unsorted[i];
  }

  int tmp;
  int change = 1;
  long j = 0;
  int tid;
  int nthreads;
  long N = num_elems;

  omp_lock_t lock;
  omp_init_lock(&lock);

  while (change) {
    change = 0;

    #pragma omp parallel private(j,tmp,tid,nthreads) shared(change,sorted,N) num_threads(threads)
    {
      tid = omp_get_thread_num();
      nthreads = omp_get_num_threads();

      long min1 = (long)(((double)tid/nthreads)*N);
      long min2 = (long)(((double)tid/nthreads)*N);
      long max = (long)(((double)(tid+1)/nthreads)*N);
      
      if (min1%2 == 1) {
        min1++;
      }

      if (min2%2 == 0) {
        min2++;
      }

      for (j=min1; j < max; j+=2) {
        if (j==N) continue;
        if (sorted[j] > sorted[j+1]) {
          tmp = sorted[j];
          sorted[j] = sorted[j+1];
          sorted[j+1] = tmp;
          if (change == 0) {
            omp_set_lock(&lock);
            change++;
            omp_unset_lock(&lock);
          }
        }
      }

      #pragma omp barrier

      for (j=min2; j < max; j+=2) {
        if (j==N) continue;
        if (sorted[j] > sorted[j+1]) {
          tmp = sorted[j];
          sorted[j] = sorted[j+1];
          sorted[j+1] = tmp;
          if (change == 0) {
            omp_set_lock(&lock);
            change++;
            omp_unset_lock(&lock);
          }
        }
      }
    }

  }

  return sorted;
}
