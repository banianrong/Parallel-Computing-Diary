#include<stdio.h>
#include<omp.h>

int main() {
//  #pragma omp parallel for schedule(static) num_threads(4)//������Ӳ�������ôsizeĬ�ϵľ��ǵ����������߳������������Ƿ����ÿ���̵߳�������һ���� 
//  #pragma omp parallel for schedule(static, 5) num_threads(4)
//  #pragma omp parallel for schedule(dynamic) num_threads(4)//������Ӳ�������ôsizeĬ��Ϊ1�������ǽ�size�����������̣߳����Ŀ�Ŀ�������ȡ����˴����������������һ��һ�� 
//  #pragma omp parallel for schedule(dynamic, 5) num_threads(4)
//  #pragma omp parallel for schedule(guided) num_threads(4)//��dynamic���ƣ����������������������𽥵ݼ������ǲ���С��size��sizeĬ��Ϊ1 
//  #pragma omp parallel for schedule(guided, 5) num_threads(4)
  for(int i = 0; i < 100; i++) {
  	printf("thread %d: i = %d\n", omp_get_thread_num(), i);
  } 
  return 0;
}
