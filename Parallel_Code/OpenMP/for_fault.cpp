#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel for
  for(int i = 0; i != 8; i++) { //error:����ʹ��!=�����ж� 
    printf("Hello World\n");
    break;//ֻ�ܵ���ڣ������ڣ���Ȼ����� 
  }
  return 0;
}
