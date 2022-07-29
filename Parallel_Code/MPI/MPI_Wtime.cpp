#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main(int argc, char* argv[]) {
  int myid, numprocs, next, namelen;
  char buffer[BUFLEN], processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  double t1, t2, t3, tick;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Get_processor_name(processor_name, &namelen);

  t1 = MPI_Wtime();

  printf("Processor %d on %s\n", myid, processor_name);
  printf("Processor %d of %d\n", myid, numprocs);
  memset(buffer, 0, BUFLEN*sizeof(char));
  if(myid == numprocs-1)
    next = 0;
  else
    next = myid + 1;

  if(myid == 0) {
    strcpy(buffer, "hello there");
    printf("%d sending '%s'\n", myid, buffer); fflush(stdout);
    MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);
    printf("%d receiving\n", myid); fflush(stdout);
    MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
    printf("%d received '%s'\n", myid, buffer); fflush(stdout);
  }else{
    printf("%d receiving\n", myid); fflush(stdout);
    MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
    printf("%d received '%s'\n", myid, buffer); fflush(stdout);
    MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);
    printf("%d sent '%s'\n", myid, buffer); fflush(stdout);
  }
  
  t2 = MPI_Wtime();
  t3 = t2 - t1;
  tick = MPI_Wtick();
  printf("%d process time is '%.10f'\n", myid, t3);
  printf("%d process tick is '%.10f'\n", myid, tick);
  MPI_Finalize();
  return 0;
}