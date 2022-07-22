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
	#pragma omp barrier//���û�����ָ���ôtotalһ�㲻��1000 
	int thread = omp_get_thread_num();
	#pragma omp atomic
	total += histogram[thread]; //ע����ߵ�ͳ�Ʒ�ʽ�����ǰ����̵߳ı����ʵ�֣������ʱ�����߳�û����ȫִ����ϣ���ô�ͺ�����������ݶ�ʧ 
  }
  printf("%d\n", total);
  return 0;
}
