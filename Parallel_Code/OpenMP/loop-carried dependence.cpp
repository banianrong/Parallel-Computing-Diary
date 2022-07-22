#include<stdio.h>
#include<omp.h>

int main() {
  int a[10], b[10];
  for(int i = 0; i < 10; i++) a[i] = b[i] = i;
  #pragma omp parallel for
  for(int i = 1; i < 10; i++) a[i] = a[i-1]+2;
  for(int i = 0; i < 10; i++) printf("%d ", a[i]);
  return 0;
} 
