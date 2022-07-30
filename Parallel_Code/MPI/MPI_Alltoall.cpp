#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid, numprocs, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME], buf[BUFLEN], BUF[BUFLEN];
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processor_name, &namelen);
    printf("%d of %d running on %s\n", myid, numprocs, processor_name); fflush(stdout);
    sprintf(buf, "I\'m processor %d, hello!", myid);
    printf("processor %d : %s\n", myid, buf); fflush(stdout);
    memset(BUF, 0, sizeof(BUF));
    int len = strlen(buf);
    MPI_Alltoall(buf, len/numprocs, MPI_CHAR, BUF, len/numprocs, MPI_CHAR, MPI_COMM_WORLD);
    printf("processor %d get message: %s\n", myid, BUF);
  MPI_Finalize();
  return 0;
}