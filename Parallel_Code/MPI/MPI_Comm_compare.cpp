#include"mpi.h"
#include<stdio.h>

int main() {
  MPI_Init(NULL, NULL);
    int result = 0;
    MPI_Comm_compare(MPI_COMM_WORLD, MPI_COMM_WORLD, &result);
    printf("%d\n", result);
  MPI_Finalize();
  return 0;
}