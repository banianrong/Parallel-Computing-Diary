#include<stdio.h>
#include<omp.h>

int main() {
  int i = 10;
  #pragma omp parallel for private(i)
  for(int j = 0; j < 4; j++) {
  	printf("Thread %d: i = %d\n", omp_get_thread_num(), i);//����i��private����ÿ���߳���û�г�ʼ����������������� 
  }
  printf("i = %d\n", i);
  return 0;
}
