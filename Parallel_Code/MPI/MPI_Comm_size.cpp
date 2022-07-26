#include<stdio.h>
#include"mpi.h"

int main() {
  int r1, r2, s1, s2;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &r1);
    MPI_Comm_rank(MPI_COMM_SELF, &r2);
    MPI_Comm_size(MPI_COMM_WORLD, &s1);
    MPI_Comm_size(MPI_COMM_SELF, &s2);
    printf("world %d of %d, self %d of %d\n", r1, s1, r2, s2);
  MPI_Finalize();
  return 0;
}