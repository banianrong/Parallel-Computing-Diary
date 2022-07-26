#include<stdio.h>
#include "mpi.h"

int main() {
  int rank;
  int size;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Hello World from process %d of %d\n", rank, size);
  MPI_Finalize();
  return 0;
}