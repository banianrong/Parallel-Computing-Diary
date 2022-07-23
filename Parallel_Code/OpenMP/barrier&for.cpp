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
    #pragma omp for//ע��for�Դ�barrier�����ԣ�ֻ�����е��̶߳�ִ�������Լ������񣬲��ܼ������£���Ȼ�����#pragma omp for nowait��ô�ͻ��˱��û��barrier������������ȷ���� 
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
//ʹ��barrier��ǰ���Ѿ��������ˣ���֤���е��߳����е�barrier���ڵĽڵ�Ż�����������ȵ����̱߳��ֵȴ�״̬ 
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
