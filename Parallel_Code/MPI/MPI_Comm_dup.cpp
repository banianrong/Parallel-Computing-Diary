#include"mpi.h"
#include<stdio.h>

int main() {
  int myid, numprocs, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Comm a;
  MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);
    printf("processor %d of %d running on %s\n", myid, numprocs, processor_name);
    MPI_Comm_dup(MPI_COMM_SELF, &a);
    int result = -1;
    MPI_Comm_compare(MPI_COMM_WORLD, a, &result);
    printf("%d\n", result);
    MPI_Comm_dup(MPI_COMM_WORLD, &a);
    MPI_Comm_compare(MPI_COMM_WORLD, a, &result);
    printf("%d\n", result);
  MPI_Finalize();
  return 0;
}