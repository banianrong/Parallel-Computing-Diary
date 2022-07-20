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
#include<omp.h>//如果在编译器的时候加入-fopenmp，那么这边没有作用 

int main() {
  #pragma omp parallel
  {
  	printf("Hello World\n");
  }
  return 0;
}
