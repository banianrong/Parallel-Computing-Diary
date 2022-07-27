#include"mpi.h"
#include<stdio.h>

int main() {
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int namelen, myid, numprocs;  
  MPI_Init(NULL, NULL);
    MPI_Get_processor_name(processor_name, &namelen);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("%d of %d\n", myid, numprocs);
    printf("%d is %s %d\n", myid, processor_name, namelen);
  MPI_Finalize();
  return 0;
}