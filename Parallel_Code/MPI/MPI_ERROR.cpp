#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid;
  MPI_Status status;
  char buf[BUFLEN];
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("processor %d running\n", myid);
    if(myid == 0) {
      strcpy(buf, "hello, processor 1 from processor 0");
      printf("processor %d sending %s\n", myid, buf); fflush(stdout);
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 1, 101, MPI_COMM_WORLD);
    }
    if(myid == 1) {
      MPI_Recv(buf, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      printf("processor %d received %s\n", myid, buf); fflush(stdout);
      printf("tag %d source %d\n", status.MPI_TAG, status.MPI_SOURCE);
      printf("error code %d\n", status.MPI_ERROR);
    }
  MPI_Finalize();
  return 0;
}