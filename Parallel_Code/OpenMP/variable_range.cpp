#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<omp.h>

int main() {
  srand(time(NULL));
  int histogram[20];//shared
  memset(histogram, 0, sizeof(histogram));
  int total = 0;//shared
  
  int i, j;
  #pragma omp parallel for
  for(i = 0; i < 1000; i++) {//i private
  	int value = rand()%20;
  	#pragma omp atomic
  	histogram[value]++;
	for(j = 0; j < 1000; j++) {//j private
		
	} 
  }
  return 0;
} 
