#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid, next, numprocs;
  char buf[BUFLEN];
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    printf("processor %d running\n", myid); fflush(stdout);
    if(myid == numprocs-1) {
      MPI_Abort(MPI_COMM_SELF, 101);
    }
    printf("processor %d exit\n", myid); fflush(stdout);
    if(myid == 1) {
      strcpy(buf, "hello world");
      printf("%d sending %s\n", myid, buf); fflush(stdout);
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 0, 101, MPI_COMM_WORLD);
    }
    if(myid == 0) {
      printf("%d receiving\n", myid); fflush(stdout);
      MPI_Recv(buf, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      printf("%d received %s\n", myid, buf); fflush(stdout);
    }
    strcpy(buf, "hello cse");
    next = (myid + 1) % (numprocs - 1);
    printf("processor %d sending %s\n", myid, buf); fflush(stdout);
    MPI_Send(buf, strlen(buf)+1, MPI_CHAR, next, 110, MPI_COMM_WORLD);
    printf("processor %d receiving\n", myid); fflush(stdout);
    MPI_Recv(buf, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 110, MPI_COMM_WORLD, &status);
    printf("processor %d received %s\n", myid, buf); fflush(stdout);
  MPI_Finalize();
  return 0;
}