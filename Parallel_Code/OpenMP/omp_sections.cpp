#include<stdio.h>
#include<omp.h> 

int main() {
  #pragma omp parallel
  {
  	#pragma omp sections //nowait//����֤����sections�Դ���ʽդ�� 
  	{
  	  #pragma omp section
	  {
	    int thread = omp_get_thread_num();
		printf("Thread %d : tastk 1\n", thread);	
	  }	
	  #pragma omp section
	  {
	  	int thread = omp_get_thread_num();
	  	printf("Thread %d : task 2\n", thread);
	  }
	  #pragma omp section
	  {
	  	int thread = omp_get_thread_num();
	  	printf("Thread %d : task 3\n", thread);
	  }
	}
	int thread = omp_get_thread_num();
	printf("Thread %d : task 4\n", thread);
  }
  return 0;
}
