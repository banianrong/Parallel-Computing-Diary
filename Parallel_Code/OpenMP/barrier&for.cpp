#include<string.h>
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main() {
  int total = 0;
  int histogram[20];
  memset(histogram, 0, sizeof(histogram));
  #pragma omp parallel num_threads(20)
  {
    #pragma omp for//注意for自带barrier的特性，只有所有的线程都执行完了自己的任务，才能继续往下，当然如果是#pragma omp for nowait那么就会退变成没有barrier的情况，结果不确定了 
	for(int i = 0; i < 1000; i++) {
	  int value = rand()%20;
	  #pragma omp atomic
	  histogram[value]++;
	}	
	int thread = omp_get_thread_num();
	#pragma omp atomic
	total += histogram[thread];
  } 
  printf("%d\n", total);
  return 0;
}

/*
//使用barrier，前面已经讲述过了，保证所有的线程运行到barrier所在的节点才会继续，否则先到的线程保持等待状态 
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
	#pragma omp barrier 
	int thread = omp_get_thread_num();
	#pragma omp atomic
	total += histogram[thread];
  }
  printf("%d\n", total);
  return 0;
} 
*/
