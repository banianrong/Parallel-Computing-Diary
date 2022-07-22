/*
//edition 1
#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel
  {
    int thread = omp_get_thread_num();
    for(int i = 0; i < 4; i++)
	  printf("thread %d\n", thread);
  }
  return 0;
} 
*/
/*
//edition 2

#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel
  {
  	int n;
  	#pragma omp for
  	for(n = 0; n < 5; n++) {
  	  int thread = omp_get_thread_num();
	  printf("thread %d\n", thread);	
	}
  }
  return 0;
}
*/
/*
//edition 3

#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel for
  for(int i = 0; i < 5; i++) {
    int thread = omp_get_thread_num();
    printf("thread %d\n", thread);
  }
  return 0;
}
*/
/*
//ediotion 4
 
#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel
  {
  	int n;
  	printf("thread %d of %d\n", omp_get_thread_num(), omp_get_max_threads());
  	#pragma omp for//只有这段的循环被分配了，前面的语句仍然是并行的 
  	for(n = 0; n < 4; n++) {
  	  int thread = omp_get_thread_num();
	  printf("thread %d\n", thread);	
	}
  }
  return 0;
} 
*/
