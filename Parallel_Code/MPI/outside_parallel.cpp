#include"mpi.h"
#include<stdio.h>

int main() {
  MPI_Init(NULL, NULL);
  MPI_Finalize();
  printf("Hello World\n");
  return 0;
}