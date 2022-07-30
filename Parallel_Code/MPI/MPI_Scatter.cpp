#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid, numprocs, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME], buf[BUFLEN], BUF[BUFLEN]; 
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);
    printf("%d of %d running on %s\n", myid, numprocs, processor_name);
    memset(buf, 0, sizeof(buf));
    if(myid == 0) strcpy(buf, "hello, I\'m processor 0");
    printf("processor %d buf %s\n", myid, buf); fflush(stdout);
    int len = strlen(buf), next = (myid + 1) % numprocs;
    MPI_Barrier(MPI_COMM_WORLD);
    if(myid == 0) {
      MPI_Send(&len, 1, MPI_INT, next, 101,  MPI_COMM_WORLD);
      MPI_Recv(&len, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }else{
      MPI_Recv(&len, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Send(&len, 1, MPI_INT, next, 101,  MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("processor %d len %d\n", myid, len); fflush(stdout);
    MPI_Scatter(buf, len/4, MPI_CHAR, BUF, len/4, MPI_CHAR, 0, MPI_COMM_WORLD);
    printf("processor %d BUF %s\n", myid, BUF); fflush(stdout);
  MPI_Finalize();
  return 0;
}