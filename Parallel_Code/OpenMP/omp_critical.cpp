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
  	#pragma omp critical//���û����䣬��ô���յĽ��δ����1000����Ϊ++���е��������� 
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
