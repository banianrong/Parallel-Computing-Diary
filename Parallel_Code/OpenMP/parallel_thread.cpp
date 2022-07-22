#include<stdio.h>
#include<omp.h>

int main() {
  omp_set_nested(1);
  #pragma omp parallel for
  for(int i = 0; i < 2; i++) {
  	int outer_thread = omp_get_thread_num();
  	
  	#pragma omp parallel for
  	for(int j = 0; j < 4; j++) {
  	  int inner_thread = omp_get_thread_num();
	  printf("Hello World (i = %d j = %d)\n", outer_thread, inner_thread);	
	}
  }
  return 0;
}
