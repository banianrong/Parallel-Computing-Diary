#include<stdio.h>
#include<omp.h>

int main() {
  int a, b, c;
  a = b = c = 1;
/*
  #pragma omp parallel default(none) shared(b)
  {
  	b += a;//error aû��ָ������������ 
  }
*/
  #pragma omp parallel default(shared) shared(a) private(b)
  {
  	b = 10;//b��private�����b����ÿ���߳���˵�����µģ�Ҫ��ʼ�� 
  	b += a;
  }
  return 0;
}
