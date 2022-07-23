#include<stdio.h>
#include<omp.h>

int main() {
  int a, b, c;
  a = b = c = 1;
/*
  #pragma omp parallel default(none) shared(b)
  {
  	b += a;//error a没有指明变量作用域 
  }
*/
  #pragma omp parallel default(shared) shared(a) private(b)
  {
  	b = 10;//b是private，因此b对于每个线程来说都是新的，要初始化 
  	b += a;
  }
  return 0;
}
