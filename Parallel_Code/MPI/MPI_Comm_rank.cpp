#include<stdio.h>
#include"mpi.h"

int main() {
  int r1, r2;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &r1);
    MPI_Comm_rank(MPI_COMM_SELF, &r2);
    printf("%d %d\n", r1, r2);
  MPI_Finalize();
  return 0;
}