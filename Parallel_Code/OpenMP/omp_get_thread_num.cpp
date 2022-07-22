#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel 
  {
  	int thread = omp_get_thread_num();
  	int max_threads = omp_get_max_threads();
  	printf("Hello World(Thread %d of %d)\n", thread, max_threads);
  }
  return 0;
} 
