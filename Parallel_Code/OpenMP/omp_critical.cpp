#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

int main() {
  srand(time(NULL));
  int histogram[20];
  memset(histogram, 0, sizeof(histogram));
  #pragma omp parallel for
  for(int i = 0; i < 1000; ++i) {
  	int value = rand()%20;
  	#pragma omp critical//如果没有这句，那么最终的结果未必是1000，因为++具有迭代依赖性 
  	{
  	  histogram[value]++;	
	}
  }
  
  int total = 0;
  for(int i = 0; i < 20; i++) {
  	total += histogram[i];
  	printf("%d\n", histogram[i]);
  }
  printf("total:%d\n", total);
  return 0;
}
