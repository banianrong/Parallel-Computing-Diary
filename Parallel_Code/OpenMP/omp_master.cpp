#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<omp.h>

int main() {
  srand(time(NULL));
  int total = 0;
  int histogram[20];
  memset(histogram, 0, sizeof(histogram));
  #pragma omp parallel
  {
  	#pragma omp for
  	for(int i = 0; i < 1000; i++) {
  	  int value = rand()%20;
	  #pragma omp atomic
	  histogram[value]++;	
	}
    #pragma omp master
    {
      for(int i = 0; i < 20; i++) {
      	total += histogram[i];
	  }
	}
  }
  printf("%d\n", total);
  return 0;
}
