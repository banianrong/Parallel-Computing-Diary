/*
//serial version
#include<stdio.h>

int main() {
  printf("Hello World\n"); 
  return 0;
}
*/

//parallel version
#include<cstdio>
#include<omp.h>//����ڱ�������ʱ�����-fopenmp����ô���û������ 

int main() {
  #pragma omp parallel
  {
  	printf("Hello World\n");
  }
  return 0;
}
