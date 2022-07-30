#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid, numprocs, namelen;
  char buf[BUFLEN], BUF[BUFLEN], name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(name, &namelen);
    printf("processor %d of %d running on %s\n", myid, numprocs, name);
    memset(buf, 0, sizeof(buf));
    memset(BUF, 0, sizeof(BUF));
    sprintf(buf, "hello, I'm processor %d", myid);
    MPI_Allgather(buf, strlen(buf), MPI_CHAR, BUF, strlen(buf), MPI_CHAR, MPI_COMM_WORLD);
    printf("processor %d get message : %s\n", myid, BUF); fflush(stdout);
  MPI_Finalize();
  return 0;
}