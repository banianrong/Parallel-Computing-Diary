#include<iostream>
#include<omp.h>
using namespace std;

int main() {
  omp_set_num_threads(16);//��������ȼ�����num_threads(int) 
  #pragma omp parallel num_threads(8)//num_threads(int)�����ȼ��ϸߣ������Ϊ׼ 
  {
  	int thread = omp_get_thread_num();
  	printf("Hello World(%d)\n", thread);
  }
  return 0;
}
