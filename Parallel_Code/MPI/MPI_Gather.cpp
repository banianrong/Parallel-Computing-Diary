#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid, numprocs, namelen;
  char name[MPI_MAX_PROCESSOR_NAME], buf[BUFLEN], BUF[BUFLEN];
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(name, &namelen);
    printf("%d of %d running on %s\n", myid, numprocs, name); fflush(stdout);
    sprintf(buf, "hello, I\'m processor %d.", myid);
    printf("%s\n", buf); fflush(stdout);
    int len = strlen(buf);
    MPI_Gather(buf, len, MPI_CHAR, BUF, len, MPI_CHAR, 1, MPI_COMM_WORLD);
    //MPI_Barrier(MPI_COMM_WORLD);
    printf("processor %d\'BUF is %s\n", myid, BUF); fflush(stdout);
  MPI_Finalize(); 
  return 0;
}