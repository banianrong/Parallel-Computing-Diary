#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

int main() {
  int total = 0;
  int histogram[20];
  memset(histogram, 0, sizeof(histogram));
  #pragma omp parallel num_threads(20)
  {
  	for(int i = 0; i < 50; i++) {
  	  int value = rand()%20;
  	  #pragma omp atomic
  	  histogram[value]++;
	}
	#pragma omp barrier//如果没有这个指令，那么total一般不是1000 
	int thread = omp_get_thread_num();
	#pragma omp atomic
	total += histogram[thread]; //注意这边的统计方式，他是按照线程的编号来实现，如果此时其他线程没有完全执行完毕，那么就很容易造成数据丢失 
  }
  printf("%d\n", total);
  return 0;
}
