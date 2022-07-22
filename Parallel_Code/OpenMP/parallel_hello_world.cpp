#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel
  {
    int thread = omp_get_thread_num();
	printf("hello(%d)", thread);
	printf("world(%d)", thread);	
  } 
  return 0;
}
