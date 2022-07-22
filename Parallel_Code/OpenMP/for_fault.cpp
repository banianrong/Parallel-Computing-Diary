#include<stdio.h>
#include<omp.h>

int main() {
  #pragma omp parallel for
  for(int i = 0; i != 8; i++) { //error:不能使用!=参与判断 
    printf("Hello World\n");
    break;//只能单入口，单出口，不然会错误。 
  }
  return 0;
}
