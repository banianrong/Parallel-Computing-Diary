#include"mpi.h"
#include<stdio.h>

int main() {
  int myid;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("processor %d running\n", myid); fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("hello world %d\n", myid); fflush(stdout);
  MPI_Finalize();
  return 0;
}